// SpriteFont5x5.h

#ifndef _SPRITE_FONT_5X5_h
#define _SPRITE_FONT_5X5_h

#include <stdint.h>

#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

#include <Sprite/BitMaskSprite.h>

namespace Font5x5
{
	static constexpr uint8_t Width = 5;
	static constexpr uint8_t Height = 5;

	static constexpr uint8_t LetterA[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b01110000,
		0b10001000,
		0b11111000,
		0b10001000,
		0b10001000
	};

	static constexpr uint8_t LetterB[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11110000,
		0b10001000,
		0b11110000,
		0b10001000,
		0b11110000
	};

	static constexpr uint8_t LetterC[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b01110000,
		0b10001000,
		0b10000000,
		0b10001000,
		0b01110000
	};

	static constexpr uint8_t LetterD[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11110000,
		0b10001000,
		0b10001000,
		0b10001000,
		0b11110000
	};

	static constexpr uint8_t LetterE[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11111000,
		0b10000000,
		0b11100000,
		0b10000000,
		0b11111000
	};

	static constexpr uint8_t LetterF[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11111000,
		0b10000000,
		0b11110000,
		0b10000000,
		0b10000000
	};

	static constexpr uint8_t LetterG[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b01110000,
		0b10000000,
		0b10011000,
		0b10001000,
		0b01110000
	};

	static constexpr uint8_t LetterH[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b10001000,
		0b10001000,
		0b11111000,
		0b10001000,
		0b10001000
	};

	static constexpr uint8_t LetterI[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b01110000,
		0b00100000,
		0b00100000,
		0b00100000,
		0b01110000
	};

	static constexpr uint8_t LetterJ[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b00001000,
		0b00001000,
		0b00001000,
		0b10001000,
		0b01110000
	};

	static constexpr uint8_t LetterK[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b10001000,
		0b10010000,
		0b11100000,
		0b10010000,
		0b10001000
	};

	static constexpr uint8_t LetterL[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b10000000,
		0b10000000,
		0b10000000,
		0b10000000,
		0b11111000
	};

	static constexpr uint8_t LetterM[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b10001000,
		0b11011000,
		0b10101000,
		0b10001000,
		0b10001000
	};

	static constexpr uint8_t LetterN[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b10001000,
		0b11001000,
		0b10101000,
		0b10011000,
		0b10001000
	};

	static constexpr uint8_t LetterO[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b01110000,
		0b10001000,
		0b10001000,
		0b10001000,
		0b01110000
	};

	static constexpr uint8_t LetterP[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11110000,
		0b10001000,
		0b11110000,
		0b10000000,
		0b10000000
	};

	static constexpr uint8_t LetterQ[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b01110000,
		0b10001000,
		0b10001000,
		0b10010000,
		0b01101000
	};

	static constexpr uint8_t LetterR[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11110000,
		0b10001000,
		0b11110000,
		0b10010000,
		0b10001000
	};

	static constexpr uint8_t LetterS[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b01110000,
		0b10000000,
		0b01110000,
		0b00001000,
		0b01110000
	};

	static constexpr uint8_t LetterT[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11111000,
		0b00100000,
		0b00100000,
		0b00100000,
		0b00100000
	};

	static constexpr uint8_t LetterU[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b10001000,
		0b10001000,
		0b10001000,
		0b10001000,
		0b01110000
	};

	static constexpr uint8_t LetterV[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b10001000,
		0b10001000,
		0b10001000,
		0b01010000,
		0b00100000
	};

	static constexpr uint8_t LetterW[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b10001000,
		0b10001000,
		0b10101000,
		0b01110000,
		0b01010000
	};

	static constexpr uint8_t LetterX[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b10001000,
		0b01010000,
		0b00100000,
		0b01010000,
		0b10001000
	};

	static constexpr uint8_t LetterY[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b10001000,
		0b01010000,
		0b00100000,
		0b00100000,
		0b00100000
	};

	static constexpr uint8_t LetterZ[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11111000,
		0b00010000,
		0b00100000,
		0b01000000,
		0b11111000
	};

	static constexpr uint8_t Number0[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b01110000,
		0b10011000,
		0b10101000,
		0b11001000,
		0b01110000
	};


	static constexpr uint8_t Number1[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b01100000,
		0b00100000,
		0b00100000,
		0b00100000,
		0b01110000
	};

	static constexpr uint8_t Number2[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b01110000,
		0b10001000,
		0b00010000,
		0b00100000,
		0b11111000
	};

	static constexpr uint8_t Number3[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b01110000,
		0b10001000,
		0b00010000,
		0b10001000,
		0b01110000
	};

	static constexpr uint8_t Number4[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b00101000,
		0b01001000,
		0b10001000,
		0b11111000,
		0b00001000
	};

	static constexpr uint8_t Number5[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11111000,
		0b10000000,
		0b11110000,
		0b00001000,
		0b11110000
	};

	static constexpr uint8_t Number6[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b01111000,
		0b10000000,
		0b11110000,
		0b10001000,
		0b01110000
	};

	static constexpr uint8_t Number7[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11111000,
		0b00010000,
		0b00100000,
		0b01000000,
		0b10000000
	};

	static constexpr uint8_t Number8[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b01110000,
		0b10001000,
		0b01110000,
		0b10001000,
		0b01110000
	};

	static constexpr uint8_t Number9[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b01110000,
		0b10001000,
		0b01111000,
		0b00001000,
		0b00001000
	};

	static constexpr uint8_t SymbolExclamation[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b00100000,
		0b00100000,
		0b00100000,
		0b00000000,
		0b00100000
	};

	static constexpr uint8_t SymbolQuestion[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b01110000,
		0b10001000,
		0b00110000,
		0b00000000,
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

	static constexpr uint8_t SymbolPercent[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11001000,
		0b11010000,
		0b00100000,
		0b01011000,
		0b10011000
	};

	static constexpr uint8_t SymbolParenthesisOpen[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b00001000,
		0b00010000,
		0b00010000,
		0b00010000,
		0b00001000
	};

	static constexpr uint8_t SymbolParenthesisClose[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b10000000,
		0b01000000,
		0b01000000,
		0b01000000,
		0b10000000
	};

	static constexpr uint8_t SymbolBracketsOpen[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b00011000,
		0b00010000,
		0b00010000,
		0b00010000,
		0b00011000
	};

	static constexpr uint8_t SymbolBracketsClose[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b11000000,
		0b01000000,
		0b01000000,
		0b01000000,
		0b11000000
	};

	static constexpr uint8_t SymbolEquals[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b00000000,
		0b01110000,
		0b00000000,
		0b01110000,
		0b00000000
	};

	static constexpr uint8_t SymbolPlus[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b00100000,
		0b00100000,
		0b11111000,
		0b00100000,
		0b00100000
	};

	static constexpr uint8_t SymbolMinus[Height]
#if defined(ARDUINO_ARCH_AVR)
		PROGMEM
#endif 
	{
		0b00000000,
		0b00000000,
		0b01111000,
		0b00000000,
		0b00000000
	};
};

class SpriteFont5x5Renderer : public AbstractSpriteFontRenderer<FlashBitMaskSprite<Font5x5::Width, Font5x5::Height>>
{
private:
	using SpriteType = FlashBitMaskSprite<Font5x5::Width, Font5x5::Height>;

public:
	SpriteFont5x5Renderer()
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
			return Font5x5::Number0;
		case '1':
			return Font5x5::Number1;
		case '2':
			return Font5x5::Number2;
		case '3':
			return Font5x5::Number3;
		case '4':
			return Font5x5::Number4;
		case '5':
			return Font5x5::Number5;
		case '6':
			return Font5x5::Number6;
		case '7':
			return Font5x5::Number7;
		case '8':
			return Font5x5::Number8;
		case '9':
			return Font5x5::Number9;
		case 'a':
		case 'A':
			return Font5x5::LetterA;
		case 'b':
		case 'B':
			return Font5x5::LetterB;
		case 'c':
		case 'C':
			return Font5x5::LetterC;
		case 'd':
		case 'D':
			return Font5x5::LetterD;
		case 'e':
		case 'E':
			return Font5x5::LetterE;
		case 'f':
		case 'F':
			return Font5x5::LetterF;
		case 'g':
		case 'G':
			return Font5x5::LetterG;
		case 'h':
		case 'H':
			return Font5x5::LetterH;
		case 'i':
		case 'I':
			return Font5x5::LetterI;
		case 'j':
		case 'J':
			return Font5x5::LetterJ;
		case 'k':
		case 'K':
			return Font5x5::LetterK;
		case 'l':
		case 'L':
			return Font5x5::LetterL;
		case 'm':
		case 'M':
			return Font5x5::LetterM;
		case 'n':
		case 'N':
			return Font5x5::LetterN;
		case 'o':
		case 'O':
			return Font5x5::LetterO;
		case 'p':
		case 'P':
			return Font5x5::LetterP;
		case 'q':
		case 'Q':
			return Font5x5::LetterQ;
		case 'r':
		case 'R':
			return Font5x5::LetterR;
		case 's':
		case 'S':
			return Font5x5::LetterS;
		case 't':
		case 'T':
			return Font5x5::LetterT;
		case 'u':
		case 'U':
			return Font5x5::LetterU;
		case 'v':
		case 'V':
			return Font5x5::LetterV;
		case 'w':
		case 'W':
			return Font5x5::LetterW;
		case 'y':
		case 'Y':
			return Font5x5::LetterY;
		case 'x':
		case 'X':
			return Font5x5::LetterX;
		case 'z':
		case 'Z':
			return Font5x5::LetterZ;
		case '!':
			return Font5x5::SymbolExclamation;
		case '?':
			return Font5x5::SymbolQuestion;
		case '.':
			return Font5x5::SymbolDecimalPoint;
		case ',':
			return Font5x5::SymbolDecimalComma;
		case '%':
			return Font5x5::SymbolPercent;
		case '(':
			return Font5x5::SymbolParenthesisOpen;
		case ')':
			return Font5x5::SymbolParenthesisClose;
		case '[':
			return Font5x5::SymbolBracketsOpen;
		case ']':
			return Font5x5::SymbolBracketsClose;
		case '=':
			return Font5x5::SymbolEquals;
		case '+':
			return Font5x5::SymbolPlus;
		case '-':
			return Font5x5::SymbolMinus;
		default:
			return nullptr;
		}
	}
};
#endif