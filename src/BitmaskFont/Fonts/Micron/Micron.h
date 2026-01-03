#ifndef _EGFX_BITMASK_FONT_MICRON_h
#define _EGFX_BITMASK_FONT_MICRON_h

#include "../../../Platform/Platform.h"
#include "Micron1x5.h"
#include "Micron2x5.h"

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
			struct FontType1x5
			{
				using dimension_t = uint8_t;

				static constexpr dimension_t Width = Source1x5::Width;
				static constexpr dimension_t Height = Source1x5::Height;

				static const uint8_t* GetBitmask(const char printableChar)
				{
					switch (static_cast<uint8_t>(printableChar))
					{
					case uint8_t(AsciiDefinition::Printable::a):
					case uint8_t(AsciiDefinition::Printable::A):
						return static_cast<const uint8_t*>(Source1x5::LetterA);
					case uint8_t(AsciiDefinition::Printable::b):
					case uint8_t(AsciiDefinition::Printable::B):
						return static_cast<const uint8_t*>(Source1x5::LetterB);
					case uint8_t(AsciiDefinition::Printable::c):
					case uint8_t(AsciiDefinition::Printable::C):
						return static_cast<const uint8_t*>(Source1x5::LetterC);
					case uint8_t(AsciiDefinition::Printable::d):
					case uint8_t(AsciiDefinition::Printable::D):
						return static_cast<const uint8_t*>(Source1x5::LetterD);
					case uint8_t(AsciiDefinition::Printable::e):
					case uint8_t(AsciiDefinition::Printable::E):
						return static_cast<const uint8_t*>(Source1x5::LetterE);
					case uint8_t(AsciiDefinition::Printable::f):
					case uint8_t(AsciiDefinition::Printable::F):
						return static_cast<const uint8_t*>(Source1x5::LetterF);
					case uint8_t(AsciiDefinition::Printable::g):
					case uint8_t(AsciiDefinition::Printable::G):
						return static_cast<const uint8_t*>(Source1x5::LetterG);
					case uint8_t(AsciiDefinition::Printable::h):
					case uint8_t(AsciiDefinition::Printable::H):
						return static_cast<const uint8_t*>(Source1x5::LetterH);
					case uint8_t(AsciiDefinition::Printable::i):
					case uint8_t(AsciiDefinition::Printable::I):
						return static_cast<const uint8_t*>(Source1x5::LetterI);
					case uint8_t(AsciiDefinition::Printable::j):
					case uint8_t(AsciiDefinition::Printable::J):
						return static_cast<const uint8_t*>(Source1x5::LetterJ);
					case uint8_t(AsciiDefinition::Printable::k):
					case uint8_t(AsciiDefinition::Printable::K):
						return static_cast<const uint8_t*>(Source1x5::LetterK);
					case uint8_t(AsciiDefinition::Printable::l):
					case uint8_t(AsciiDefinition::Printable::L):
						return static_cast<const uint8_t*>(Source1x5::LetterL);
					case uint8_t(AsciiDefinition::Printable::m):
					case uint8_t(AsciiDefinition::Printable::M):
						return static_cast<const uint8_t*>(Source1x5::LetterM);
					case uint8_t(AsciiDefinition::Printable::n):
					case uint8_t(AsciiDefinition::Printable::N):
						return static_cast<const uint8_t*>(Source1x5::LetterN);
					case uint8_t(AsciiDefinition::Printable::o):
					case uint8_t(AsciiDefinition::Printable::O):
						return static_cast<const uint8_t*>(Source1x5::LetterO);
					case uint8_t(AsciiDefinition::Printable::p):
					case uint8_t(AsciiDefinition::Printable::P):
						return static_cast<const uint8_t*>(Source1x5::LetterP);
					case uint8_t(AsciiDefinition::Printable::q):
					case uint8_t(AsciiDefinition::Printable::Q):
						return static_cast<const uint8_t*>(Source1x5::LetterQ);
					case uint8_t(AsciiDefinition::Printable::r):
					case uint8_t(AsciiDefinition::Printable::R):
						return static_cast<const uint8_t*>(Source1x5::LetterR);
					case uint8_t(AsciiDefinition::Printable::s):
					case uint8_t(AsciiDefinition::Printable::S):
						return static_cast<const uint8_t*>(Source1x5::LetterS);
					case uint8_t(AsciiDefinition::Printable::t):
					case uint8_t(AsciiDefinition::Printable::T):
						return static_cast<const uint8_t*>(Source1x5::LetterT);
					case uint8_t(AsciiDefinition::Printable::u):
					case uint8_t(AsciiDefinition::Printable::U):
						return static_cast<const uint8_t*>(Source1x5::LetterU);
					case uint8_t(AsciiDefinition::Printable::v):
					case uint8_t(AsciiDefinition::Printable::V):
						return static_cast<const uint8_t*>(Source1x5::LetterV);
					case uint8_t(AsciiDefinition::Printable::w):
					case uint8_t(AsciiDefinition::Printable::W):
						return static_cast<const uint8_t*>(Source1x5::LetterW);
					case uint8_t(AsciiDefinition::Printable::x):
					case uint8_t(AsciiDefinition::Printable::X):
						return static_cast<const uint8_t*>(Source1x5::LetterX);
					case uint8_t(AsciiDefinition::Printable::y):
					case uint8_t(AsciiDefinition::Printable::Y):
						return static_cast<const uint8_t*>(Source1x5::LetterY);
					case uint8_t(AsciiDefinition::Printable::z):
					case uint8_t(AsciiDefinition::Printable::Z):
						return static_cast<const uint8_t*>(Source1x5::LetterZ);
					case uint8_t(AsciiDefinition::Printable::Digit0):
						return static_cast<const uint8_t*>(Source1x5::Number0);
					case uint8_t(AsciiDefinition::Printable::Digit1):
						return static_cast<const uint8_t*>(Source1x5::Number1);
					case uint8_t(AsciiDefinition::Printable::Digit2):
						return static_cast<const uint8_t*>(Source1x5::Number2);
					case uint8_t(AsciiDefinition::Printable::Digit3):
						return static_cast<const uint8_t*>(Source1x5::Number3);
					case uint8_t(AsciiDefinition::Printable::Digit4):
						return static_cast<const uint8_t*>(Source1x5::Number4);
					case uint8_t(AsciiDefinition::Printable::Digit5):
						return static_cast<const uint8_t*>(Source1x5::Number5);
					case uint8_t(AsciiDefinition::Printable::Digit6):
						return static_cast<const uint8_t*>(Source1x5::Number6);
					case uint8_t(AsciiDefinition::Printable::Digit7):
						return static_cast<const uint8_t*>(Source1x5::Number7);
					case uint8_t(AsciiDefinition::Printable::Digit8):
						return static_cast<const uint8_t*>(Source1x5::Number8);
					case uint8_t(AsciiDefinition::Printable::Digit9):
						return static_cast<const uint8_t*>(Source1x5::Number9);
					case uint8_t(AsciiDefinition::Printable::Period):
						return static_cast<const uint8_t*>(Source1x5::SymbolDecimalPoint);
					case uint8_t(AsciiDefinition::Printable::Comma):
						return static_cast<const uint8_t*>(Source1x5::SymbolDecimalComma);
					case uint8_t(AsciiDefinition::Printable::Plus):
						return static_cast<const uint8_t*>(Source1x5::SymbolPlus);
					case uint8_t(AsciiDefinition::Printable::Minus):
						return static_cast<const uint8_t*>(Source1x5::SymbolMinus);
					case uint8_t(AsciiDefinition::Printable::Equals):
						return static_cast<const uint8_t*>(Source1x5::SymbolEquals);
					case uint8_t(AsciiDefinition::Printable::Exclamation):
						return static_cast<const uint8_t*>(Source1x5::SymbolExclamation);
					case uint8_t(AsciiDefinition::Printable::Question):
						return static_cast<const uint8_t*>(Source1x5::SymbolQuestion);
					case uint8_t(AsciiDefinition::Printable::Percent):
						return static_cast<const uint8_t*>(Source1x5::SymbolPercent);
					case uint8_t(AsciiDefinition::Printable::ParenthesisOpen):
						return static_cast<const uint8_t*>(Source1x5::SymbolParenthesisOpen);
					case uint8_t(AsciiDefinition::Printable::ParenthesisClose):
						return static_cast<const uint8_t*>(Source1x5::SymbolParenthesisClose);
					case uint8_t(AsciiDefinition::Printable::BracketOpen):
						return static_cast<const uint8_t*>(Source1x5::SymbolBracketsOpen);
					case uint8_t(AsciiDefinition::Printable::BracketClose):
						return static_cast<const uint8_t*>(Source1x5::SymbolBracketsClose);
					case uint8_t(AsciiDefinition::Printable::LessThan):
						return static_cast<const uint8_t*>(Source1x5::SymbolSmaller);
					case uint8_t(AsciiDefinition::Printable::GreaterThan):
						return static_cast<const uint8_t*>(Source1x5::SymbolBigger);
					default:
						return nullptr;
					}
				}
			};

			struct FontType2x5
			{
				using dimension_t = uint8_t;
				static constexpr dimension_t Width = Source2x5::Width;
				static constexpr dimension_t Height = Source2x5::Height;

				static const uint8_t* GetBitmask(const char printableChar)
				{
					switch (static_cast<uint8_t>(printableChar))
					{
					case uint8_t(AsciiDefinition::Printable::a):
					case uint8_t(AsciiDefinition::Printable::A):
						return static_cast<const uint8_t*>(Source2x5::LetterA);
					case uint8_t(AsciiDefinition::Printable::b):
					case uint8_t(AsciiDefinition::Printable::B):
						return static_cast<const uint8_t*>(Source2x5::LetterB);
					case uint8_t(AsciiDefinition::Printable::c):
					case uint8_t(AsciiDefinition::Printable::C):
						return static_cast<const uint8_t*>(Source2x5::LetterC);
					case uint8_t(AsciiDefinition::Printable::d):
					case uint8_t(AsciiDefinition::Printable::D):
						return static_cast<const uint8_t*>(Source2x5::LetterD);
					case uint8_t(AsciiDefinition::Printable::e):
					case uint8_t(AsciiDefinition::Printable::E):
						return static_cast<const uint8_t*>(Source2x5::LetterE);
					case uint8_t(AsciiDefinition::Printable::f):
					case uint8_t(AsciiDefinition::Printable::F):
						return static_cast<const uint8_t*>(Source2x5::LetterF);
					case uint8_t(AsciiDefinition::Printable::g):
					case uint8_t(AsciiDefinition::Printable::G):
						return static_cast<const uint8_t*>(Source2x5::LetterG);
					case uint8_t(AsciiDefinition::Printable::h):
					case uint8_t(AsciiDefinition::Printable::H):
						return static_cast<const uint8_t*>(Source2x5::LetterH);
					case uint8_t(AsciiDefinition::Printable::i):
					case uint8_t(AsciiDefinition::Printable::I):
						return static_cast<const uint8_t*>(Source2x5::LetterI);
					case uint8_t(AsciiDefinition::Printable::j):
					case uint8_t(AsciiDefinition::Printable::J):
						return static_cast<const uint8_t*>(Source2x5::LetterJ);
					case uint8_t(AsciiDefinition::Printable::k):
					case uint8_t(AsciiDefinition::Printable::K):
						return static_cast<const uint8_t*>(Source2x5::LetterK);
					case uint8_t(AsciiDefinition::Printable::l):
					case uint8_t(AsciiDefinition::Printable::L):
						return static_cast<const uint8_t*>(Source2x5::LetterL);
					case uint8_t(AsciiDefinition::Printable::m):
					case uint8_t(AsciiDefinition::Printable::M):
						return static_cast<const uint8_t*>(Source2x5::LetterM);
					case uint8_t(AsciiDefinition::Printable::n):
					case uint8_t(AsciiDefinition::Printable::N):
						return static_cast<const uint8_t*>(Source2x5::LetterN);
					case uint8_t(AsciiDefinition::Printable::o):
					case uint8_t(AsciiDefinition::Printable::O):
						return static_cast<const uint8_t*>(Source2x5::LetterO);
					case uint8_t(AsciiDefinition::Printable::p):
					case uint8_t(AsciiDefinition::Printable::P):
						return static_cast<const uint8_t*>(Source2x5::LetterP);
					case uint8_t(AsciiDefinition::Printable::q):
					case uint8_t(AsciiDefinition::Printable::Q):
						return static_cast<const uint8_t*>(Source2x5::LetterQ);
					case uint8_t(AsciiDefinition::Printable::r):
					case uint8_t(AsciiDefinition::Printable::R):
						return static_cast<const uint8_t*>(Source2x5::LetterR);
					case uint8_t(AsciiDefinition::Printable::s):
					case uint8_t(AsciiDefinition::Printable::S):
						return static_cast<const uint8_t*>(Source2x5::LetterS);
					case uint8_t(AsciiDefinition::Printable::t):
					case uint8_t(AsciiDefinition::Printable::T):
						return static_cast<const uint8_t*>(Source2x5::LetterT);
					case uint8_t(AsciiDefinition::Printable::u):
					case uint8_t(AsciiDefinition::Printable::U):
						return static_cast<const uint8_t*>(Source2x5::LetterU);
					case uint8_t(AsciiDefinition::Printable::v):
					case uint8_t(AsciiDefinition::Printable::V):
						return static_cast<const uint8_t*>(Source2x5::LetterV);
					case uint8_t(AsciiDefinition::Printable::w):
					case uint8_t(AsciiDefinition::Printable::W):
						return static_cast<const uint8_t*>(Source2x5::LetterW);
					case uint8_t(AsciiDefinition::Printable::x):
					case uint8_t(AsciiDefinition::Printable::X):
						return static_cast<const uint8_t*>(Source2x5::LetterX);
					case uint8_t(AsciiDefinition::Printable::y):
					case uint8_t(AsciiDefinition::Printable::Y):
						return static_cast<const uint8_t*>(Source2x5::LetterY);
					case uint8_t(AsciiDefinition::Printable::z):
					case uint8_t(AsciiDefinition::Printable::Z):
						return static_cast<const uint8_t*>(Source2x5::LetterZ);
					case uint8_t(AsciiDefinition::Printable::Digit0):
						return static_cast<const uint8_t*>(Source2x5::Number0);
					case uint8_t(AsciiDefinition::Printable::Digit1):
						return static_cast<const uint8_t*>(Source2x5::Number1);
					case uint8_t(AsciiDefinition::Printable::Digit2):
						return static_cast<const uint8_t*>(Source2x5::Number2);
					case uint8_t(AsciiDefinition::Printable::Digit3):
						return static_cast<const uint8_t*>(Source2x5::Number3);
					case uint8_t(AsciiDefinition::Printable::Digit4):
						return static_cast<const uint8_t*>(Source2x5::Number4);
					case uint8_t(AsciiDefinition::Printable::Digit5):
						return static_cast<const uint8_t*>(Source2x5::Number5);
					case uint8_t(AsciiDefinition::Printable::Digit6):
						return static_cast<const uint8_t*>(Source2x5::Number6);
					case uint8_t(AsciiDefinition::Printable::Digit7):
						return static_cast<const uint8_t*>(Source2x5::Number7);
					case uint8_t(AsciiDefinition::Printable::Digit8):
						return static_cast<const uint8_t*>(Source2x5::Number8);
					case uint8_t(AsciiDefinition::Printable::Digit9):
						return static_cast<const uint8_t*>(Source2x5::Number9);
					case uint8_t(AsciiDefinition::Printable::Period):
						return static_cast<const uint8_t*>(Source2x5::SymbolDecimalPoint);
					case uint8_t(AsciiDefinition::Printable::Comma):
						return static_cast<const uint8_t*>(Source2x5::SymbolDecimalComma);
					case uint8_t(AsciiDefinition::Printable::Plus):
						return static_cast<const uint8_t*>(Source2x5::SymbolPlus);
					case uint8_t(AsciiDefinition::Printable::Minus):
						return static_cast<const uint8_t*>(Source2x5::SymbolMinus);
					case uint8_t(AsciiDefinition::Printable::Equals):
						return static_cast<const uint8_t*>(Source2x5::SymbolEquals);
					case uint8_t(AsciiDefinition::Printable::Exclamation):
						return static_cast<const uint8_t*>(Source2x5::SymbolExclamation);
					case uint8_t(AsciiDefinition::Printable::Question):
						return static_cast<const uint8_t*>(Source2x5::SymbolQuestion);
					case uint8_t(AsciiDefinition::Printable::Percent):
						return static_cast<const uint8_t*>(Source2x5::SymbolPercent);
					case uint8_t(AsciiDefinition::Printable::ParenthesisOpen):
						return static_cast<const uint8_t*>(Source2x5::SymbolParenthesisOpen);
					case uint8_t(AsciiDefinition::Printable::ParenthesisClose):
						return static_cast<const uint8_t*>(Source2x5::SymbolParenthesisClose);
					case uint8_t(AsciiDefinition::Printable::BracketOpen):
						return static_cast<const uint8_t*>(Source2x5::SymbolBracketsOpen);
					case uint8_t(AsciiDefinition::Printable::BracketClose):
						return static_cast<const uint8_t*>(Source2x5::SymbolBracketsClose);
					case uint8_t(AsciiDefinition::Printable::LessThan):
						return static_cast<const uint8_t*>(Source2x5::SymbolSmaller);
					case uint8_t(AsciiDefinition::Printable::GreaterThan):
						return static_cast<const uint8_t*>(Source2x5::SymbolBigger);
					default:
						return nullptr;
					}
				}
			};
		}
	}
}
#endif