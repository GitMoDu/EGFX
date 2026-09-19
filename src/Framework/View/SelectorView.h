#ifndef _EGFX_FRAMEWORK_SELECTOR_VIEW_h
#define _EGFX_FRAMEWORK_SELECTOR_VIEW_h

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
			/// SelectorView owns multiple child views and renders only the selected child view.
			/// Bounds and translation are forwarded to every child so selection can change without reconfiguration.
			/// ViewStep() is invoked once per cycle before the selected child view executes.
			/// Access individual child views via view<Index>() and select one with SetSelectedView().
			/// </summary>
			/// <typeparam name="ViewTypes">Child view types implementing DrawCall().</typeparam>
			template<typename ParentLayout, typename... ViewTypes>
			class SelectorView
			{
			public:
				using ViewLayout = ParentLayout;
				static constexpr uint8_t ViewCount = static_cast<uint8_t>(sizeof...(ViewTypes));

			private:
				template<typename ViewType>
				using ChildType = Detail::LayoutChild<ViewType, ParentLayout, typename ViewType::ViewLayout>;

				Support::ParameterPack::ElementPack<ChildType<ViewTypes>...> InnerViews;
				Shader::viewport_t<int16_t> Viewport{
					static_cast<int16_t>(0),
					static_cast<int16_t>(0),
					static_cast<int16_t>(ParentLayout::Width()),
					static_cast<int16_t>(ParentLayout::Height()),
					0, 0 };
				uint8_t SelectedView = 0;
				bool Stepped = false;

				template<uint8_t Index, uint8_t N>
				struct BoundsDispatcher
				{
					static void Dispatch(SelectorView* self, const int16_t left, const int16_t top,
						const int16_t right, const int16_t bottom)
					{
						self->InnerViews.template Get<Index>().SetBounds(left, top, right, bottom);
						BoundsDispatcher<Index + 1, N>::Dispatch(self, left, top, right, bottom);
					}
				};

				template<uint8_t N>
				struct BoundsDispatcher<N, N>
				{
					static void Dispatch(SelectorView*, const int16_t, const int16_t,
						const int16_t, const int16_t) {}
				};

				template<uint8_t Index, uint8_t N>
				struct TranslationDispatcher
				{
					static void Dispatch(SelectorView* self, const int16_t x, const int16_t y)
					{
						self->InnerViews.template Get<Index>().SetTranslation(x, y);
						TranslationDispatcher<Index + 1, N>::Dispatch(self, x, y);
					}
				};

				template<uint8_t N>
				struct TranslationDispatcher<N, N>
				{
					static void Dispatch(SelectorView*, const int16_t, const int16_t) {}
				};

				template<uint8_t Index, uint8_t N>
				struct VisibilityDispatcher
				{
					static bool Check(const SelectorView* self)
					{
						if (Index == self->SelectedView)
						{
							return self->InnerViews.template Get<Index>().IsVisible();
						}

						return VisibilityDispatcher<Index + 1, N>::Check(self);
					}
				};

				template<uint8_t N>
				struct VisibilityDispatcher<N, N>
				{
					static bool Check(const SelectorView*) { return false; }
				};

				template<uint8_t Index, uint8_t N>
				struct DrawDispatcher
				{
					static bool Dispatch(SelectorView* self, IFrameBuffer* frame,
						const uint32_t frameTime, const uint16_t frameCounter)
					{
						if (Index == self->SelectedView)
						{
							return self->InnerViews.template Get<Index>().DrawCall(frame, frameTime, frameCounter);
						}

						return DrawDispatcher<Index + 1, N>::Dispatch(self, frame, frameTime, frameCounter);
					}
				};

				template<uint8_t N>
				struct DrawDispatcher<N, N>
				{
					static bool Dispatch(SelectorView*, IFrameBuffer*, const uint32_t,
						const uint16_t) { return true; }
				};

			protected:
				virtual bool ViewStep(const uint32_t /*frameTime*/, const uint16_t /*frameCounter*/) { return true; }

			public:
				SelectorView() : InnerViews() {}

				template<typename... Args>
				explicit SelectorView(Args&&... args)
					: InnerViews(static_cast<Args&&>(args)...) {}

				virtual ~SelectorView() = default;

				bool SetSelectedView(const uint8_t index)
				{
					if (index >= ViewCount)
					{
						return false;
					}

					SelectedView = index;
					Stepped = false;
					return true;
				}

				uint8_t GetSelectedView() const
				{
					return SelectedView;
				}

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
					return Viewport.IsValid() && VisibilityDispatcher<0, ViewCount>::Check(this);
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

				template<uint8_t Index>
				typename Support::ParameterPack::GetHelper<Index, ChildType<ViewTypes>...>::type& view()
				{
					static_assert(Index < ViewCount, "SelectorView::view<Index>() - Index out of range");
					return InnerViews.template Get<Index>();
				}

				template<uint8_t Index>
				const typename Support::ParameterPack::GetHelper<Index, ChildType<ViewTypes>...>::type& view() const
				{
					static_assert(Index < ViewCount, "SelectorView::view<Index>() - Index out of range");
					return InnerViews.template Get<Index>();
				}

				bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter)
				{
					if (ViewCount == 0u)
					{
						Stepped = false;
						return true;
					}

					if (!Stepped)
					{
						if (!ViewStep(frameTime, frameCounter))
						{
							Stepped = false;
							return true;
						}

						Stepped = true;
					}

					if (DrawDispatcher<0, ViewCount>::Dispatch(this, frame, frameTime, frameCounter))
					{
						Stepped = false;
						return true;
					}

					return false;
				}
			};
		}
	}
}

#endif
