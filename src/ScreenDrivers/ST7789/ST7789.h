#ifndef _ST7789_h
#define _ST7789_h

#include <stdint.h>

namespace ST7789
{
	static constexpr uint8_t Width = 240;
	static constexpr uint8_t Height = 240;

	static constexpr uint32_t RESET_DELAY_MICROS = 1000;
	static constexpr uint8_t RESET_WAIT_MICROS = 10;

	static constexpr uint32_t SpiMaxSpeed = 62500000; // Serial write clock cycle = 16 ns.

	static constexpr uint32_t LimitedSpiSpeed(const uint32_t speed)
	{
		return ((speed < SpiMaxSpeed) * (speed)) | ((speed >= SpiMaxSpeed) * (SpiMaxSpeed));
	}

#if defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F4)
	static constexpr size_t SpiMaxChunkSize = size_t(UINT8_MAX);
#else
	static constexpr size_t SpiMaxChunkSize = SIZE_MAX;
#endif

	enum class CommandEnum : uint8_t
	{
		Nop = 0x00,
		SoftwareReset = 0x01,
		SetSleepOn = 0x10,
		SetSleepOff = 0x11,
		PartialDisplayOn = 0x12,
		PartialDisplayOff = 0x13,
		SetInversionOff = 0x20,
		SetInversionOn = 0x21,
		SetDisplayOff = 0x28,
		SetDisplayOn = 0x29,
		SetIdleOff = 0x38,
		SetIdleOn = 0x39,
		SetColumnAddress = 0x2A,
		SetRowAddress = 0x2B,
		MemoryWrite = 0x2C,
		MemoryRead = 0x2E,
		SetPixelFormat = 0x3A,
		SetMemoryDataAccessControl = 0x36,
		SetFrameRateControl1 = 0xB3,
		SetFrameRateControl2 = 0xC6
	};
}
#endif