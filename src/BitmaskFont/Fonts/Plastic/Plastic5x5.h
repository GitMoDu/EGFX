#ifndef _EGFX_BITMASK_FONT_PLASTIC_5X5_h
#define _EGFX_BITMASK_FONT_PLASTIC_5X5_h

#include "../../../Platform/Platform.h"

#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

namespace Egfx
{
	namespace BitmaskFonts
	{
		/// <summary>
		/// Simple wide font for small sizes with clear legibility.
		/// Uppercase-only letters.
		/// </summary>
		namespace Plastic
		{
			namespace Source5x5
			{
				static constexpr uint8_t Width = 5;
				static constexpr uint8_t Height = 5;

				static constexpr uint8_t LetterA[Height] PROGMEM
				{
					0b01110000,
					0b10001000,
					0b11111000,
					0b10001000,
					0b10001000
				};

				static constexpr uint8_t LetterB[Height] PROGMEM
				{
					0b11110000,
					0b10001000,
					0b11110000,
					0b10001000,
					0b11110000
				};

				static constexpr uint8_t LetterC[Height] PROGMEM
				{
					0b01110000,
					0b10001000,
					0b10000000,
					0b10001000,
					0b01110000
				};

				static constexpr uint8_t LetterD[Height] PROGMEM
				{
					0b11110000,
					0b10001000,
					0b10001000,
					0b10001000,
					0b11110000
				};

				static constexpr uint8_t LetterE[Height] PROGMEM
				{
					0b11111000,
					0b10000000,
					0b11100000,
					0b10000000,
					0b11111000
				};

				static constexpr uint8_t LetterF[Height] PROGMEM
				{
					0b11111000,
					0b10000000,
					0b11110000,
					0b10000000,
					0b10000000
				};

				static constexpr uint8_t LetterG[Height] PROGMEM
				{
					0b01110000,
					0b10000000,
					0b10011000,
					0b10001000,
					0b01110000
				};

				static constexpr uint8_t LetterH[Height] PROGMEM
				{
					0b10001000,
					0b10001000,
					0b11111000,
					0b10001000,
					0b10001000
				};

				static constexpr uint8_t LetterI[Height] PROGMEM
				{
					0b01110000,
					0b00100000,
					0b00100000,
					0b00100000,
					0b01110000
				};

				static constexpr uint8_t LetterJ[Height] PROGMEM
				{
					0b00001000,
					0b00001000,
					0b00001000,
					0b10001000,
					0b01110000
				};

				static constexpr uint8_t LetterK[Height] PROGMEM
				{
					0b10001000,
					0b10010000,
					0b11100000,
					0b10010000,
					0b10001000
				};

				static constexpr uint8_t LetterL[Height] PROGMEM
				{
					0b10000000,
					0b10000000,
					0b10000000,
					0b10000000,
					0b11111000
				};

				static constexpr uint8_t LetterM[Height] PROGMEM
				{
					0b10001000,
					0b11011000,
					0b10101000,
					0b10001000,
					0b10001000
				};

				static constexpr uint8_t LetterN[Height] PROGMEM
				{
					0b10001000,
					0b11001000,
					0b10101000,
					0b10011000,
					0b10001000
				};

				static constexpr uint8_t LetterO[Height] PROGMEM
				{
					0b01110000,
					0b10001000,
					0b10001000,
					0b10001000,
					0b01110000
				};

				static constexpr uint8_t LetterP[Height] PROGMEM
				{
					0b11110000,
					0b10001000,
					0b11110000,
					0b10000000,
					0b10000000
				};

				static constexpr uint8_t LetterQ[Height] PROGMEM
				{
					0b01110000,
					0b10001000,
					0b10001000,
					0b10010000,
					0b01101000
				};

				static constexpr uint8_t LetterR[Height] PROGMEM
				{
					0b11110000,
					0b10001000,
					0b11110000,
					0b10010000,
					0b10001000
				};

				static constexpr uint8_t LetterS[Height] PROGMEM
				{
					0b01110000,
					0b10000000,
					0b01110000,
					0b00001000,
					0b01110000
				};

				static constexpr uint8_t LetterT[Height] PROGMEM
				{
					0b11111000,
					0b00100000,
					0b00100000,
					0b00100000,
					0b00100000
				};

				static constexpr uint8_t LetterU[Height] PROGMEM
				{
					0b10001000,
					0b10001000,
					0b10001000,
					0b10001000,
					0b01111000
				};

				static constexpr uint8_t LetterV[Height] PROGMEM
				{
					0b10001000,
					0b10001000,
					0b10001000,
					0b01010000,
					0b00100000
				};

				static constexpr uint8_t LetterW[Height] PROGMEM
				{
					0b10001000,
					0b10001000,
					0b10101000,
					0b01110000,
					0b01010000
				};

				static constexpr uint8_t LetterX[Height] PROGMEM
				{
					0b10001000,
					0b01010000,
					0b00100000,
					0b01010000,
					0b10001000
				};

				static constexpr uint8_t LetterY[Height] PROGMEM
				{
					0b10001000,
					0b01010000,
					0b00100000,
					0b00100000,
					0b00100000
				};

				static constexpr uint8_t LetterZ[Height] PROGMEM
				{
					0b11111000,
					0b00010000,
					0b00100000,
					0b01000000,
					0b11111000
				};

				static constexpr uint8_t Number0[Height] PROGMEM
				{
					0b01110000,
					0b10011000,
					0b10101000,
					0b11001000,
					0b01110000
				};


				static constexpr uint8_t Number1[Height] PROGMEM
				{
					0b01100000,
					0b00100000,
					0b00100000,
					0b00100000,
					0b01110000
				};

				static constexpr uint8_t Number2[Height] PROGMEM
				{
					0b01110000,
					0b10001000,
					0b00010000,
					0b00100000,
					0b11111000
				};

				static constexpr uint8_t Number3[Height] PROGMEM
				{
					0b01110000,
					0b10001000,
					0b00010000,
					0b10001000,
					0b01110000
				};

				static constexpr uint8_t Number4[Height] PROGMEM
				{
					0b00101000,
					0b01001000,
					0b10001000,
					0b11111000,
					0b00001000
				};

				static constexpr uint8_t Number5[Height] PROGMEM
				{
					0b11111000,
					0b10000000,
					0b11110000,
					0b00001000,
					0b11110000
				};

				static constexpr uint8_t Number6[Height] PROGMEM
				{
					0b01111000,
					0b10000000,
					0b11110000,
					0b10001000,
					0b01110000
				};

				static constexpr uint8_t Number7[Height] PROGMEM
				{
					0b11111000,
					0b00010000,
					0b00100000,
					0b01000000,
					0b10000000
				};

				static constexpr uint8_t Number8[Height] PROGMEM
				{
					0b01110000,
					0b10001000,
					0b01110000,
					0b10001000,
					0b01110000
				};

				static constexpr uint8_t Number9[Height] PROGMEM
				{
					0b01110000,
					0b10001000,
					0b01111000,
					0b00001000,
					0b00001000
				};

				static constexpr uint8_t SymbolExclamation[Height] PROGMEM
				{
					0b00100000,
					0b00100000,
					0b00100000,
					0b00000000,
					0b00100000
				};

				static constexpr uint8_t SymbolQuestion[Height] PROGMEM
				{
					0b01110000,
					0b10001000,
					0b00110000,
					0b00000000,
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

				static constexpr uint8_t SymbolPercent[Height] PROGMEM
				{
					0b11001000,
					0b11010000,
					0b00100000,
					0b01011000,
					0b10011000
				};

				static constexpr uint8_t SymbolParenthesisOpen[Height] PROGMEM
				{
					0b00001000,
					0b00010000,
					0b00010000,
					0b00010000,
					0b00001000
				};

				static constexpr uint8_t SymbolParenthesisClose[Height] PROGMEM
				{
					0b10000000,
					0b01000000,
					0b01000000,
					0b01000000,
					0b10000000
				};

				static constexpr uint8_t SymbolBracketsOpen[Height] PROGMEM
				{
					0b00011000,
					0b00010000,
					0b00010000,
					0b00010000,
					0b00011000
				};

				static constexpr uint8_t SymbolBracketsClose[Height] PROGMEM
				{
					0b11000000,
					0b01000000,
					0b01000000,
					0b01000000,
					0b11000000
				};

				static constexpr uint8_t SymbolEquals[Height] PROGMEM
				{
					0b00000000,
					0b01110000,
					0b00000000,
					0b01110000,
					0b00000000
				};

				static constexpr uint8_t SymbolPlus[Height] PROGMEM
				{
					0b00000000,
					0b00100000,
					0b01110000,
					0b00100000,
					0b00000000
				};

				static constexpr uint8_t SymbolMinus[Height] PROGMEM
				{
					0b00000000,
					0b00000000,
					0b01110000,
					0b00000000,
					0b00000000
				};

				static constexpr uint8_t SymbolSmaller[Height] PROGMEM
				{
					0b00010000,
					0b00100000,
					0b01000000,
					0b00100000,
					0b00010000
				};

				static constexpr uint8_t SymbolBigger[Height] PROGMEM
				{
					0b01000000,
					0b00100000,
					0b00010000,
					0b00100000,
					0b01000000
				};
			};
		}
	}
}
#endif