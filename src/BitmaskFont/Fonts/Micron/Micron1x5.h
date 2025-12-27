#ifndef _EGFX_BITMASK_FONT_MICRON_1X5_h
#define _EGFX_BITMASK_FONT_MICRON_1X5_h

#include <stdint.h>

#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

namespace Egfx
{
	namespace BitmaskFonts
	{
		/// <summary>
		/// 1 pixel wide font. Exploits RGB sub-pixel rendering for increased apparent resolution.
		/// Uppercase-only letters.	
		/// </summary>
		namespace Micron
		{
			namespace Source1x5
			{
				static constexpr uint8_t Width = 1;
				static constexpr uint8_t Height = 5;

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
			}
		}
	}
}
#endif