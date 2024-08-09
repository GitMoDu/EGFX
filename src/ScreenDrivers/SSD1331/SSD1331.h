// SSD1331.h

#ifndef _SSD1331_h
#define _SSD1331_h

#include <stdint.h>

namespace SSD1331
{
	static constexpr uint8_t Width = 96;
	static constexpr uint8_t Height = 64;

	static constexpr uint32_t RESET_DELAY_MICROS = 1250;
	static constexpr uint8_t RESET_WAIT_MICROS = 5;
	static constexpr uint8_t PointConfig[]{ 0x15, 0, Width - 1 , 0x75, 0, Height - 1 };

	static constexpr uint8_t MapRgb332 = 0x32;
	static constexpr uint8_t MapRgb565 = 0x72;

	enum class CommandEnum : uint8_t
	{
		SetColumnAddress = 0x15,
		DrawLine = 0x21,
		SetRowAddress = 0x75,
		SetContrastA = 0x81,
		SetContrastB = 0x82,
		SetContrastC = 0x83,
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
		NOP = 0xE3,
	};

	namespace Rgb332
	{
		static constexpr uint8_t ConfigBatch[]{
			(uint8_t)CommandEnum::DisplayOff,
			(uint8_t)CommandEnum::RemapAndColor, MapRgb332,
			(uint8_t)CommandEnum::SetStartLine, 0x00,				// First line to start scanning from.
			(uint8_t)CommandEnum::SetDisplayOffset, 0x00,			// Set display offset.
			(uint8_t)CommandEnum::SetDisplayModeNormal,
			(uint8_t)CommandEnum::SetMuxRatio, 63,					// Reset to default MUX. See datasheet
			(uint8_t)CommandEnum::SetMasterConfiguration, 0x8E,		// Set master mode.
			(uint8_t)CommandEnum::SetPowerSaveMode, 0x0B,			// Disable power-safe mode.
			(uint8_t)CommandEnum::SetPhasesPeriodAdjustment, 0x31,	// Phase 1 and Phase 2 periods.
			(uint8_t)CommandEnum::SetDisplayClockDivider, 0xF0,		// 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16).
			(uint8_t)CommandEnum::SetPrechargeSpeed1, 0x64,
			(uint8_t)CommandEnum::SetPrechargeSpeed2, 0x78,
			(uint8_t)CommandEnum::SetPrechargeVoltage, 0x3A,
			(uint8_t)CommandEnum::SetVCOMH, 0x3E,
			(uint8_t)CommandEnum::SetMasterCurrentAF, 0x09,
			(uint8_t)CommandEnum::SetContrastA, 0x91,				// Red.
			(uint8_t)CommandEnum::SetContrastB, 0x50,				// Green.
			(uint8_t)CommandEnum::SetContrastC, 0x7D,				// Blue.
			(uint8_t)CommandEnum::DisplayOn
		};
	}

	namespace Rgb565
	{
		static constexpr uint8_t ConfigBatch[]{
			(uint8_t)CommandEnum::DisplayOff,
			(uint8_t)CommandEnum::RemapAndColor, MapRgb565,
			(uint8_t)CommandEnum::SetStartLine, 0x00,				// First line to start scanning from.
			(uint8_t)CommandEnum::SetDisplayOffset, 0x00,			// Set display offset.
			(uint8_t)CommandEnum::SetDisplayModeNormal,
			(uint8_t)CommandEnum::SetMuxRatio, 63,					// Reset to default MUX. See datasheet
			(uint8_t)CommandEnum::SetMasterConfiguration, 0x8E,		// Set master mode.
			(uint8_t)CommandEnum::SetPowerSaveMode, 0x0B,			// Disable power-safe mode.
			(uint8_t)CommandEnum::SetPhasesPeriodAdjustment, 0x31,	// Phase 1 and Phase 2 periods.
			(uint8_t)CommandEnum::SetDisplayClockDivider, 0xF0,		// 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16).
			(uint8_t)CommandEnum::SetPrechargeSpeed1, 0x64,
			(uint8_t)CommandEnum::SetPrechargeSpeed2, 0x78,
			(uint8_t)CommandEnum::SetPrechargeVoltage, 0x3A,
			(uint8_t)CommandEnum::SetVCOMH, 0x3E,
			(uint8_t)CommandEnum::SetMasterCurrentAF, 0x09,
			(uint8_t)CommandEnum::SetContrastA, 0x91,				// Red.
			(uint8_t)CommandEnum::SetContrastB, 0x50,				// Green.
			(uint8_t)CommandEnum::SetContrastC, 0x7D,				// Blue.
			(uint8_t)CommandEnum::DisplayOn
		};
	}
}
#endif