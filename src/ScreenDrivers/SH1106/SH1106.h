// SH1106.h

#ifndef _SH1106_h
#define _SH1106_h

#include <stdint.h>

namespace SH1106
{
	static constexpr uint8_t ConfigBatch[]{ 0x00, 0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40, 0xAD, 0x8B, 0xA1, 0xC8, 0xDA, 0x12, 0x81, 0xFF, 0xD9, 0x1F, 0xDB, 0x40, 0x33, 0xA6, 0x20, 0x10, 0xA4 };

	static constexpr uint8_t I2CAddress = 0x3C;
	static constexpr uint8_t Width = 132;
	static constexpr uint8_t Height = 64;

	static constexpr uint32_t BOOT_DELAY_MICROS = 100000;
	static constexpr uint32_t RESET_DELAY_MICROS = 100;
	static constexpr uint8_t RESET_WAIT_MICROS = 5;

	enum class CommandEnum : uint8_t
	{
		CommandStart = 0x00,
		Reset = 0x03,
		PushPage = 0xB0,
		PushAddress = 0x10,
		DisplayOn = 0xAF
	};
}

namespace SH1106_128x64
{
	static constexpr uint8_t Width = 128;
	static constexpr uint8_t Height = 64;

	static constexpr uint8_t PageStartOffset = 2;
	static constexpr uint8_t Pages = (Height + 7) / 8;
	static constexpr uint8_t PageChunkSize = Width;

	enum class CommandEnum : uint8_t
	{
		PushPage1 = (uint8_t)SH1106::CommandEnum::PushAddress + ((SH1106_128x64::PageStartOffset) >> 4),
		PushPage2 = (SH1106_128x64::PageStartOffset) & 0x0F,
	};
}
#endif