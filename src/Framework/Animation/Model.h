#ifndef _EGFX_FRAMEWORK_ANIMATION_MODEL_h
#define _EGFX_FRAMEWORK_ANIMATION_MODEL_h

#include "../../EgfxCore.h"
#include "../Support.h"


namespace Egfx
{
	namespace Framework
	{
		namespace Animation
		{
			/// <summary>
			/// Framework animations use a Q0.15 fixed-point fraction type for progress values.
			/// </summary>
			using progress_t = UFraction16::scalar_t;

			/// <summary>
			/// Wide progress can be negative or overflow UFraction16::SCALAR_UNIT.
			/// This enables over and under shooting animations, such as anticipation and overshoot curves.
			/// </summary>
			using progress_wide_t = IntegerSignal::TypeTraits::TypeNext::next_int_type<progress_t>::type;

			template<typename value_t>
			struct animation_state_t
			{
				progress_t Progress;
				progress_wide_t CurveProgress;


				value_t Value;
			};
		}
	}
}

#endif