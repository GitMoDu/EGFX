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

			template<bool monochrome = true,
				OrientationEnum orientation = OrientationEnum::LeftToRight,
				uint8_t barCount = 4,
				uint32_t chargeAnimationPeriod = 600000,
				ufraction16_t stroke = UFraction16::GetScalar<int16_t>(2, 27),
				ufraction16_t innerMargin = stroke + 1,
				ufraction16_t headHeight = UFraction16::GetScalar<int16_t>(12, 20),
				ufraction16_t headWidth = UFraction16::GetScalar<int16_t>(4, 29)
			>
			struct TemplateBatteryStyle
			{
				static constexpr uint32_t ChargeAnimationPeriod = chargeAnimationPeriod;
				static constexpr ufraction16_t Stroke = stroke;
				static constexpr ufraction16_t HeadHeight = headHeight;
				static constexpr ufraction16_t HeadWidth = headWidth;
				static constexpr ufraction16_t InnerMargin = innerMargin;

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