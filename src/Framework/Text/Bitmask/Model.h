#ifndef _EGFX_FRAMEWORK_TEXT_BITMASK_MODEL_h
#define _EGFX_FRAMEWORK_TEXT_BITMASK_MODEL_h

#include "../Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Text
		{
			namespace Bitmask
			{
				template<typename dimension_t>
				struct GlyphBitmask
				{
					const uint8_t* BitmaskData;
					dimension_t Width;
					dimension_t Height;
				};

				namespace Contract
				{
					struct GlyphSource
					{
						using dimension_t = uint8_t;
						static constexpr bool IsMonospace = false;
						static constexpr Framework::DataSourceTypeEnum DataSourceType = Framework::DataSourceTypeEnum::Ram;

						/// <summary>Gets the glyph image for the given character.</summary>
						/// <param name="character">The character to get the glyph for.</param>
						/// <returns>The glyph image for the given character.</returns>
						GlyphBitmask<dimension_t> GetGlyph(const uint8_t /*character*/) const
						{
							return GlyphBitmask<dimension_t>{ nullptr, 0, 0 };
						}

						dimension_t GetFontWidth() const { return 0; }
						dimension_t GetFontHeight() const { return 0; }

						/// <summary>Gets the horizontal advance for a glyph without reading its bitmap data.</summary>
						dimension_t GetAdvance(const char /*printableChar*/) const { return 0; }
					};
				}
			}
		}
	}
}
#endif