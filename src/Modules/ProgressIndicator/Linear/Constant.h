#ifndef _INTEGERGLASS_MODULES_PROGRESS_INDICATOR_LINEAR_CONSTANT_h
#define _INTEGERGLASS_MODULES_PROGRESS_INDICATOR_LINEAR_CONSTANT_h

#include "../../../IntegerGlassFramework.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace ProgressIndicator
		{
			enum class OrientationEnum : uint8_t
			{
				LeftToRight,
				RightToLeft,
				TopToBottom,
				BottomToTop
			};

			template<OrientationEnum orientation = OrientationEnum::LeftToRight,
				pixel_t outlineWidth = 1,
				pixel_t fillMargin = 0,
				pixel_t cornerRadius = 0
			>
			struct TemplateLinearProgressStyle
			{
				static constexpr OrientationEnum Orientation = orientation;
				static constexpr pixel_t OutlineWidth = outlineWidth;
				static constexpr pixel_t FillMargin = fillMargin;
				static constexpr pixel_t CornerRadius = cornerRadius;
			};		
		}
	}
}

#endif
