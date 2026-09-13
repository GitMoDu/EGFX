#ifndef _EGFX_FRAMEWORK_IMAGE_MODEL_h
#define _EGFX_FRAMEWORK_IMAGE_MODEL_h

#include "../../EgfxCore.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Image
		{
			enum class ScaledTransformOrderEnum : uint8_t
			{
				TransformThenScale,
				ScaleThenTransform,
				EnumCount
			};


			namespace Contract
			{
				struct ImageStyle
				{
					static constexpr Framework::Layout::AlignmentEnum Alignment = Framework::Layout::AlignmentEnum::MiddleCenter;
					static constexpr rgb_color_t TransparentColor = RGB_COLOR_BLACK;
					static constexpr bool UseTransparency = false;
					static constexpr uint8_t ScaleX = 1;
					static constexpr uint8_t ScaleY = 1;
					static constexpr ScaledTransformOrderEnum TransformOrder = ScaledTransformOrderEnum::TransformThenScale;
				};
			}

			template<Framework::Layout::AlignmentEnum alignment = Framework::Layout::AlignmentEnum::MiddleCenter,
				bool useTransparency = false,
				rgb_color_t transparentColor = RGB_COLOR_BLACK,
				uint8_t scaleX = 1,
				uint8_t scaleY = 1,
				ScaledTransformOrderEnum transformOrder = ScaledTransformOrderEnum::TransformThenScale
			>
			struct TemplateImageStyle
			{
				static constexpr Framework::Layout::AlignmentEnum Alignment = alignment;
				static constexpr rgb_color_t TransparentColor = transparentColor;
				static constexpr bool UseTransparency = useTransparency;
				static constexpr uint8_t ScaleX = scaleX;
				static constexpr uint8_t ScaleY = scaleY;
				static constexpr ScaledTransformOrderEnum TransformOrder = transformOrder;
			};
		}
	}
}
#endif