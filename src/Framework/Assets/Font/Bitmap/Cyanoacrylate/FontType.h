#ifndef _EGFX_FRAMEWORK_ASSETS_FONT_BITMAP_CYANOACRYLATE_h
#define _EGFX_FRAMEWORK_ASSETS_FONT_BITMAP_CYANOACRYLATE_h

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
				namespace Bitmap
				{
					/// <summary>
					/// Subpixel-font with 1x5 and 2x5 variants, designed for use with subpixel rendering on RGB stripe displays
					/// Only works for RGB color order.
					/// </summary>
					namespace Cyanoacrylate
					{
						struct FontType1x5
						{
							using dimension_t = uint8_t;
							using color_t = uint8_t;
							using glyph_t = Framework::Text::Bitmap::GlyphBitmap<dimension_t, color_t>;

							static constexpr bool IsMonospace = true;
							static constexpr Framework::DataSourceTypeEnum DataSourceType = Framework::DataSourceTypeEnum::Flash;

							static constexpr dimension_t GetFontWidth() { return Source1x5::Width; }
							static constexpr dimension_t GetFontHeight() { return Source1x5::Height; }

							static constexpr dimension_t GetAdvance(const char /*printableChar*/)
							{
								return Source1x5::Width;
							}

							static glyph_t GetGlyph(const uint8_t character)
							{
								switch (static_cast<char>(character))
								{
								case 'A':
									return glyph_t{ Source1x5::Atlas + 0, Source1x5::Width, Source1x5::Height };
								case 'B':
									return glyph_t{ Source1x5::Atlas + 5, Source1x5::Width, Source1x5::Height };
								case 'C':
									return glyph_t{ Source1x5::Atlas + 10, Source1x5::Width, Source1x5::Height };
								case 'D':
									return glyph_t{ Source1x5::Atlas + 15, Source1x5::Width, Source1x5::Height };
								case 'E':
									return glyph_t{ Source1x5::Atlas + 20, Source1x5::Width, Source1x5::Height };
								case 'F':
									return glyph_t{ Source1x5::Atlas + 25, Source1x5::Width, Source1x5::Height };
								case 'G':
									return glyph_t{ Source1x5::Atlas + 30, Source1x5::Width, Source1x5::Height };
								case 'H':
									return glyph_t{ Source1x5::Atlas + 35, Source1x5::Width, Source1x5::Height };
								case 'I':
									return glyph_t{ Source1x5::Atlas + 40, Source1x5::Width, Source1x5::Height };
								case 'J':
									return glyph_t{ Source1x5::Atlas + 45, Source1x5::Width, Source1x5::Height };
								case 'K':
									return glyph_t{ Source1x5::Atlas + 50, Source1x5::Width, Source1x5::Height };
								case 'L':
									return glyph_t{ Source1x5::Atlas + 55, Source1x5::Width, Source1x5::Height };
								case 'M':
									return glyph_t{ Source1x5::Atlas + 60, Source1x5::Width, Source1x5::Height };
								case 'N':
									return glyph_t{ Source1x5::Atlas + 65, Source1x5::Width, Source1x5::Height };
								case 'O':
									return glyph_t{ Source1x5::Atlas + 70, Source1x5::Width, Source1x5::Height };
								case 'P':
									return glyph_t{ Source1x5::Atlas + 75, Source1x5::Width, Source1x5::Height };
								case 'Q':
									return glyph_t{ Source1x5::Atlas + 80, Source1x5::Width, Source1x5::Height };
								case 'R':
									return glyph_t{ Source1x5::Atlas + 85, Source1x5::Width, Source1x5::Height };
								case 'S':
									return glyph_t{ Source1x5::Atlas + 90, Source1x5::Width, Source1x5::Height };
								case 'T':
									return glyph_t{ Source1x5::Atlas + 95, Source1x5::Width, Source1x5::Height };
								case 'U':
									return glyph_t{ Source1x5::Atlas + 100, Source1x5::Width, Source1x5::Height };
								case 'V':
									return glyph_t{ Source1x5::Atlas + 105, Source1x5::Width, Source1x5::Height };
								case 'W':
									return glyph_t{ Source1x5::Atlas + 110, Source1x5::Width, Source1x5::Height };
								case 'X':
									return glyph_t{ Source1x5::Atlas + 115, Source1x5::Width, Source1x5::Height };
								case 'Y':
									return glyph_t{ Source1x5::Atlas + 120, Source1x5::Width, Source1x5::Height };
								case 'Z':
									return glyph_t{ Source1x5::Atlas + 125, Source1x5::Width, Source1x5::Height };
								case '0':
									return glyph_t{ Source1x5::Atlas + 130, Source1x5::Width, Source1x5::Height };
								case '1':
									return glyph_t{ Source1x5::Atlas + 135, Source1x5::Width, Source1x5::Height };
								case '2':
									return glyph_t{ Source1x5::Atlas + 140, Source1x5::Width, Source1x5::Height };
								case '3':
									return glyph_t{ Source1x5::Atlas + 145, Source1x5::Width, Source1x5::Height };
								case '4':
									return glyph_t{ Source1x5::Atlas + 150, Source1x5::Width, Source1x5::Height };
								case '5':
									return glyph_t{ Source1x5::Atlas + 155, Source1x5::Width, Source1x5::Height };
								case '6':
									return glyph_t{ Source1x5::Atlas + 160, Source1x5::Width, Source1x5::Height };
								case '7':
									return glyph_t{ Source1x5::Atlas + 165, Source1x5::Width, Source1x5::Height };
								case '8':
									return glyph_t{ Source1x5::Atlas + 170, Source1x5::Width, Source1x5::Height };
								case '9':
									return glyph_t{ Source1x5::Atlas + 175, Source1x5::Width, Source1x5::Height };
								default:
									return glyph_t{ nullptr, 0, 0 };
								}
							}
						};

						struct FontType2x5
						{
							using dimension_t = uint8_t;
							using color_t = uint8_t;
							using glyph_t = Framework::Text::Bitmap::GlyphBitmap<dimension_t, color_t>;

							static constexpr bool IsMonospace = true;
							static constexpr Framework::DataSourceTypeEnum DataSourceType = Framework::DataSourceTypeEnum::Flash;

							static constexpr dimension_t GetAdvance(const char /*printableChar*/)
							{
								return Source2x5::Width;
							}

							static glyph_t GetGlyph(const uint8_t character)
							{
								switch (static_cast<char>(character))
								{
								case 'A':
									return glyph_t{ Source2x5::Atlas + 0, Source2x5::Width, Source2x5::Height };
								case 'B':
									return glyph_t{ Source2x5::Atlas + 5, Source2x5::Width, Source2x5::Height };
								case 'C':
									return glyph_t{ Source2x5::Atlas + 10, Source2x5::Width, Source2x5::Height };
								case 'D':
									return glyph_t{ Source2x5::Atlas + 15, Source2x5::Width, Source2x5::Height };
								case 'E':
									return glyph_t{ Source2x5::Atlas + 20, Source2x5::Width, Source2x5::Height };
								case 'F':
									return glyph_t{ Source2x5::Atlas + 25, Source2x5::Width, Source2x5::Height };
								case 'G':
									return glyph_t{ Source2x5::Atlas + 30, Source2x5::Width, Source2x5::Height };
								case 'H':
									return glyph_t{ Source2x5::Atlas + 35, Source2x5::Width, Source2x5::Height };
								case 'I':
									return glyph_t{ Source2x5::Atlas + 40, Source2x5::Width, Source2x5::Height };
								case 'J':
									return glyph_t{ Source2x5::Atlas + 45, Source2x5::Width, Source2x5::Height };
								case 'K':
									return glyph_t{ Source2x5::Atlas + 50, Source2x5::Width, Source2x5::Height };
								case 'L':
									return glyph_t{ Source2x5::Atlas + 55, Source2x5::Width, Source2x5::Height };
								case 'M':
									return glyph_t{ Source2x5::Atlas + 60, Source2x5::Width, Source2x5::Height };
								case 'N':
									return glyph_t{ Source2x5::Atlas + 65, Source2x5::Width, Source2x5::Height };
								case 'O':
									return glyph_t{ Source2x5::Atlas + 70, Source2x5::Width, Source2x5::Height };
								case 'P':
									return glyph_t{ Source2x5::Atlas + 75, Source2x5::Width, Source2x5::Height };
								case 'Q':
									return glyph_t{ Source2x5::Atlas + 80, Source2x5::Width, Source2x5::Height };
								case 'R':
									return glyph_t{ Source2x5::Atlas + 85, Source2x5::Width, Source2x5::Height };
								case 'S':
									return glyph_t{ Source2x5::Atlas + 90, Source2x5::Width, Source2x5::Height };
								case 'T':
									return glyph_t{ Source2x5::Atlas + 95, Source2x5::Width, Source2x5::Height };
								case 'U':
									return glyph_t{ Source2x5::Atlas + 100, Source2x5::Width, Source2x5::Height };
								case 'V':
									return glyph_t{ Source2x5::Atlas + 105, Source2x5::Width, Source2x5::Height };
								case 'W':
									return glyph_t{ Source2x5::Atlas + 110, Source2x5::Width, Source2x5::Height };
								case 'X':
									return glyph_t{ Source2x5::Atlas + 115, Source2x5::Width, Source2x5::Height };
								case 'Y':
									return glyph_t{ Source2x5::Atlas + 120, Source2x5::Width, Source2x5::Height };
								case 'Z':
									return glyph_t{ Source2x5::Atlas + 125, Source2x5::Width, Source2x5::Height };
								case '0':
									return glyph_t{ Source2x5::Atlas + 130, Source2x5::Width, Source2x5::Height };
								case '1':
									return glyph_t{ Source2x5::Atlas + 135, Source2x5::Width, Source2x5::Height };
								case '2':
									return glyph_t{ Source2x5::Atlas + 140, Source2x5::Width, Source2x5::Height };
								case '3':
									return glyph_t{ Source2x5::Atlas + 145, Source2x5::Width, Source2x5::Height };
								case '4':
									return glyph_t{ Source2x5::Atlas + 150, Source2x5::Width, Source2x5::Height };
								case '5':
									return glyph_t{ Source2x5::Atlas + 155, Source2x5::Width, Source2x5::Height };
								case '6':
									return glyph_t{ Source2x5::Atlas + 160, Source2x5::Width, Source2x5::Height };
								case '7':
									return glyph_t{ Source2x5::Atlas + 165, Source2x5::Width, Source2x5::Height };
								case '8':
									return glyph_t{ Source2x5::Atlas + 170, Source2x5::Width, Source2x5::Height };
								case '9':
									return glyph_t{ Source2x5::Atlas + 175, Source2x5::Width, Source2x5::Height };
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