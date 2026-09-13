#ifndef _EGFX_MODULES_PROGRESS_INDETERMINATE_CIRCULAR_CONSTANT_h
#define _EGFX_MODULES_PROGRESS_INDETERMINATE_CIRCULAR_CONSTANT_h

#include "../../../EgfxFramework.h"

namespace Egfx
{
	namespace Modules
	{
		namespace ProgressIndeterminate
		{
			namespace Circular
			{
				enum class AnimationModeEnum : uint8_t
				{
					ChaseBounce,
					Outrun,
					BottomBounce,
					GapRotate,
					EnumCount
				};
			}
		}
		
	}
}

#endif
