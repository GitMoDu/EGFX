#ifndef _EGFX_FRAMEWORK_ASSETS_FONT_VECTOR_EPOXY_FULL_TYPE_h
#define _EGFX_FRAMEWORK_ASSETS_FONT_VECTOR_EPOXY_FULL_TYPE_h

#include "Numbers.h"
#include "Symbols.h"
#include "Letters.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace Font
			{
				namespace Vector
				{
					namespace Epoxy
					{
						namespace AsciiDefinition = Framework::AsciiDefinition;

						using namespace VectorData;


						/// <summary>
						/// Vector Font 8x8 type.
						/// Full ASCII font type with letters, numbers, and symbols.
						/// </summary>
						struct FullFontType
						{
							using CharacterSetType = Epoxy::CharacterSetType;
							using dimension_t = CharacterSetType::VectorModel::axis_t;
							static constexpr CharacterSetType::VectorModel::axis_t SpaceWidth = Dimensions::Single::Width;
							static constexpr CharacterSetType::VectorModel::axis_t TabWidth = Dimensions::Triple::Width;
							static constexpr bool Monospace = Epoxy::Monospace;

							static CharacterSetType GetCharacterSet(const char character)
							{
								switch (static_cast<uint8_t>(character))
								{
								case uint8_t(AsciiDefinition::Printable::VerticalBar):
									return { Symbols::CharVerticalBar, sizeof(Symbols::CharVerticalBar) / sizeof(uint8_t), Dimensions::Single::EndX };
								case uint8_t(AsciiDefinition::Printable::Underscore):
									return { Symbols::CharUnderscore, sizeof(Symbols::CharUnderscore) / sizeof(uint8_t), Dimensions::Triple::EndX };
								case uint8_t(AsciiDefinition::Printable::Backslash):
									return { Symbols::CharBackslash, sizeof(Symbols::CharBackslash) / sizeof(uint8_t), Dimensions::Triple::EndX };
								case uint8_t(AsciiDefinition::Printable::ForwardSlash):
									return { Symbols::CharForwardSlash, sizeof(Symbols::CharForwardSlash) / sizeof(uint8_t), Dimensions::Triple::EndX };
								case uint8_t(AsciiDefinition::Printable::Apostrophe):
									return { Symbols::CharApostrophe, sizeof(Symbols::CharApostrophe) / sizeof(uint8_t), Dimensions::Single::EndX };
								case uint8_t(AsciiDefinition::Printable::DoubleQuote):
									return { Symbols::CharDoubleQuote, sizeof(Symbols::CharDoubleQuote) / sizeof(uint8_t), Dimensions::Double::EndX };
								case uint8_t(AsciiDefinition::Printable::Colon):
									return { Symbols::CharColon, sizeof(Symbols::CharColon) / sizeof(uint8_t), Dimensions::Single::EndX };
								case uint8_t(AsciiDefinition::Printable::Semicolon):
									return { Symbols::CharSemicolon, sizeof(Symbols::CharSemicolon) / sizeof(uint8_t), Dimensions::Double::EndX };
								case uint8_t(AsciiDefinition::Printable::LessThan):
									return { Symbols::CharLessThan, sizeof(Symbols::CharLessThan) / sizeof(uint8_t), Dimensions::Triple::EndX };
								case uint8_t(AsciiDefinition::Printable::GreaterThan):
									return { Symbols::CharGreaterThan, sizeof(Symbols::CharGreaterThan) / sizeof(uint8_t), Dimensions::Triple::EndX };
								case uint8_t(AsciiDefinition::Printable::Period):
									return { Symbols::CharPeriod, sizeof(Symbols::CharPeriod) / sizeof(uint8_t), Dimensions::Single::EndX };
								case uint8_t(AsciiDefinition::Printable::Comma):
									return { Symbols::CharComma, sizeof(Symbols::CharComma) / sizeof(uint8_t), Dimensions::Double::EndX };
								case uint8_t(AsciiDefinition::Printable::Minus):
									return { Symbols::CharMinus, sizeof(Symbols::CharMinus) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::Plus):
									return { Symbols::CharPlus, sizeof(Symbols::CharPlus) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::Asterisk):
									return { Symbols::CharAsterisk, sizeof(Symbols::CharAsterisk) / sizeof(uint8_t), Dimensions::Triple::EndX };
								case uint8_t(AsciiDefinition::Printable::Equals):
									return { Symbols::CharEquals, sizeof(Symbols::CharEquals) / sizeof(uint8_t), Dimensions::Triple::EndX };
								case uint8_t(AsciiDefinition::Printable::Question):
									return { Symbols::CharQuestion, sizeof(Symbols::CharQuestion) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::Exclamation):
									return { Symbols::CharExclamation, sizeof(Symbols::CharExclamation) / sizeof(uint8_t), Dimensions::Double::EndX };
								case uint8_t(AsciiDefinition::Printable::NumberSign):
									return { Symbols::CharNumberSign, sizeof(Symbols::CharNumberSign) / sizeof(uint8_t), Dimensions::Largest::EndX };
								case uint8_t(AsciiDefinition::Printable::Dollar):
									return { Symbols::CharDollar, sizeof(Symbols::CharDollar) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::Percent):
									return { Symbols::CharPercent, sizeof(Symbols::CharPercent) / sizeof(uint8_t), Dimensions::Big::EndX };
								case uint8_t(AsciiDefinition::Printable::Ampersand):
									return { Symbols::CharAmpersand, sizeof(Symbols::CharAmpersand) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::ParenthesisOpen):
									return { Symbols::CharParenthesisOpen, sizeof(Symbols::CharParenthesisOpen) / sizeof(uint8_t), Dimensions::Triple::EndX };
								case uint8_t(AsciiDefinition::Printable::ParenthesisClose):
									return { Symbols::CharParenthesisClose, sizeof(Symbols::CharParenthesisClose) / sizeof(uint8_t), Dimensions::Triple::EndX };
								case uint8_t(AsciiDefinition::Printable::BracketOpen):
									return { Symbols::CharBracketOpen, sizeof(Symbols::CharBracketOpen) / sizeof(uint8_t), Dimensions::Triple::EndX };
								case uint8_t(AsciiDefinition::Printable::BracketClose):
									return { Symbols::CharBracketClose, sizeof(Symbols::CharBracketClose) / sizeof(uint8_t), Dimensions::Triple::EndX };

								case uint8_t(AsciiDefinition::Printable::Digit0):
									return { Numbers::Char0, sizeof(Numbers::Char0) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::Digit1):
									return { Numbers::Char1, sizeof(Numbers::Char1) / sizeof(uint8_t), Dimensions::Triple::EndX };
								case uint8_t(AsciiDefinition::Printable::Digit2):
									return { Numbers::Char2, sizeof(Numbers::Char2) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::Digit3):
									return { Numbers::Char3, sizeof(Numbers::Char3) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::Digit4):
									return { Numbers::Char4, sizeof(Numbers::Char4) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::Digit5):
									return { Numbers::Char5, sizeof(Numbers::Char5) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::Digit6):
									return { Numbers::Char6, sizeof(Numbers::Char6) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::Digit7):
									return { Numbers::Char7, sizeof(Numbers::Char7) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::Digit8):
									return { Numbers::Char8, sizeof(Numbers::Char8) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::Digit9):
									return { Numbers::Char9, sizeof(Numbers::Char9) / sizeof(uint8_t), Dimensions::Small::EndX };

								case uint8_t(AsciiDefinition::Printable::A):
									return { Letters::CharA, sizeof(Letters::CharA) / sizeof(uint8_t), Dimensions::Big::EndX };
								case uint8_t(AsciiDefinition::Printable::a):
									return { Letters::Chara, sizeof(Letters::Chara) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::B):
									return { Letters::CharB, sizeof(Letters::CharB) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::b):
									return { Letters::Charb, sizeof(Letters::Charb) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::C):
									return { Letters::CharC, sizeof(Letters::CharC) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::c):
									return { Letters::Charc, sizeof(Letters::Charc) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::D):
									return { Letters::CharD, sizeof(Letters::CharD) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::d):
									return { Letters::Chard, sizeof(Letters::Chard) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::E):
									return { Letters::CharE, sizeof(Letters::CharE) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::e):
									return { Letters::Chare, sizeof(Letters::Chare) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::F):
									return { Letters::CharF, sizeof(Letters::CharF) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::f):
									return { Letters::Charf, sizeof(Letters::Charf) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::G):
									return { Letters::CharG, sizeof(Letters::CharG) / sizeof(uint8_t), Dimensions::Big::EndX };
								case uint8_t(AsciiDefinition::Printable::g):
									return { Letters::Charg, sizeof(Letters::Charg) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::H):
									return { Letters::CharH, sizeof(Letters::CharH) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::h):
									return { Letters::Charh, sizeof(Letters::Charh) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::I):
									return { Letters::CharI, sizeof(Letters::CharI) / sizeof(uint8_t), Dimensions::Triple::EndX };
								case uint8_t(AsciiDefinition::Printable::i):
									return { Letters::Chari, sizeof(Letters::Chari) / sizeof(uint8_t), Dimensions::Double::EndX };
								case uint8_t(AsciiDefinition::Printable::J):
									return { Letters::CharJ, sizeof(Letters::CharJ) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::j):
									return { Letters::Charj, sizeof(Letters::Charj) / sizeof(uint8_t), Dimensions::Double::EndX };
								case uint8_t(AsciiDefinition::Printable::K):
									return { Letters::CharK, sizeof(Letters::CharK) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::k):
									return { Letters::Chark, sizeof(Letters::Chark) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::L):
									return { Letters::CharL, sizeof(Letters::CharL) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::l):
									return { Letters::Charl, sizeof(Letters::Charl) / sizeof(uint8_t), Dimensions::Triple::EndX };
								case uint8_t(AsciiDefinition::Printable::M):
									return { Letters::CharM, sizeof(Letters::CharM) / sizeof(uint8_t), Dimensions::Largest::EndX };
								case uint8_t(AsciiDefinition::Printable::m):
									return { Letters::Charm, sizeof(Letters::Charm) / sizeof(uint8_t), Dimensions::Largest::EndX };
								case uint8_t(AsciiDefinition::Printable::N):
									return { Letters::CharN, sizeof(Letters::CharN) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::n):
									return { Letters::Charn, sizeof(Letters::Charn) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::O):
									return { Letters::CharO, sizeof(Letters::CharO) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::o):
									return { Letters::Charo, sizeof(Letters::Charo) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::P):
									return { Letters::CharP, sizeof(Letters::CharP) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::p):
									return { Letters::Charp, sizeof(Letters::Charp) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::Q):
									return { Letters::CharQ, sizeof(Letters::CharQ) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::q):
									return { Letters::Charq, sizeof(Letters::Charq) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::R):
									return { Letters::CharR, sizeof(Letters::CharR) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::r):
									return { Letters::Charr, sizeof(Letters::Charr) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::S):
									return { Letters::CharS, sizeof(Letters::CharS) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::s):
									return { Letters::Chars, sizeof(Letters::Chars) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::T):
									return { Letters::CharT, sizeof(Letters::CharT) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::t):
									return { Letters::Chart, sizeof(Letters::Chart) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::U):
									return { Letters::CharU, sizeof(Letters::CharU) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::u):
									return { Letters::Charu, sizeof(Letters::Charu) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::V):
									return { Letters::CharV, sizeof(Letters::CharV) / sizeof(uint8_t), Dimensions::Big::EndX };
								case uint8_t(AsciiDefinition::Printable::v):
									return { Letters::Charv, sizeof(Letters::Charv) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::W):
									return { Letters::CharW, sizeof(Letters::CharW) / sizeof(uint8_t), Dimensions::Largest::EndX };
								case uint8_t(AsciiDefinition::Printable::w):
									return { Letters::Charw, sizeof(Letters::Charw) / sizeof(uint8_t), Dimensions::Largest::EndX };
								case uint8_t(AsciiDefinition::Printable::X):
									return { Letters::CharX, sizeof(Letters::CharX) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::x):
									return { Letters::Charx, sizeof(Letters::Charx) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::Y):
									return { Letters::CharY, sizeof(Letters::CharY) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::y):
									return { Letters::Chary, sizeof(Letters::Chary) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								case uint8_t(AsciiDefinition::Printable::Z):
									return { Letters::CharZ, sizeof(Letters::CharZ) / sizeof(uint8_t), Dimensions::Small::EndX };
								case uint8_t(AsciiDefinition::Printable::z):
									return { Letters::Charz, sizeof(Letters::Charz) / sizeof(uint8_t), Dimensions::Lowercase::EndX };
								default:
									return { nullptr, 0, 0 }; // Character not found.
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