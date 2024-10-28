// SpriteFont2x5.h

#ifndef _SPRITE_FONT_2X5_h
#define _SPRITE_FONT_2X5_h

#include <stdint.h>

#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

#include "../BitmapSprite.h"

namespace Font2x5
{
	static constexpr uint8_t Width = 2;
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

	static constexpr uint8_t LetterA[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO
	};

	static constexpr uint8_t LetterB[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO
	};

	static constexpr uint8_t LetterC[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
	};

	static constexpr uint8_t LetterD[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO
	};

	static constexpr uint8_t LetterE[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO
	};

	static constexpr uint8_t LetterF[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO
	};

	static constexpr uint8_t LetterG[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::IIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
	};

	static constexpr uint8_t LetterH[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO
	};

	static constexpr uint8_t LetterI[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
	};

	static constexpr uint8_t LetterJ[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
	};

	static constexpr uint8_t LetterK[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO
	};

	static constexpr uint8_t LetterL[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO
	};

	static constexpr uint8_t LetterM[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IIO, (uint8_t)OrderRgb::IIO,
		(uint8_t)OrderRgb::IOI, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO
	};

	static constexpr uint8_t LetterN[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IIO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOI, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::IIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO
	};

	static constexpr uint8_t LetterO[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
	};

	static constexpr uint8_t LetterP[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO
	};

	static constexpr uint8_t LetterQ[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::OIO
	};

	static constexpr uint8_t LetterR[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO
	};

	static constexpr uint8_t LetterS[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
	};

	static constexpr uint8_t LetterT[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO
	};

	static constexpr uint8_t LetterU[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IIO
	};

	static constexpr uint8_t LetterV[Width * Height] PROGMEM
	{

		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO
	};

	static constexpr uint8_t LetterW[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOI, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::IOO
	};

	static constexpr uint8_t LetterX[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO
	};

	static constexpr uint8_t LetterY[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO
	};

	static constexpr uint8_t LetterZ[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO
	};

	static constexpr uint8_t Number0[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::IIO,
		(uint8_t)OrderRgb::IOI, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IIO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
	};

	static constexpr uint8_t Number1[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO
	};

	static constexpr uint8_t Number2[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO
	};

	static constexpr uint8_t Number3[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
	};

	static constexpr uint8_t Number4[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO
	};

	static constexpr uint8_t Number5[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO
	};

	static constexpr uint8_t Number6[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
	};

	static constexpr uint8_t Number7[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::III, (uint8_t)OrderRgb::IIO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO
	};

	static constexpr uint8_t Number8[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO
	};

	static constexpr uint8_t Number9[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IIO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO
	};

	static constexpr uint8_t SymbolExclamation[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::IOO
	};

	static constexpr uint8_t SymbolQuestion[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO
	};

	static constexpr uint8_t SymbolDecimalPoint[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::IOO
	};

	static constexpr uint8_t SymbolDecimalComma[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
	};

	static constexpr uint8_t SymbolPercent[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::IIO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::IIO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::IIO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::IIO
	};

	static constexpr uint8_t SymbolParenthesisOpen[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OIO
	};

	static constexpr uint8_t SymbolParenthesisClose[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::IOO, (uint8_t)OrderRgb::OOO
	};

	static constexpr uint8_t SymbolBracketsOpen[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IIO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IIO
	};

	static constexpr uint8_t SymbolBracketsClose[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::IIO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::IIO, (uint8_t)OrderRgb::OOO
	};

	static constexpr uint8_t SymbolEquals[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO
	};

	static constexpr uint8_t SymbolPlus[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO
	};

	static constexpr uint8_t SymbolMinus[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OII, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::OOO
	};

	static constexpr uint8_t SymbolSmaller[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO
	};

	static constexpr uint8_t SymbolBigger[Width * Height] PROGMEM
	{
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OOO, (uint8_t)OrderRgb::IOO,
		(uint8_t)OrderRgb::OOI, (uint8_t)OrderRgb::OOO,
		(uint8_t)OrderRgb::OIO, (uint8_t)OrderRgb::OOO
	};
};

class SpriteFont2x5Renderer : public AbstractSpriteFontRenderer<FlashBitmapRgb332Sprite<Font2x5::Width, Font2x5::Height>, Font2x5::Kerning>
{
public:
	SpriteFont2x5Renderer()
		: AbstractSpriteFontRenderer<FlashBitmapRgb332Sprite<Font2x5::Width, Font2x5::Height>, Font2x5::Kerning>()
	{}

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
			return Font2x5::Number0;
		case '1':
			return Font2x5::Number1;
		case '2':
			return Font2x5::Number2;
		case '3':
			return Font2x5::Number3;
		case '4':
			return Font2x5::Number4;
		case '5':
			return Font2x5::Number5;
		case '6':
			return Font2x5::Number6;
		case '7':
			return Font2x5::Number7;
		case '8':
			return Font2x5::Number8;
		case '9':
			return Font2x5::Number9;
		case 'a':
		case 'A':
			return Font2x5::LetterA;
		case 'b':
		case 'B':
			return Font2x5::LetterB;
		case 'c':
		case 'C':
			return Font2x5::LetterC;
		case 'd':
		case 'D':
			return Font2x5::LetterD;
		case 'e':
		case 'E':
			return Font2x5::LetterE;
		case 'f':
		case 'F':
			return Font2x5::LetterF;
		case 'g':
		case 'G':
			return Font2x5::LetterG;
		case 'h':
		case 'H':
			return Font2x5::LetterH;
		case 'i':
		case 'I':
			return Font2x5::LetterI;
		case 'j':
		case 'J':
			return Font2x5::LetterJ;
		case 'k':
		case 'K':
			return Font2x5::LetterK;
		case 'l':
		case 'L':
			return Font2x5::LetterL;
		case 'm':
		case 'M':
			return Font2x5::LetterM;
		case 'n':
		case 'N':
			return Font2x5::LetterN;
		case 'o':
		case 'O':
			return Font2x5::LetterO;
		case 'p':
		case 'P':
			return Font2x5::LetterP;
		case 'q':
		case 'Q':
			return Font2x5::LetterQ;
		case 'r':
		case 'R':
			return Font2x5::LetterR;
		case 's':
		case 'S':
			return Font2x5::LetterS;
		case 't':
		case 'T':
			return Font2x5::LetterT;
		case 'u':
		case 'U':
			return Font2x5::LetterU;
		case 'v':
		case 'V':
			return Font2x5::LetterV;
		case 'w':
		case 'W':
			return Font2x5::LetterW;
		case 'y':
		case 'Y':
			return Font2x5::LetterY;
		case 'x':
		case 'X':
			return Font2x5::LetterX;
		case 'z':
		case 'Z':
			return Font2x5::LetterZ;
		case '.':
			return Font2x5::SymbolDecimalPoint;
		case ',':
			return Font2x5::SymbolDecimalComma;
		case '=':
			return Font2x5::SymbolEquals;
		case '+':
			return Font2x5::SymbolPlus;
		case '-':
			return Font2x5::SymbolMinus;
		case '!':
			return Font2x5::SymbolExclamation;
		case '?':
			return Font2x5::SymbolQuestion;
		case '%':
			return Font2x5::SymbolPercent;
		case '(':
			return Font2x5::SymbolParenthesisOpen;
		case ')':
			return Font2x5::SymbolParenthesisClose;
		case '[':
			return Font2x5::SymbolBracketsOpen;
		case ']':
			return Font2x5::SymbolBracketsClose;
		case '<':
			return Font2x5::SymbolSmaller;
		case '>':
			return Font2x5::SymbolBigger;
		default:
			return nullptr;
		}
	}
};
#endif