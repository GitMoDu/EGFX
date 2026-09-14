#ifndef _EGFX_FRAMEWORK_DRAWABLES_VIEW_h
#define _EGFX_FRAMEWORK_DRAWABLES_VIEW_h

#include "Model.h"
#include "Child.h"
#include "../Shader/Model.h"

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
			template<typename ParentLayout, typename... DrawableTypes>
			class DrawablesView
			{
			public:
				using ViewLayout = ParentLayout;

			private:
				template<typename DrawableType>
				using ChildType = Detail::LayoutChild<DrawableType, ParentLayout, ParentLayout>;

			private:
				Support::ParameterPack::ElementPack<ChildType<DrawableTypes>...> drawables_;
				Shader::viewport_t<int16_t> Viewport{
					static_cast<int16_t>(0),
					static_cast<int16_t>(0),
					static_cast<int16_t>(ParentLayout::Width()),
					static_cast<int16_t>(ParentLayout::Height()),
					0, 0 };
				uint8_t CurrentDrawable = 0;

			public:
				static constexpr uint8_t DrawableCount = static_cast<uint8_t>(sizeof...(DrawableTypes));

			private:
				template<uint8_t Index, uint8_t N>
				struct BoundsDispatcher
				{
					static void Dispatch(DrawablesView* self, const int16_t left, const int16_t top,
						const int16_t right, const int16_t bottom)
					{
						self->drawables_.template Get<Index>().SetBounds(left, top, right, bottom);
						BoundsDispatcher<Index + 1, N>::Dispatch(self, left, top, right, bottom);
					}
				};

				template<uint8_t N>
				struct BoundsDispatcher<N, N>
				{
					static void Dispatch(DrawablesView*, const int16_t, const int16_t,
						const int16_t, const int16_t) {}
				};

				template<uint8_t Index, uint8_t N>
				struct TranslationDispatcher
				{
					static void Dispatch(DrawablesView* self, const int16_t x, const int16_t y)
					{
						self->drawables_.template Get<Index>().SetTranslation(x, y);
						TranslationDispatcher<Index + 1, N>::Dispatch(self, x, y);
					}
				};

				template<uint8_t N>
				struct TranslationDispatcher<N, N>
				{
					static void Dispatch(DrawablesView*, const int16_t, const int16_t) {}
				};

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
							if (self->drawables_.template Get<Index>().IsVisible())
							{
								self->template DrawAt<Index>(frame);
							}
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

				template<uint8_t Index, uint8_t N>
				struct VisibilityChecker
				{
					static bool Check(const DrawablesView* self)
					{
						if (self->drawables_.template Get<Index>().IsVisible())
						{
							return true;
						}

						return VisibilityChecker<Index + 1, N>::Check(self);
					}
				};

				template<uint8_t N>
				struct VisibilityChecker<N, N>
				{
					static bool Check(const DrawablesView*) { return false; }
				};

			protected:
				/// <summary>
				/// View-level animation step executed once per cycle before rendering.
				/// Override to update drawable state (positions, colors, etc.).
				/// No framebuffer access - pure state update for passive drawables.
				/// </summary>
				/// <param name="frameTime">Rolling frame timestamp (microseconds).</param>
				/// <param name="frameCounter">Rolling frame counter.</param>
				/// <returns>True to proceed with rendering, false to skip the entire cycle.</returns>
				virtual bool ViewStep(const uint32_t /*frameTime*/, const uint16_t /*frameCounter*/) { return true; }

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
					: drawables_(static_cast<Args&&>(args)...) {}

				virtual ~DrawablesView() = default;

				/// <summary>
				/// Access a specific drawable by index.
				/// Example: drawable<0>().TransformShader.SetRotation(45);
				/// </summary>
				template<uint8_t Index>
				typename Support::ParameterPack::GetHelper<Index, ChildType<DrawableTypes>...>::type& drawable()
				{
					static_assert(Index < DrawableCount, "DrawablesView::drawable<Index>() - Index out of range.");
					return drawables_.template Get<Index>();
				}

				template<uint8_t Index>
				auto colorSource() -> decltype(drawable<Index>().ColorSource)& { return drawable<Index>().ColorSource; }

				template<uint8_t Index>
				auto colorSource() const -> const decltype(drawable<Index>().ColorSource)& { return drawable<Index>().ColorSource; }

				template<uint8_t Index>
				auto colorShader() -> decltype(drawable<Index>().ColorShader)& { return drawable<Index>().ColorShader; }

				template<uint8_t Index>
				auto colorShader() const -> const decltype(drawable<Index>().ColorShader)& { return drawable<Index>().ColorShader; }

				template<uint8_t Index>
				auto transformShader() -> decltype(drawable<Index>().TransformShader)& { return drawable<Index>().TransformShader; }

				template<uint8_t Index>
				auto transformShader() const -> const decltype(drawable<Index>().TransformShader)& { return drawable<Index>().TransformShader; }

				void SetBounds(const int16_t left, const int16_t top,
					const int16_t right, const int16_t bottom)
				{
					Viewport.BoundsLeft = left;
					Viewport.BoundsTop = top;
					Viewport.BoundsRight = right;
					Viewport.BoundsBottom = bottom;
					BoundsDispatcher<0, DrawableCount>::Dispatch(this, left, top, right, bottom);
				}

				bool IsVisible() const
				{
					return Viewport.IsValid() && VisibilityChecker<0, DrawableCount>::Check(this);
				}

			protected:
				int16_t GetBoundsWidth() const { return Viewport.GetWidth(); }

				int16_t GetBoundsHeight() const { return Viewport.GetHeight(); }

				int16_t GetBoundsLeft() const { return Viewport.GetTopLeftX(); }

				int16_t GetBoundsTop() const { return Viewport.GetTopLeftY(); }

				int16_t GetBoundsRight() const { return Viewport.GetBottomRightX(); }

				int16_t GetBoundsBottom() const { return Viewport.GetBottomRightY(); }

				int16_t GetTranslationX() const { return Viewport.TranslationX; }

				int16_t GetTranslationY() const { return Viewport.TranslationY; }

			public:

				void SetTranslation(const int16_t x, const int16_t y)
				{
					Viewport.TranslationX = x;
					Viewport.TranslationY = y;
					TranslationDispatcher<0, DrawableCount>::Dispatch(this, x, y);
				}

				/// <summary>
				/// Access a specific const drawable by index.
				/// Example: drawable<0>().TransformShader.SetRotation(45);
				/// </summary>
				template<uint8_t Index>
				const typename Support::ParameterPack::GetHelper<Index, ChildType<DrawableTypes>...>::type& drawable() const
				{
					static_assert(Index < DrawableCount, "DrawablesView::drawable<Index>() - Index out of range.");
					return drawables_.template Get<Index>();
				}

				/// <summary>
				/// DrawCall orchestration implementing View contract:
				/// 1. Calls ViewStep() once at start of cycle to update animation state.
				/// 2. If ViewStep() returns false, the cycle is skipped and completed immediately.
				/// 3. Renders current drawable via Draw(framebuffer).
				/// 4. Advances to the next drawable on the next call.
				/// 5. Returns true when all drawables rendered (cycle complete).
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
						// If ViewStep returns false, skip the entire cycle
						if (!ViewStep(frameTime, frameCounter))
						{
							// Skip cycle - complete immediately (CurrentDrawable already 0)
							return true;
						}
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