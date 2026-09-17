#ifndef _EGFX_MODULES_KIT_FONT_BITMASK_SEALANT_FONT_TYPE_h
#define _EGFX_MODULES_KIT_FONT_BITMASK_SEALANT_FONT_TYPE_h

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
					/// Font type for FPS counter with 4x5 dimensions.
					/// Reduced character set: uppercase letters (F, P, S) and numbers (0-9).
					/// </summary>
					namespace Sealant
					{
						struct FontType4x5
						{
							using dimension_t = uint8_t;
							using glyph_t = Framework::Text::Bitmask::GlyphBitmask<dimension_t>;

							static constexpr bool IsMonospace = true;
							static constexpr Framework::DataSourceTypeEnum DataSourceType = Framework::DataSourceTypeEnum::Flash;

							static constexpr dimension_t GetFontWidth() { return Source3x5::Width; }
							static constexpr dimension_t GetFontHeight() { return Source3x5::Height; }

							static glyph_t GetGlyph(const uint8_t character)
							{
								switch (static_cast<char>(character))
								{
								case 'F':
									return glyph_t{ Source3x5::Atlas + 0, Source3x5::Width, Source3x5::Height };
								case 'P':
									return glyph_t{ Source3x5::Atlas + 5, Source3x5::Width, Source3x5::Height };
								case 'S':
									return glyph_t{ Source3x5::Atlas + 10, Source3x5::Width, Source3x5::Height };
								case '0':
									return glyph_t{ Source3x5::Atlas + 15, Source3x5::Width, Source3x5::Height };
								case '1':
									return glyph_t{ Source3x5::Atlas + 20, Source3x5::Width, Source3x5::Height };
								case '2':
									return glyph_t{ Source3x5::Atlas + 25, Source3x5::Width, Source3x5::Height };
								case '3':
									return glyph_t{ Source3x5::Atlas + 30, Source3x5::Width, Source3x5::Height };
								case '4':
									return glyph_t{ Source3x5::Atlas + 35, Source3x5::Width, Source3x5::Height };
								case '5':
									return glyph_t{ Source3x5::Atlas + 40, Source3x5::Width, Source3x5::Height };
								case '6':
									return glyph_t{ Source3x5::Atlas + 45, Source3x5::Width, Source3x5::Height };
								case '7':
									return glyph_t{ Source3x5::Atlas + 50, Source3x5::Width, Source3x5::Height };
								case '8':
									return glyph_t{ Source3x5::Atlas + 55, Source3x5::Width, Source3x5::Height };
								case '9':
									return glyph_t{ Source3x5::Atlas + 60, Source3x5::Width, Source3x5::Height };
								default:
									return glyph_t{ nullptr, 0, 0 };
								}
							}

							static constexpr dimension_t GetAdvance(const char /*printableChar*/)
							{
								return Source3x5::Width;
							}
						};
					}
				}
			}
		}
	}
}
#endif