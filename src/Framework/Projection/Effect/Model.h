#ifndef _EGFX_FRAMEWORK_PROJECTION_EFFECT_MODEL_h
#define _EGFX_FRAMEWORK_PROJECTION_EFFECT_MODEL_h

#include "../Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Projection
		{
			namespace Effect
			{
				/// <summary>
				/// Effect values derived from a projected view's effective viewport.
				/// Visibility is a normalized area fraction; scroll is a signed logical offset.
				/// </summary>
				struct effect_t
				{
					UFraction16::scalar_t Visibility;
					int32_t Scroll;
				};
			}
		}
	}
}
#endif