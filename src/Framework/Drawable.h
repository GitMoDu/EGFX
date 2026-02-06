#ifndef _EGFX_FRAMEWORK_DRAWABLE_h
#define _EGFX_FRAMEWORK_DRAWABLE_h

#include "../Model/IFrameDraw.h"
#include "Support.h"

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
				/// Drawables perform their rendering in a single Draw() call without any parameters, unless where required (eg: fonts).
				/// </summary>
				struct Drawable
				{
					/// <summary>
					/// Draw the drawable's current state to the provided framebuffer.
					/// </summary>
					/// <param name="frame">Target framebuffer to draw into.</param>
					void Draw(IFrameBuffer* /*frame*/) {}
				};
			}
		}
	}
}
#endif