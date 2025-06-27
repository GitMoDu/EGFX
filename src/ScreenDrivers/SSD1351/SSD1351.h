#ifndef _SSD1351_h
#define _SSD1351_h

#include <stdint.h>

namespace SSD1351
{
	static constexpr uint8_t Width = 128;
	static constexpr uint8_t Height = 128;

	static constexpr uint32_t RESET_DELAY_MICROS = 1000;
	static constexpr uint8_t RESET_WAIT_MICROS = 10;

	static constexpr uint32_t SpiMaxSpeed = 4000000;

	static constexpr uint32_t LimitedSpiSpeed(const uint32_t speed)
	{
		return ((speed < SpiMaxSpeed) * (speed)) | ((speed >= SpiMaxSpeed) * (SpiMaxSpeed));
	}

	enum class CommandEnum : uint8_t
	{
		SetColumnAddress = 0x15,
		WriteData = 0x5C,
		SetRowAddress = 0x75,
		SetContrastA = 0xC1,
		SetMasterCurrentAF = 0xC7,
		RemapAndColor = 0xA0,
		SetStartLine = 0xA1,
		SetDisplayOffset = 0xA2,
		DisplayOn = 0xA5,
		SetDisplayModeOff = 0xA4,
		SetDisplayModeNormal = 0xA6,
		SetDisplayModeInverted = 0xA7,
		SetSleepOn = 0xAE,
		SetSleepOff = 0xAF,
		SetSegmentLowVoltage = 0xB4,
		SetDisplayClockDivider = 0xB3,
		SetPrechargeSpeed1 = 0xB1,
		SetPrechargeSpeed2 = 0xB6,
		SetPrechargeVoltage = 0xBB,
		SetGPIO = 0xB5,
		SetVCOMH = 0xBE,
		SetMuxRatio = 0xCA,
		SetPrechargeA = 0x8A,
		SetPrechargeB = 0x8B,
		SetFunction = 0xAB,
		Unlock = 0xFD,

		Nop1 = 0xAD,
		Nop2 = 0xB0,
		Nop3 = 0xD1,
		Nop4 = 0xE3
	};

	static constexpr uint8_t CommonCommandArgBatch[]
	{
		(uint8_t)CommandEnum::SetDisplayClockDivider, 0xF1,
		(uint8_t)CommandEnum::SetMuxRatio, 127,
		(uint8_t)CommandEnum::RemapAndColor, 0x34,
		(uint8_t)CommandEnum::SetStartLine, 0x00,
		(uint8_t)CommandEnum::SetDisplayOffset, 0x00,
		(uint8_t)CommandEnum::SetGPIO, 0x0,
		(uint8_t)CommandEnum::SetFunction, 0x00,
		(uint8_t)CommandEnum::SetPrechargeSpeed1, 0x32,
		(uint8_t)CommandEnum::SetVCOMH, 0x05,
		(uint8_t)CommandEnum::SetPrechargeVoltage, 0x17
	};
}
#endif