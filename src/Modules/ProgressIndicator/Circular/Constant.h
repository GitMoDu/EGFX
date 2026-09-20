#ifndef _INTEGERGLASS_MODULES_PROGRESS_INDICATOR_CIRCULAR_CONSTANT_h
#define _INTEGERGLASS_MODULES_PROGRESS_INDICATOR_CIRCULAR_CONSTANT_h

#include "../../../IntegerGlassFramework.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace ProgressIndicator
		{
			namespace Circular
			{
				template<
					uint16_t arcThickness = 3,
					uint16_t outerBorderStroke = 1,
					uint16_t innerBorderStroke = outerBorderStroke,
					uint16_t indicatorMargin = 0,
					uint16_t trackMargin = indicatorMargin,
					angle_t startAngle = 0,
					angle_t endAngle = angle_t(startAngle + ANGLE_RANGE)
				>
				struct TemplateCircularStyle
				{
					static constexpr uint16_t ArcThickness = arcThickness;
					static constexpr uint16_t OuterBorderStroke = outerBorderStroke;
					static constexpr uint16_t InnerBorderStroke = innerBorderStroke;
					static constexpr uint16_t IndicatorMargin = indicatorMargin;
					static constexpr uint16_t TrackMargin = trackMargin;
					static constexpr angle_t StartAngle = startAngle;
					static constexpr angle_t EndAngle = endAngle;
				};
			}
		}
	}
}

#endif
