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

		virtual const uint32_t PushBuffer(const uint8_t* frameBuffer) final
		{
			PushIndex = 0;

			if (WHOLE_SIZE > 0)
			{
				SpiInstance.transfer((void*)frameBuffer, CHUNK_SIZE);
				PushIndex += CHUNK_SIZE;
			}

			return 0;
		}

		virtual const bool PushingBuffer(const uint8_t* frameBuffer) final
		{
			if (PushIndex < WHOLE_SIZE)
			{
				SpiInstance.transfer((void*)&frameBuffer[PushIndex], CHUNK_SIZE);
				PushIndex += CHUNK_SIZE;

				return true;
			}
			else
			{
				if (REMAINDER_SIZE > 0)
				{
					SpiInstance.transfer((void*)&frameBuffer[REMAINDER_START], REMAINDER_SIZE);
				}

				return false;
			}
		}
	};
}
#endif