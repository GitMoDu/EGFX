#ifndef _EGFX_ESP32_SPI_H
#define _EGFX_ESP32_SPI_H

#if defined(ARDUINO_ARCH_ESP32)

#include <Arduino.h>
#include <SPI.h>
#include <driver/spi_master.h>

namespace Egfx
{
	/// <summary>
	/// ESP32-specific SPI helper that extends Arduino `SPIClass` with an optional ESP-IDF `spi_master` DMA path.
	/// 
	/// Design goals:
	/// - Keep compatibility with existing EGFX inline SPI drivers (they still see a `SPIClass`-like interface).
	/// - Provide a non-blocking (poll-driven) DMA transfer mechanism for large framebuffer pushes.
	/// - Store pin/host configuration so presets or templates can configure the bus once and reuse it.
	/// 
	/// Notes:
	/// - The DMA path uses ESP-IDF `spi_device_queue_trans()` and requires polling via `DmaBusy()`.
	/// - This class does not attempt to synchronize Arduino transactions with IDF transactions. In EGFX,
	///   commands typically use the inline driver (`SPIClass`), while framebuffer bulk data may use DMA.
	/// </summary>
	class Esp32Spi : public SPIClass
	{
	private:
		// Arduino SPI bus selector used by SPIClass. Typically VSPI/HSPI on classic ESP32.
		uint8_t ArduinoBusId = VSPI;

		// Tracks whether the Arduino SPI bus has been started through BeginConfigured().
		bool Started = false;

		// Configured SPI pins (optional). If not set, SPIClass defaults are used.
		int8_t Sck = -1;
		int8_t Mosi = -1;
		int8_t Miso = -1;
		int8_t Ss = -1;

		// IDF spi_master host selection and per-transaction size limit.
		spi_host_device_t Host = SPI_HOST_MAX;
		size_t MaxTransferBytes = 4092;

		// Cached DMA configuration.
		bool DmaReady = false;
		uint32_t DmaClockHz = 40000000;
		uint8_t DmaMode = 0;
		bool DmaUseHwCs = false;

		// IDF device handle used for queued DMA transactions.
		spi_device_handle_t Device = nullptr;

		// Async transfer state tracking.
		bool AsyncInFlight = false;

		// Two transaction slots; the driver can have up to 2 queued/in-flight transactions.
		spi_transaction_t TransA{};
		spi_transaction_t TransB{};
		bool TransAInUse = false;
		bool TransBInUse = false;

		// Pointer to the caller-provided buffer. EGFX guarantees it remains valid until push ends.
		const uint8_t* Pending = nullptr;
		size_t PendingOffset = 0;
		size_t PendingRemaining = 0;

	public:
		/// <param name="arduinoBusId">Arduino bus id (typically `VSPI` or `HSPI`).</param>
		explicit Esp32Spi(uint8_t arduinoBusId = VSPI)
			: SPIClass(arduinoBusId)
			, ArduinoBusId(arduinoBusId)
		{
		}

	public:
		/// <summary>
		/// Defines the pins used by this SPI instance. Call before `BeginConfigured()` and before `DmaBegin()`.
		/// </summary>
		void ConfigurePins(int8_t sck, int8_t miso, int8_t mosi, int8_t ss = -1)
		{
			Sck = sck;
			Miso = miso;
			Mosi = mosi;
			Ss = ss;
		}

		/// <summary>
		/// Defines the ESP-IDF spi_master host. Optional; if not set, a default is derived from `ArduinoBusId`.
		/// </summary>
		void ConfigureHost(spi_host_device_t host)
		{
			Host = host;
		}

		/// <summary>
		/// Sets the maximum bytes per queued DMA transaction.
		/// Larger values reduce queue overhead, but must be supported by the IDF bus configuration.
		/// </summary>
		void ConfigureMaxTransferBytes(size_t maxBytes)
		{
			MaxTransferBytes = maxBytes;
		}

		/// <summary>
		/// Starts Arduino SPIClass using configured pins (if provided).
		/// This should be called before inline driver initialization sequences that use `SPIClass`.
		/// </summary>
		void BeginConfigured()
		{
			if (Started)
				return;

			// Require SCK + MOSI for custom pin mapping.
			if (Sck >= 0 && Mosi >= 0)
			{
				SPIClass::begin(Sck, Miso, Mosi, Ss);
			}
			else
			{
				SPIClass::begin();
			}

			Started = true;
		}

		/// <summary>
		/// Stops Arduino SPIClass and clears DMA state.
		/// (Matches `SPIClass::end()` signature; not marked override because SPIClass::end is not virtual in all cores.)
		/// </summary>
		void end()
		{
			DmaEnd();
			Started = false;
			SPIClass::end();
		}

	public:
		/// <summary>
		/// Initializes ESP-IDF spi_master for queued DMA transfers.
		/// Must be called before `DmaWriteAsync()`.
		/// </summary>
		/// <param name="clockHz">SPI clock frequency in Hz.</param>
		/// <param name="mode">SPI mode (0-3).</param>
		/// <param name="useHwCs">
		/// If true and `Ss` is valid, IDF hardware CS may be used; otherwise CS is expected to be handled by the caller.
		/// </param>
		/// <returns>True if initialized (or already initialized), otherwise false.</returns>
		bool DmaBegin(uint32_t clockHz, uint8_t mode, bool useHwCs = false)
		{
			// If already initialized, just update cached parameters and return.
			// (Note: this does not reconfigure the underlying IDF device once created.)
			if (DmaReady)
			{
				DmaClockHz = clockHz;
				DmaMode = mode;
				DmaUseHwCs = useHwCs;
				return true;
			}

			// DMA requires at minimum SCK + MOSI.
			if (Sck < 0 || Mosi < 0)
				return false;

#if CONFIG_IDF_TARGET_ESP32
			// If host not set explicitly, derive from Arduino bus id.
			if (Host == SPI_HOST_MAX)
				Host = (ArduinoBusId == VSPI) ? VSPI_HOST : HSPI_HOST;
#else
			// Non-classic ESP32 targets use different host naming.
			if (Host == SPI_HOST_MAX)
				Host = SPI2_HOST;
#endif

			DmaClockHz = clockHz;
			DmaMode = mode;
			DmaUseHwCs = useHwCs;

			// Configure bus for DMA.
			spi_bus_config_t buscfg{};
			buscfg.sclk_io_num = Sck;
			buscfg.mosi_io_num = Mosi;
			buscfg.miso_io_num = Miso;
			buscfg.quadwp_io_num = -1;
			buscfg.quadhd_io_num = -1;
			buscfg.max_transfer_sz = (int)MaxTransferBytes;

			// Initialize bus. If already initialized elsewhere, ESP_ERR_INVALID_STATE can be tolerated.
			const esp_err_t busOk = spi_bus_initialize(Host, &buscfg, SPI_DMA_CH_AUTO);
			if (busOk != ESP_OK && busOk != ESP_ERR_INVALID_STATE)
				return false;

			// Attach a device to the bus for queued transfers.
			spi_device_interface_config_t devcfg{};
			devcfg.clock_speed_hz = (int)DmaClockHz;
			devcfg.mode = DmaMode;
			devcfg.queue_size = 2;
			devcfg.flags = SPI_DEVICE_NO_DUMMY;

			// Hardware CS if requested; otherwise caller toggles CS (common for EGFX drivers).
			devcfg.spics_io_num = (DmaUseHwCs && Ss >= 0) ? Ss : -1;

			// Slight CS lead/lag can help some displays.
			devcfg.cs_ena_pretrans = 1;
			devcfg.cs_ena_posttrans = 1;

			const esp_err_t devOk = spi_bus_add_device(Host, &devcfg, &Device);
			if (devOk != ESP_OK)
				return false;

			DmaReady = true;
			return true;
		}

		/// <summary>
		/// Stops the IDF DMA device and clears async state.
		/// </summary>
		void DmaEnd()
		{
			AsyncInFlight = false;
			TransAInUse = false;
			TransBInUse = false;

			if (Device != nullptr)
			{
				// Drain any pending results (non-blocking).
				spi_transaction_t* done = nullptr;
				while (spi_device_get_trans_result(Device, &done, 0) == ESP_OK && done != nullptr) {}

				spi_bus_remove_device(Device);
				Device = nullptr;
			}

			DmaReady = false;
		}

		/// <summary>
		/// Begins an async DMA write. This queues the first chunk and returns immediately.
		/// Completion/progress is driven by calling `DmaBusy()` repeatedly.
		/// </summary>
		/// <param name="data">Pointer to the source buffer (must remain valid until NOT busy).</param>
		/// <param name="lenBytes">Total bytes to transmit.</param>
		/// <returns>True if queued successfully, false otherwise.</returns>
		bool DmaWriteAsync(const void* data, size_t lenBytes)
		{
			if (!DmaReady || Device == nullptr || data == nullptr || lenBytes == 0)
				return false;
			if (AsyncInFlight)
				return false;

			Pending = static_cast<const uint8_t*>(data);
			PendingOffset = 0;
			PendingRemaining = lenBytes;

			// Clear transaction structs and in-use flags for a new sequence.
			memset(&TransA, 0, sizeof(TransA));
			memset(&TransB, 0, sizeof(TransB));
			TransAInUse = false;
			TransBInUse = false;

			AsyncInFlight = true;

			// Queue the first transaction (kick-off).
			QueueOne();
			return true;
		}

		/// <summary>
		/// Polls the async DMA transfer state.
		/// Returns true while there is still outstanding work (queued/in-flight).
		/// </summary>
		bool DmaBusy()
		{
			if (!AsyncInFlight)
				return false;

			// Attempt to reap at most one finished transaction (non-blocking).
			ReapCompleted();

			// If we have no data left and nothing in flight, we're done.
			if (PendingRemaining == 0 && !TransAInUse && !TransBInUse)
			{
				AsyncInFlight = false;
				return false;
			}

			// Keep feeding one chunk per poll (bounded work model).
			QueueOne();
			return true;
		}

	private:
		/// <summary>
		/// Reaps at most one completed transaction from the IDF queue (non-blocking).
		/// This function is intentionally non-greedy: caller is expected to poll frequently.
		/// </summary>
		void ReapCompleted()
		{
			spi_transaction_t* done = nullptr;
			const esp_err_t r = spi_device_get_trans_result(Device, &done, 0);

			if (r == ESP_OK && done != nullptr)
			{
				if (done == &TransA) TransAInUse = false;
				if (done == &TransB) TransBInUse = false;
			}
		}

		/// <summary>
		/// Queues a single chunk if a transaction slot is available.
		/// Chunk size is limited by `MaxTransferBytes`.
		/// </summary>
		void QueueOne()
		{
			if (PendingRemaining == 0)
				return;

			spi_transaction_t* t = nullptr;

			if (!TransAInUse) t = &TransA;
			else if (!TransBInUse) t = &TransB;
			else return;

			const size_t chunk = (PendingRemaining > MaxTransferBytes)
				? MaxTransferBytes
				: PendingRemaining;

			// Reset the transaction fields for this queued chunk.
			memset(t, 0, sizeof(*t));
			t->tx_buffer = Pending + PendingOffset;
			t->length = chunk * 8;

			const esp_err_t q = spi_device_queue_trans(Device, t, 0);
			if (q != ESP_OK)
			{
				// Abort async sequence on queue failure.
				AsyncInFlight = false;
				PendingRemaining = 0;
				TransAInUse = false;
				TransBInUse = false;
				return;
			}

			if (t == &TransA) TransAInUse = true;
			if (t == &TransB) TransBInUse = true;

			PendingOffset += chunk;
			PendingRemaining -= chunk;
		}
	};
}

#endif
#endif