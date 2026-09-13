#ifndef _EGFX_FRAMEWORK_ASSETS_FONT_BITMASK_RESIN_FONT_TYPE_h
#define _EGFX_FRAMEWORK_ASSETS_FONT_BITMASK_RESIN_FONT_TYPE_h

#include <EgfxFramework.h>
#include "Source.h"

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
					/// Monospace 6x6 bitmask font for retro terminal and serial output.
					/// </summary>
					namespace Resin
					{
						struct FontType6x6
						{
							using dimension_t = uint8_t;
							using glyph_t = Framework::Text::Bitmask::GlyphBitmask<dimension_t>;

							static constexpr bool IsMonospace = true;
							static constexpr Framework::DataSourceTypeEnum DataSourceType = Framework::DataSourceTypeEnum::Flash;

							static constexpr dimension_t GetFontWidth() { return Source6x6::Width; }
							static constexpr dimension_t GetFontHeight() { return Source6x6::Height; }

							static dimension_t GetAdvance(const char /*printableChar*/)
							{
								return Source6x6::Width;
							}

							static glyph_t GetGlyph(const uint8_t character)
							{
								switch (static_cast<char>(character))
								{
								case 'A':
									return glyph_t{ Source6x6::Atlas + 0, Source6x6::Width, Source6x6::Height };
								case 'B':
									return glyph_t{ Source6x6::Atlas + 6, Source6x6::Width, Source6x6::Height };
								case 'C':
									return glyph_t{ Source6x6::Atlas + 12, Source6x6::Width, Source6x6::Height };
								case 'D':
									return glyph_t{ Source6x6::Atlas + 18, Source6x6::Width, Source6x6::Height };
								case 'E':
									return glyph_t{ Source6x6::Atlas + 24, Source6x6::Width, Source6x6::Height };
								case 'F':
									return glyph_t{ Source6x6::Atlas + 30, Source6x6::Width, Source6x6::Height };
								case 'G':
									return glyph_t{ Source6x6::Atlas + 36, Source6x6::Width, Source6x6::Height };
								case 'H':
									return glyph_t{ Source6x6::Atlas + 42, Source6x6::Width, Source6x6::Height };
								case 'I':
									return glyph_t{ Source6x6::Atlas + 48, Source6x6::Width, Source6x6::Height };
								case 'J':
									return glyph_t{ Source6x6::Atlas + 54, Source6x6::Width, Source6x6::Height };
								case 'K':
									return glyph_t{ Source6x6::Atlas + 60, Source6x6::Width, Source6x6::Height };
								case 'L':
									return glyph_t{ Source6x6::Atlas + 66, Source6x6::Width, Source6x6::Height };
								case 'M':
									return glyph_t{ Source6x6::Atlas + 72, Source6x6::Width, Source6x6::Height };
								case 'N':
									return glyph_t{ Source6x6::Atlas + 78, Source6x6::Width, Source6x6::Height };
								case 'O':
									return glyph_t{ Source6x6::Atlas + 84, Source6x6::Width, Source6x6::Height };
								case 'P':
									return glyph_t{ Source6x6::Atlas + 90, Source6x6::Width, Source6x6::Height };
								case 'Q':
									return glyph_t{ Source6x6::Atlas + 96, Source6x6::Width, Source6x6::Height };
								case 'R':
									return glyph_t{ Source6x6::Atlas + 102, Source6x6::Width, Source6x6::Height };
								case 'S':
									return glyph_t{ Source6x6::Atlas + 108, Source6x6::Width, Source6x6::Height };
								case 'T':
									return glyph_t{ Source6x6::Atlas + 114, Source6x6::Width, Source6x6::Height };
								case 'U':
									return glyph_t{ Source6x6::Atlas + 120, Source6x6::Width, Source6x6::Height };
								case 'V':
									return glyph_t{ Source6x6::Atlas + 126, Source6x6::Width, Source6x6::Height };
								case 'W':
									return glyph_t{ Source6x6::Atlas + 132, Source6x6::Width, Source6x6::Height };
								case 'X':
									return glyph_t{ Source6x6::Atlas + 138, Source6x6::Width, Source6x6::Height };
								case 'Y':
									return glyph_t{ Source6x6::Atlas + 144, Source6x6::Width, Source6x6::Height };
								case 'Z':
									return glyph_t{ Source6x6::Atlas + 150, Source6x6::Width, Source6x6::Height };
								case '0':
									return glyph_t{ Source6x6::Atlas + 156, Source6x6::Width, Source6x6::Height };
								case '1':
									return glyph_t{ Source6x6::Atlas + 162, Source6x6::Width, Source6x6::Height };
								case '2':
									return glyph_t{ Source6x6::Atlas + 168, Source6x6::Width, Source6x6::Height };
								case '3':
									return glyph_t{ Source6x6::Atlas + 174, Source6x6::Width, Source6x6::Height };
								case '4':
									return glyph_t{ Source6x6::Atlas + 180, Source6x6::Width, Source6x6::Height };
								case '5':
									return glyph_t{ Source6x6::Atlas + 186, Source6x6::Width, Source6x6::Height };
								case '6':
									return glyph_t{ Source6x6::Atlas + 192, Source6x6::Width, Source6x6::Height };
								case '7':
									return glyph_t{ Source6x6::Atlas + 198, Source6x6::Width, Source6x6::Height };
								case '8':
									return glyph_t{ Source6x6::Atlas + 204, Source6x6::Width, Source6x6::Height };
								case '9':
									return glyph_t{ Source6x6::Atlas + 210, Source6x6::Width, Source6x6::Height };
								case '!':
									return glyph_t{ Source6x6::Atlas + 216, Source6x6::Width, Source6x6::Height };
								case '?':
									return glyph_t{ Source6x6::Atlas + 222, Source6x6::Width, Source6x6::Height };
								case '.':
									return glyph_t{ Source6x6::Atlas + 228, Source6x6::Width, Source6x6::Height };
								case ',':
									return glyph_t{ Source6x6::Atlas + 234, Source6x6::Width, Source6x6::Height };
								case '%':
									return glyph_t{ Source6x6::Atlas + 240, Source6x6::Width, Source6x6::Height };
								case '(':
									return glyph_t{ Source6x6::Atlas + 246, Source6x6::Width, Source6x6::Height };
								case ')':
									return glyph_t{ Source6x6::Atlas + 252, Source6x6::Width, Source6x6::Height };
								case '[':
									return glyph_t{ Source6x6::Atlas + 258, Source6x6::Width, Source6x6::Height };
								case ']':
									return glyph_t{ Source6x6::Atlas + 264, Source6x6::Width, Source6x6::Height };
								case '=':
									return glyph_t{ Source6x6::Atlas + 270, Source6x6::Width, Source6x6::Height };
								case '+':
									return glyph_t{ Source6x6::Atlas + 276, Source6x6::Width, Source6x6::Height };
								case '-':
									return glyph_t{ Source6x6::Atlas + 282, Source6x6::Width, Source6x6::Height };
								case '<':
									return glyph_t{ Source6x6::Atlas + 288, Source6x6::Width, Source6x6::Height };
								case '>':
									return glyph_t{ Source6x6::Atlas + 294, Source6x6::Width, Source6x6::Height };
								case 'a':
									return glyph_t{ Source6x6::Atlas + 300, Source6x6::Width, Source6x6::Height };
								case 'b':
									return glyph_t{ Source6x6::Atlas + 306, Source6x6::Width, Source6x6::Height };
								case 'h':
									return glyph_t{ Source6x6::Atlas + 312, Source6x6::Width, Source6x6::Height };
								case 'c':
									return glyph_t{ Source6x6::Atlas + 318, Source6x6::Width, Source6x6::Height };
								case 'd':
									return glyph_t{ Source6x6::Atlas + 324, Source6x6::Width, Source6x6::Height };
								case 'e':
									return glyph_t{ Source6x6::Atlas + 330, Source6x6::Width, Source6x6::Height };
								case 'f':
									return glyph_t{ Source6x6::Atlas + 336, Source6x6::Width, Source6x6::Height };
								case 'g':
									return glyph_t{ Source6x6::Atlas + 342, Source6x6::Width, Source6x6::Height };
								case 'i':
									return glyph_t{ Source6x6::Atlas + 348, Source6x6::Width, Source6x6::Height };
								case 'j':
									return glyph_t{ Source6x6::Atlas + 354, Source6x6::Width, Source6x6::Height };
								case 'k':
									return glyph_t{ Source6x6::Atlas + 360, Source6x6::Width, Source6x6::Height };
								case 'l':
									return glyph_t{ Source6x6::Atlas + 366, Source6x6::Width, Source6x6::Height };
								case 'm':
									return glyph_t{ Source6x6::Atlas + 372, Source6x6::Width, Source6x6::Height };
								case 'n':
									return glyph_t{ Source6x6::Atlas + 378, Source6x6::Width, Source6x6::Height };
								case 'o':
									return glyph_t{ Source6x6::Atlas + 384, Source6x6::Width, Source6x6::Height };
								case 'p':
									return glyph_t{ Source6x6::Atlas + 390, Source6x6::Width, Source6x6::Height };
								case 'r':
									return glyph_t{ Source6x6::Atlas + 396, Source6x6::Width, Source6x6::Height };
								case 'q':
									return glyph_t{ Source6x6::Atlas + 402, Source6x6::Width, Source6x6::Height };
								case 's':
									return glyph_t{ Source6x6::Atlas + 408, Source6x6::Width, Source6x6::Height };
								case 't':
									return glyph_t{ Source6x6::Atlas + 414, Source6x6::Width, Source6x6::Height };
								case 'u':
									return glyph_t{ Source6x6::Atlas + 420, Source6x6::Width, Source6x6::Height };
								case 'v':
									return glyph_t{ Source6x6::Atlas + 426, Source6x6::Width, Source6x6::Height };
								case 'w':
									return glyph_t{ Source6x6::Atlas + 432, Source6x6::Width, Source6x6::Height };
								case 'x':
									return glyph_t{ Source6x6::Atlas + 438, Source6x6::Width, Source6x6::Height };
								case 'y':
									return glyph_t{ Source6x6::Atlas + 444, Source6x6::Width, Source6x6::Height };
								case 'z':
									return glyph_t{ Source6x6::Atlas + 450, Source6x6::Width, Source6x6::Height };
								case '`':
									return glyph_t{ Source6x6::Atlas + 456, Source6x6::Width, Source6x6::Height };
								case '~':
									return glyph_t{ Source6x6::Atlas + 462, Source6x6::Width, Source6x6::Height };
								case '@':
									return glyph_t{ Source6x6::Atlas + 468, Source6x6::Width, Source6x6::Height };
								case '#':
									return glyph_t{ Source6x6::Atlas + 474, Source6x6::Width, Source6x6::Height };
								case '$':
									return glyph_t{ Source6x6::Atlas + 480, Source6x6::Width, Source6x6::Height };
								case '^':
									return glyph_t{ Source6x6::Atlas + 486, Source6x6::Width, Source6x6::Height };
								case '&':
									return glyph_t{ Source6x6::Atlas + 492, Source6x6::Width, Source6x6::Height };
								case '*':
									return glyph_t{ Source6x6::Atlas + 498, Source6x6::Width, Source6x6::Height };
								case '_':
									return glyph_t{ Source6x6::Atlas + 504, Source6x6::Width, Source6x6::Height };
								case '{':
									return glyph_t{ Source6x6::Atlas + 510, Source6x6::Width, Source6x6::Height };
								case '}':
									return glyph_t{ Source6x6::Atlas + 516, Source6x6::Width, Source6x6::Height };
								case '\\':
									return glyph_t{ Source6x6::Atlas + 522, Source6x6::Width, Source6x6::Height };
								case '|':
									return glyph_t{ Source6x6::Atlas + 528, Source6x6::Width, Source6x6::Height };
								case ';':
									return glyph_t{ Source6x6::Atlas + 534, Source6x6::Width, Source6x6::Height };
								case ':':
									return glyph_t{ Source6x6::Atlas + 540, Source6x6::Width, Source6x6::Height };
								case '\'':
									return glyph_t{ Source6x6::Atlas + 546, Source6x6::Width, Source6x6::Height };
								case '"':
									return glyph_t{ Source6x6::Atlas + 552, Source6x6::Width, Source6x6::Height };
								case '/':
									return glyph_t{ Source6x6::Atlas + 558, Source6x6::Width, Source6x6::Height };
								default:
									return glyph_t{ nullptr, 0, 0 };
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
