// SpriteFont3x5.h

#ifndef _SPRITE_FONT_3X5_h
#define _SPRITE_FONT_3X5_h

#include <stdint.h>

#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

#include <Sprite/BitMaskSprite.h>

namespace Font3x5
{
	static constexpr uint8_t Width = 3;
	static constexpr uint8_t Height = 5;

	static constexpr uint8_t Number0[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11100000,
		0b10100000,
		0b10100000,
		0b10100000,
		0b11100000
	};


	static constexpr uint8_t Number1[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11000000,
		0b01000000,
		0b01000000,
		0b01000000,
		0b11100000
	};

	static constexpr uint8_t Number2[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11100000,
		0b00100000,
		0b01000000,
		0b10000000,
		0b11100000
	};

	static constexpr uint8_t Number3[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11100000,
		0b00100000,
		0b01100000,
		0b00100000,
		0b11100000
	};

	static constexpr uint8_t Number4[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b10100000,
		0b10100000,
		0b11100000,
		0b00100000,
		0b00100000
	};

	static constexpr uint8_t Number5[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11100000,
		0b10000000,
		0b11000000,
		0b00100000,
		0b11000000
	};

	static constexpr uint8_t Number6[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b01100000,
		0b10000000,
		0b11100000,
		0b10100000,
		0b11100000
	};

	static constexpr uint8_t Number7[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11100000,
		0b00100000,
		0b01000000,
		0b01000000,
		0b10000000
	};

	static constexpr uint8_t Number8[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11100000,
		0b10100000,
		0b11100000,
		0b10100000,
		0b11100000
	};

	static constexpr uint8_t Number9[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11100000,
		0b10100000,
		0b11100000,
		0b00100000,
		0b00100000
	};

	static constexpr uint8_t SymbolDecimalPoint[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b01000000
	};

	static constexpr uint8_t SymbolDecimalComma[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b00000000,
		0b00000000,
		0b00000000,
		0b01000000,
		0b10000000
	};

	static constexpr uint8_t SymbolPlus[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b00000000,
		0b01000000,
		0b11100000,
		0b01000000,
		0b00000000
	};

	static constexpr uint8_t SymbolMinus[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b00000000,
		0b00000000,
		0b11100000,
		0b00000000,
		0b00000000
	};

	static constexpr uint8_t SymbolEquals[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b00000000,
		0b11100000,
		0b00000000,
		0b11100000,
		0b00000000
	};
};

class SpriteFont3x5Renderer : public AbstractSpriteFontRenderer<FlashBitMaskSprite<Font3x5::Width, Font3x5::Height>>
{
private:
	using SpriteType = FlashBitMaskSprite<Font3x5::Width, Font3x5::Height>;

public:
	SpriteFont3x5Renderer()
		: AbstractSpriteFontRenderer<SpriteType>()
	{}

protected:
	void SetCharacter(SpriteType& spriteSource, const int8_t character) final
	{
		spriteSource.SetMask(GetMask(character));
	}

private:
	const uint8_t* GetMask(const int8_t character) const
	{
		switch (character)
		{
		case '0':
			return Font3x5::Number0;
		case '1':
			return Font3x5::Number1;
		case '2':
			return Font3x5::Number2;
		case '3':
			return Font3x5::Number3;
		case '4':
			return Font3x5::Number4;
		case '5':
			return Font3x5::Number5;
		case '6':
			return Font3x5::Number6;
		case '7':
			return Font3x5::Number7;
		case '8':
			return Font3x5::Number8;
		case '9':
			return Font3x5::Number9;
		case '.':
			return Font3x5::SymbolDecimalPoint;
		case ',':
			return Font3x5::SymbolDecimalComma;
		case '=':
			return Font3x5::SymbolEquals;
		case '+':
			return Font3x5::SymbolPlus;
		case '-':
			return Font3x5::SymbolMinus;
		default:
			return nullptr;
		}
	}
};
#endif