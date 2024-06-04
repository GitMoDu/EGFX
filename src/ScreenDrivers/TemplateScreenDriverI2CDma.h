// TemplateScreenDriverI2CDma.h

#ifndef _TEMPLATE_SCREEN_DRIVER_I2C_DMA_h
#define _TEMPLATE_SCREEN_DRIVER_I2C_DMA_h

#if defined(ARDUINO_ARCH_RP2040)
#define TEMPLATE_SCREEN_DRIVER_I2C_DMA

#include <stdint.h>

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
	TemplateScreenDriverI2CDma() : InlineI2cScreenDriver() {}

	virtual const uint32_t PushBuffer(const uint8_t* frameBuffer) final
	{
		PushIndex = 0;

		return 0;
	}

	virtual const bool PushingBuffer(const uint8_t* frameBuffer) final
	{
#if defined(ARDUINO_ARCH_RP2040)
		if (WireInstance.finishedAsync())
#endif
		{
			if (BUFFER_WHOLE > 0
				&& PushIndex < (BUFFER_WHOLE * I2C_BUFFER_SIZE))
			{
				PushChunkDma(&frameBuffer[PushIndex], I2C_BUFFER_SIZE);
				PushIndex += I2C_BUFFER_SIZE;
			}
			else if (BUFFER_REMAINDER > 0)
			{
				if (PushIndex < ((BUFFER_WHOLE * I2C_BUFFER_SIZE) + BUFFER_REMAINDER))
				{
					PushChunkDma(&frameBuffer[REMAINDER_START], BUFFER_REMAINDER);
					PushIndex += I2C_BUFFER_SIZE;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}

		return true;
	}
};
#endif
#endif