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
				/// Protected DrawCall for derived classes to implement rendering logic.
				/// Called after ViewStep() has updated animation state.
				/// Should render the view and return true when complete.
				/// </summary>
				/// <param name="frame">Frame buffer to render to.</param>
				/// <returns>True when rendering is complete.</returns>
				virtual bool Draw(IFrameBuffer* /*frame*/) = 0;

				/// <summary>
				/// View-level animation step executed before rendering.
				/// Override to update view state (positions, colors, etc.).
				/// No framebuffer access - pure state update.
				/// </summary>
				/// <param name="frameTime">Current frame time in microseconds.</param>
				/// <param name="frameCounter">Current frame number.</param>
				virtual void ViewStep(const uint32_t /*frameTime*/, const uint16_t /*frameCounter*/) = 0;

			private:
				bool Stepped = false;

			public:
				AbstractView() = default;
				virtual ~AbstractView() = default;

				bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter)
				{
					if (!Stepped)
					{
						// First call - reset step state
						Stepped = true;

						// Update animation state (no framebuffer - pure state update)
						ViewStep(frameTime, frameCounter);

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