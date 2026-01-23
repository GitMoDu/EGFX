// TemplateScreenDriverSpiAsync.h

#ifndef _TEMPLATE_SCREEN_DRIVER_SPI_ASYNC_h
#define _TEMPLATE_SCREEN_DRIVER_SPI_ASYNC_h

#include <stdint.h>

namespace Egfx
{
	template<typename InlineSpiScreenDriver,
		const uint8_t spiChunkDivisor = 2>
	class TemplateScreenDriverSpiAsync : public InlineSpiScreenDriver
	{
	public:
		using InlineSpiScreenDriver::BufferSize;

	protected:
		using InlineSpiScreenDriver::SpiInstance;

	private:
		static constexpr size_t CHUNK_SIZE = BufferSize / spiChunkDivisor;
		static constexpr size_t WHOLE_SIZE = (BufferSize / CHUNK_SIZE) * CHUNK_SIZE;
		static constexpr size_t REMAINDER_SIZE = BufferSize - WHOLE_SIZE;
		static constexpr size_t REMAINDER_START = WHOLE_SIZE;

	private:
		size_t PushIndex = 0;

	public:
		TemplateScreenDriverSpiAsync(Egfx::SpiType& spi) : InlineSpiScreenDriver(spi) {}

		~TemplateScreenDriverSpiAsync() override = default;

		uint32_t PushBuffer(const uint8_t* frameBuffer) final
		{
			PushIndex = 0;

			if (WHOLE_SIZE > 0)
			{
#if defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F4)
				SpiInstance.transfer((uint8_t*)frameBuffer, CHUNK_SIZE);
#else
				SpiInstance.transfer((void*)frameBuffer, CHUNK_SIZE);
#endif
				PushIndex += CHUNK_SIZE;
			}

			return 0;
		}

		bool PushingBuffer(const uint8_t* frameBuffer) final
		{
			if (PushIndex < WHOLE_SIZE)
			{
#if defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F4)
				SpiInstance.transfer((uint8_t*)&frameBuffer[PushIndex], CHUNK_SIZE);
#else
				SpiInstance.transfer((void*)&frameBuffer[PushIndex], CHUNK_SIZE);
#endif
				PushIndex += CHUNK_SIZE;

				return true;
			}
			else
			{
				if (REMAINDER_SIZE > 0)
				{
#if defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F4)
					SpiInstance.transfer((uint8_t*)&frameBuffer[REMAINDER_START], CHUNK_SIZE);
#else
					SpiInstance.transfer((void*)&frameBuffer[REMAINDER_START], REMAINDER_SIZE);
#endif
				}

				return false;
			}
		}
	};
}
#endif