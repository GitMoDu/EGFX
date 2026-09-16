#ifndef _EGFX_FRAMEWORK_COMPOSITE_VIEW_h
#define _EGFX_FRAMEWORK_COMPOSITE_VIEW_h

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
			/// CompositeView orchestrates multiple child views that already implement the Drawable contract.
			/// Each child view receives the same frame parameters and advances sequentially.
			/// ViewStep() is invoked once per composite cycle before the first child view executes.
			/// Access individual child views via view<Index>() to configure shared state or wiring.
			/// The composite DrawCall completes only after every child view has reported completion.
			/// </summary>
			/// <typeparam name="ViewTypes">Child view types implementing DrawCall().</typeparam>
			template<typename ParentLayout, typename... ViewTypes>
			class CompositeView
			{
			public:
				using ViewLayout = ParentLayout;

			private:
				template<typename ViewType>
				using ChildType = Detail::LayoutChild<ViewType, ParentLayout, typename ViewType::ViewLayout>;

			public:
				static constexpr uint8_t ViewCount = static_cast<uint8_t>(sizeof...(ViewTypes));

			private:
				template<uint8_t Index, uint8_t N>
				struct Dispatcher
				{
					static bool Dispatch(CompositeView* self, IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter, const uint8_t target)
					{
						if (Index == target)
						{
							if (!self->InnerViews.template Get<Index>().IsVisible())
							{
								return true;
							}

							return self->template DrawViewAt<Index>(frame, frameTime, frameCounter);
						}

						return Dispatcher<Index + 1, N>::Dispatch(self, frame, frameTime, frameCounter, target);
					}
				};

				template<uint8_t Index, uint8_t N>
				struct VisibilityChecker
				{
					static bool Check(const CompositeView* self)
					{
						if (self->InnerViews.template Get<Index>().IsVisible())
						{
							return true;
						}

						return VisibilityChecker<Index + 1, N>::Check(self);
					}
				};

				template<uint8_t N>
				struct VisibilityChecker<N, N>
				{
					static bool Check(const CompositeView*) { return false; }
				};

				template<uint8_t N>
				struct Dispatcher<N, N>
				{
					static bool Dispatch(CompositeView*, IFrameBuffer*, const uint32_t, const uint16_t, const uint8_t)
					{
						return true;
					}
				};

			private:
				Support::ParameterPack::ElementPack<ChildType<ViewTypes>...> InnerViews;
				Shader::viewport_t<int16_t> Viewport{
					static_cast<int16_t>(0),
					static_cast<int16_t>(0),
					static_cast<int16_t>(ParentLayout::Width()),
					static_cast<int16_t>(ParentLayout::Height()),
					0, 0 };
				uint8_t CurrentView = 0;
				bool Stepped = false;

			protected:
				/// <summary>
				/// Composite-level animation step invoked once per cycle before any child view renders.
				/// Override to update shared state or coordinate child views.
				/// </summary>
				/// <param name="frameTime">Rolling frame timestamp (microseconds).</param>
				/// <param name="frameCounter">Rolling frame counter.</param>
				/// <returns>True to proceed with rendering, false to skip the entire cycle.</returns>
				virtual bool ViewStep(const uint32_t /*frameTime*/, const uint16_t /*frameCounter*/) { return true; }

			public:
				CompositeView() : InnerViews() {}

				template<typename... Args>
				explicit CompositeView(Args&&... args)
					: InnerViews(static_cast<Args&&>(args)...) {}

				virtual ~CompositeView() = default;

				void SetBounds(const int16_t left, const int16_t top,
					const int16_t right, const int16_t bottom)
				{
					Viewport.BoundsLeft = left;
					Viewport.BoundsTop = top;
					Viewport.BoundsRight = right;
					Viewport.BoundsBottom = bottom;
					BoundsDispatcher<0, ViewCount>::Dispatch(this, left, top, right, bottom);
				}

				bool IsVisible() const
				{
					return Viewport.IsValid() && VisibilityChecker<0, ViewCount>::Check(this);
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
					TranslationDispatcher<0, ViewCount>::Dispatch(this, x, y);
				}

				/// <summary>
				/// Access a specific view by index.
				/// Example: view<0>().Color = rgb_color_t{255, 0, 0};
				/// </summary>
				template<uint8_t Index>
				typename Support::ParameterPack::GetHelper<Index, ChildType<ViewTypes>...>::type& view()
				{
					return InnerViews.template Get<Index>();
				}

				/// <summary>
				/// Access a specific const view by index.
				/// Example: view<0>().Color = rgb_color_t{255, 0, 0};
				/// </summary>
				template<uint8_t Index>
				const typename Support::ParameterPack::GetHelper<Index, ChildType<ViewTypes>...>::type& view() const
				{
					return InnerViews.template Get<Index>();
				}

				/// <summary>
				/// Visits a child selected by a runtime index.
				/// The visitor must provide a templated call operator accepting the selected child.
				/// </summary>
				template<typename Visitor>
				bool VisitView(const uint8_t index, Visitor&& visitor)
				{
					return RuntimeViewDispatcher<0, ViewCount, Visitor>::Dispatch(this, index, visitor);
				}

			/// <summary>
			/// Visits a child selected by a runtime index through a shared interface.
			/// This overload supports ordinary C++11 lambdas for heterogeneous child views.
			/// Every child type must derive from Interface.
			/// </summary>
			template<typename Interface, typename Visitor>
			bool VisitView(const uint8_t index, Visitor&& visitor)
			{
				return InterfaceViewDispatcher<Interface, 0, ViewCount, Visitor>::Dispatch(this, index, visitor);
			}

				/// <summary>
				/// Advances the current child view and completes when all child views have completed.
				/// If ViewStep() returns false, the cycle is skipped and completed immediately.
				/// </summary>
				/// <param name="frame">Target framebuffer to draw into.</param>
				/// <param name="frameTime">Rolling frame timestamp (microseconds).</param>
				/// <param name="frameCounter">Rolling frame counter.</param>
				/// <returns>True when the view has completed its current draw cycle.</returns>
				bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter)
				{
					if (ViewCount == 0u)
					{
						Stepped = false;
						return true;
					}
					if (CurrentView >= ViewCount)
					{
						CurrentView = 0;
					}

					if (!Stepped)
					{
						// If ViewStep returns false, skip the entire cycle
						if (!ViewStep(frameTime, frameCounter))
						{
							// Skip cycle - reset state and complete immediately
							Stepped = false;
							CurrentView = 0;
							return true;
						}
						Stepped = true;
					}

					const bool completed = Dispatcher<0, ViewCount>::Dispatch(this, frame, frameTime, frameCounter, CurrentView);

					if (completed)
					{
						++CurrentView;

						if (CurrentView >= ViewCount)
						{
							CurrentView = 0;
							Stepped = false;
							return true;
						}
					}

					return false;
				}

			private:
				template<uint8_t Index, uint8_t N, typename Visitor>
				struct RuntimeViewDispatcher
				{
					static bool Dispatch(CompositeView* self, const uint8_t target, Visitor& visitor)
					{
						if (Index == target)
						{
							visitor(self->InnerViews.template Get<Index>());
							return true;
						}

						return RuntimeViewDispatcher<Index + 1, N, Visitor>::Dispatch(self, target, visitor);
					}
				};

				template<uint8_t N, typename Visitor>
				struct RuntimeViewDispatcher<N, N, Visitor>
				{
					static bool Dispatch(CompositeView*, const uint8_t, Visitor&) { return false; }
				};

			template<typename Interface, uint8_t Index, uint8_t N, typename Visitor>
			struct InterfaceViewDispatcher
			{
				static bool Dispatch(CompositeView* self, const uint8_t target, Visitor& visitor)
				{
					if (Index == target)
					{
						visitor(static_cast<Interface&>(self->InnerViews.template Get<Index>()));
						return true;
					}

					return InterfaceViewDispatcher<Interface, Index + 1, N, Visitor>::Dispatch(self, target, visitor);
				}
			};

			template<typename Interface, uint8_t N, typename Visitor>
			struct InterfaceViewDispatcher<Interface, N, N, Visitor>
			{
				static bool Dispatch(CompositeView*, const uint8_t, Visitor&) { return false; }
			};

				template<uint8_t Index, uint8_t N>
				struct BoundsDispatcher
				{
					static void Dispatch(CompositeView* self, const int16_t left, const int16_t top,
						const int16_t right, const int16_t bottom)
					{
						self->InnerViews.template Get<Index>().SetBounds(left, top, right, bottom);
						BoundsDispatcher<Index + 1, N>::Dispatch(self, left, top, right, bottom);
					}
				};

				template<uint8_t N>
				struct BoundsDispatcher<N, N>
				{
					static void Dispatch(CompositeView*, const int16_t, const int16_t,
						const int16_t, const int16_t) {}
				};

				template<uint8_t Index, uint8_t N>
				struct TranslationDispatcher
				{
					static void Dispatch(CompositeView* self, const int16_t x, const int16_t y)
					{
						self->InnerViews.template Get<Index>().SetTranslation(x, y);
						TranslationDispatcher<Index + 1, N>::Dispatch(self, x, y);
					}
				};

				template<uint8_t N>
				struct TranslationDispatcher<N, N>
				{
					static void Dispatch(CompositeView*, const int16_t, const int16_t) {}
				};

				template<uint8_t Index>
				bool DrawViewAt(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter)
				{
					return InnerViews.template Get<Index>().DrawCall(frame, frameTime, frameCounter);
				}

			};
		}
	}
}
#endif