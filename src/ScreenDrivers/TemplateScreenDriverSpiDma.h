#ifndef _TEMPLATE_SCREEN_DRIVER_SPI_DMA_h
#define _TEMPLATE_SCREEN_DRIVER_SPI_DMA_h

#if defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_ESP32)
#define TEMPLATE_SCREEN_DRIVER_SPI_DMA

#include <stdint.h>

#if defined(ARDUINO_ARCH_ESP32)
#include "Esp32Spi.h"
#endif
#if defined(ARDUINO_ARCH_RP2040)
#include <hardware/dma.h>
#include <hardware/regs/addressmap.h>
#include <hardware/spi.h>
#ifndef __SPI0_DEVICE
#define __SPI0_DEVICE spi0
#endif
#ifndef __SPI1_DEVICE
#define __SPI1_DEVICE spi1
#endif
#endif

namespace Egfx
{
	template<typename InlineSpiScreenDriver,
		uint32_t pushSleepDuration,
		size_t pushChunckMaxSize = UINT8_MAX
	>
	class TemplateScreenDriverSpiDma : public InlineSpiScreenDriver
	{
	public:
		using InlineSpiScreenDriver::BufferSize;

	protected:
		using InlineSpiScreenDriver::SpiInstance;

	private:
		volatile bool _pushInProgress = false;

#if defined(ARDUINO_ARCH_RP2040)
	private:
		int DmaTxChannel = -1;
		int DmaRxChannel = -1;
		uint32_t DmaRxSink = 0;
		const uint8_t* DmaFrameBuffer = nullptr;
#endif

#if defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F4)
	private: // Some DMA controllers are limited in the transaction size, async style implementation to work around.
		static constexpr size_t CHUNK_SIZE = BufferSize / pushChunckMaxSize;
		static constexpr size_t WHOLE_SIZE = (uint32_t(BufferSize) / CHUNK_SIZE) * CHUNK_SIZE;
		static constexpr size_t REMAINDER_SIZE = BufferSize - WHOLE_SIZE;
		static constexpr size_t REMAINDER_START = WHOLE_SIZE;

		size_t PushIndex = 0;
#endif

	public:
		TemplateScreenDriverSpiDma(Egfx::SpiType& spi)
			: InlineSpiScreenDriver(spi)
		{}

		bool IsPushInProgress() const { return _pushInProgress; }

		bool CanPushBuffer() final
		{
			return !_pushInProgress && InlineSpiScreenDriver::CanPushBuffer();
		}

		bool Start() override
		{
#if defined(ARDUINO_ARCH_ESP32)
			// Ensure SPI pins are applied before the inline driver's init sequence runs.
			// Config is expected to have called ConfigurePins()/ConfigureHost(), but this is safe.
			SpiInstance.BeginConfigured();
#endif
			return InlineSpiScreenDriver::Start();
		}

		uint32_t PushBuffer(const uint8_t* frameBuffer) final
		{
			if (_pushInProgress)
			{
				return pushSleepDuration;
			}

			_pushInProgress = true;

#if defined(ARDUINO_ARCH_STM32F1)
			SpiInstance.dmaSendAsync((void*)frameBuffer, (size_t)BufferSize, true);
			return pushSleepDuration;
#elif defined(ARDUINO_ARCH_STM32F4)
			PushIndex = 0;
			if (WHOLE_SIZE > 0)
			{
				SpiInstance.dmaSend((void*)frameBuffer, (uint16_t)CHUNK_SIZE, true);
				PushIndex += CHUNK_SIZE;
			}
			// will continue in PushingBuffer()
			return 0;
#elif defined(ARDUINO_ARCH_RP2040)
			DmaFrameBuffer = frameBuffer;
			if (!StartRp2040DmaTransfer())
			{
				DmaFrameBuffer = nullptr;
				_pushInProgress = false;
				return 0;
			}
			return pushSleepDuration;
#elif defined(ARDUINO_ARCH_ESP32)
			// Make sure the IDF DMA backend is ready. Mode is assumed SPI_MODE0 for display drivers.
			// Clock defaults to Esp32Spi's internal value unless configured by caller.
			// No HW-CS: inline driver controls CS via GPIO.
			if (!SpiInstance.DmaBegin(80000000, SPI_MODE0, false))
			{
				_pushInProgress = false;
				return 0;
			}

			if (!SpiInstance.DmaWriteAsync(frameBuffer, BufferSize))
			{
				_pushInProgress = false;
				return 0;
			}

			return 0;
#else
			_pushInProgress = false;
			return 0;
#endif
		}

		bool PushingBuffer(const uint8_t* frameBuffer) final
		{
			if (!_pushInProgress)
			{
				return false;
			}

#if defined(ARDUINO_ARCH_STM32F1)
			const bool busy = (!spi_is_tx_empty(SpiInstance.dev()) || spi_is_busy(SpiInstance.dev()));
			if (!busy) _pushInProgress = false;
			return busy;
#elif defined(ARDUINO_ARCH_STM32F4)
			if (SpiInstance.dmaSendReady())
			{
				if (PushIndex < WHOLE_SIZE)
				{
					SpiInstance.dmaSend((void*)&frameBuffer[PushIndex], (uint16_t)CHUNK_SIZE, true);
					PushIndex += CHUNK_SIZE;
					return true;
				}
				else
				{
					if (REMAINDER_SIZE > 0)
					{
						SpiInstance.dmaSend((void*)&frameBuffer[REMAINDER_START], REMAINDER_SIZE, true);
					}
					_pushInProgress = false;
					return false;
				}
			}
			return true;
#elif defined(ARDUINO_ARCH_RP2040)
			const bool busy = IsRp2040DmaTransferBusy();
			if (!busy) _pushInProgress = false;
			return busy;
#elif defined(ARDUINO_ARCH_ESP32)
			const bool busy = SpiInstance.DmaBusy();
			if (!busy) _pushInProgress = false;
			return busy;
#else
			_pushInProgress = false;
			return false;
#endif
		}

#if defined(ARDUINO_ARCH_RP2040)
	private:
		bool StartRp2040DmaTransfer()
		{
			spi_inst_t* dmaSpiInstance = GetRp2040DmaSpiInstance();

			if (dmaSpiInstance == nullptr)
			{
				return false;
			}

			if (!IsRp2040SramBuffer(DmaFrameBuffer, BufferSize))
			{
				return false;
			}

			if (DmaTxChannel >= 0 || DmaRxChannel >= 0)
			{
				return false;
			}

			DmaRxChannel = dma_claim_unused_channel(false);
			if (DmaRxChannel < 0)
			{
				return false;
			}

			DmaTxChannel = dma_claim_unused_channel(false);
			if (DmaTxChannel < 0)
			{
				dma_channel_unclaim(DmaRxChannel);
				DmaRxChannel = -1;
				return false;
			}

			hw_write_masked(&spi_get_hw(dmaSpiInstance)->cr0,
				(8 - 1) << SPI_SSPCR0_DSS_LSB,
				SPI_SSPCR0_DSS_BITS);

			dma_channel_config txConfig = dma_channel_get_default_config(DmaTxChannel);
			channel_config_set_transfer_data_size(&txConfig, DMA_SIZE_8);
			channel_config_set_read_increment(&txConfig, true);
			channel_config_set_write_increment(&txConfig, false);
			channel_config_set_dreq(&txConfig, spi_get_dreq(dmaSpiInstance, true));
			dma_channel_configure(DmaTxChannel, &txConfig, &spi_get_hw(dmaSpiInstance)->dr, DmaFrameBuffer, BufferSize, false);

			dma_channel_config rxConfig = dma_channel_get_default_config(DmaRxChannel);
			channel_config_set_transfer_data_size(&rxConfig, DMA_SIZE_8);
			channel_config_set_read_increment(&rxConfig, false);
			channel_config_set_write_increment(&rxConfig, false);
			channel_config_set_dreq(&rxConfig, spi_get_dreq(dmaSpiInstance, false));
			dma_channel_configure(DmaRxChannel, &rxConfig, &DmaRxSink, &spi_get_hw(dmaSpiInstance)->dr, BufferSize, false);

			spi_get_hw(dmaSpiInstance)->dmacr = 1 | (1 << 1);
			dma_channel_start(DmaRxChannel);
			dma_channel_start(DmaTxChannel);

			return true;
		}

		bool IsRp2040DmaTransferBusy()
		{
			spi_inst_t* dmaSpiInstance = GetRp2040DmaSpiInstance();

			if (dmaSpiInstance == nullptr)
			{
				return false;
			}

			if (DmaTxChannel < 0 || DmaRxChannel < 0)
			{
				return false;
			}

			if (dma_channel_is_busy(DmaTxChannel) || dma_channel_is_busy(DmaRxChannel) || spi_is_busy(dmaSpiInstance))
			{
				return true;
			}

			spi_get_hw(dmaSpiInstance)->dmacr = 0;
			dma_channel_cleanup(DmaRxChannel);
			dma_channel_unclaim(DmaRxChannel);
			dma_channel_cleanup(DmaTxChannel);
			dma_channel_unclaim(DmaTxChannel);
			DmaRxChannel = -1;
			DmaTxChannel = -1;
			DmaFrameBuffer = nullptr;

			return false;
		}

		spi_inst_t* GetRp2040DmaSpiInstance()
		{
			if (&SpiInstance == &SPI)
			{
				return __SPI0_DEVICE;
			}
			if (&SpiInstance == &SPI1)
			{
				return __SPI1_DEVICE;
			}

			return nullptr;
		}

		static bool IsRp2040SramBuffer(const void* buffer, const size_t size)
		{
			if (buffer == nullptr || size == 0)
			{
				return false;
			}

			const uintptr_t start = reinterpret_cast<uintptr_t>(buffer);
			const uintptr_t end = start + size - 1;

			return end >= start && start >= SRAM_BASE && end < SRAM_END;
		}
#endif
	};
}
#endif
#endif