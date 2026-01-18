#ifndef _TEMPLATE_SCREEN_DRIVER_SPI_DMA_h
#define _TEMPLATE_SCREEN_DRIVER_SPI_DMA_h

#if defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_ESP32)
#define TEMPLATE_SCREEN_DRIVER_SPI_DMA

#include <stdint.h>

#if defined(ARDUINO_ARCH_ESP32)
#include "Esp32Spi.h"
#endif

namespace Egfx
{
	template<typename InlineSpiScreenDriver,
		uint32_t pushSleepDuration,
		size_t pushChunckMaxSize = UINT8_MAX>
	class TemplateScreenDriverSpiDma : public InlineSpiScreenDriver
	{
	public:
		using InlineSpiScreenDriver::BufferSize;

	protected:
		using InlineSpiScreenDriver::SpiInstance;

	private:
		volatile bool _pushInProgress = false;

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
		{
		}

		bool IsPushInProgress() const { return _pushInProgress; }

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
			SpiInstance.transferAsync((const void*)frameBuffer, (void*)nullptr, BufferSize);
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
			const bool busy = !SpiInstance.finishedAsync();
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
	};
}
#endif
#endif