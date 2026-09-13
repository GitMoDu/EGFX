#ifndef _EGFX_FRAMEWORK_BITMAP_MODEL_h
#define _EGFX_FRAMEWORK_BITMAP_MODEL_h

#include "../../EgfxCore.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Bitmap
		{
			namespace Contract
			{
				/// <summary>
				/// Represents a typed image source with:
				/// - Compile-time dimensions (Width, Height)
				/// - Compile-time color type (color_t)
				/// - Compile-time data source type (Ram or Flash)
				/// - A static method GetBitmap() that returns a pointer to the bitmap data.
				/// </summary>
				struct StaticBitmapSource
				{
					using dimension_t = uint8_t;
					using color_t = uint8_t;
					static constexpr dimension_t Width = 0;
					static constexpr dimension_t Height = 0;

					static constexpr Framework::DataSourceTypeEnum DataSourceType = Framework::DataSourceTypeEnum::Ram;

					static const color_t* GetBitmap()
					{
						return nullptr;
					}
				};
			}
		}
	}
}
#endif