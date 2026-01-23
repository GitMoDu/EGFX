#ifndef _SH1107_h
#define _SH1107_h

#include <stdint.h>

namespace SH1107
{
	static constexpr uint8_t I2CAddress = 0x3C;

	static constexpr uint8_t Width = 128;
	static constexpr uint8_t Height = 128;

	static constexpr uint16_t BOOT_DELAY_MICROS = UINT16_MAX;
	static constexpr uint8_t RESET_DELAY_MICROS = 100;
	static constexpr uint8_t RESET_WAIT_MICROS = 5;

	static constexpr uint8_t Pages = (Height + 7) / 8;
	static constexpr uint16_t PageChunkSize = Width;

	static constexpr uint8_t ConfigBatch[]
	{
		0x00,
		0xAE, // Display OFF
		0xA8, 0x7F, // Multiplex ratio = 0x7F (128)
		0xD3, 0x00, // Display offset
		0x40, // Display start line
		0xA1, // Segment remap
		0xC8, // COM scan direction
		0xDA, 0x12, // COM pins
		0x81, 0xFF, // Contrast
		0xA6, // Normal display (not inverted)
		0xAF  // Display ON
	};

	enum class CommandEnum : uint8_t
	{
		CommandStart = 0x00,

		PushPage = 0xB0,

		PushAddressHigh = 0x10,
		PushAddressLow = 0x00,

		DisplayOn = 0xAF,
		DisplayOff = 0xAE,
	};
}

#endif