#ifndef _EGFX_SPRITE_FONT_3X5_h
#define _EGFX_SPRITE_FONT_3X5_h

#include <stdint.h>

#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

#include "Sprite/BitMaskSprite.h"
#include "GraphicsRender/SpriteFontRenderer.h"

namespace Egfx
{
	namespace Font3x5
	{
		static constexpr uint8_t Width = 3;
		static constexpr uint8_t Height = 5;
		static constexpr uint8_t Kerning = 1;

		static constexpr uint8_t LetterA[Height] PROGMEM
		{
			0b01000000,
			0b10100000,
			0b11100000,
			0b10100000,
			0b10100000
		};

		static constexpr uint8_t LetterB[Height] PROGMEM
		{
			0b11000000,
			0b10100000,
			0b11000000,
			0b10100000,
			0b11000000
		};

		static constexpr uint8_t LetterC[Height] PROGMEM
		{
			0b01100000,
			0b10000000,
			0b10000000,
			0b10000000,
			0b01100000
		};

		static constexpr uint8_t LetterD[Height] PROGMEM
		{
			0b11000000,
			0b10100000,
			0b10100000,
			0b10100000,
			0b11000000
		};

		static constexpr uint8_t LetterE[Height] PROGMEM
		{
			0b11100000,
			0b10000000,
			0b11000000,
			0b10000000,
			0b11100000
		};

		static constexpr uint8_t LetterF[Height] PROGMEM
		{
			0b11100000,
			0b10000000,
			0b11000000,
			0b10000000,
			0b10000000
		};

		static constexpr uint8_t LetterG[Height] PROGMEM
		{
			0b01100000,
			0b10000000,
			0b10100000,
			0b10100000,
			0b01000000
		};

		static constexpr uint8_t LetterH[Height] PROGMEM
		{
			0b10100000,
			0b10100000,
			0b11100000,
			0b10100000,
			0b10100000
		};

		static constexpr uint8_t LetterI[Height] PROGMEM
		{
			0b11100000,
			0b01000000,
			0b01000000,
			0b01000000,
			0b11100000
		};

		static constexpr uint8_t LetterJ[Height] PROGMEM
		{
			0b11100000,
			0b00100000,
			0b00100000,
			0b10100000,
			0b01000000
		};

		static constexpr uint8_t LetterK[Height] PROGMEM
		{
			0b10100000,
			0b10100000,
			0b11000000,
			0b10100000,
			0b10100000
		};

		static constexpr uint8_t LetterL[Height] PROGMEM
		{
			0b10000000,
			0b10000000,
			0b10000000,
			0b10000000,
			0b11100000
		};

		static constexpr uint8_t LetterM[Height] PROGMEM
		{
			0b10100000,
			0b11100000,
			0b11100000,
			0b10100000,
			0b10100000
		};

		static constexpr uint8_t LetterN[Height] PROGMEM
		{
			0b10100000,
			0b10100000,
			0b10100000,
			0b11100000,
			0b10100000
		};

		static constexpr uint8_t LetterO[Height] PROGMEM
		{
			0b01000000,
			0b10100000,
			0b10100000,
			0b10100000,
			0b01000000
		};

		static constexpr uint8_t LetterP[Height] PROGMEM
		{
			0b11000000,
			0b10100000,
			0b11000000,
			0b10000000,
			0b10000000
		};

		static constexpr uint8_t LetterQ[Height] PROGMEM
		{
			0b01000000,
			0b10100000,
			0b10100000,
			0b11100000,
			0b01100000
		};

		static constexpr uint8_t LetterR[Height] PROGMEM
		{
			0b11000000,
			0b10100000,
			0b11000000,
			0b10100000,
			0b10100000
		};

		static constexpr uint8_t LetterS[Height] PROGMEM
		{
			0b01100000,
			0b10000000,
			0b01000000,
			0b00100000,
			0b11000000
		};

		static constexpr uint8_t LetterT[Height] PROGMEM
		{
			0b11100000,
			0b01000000,
			0b01000000,
			0b01000000,
			0b01000000
		};

		static constexpr uint8_t LetterU[Height] PROGMEM
		{
			0b10100000,
			0b10100000,
			0b10100000,
			0b10100000,
			0b01100000
		};

		static constexpr uint8_t LetterV[Height] PROGMEM
		{
			0b10100000,
			0b10100000,
			0b10100000,
			0b10100000,
			0b01000000
		};

		static constexpr uint8_t LetterW[Height] PROGMEM
		{
			0b10100000,
			0b10100000,
			0b11100000,
			0b11100000,
			0b01000000
		};

		static constexpr uint8_t LetterX[Height] PROGMEM
		{
			0b10100000,
			0b10100000,
			0b01000000,
			0b10100000,
			0b10100000
		};

		static constexpr uint8_t LetterY[Height] PROGMEM
		{
			0b10100000,
			0b10100000,
			0b01000000,
			0b01000000,
			0b01000000
		};

		static constexpr uint8_t LetterZ[Height] PROGMEM
		{
			0b11100000,
			0b00100000,
			0b01000000,
			0b10000000,
			0b11100000
		};

		static constexpr uint8_t Number0[Height] PROGMEM
		{
			0b11100000,
			0b10100000,
			0b10100000,
			0b10100000,
			0b11100000
		};

		static constexpr uint8_t Number1[Height] PROGMEM
		{
			0b11000000,
			0b01000000,
			0b01000000,
			0b01000000,
			0b11100000
		};

		static constexpr uint8_t Number2[Height] PROGMEM
		{
			0b11100000,
			0b00100000,
			0b01100000,
			0b10000000,
			0b11100000
		};

		static constexpr uint8_t Number3[Height] PROGMEM
		{
			0b11100000,
			0b00100000,
			0b01100000,
			0b00100000,
			0b11100000
		};

		static constexpr uint8_t Number4[Height] PROGMEM
		{
			0b10100000,
			0b10100000,
			0b11100000,
			0b00100000,
			0b00100000
		};

		static constexpr uint8_t Number5[Height] PROGMEM
		{
			0b11100000,
			0b10000000,
			0b11000000,
			0b00100000,
			0b11000000
		};

		static constexpr uint8_t Number6[Height] PROGMEM
		{
			0b01100000,
			0b10000000,
			0b11100000,
			0b10100000,
			0b11100000
		};

		static constexpr uint8_t Number7[Height] PROGMEM
		{
			0b11100000,
			0b00100000,
			0b01000000,
			0b01000000,
			0b10000000
		};

		static constexpr uint8_t Number8[Height] PROGMEM
		{
			0b11100000,
			0b10100000,
			0b11100000,
			0b10100000,
			0b11100000
		};

		static constexpr uint8_t Number9[Height] PROGMEM
		{
			0b11100000,
			0b10100000,
			0b11100000,
			0b00100000,
			0b00100000
		};

		static constexpr uint8_t SymbolDecimalPoint[Height] PROGMEM
		{
			0b00000000,
			0b00000000,
			0b00000000,
			0b00000000,
			0b01000000
		};

		static constexpr uint8_t SymbolDecimalComma[Height] PROGMEM
		{
			0b00000000,
			0b00000000,
			0b00000000,
			0b01000000,
			0b10000000
		};

		static constexpr uint8_t SymbolPlus[Height] PROGMEM
		{
			0b00000000,
			0b01000000,
			0b11100000,
			0b01000000,
			0b00000000
		};

		static constexpr uint8_t SymbolMinus[Height] PROGMEM
		{
			0b00000000,
			0b00000000,
			0b11100000,
			0b00000000,
			0b00000000
		};

		static constexpr uint8_t SymbolEquals[Height] PROGMEM
		{
			0b00000000,
			0b11100000,
			0b00000000,
			0b11100000,
			0b00000000
		};

		static constexpr uint8_t SymbolExclamation[Height] PROGMEM
		{
			0b01000000,
			0b01000000,
			0b01000000,
			0b00000000,
			0b01000000
		};
		static constexpr uint8_t SymbolQuestion[Height] PROGMEM
		{
			0b01000000,
			0b10100000,
			0b00100000,
			0b00000000,
			0b01000000
		};

		static constexpr uint8_t SymbolPercent[Height] PROGMEM
		{
			0b10000000,
			0b00100000,
			0b01000000,
			0b10000000,
			0b00100000
		};

		static constexpr uint8_t SymbolParenthesisOpen[Height] PROGMEM
		{
			0b01000000,
			0b10000000,
			0b10000000,
			0b10000000,
			0b01000000
		};

		static constexpr uint8_t SymbolParenthesisClose[Height] PROGMEM
		{
			0b01000000,
			0b00100000,
			0b00100000,
			0b00100000,
			0b01000000
		};

		static constexpr uint8_t SymbolBracketsOpen[Height] PROGMEM
		{
			0b11000000,
			0b10000000,
			0b10000000,
			0b10000000,
			0b11000000
		};

		static constexpr uint8_t SymbolBracketsClose[Height] PROGMEM
		{
			0b01100000,
			0b00100000,
			0b00100000,
			0b00100000,
			0b01100000
		};

		static constexpr uint8_t SymbolSmaller[Height] PROGMEM
		{
			0b00100000,
			0b01000000,
			0b10000000,
			0b01000000,
			0b00100000
		};

		static constexpr uint8_t SymbolBigger[Height] PROGMEM
		{
			0b10000000,
			0b01000000,
			0b00100000,
			0b01000000,
			0b10000000
		};
	};

	class SpriteFont3x5Renderer : public AbstractSpriteFontRenderer<FlashBitMaskSprite<Font3x5::Width, Font3x5::Height>, Font3x5::Kerning>
	{
	public:
		SpriteFont3x5Renderer()
			: AbstractSpriteFontRenderer<FlashBitMaskSprite<Font3x5::Width, Font3x5::Height>, Font3x5::Kerning>()
		{
		}

	protected:
		void SetCharacter(const int8_t character) final
		{
			SetMask(GetMask(character));
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
			case 'a':
			case 'A':
				return Font3x5::LetterA;
			case 'b':
			case 'B':
				return Font3x5::LetterB;
			case 'c':
			case 'C':
				return Font3x5::LetterC;
			case 'd':
			case 'D':
				return Font3x5::LetterD;
			case 'e':
			case 'E':
				return Font3x5::LetterE;
			case 'f':
			case 'F':
				return Font3x5::LetterF;
			case 'g':
			case 'G':
				return Font3x5::LetterG;
			case 'h':
			case 'H':
				return Font3x5::LetterH;
			case 'i':
			case 'I':
				return Font3x5::LetterI;
			case 'j':
			case 'J':
				return Font3x5::LetterJ;
			case 'k':
			case 'K':
				return Font3x5::LetterK;
			case 'l':
			case 'L':
				return Font3x5::LetterL;
			case 'm':
			case 'M':
				return Font3x5::LetterM;
			case 'n':
			case 'N':
				return Font3x5::LetterN;
			case 'o':
			case 'O':
				return Font3x5::LetterO;
			case 'p':
			case 'P':
				return Font3x5::LetterP;
			case 'q':
			case 'Q':
				return Font3x5::LetterQ;
			case 'r':
			case 'R':
				return Font3x5::LetterR;
			case 's':
			case 'S':
				return Font3x5::LetterS;
			case 't':
			case 'T':
				return Font3x5::LetterT;
			case 'u':
			case 'U':
				return Font3x5::LetterU;
			case 'v':
			case 'V':
				return Font3x5::LetterV;
			case 'w':
			case 'W':
				return Font3x5::LetterW;
			case 'y':
			case 'Y':
				return Font3x5::LetterY;
			case 'x':
			case 'X':
				return Font3x5::LetterX;
			case 'z':
			case 'Z':
				return Font3x5::LetterZ;
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
			case '!':
				return Font3x5::SymbolExclamation;
			case '?':
				return Font3x5::SymbolQuestion;
			case '%':
				return Font3x5::SymbolPercent;
			case '(':
				return Font3x5::SymbolParenthesisOpen;
			case ')':
				return Font3x5::SymbolParenthesisClose;
			case '[':
				return Font3x5::SymbolBracketsOpen;
			case ']':
				return Font3x5::SymbolBracketsClose;
			case '<':
				return Font3x5::SymbolSmaller;
			case '>':
				return Font3x5::SymbolBigger;
			default:
				return nullptr;
			}
		}
	};
}
#endif