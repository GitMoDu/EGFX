#ifndef _EGFX_MODULES_PROGRESS_INDETERMINATE_LINEAR_CONSTANT_h
#define _EGFX_MODULES_PROGRESS_INDETERMINATE_LINEAR_CONSTANT_h

#include "../../../EgfxFramework.h"

namespace Egfx
{
	namespace Modules
	{
		namespace ProgressIndeterminate
		{
			namespace Linear
			{
				enum class AnimationModeEnum : uint8_t
				{
					PingPong,
					Test1,
					Progressive,
					Symmetric,
					EnumCount
				};

				template<typename dimension_t>
				struct SegmentPair
				{
					dimension_t LeftStart;
					dimension_t LeftEnd;
					dimension_t RightStart;
					dimension_t RightEnd;

					void Coalesce()
					{
						// Treat zero-length ranges as inactive/empty
						bool leftActive = (LeftStart < LeftEnd);
						bool rightActive = (RightStart < RightEnd);

						if (!leftActive && !rightActive) return;
						if (!rightActive) return;
						if (!leftActive)
						{
							LeftStart = RightStart;
							LeftEnd = RightEnd;
							RightStart = 0;
							RightEnd = 0;
							return;
						}

						// Check for overlap, containment, or adjacency
						if (LeftEnd >= RightStart && RightEnd >= LeftStart)
						{
							// Merge into Left, clear Right
							LeftStart = MinValue(LeftStart, RightStart);
							LeftEnd = MaxValue(LeftEnd, RightEnd);
							RightStart = 0;
							RightEnd = 0;
						}
					}
				};
			}
		}
	}
}
#endif