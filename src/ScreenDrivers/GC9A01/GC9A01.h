#ifndef _EGFX_SCREEN_DRIVERS_GC9A01_h
#define _EGFX_SCREEN_DRIVERS_GC9A01_h

#include <stdint.h>

namespace GC9A01
{
	static constexpr uint32_t RESET_DELAY_MICROS = 10000;
	static constexpr uint32_t RESET_WAIT_MICROS = 10;
	static constexpr uint32_t SLEEP_EXIT_DELAY_MICROS = 120000; // 120 ms
	static constexpr uint32_t DISPLAY_ON_DELAY_MICROS = 10000;  // 10 ms

	static constexpr uint16_t Width = 240;
	static constexpr uint16_t Height = 240;

	static constexpr uint32_t SpiMaxSpeed = 80000000;
	static constexpr uint32_t SpiDefaultSpeed = 40000000;

	static constexpr uint32_t LimitedSpiSpeed(const uint32_t speed)
	{
		return speed < SpiMaxSpeed ? speed : SpiMaxSpeed;
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
		SleepIn = 0x10,
		SleepOut = 0x11,
		NormalDisplayOn = 0x13,
		DisplayInversionOn = 0x21,
		DisplayOn = 0x29,
		MemoryWrite = 0x2C,
		ColumnAddressSet = 0x2A,
		RowAddressSet = 0x2B,
		MemoryAccessCtrl = 0x36,
		PixelFormatSet = 0x3A,
		PorchSetting = 0xB2,
		GateControl = 0xB7,
		VCOMSSetting = 0xBB,
		LCMControl = 0xC0,
		VDVVRHEN = 0xC2,
		VRHSet = 0xC3,
		VDVSet = 0xC4,
		FrameRateControl = 0xC6,
		PowerControl1 = 0xD0,
		PositiveGamma = 0xE0,
		NegativeGamma = 0xE1
	};

	namespace PixelFormats
	{
		static constexpr uint8_t Rgb565 = 0x55;
	}

	namespace Init
	{
		struct RawCommand final
		{
			uint8_t Command;
			const uint8_t* Data;
			size_t Length;
		};

		static constexpr RawCommand Cmd(const uint8_t command)
		{
			return { command, nullptr, 0 };
		}

		static constexpr RawCommand Cmd(const uint8_t command, const uint8_t* data, const size_t length)
		{
			return { command, data, length };
		}

		// Payload bytes
		static constexpr uint8_t EB_14[]{ 0x14 };
		static constexpr uint8_t C_40[]{ 0x40 };
		static constexpr uint8_t C_FF[]{ 0xFF };
		static constexpr uint8_t C_0A[]{ 0x0A };
		static constexpr uint8_t C_21[]{ 0x21 };
		static constexpr uint8_t C_00[]{ 0x00 };
		static constexpr uint8_t C_80[]{ 0x80 };
		static constexpr uint8_t C_01[]{ 0x01 };
		static constexpr uint8_t B6[]{ 0x00, 0x20 };
		static constexpr uint8_t C90[]{ 0x08, 0x08, 0x08, 0x08 };
		static constexpr uint8_t BD[]{ 0x06 };
		static constexpr uint8_t BC[]{ 0x00 };
		static constexpr uint8_t FF[]{ 0x60, 0x01, 0x04 };
		static constexpr uint8_t C_13[]{ 0x13 };
		static constexpr uint8_t C_22[]{ 0x22 };
		static constexpr uint8_t C_11[]{ 0x11 };
		static constexpr uint8_t E1[]{ 0x10, 0x0E };
		static constexpr uint8_t DF[]{ 0x21, 0x0C, 0x02 };
		static constexpr uint8_t F0[]{ 0x45, 0x09, 0x08, 0x08, 0x26, 0x2A };
		static constexpr uint8_t F1[]{ 0x43, 0x70, 0x72, 0x36, 0x37, 0x6F };
		static constexpr uint8_t ED[]{ 0x1B, 0x0B };
		static constexpr uint8_t AE[]{ 0x77 };
		static constexpr uint8_t CD[]{ 0x63 };
		static constexpr uint8_t C70[]{ 0x07, 0x07, 0x04, 0x0E, 0x0F, 0x09, 0x07, 0x08, 0x03 };
		static constexpr uint8_t E8[]{ 0x34 };
		static constexpr uint8_t C62[]{ 0x18, 0x0D, 0x71, 0xED, 0x70, 0x70, 0x18, 0x0F, 0x71, 0xEF, 0x70, 0x70 };
		static constexpr uint8_t C63[]{ 0x18, 0x11, 0x71, 0xF1, 0x70, 0x70, 0x18, 0x13, 0x71, 0xF3, 0x70, 0x70 };
		static constexpr uint8_t C64[]{ 0x28, 0x29, 0xF1, 0x01, 0xF1, 0x00, 0x07 };
		static constexpr uint8_t C66[]{ 0x3C, 0x00, 0xCD, 0x67, 0x45, 0x45, 0x10, 0x00, 0x00, 0x00 };
		static constexpr uint8_t C67[]{ 0x00, 0x3C, 0x00, 0x00, 0x00, 0x01, 0x54, 0x10, 0x32, 0x98 };
		static constexpr uint8_t C74[]{ 0x10, 0x85, 0x80, 0x00, 0x00, 0x4E, 0x00 };
		static constexpr uint8_t C98[]{ 0x3E, 0x07 };

		// Used by the driver. This is the “vendor init block”.
		static constexpr RawCommand Sequence[] =
		{
			Cmd(0xEF),
			Cmd(0xEB, EB_14, sizeof(EB_14)),
			Cmd(0xFE),
			Cmd(0xEF),
			Cmd(0xEB, EB_14, sizeof(EB_14)),

			Cmd(0x84, C_40, sizeof(C_40)),
			Cmd(0x85, C_FF, sizeof(C_FF)),
			Cmd(0x86, C_FF, sizeof(C_FF)),
			Cmd(0x87, C_FF, sizeof(C_FF)),
			Cmd(0x88, C_0A, sizeof(C_0A)),
			Cmd(0x89, C_21, sizeof(C_21)),
			Cmd(0x8A, C_00, sizeof(C_00)),
			Cmd(0x8B, C_80, sizeof(C_80)),
			Cmd(0x8C, C_01, sizeof(C_01)),
			Cmd(0x8D, C_01, sizeof(C_01)),
			Cmd(0x8E, C_FF, sizeof(C_FF)),
			Cmd(0x8F, C_FF, sizeof(C_FF)),

			Cmd(0xB6, B6, sizeof(B6)),
			Cmd(0x90, C90, sizeof(C90)),
			Cmd(0xBD, BD, sizeof(BD)),
			Cmd(0xBC, BC, sizeof(BC)),
			Cmd(0xFF, FF, sizeof(FF)),
			Cmd(0xC3, C_13, sizeof(C_13)),
			Cmd(0xC4, C_13, sizeof(C_13)),
			Cmd(0xC9, C_22, sizeof(C_22)),
			Cmd(0xBE, C_11, sizeof(C_11)),
			Cmd(0xE1, E1, sizeof(E1)),
			Cmd(0xDF, DF, sizeof(DF)),

			Cmd(0xF0, F0, sizeof(F0)),
			Cmd(0xF1, F1, sizeof(F1)),
			Cmd(0xF2, F0, sizeof(F0)),
			Cmd(0xF3, F1, sizeof(F1)),

			Cmd(0xED, ED, sizeof(ED)),
			Cmd(0xAE, AE, sizeof(AE)),
			Cmd(0xCD, CD, sizeof(CD)),
			Cmd(0x70, C70, sizeof(C70)),
			Cmd(0xE8, E8, sizeof(E8)),
			Cmd(0x62, C62, sizeof(C62)),
			Cmd(0x63, C63, sizeof(C63)),
			Cmd(0x64, C64, sizeof(C64)),
			Cmd(0x66, C66, sizeof(C66)),
			Cmd(0x67, C67, sizeof(C67)),
			Cmd(0x74, C74, sizeof(C74)),
			Cmd(0x98, C98, sizeof(C98)),
		};
	}
}

#endif