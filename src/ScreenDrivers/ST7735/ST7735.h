#ifndef _ST7735_h
#define _ST7735_h

#include <stdint.h>

namespace ST7735
{
	static constexpr uint8_t Width = 162;
	static constexpr uint8_t Height = 132;

	namespace ST7735S
	{
		static constexpr uint8_t Width = 160;
		static constexpr uint8_t Height = 80;

		static constexpr uint8_t XOffset = (ST7735::Width - ST7735S::Width) / 2;
		static constexpr uint8_t YOffset = (ST7735::Height - ST7735S::Height) / 2;
	}

	static constexpr uint32_t RESET_DELAY_MICROS = 1000;
	static constexpr uint8_t RESET_WAIT_MICROS = 10;

	static constexpr uint32_t SpiMaxSpeed = 33333333; // Serial write clock cycle ~33 ns.

	static constexpr uint32_t LimitedSpiSpeed(const uint32_t speed)
	{
		return ((speed < SpiMaxSpeed) * (speed)) | ((speed >= SpiMaxSpeed) * (SpiMaxSpeed));
	}

	enum class CommandEnum : uint8_t
	{
		Nop = 0x00,
		SoftwareReset = 0x01,

		SetSleepOn = 0x10,
		SetSleepOff = 0x11,
		PartialDisplayOn = 0x12,
		PartialDisplayOff = 0x13,
		SetInversionOn = 0x20,
		SetInversionOff = 0x21,
		SetGamma = 0x26,
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
		TearingOff = 0x34,
		SetTearing = 0x35,
		FrameRateControl1 = 0xB1,
		FrameRateControl2 = 0xB2,
		FrameRateControl3 = 0xB3,
		InversionControl = 0xB4,
		DisplaySettings = 0xB6,

		PowerControl1 = 0xC0,
		PowerControl2 = 0xC1,
		PowerControl3 = 0xC2,
		PowerControl4 = 0xC3,
		PowerControl5 = 0xC4,

		PowerVmControl = 0xC5,
		PowerLControl = 0xC7,

		GatePumpClockFrequency = 0xFC,
		SetGammaPositiveCorrection = 0xE0,
		SetGammaNegativeCorrection = 0xE1,

		NvmControlStatus = 0xD9,
	};

	static constexpr uint8_t GammaPositiveCorrection[]
	{
		0x02, 0x1c, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2c,
		0x29, 0x25, 0x2b, 0x39, 0x00, 0x01, 0x03, 0x10
	};

	static constexpr uint8_t GammaNegativeCorrection[]
	{
		0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2c,
		0x2e, 0x2e, 0x37, 0x3f, 0x00, 0x00, 0x02, 0x10
	};
}
#endif