#ifndef _TEMPLATE_SCREEN_DRIVER_I2C_DMA_h
#define _TEMPLATE_SCREEN_DRIVER_I2C_DMA_h

#if defined(ARDUINO_ARCH_RP2040)
#define TEMPLATE_SCREEN_DRIVER_I2C_DMA

#include <stdint.h>

namespace Egfx
{
	template<typename InlineI2cScreenDriver>
	class TemplateScreenDriverI2CDma : public InlineI2cScreenDriver
	{
	public:
		using InlineI2cScreenDriver::BufferSize;

	protected:
		using InlineI2cScreenDriver::I2C_BUFFER_SIZE;
		using InlineI2cScreenDriver::BUFFER_WHOLE;
		using InlineI2cScreenDriver::BUFFER_REMAINDER;
		using InlineI2cScreenDriver::REMAINDER_START;

		using InlineI2cScreenDriver::PushChunkDma;
		using InlineI2cScreenDriver::WireInstance;

	private:
		size_t PushIndex = 0;

	public:
		TemplateScreenDriverI2CDma(TwoWire& wire) : InlineI2cScreenDriver(wire) {}

		virtual uint32_t PushBuffer(const uint8_t* frameBuffer) override
		{
			PushIndex = 0;

			// Kick off first transfer immediately.
			if (BUFFER_WHOLE > 0)
			{
				if (PushChunkDma(&frameBuffer[0], I2C_BUFFER_SIZE))
				{
					PushIndex = I2C_BUFFER_SIZE;
				}
				return 0;
			}

			if (BUFFER_REMAINDER > 0)
			{
				if (PushChunkDma(&frameBuffer[0], BUFFER_REMAINDER))
				{
					PushIndex = BUFFER_REMAINDER;
				}
			}

			return 0;
		}

		virtual bool PushingBuffer(const uint8_t* frameBuffer) override
		{
			// Check if previous async transfer is done.
			if (!WireInstance.finishedAsync())
			{
				return true;
			}

			// Start next transfer if any data remains.
			if (BUFFER_WHOLE > 0 && PushIndex < REMAINDER_START)
			{
				if (PushChunkDma(&frameBuffer[PushIndex], I2C_BUFFER_SIZE))
				{
					PushIndex += I2C_BUFFER_SIZE;
				}
				return true;
			}

			// Handle remainder if any.
			if (BUFFER_REMAINDER > 0 && PushIndex < BufferSize)
			{
				if (PushChunkDma(&frameBuffer[REMAINDER_START], BUFFER_REMAINDER))
				{
					PushIndex = BufferSize;
				}
				return true;
			}

			// Buffer fully pushed.
			return false;
		}
	};
}
#endif
#endif