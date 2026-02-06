#ifndef _EGFX_FRAMEWORK_DRAWABLES_VIEW_h
#define _EGFX_FRAMEWORK_DRAWABLES_VIEW_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace View
		{
			/// <summary>
			/// DrawablesView composes multiple passive drawables and orchestrates their rendering.
			/// Drawables are owned and default-constructed by the view, and must follow the Drawable contract.
			/// Access drawables via drawable<Index>() to configure state.
			/// Each DrawCall() cycle:
			/// 1. Calls ViewStep() once to update animation state (no framebuffer).
			/// 2. Renders drawables one-by-one via Draw(framebuffer) - pure rendering.
			/// 3. Advances to the next drawable.
			/// When all drawables have been rendered, returns true and resets.
			/// </summary>
			/// <typeparam name="DrawableTypes">Drawable types implementing Draw(IFrameBuffer*).</typeparam>
			template<typename... DrawableTypes>
			class DrawablesView
			{
			private:
				Support::ParameterPack::ElementPack<DrawableTypes...> drawables_;
				uint8_t CurrentDrawable = 0;

			public:
				static constexpr uint8_t DrawableCount = static_cast<uint8_t>(sizeof...(DrawableTypes));

			private:
				// Compile-time dispatcher: calls Draw(frame) on target drawable.
				template<uint8_t Index>
				void DrawAt(IFrameBuffer* frame)
				{
					drawables_.template Get<Index>().Draw(frame);
				}

				template<uint8_t Index, uint8_t N>
				struct Dispatcher
				{
					static void Dispatch(DrawablesView* self, IFrameBuffer* frame, const uint8_t target)
					{
						if (Index == target)
						{
							self->template DrawAt<Index>(frame);
						}
						else
						{
							Dispatcher<Index + 1, N>::Dispatch(self, frame, target);
						}
					}
				};

				template<uint8_t N>
				struct Dispatcher<N, N>
				{
					static void Dispatch(DrawablesView*, IFrameBuffer*, const uint8_t) {}
				};

			protected:
				/// <summary>
				/// View-level animation step executed once per cycle before rendering.
				/// Override to update drawable state (positions, colors, etc.).
				/// No framebuffer access - pure state update for passive drawables.
				/// </summary>
				/// <param name="frameTime">Rolling frame timestamp (microseconds).</param>
				/// <param name="frameCounter">Rolling frame counter.</param>
				virtual void ViewStep(const uint32_t /*frameTime*/, const uint16_t /*frameCounter*/) {}

			public:
				/// <summary>
				/// Default constructor - default-constructs all drawables.
				/// Drawables must be default-constructible.
				/// </summary>
				DrawablesView() : drawables_() {}

				/// <summary>
				/// Forwarding constructor for non-default drawable construction (optional).
				/// Pass drawable instances if they require special initialization.
				/// </summary>
				template<typename... Args>
				explicit DrawablesView(Args&&... args)
					: drawables_(static_cast<Args&&>(args)...) {
				}

				virtual ~DrawablesView() = default;

				/// <summary>
				/// Access a specific drawable by index.
				/// Example: drawable<0>().TransformShader.SetRotation(45);
				/// </summary>
				template<uint8_t Index>
				typename Support::ParameterPack::GetHelper<Index, DrawableTypes...>::type& drawable()
				{
					return drawables_.template Get<Index>();
				}

				/// <summary>
				/// Access a specific const drawable by index.
				/// Example: drawable<0>().TransformShader.SetRotation(45);
				/// </summary>
				template<uint8_t Index>
				const typename Support::ParameterPack::GetHelper<Index, DrawableTypes...>::type& drawable() const
				{
					return drawables_.template Get<Index>();
				}

				/// <summary>
				/// DrawCall orchestration implementing View contract:
				/// 1. Calls ViewStep() once at start of cycle to update animation state.
				/// 2. Renders current drawable via Draw(framebuffer).
				/// 3. Advances to the next drawable on the next call.
				/// 4. Returns true when all drawables rendered (cycle complete).
				/// </summary>
				/// <param name="frame">Target framebuffer to draw into.</param>
				/// <param name="frameTime">Rolling frame timestamp (microseconds).</param>
				/// <param name="frameCounter">Rolling frame counter.</param>
				/// <returns>True when the view has completed its current draw cycle.</returns>
				bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter)
				{
					if (DrawableCount == 0u) { return true; }

					// Reset index if out of bounds
					if (CurrentDrawable >= DrawableCount)
					{
						CurrentDrawable = 0;
					}

					// Update animation state once per cycle (when starting from first drawable)
					if (CurrentDrawable == 0)
					{
						ViewStep(frameTime, frameCounter);
					}

					// Render current drawable (passive - framebuffer only)
					Dispatcher<0, DrawableCount>::Dispatch(this, frame, CurrentDrawable);

					// Advance to next drawable for next call
					++CurrentDrawable;

					// Check if cycle is complete (all drawables have been rendered)
					if (CurrentDrawable >= DrawableCount)
					{
						CurrentDrawable = 0; // Reset for next cycle
						return true; // Cycle complete
					}

					return false; // More drawables remain in this cycle
				}
			};
		}
	}
}
#endif