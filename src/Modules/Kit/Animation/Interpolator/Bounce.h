#ifndef _INTEGERGLASS_MODULES_KIT_ANIMATION_INTERPOLATOR_BOUNCE_h
#define _INTEGERGLASS_MODULES_KIT_ANIMATION_INTERPOLATOR_BOUNCE_h	

#include <IntegerGlassFramework.h>

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Kit
		{
			namespace Animation
			{
				namespace Interpolator
				{
					using namespace Framework::Animation;
					using namespace Framework::Animation::Interpolator;

					struct Bounce
					{
						static progress_wide_t Segment(const int64_t value,
							const int64_t offset, const int64_t height)
						{
							const int64_t distance = value - offset;
							return static_cast<progress_wide_t>(
								(8 * distance * distance) / CURVE_SCALAR_UNIT + height);
						}

						static progress_wide_t Get(const progress_t value)
						{
							const int64_t scalarUnit = CURVE_SCALAR_UNIT;
							const int64_t firstLimit = (4 * scalarUnit) / 11;
							const int64_t secondLimit = (8 * scalarUnit) / 11;
							const int64_t thirdLimit = (10 * scalarUnit) / 11;

							if (value < firstLimit)
								return Segment(value, 0, 0);
							if (value < secondLimit)
								return Segment(value, (6 * scalarUnit) / 11, (3 * scalarUnit) / 4);
							if (value < thirdLimit)
								return Segment(value, (9 * scalarUnit) / 11, (15 * scalarUnit) / 16);

							return Segment(value, (21 * scalarUnit) / 22, (63 * scalarUnit) / 64);
						}
					};
				}
			}
		}
	}
}
#endif