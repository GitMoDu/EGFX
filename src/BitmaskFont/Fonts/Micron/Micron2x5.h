#ifndef _EGFX_BITMASK_FONT_MICRON_2X5_h
#define _EGFX_BITMASK_FONT_MICRON_2X5_h

#include <stdint.h>
#include "../../../Platform/Platform.h"

#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

namespace Egfx
{
	namespace BitmaskFonts
	{
		/// <summary>
		/// 2 pixel wide font. Exploits RGB sub-pixel rendering for increased apparent resolution.
		/// Uppercase-only letters.
		/// </summary>
		namespace Micron
		{
			namespace Source2x5
			{
				static constexpr uint8_t Width = 2;
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

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterA[] PROGMEM
				{
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterB[] PROGMEM
				{
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterC[] PROGMEM
				{
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterD[] PROGMEM
				{
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterE[] PROGMEM
				{
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterF[] PROGMEM
				{
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterG[] PROGMEM
				{
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::IIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterH[] PROGMEM
				{
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterI[] PROGMEM
				{
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterJ[] PROGMEM
				{
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterK[] PROGMEM
				{
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterL[] PROGMEM
				{
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterM[] PROGMEM
				{
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IIO, (uint8_t)OrderRgb::IIO,
					(uint8_t)OrderRgb::IOI, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterN[] PROGMEM
				{
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IIO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOI, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::IIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterO[] PROGMEM
				{
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterP[] PROGMEM
				{
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterQ[] PROGMEM
				{
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::OIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterR[] PROGMEM
				{
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterS[] PROGMEM
				{
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterT[] PROGMEM
				{
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterU[] PROGMEM
				{
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterV[] PROGMEM
				{

					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterW[] PROGMEM
				{
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOI, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::IOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterX[] PROGMEM
				{
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterY[] PROGMEM
				{
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t LetterZ[] PROGMEM
				{
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t Number0[] PROGMEM
				{
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::IIO,
					(uint8_t)OrderRgb::IOI, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IIO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t Number1[] PROGMEM
				{
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t Number2[] PROGMEM
				{
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t Number3[] PROGMEM
				{
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t Number4[] PROGMEM
				{
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t Number5[] PROGMEM
				{
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t Number6[] PROGMEM
				{
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t Number7[] PROGMEM
				{
					(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t Number8[] PROGMEM
				{
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t Number9[] PROGMEM
				{
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IIO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t SymbolExclamation[] PROGMEM
				{
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::IOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t SymbolQuestion[] PROGMEM
				{
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t SymbolDecimalPoint[] PROGMEM
				{
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::IOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t SymbolDecimalComma[] PROGMEM
				{
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t SymbolPercent[] PROGMEM
				{
					(uint8_t)OrderRgb::IIO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::IIO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::IIO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::IIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t SymbolParenthesisOpen[] PROGMEM
				{
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t SymbolParenthesisClose[] PROGMEM
				{
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t SymbolBracketsOpen[] PROGMEM
				{
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IIO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IIO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t SymbolBracketsClose[] PROGMEM
				{
					(uint8_t)OrderRgb::IIO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::IIO, (uint8_t)OrderRgb::OOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t SymbolEquals[] PROGMEM
				{
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t SymbolPlus[] PROGMEM
				{
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t SymbolMinus[] PROGMEM
				{
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t SymbolSmaller[] PROGMEM
				{
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO
				};

				static EGFX_PLATFORM_CONST_DATA uint8_t SymbolBigger[] PROGMEM
				{
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
					(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
					(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO
				};
			}
		}
	}
}
#endif