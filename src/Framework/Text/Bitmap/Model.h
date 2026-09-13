#ifndef _EGFX_FRAMEWORK_TEXT_BITMAP_MODEL_h
#define _EGFX_FRAMEWORK_TEXT_BITMAP_MODEL_h

#include "../Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Text
		{
			namespace Bitmap
			{
				template<typename dimension_t, typename color_t>
				struct GlyphBitmap
				{
					const color_t* BitmapData;
					dimension_t Width;
					dimension_t Height;
				};

				namespace Contract
				{
					struct GlyphSource
					{
						using dimension_t = uint8_t;
						using color_t = uint8_t;
						static constexpr bool IsMonospace = false;
						static constexpr dimension_t Width = 0;
						static constexpr dimension_t Height = 0;
						static constexpr Framework::DataSourceTypeEnum DataSourceType = Framework::DataSourceTypeEnum::Ram;

						GlyphBitmap<dimension_t, color_t> GetGlyph(const uint8_t /*character*/) const
						{
							return GlyphBitmap<dimension_t, color_t>{ nullptr, 0, 0 };
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
