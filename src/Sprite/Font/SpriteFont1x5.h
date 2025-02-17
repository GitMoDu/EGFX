// SpriteFont1x5.h

#ifndef _EGFX_SPRITE_FONT_1X5_h
#define _EGFX_SPRITE_FONT_1X5_h

#include <stdint.h>

#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

#include "../BitmapSprite.h"

namespace Egfx
{
	namespace Font1x5
	{
		static constexpr uint8_t Width = 1;
		static constexpr uint8_t Height = 5;
		static constexpr uint8_t Kerning = 1;

		/// <summary>
		/// All possible sub-pixel masks in RGB 3:3:2 color space.
		/// </summary>
		enum class SubPixels : uint8_t
		{
			Red = 0xE0,
			Green = 0x1C,
			Blue = 6
		};

		/// <summary>
		/// Typefied sub-pixel line definition.
		/// </summary>
		enum class OrderRgb : uint8_t
		{
			OOO = 0,
			OOI = (uint8_t)SubPixels::Blue,
			OIO = (uint8_t)SubPixels::Green,
			OII = (uint8_t)SubPixels::Green | (uint8_t)SubPixels::Blue,
			IOO = (uint8_t)SubPixels::Red,
			IOI = (uint8_t)SubPixels::Red | (uint8_t)SubPixels::Blue,
			IIO = (uint8_t)SubPixels::Red | (uint8_t)SubPixels::Green,
			III = (uint8_t)SubPixels::Red | (uint8_t)SubPixels::Green | (uint8_t)SubPixels::Blue
		};

		static constexpr uint8_t LetterA[Height] PROGMEM
		{
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
		};

		static constexpr uint8_t LetterB[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IIO,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IIO,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IIO
		};

		static constexpr uint8_t LetterC[Height] PROGMEM
		{
			(uint8_t)OrderRgb::OII,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::OOI
		};

		static constexpr uint8_t LetterD[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IIO,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IIO
		};

		static constexpr uint8_t LetterE[Height] PROGMEM
		{
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::IIO,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::III
		};

		static constexpr uint8_t LetterF[Height] PROGMEM
		{
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::IIO,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::IOO
		};

		static constexpr uint8_t LetterG[Height] PROGMEM
		{
			(uint8_t)OrderRgb::OII,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::OIO
		};

		static constexpr uint8_t LetterH[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI
		};

		static constexpr uint8_t LetterI[Height] PROGMEM
		{
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::III
		};

		static constexpr uint8_t LetterJ[Height] PROGMEM
		{
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::OIO
		};

		static constexpr uint8_t LetterK[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IIO,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI
		};

		static constexpr uint8_t LetterL[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::III
		};

		static constexpr uint8_t LetterM[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI
		};

		static constexpr uint8_t LetterN[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::IOI
		};

		static constexpr uint8_t LetterO[Height] PROGMEM
		{
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::OIO
		};

		static constexpr uint8_t LetterP[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IIO,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IIO,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::IOO
		};

		static constexpr uint8_t LetterQ[Height] PROGMEM
		{
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::OII
		};

		static constexpr uint8_t LetterR[Height] PROGMEM
		{
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IIO,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI
		};

		static constexpr uint8_t LetterS[Height] PROGMEM
		{
			(uint8_t)OrderRgb::OII,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::IIO
		};

		static constexpr uint8_t LetterT[Height] PROGMEM
		{
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::OIO
		};

		static constexpr uint8_t LetterU[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::OII
		};

		static constexpr uint8_t LetterV[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::OIO
		};

		static constexpr uint8_t LetterW[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::OIO
		};

		static constexpr uint8_t LetterX[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI
		};

		static constexpr uint8_t LetterY[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::OIO
		};

		static constexpr uint8_t LetterZ[Height] PROGMEM
		{
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::III
		};

		static constexpr uint8_t Number0[Height] PROGMEM
		{
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::III
		};

		static constexpr uint8_t Number1[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IIO,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::III
		};

		static constexpr uint8_t Number2[Height] PROGMEM
		{
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::III
		};

		static constexpr uint8_t Number3[Height] PROGMEM
		{
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::III
		};

		static constexpr uint8_t Number4[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::OOI
		};

		static constexpr uint8_t Number5[Height] PROGMEM
		{
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::III
		};

		static constexpr uint8_t Number6[Height] PROGMEM
		{
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::III
		};

		static constexpr uint8_t Number7[Height] PROGMEM
		{
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::OOI
		};

		static constexpr uint8_t Number8[Height] PROGMEM
		{
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::III
		};

		static constexpr uint8_t Number9[Height] PROGMEM
		{
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::OOI,
		};

		static constexpr uint8_t SymbolDecimalPoint[Height] PROGMEM
		{
			(uint8_t)OrderRgb::OOO,
			(uint8_t)OrderRgb::OOO,
			(uint8_t)OrderRgb::OOO,
			(uint8_t)OrderRgb::OOO,
			(uint8_t)OrderRgb::OIO
		};

		static constexpr uint8_t SymbolDecimalComma[Height] PROGMEM
		{
			(uint8_t)OrderRgb::OOO,
			(uint8_t)OrderRgb::OOO,
			(uint8_t)OrderRgb::OOO,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::IOO
		};

		static constexpr uint8_t SymbolPlus[Height] PROGMEM
		{
			(uint8_t)OrderRgb::OOO,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::OOO
		};

		static constexpr uint8_t SymbolMinus[Height] PROGMEM
		{
			(uint8_t)OrderRgb::OOO,
			(uint8_t)OrderRgb::OOO,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::OOO,
			(uint8_t)OrderRgb::OOO
		};

		static constexpr uint8_t SymbolEquals[Height] PROGMEM
		{
			(uint8_t)OrderRgb::OOO,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::OOO,
			(uint8_t)OrderRgb::III,
			(uint8_t)OrderRgb::OOO
		};

		static constexpr uint8_t SymbolExclamation[Height] PROGMEM
		{
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::OOO,
			(uint8_t)OrderRgb::OIO
		};

		static constexpr uint8_t SymbolQuestion[Height] PROGMEM
		{
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::IOI,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::OOO,
			(uint8_t)OrderRgb::OIO,

		};

		static constexpr uint8_t SymbolPercent[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::OOI
		};

		static constexpr uint8_t SymbolParenthesisOpen[Height] PROGMEM
		{
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::OIO
		};

		static constexpr uint8_t SymbolParenthesisClose[Height] PROGMEM
		{
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::OIO
		};

		static constexpr uint8_t SymbolBracketsOpen[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IIO,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::IIO
		};

		static constexpr uint8_t SymbolBracketsClose[Height] PROGMEM
		{
			(uint8_t)OrderRgb::OII,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::OII
		};

		static constexpr uint8_t SymbolSmaller[Height] PROGMEM
		{
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::OOI
		};

		static constexpr uint8_t SymbolBigger[Height] PROGMEM
		{
			(uint8_t)OrderRgb::IOO,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::OOI,
			(uint8_t)OrderRgb::OIO,
			(uint8_t)OrderRgb::IOO,
		};
	};

	class SpriteFont1x5Renderer : public AbstractSpriteFontRenderer<FlashBitmapRgb332Sprite<Font1x5::Width, Font1x5::Height>, Font1x5::Kerning>
	{
	public:
		SpriteFont1x5Renderer()
			: AbstractSpriteFontRenderer<FlashBitmapRgb332Sprite<Font1x5::Width, Font1x5::Height>, Font1x5::Kerning>()
		{
		}

	protected:
		void SetCharacter(const int8_t character) final
		{
			SetBitmap(GetBitmap(character));
		}

	private:
		const uint8_t* GetBitmap(const int8_t character) const
		{
			switch (character)
			{
			case '0':
				return Font1x5::Number0;
			case '1':
				return Font1x5::Number1;
			case '2':
				return Font1x5::Number2;
			case '3':
				return Font1x5::Number3;
			case '4':
				return Font1x5::Number4;
			case '5':
				return Font1x5::Number5;
			case '6':
				return Font1x5::Number6;
			case '7':
				return Font1x5::Number7;
			case '8':
				return Font1x5::Number8;
			case '9':
				return Font1x5::Number9;
			case 'a':
			case 'A':
				return Font1x5::LetterA;
			case 'b':
			case 'B':
				return Font1x5::LetterB;
			case 'c':
			case 'C':
				return Font1x5::LetterC;
			case 'd':
			case 'D':
				return Font1x5::LetterD;
			case 'e':
			case 'E':
				return Font1x5::LetterE;
			case 'f':
			case 'F':
				return Font1x5::LetterF;
			case 'g':
			case 'G':
				return Font1x5::LetterG;
			case 'h':
			case 'H':
				return Font1x5::LetterH;
			case 'i':
			case 'I':
				return Font1x5::LetterI;
			case 'j':
			case 'J':
				return Font1x5::LetterJ;
			case 'k':
			case 'K':
				return Font1x5::LetterK;
			case 'l':
			case 'L':
				return Font1x5::LetterL;
			case 'm':
			case 'M':
				return Font1x5::LetterM;
			case 'n':
			case 'N':
				return Font1x5::LetterN;
			case 'o':
			case 'O':
				return Font1x5::LetterO;
			case 'p':
			case 'P':
				return Font1x5::LetterP;
			case 'q':
			case 'Q':
				return Font1x5::LetterQ;
			case 'r':
			case 'R':
				return Font1x5::LetterR;
			case 's':
			case 'S':
				return Font1x5::LetterS;
			case 't':
			case 'T':
				return Font1x5::LetterT;
			case 'u':
			case 'U':
				return Font1x5::LetterU;
			case 'v':
			case 'V':
				return Font1x5::LetterV;
			case 'w':
			case 'W':
				return Font1x5::LetterW;
			case 'y':
			case 'Y':
				return Font1x5::LetterY;
			case 'x':
			case 'X':
				return Font1x5::LetterX;
			case 'z':
			case 'Z':
				return Font1x5::LetterZ;
			case '.':
				return Font1x5::SymbolDecimalPoint;
			case ',':
				return Font1x5::SymbolDecimalComma;
			case '=':
				return Font1x5::SymbolEquals;
			case '+':
				return Font1x5::SymbolPlus;
			case '-':
				return Font1x5::SymbolMinus;
			case '!':
				return Font1x5::SymbolExclamation;
			case '?':
				return Font1x5::SymbolQuestion;
			case '%':
				return Font1x5::SymbolPercent;
			case '(':
				return Font1x5::SymbolParenthesisOpen;
			case ')':
				return Font1x5::SymbolParenthesisClose;
			case '[':
				return Font1x5::SymbolBracketsOpen;
			case ']':
				return Font1x5::SymbolBracketsClose;
			case '<':
				return Font1x5::SymbolSmaller;
			case '>':
				return Font1x5::SymbolBigger;
			default:
				return nullptr;
			}
		}
	};
}
#endif