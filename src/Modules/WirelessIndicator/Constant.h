#ifndef _INTEGERGLASS_MODULES_WIRELESS_INDICATOR_CONSTANT_h
#define _INTEGERGLASS_MODULES_WIRELESS_INDICATOR_CONSTANT_h

#include "../../IntegerGlassFramework.h"
#include "Assets.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace WirelessIndicator
		{
			using namespace IntegerGlass;

			enum class IndicatorModeEnum : uint8_t
			{
				NoLink,
				Searching,
				SignalStrength,
				EnumCount
			};

			namespace Durations
			{
				static constexpr uint32_t SearchAnimationDuration = 375000;

			}
			namespace Dimensions
			{
				static constexpr uint8_t BarCount = 4;
				static constexpr uint8_t PrimitiveBarHeight = 2;
				static constexpr uint8_t ParallelogramCornerDivisor = 12;
				static constexpr uint8_t ParallelogramBackgroundLinesDivisor = 7;
			}
		}
	}
}
#endif