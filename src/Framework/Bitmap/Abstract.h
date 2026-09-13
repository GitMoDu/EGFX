#ifndef _EGFX_FRAMEWORK_BITMAP_ABSTRACT_BITMAP_h
#define _EGFX_FRAMEWORK_BITMAP_ABSTRACT_BITMAP_h

#include "Model.h"
#include "Source.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Bitmap
		{
			template<typename dimension_t,
				typename color_t,
				Framework::DataSourceTypeEnum dataSourceType>
			struct AbstractBitmap
			{
				using ReaderType = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
					Source::Flash,
					Source::Ram,
					dataSourceType == Framework::DataSourceTypeEnum::Flash>::type;

				static constexpr bool IsConstantColor = false;

				const color_t* BitmapData = nullptr;
				dimension_t Width = 0;
				dimension_t Height = 0;

				bool PrepareBitmapData(const color_t* bitmapData, const dimension_t width, const dimension_t height)
				{
					BitmapData = bitmapData;
					Width = width;
					Height = height;

					return BitmapData != nullptr && Width > 0 && Height > 0;
				}

				rgb_color_t Source(const dimension_t x, const dimension_t y)
				{
					// Read the pixel value at (x, y) from the dynamic bitmap using the appropriate reader type (Flash or Ram).
					const auto nativeColor = ReaderType::template ReadColorAt<dimension_t>(BitmapData, x, y, Width);

					// Convert the native color to an RGB color and return it.
					return Rgb::Color(nativeColor);
				}
			};
		}
	}
}
#endif