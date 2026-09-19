#ifndef _EGFX_MODULES_BATTERY_INDICATOR_CONSTANT_h
#define _EGFX_MODULES_BATTERY_INDICATOR_CONSTANT_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Modules
	{
		namespace BatteryIndicator
		{
			using namespace IntegerSignal::FixedPoint::FactorScale;
			using namespace IntegerSignal::FixedPoint::ScalarFraction;

			enum class OrientationEnum : uint8_t
			{
				LeftToRight,
				RightToLeft,
				TopToBottom,
				BottomToTop
			};

			namespace Constants
			{
				static constexpr bool DefaultMonochrome = true;
				static constexpr OrientationEnum DefaultOrientation = OrientationEnum::LeftToRight;
				static constexpr uint8_t DefaultBarCount = 4;
				static constexpr uint32_t DefaultChargeAnimationPeriod = 600000;
				static constexpr ufraction16_t DefaultStroke = UFraction16::GetScalar<int16_t>(2, 27);
				static constexpr ufraction16_t DefaultInnerMargin = DefaultStroke + 1;
				static constexpr ufraction16_t DefaultHeadHeight = UFraction16::GetScalar<int16_t>(12, 20);
				static constexpr ufraction16_t DefaultHeadWidth = UFraction16::GetScalar<int16_t>(3, 29);
				static constexpr Framework::Layout::AlignmentEnum DefaultAlignment = Framework::Layout::AlignmentEnum::MiddleCenter;
			}

			template<bool monochrome = Constants::DefaultMonochrome,
				OrientationEnum orientation = Constants::DefaultOrientation,
				uint8_t barCount = Constants::DefaultBarCount,
				uint32_t chargeAnimationPeriod = Constants::DefaultChargeAnimationPeriod,
				ufraction16_t stroke = Constants::DefaultStroke,
				ufraction16_t innerMargin = Constants::DefaultInnerMargin,
				ufraction16_t headHeight = Constants::DefaultHeadHeight,
				ufraction16_t headWidth = Constants::DefaultHeadWidth,
				Framework::Layout::AlignmentEnum alignment = Constants::DefaultAlignment
			>
			struct TemplateBatteryStyle
			{
				static constexpr uint32_t ChargeAnimationPeriod = chargeAnimationPeriod;
				static constexpr ufraction16_t Stroke = stroke;
				static constexpr ufraction16_t HeadHeight = headHeight;
				static constexpr ufraction16_t HeadWidth = headWidth;
				static constexpr ufraction16_t InnerMargin = innerMargin;
				static constexpr Framework::Layout::AlignmentEnum Alignment = alignment;

				static constexpr OrientationEnum Orientation = orientation;

				static constexpr uint8_t BarCount = barCount;

				static constexpr bool Monochrome = monochrome;
			};

			namespace Dimensions
			{
				static constexpr uint8_t MaxDitherModulus = 2;
			}
		}
	}
}

#endif