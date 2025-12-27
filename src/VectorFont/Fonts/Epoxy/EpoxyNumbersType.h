#ifndef _EGFX_VECTOR_FONTS_EPOXY_NUMBERS_TYPE_h
#define _EGFX_VECTOR_FONTS_EPOXY_NUMBERS_TYPE_h

#include "Numbers.h"
#include "Symbols.h"
#include "Letters.h"

namespace Egfx
{
	namespace VectorFonts
	{
		namespace Epoxy
		{
			using namespace VectorData;

			/// <summary>
			/// Vector Font 8x8 type.
			/// Reduced font set with only numbers.
			/// </summary>
			struct NumbersFontType
			{
				using CharacterSetType = Epoxy::CharacterSetType;
				static constexpr CharacterSetType::VectorModel::axis_t SpaceWidth = Dimensions::Single::Width;
				static constexpr CharacterSetType::VectorModel::axis_t TabWidth = Dimensions::Triple::Width;
				static constexpr bool Monospace = Epoxy::Monospace;

				static CharacterSetType GetCharacterSet(const char character)
				{
					switch (static_cast<uint8_t>(character))
					{
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
					default:
						return { nullptr, 0, 0 }; // Character not found.
					}
				}
			};
		}
	}
}
#endif