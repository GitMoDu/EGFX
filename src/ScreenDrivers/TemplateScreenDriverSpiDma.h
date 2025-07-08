#ifndef _TEMPLATE_SCREEN_DRIVER_SPI_DMA_h
#define _TEMPLATE_SCREEN_DRIVER_SPI_DMA_h

#if defined(ARDUINO_ARCH_STM32F1) ||defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_RP2040)
#define TEMPLATE_SCREEN_DRIVER_SPI_DMA

#include <stdint.h>

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

#if defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F4)
	private: // Some DMA controllers are limited in the transaction size, async style implementation to work around.
		static constexpr size_t CHUNK_SIZE = BufferSize / pushChunckMaxSize;
		static constexpr size_t WHOLE_SIZE = (uint32_t(BufferSize) / CHUNK_SIZE) * CHUNK_SIZE;
		static constexpr size_t REMAINDER_SIZE = BufferSize - WHOLE_SIZE;
		static constexpr size_t REMAINDER_START = WHOLE_SIZE;

		size_t PushIndex = 0;
#endif

	public:
		TemplateScreenDriverSpiDma(Egfx::SpiType& spi) : InlineSpiScreenDriver(spi) {}

		uint32_t PushBuffer(const uint8_t* frameBuffer) final
		{
#if defined(ARDUINO_ARCH_STM32F1)
			SpiInstance.dmaSendAsync((void*)frameBuffer, (size_t)BufferSize, true);
#elif defined(ARDUINO_ARCH_STM32F4)
			PushIndex = 0;
			if (WHOLE_SIZE > 0)
			{
				SpiInstance.dmaSend((void*)frameBuffer, (uint16_t)CHUNK_SIZE, true);
				PushIndex += CHUNK_SIZE;
			}
			return 0;
#elif defined(ARDUINO_ARCH_RP2040)
			SpiInstance.transferAsync((const void*)frameBuffer, (void*)nullptr, BufferSize);
#endif

			return pushSleepDuration;
		}

		bool PushingBuffer(const uint8_t* frameBuffer) final
		{
#if defined(ARDUINO_ARCH_STM32F1)
			return !spi_is_tx_empty(SpiInstance.dev())
				|| spi_is_busy(SpiInstance.dev());
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
					return false;
				}
			}
			else
			{
				return true;
			}
#elif defined(ARDUINO_ARCH_RP2040)
			return !SpiInstance.finishedAsync();
#endif
		}
	};
}
#endif
#endif