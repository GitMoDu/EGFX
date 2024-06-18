// Sprites.h

#ifndef _SPRITES_h
#define _SPRITES_h

#include <stdint.h>

#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

namespace SpriteSource
{
	namespace Arrow
	{
		static constexpr uint8_t Mask[]
#if defined(ARDUINO_ARCH_AVR)
			PROGMEM
#endif 
		{
			0b00011000,
			0b00011000,
			0b00111100,
			0b01111110,
			0b01111110,
			0b11111111,
			0b00011000,
			0b00011000,
			0b00011000,
			0b00011000,
			0b00011000,
			0b00011000
		};

		static constexpr uint8_t Width = 8;
		static constexpr uint8_t Height = sizeof(Mask);
	}

	namespace RainDrop
	{
		static constexpr uint8_t Mask[]
#if defined(ARDUINO_ARCH_AVR)
			PROGMEM
#endif 
		{
			0b00110000,
			0b00110000,
			0b00110000,
			0b01111000,
			0b01111000,
			0b01111000,
			0b11111100,
			0b11111100,
			0b11111100,
			0b01111000,
			0b01111000,
		};

		static constexpr uint8_t Width = 6;
		static constexpr uint8_t Height = sizeof(Mask);
	}

	namespace Pyramid
	{
		static constexpr uint8_t Mask[]
#if defined(ARDUINO_ARCH_AVR)
			PROGMEM
#endif 
		{
			0b00000000,0b00001110,
			0b00000000,0b00011111,
			0b00000000,0b00011111,
			0b00000000,0b00001110,
			0b00000001,0b00000000,
			0b00000011,0b10000000,
			0b00000110,0b11000000,
			0b00001100,0b11100000,
			0b00011000,0b11110000,
			0b00110000,0b11111000,
			0b01100000,0b11111100,
			0b11111111,0b11111110,
			0b00000000,0b00000000,
			0b00000000,0b00000000,
			0b00000000,0b00000000,
			0b00000000,0b00000000
		};

		static constexpr uint8_t Width = 16;
		static constexpr uint8_t Height = sizeof(Mask) / sizeof(uint16_t);
	}

	namespace Heart
	{
		static constexpr uint8_t Mask[]
#if defined(ARDUINO_ARCH_AVR)
			PROGMEM
#endif 
		{
			0b00111000, 0b00011100,
			0b01111110, 0b01111110,
			0b11111111, 0b11111111,
			0b11111111, 0b11111111,
			0b11111111, 0b11111111,
			0b01111111, 0b11111110,
			0b00111111, 0b11111100,
			0b00011111, 0b11111000,
			0b00001111, 0b11110000,
			0b00000111, 0b11100000,
			0b00000011, 0b11000000,
			0b00000001, 0b10000000
		};

		static constexpr uint8_t Width = 16;
		static constexpr uint8_t Height = sizeof(Mask) / sizeof(uint16_t);
	}

	namespace Explosion
	{
		static constexpr uint8_t Mask[]
#if defined(ARDUINO_ARCH_AVR)
			PROGMEM
#endif 
		{
			0b00000000, 0b00000000,
			0b00000000, 0b00000000,
			0b00000011, 0b10000000,
			0b00000011, 0b10000000,
			0b00000011, 0b10000000,
			0b00000000, 0b00000000,
			0b00000000, 0b00000000
		};

		static constexpr uint8_t Width = 16;
		static constexpr uint8_t Height = sizeof(Mask) / sizeof(uint16_t);

		static constexpr uint8_t Mask2[sizeof(Mask)]
#if defined(ARDUINO_ARCH_AVR)
			PROGMEM
#endif 
		{
			0b00000000, 0b00000000,
			0b00000001, 0b10000000,
			0b00000011, 0b10000000,
			0b00000011, 0b11000000,
			0b00000111, 0b10000000,
			0b00000000, 0b10000000,
			0b00000000, 0b00000000
		};

		static constexpr uint8_t Mask3[sizeof(Mask)]
#if defined(ARDUINO_ARCH_AVR)
			PROGMEM
#endif 
		{
			0b00000001, 0b10000000,
			0b00000011, 0b10000000,
			0b00000110, 0b01000000,
			0b00000110, 0b00100000,
			0b00001010, 0b01000000,
			0b00001001, 0b01000000,
			0b00000000, 0b10000000
		};

		static constexpr uint8_t Mask4[sizeof(Mask)]
#if defined(ARDUINO_ARCH_AVR)
			PROGMEM
#endif 
		{
			0b00000100, 0b10000000,
			0b00100000, 0b00100000,
			0b00000000, 0b00000000,
			0b00010000, 0b00001000,
			0b00100001, 0b00010000,
			0b00000000, 0b00000000,
			0b00110001, 0b01100000
		};

		static constexpr uint8_t Mask5[sizeof(Mask)]
#if defined(ARDUINO_ARCH_AVR)
			PROGMEM
#endif 
		{
			0b10000000, 0b00010000,
			0b00000000, 0b00000000,
			0b00000000, 0b01000000,
			0b10000000, 0b00000001,
			0b00000000, 0b00000000,
			0b00000000, 0b00000010,
			0b00000000, 0b00010000
		};
	}

	namespace DogeBit
	{
		static constexpr uint8_t Mask[]
#if defined(ARDUINO_ARCH_AVR)
			PROGMEM
#endif 
		{
			0x10, 0x00, 0x04, 0x00,	0x08, 0x00, 0x00, 0x00,
			0x14, 0x00, 0x14, 0x00, 0x2a, 0x00, 0x6c, 0x00,
			0x15, 0x55, 0x54, 0x00,	0x22, 0x3b, 0xb8, 0x00,
			0x15, 0x55, 0x55, 0x00, 0x20, 0x2e, 0xbf, 0x80,
			0x11, 0x55, 0x55, 0x40,	0x0a, 0xbb, 0xab, 0xf0,
			0x15, 0x55, 0x40, 0x00,	0x60, 0x00, 0x00, 0x00,
			0x54, 0x00, 0x00, 0x20,	0x28, 0x01, 0x20, 0x48,
			0x55, 0x02, 0x40, 0x98,	0x2f, 0x84,	0xb8, 0x38,
			0x55, 0x01, 0x55, 0x54,	0xbb, 0xfb, 0xf8, 0x1c,
			0x55, 0x55, 0x54, 0x1c,	0xae, 0xff, 0xf8, 0x1c,
			0x55, 0x55, 0x55, 0x14,	0xab, 0xbb, 0xa8, 0x3c,
			0x55, 0x55, 0x54, 0x54,	0xaf, 0xee, 0x80, 0x3c,
			0x55, 0x55, 0x55, 0x50,	0x2b, 0xbb, 0xbb, 0xb8,
			0x55, 0x55, 0x55, 0x50,	0x0a, 0xfe, 0xee, 0xe0,
			0x05, 0x55, 0x55, 0x40,	0x03, 0xbb, 0xbb, 0x80,
			0x00, 0x55, 0x55, 0x00,	0x00, 0x0e, 0xaa, 0x00
		};

		static constexpr uint8_t Width = 31;
		static constexpr uint8_t Height = 32;
	}

	namespace Doge565
	{
		static constexpr uint16_t Bitmap[]
#if defined(ARDUINO_ARCH_AVR)
			PROGMEM
#endif 
		{
			0x0000, 0x0000, 0x0000, 0xc52c, 0x4a05, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xb382, 0xd56c, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
			0x0000,	0x0000, 0x0000, 0x8a61, 0xa3c5, 0xac26, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,	0x0000, 0x0000, 0xab21, 0xab61, 0xd54b, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
			0x0000, 0x0000,	0x0000, 0x8a61, 0xa343, 0xa448, 0xc50a, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,	0x0000, 0xab61, 0xab61, 0xd56b, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
			0x0000, 0x0000, 0x9b84, 0x8a61, 0x9b43, 0x9b65, 0xabe5, 0xac26, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xbcec, 0xc423,	0xa301, 0xab61, 0xd4e7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
			0x0000, 0x0000, 0xa342, 0x9b65,	0x82a4, 0x9304, 0xa384, 0xa384, 0xb445, 0x4a06, 0x0000, 0x5247, 0xac8a, 0xc4c8, 0xc4c8, 0xbc45, 0xcc64, 0xcc85, 0xd4c6, 0xd4c6,	0xcc85, 0xb382, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
			0x0000, 0x0000, 0xa2e1, 0x8a61, 0x7223,	0x7223, 0x9b43, 0xa343, 0x9b02, 0xbc24, 0xc443, 0xc443, 0xcc64, 0xd4c6, 0xd4c6, 0xcc64, 0xcc64, 0xcca6, 0xcc85, 0xd528, 0xd4e7, 0xc443, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
			0x0000, 0x0000, 0xa363, 0xa343, 0x4162, 0x4121,	0x9b43, 0xa384, 0x8200, 0x8a40, 0xbc24, 0xc485, 0xcca6, 0xccc7, 0xc4a7, 0xc444, 0xbbc2, 0xb3c3, 0xbc45, 0xc4e9, 0xcd2a, 0xdd8c,	0xdd8b, 0xcce8, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
			0x0000, 0x0000, 0xbc89, 0xa384, 0x5181, 0x30c0, 0x9304,	0xa384, 0x8a61, 0xb3c3, 0xcca6, 0xcca6, 0xc486, 0xc4a7, 0xb466, 0xb445, 0xbc24, 0xb382, 0xb445, 0xcd2a, 0xd56c, 0xe5cd, 0xdd8c,	0xe5ac, 0xc4c9, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
			0x0000, 0x0000, 0x0000, 0xabc4, 0x9304, 0x5181, 0x30c0, 0x61c1,	0xa301, 0xbc86, 0xc4a7, 0xbc45, 0xbca7, 0xb488, 0xb488, 0xb488, 0xb487, 0xbc24, 0xaba2, 0xc4e9, 0xddae, 0xddef, 0xd52a, 0xd52a,	0xe5cd, 0xe5cd, 0xe5ac, 0x0000, 0x0000, 0x0000, 0x0000,
			0x0000, 0x0000, 0x0000, 0x0000, 0xabc4, 0xa384, 0x92c2, 0xb3c3, 0xc4c8,	0xcce8, 0xbc86, 0xb466, 0xb425, 0xac67, 0xc54d, 0xbd4d, 0xcd4b, 0xb446, 0xb3e3, 0xb425, 0xbceb, 0xcd6c, 0xcdae, 0xe5ee, 0xdd8c,	0xe5ed, 0xddad, 0xdd8c, 0x0000, 0x0000, 0x0000,
			0x0000, 0x0000, 0xd52a, 0xab62, 0x0000, 0xab61, 0xc443, 0xd4e7, 0xcd09, 0xb466,	0xc4a7, 0xabc4, 0xabc4, 0xbc87, 0xcd8d, 0xbd0b, 0xd5cf, 0xb425, 0xabc3, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
			0x0000, 0xcd2a, 0xc444, 0x8a40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,	0x0000, 0x0000, 0x0000, 0x0000,
			0x0000, 0xccc7, 0xbbe3, 0x9281, 0xc464, 0xccc7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000,	0x0000, 0x0000, 0x0000,
			0x0000, 0xc485, 0xbbe3, 0xa301, 0xc485, 0xc4a7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,	0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0xc4a7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0xe693,	0x0000, 0x0000,
			0x0000, 0xc485, 0xbbe3, 0xc443, 0xc4a7, 0xcd2b, 0xc52c, 0xc54c, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,	0xffff, 0x0000, 0x0000, 0xb424, 0xcd09, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0xe716, 0xe6f7, 0x0000,	0x0000,
			0x0000, 0xbc45, 0xbc24, 0xc444, 0xb4a9, 0xc54c, 0xcdaf, 0xcdaf, 0xcdcf, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000,	0x0000, 0xbd0b, 0xac04, 0xc4e9, 0xdd8c, 0xd58d, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xe6f5, 0xe6f6, 0xe716, 0x0000, 0x0000,
			0x0000, 0xc4a7, 0xc486, 0xcce8, 0xcd6e, 0xcd6e, 0xcd8e, 0xcdf0, 0xb50d, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xcd0a,	0xd58d, 0xc50a, 0xd54b, 0xddef, 0xd5cf, 0xcdd0, 0x49c4, 0x4206, 0x4206, 0x4206, 0x6b4a, 0xe693, 0xe6f5, 0xde95, 0x0000,
			0xc4e9,	0xccc7, 0xccc7, 0xcd0a, 0xc54d, 0xc56e, 0xcdd0, 0xce11, 0xce11, 0xcdf2, 0xd632, 0xcdf1, 0xbd6f, 0xc5b0, 0xce11, 0xce11, 0xcdae,	0xd5ae, 0xd58d, 0xce11, 0xd633, 0x7bac, 0x1081, 0x0820, 0x18a1, 0x18a2, 0x2923, 0xe717, 0xbd2d, 0xd633, 0x0000,
			0xcd09, 0xbca7,	0xbc66, 0xbc87, 0xbd2d, 0xc56e, 0xc56e, 0xc5af, 0xc5b0, 0xc5d1, 0xce11, 0xcdf1, 0xd633, 0xd633, 0xc5b1, 0xc590, 0xc58f, 0xd58d,	0xcd8d, 0xc590, 0xbdb3, 0x7bac, 0x0840, 0x0840, 0x1081, 0x18a2, 0x6b2a, 0xd654, 0xe6b4, 0xef16, 0x0000,
			0xc4a6, 0xb446, 0xb424,	0xb445, 0xb4c9, 0xbceb, 0xbd2d, 0xb4ec, 0xc56e, 0xcdd0, 0xc5d1, 0xc590, 0xcdf2, 0xc5d1, 0xc590, 0xcdf1, 0xc5d1, 0xbd0b, 0xcd8d,	0xc5d1, 0xa511, 0x946f, 0x5247, 0x18e2, 0x18e2, 0x0860, 0x4206, 0xb550, 0xd653, 0xd633, 0x0000,
			0xc4a8, 0xb445, 0xac04, 0xb446,	0xb4a9, 0xc52c, 0xc56e, 0xbd4e, 0xb4eb, 0xbd2d, 0xbd2e, 0xc56e, 0xb4cb, 0xc5af, 0xbd2e, 0xbd6f, 0xbd4e, 0xbceb, 0xbcca, 0xbd2d,	0xaccd, 0x7b89, 0x3184, 0x3184, 0x3184, 0x39a4, 0x5aa8, 0xbd6f, 0xde94, 0xde73, 0x0000,
			0xbc87, 0xb404, 0xbc66, 0xb425, 0xb4a9,	0xaca9, 0xbd4d, 0xc56e, 0xc56e, 0xb4ec, 0xbd0b, 0xbd2d, 0xbd4d, 0xcd6e, 0xbd2e, 0xbd2e, 0xacaa, 0xb4a9, 0xbcca, 0x9c08, 0x940a,	0x7b89, 0x5247, 0x4206, 0x18e2, 0x18c2, 0x62a6, 0xc58f, 0xcdf1, 0xe6b4, 0x0000,
			0xb445, 0xac04, 0xb3e3, 0xbc66, 0xbca8, 0xc52c,	0xc56e, 0xc58f, 0xcdd0, 0xbd2d, 0xb4ec, 0xc58f, 0xc56e, 0xb4cb, 0xbd0c, 0xb4ec, 0x8ba8, 0x3184, 0x4162, 0x942a, 0xa48b, 0x4162,	0x2102, 0x2923, 0x2102, 0x3143, 0x8ba9, 0xd611, 0xde72, 0xe693, 0x0000,
			0xb445, 0xb424, 0xb3e3, 0xabc4, 0xb466, 0xbcc9, 0xcd8e,	0xcd6e, 0xbd4d, 0xc5af, 0xbd2e, 0xb4ec, 0xc56e, 0xbd4e, 0xb4cb, 0xb4eb, 0xa449, 0x6286, 0x4206, 0x39a4, 0x39a4, 0x2102, 0x2923,	0x3143, 0x39a4, 0x41c4, 0xbd4f, 0xd5f0, 0xde73, 0xe693, 0x0000,
			0xb424, 0xab62, 0xb3e3, 0xb3e3, 0xb445, 0xb445, 0xb4a9, 0xc54d,	0xc52d, 0xc56e, 0xbd2e, 0xc56e, 0xbd2d, 0xbd0b, 0xb4c9, 0xbceb, 0xbd2c, 0xa448, 0xa448, 0x8b66, 0xa448, 0x8ba8, 0x9c08, 0x8b66,	0x7b27, 0xaccb, 0xc56e, 0xde52, 0xe6b3, 0x0000, 0x0000,
			0x0000, 0xaba2, 0xab62, 0xb3c3, 0xb3e3, 0xb424, 0xbca7, 0xb487, 0xbd2c,	0xbd4e, 0xc5b0, 0xbd6f, 0xbd2d, 0xbd2c, 0xbcca, 0xb4cb, 0xaca9, 0xb4a9, 0xb4ca, 0xb4ca, 0xbceb, 0xbceb, 0xc54c, 0xc52c, 0xcdae,	0xcdae, 0xcdae, 0xd5f0, 0xde30, 0x0000, 0x0000,
			0x0000, 0xa363, 0xa321, 0xab21, 0xbc45, 0xb3e3, 0xb487, 0xbcc9, 0xbd0b, 0xc56e,	0xbd4d, 0xc5af, 0xbd2e, 0xc5af, 0xb4cb, 0xac69, 0xaca9, 0xb4ca, 0xb4cb, 0xacaa, 0xb4ca, 0xb4cb, 0xbceb, 0xcd6d, 0xcd6d, 0xcdae,	0xcdae, 0xde11, 0xde0f, 0x0000, 0x0000,
			0x0000, 0x0000, 0x0000, 0xa301, 0xbbe3, 0xb3e3, 0xb445, 0xc4e8, 0xb4c9, 0xb4eb, 0xb4eb,	0xcd8e, 0xbd2e, 0xc58f, 0xc56e, 0xbd0b, 0xb4ca, 0xbceb, 0xb4c9, 0xb4ca, 0xb4a9, 0xc52c, 0xc52b, 0xc50b, 0xcd6d, 0xcd6d, 0xcd8d,	0x9c6d, 0x0000, 0x0000, 0x0000,
			0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xab62, 0xb446, 0xcd0a, 0xb4eb, 0xbd2c, 0xbd0c, 0xb4aa,	0xbd0b, 0xbd0b, 0xbd0c, 0xb4a9, 0xac68, 0xbcc9, 0xb4a9, 0xb4a9, 0xb4a9, 0xbce9, 0xc52b, 0xc54c, 0xcd6d, 0xcd6c, 0x0000, 0x0000,	0x0000, 0x0000, 0x0000,
			0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xbc66, 0xb4a8, 0xbceb, 0xc54c, 0xbd0c, 0xbd0b, 0xac67,	0xb4a9, 0xb4c9, 0xb4a9, 0xb489, 0xb4a9, 0xb488, 0xbcea, 0xbca9, 0xc50b, 0xc52b, 0xc52b, 0xc54c, 0xc50a, 0x0000, 0x0000, 0x0000,	0x0000, 0x0000,
			0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x6286, 0xc50b, 0xb4aa, 0xbd0c, 0xb4a9,	0xac47, 0xa447, 0xac47, 0xac47, 0xbca8, 0xac67, 0xb4a8, 0xbca8, 0xb488, 0xb488, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,	0x0000,
			0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x9c4b, 0xb50c, 0xb4eb, 0xac89,	0xa447, 0xa447, 0xa3c5, 0xac05, 0xa3c5, 0xac25, 0xa3e5, 0xa4ad, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
		};

		static constexpr uint8_t Width = 31;
		static constexpr uint8_t Height = (sizeof(Bitmap) / Width) / sizeof(uint16_t);
	}
}

namespace DemoSprites
{
	struct HeartSprite : public FlashBitMaskSprite<SpriteSource::Heart::Width, SpriteSource::Heart::Height>
	{
		HeartSprite() : FlashBitMaskSprite<SpriteSource::Heart::Width, SpriteSource::Heart::Height>(SpriteSource::Heart::Mask)
		{}
	};

	struct ExplosionSprite : public FlashBitMaskSprite<SpriteSource::Explosion::Width, SpriteSource::Explosion::Height>
	{
		ExplosionSprite() : FlashBitMaskSprite<SpriteSource::Explosion::Width, SpriteSource::Explosion::Height>(SpriteSource::Explosion::Mask)
		{}
	};

	struct ArrowSprite : public FlashBitMaskSprite<SpriteSource::Arrow::Width, SpriteSource::Arrow::Height>
	{
		ArrowSprite() : FlashBitMaskSprite<SpriteSource::Arrow::Width, SpriteSource::Arrow::Height>(SpriteSource::Arrow::Mask)
		{}
	};

	struct PyramidSprite : public FlashBitMaskSprite<SpriteSource::Pyramid::Width, SpriteSource::Pyramid::Height>
	{
		PyramidSprite() : FlashBitMaskSprite<SpriteSource::Pyramid::Width, SpriteSource::Pyramid::Height>(SpriteSource::Pyramid::Mask)
		{}
	};

	struct RainDropSprite : public FlashBitMaskSprite<SpriteSource::RainDrop::Width, SpriteSource::RainDrop::Height>
	{
		RainDropSprite() : FlashBitMaskSprite<SpriteSource::RainDrop::Width, SpriteSource::RainDrop::Height>(SpriteSource::RainDrop::Mask)
		{}
	};

	struct DogeBitSprite : public FlashBitMaskSprite<SpriteSource::DogeBit::Width, SpriteSource::DogeBit::Height>
	{
		DogeBitSprite() : FlashBitMaskSprite<SpriteSource::DogeBit::Width, SpriteSource::DogeBit::Height>(SpriteSource::DogeBit::Mask)
		{}
	};

	struct DogeSprite : public FlashBitmapRgb565Sprite<SpriteSource::Doge565::Width, SpriteSource::Doge565::Height>
	{
		DogeSprite() : FlashBitmapRgb565Sprite<SpriteSource::Doge565::Width, SpriteSource::Doge565::Height>(SpriteSource::Doge565::Bitmap)
		{}
	};
}
#endif

