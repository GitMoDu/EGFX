#ifndef _EGFX_FRAMEWORK_ASSETS_FONT_BITMASK_PLASTIC_h
#define _EGFX_FRAMEWORK_ASSETS_FONT_BITMASK_PLASTIC_h

#include <EgfxFramework.h>
#include "Plastic3x5.h"
#include "Plastic5x5.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace Font
			{
				namespace Bitmask
				{
					/// <summary>
					/// Simple font for small sizes with clear legibility.
					/// Uppercase-only letters.
					/// </summary>
					namespace Plastic
					{
						namespace AsciiDefinition = Framework::AsciiDefinition;

						struct FontType3x5
						{
							using dimension_t = uint8_t;
							static constexpr dimension_t Width = Source3x5::Width;
							static constexpr dimension_t Height = Source3x5::Height;

							using ReaderType = Framework::Bitmask::Reader::Flash;

							static const uint8_t* GetBitmask(const char printableChar)
							{
								switch (static_cast<uint8_t>(printableChar))
								{
								case uint8_t(AsciiDefinition::Printable::a):
								case uint8_t(AsciiDefinition::Printable::A):
									return Source3x5::LetterA;
								case uint8_t(AsciiDefinition::Printable::b):
								case uint8_t(AsciiDefinition::Printable::B):
									return Source3x5::LetterB;
								case uint8_t(AsciiDefinition::Printable::c):
								case uint8_t(AsciiDefinition::Printable::C):
									return Source3x5::LetterC;
								case uint8_t(AsciiDefinition::Printable::d):
								case uint8_t(AsciiDefinition::Printable::D):
									return Source3x5::LetterD;
								case uint8_t(AsciiDefinition::Printable::e):
								case uint8_t(AsciiDefinition::Printable::E):
									return Source3x5::LetterE;
								case uint8_t(AsciiDefinition::Printable::f):
								case uint8_t(AsciiDefinition::Printable::F):
									return Source3x5::LetterF;
								case uint8_t(AsciiDefinition::Printable::g):
								case uint8_t(AsciiDefinition::Printable::G):
									return Source3x5::LetterG;
								case uint8_t(AsciiDefinition::Printable::h):
								case uint8_t(AsciiDefinition::Printable::H):
									return Source3x5::LetterH;
								case uint8_t(AsciiDefinition::Printable::i):
								case uint8_t(AsciiDefinition::Printable::I):
									return Source3x5::LetterI;
								case uint8_t(AsciiDefinition::Printable::j):
								case uint8_t(AsciiDefinition::Printable::J):
									return Source3x5::LetterJ;
								case uint8_t(AsciiDefinition::Printable::k):
								case uint8_t(AsciiDefinition::Printable::K):
									return Source3x5::LetterK;
								case uint8_t(AsciiDefinition::Printable::l):
								case uint8_t(AsciiDefinition::Printable::L):
									return Source3x5::LetterL;
								case uint8_t(AsciiDefinition::Printable::m):
								case uint8_t(AsciiDefinition::Printable::M):
									return Source3x5::LetterM;
								case uint8_t(AsciiDefinition::Printable::n):
								case uint8_t(AsciiDefinition::Printable::N):
									return Source3x5::LetterN;
								case uint8_t(AsciiDefinition::Printable::o):
								case uint8_t(AsciiDefinition::Printable::O):
									return Source3x5::LetterO;
								case uint8_t(AsciiDefinition::Printable::p):
								case uint8_t(AsciiDefinition::Printable::P):
									return Source3x5::LetterP;
								case uint8_t(AsciiDefinition::Printable::q):
								case uint8_t(AsciiDefinition::Printable::Q):
									return Source3x5::LetterQ;
								case uint8_t(AsciiDefinition::Printable::r):
								case uint8_t(AsciiDefinition::Printable::R):
									return Source3x5::LetterR;
								case uint8_t(AsciiDefinition::Printable::s):
								case uint8_t(AsciiDefinition::Printable::S):
									return Source3x5::LetterS;
								case uint8_t(AsciiDefinition::Printable::t):
								case uint8_t(AsciiDefinition::Printable::T):
									return Source3x5::LetterT;
								case uint8_t(AsciiDefinition::Printable::u):
								case uint8_t(AsciiDefinition::Printable::U):
									return Source3x5::LetterU;
								case uint8_t(AsciiDefinition::Printable::v):
								case uint8_t(AsciiDefinition::Printable::V):
									return Source3x5::LetterV;
								case uint8_t(AsciiDefinition::Printable::w):
								case uint8_t(AsciiDefinition::Printable::W):
									return Source3x5::LetterW;
								case uint8_t(AsciiDefinition::Printable::x):
								case uint8_t(AsciiDefinition::Printable::X):
									return Source3x5::LetterX;
								case uint8_t(AsciiDefinition::Printable::y):
								case uint8_t(AsciiDefinition::Printable::Y):
									return Source3x5::LetterY;
								case uint8_t(AsciiDefinition::Printable::z):
								case uint8_t(AsciiDefinition::Printable::Z):
									return Source3x5::LetterZ;
								case uint8_t(AsciiDefinition::Printable::Digit0):
									return Source3x5::Number0;
								case uint8_t(AsciiDefinition::Printable::Digit1):
									return Source3x5::Number1;
								case uint8_t(AsciiDefinition::Printable::Digit2):
									return Source3x5::Number2;
								case uint8_t(AsciiDefinition::Printable::Digit3):
									return Source3x5::Number3;
								case uint8_t(AsciiDefinition::Printable::Digit4):
									return Source3x5::Number4;
								case uint8_t(AsciiDefinition::Printable::Digit5):
									return Source3x5::Number5;
								case uint8_t(AsciiDefinition::Printable::Digit6):
									return Source3x5::Number6;
								case uint8_t(AsciiDefinition::Printable::Digit7):
									return Source3x5::Number7;
								case uint8_t(AsciiDefinition::Printable::Digit8):
									return Source3x5::Number8;
								case uint8_t(AsciiDefinition::Printable::Digit9):
									return Source3x5::Number9;
								case uint8_t(AsciiDefinition::Printable::Period):
									return Source3x5::SymbolDecimalPoint;
								case uint8_t(AsciiDefinition::Printable::Comma):
									return Source3x5::SymbolDecimalComma;
								case uint8_t(AsciiDefinition::Printable::Plus):
									return Source3x5::SymbolPlus;
								case uint8_t(AsciiDefinition::Printable::Minus):
									return Source3x5::SymbolMinus;
								case uint8_t(AsciiDefinition::Printable::Equals):
									return Source3x5::SymbolEquals;
								case uint8_t(AsciiDefinition::Printable::Exclamation):
									return Source3x5::SymbolExclamation;
								case uint8_t(AsciiDefinition::Printable::Question):
									return Source3x5::SymbolQuestion;
								case uint8_t(AsciiDefinition::Printable::Percent):
									return Source3x5::SymbolPercent;
								case uint8_t(AsciiDefinition::Printable::ParenthesisOpen):
									return Source3x5::SymbolParenthesisOpen;
								case uint8_t(AsciiDefinition::Printable::ParenthesisClose):
									return Source3x5::SymbolParenthesisClose;
								case uint8_t(AsciiDefinition::Printable::BracketOpen):
									return Source3x5::SymbolBracketsOpen;
								case uint8_t(AsciiDefinition::Printable::BracketClose):
									return Source3x5::SymbolBracketsClose;
								case uint8_t(AsciiDefinition::Printable::LessThan):
									return Source3x5::SymbolSmaller;
								case uint8_t(AsciiDefinition::Printable::GreaterThan):
									return Source3x5::SymbolBigger;
								default:
									return nullptr;
								}
							}

							static Framework::Bitmask::Font::Character<dimension_t> GetCharacter(const char printableChar)
							{
								const uint8_t* bitmask = GetBitmask(printableChar);
								if (bitmask != nullptr)
								{
									// Monospaced font: advance is constant width.
									return Framework::Bitmask::Font::Character<dimension_t>{ GetBitmask(printableChar), Width };
								}
								else
								{
									return Framework::Bitmask::Font::Character<dimension_t>{ nullptr, 0 };
								}
							}
						};

						struct FontType5x5
						{
							using ReaderType = Framework::Bitmask::Reader::Flash;

							using dimension_t = uint8_t;

							static constexpr dimension_t Width = Source5x5::Width;
							static constexpr dimension_t Height = Source5x5::Height;

							static const uint8_t* GetBitmask(const char printableChar)
							{
								switch (static_cast<uint8_t>(printableChar))
								{
								case uint8_t(AsciiDefinition::Printable::a):
								case uint8_t(AsciiDefinition::Printable::A):
									return Source5x5::LetterA;
								case uint8_t(AsciiDefinition::Printable::b):
								case uint8_t(AsciiDefinition::Printable::B):
									return Source5x5::LetterB;
								case uint8_t(AsciiDefinition::Printable::c):
								case uint8_t(AsciiDefinition::Printable::C):
									return Source5x5::LetterC;
								case uint8_t(AsciiDefinition::Printable::d):
								case uint8_t(AsciiDefinition::Printable::D):
									return Source5x5::LetterD;
								case uint8_t(AsciiDefinition::Printable::e):
								case uint8_t(AsciiDefinition::Printable::E):
									return Source5x5::LetterE;
								case uint8_t(AsciiDefinition::Printable::f):
								case uint8_t(AsciiDefinition::Printable::F):
									return Source5x5::LetterF;
								case uint8_t(AsciiDefinition::Printable::g):
								case uint8_t(AsciiDefinition::Printable::G):
									return Source5x5::LetterG;
								case uint8_t(AsciiDefinition::Printable::h):
								case uint8_t(AsciiDefinition::Printable::H):
									return Source5x5::LetterH;
								case uint8_t(AsciiDefinition::Printable::i):
								case uint8_t(AsciiDefinition::Printable::I):
									return Source5x5::LetterI;
								case uint8_t(AsciiDefinition::Printable::j):
								case uint8_t(AsciiDefinition::Printable::J):
									return Source5x5::LetterJ;
								case uint8_t(AsciiDefinition::Printable::k):
								case uint8_t(AsciiDefinition::Printable::K):
									return Source5x5::LetterK;
								case uint8_t(AsciiDefinition::Printable::l):
								case uint8_t(AsciiDefinition::Printable::L):
									return Source5x5::LetterL;
								case uint8_t(AsciiDefinition::Printable::m):
								case uint8_t(AsciiDefinition::Printable::M):
									return Source5x5::LetterM;
								case uint8_t(AsciiDefinition::Printable::n):
								case uint8_t(AsciiDefinition::Printable::N):
									return Source5x5::LetterN;
								case uint8_t(AsciiDefinition::Printable::o):
								case uint8_t(AsciiDefinition::Printable::O):
									return Source5x5::LetterO;
								case uint8_t(AsciiDefinition::Printable::p):
								case uint8_t(AsciiDefinition::Printable::P):
									return Source5x5::LetterP;
								case uint8_t(AsciiDefinition::Printable::q):
								case uint8_t(AsciiDefinition::Printable::Q):
									return Source5x5::LetterQ;
								case uint8_t(AsciiDefinition::Printable::r):
								case uint8_t(AsciiDefinition::Printable::R):
									return Source5x5::LetterR;
								case uint8_t(AsciiDefinition::Printable::s):
								case uint8_t(AsciiDefinition::Printable::S):
									return Source5x5::LetterS;
								case uint8_t(AsciiDefinition::Printable::t):
								case uint8_t(AsciiDefinition::Printable::T):
									return Source5x5::LetterT;
								case uint8_t(AsciiDefinition::Printable::u):
								case uint8_t(AsciiDefinition::Printable::U):
									return Source5x5::LetterU;
								case uint8_t(AsciiDefinition::Printable::v):
								case uint8_t(AsciiDefinition::Printable::V):
									return Source5x5::LetterV;
								case uint8_t(AsciiDefinition::Printable::w):
								case uint8_t(AsciiDefinition::Printable::W):
									return Source5x5::LetterW;
								case uint8_t(AsciiDefinition::Printable::x):
								case uint8_t(AsciiDefinition::Printable::X):
									return Source5x5::LetterX;
								case uint8_t(AsciiDefinition::Printable::y):
								case uint8_t(AsciiDefinition::Printable::Y):
									return Source5x5::LetterY;
								case uint8_t(AsciiDefinition::Printable::z):
								case uint8_t(AsciiDefinition::Printable::Z):
									return Source5x5::LetterZ;
								case uint8_t(AsciiDefinition::Printable::Digit0):
									return Source5x5::Number0;
								case uint8_t(AsciiDefinition::Printable::Digit1):
									return Source5x5::Number1;
								case uint8_t(AsciiDefinition::Printable::Digit2):
									return Source5x5::Number2;
								case uint8_t(AsciiDefinition::Printable::Digit3):
									return Source5x5::Number3;
								case uint8_t(AsciiDefinition::Printable::Digit4):
									return Source5x5::Number4;
								case uint8_t(AsciiDefinition::Printable::Digit5):
									return Source5x5::Number5;
								case uint8_t(AsciiDefinition::Printable::Digit6):
									return Source5x5::Number6;
								case uint8_t(AsciiDefinition::Printable::Digit7):
									return Source5x5::Number7;
								case uint8_t(AsciiDefinition::Printable::Digit8):
									return Source5x5::Number8;
								case uint8_t(AsciiDefinition::Printable::Digit9):
									return Source5x5::Number9;
								case uint8_t(AsciiDefinition::Printable::Period):
									return Source5x5::SymbolDecimalPoint;
								case uint8_t(AsciiDefinition::Printable::Comma):
									return Source5x5::SymbolDecimalComma;
								case uint8_t(AsciiDefinition::Printable::Plus):
									return Source5x5::SymbolPlus;
								case uint8_t(AsciiDefinition::Printable::Minus):
									return Source5x5::SymbolMinus;
								case uint8_t(AsciiDefinition::Printable::Equals):
									return Source5x5::SymbolEquals;
								case uint8_t(AsciiDefinition::Printable::Exclamation):
									return Source5x5::SymbolExclamation;
								case uint8_t(AsciiDefinition::Printable::Question):
									return Source5x5::SymbolQuestion;
								case uint8_t(AsciiDefinition::Printable::Percent):
									return Source5x5::SymbolPercent;
								case uint8_t(AsciiDefinition::Printable::ParenthesisOpen):
									return Source5x5::SymbolParenthesisOpen;
								case uint8_t(AsciiDefinition::Printable::ParenthesisClose):
									return Source5x5::SymbolParenthesisClose;
								case uint8_t(AsciiDefinition::Printable::BracketOpen):
									return Source5x5::SymbolBracketsOpen;
								case uint8_t(AsciiDefinition::Printable::BracketClose):
									return Source5x5::SymbolBracketsClose;
								case uint8_t(AsciiDefinition::Printable::LessThan):
									return Source5x5::SymbolSmaller;
								case uint8_t(AsciiDefinition::Printable::GreaterThan):
									return Source5x5::SymbolBigger;
								default:
									return nullptr;
								}
							}

							static Framework::Bitmask::Font::Character<dimension_t> GetCharacter(const char printableChar)
							{
								const uint8_t* bitmask = GetBitmask(printableChar);
								if (bitmask != nullptr)
								{
									// Monospaced font: advance is constant width.
									return Framework::Bitmask::Font::Character<dimension_t>{ GetBitmask(printableChar), Width };
								}
								else
								{
									return Framework::Bitmask::Font::Character<dimension_t>{ nullptr, 0 };
								}
							}
						};
					}
				}
			}
		}
	}
}
#endif