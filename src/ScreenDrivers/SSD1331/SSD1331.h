#ifndef _SSD1331_h
#define _SSD1331_h

#include <stdint.h>

namespace SSD1331
{
	static constexpr uint8_t Width = 96;
	static constexpr uint8_t Height = 64;

	static constexpr uint16_t RESET_DELAY_MICROS = 1250;
	static constexpr uint8_t RESET_WAIT_MICROS = 5;

	static constexpr uint8_t MapRgb332 = 0x32;
	static constexpr uint8_t MapRgb565 = 0x72;

	enum class CommandEnum : uint8_t
	{
		SetColumnAddress = 0x15,
		DrawLine = 0x21,
		SetRowAddress = 0x75,
		SetContrastRed = 0x81,
		SetContrastGreen = 0x82,
		SetContrastBlue = 0x83,
		SetMasterCurrentAF = 0x87,
		SetPrechargeSpeed1 = 0x8A,
		SetPrechargeSpeed2 = 0x8B,
		RemapAndColor = 0xA0,
		SetStartLine = 0xA1,
		SetDisplayOffset = 0xA2,
		SetDisplayModeNormal = 0xA4,
		SetDisplayModeOff = 0xA5,
		SetDisplayModeInverted = 0xA6,
		SetMuxRatio = 0xA8,
		SetMasterConfiguration = 0xAD,
		DisplayOff = 0xAE,
		DisplayOn = 0xAF,
		SetPowerSaveMode = 0xB0,
		SetPhasesPeriodAdjustment = 0xB1,
		SetDisplayClockDivider = 0xB3,
		SetPrechargeVoltage = 0xBB,
		SetVCOMH = 0xBE,
		Nop = 0xE3,
	};

	static constexpr uint8_t CommonConfigBatch[]
	{
		(uint8_t)CommandEnum::SetStartLine, 0x00,
		(uint8_t)CommandEnum::SetDisplayOffset, 0x00,
		(uint8_t)CommandEnum::SetDisplayModeNormal,
		(uint8_t)CommandEnum::SetMuxRatio, 63,
		(uint8_t)CommandEnum::SetMasterConfiguration, 0x8E,
		(uint8_t)CommandEnum::SetPowerSaveMode, 0x0B,
		(uint8_t)CommandEnum::SetPhasesPeriodAdjustment, 0x31,
		(uint8_t)CommandEnum::SetDisplayClockDivider, 0xF0,
		(uint8_t)CommandEnum::SetPrechargeSpeed1, 0x64,
		(uint8_t)CommandEnum::SetPrechargeSpeed2, 0x78,
		(uint8_t)CommandEnum::SetPrechargeVoltage, 0x3A,
		(uint8_t)CommandEnum::SetVCOMH, 0x3E,
		(uint8_t)CommandEnum::SetMasterCurrentAF, 0x09,
		(uint8_t)CommandEnum::SetContrastRed, 0x91,
		(uint8_t)CommandEnum::SetContrastGreen, 0x50,
		(uint8_t)CommandEnum::SetContrastBlue, 0x7D,
		(uint8_t)CommandEnum::SetColumnAddress, 0, Width - 1,
		(uint8_t)CommandEnum::SetRowAddress, 0, Height - 1,
		(uint8_t)CommandEnum::DisplayOn
	};
}
#endif