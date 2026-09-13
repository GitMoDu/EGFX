#ifndef _EGFX_FRAMEWORK_ASSETS_FONT_VECTOR_EPOXY_FONT_TYPE_h
#define _EGFX_FRAMEWORK_ASSETS_FONT_VECTOR_EPOXY_FONT_TYPE_h

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
				namespace Vector
				{
					namespace Epoxy
					{
						struct FontType8x8
						{
							using dimension_t = uint8_t;
							using glyph_t = Egfx::Framework::Text::Vector::Glyph15x15::Glyph;

							static constexpr bool IsMonospace = false;
							static constexpr dimension_t Width = Source8x8::Width;
							static constexpr dimension_t Height = Source8x8::Height;
							static constexpr dimension_t Inset = 1;
							static constexpr Egfx::Framework::DataSourceTypeEnum DataSourceType = Egfx::Framework::DataSourceTypeEnum::Flash;
							static constexpr Egfx::Framework::Vector::PackedVectorEnum VectorType = Egfx::Framework::Vector::PackedVectorEnum::Image15x15;

							static glyph_t GetGlyph(const uint8_t character)
							{
								switch (static_cast<char>(character))
								{
								case 'A':
									return glyph_t{ Source8x8::Atlas + 0, 12, 5, Height };
								case 'T':
									return glyph_t{ Source8x8::Atlas + 12, 5, 5, Height };
								case 'B':
									return glyph_t{ Source8x8::Atlas + 17, 13, 5, Height };
								case 'h':
									return glyph_t{ Source8x8::Atlas + 30, 8, 4, Height };
								case 'e':
									return glyph_t{ Source8x8::Atlas + 38, 10, 4, Height };
								case '5':
									return glyph_t{ Source8x8::Atlas + 48, 9, 5, Height };
								case '0':
									return glyph_t{ Source8x8::Atlas + 57, 12, 5, Height };
								case 'M':
									return glyph_t{ Source8x8::Atlas + 69, 11, 7, Height };
								case 'N':
									return glyph_t{ Source8x8::Atlas + 80, 5, 5, Height };
								case 'q':
									return glyph_t{ Source8x8::Atlas + 85, 11, 4, Height };
								case 'S':
									return glyph_t{ Source8x8::Atlas + 96, 12, 5, Height };
								case 'Q':
									return glyph_t{ Source8x8::Atlas + 108, 12, 5, Height };
								case 'u':
									return glyph_t{ Source8x8::Atlas + 120, 6, 4, Height };
								case 'i':
									return glyph_t{ Source8x8::Atlas + 126, 5, 1, Height };
								case 'c':
									return glyph_t{ Source8x8::Atlas + 131, 8, 4, Height };
								case 'k':
									return glyph_t{ Source8x8::Atlas + 139, 8, 4, Height };
								case 'b':
									return glyph_t{ Source8x8::Atlas + 147, 11, 4, Height };
								case 'r':
									return glyph_t{ Source8x8::Atlas + 158, 6, 3, Height };
								case 'o':
									return glyph_t{ Source8x8::Atlas + 164, 9, 4, Height };
								case 'p':
									return glyph_t{ Source8x8::Atlas + 173, 9, 4, Height };
								case 'w':
									return glyph_t{ Source8x8::Atlas + 182, 5, 5, Height };
								case 'n':
									return glyph_t{ Source8x8::Atlas + 187, 8, 4, Height };
								case 'f':
									return glyph_t{ Source8x8::Atlas + 195, 7, 3, Height };
								case 'x':
									return glyph_t{ Source8x8::Atlas + 202, 10, 3, Height };
								case 'j':
									return glyph_t{ Source8x8::Atlas + 212, 5, 2, Height };
								case 'm':
									return glyph_t{ Source8x8::Atlas + 217, 14, 7, Height };
								case 'v':
									return glyph_t{ Source8x8::Atlas + 231, 3, 4, Height };
								case 's':
									return glyph_t{ Source8x8::Atlas + 234, 8, 4, Height };
								case 't':
									return glyph_t{ Source8x8::Atlas + 242, 6, 3, Height };
								case 'l':
									return glyph_t{ Source8x8::Atlas + 248, 3, 2, Height };
								case 'a':
									return glyph_t{ Source8x8::Atlas + 251, 13, 4, Height };
								case 'z':
									return glyph_t{ Source8x8::Atlas + 264, 6, 4, Height };
								case 'y':
									return glyph_t{ Source8x8::Atlas + 270, 6, 4, Height };
								case 'd':
									return glyph_t{ Source8x8::Atlas + 276, 11, 4, Height };
								case 'g':
									return glyph_t{ Source8x8::Atlas + 287, 10, 4, Height };
								case 'C':
									return glyph_t{ Source8x8::Atlas + 297, 8, 5, Height };
								case 'D':
									return glyph_t{ Source8x8::Atlas + 305, 7, 5, Height };
								case 'E':
									return glyph_t{ Source8x8::Atlas + 312, 7, 4, Height };
								case 'F':
									return glyph_t{ Source8x8::Atlas + 319, 6, 4, Height };
								case 'H':
									return glyph_t{ Source8x8::Atlas + 325, 8, 5, Height };
								case 'G':
									return glyph_t{ Source8x8::Atlas + 333, 10, 5, Height };
								case 'I':
									return glyph_t{ Source8x8::Atlas + 343, 2, 1, Height };
								case 'J':
									return glyph_t{ Source8x8::Atlas + 345, 5, 4, Height };
								case 'K':
									return glyph_t{ Source8x8::Atlas + 350, 9, 5, Height };
								case 'L':
									return glyph_t{ Source8x8::Atlas + 359, 3, 4, Height };
								case 'O':
									return glyph_t{ Source8x8::Atlas + 362, 9, 5, Height };
								case 'P':
									return glyph_t{ Source8x8::Atlas + 371, 7, 5, Height };
								case 'R':
									return glyph_t{ Source8x8::Atlas + 378, 11, 5, Height };
								case 'U':
									return glyph_t{ Source8x8::Atlas + 389, 6, 5, Height };
								case 'V':
									return glyph_t{ Source8x8::Atlas + 395, 3, 5, Height };
								case 'W':
									return glyph_t{ Source8x8::Atlas + 398, 5, 7, Height };
								case 'X':
									return glyph_t{ Source8x8::Atlas + 403, 5, 5, Height };
								case 'Y':
									return glyph_t{ Source8x8::Atlas + 408, 6, 5, Height };
								case 'Z':
									return glyph_t{ Source8x8::Atlas + 414, 4, 5, Height };
								case '!':
									return glyph_t{ Source8x8::Atlas + 418, 4, 1, Height };
								case '1':
									return glyph_t{ Source8x8::Atlas + 422, 3, 2, Height };
								case '2':
									return glyph_t{ Source8x8::Atlas + 425, 8, 5, Height };
								case '3':
									return glyph_t{ Source8x8::Atlas + 433, 14, 5, Height };
								case '4':
									return glyph_t{ Source8x8::Atlas + 447, 6, 5, Height };
								case '6':
									return glyph_t{ Source8x8::Atlas + 453, 10, 5, Height };
								case '7':
									return glyph_t{ Source8x8::Atlas + 463, 5, 5, Height };
								case '8':
									return glyph_t{ Source8x8::Atlas + 468, 18, 5, Height };
								case '9':
									return glyph_t{ Source8x8::Atlas + 486, 8, 5, Height };
								case '@':
									return glyph_t{ Source8x8::Atlas + 494, 16, 8, Height };
								case '?':
									return glyph_t{ Source8x8::Atlas + 510, 10, 5, Height };
								case '>':
									return glyph_t{ Source8x8::Atlas + 520, 3, 4, Height };
								case '<':
									return glyph_t{ Source8x8::Atlas + 523, 3, 4, Height };
								case '#':
									return glyph_t{ Source8x8::Atlas + 526, 11, 8, Height };
								case '$':
									return glyph_t{ Source8x8::Atlas + 537, 15, 5, Height };
								case '%':
									return glyph_t{ Source8x8::Atlas + 552, 8, 5, Height };
								case '&':
									return glyph_t{ Source8x8::Atlas + 560, 14, 5, Height };
								case '{':
									return glyph_t{ Source8x8::Atlas + 574, 7, 3, Height };
								case '|':
									return glyph_t{ Source8x8::Atlas + 581, 2, 1, Height };
								case '}':
									return glyph_t{ Source8x8::Atlas + 583, 7, 3, Height };
								case ')':
									return glyph_t{ Source8x8::Atlas + 590, 4, 2, Height };
								case '*':
									return glyph_t{ Source8x8::Atlas + 594, 14, 10, Height };
								case '(':
									return glyph_t{ Source8x8::Atlas + 608, 4, 2, Height };
								case '+':
									return glyph_t{ Source8x8::Atlas + 612, 5, 3, Height };
								case ',':
									return glyph_t{ Source8x8::Atlas + 617, 2, 1, Height };
								case '-':
									return glyph_t{ Source8x8::Atlas + 619, 2, 3, Height };
								case '.':
									return glyph_t{ Source8x8::Atlas + 621, 1, 1, Height };
								case '/':
									return glyph_t{ Source8x8::Atlas + 622, 2, 5, Height };
								case '\\':
									return glyph_t{ Source8x8::Atlas + 624, 2, 5, Height };
								case ':':
									return glyph_t{ Source8x8::Atlas + 626, 3, 1, Height };
								case ';':
									return glyph_t{ Source8x8::Atlas + 629, 4, 1, Height };
								case '=':
									return glyph_t{ Source8x8::Atlas + 633, 5, 4, Height };
								case ']':
									return glyph_t{ Source8x8::Atlas + 638, 4, 2, Height };
								case '[':
									return glyph_t{ Source8x8::Atlas + 642, 4, 2, Height };
								case '^':
									return glyph_t{ Source8x8::Atlas + 646, 3, 3, Height };
								case '~':
									return glyph_t{ Source8x8::Atlas + 649, 4, 4, Height };
								case '`':
									return glyph_t{ Source8x8::Atlas + 653, 2, 2, Height };
								case '_':
									return glyph_t{ Source8x8::Atlas + 655, 2, 5, Height };
								case '"':
									return glyph_t{ Source8x8::Atlas + 657, 5, 3, Height };
								default:
									return glyph_t{ nullptr, 0, 0, 0 };
								}
							}

							static dimension_t GetAdvance(const char printableChar)
							{
								switch (printableChar)
								{
								case 'A':
									return 5;
								case 'T':
									return 5;
								case 'B':
									return 5;
								case 'h':
									return 4;
								case 'e':
									return 4;
								case '5':
									return 5;
								case '0':
									return 5;
								case 'M':
									return 7;
								case 'N':
									return 5;
								case 'q':
									return 4;
								case 'S':
									return 5;
								case 'Q':
									return 5;
								case 'u':
									return 4;
								case 'i':
									return 1;
								case 'c':
									return 4;
								case 'k':
									return 4;
								case 'b':
									return 4;
								case 'r':
									return 3;
								case 'o':
									return 4;
								case 'p':
									return 4;
								case 'w':
									return 5;
								case 'n':
									return 4;
								case 'f':
									return 3;
								case 'x':
									return 3;
								case 'j':
									return 2;
								case 'm':
									return 7;
								case 'v':
									return 4;
								case 's':
									return 4;
								case 't':
									return 3;
								case 'l':
									return 2;
								case 'a':
									return 4;
								case 'z':
									return 4;
								case 'y':
									return 4;
								case 'd':
									return 4;
								case 'g':
									return 4;
								case 'C':
									return 5;
								case 'D':
									return 5;
								case 'E':
									return 4;
								case 'F':
									return 4;
								case 'H':
									return 5;
								case 'G':
									return 5;
								case 'I':
									return 1;
								case 'J':
									return 4;
								case 'K':
									return 5;
								case 'L':
									return 4;
								case 'O':
									return 5;
								case 'P':
									return 5;
								case 'R':
									return 5;
								case 'U':
									return 5;
								case 'V':
									return 5;
								case 'W':
									return 7;
								case 'X':
									return 5;
								case 'Y':
									return 5;
								case 'Z':
									return 5;
								case '!':
									return 1;
								case '1':
									return 2;
								case '2':
									return 5;
								case '3':
									return 5;
								case '4':
									return 5;
								case '6':
									return 5;
								case '7':
									return 5;
								case '8':
									return 5;
								case '9':
									return 5;
								case '@':
									return 8;
								case '?':
									return 5;
								case '>':
									return 4;
								case '<':
									return 4;
								case '#':
									return 8;
								case '$':
									return 5;
								case '%':
									return 5;
								case '&':
									return 5;
								case '{':
									return 3;
								case '|':
									return 1;
								case '}':
									return 3;
								case ')':
									return 2;
								case '*':
									return 10;
								case '(':
									return 2;
								case '+':
									return 3;
								case ',':
									return 1;
								case '-':
									return 3;
								case '.':
									return 1;
								case '/':
									return 5;
								case '\\':
									return 5;
								case ':':
									return 1;
								case ';':
									return 1;
								case '=':
									return 4;
								case ']':
									return 2;
								case '[':
									return 2;
								case '^':
									return 3;
								case '~':
									return 4;
								case '`':
									return 2;
								case '_':
									return 5;
								case '"':
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