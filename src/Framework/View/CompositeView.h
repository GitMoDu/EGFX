#ifndef _EGFX_FRAMEWORK_COMPOSITE_VIEW_h
#define _EGFX_FRAMEWORK_COMPOSITE_VIEW_h

#include "Model.h"

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
			template<typename... ViewTypes>
			class CompositeView
			{
			private:
				Support::ParameterPack::ElementPack<ViewTypes...> InnerViews;
				uint8_t CurrentView = 0;
				bool Stepped = false;
				static constexpr uint8_t ViewCount = static_cast<uint8_t>(sizeof...(ViewTypes));

				template<uint8_t Index>
				bool DrawViewAt(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter)
				{
					return InnerViews.template Get<Index>().DrawCall(frame, frameTime, frameCounter);
				}

				template<uint8_t Index, uint8_t N>
				struct Dispatcher
				{
					static bool Dispatch(CompositeView* self, IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter, const uint8_t target)
					{
						if (Index == target)
						{
							return self->template DrawViewAt<Index>(frame, frameTime, frameCounter);
						}

						return Dispatcher<Index + 1, N>::Dispatch(self, frame, frameTime, frameCounter, target);
					}
				};

				template<uint8_t N>
				struct Dispatcher<N, N>
				{
					static bool Dispatch(CompositeView*, IFrameBuffer*, const uint32_t, const uint16_t, const uint8_t)
					{
						return true;
					}
				};

			protected:
				/// <summary>
				/// Composite-level animation step invoked once per cycle before any child view renders.
				/// Override to update shared state or coordinate child views.
				/// </summary>
				/// <param name="frameTime">Rolling frame timestamp (microseconds).</param>
				/// <param name="frameCounter">Rolling frame counter.</param>
				virtual void ViewStep(const uint32_t /*frameTime*/, const uint16_t /*frameCounter*/) {}

			public:
				CompositeView() : InnerViews() {}

				template<typename... Args>
				explicit CompositeView(Args&&... args)
					: InnerViews(static_cast<Args&&>(args)...) {
				}

				virtual ~CompositeView() = default;

				/// <summary>
				/// Access a specific view by index.
				/// Example: view<0>().Color = rgb_color_t{255, 0, 0};
				/// </summary>
				template<uint8_t Index>
				typename Support::ParameterPack::GetHelper<Index, ViewTypes...>::type& view()
				{
					return InnerViews.template Get<Index>();
				}

				/// <summary>
				/// Access a specific const view by index.
				/// Example: view<0>().Color = rgb_color_t{255, 0, 0};
				/// </summary>
				template<uint8_t Index>
				const typename Support::ParameterPack::GetHelper<Index, ViewTypes...>::type& view() const
				{
					return InnerViews.template Get<Index>();
				}

				/// <summary>
				/// Advances the current child view and completes when all child views have completed.
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
						ViewStep(frameTime, frameCounter);
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
			};
		}
	}
}
#endif