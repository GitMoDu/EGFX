// SSD1306.h

#ifndef _SSD1306_h
#define _SSD1306_h

#include <stdint.h>

namespace SSD1306
{
	static constexpr uint8_t ConfigBatch[]{ 0x00, 0xAE, 0x20, 0x00, 0xC8, 0x40, 0x81, 0x7F, 0xA1, 0xA6, 0xA8, 0x3F, 0xD3, 0x00, 0xD5, 0x80, 0xD9, 0x22, 0xDA, 0x12, 0xDB, 0x20, 0x8D, 0x14, 0xA4, 0xAF };

	static constexpr uint8_t I2CAddress = 0x3C;
	static constexpr uint8_t Width = 128;
	static constexpr uint8_t Height = 64;

	static constexpr uint32_t RESET_DELAY_MICROS = 250;
	static constexpr uint8_t RESET_WAIT_MICROS = 10;

	enum class CommandEnum : uint8_t
	{
		CommandStart = 0x00,
		Reset = 0x03,
		BufferStart = 0x40,
		Column = 0x21,
		Page = 0x22,
		SetContrast = 0x81,
		ShowFullWhite = 0xA4,
		RamDisplay = 0xA5,
		InvertDisplay = 0xA5,
		Backlight = 0xAD,
		DisplayOff = 0xAE,
		DisplayOn = 0xAF
	};

	enum class BacklightEnum : uint8_t
	{
		External = 0x00,
		Internal = 0x30
	};
}

namespace SSD1306_128x64
{
	static constexpr uint8_t Width = 128;
	static constexpr uint8_t Height = 64;
}

namespace SSD1306_128x32
{
	static constexpr uint8_t Width = 128;
	static constexpr uint8_t Height = 32;

	static constexpr int8_t VerticalOffset = -(int8_t)Height / 8;
}

namespace SSD1306_64x48
{
	static constexpr uint8_t Width = 64;
	static constexpr uint8_t Height = 48;

	static constexpr int8_t VerticalOffset = -(int8_t)Height / 2;
}

namespace SSD1306_64x32
{
	static constexpr uint8_t Width = 64;
	static constexpr uint8_t Height = 32;
}

namespace SSD1306_72x40
{
	static constexpr uint8_t Width = 72;
	static constexpr uint8_t Height = 40;
}

#endif