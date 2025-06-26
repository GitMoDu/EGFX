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
		/// <summary>
		/// Indicates the start of a command sequence.
		/// </summary>
		CommandStart = 0x00,

		/// <summary>
		/// Resets the display.
		/// </summary>
		Reset = 0x03,

		/// <summary>
		/// Indicates the start of data buffer.
		/// </summary>
		SetLineStart = 0x40,

		/// <summary>
		/// Sets the column address.
		/// </summary>
		Column = 0x21,

		/// <summary>
		/// Sets the page address.
		/// </summary>
		Page = 0x22,

		/// <summary>
		/// Sets the contrast level.
		/// </summary>
		SetContrast = 0x81,

		/// <summary>
		/// Controls the charge pump setting.
		/// </summary>
		ChargePump = 0x8D,

		/// <summary>
		/// Sets the segment remap.
		/// </summary>
		SegmentRemap = 0xA0,

		/// <summary>
		/// Turns on all pixels.
		/// </summary>
		EntireDisplayOn = 0xA4,

		/// <summary>
		/// Resumes to RAM content display.
		/// </summary>
		EntireDisplayOnResume = 0xA5,

		/// <summary>
		/// Sets the display to normal mode.
		/// </summary>
		NormalDisplay = 0xA6,

		/// <summary>
		/// Inverts the display.
		/// </summary>
		InvertDisplay = 0xA7,

		/// <summary>
		/// Sets the multiplex ratio.
		/// </summary>
		SetMultiplexRatio = 0xA8,

		/// <summary>
		/// Turns the display off.
		/// </summary>
		DisplayOff = 0xAE,

		/// <summary>
		/// Turns the display on.
		/// </summary>
		DisplayOn = 0xAF,

		/// <summary>
		/// Sets the display offset.
		/// </summary>
		SetDisplayOffset = 0xD3,

		/// <summary>
		/// Sets the display clock divide ratio/oscillator frequency.
		/// </summary>
		SetDisplayClockDiv = 0xD5,

		/// <summary>
		/// Sets the pre-charge period.
		/// </summary>
		SetPrecharge = 0xD9,

		/// <summary>
		/// Sets the COM pins hardware configuration.
		/// </summary>
		SetComPins = 0xDA,

		/// <summary>
		/// Sets the VCOMH deselect level.
		/// </summary>
		SetVcomDetect = 0xDB,

		/// <summary>
		/// No operation.
		/// </summary>
		Nop = 0xE3,

		/// <summary>
		/// Scrolls the display to the right.
		/// </summary>
		ScrollRight = 0x26,

		/// <summary>
		/// Scrolls the display to the left.
		/// </summary>
		ScrollLeft = 0x27,

		/// <summary>
		/// Scrolls the display vertically and to the right.
		/// </summary>
		ScrollVerticalRight = 0x29,

		/// <summary>
		/// Scrolls the display vertically and to the left.
		/// </summary>
		ScrollVerticalLeft = 0x2A,

		/// <summary>
		/// Deactivates scrolling.
		/// </summary>
		DeactivateScroll = 0x2E,

		/// <summary>
		/// Activates scrolling.
		/// </summary>
		ActivateScroll = 0x2F,

		/// <summary>
		/// Sets the vertical scroll area.
		/// </summary>
		SetVerticalScrollArea = 0xA3,
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