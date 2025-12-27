// TemplateScreenDriverI2CAsync.h

#ifndef _TEMPLATE_SCREEN_DRIVER_I2C_ASYNC_h
#define _TEMPLATE_SCREEN_DRIVER_I2C_ASYNC_h

#include <stdint.h>

namespace Egfx
{
	template<typename InlineI2cScreenDriver,
		const uint8_t i2cChunkSize = 8>
	class TemplateScreenDriverI2CAsync : public InlineI2cScreenDriver
	{
	public:
		using InlineI2cScreenDriver::BufferSize;

	protected:
		using InlineI2cScreenDriver::PushChunk;

	protected:
		static constexpr uint8_t I2C_BUFFER_SIZE = 31;

	private:
		static constexpr uint8_t CHUNK_SIZE = i2cChunkSize;
		static constexpr size_t WHOLE_SIZE = (BufferSize / CHUNK_SIZE) * CHUNK_SIZE;
		static constexpr size_t REMAINDER_SIZE = BufferSize - WHOLE_SIZE;
		static constexpr size_t REMAINDER_START = WHOLE_SIZE;

	private:
		size_t PushIndex = 0;

	public:
		TemplateScreenDriverI2CAsync(TwoWire& wire) : InlineI2cScreenDriver(wire) {}

		virtual bool Start()
		{
			return CHUNK_SIZE <= BufferSize
				&& CHUNK_SIZE <= I2C_BUFFER_SIZE
				&& InlineI2cScreenDriver::Start();
		}

		virtual uint32_t PushBuffer(const uint8_t* frameBuffer)
		{
			PushIndex = 0;

			if (WHOLE_SIZE > 0)
			{
				PushChunk(frameBuffer, CHUNK_SIZE);
				PushIndex += CHUNK_SIZE;
			}

			return 0;
		}

		virtual bool PushingBuffer(const uint8_t* frameBuffer) final
		{
			if (PushIndex < WHOLE_SIZE)
			{
				PushChunk(&frameBuffer[PushIndex], CHUNK_SIZE);
				PushIndex += CHUNK_SIZE;

				return true;
			}
			else
			{
				if (REMAINDER_SIZE > 0)
				{
					PushChunk(&frameBuffer[REMAINDER_START], REMAINDER_SIZE);
				}

				return false;
			}
		}
	};
}
#endif