#ifndef _INTEGERGLASS_MODULES_KIT_FONT_BITMASK_CONTACT_FONT_TYPE_h
#define _INTEGERGLASS_MODULES_KIT_FONT_BITMASK_CONTACT_FONT_TYPE_h

#include <IntegerGlassFramework.h>
#include "Source.h"

namespace IntegerGlass
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
					/// Simple font for small sizes with clear legibility.
					/// Uppercase-only letters.
					/// </summary>
					namespace Contact
					{
						struct FontType5x6
						{
							using dimension_t = uint8_t;
							using glyph_t = Framework::Text::Bitmask::GlyphBitmask<dimension_t>;

							static constexpr bool IsMonospace = false;
							static constexpr Framework::DataSourceTypeEnum DataSourceType = Framework::DataSourceTypeEnum::Flash;

							static constexpr dimension_t GetFontWidth() { return Source5x6::Width; }
							static constexpr dimension_t GetFontHeight() { return Source5x6::Height; }


							static glyph_t GetGlyph(const uint8_t character)
							{
								switch (static_cast<char>(character))
								{
								case 'A':
									return glyph_t{ Source5x6::Atlas + 0, 4, Source5x6::Height };
								case 'B':
									return glyph_t{ Source5x6::Atlas + 6, 4, Source5x6::Height };
								case 'C':
									return glyph_t{ Source5x6::Atlas + 12, 4, Source5x6::Height };
								case 'D':
									return glyph_t{ Source5x6::Atlas + 18, 4, Source5x6::Height };
								case 'E':
									return glyph_t{ Source5x6::Atlas + 24, 4, Source5x6::Height };
								case 'F':
									return glyph_t{ Source5x6::Atlas + 30, 4, Source5x6::Height };
								case 'G':
									return glyph_t{ Source5x6::Atlas + 36, 5, Source5x6::Height };
								case 'H':
									return glyph_t{ Source5x6::Atlas + 42, 4, Source5x6::Height };
								case 'I':
									return glyph_t{ Source5x6::Atlas + 48, 3, Source5x6::Height };
								case 'J':
									return glyph_t{ Source5x6::Atlas + 54, 4, Source5x6::Height };
								case 'K':
									return glyph_t{ Source5x6::Atlas + 60, 4, Source5x6::Height };
								case 'L':
									return glyph_t{ Source5x6::Atlas + 66, 4, Source5x6::Height };
								case 'M':
									return glyph_t{ Source5x6::Atlas + 72, 5, Source5x6::Height };
								case 'N':
									return glyph_t{ Source5x6::Atlas + 78, 4, Source5x6::Height };
								case 'O':
									return glyph_t{ Source5x6::Atlas + 84, 4, Source5x6::Height };
								case 'P':
									return glyph_t{ Source5x6::Atlas + 90, 4, Source5x6::Height };
								case 'Q':
									return glyph_t{ Source5x6::Atlas + 96, 4, Source5x6::Height };
								case 'R':
									return glyph_t{ Source5x6::Atlas + 102, 4, Source5x6::Height };
								case 'S':
									return glyph_t{ Source5x6::Atlas + 108, 4, Source5x6::Height };
								case 'T':
									return glyph_t{ Source5x6::Atlas + 114, 5, Source5x6::Height };
								case 'U':
									return glyph_t{ Source5x6::Atlas + 120, 4, Source5x6::Height };
								case 'V':
									return glyph_t{ Source5x6::Atlas + 126, 5, Source5x6::Height };
								case 'W':
									return glyph_t{ Source5x6::Atlas + 132, 5, Source5x6::Height };
								case 'X':
									return glyph_t{ Source5x6::Atlas + 138, 5, Source5x6::Height };
								case 'Y':
									return glyph_t{ Source5x6::Atlas + 144, 5, Source5x6::Height };
								case 'Z':
									return glyph_t{ Source5x6::Atlas + 150, 5, Source5x6::Height };
								case '0':
									return glyph_t{ Source5x6::Atlas + 156, 4, Source5x6::Height };
								case '1':
									return glyph_t{ Source5x6::Atlas + 162, 2, Source5x6::Height };
								case '2':
									return glyph_t{ Source5x6::Atlas + 168, 4, Source5x6::Height };
								case '3':
									return glyph_t{ Source5x6::Atlas + 174, 4, Source5x6::Height };
								case '4':
									return glyph_t{ Source5x6::Atlas + 180, 4, Source5x6::Height };
								case '5':
									return glyph_t{ Source5x6::Atlas + 186, 4, Source5x6::Height };
								case '6':
									return glyph_t{ Source5x6::Atlas + 192, 4, Source5x6::Height };
								case '7':
									return glyph_t{ Source5x6::Atlas + 198, 4, Source5x6::Height };
								case '8':
									return glyph_t{ Source5x6::Atlas + 204, 4, Source5x6::Height };
								case '9':
									return glyph_t{ Source5x6::Atlas + 210, 4, Source5x6::Height };
								case '!':
									return glyph_t{ Source5x6::Atlas + 216, 1, Source5x6::Height };
								case '?':
									return glyph_t{ Source5x6::Atlas + 222, 4, Source5x6::Height };
								case '.':
									return glyph_t{ Source5x6::Atlas + 228, 1, Source5x6::Height };
								case ',':
									return glyph_t{ Source5x6::Atlas + 234, 2, Source5x6::Height };
								case '%':
									return glyph_t{ Source5x6::Atlas + 240, 4, Source5x6::Height };
								case '(':
									return glyph_t{ Source5x6::Atlas + 246, 2, Source5x6::Height };
								case ')':
									return glyph_t{ Source5x6::Atlas + 252, 2, Source5x6::Height };
								case '[':
									return glyph_t{ Source5x6::Atlas + 258, 2, Source5x6::Height };
								case ']':
									return glyph_t{ Source5x6::Atlas + 264, 2, Source5x6::Height };
								case '=':
									return glyph_t{ Source5x6::Atlas + 270, 3, Source5x6::Height };
								case '+':
									return glyph_t{ Source5x6::Atlas + 276, 3, Source5x6::Height };
								case '-':
									return glyph_t{ Source5x6::Atlas + 282, 3, Source5x6::Height };
								case '<':
									return glyph_t{ Source5x6::Atlas + 288, 3, Source5x6::Height };
								case '>':
									return glyph_t{ Source5x6::Atlas + 294, 3, Source5x6::Height };
								case 'a':
									return glyph_t{ Source5x6::Atlas + 300, 4, Source5x6::Height };
								case 'b':
									return glyph_t{ Source5x6::Atlas + 306, 4, Source5x6::Height };
								case 'h':
									return glyph_t{ Source5x6::Atlas + 312, 3, Source5x6::Height };
								case 'c':
									return glyph_t{ Source5x6::Atlas + 318, 3, Source5x6::Height };
								case 'd':
									return glyph_t{ Source5x6::Atlas + 324, 4, Source5x6::Height };
								case 'e':
									return glyph_t{ Source5x6::Atlas + 330, 4, Source5x6::Height };
								case 'f':
									return glyph_t{ Source5x6::Atlas + 336, 2, Source5x6::Height };
								case 'g':
									return glyph_t{ Source5x6::Atlas + 342, 3, Source5x6::Height };
								case 'i':
									return glyph_t{ Source5x6::Atlas + 348, 1, Source5x6::Height };
								case 'j':
									return glyph_t{ Source5x6::Atlas + 354, 2, Source5x6::Height };
								case 'k':
									return glyph_t{ Source5x6::Atlas + 360, 3, Source5x6::Height };
								case 'l':
									return glyph_t{ Source5x6::Atlas + 366, 1, Source5x6::Height };
								case 'm':
									return glyph_t{ Source5x6::Atlas + 372, 5, Source5x6::Height };
								case 'n':
									return glyph_t{ Source5x6::Atlas + 378, 3, Source5x6::Height };
								case 'o':
									return glyph_t{ Source5x6::Atlas + 384, 4, Source5x6::Height };
								case 'p':
									return glyph_t{ Source5x6::Atlas + 390, 3, Source5x6::Height };
								case 'r':
									return glyph_t{ Source5x6::Atlas + 396, 3, Source5x6::Height };
								case 'q':
									return glyph_t{ Source5x6::Atlas + 402, 3, Source5x6::Height };
								case 's':
									return glyph_t{ Source5x6::Atlas + 408, 3, Source5x6::Height };
								case 't':
									return glyph_t{ Source5x6::Atlas + 414, 3, Source5x6::Height };
								case 'u':
									return glyph_t{ Source5x6::Atlas + 420, 3, Source5x6::Height };
								case 'v':
									return glyph_t{ Source5x6::Atlas + 426, 3, Source5x6::Height };
								case 'w':
									return glyph_t{ Source5x6::Atlas + 432, 5, Source5x6::Height };
								case 'x':
									return glyph_t{ Source5x6::Atlas + 438, 3, Source5x6::Height };
								case 'y':
									return glyph_t{ Source5x6::Atlas + 444, 3, Source5x6::Height };
								case 'z':
									return glyph_t{ Source5x6::Atlas + 450, 3, Source5x6::Height };
								case '`':
									return glyph_t{ Source5x6::Atlas + 456, 2, Source5x6::Height };
								case '~':
									return glyph_t{ Source5x6::Atlas + 462, 4, Source5x6::Height };
								case '@':
									return glyph_t{ Source5x6::Atlas + 468, 5, Source5x6::Height };
								case '#':
									return glyph_t{ Source5x6::Atlas + 474, 5, Source5x6::Height };
								case '$':
									return glyph_t{ Source5x6::Atlas + 480, 4, Source5x6::Height };
								case '^':
									return glyph_t{ Source5x6::Atlas + 486, 3, Source5x6::Height };
								case '&':
									return glyph_t{ Source5x6::Atlas + 492, 4, Source5x6::Height };
								case '*':
									return glyph_t{ Source5x6::Atlas + 498, 3, Source5x6::Height };
								case '_':
									return glyph_t{ Source5x6::Atlas + 504, 5, Source5x6::Height };
								case '{':
									return glyph_t{ Source5x6::Atlas + 510, 3, Source5x6::Height };
								case '}':
									return glyph_t{ Source5x6::Atlas + 516, 3, Source5x6::Height };
								case '\\':
									return glyph_t{ Source5x6::Atlas + 522, 4, Source5x6::Height };
								case '|':
									return glyph_t{ Source5x6::Atlas + 528, 1, Source5x6::Height };
								case ';':
									return glyph_t{ Source5x6::Atlas + 534, 2, Source5x6::Height };
								case ':':
									return glyph_t{ Source5x6::Atlas + 540, 1, Source5x6::Height };
								case '\'':
									return glyph_t{ Source5x6::Atlas + 546, 1, Source5x6::Height };
								case '"':
									return glyph_t{ Source5x6::Atlas + 552, 3, Source5x6::Height };
								case '/':
									return glyph_t{ Source5x6::Atlas + 558, 4, Source5x6::Height };
								default:
									return glyph_t{ nullptr, 0, 0 };
								}
							}

							static dimension_t GetAdvance(const char printableChar)
							{
								switch (printableChar)
								{
								case 'A':
									return 4;
								case 'B':
									return 4;
								case 'C':
									return 4;
								case 'D':
									return 4;
								case 'E':
									return 4;
								case 'F':
									return 4;
								case 'G':
									return 5;
								case 'H':
									return 4;
								case 'I':
									return 3;
								case 'J':
									return 4;
								case 'K':
									return 4;
								case 'L':
									return 4;
								case 'M':
									return 5;
								case 'N':
									return 4;
								case 'O':
									return 4;
								case 'P':
									return 4;
								case 'Q':
									return 4;
								case 'R':
									return 4;
								case 'S':
									return 4;
								case 'T':
									return 5;
								case 'U':
									return 4;
								case 'V':
									return 5;
								case 'W':
									return 5;
								case 'X':
									return 5;
								case 'Y':
									return 5;
								case 'Z':
									return 5;
								case '0':
									return 4;
								case '1':
									return 2;
								case '2':
									return 4;
								case '3':
									return 4;
								case '4':
									return 4;
								case '5':
									return 4;
								case '6':
									return 4;
								case '7':
									return 4;
								case '8':
									return 4;
								case '9':
									return 4;
								case '!':
									return 1;
								case '?':
									return 4;
								case '.':
									return 1;
								case ',':
									return 2;
								case '%':
									return 4;
								case '(':
									return 2;
								case ')':
									return 2;
								case '[':
									return 2;
								case ']':
									return 2;
								case '=':
									return 3;
								case '+':
									return 3;
								case '-':
									return 3;
								case '<':
									return 3;
								case '>':
									return 3;
								case 'a':
									return 4;
								case 'b':
									return 4;
								case 'h':
									return 3;
								case 'c':
									return 3;
								case 'd':
									return 4;
								case 'e':
									return 4;
								case 'f':
									return 2;
								case 'g':
									return 3;
								case 'i':
									return 1;
								case 'j':
									return 2;
								case 'k':
									return 3;
								case 'l':
									return 1;
								case 'm':
									return 5;
								case 'n':
									return 3;
								case 'o':
									return 4;
								case 'p':
									return 3;
								case 'r':
									return 3;
								case 'q':
									return 3;
								case 's':
									return 3;
								case 't':
									return 3;
								case 'u':
									return 3;
								case 'v':
									return 3;
								case 'w':
									return 5;
								case 'x':
									return 3;
								case 'y':
									return 3;
								case 'z':
									return 3;
								case '`':
									return 2;
								case '~':
									return 4;
								case '@':
									return 5;
								case '#':
									return 5;
								case '$':
									return 4;
								case '^':
									return 3;
								case '&':
									return 4;
								case '*':
									return 3;
								case '_':
									return 5;
								case '{':
									return 3;
								case '}':
									return 3;
								case '\\':
									return 4;
								case '|':
									return 1;
								case ';':
									return 2;
								case ':':
									return 1;
								case '\'':
									return 1;
								case '"':
									return 3;
								case '/':
									return 4;
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