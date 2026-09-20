#ifndef _INTEGERGLASS_MODULES_KIT_FONT_BITMAP_CONTACT_CHROMATIC_FONT_TYPE_h
#define _INTEGERGLASS_MODULES_KIT_FONT_BITMAP_CONTACT_CHROMATIC_FONT_TYPE_h

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
				namespace Bitmap
				{
					namespace ContactChromatic
					{
						namespace AsciiDefinition = Framework::AsciiDefinition;

						struct FontType5x6
						{
							using dimension_t = uint8_t;
							using color_t = uint8_t;
							using glyph_t = Framework::Text::Bitmap::GlyphBitmap<dimension_t, color_t>;
							
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
									return glyph_t{ Source5x6::Atlas + 24, 4, Source5x6::Height };
								case 'C':
									return glyph_t{ Source5x6::Atlas + 48, 4, Source5x6::Height };
								case 'D':
									return glyph_t{ Source5x6::Atlas + 72, 4, Source5x6::Height };
								case 'E':
									return glyph_t{ Source5x6::Atlas + 96, 4, Source5x6::Height };
								case 'F':
									return glyph_t{ Source5x6::Atlas + 120, 4, Source5x6::Height };
								case 'G':
									return glyph_t{ Source5x6::Atlas + 144, 5, Source5x6::Height };
								case 'H':
									return glyph_t{ Source5x6::Atlas + 174, 4, Source5x6::Height };
								case 'I':
									return glyph_t{ Source5x6::Atlas + 198, 3, Source5x6::Height };
								case 'J':
									return glyph_t{ Source5x6::Atlas + 216, 4, Source5x6::Height };
								case 'K':
									return glyph_t{ Source5x6::Atlas + 240, 4, Source5x6::Height };
								case 'L':
									return glyph_t{ Source5x6::Atlas + 264, 4, Source5x6::Height };
								case 'M':
									return glyph_t{ Source5x6::Atlas + 288, 5, Source5x6::Height };
								case 'N':
									return glyph_t{ Source5x6::Atlas + 318, 4, Source5x6::Height };
								case 'O':
									return glyph_t{ Source5x6::Atlas + 342, 4, Source5x6::Height };
								case 'P':
									return glyph_t{ Source5x6::Atlas + 366, 4, Source5x6::Height };
								case 'Q':
									return glyph_t{ Source5x6::Atlas + 390, 4, Source5x6::Height };
								case 'R':
									return glyph_t{ Source5x6::Atlas + 414, 4, Source5x6::Height };
								case 'S':
									return glyph_t{ Source5x6::Atlas + 438, 4, Source5x6::Height };
								case 'T':
									return glyph_t{ Source5x6::Atlas + 462, 5, Source5x6::Height };
								case 'U':
									return glyph_t{ Source5x6::Atlas + 492, 4, Source5x6::Height };
								case 'V':
									return glyph_t{ Source5x6::Atlas + 516, 5, Source5x6::Height };
								case 'W':
									return glyph_t{ Source5x6::Atlas + 546, 5, Source5x6::Height };
								case 'X':
									return glyph_t{ Source5x6::Atlas + 576, 5, Source5x6::Height };
								case 'Y':
									return glyph_t{ Source5x6::Atlas + 606, 5, Source5x6::Height };
								case 'Z':
									return glyph_t{ Source5x6::Atlas + 636, 5, Source5x6::Height };
								case '0':
									return glyph_t{ Source5x6::Atlas + 666, 4, Source5x6::Height };
								case '1':
									return glyph_t{ Source5x6::Atlas + 690, 2, Source5x6::Height };
								case '2':
									return glyph_t{ Source5x6::Atlas + 702, 4, Source5x6::Height };
								case '3':
									return glyph_t{ Source5x6::Atlas + 726, 4, Source5x6::Height };
								case '4':
									return glyph_t{ Source5x6::Atlas + 750, 4, Source5x6::Height };
								case '5':
									return glyph_t{ Source5x6::Atlas + 774, 4, Source5x6::Height };
								case '6':
									return glyph_t{ Source5x6::Atlas + 798, 4, Source5x6::Height };
								case '7':
									return glyph_t{ Source5x6::Atlas + 822, 4, Source5x6::Height };
								case '8':
									return glyph_t{ Source5x6::Atlas + 846, 4, Source5x6::Height };
								case '9':
									return glyph_t{ Source5x6::Atlas + 870, 4, Source5x6::Height };
								case '!':
									return glyph_t{ Source5x6::Atlas + 894, 1, Source5x6::Height };
								case '?':
									return glyph_t{ Source5x6::Atlas + 900, 4, Source5x6::Height };
								case '.':
									return glyph_t{ Source5x6::Atlas + 924, 1, Source5x6::Height };
								case ',':
									return glyph_t{ Source5x6::Atlas + 930, 2, Source5x6::Height };
								case '%':
									return glyph_t{ Source5x6::Atlas + 942, 4, Source5x6::Height };
								case '(':
									return glyph_t{ Source5x6::Atlas + 966, 2, Source5x6::Height };
								case ')':
									return glyph_t{ Source5x6::Atlas + 978, 2, Source5x6::Height };
								case '[':
									return glyph_t{ Source5x6::Atlas + 990, 2, Source5x6::Height };
								case ']':
									return glyph_t{ Source5x6::Atlas + 1002, 2, Source5x6::Height };
								case '=':
									return glyph_t{ Source5x6::Atlas + 1014, 3, Source5x6::Height };
								case '+':
									return glyph_t{ Source5x6::Atlas + 1032, 3, Source5x6::Height };
								case '-':
									return glyph_t{ Source5x6::Atlas + 1050, 3, Source5x6::Height };
								case '<':
									return glyph_t{ Source5x6::Atlas + 1068, 3, Source5x6::Height };
								case '>':
									return glyph_t{ Source5x6::Atlas + 1086, 3, Source5x6::Height };
								case 'a':
									return glyph_t{ Source5x6::Atlas + 1104, 4, Source5x6::Height };
								case 'b':
									return glyph_t{ Source5x6::Atlas + 1128, 4, Source5x6::Height };
								case 'h':
									return glyph_t{ Source5x6::Atlas + 1152, 3, Source5x6::Height };
								case 'c':
									return glyph_t{ Source5x6::Atlas + 1170, 3, Source5x6::Height };
								case 'd':
									return glyph_t{ Source5x6::Atlas + 1188, 4, Source5x6::Height };
								case 'e':
									return glyph_t{ Source5x6::Atlas + 1212, 4, Source5x6::Height };
								case 'f':
									return glyph_t{ Source5x6::Atlas + 1236, 2, Source5x6::Height };
								case 'g':
									return glyph_t{ Source5x6::Atlas + 1248, 3, Source5x6::Height };
								case 'i':
									return glyph_t{ Source5x6::Atlas + 1266, 1, Source5x6::Height };
								case 'j':
									return glyph_t{ Source5x6::Atlas + 1272, 2, Source5x6::Height };
								case 'k':
									return glyph_t{ Source5x6::Atlas + 1284, 3, Source5x6::Height };
								case 'l':
									return glyph_t{ Source5x6::Atlas + 1302, 1, Source5x6::Height };
								case 'm':
									return glyph_t{ Source5x6::Atlas + 1308, 5, Source5x6::Height };
								case 'n':
									return glyph_t{ Source5x6::Atlas + 1338, 3, Source5x6::Height };
								case 'o':
									return glyph_t{ Source5x6::Atlas + 1356, 4, Source5x6::Height };
								case 'p':
									return glyph_t{ Source5x6::Atlas + 1380, 3, Source5x6::Height };
								case 'r':
									return glyph_t{ Source5x6::Atlas + 1398, 3, Source5x6::Height };
								case 'q':
									return glyph_t{ Source5x6::Atlas + 1416, 3, Source5x6::Height };
								case 's':
									return glyph_t{ Source5x6::Atlas + 1434, 3, Source5x6::Height };
								case 't':
									return glyph_t{ Source5x6::Atlas + 1452, 3, Source5x6::Height };
								case 'u':
									return glyph_t{ Source5x6::Atlas + 1470, 3, Source5x6::Height };
								case 'v':
									return glyph_t{ Source5x6::Atlas + 1488, 3, Source5x6::Height };
								case 'w':
									return glyph_t{ Source5x6::Atlas + 1506, 5, Source5x6::Height };
								case 'x':
									return glyph_t{ Source5x6::Atlas + 1536, 3, Source5x6::Height };
								case 'y':
									return glyph_t{ Source5x6::Atlas + 1554, 3, Source5x6::Height };
								case 'z':
									return glyph_t{ Source5x6::Atlas + 1572, 3, Source5x6::Height };
								case '`':
									return glyph_t{ Source5x6::Atlas + 1590, 2, Source5x6::Height };
								case '~':
									return glyph_t{ Source5x6::Atlas + 1602, 4, Source5x6::Height };
								case '@':
									return glyph_t{ Source5x6::Atlas + 1626, 5, Source5x6::Height };
								case '#':
									return glyph_t{ Source5x6::Atlas + 1656, 5, Source5x6::Height };
								case '$':
									return glyph_t{ Source5x6::Atlas + 1686, 4, Source5x6::Height };
								case '^':
									return glyph_t{ Source5x6::Atlas + 1710, 3, Source5x6::Height };
								case '&':
									return glyph_t{ Source5x6::Atlas + 1728, 4, Source5x6::Height };
								case '*':
									return glyph_t{ Source5x6::Atlas + 1752, 3, Source5x6::Height };
								case '_':
									return glyph_t{ Source5x6::Atlas + 1770, 5, Source5x6::Height };
								case '{':
									return glyph_t{ Source5x6::Atlas + 1800, 3, Source5x6::Height };
								case '}':
									return glyph_t{ Source5x6::Atlas + 1818, 3, Source5x6::Height };
								case '\\':
									return glyph_t{ Source5x6::Atlas + 1836, 4, Source5x6::Height };
								case '|':
									return glyph_t{ Source5x6::Atlas + 1860, 1, Source5x6::Height };
								case ';':
									return glyph_t{ Source5x6::Atlas + 1866, 2, Source5x6::Height };
								case ':':
									return glyph_t{ Source5x6::Atlas + 1878, 1, Source5x6::Height };
								case '\'':
									return glyph_t{ Source5x6::Atlas + 1884, 1, Source5x6::Height };
								case '"':
									return glyph_t{ Source5x6::Atlas + 1890, 3, Source5x6::Height };
								case '/':
									return glyph_t{ Source5x6::Atlas + 1908, 4, Source5x6::Height };
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