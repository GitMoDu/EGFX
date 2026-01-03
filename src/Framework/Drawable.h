#ifndef _EGFX_FRAMEWORK_DRAWABLE_h
#define _EGFX_FRAMEWORK_DRAWABLE_h

#include "../Model/IFrameDraw.h"

namespace Egfx
{
	namespace Framework
	{
		/// <summary>
		/// Framework for drawable objects that can render to a frame buffer.
		/// Drawables are passive renderers - they draw their current state without animation logic.
		/// </summary>
		namespace Drawable
		{
			namespace Contract
			{
				/// <summary>
				/// Defines a contract for drawable objects that can render to a frame buffer.
				/// Drawables perform their rendering in a single Draw() call.
				/// Pure rendering - no time parameters, no animation state.
				/// </summary>
				struct Drawable
				{
					void Draw(IFrameBuffer* /*frame*/) {}
				};

				/// <summary>
				/// Defines a contract for drawable animators that can update drawable state over time.
				/// </summary>
				struct Animator
				{
					template<typename DrawableType>
					void Animate(DrawableType& /*drawable*/, const uint32_t /*frameTime*/, const uint16_t /*frameCounter*/) {}
				};
			}
		}
	}
}
#endif