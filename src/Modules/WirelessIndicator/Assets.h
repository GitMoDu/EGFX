#ifndef _EGFX_MODULES_WIRELESS_INDICATOR_ASSETS_h
#define _EGFX_MODULES_WIRELESS_INDICATOR_ASSETS_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Modules
	{
		namespace WirelessIndicator
		{
			namespace Assets
			{
				namespace Bitmask22x16
				{
					static constexpr uint8_t Width = 22;
					static constexpr uint8_t Height = 16;
					static constexpr uint8_t SliceHeight = 4;
					static constexpr uint8_t Slice0Height = 4;
					static constexpr uint8_t Slice1Height = 4;
					static constexpr uint8_t Slice2Height = 5;
					static constexpr uint8_t Slice3Height = 6;

					static constexpr uint8_t Slice_0_22x4[] PROGMEM
					{
						0b00000000, 0b00110000, 0b00000000,
						0b00000000, 0b01111000, 0b00000000,
						0b00000000, 0b01111000, 0b00000000,
						0b00000000, 0b00110000, 0b00000000
					};

					static constexpr uint8_t Slice_1_22x4[] PROGMEM
					{
						0b00000000, 0b11111100, 0b00000000,
						0b00000001, 0b11111110, 0b00000000,
						0b00000011, 0b10000111, 0b00000000,
						0b00000011, 0b00000011, 0b00000000
					};

					static constexpr uint8_t Slice_2_22x5[] PROGMEM
					{
						0b00000001, 0b11111110, 0b00000000,
						0b00000111, 0b11111111, 0b10000000,
						0b00001111, 0b00000011, 0b11000000,
						0b00011100, 0b00000000, 0b11100000,
						0b00011000, 0b00000000, 0b01100000
					};

					static constexpr uint8_t Slice_3_22x6[] PROGMEM
					{
						0b00000011, 0b11111111, 0b00000000,
						0b00001111, 0b11111111, 0b11000000,
						0b00111110, 0b00000001, 0b11110000,
						0b01111000, 0b00000000, 0b01111000,
						0b11100000, 0b00000000, 0b00011100,
						0b11000000, 0b00000000, 0b00001100
					};
				}

				namespace Bitmask24x16
				{
					static constexpr uint8_t Width = 24;
					static constexpr uint8_t Height = 16;
					static constexpr uint8_t SliceHeight = 4;
					static constexpr uint8_t Slice0Height = 4;
					static constexpr uint8_t Slice1Height = 5;
					static constexpr uint8_t Slice2Height = 6;
					static constexpr uint8_t Slice3Height = 7;

					static constexpr uint8_t Slice_0_24x4[] PROGMEM
					{
						0b00000000, 0b00011000, 0b00000000,
						0b00000000, 0b00111100, 0b00000000,
						0b00000000, 0b00111100, 0b00000000,
						0b00000000, 0b00011000, 0b00000000
					};

					static constexpr uint8_t Slice_1_24x5[] PROGMEM
					{
						0b00000000, 0b00111100, 0b00000000,
						0b00000000, 0b11111111, 0b00000000,
						0b00000001, 0b11100111, 0b10000000,
						0b00000011, 0b10000001, 0b11000000,
						0b00000011, 0b00000000, 0b11000000,
					};

					static constexpr uint8_t Slice_2_24x6[] PROGMEM
					{
						0b00000000, 0b01111110, 0b00000000,
						0b00000001, 0b11111111, 0b10000000,
						0b00000111, 0b11000011, 0b11100000,
						0b00001111, 0b00000000, 0b11110000,
						0b00011100, 0b00000000, 0b00111000,
						0b00011000, 0b00000000, 0b00011000,
					};

					static constexpr uint8_t Slice_3_24x7[] PROGMEM
					{
						0b00000001, 0b11111111, 0b10000000,
						0b00000111, 0b11111111, 0b11100000,
						0b00011111, 0b00000000, 0b11111000,
						0b00111100, 0b00000000, 0b00111100,
						0b01110000, 0b00000000, 0b00001110,
						0b11100000, 0b00000000, 0b00000111,
						0b11000000, 0b00000000, 0b00000011,
					};
				}

				namespace Bitmask42x30
				{
					static constexpr uint8_t Width = 42;
					static constexpr uint8_t Height = 30;
					static constexpr uint8_t SliceHeight = 7;
					static constexpr uint8_t Slice0Height = 7;
					static constexpr uint8_t Slice1Height = 7;
					static constexpr uint8_t Slice2Height = 10;
					static constexpr uint8_t Slice3Height = 12;

					static constexpr uint8_t Slice_0_42x7[] PROGMEM
					{
						0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80,
						0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00,
						0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00
					};

					static constexpr uint8_t Slice_1_42x7[] PROGMEM
					{
						0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf8,
						0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0xfe, 0x00, 0x00, 0x00, 0x1f,
						0x00, 0x3e, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x1c, 0x00, 0x00
					};

					static constexpr uint8_t Slice_2_42x10[] PROGMEM
					{
						0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfe,
						0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0x80, 0x00, 0x01, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x03, 0xff,
						0x00, 0x3f, 0xf0, 0x00, 0x07, 0xf8, 0x00, 0x07, 0xf8, 0x00, 0x07, 0xe0, 0x00, 0x01, 0xf8, 0x00,
						0x07, 0xc0, 0x00, 0x00, 0xf8, 0x00, 0x03, 0x80, 0x00, 0x00, 0x70, 0x00
					};

					static constexpr uint8_t Slice_3_42x12[] PROGMEM
					{
						0x00, 0x01, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
						0xc0, 0x00, 0x03, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x1f, 0xfe,
						0x00, 0x1f, 0xfe, 0x00, 0x3f, 0xe0, 0x00, 0x01, 0xff, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x7f, 0x80,
						0xfe, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0xfc, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0xf8, 0x00, 0x00, 0x00,
						0x07, 0xc0, 0x70, 0x00, 0x00, 0x00, 0x03, 0x80
					};
				}
			}
		}
	}
}
#endif