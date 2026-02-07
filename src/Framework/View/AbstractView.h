#ifndef _EGFX_FRAMEWORK_ABSTRACT_VIEW_h
#define _EGFX_FRAMEWORK_ABSTRACT_VIEW_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace View
		{
			/// <summary>
			/// AbstractView provides standard view orchestration with customizable steps.
			/// Separates animation state update (ViewStep) from rendering (DrawCall).
			/// Derived classes override ViewStep() for animation and DrawCall() for rendering.
			/// Use this as base when you need simple single-pass rendering with animation.
			/// </summary>
			class AbstractView
			{
			protected:
				/// <summary>
				/// Protected Draw call for derived classes to implement rendering logic.
				/// Called after ViewStep() has updated animation state.
				/// Should render the view and return true when complete.
				/// </summary>
				/// <param name="frame">Target framebuffer to draw into.</param>
				/// <returns>True when the view has completed its current draw cycle.</returns>
				virtual bool Draw(IFrameBuffer* /*frame*/) = 0;

				/// <summary>
				/// View-level animation step executed before rendering.
				/// Override to update view state (positions, colors, etc.).
				/// No framebuffer access - pure state update.
				/// </summary>
				/// <param name="frameTime">Rolling frame timestamp (microseconds).</param>
				/// <param name="frameCounter">Rolling frame counter.</param>
				/// <returns>True to proceed with rendering, false to skip the entire cycle.</returns>
				virtual bool ViewStep(const uint32_t /*frameTime*/, const uint16_t /*frameCounter*/) = 0;

			private:
				bool Stepped = false;

			public:
				AbstractView() = default;
				virtual ~AbstractView() = default;

				/// <summary>
				/// Orchestrates the view's step + draw cycle.
				/// If ViewStep() returns false, the cycle is skipped and completed immediately.
				/// </summary>
				/// <param name="frame">Target framebuffer to draw into.</param>
				/// <param name="frameTime">Rolling frame timestamp (microseconds).</param>
				/// <param name="frameCounter">Rolling frame counter.</param>
				/// <returns>True when the view has completed its current draw cycle.</returns>
				bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter)
				{
					if (!Stepped)
					{
						// First call - reset step state
						Stepped = true;

						// Update animation state (no framebuffer - pure state update)
						// If ViewStep returns false, skip the entire cycle
						if (!ViewStep(frameTime, frameCounter))
						{
							// Skip cycle - reset state and complete immediately
							Stepped = false;
							return true;
						}

						return false;
					}
					else
					{
						// Render view (framebuffer access)
						if (Draw(frame))
						{
							// Rendering complete - reset for next cycle
							Stepped = false;
							return true;
						}
						else
						{
							// More rendering needed - remain in stepped state
							return false;
						}
					}
				}
			};
		}
	}
}
#endif