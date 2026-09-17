#ifndef _EGFX_MODULES_KIT_FONT_BITMASK_THREADLOCKER_FONT_TYPE_h
#define _EGFX_MODULES_KIT_FONT_BITMASK_THREADLOCKER_FONT_TYPE_h

#include <EgfxFramework.h>
#include "Source.h"

namespace Egfx
{
	namespace Modules
	{
		namespace Kit
		{
			namespace Font
			{
				namespace Bitmask
				{
					/// <summary>
					/// Tiny font for very small sizes with clear(ish) legibility.
					/// Only 88 bytes of atlas, with 3x3 pixel glyphs for uppercase letters, numbers, and a few symbols.
					/// </summary>
					namespace Threadlocker
					{
						struct FontType3x3
						{
							using dimension_t = uint8_t;
							using glyph_t = Framework::Text::Bitmask::GlyphBitmask<dimension_t>;

							static constexpr bool IsMonospace = false;
							static constexpr Framework::DataSourceTypeEnum DataSourceType = Framework::DataSourceTypeEnum::Flash;

							static constexpr dimension_t GetFontWidth() { return Source3x3::Width; }
							static constexpr dimension_t GetFontHeight() { return Source3x3::Height; }

							static glyph_t GetGlyph(const uint8_t character)
							{
								switch (static_cast<char>(character))
								{
								case 'A':
									return glyph_t{ Source3x3::Atlas + 0, 3, Source3x3::Height };
								case 'B':
									return glyph_t{ Source3x3::Atlas + 3, 3, Source3x3::Height };
								case 'C':
									return glyph_t{ Source3x3::Atlas + 6, 3, Source3x3::Height };
								case 'D':
									return glyph_t{ Source3x3::Atlas + 9, 3, Source3x3::Height };
								case 'E':
									return glyph_t{ Source3x3::Atlas + 12, 3, Source3x3::Height };
								case 'F':
									return glyph_t{ Source3x3::Atlas + 15, 3, Source3x3::Height };
								case 'G':
									return glyph_t{ Source3x3::Atlas + 18, 3, Source3x3::Height };
								case 'H':
									return glyph_t{ Source3x3::Atlas + 21, 3, Source3x3::Height };
								case 'I':
									return glyph_t{ Source3x3::Atlas + 24, 1, Source3x3::Height };
								case 'J':
									return glyph_t{ Source3x3::Atlas + 27, 3, Source3x3::Height };
								case 'K':
									return glyph_t{ Source3x3::Atlas + 30, 3, Source3x3::Height };
								case 'L':
									return glyph_t{ Source3x3::Atlas + 33, 2, Source3x3::Height };
								case 'M':
									return glyph_t{ Source3x3::Atlas + 36, 3, Source3x3::Height };
								case 'N':
									return glyph_t{ Source3x3::Atlas + 39, 3, Source3x3::Height };
								case 'O':
									return glyph_t{ Source3x3::Atlas + 42, 3, Source3x3::Height };
								case 'P':
									return glyph_t{ Source3x3::Atlas + 45, 3, Source3x3::Height };
								case 'Q':
									return glyph_t{ Source3x3::Atlas + 48, 3, Source3x3::Height };
								case 'R':
									return glyph_t{ Source3x3::Atlas + 51, 3, Source3x3::Height };
								case 'S':
									return glyph_t{ Source3x3::Atlas + 54, 3, Source3x3::Height };
								case 'T':
									return glyph_t{ Source3x3::Atlas + 57, 3, Source3x3::Height };
								case 'U':
									return glyph_t{ Source3x3::Atlas + 60, 3, Source3x3::Height };
								case 'V':
									return glyph_t{ Source3x3::Atlas + 63, 3, Source3x3::Height };
								case 'W':
									return glyph_t{ Source3x3::Atlas + 66, 3, Source3x3::Height };
								case 'X':
									return glyph_t{ Source3x3::Atlas + 69, 3, Source3x3::Height };
								case 'Y':
									return glyph_t{ Source3x3::Atlas + 72, 3, Source3x3::Height };
								case 'Z':
									return glyph_t{ Source3x3::Atlas + 75, 3, Source3x3::Height };
								case '0':
									return glyph_t{ Source3x3::Atlas + 78, 3, Source3x3::Height };
								case '1':
									return glyph_t{ Source3x3::Atlas + 81, 2, Source3x3::Height };
								case '2':
									return glyph_t{ Source3x3::Atlas + 84, 3, Source3x3::Height };
								case '3':
									return glyph_t{ Source3x3::Atlas + 87, 3, Source3x3::Height };
								case '4':
									return glyph_t{ Source3x3::Atlas + 90, 3, Source3x3::Height };
								case '5':
									return glyph_t{ Source3x3::Atlas + 93, 3, Source3x3::Height };
								case '6':
									return glyph_t{ Source3x3::Atlas + 96, 3, Source3x3::Height };
								case '7':
									return glyph_t{ Source3x3::Atlas + 99, 3, Source3x3::Height };
								case '8':
									return glyph_t{ Source3x3::Atlas + 102, 3, Source3x3::Height };
								case '9':
									return glyph_t{ Source3x3::Atlas + 105, 3, Source3x3::Height };
								case '.':
									return glyph_t{ Source3x3::Atlas + 108, 1, Source3x3::Height };
								case ',':
									return glyph_t{ Source3x3::Atlas + 111, 1, Source3x3::Height };
								case '=':
									return glyph_t{ Source3x3::Atlas + 114, 3, Source3x3::Height };
								case '+':
									return glyph_t{ Source3x3::Atlas + 117, 3, Source3x3::Height };
								case '-':
									return glyph_t{ Source3x3::Atlas + 120, 3, Source3x3::Height };
								case '<':
									return glyph_t{ Source3x3::Atlas + 123, 2, Source3x3::Height };
								case '>':
									return glyph_t{ Source3x3::Atlas + 126, 2, Source3x3::Height };
								case '_':
									return glyph_t{ Source3x3::Atlas + 129, 3, Source3x3::Height };
								case '\\':
									return glyph_t{ Source3x3::Atlas + 132, 3, Source3x3::Height };
								case ';':
									return glyph_t{ Source3x3::Atlas + 135, 1, Source3x3::Height };
								case ':':
									return glyph_t{ Source3x3::Atlas + 138, 1, Source3x3::Height };
								case '\'':
									return glyph_t{ Source3x3::Atlas + 141, 1, Source3x3::Height };
								case '/':
									return glyph_t{ Source3x3::Atlas + 144, 3, Source3x3::Height };
								default:
									return glyph_t{ nullptr, 0, 0 };
								}
							}

							static dimension_t GetAdvance(const char printableChar)
							{
								switch (printableChar)
								{
								case 'A':
									return 3;
								case 'B':
									return 3;
								case 'C':
									return 3;
								case 'D':
									return 3;
								case 'E':
									return 3;
								case 'F':
									return 3;
								case 'G':
									return 3;
								case 'H':
									return 3;
								case 'I':
									return 1;
								case 'J':
									return 3;
								case 'K':
									return 3;
								case 'L':
									return 2;
								case 'M':
									return 3;
								case 'N':
									return 3;
								case 'O':
									return 3;
								case 'P':
									return 3;
								case 'Q':
									return 3;
								case 'R':
									return 3;
								case 'S':
									return 3;
								case 'T':
									return 3;
								case 'U':
									return 3;
								case 'V':
									return 3;
								case 'W':
									return 3;
								case 'X':
									return 3;
								case 'Y':
									return 3;
								case 'Z':
									return 3;
								case '0':
									return 3;
								case '1':
									return 2;
								case '2':
									return 3;
								case '3':
									return 3;
								case '4':
									return 3;
								case '5':
									return 3;
								case '6':
									return 3;
								case '7':
									return 3;
								case '8':
									return 3;
								case '9':
									return 3;
								case '.':
									return 1;
								case ',':
									return 1;
								case '=':
									return 3;
								case '+':
									return 3;
								case '-':
									return 3;
								case '<':
									return 2;
								case '>':
									return 2;
								case '_':
									return 3;
								case '\\':
									return 3;
								case ';':
									return 1;
								case ':':
									return 1;
								case '\'':
									return 1;
								case '/':
									return 3;
								default:
									return 0;
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