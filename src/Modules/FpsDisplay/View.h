#ifndef _EGFX_MODULES_FPS_DISPLAY_LAYOUT_VIEW_h
#define _EGFX_MODULES_FPS_DISPLAY_LAYOUT_VIEW_h

#include "Drawable.h"

namespace Egfx
{
	namespace Modules
	{
		namespace FpsDisplay
		{
			namespace View
			{
				template<typename Layout
					, typename FontDrawerType = typename Definitions::AutoFontSelector<Layout>::Drawer
					, FpsDrawerPosition fpsDrawerPosition = FpsDrawerPosition::TopRight
					, uint8_t AveragingSampleCount = 5>
				class FpsText : public Framework::View::AbstractView
				{
				private:
					using Base = Framework::View::AbstractView;

				public:
					using DrawableType = Drawable::FpsText<Layout, FontDrawerType, fpsDrawerPosition>;

				private:
					enum class StateEnum : uint8_t
					{
						Disabled,
						WaitingFirstFrame,
						AccumulatingFirstFrames,
						AveragingFrames,
					};

				private:
					static constexpr uint32_t MaxSampleDuration = UINT32_MAX / AveragingSampleCount;

				private:
					DrawableType Drawable{};

				private:
					uint32_t LastFrametime = 0;
					uint32_t Accumulator = 0;

					uint16_t LastFrameRate = 0;
					uint8_t SampleCount = 0;
					StateEnum CurrentState = StateEnum::WaitingFirstFrame;

				public:
					FpsText() : Base() {}
					~FpsText() = default;

					// Expose underlying font drawer for configuration.
					FontDrawerType& FontDrawer()
					{
						return Drawable.TextDrawer;
					}

				protected:
					bool ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
					{
						const uint32_t frameDuration = frameTime - LastFrametime;
						LastFrametime = frameTime;

						switch (CurrentState)
						{
						case StateEnum::WaitingFirstFrame:
							CurrentState = StateEnum::AccumulatingFirstFrames;
							Accumulator = 0;
							SampleCount = 0;
							LastFrameRate = 0;
							break;
						case StateEnum::AccumulatingFirstFrames:
							Accumulator += MinValue(frameDuration, MaxSampleDuration);
							SampleCount++;
							LastFrameRate = Accumulator > 0 ? (uint32_t)1000000 * SampleCount / Accumulator : 0;
							if (SampleCount >= AveragingSampleCount)
							{
								CurrentState = StateEnum::AveragingFrames;
							}
							break;
						case StateEnum::AveragingFrames:
							Accumulator += MinValue(frameDuration, MaxSampleDuration);
							SampleCount++;
							if (SampleCount >= AveragingSampleCount)
							{
								LastFrameRate = Accumulator > 0 ? (uint32_t)1000000 * AveragingSampleCount / Accumulator : 0;
								Accumulator = 0;
								SampleCount = 0;
							}
							break;
						case StateEnum::Disabled:
						default:
							break;
						};

						Drawable.FrameRate = LastFrameRate;
						return true;
					}

					bool Draw(IFrameBuffer* frame) override
					{
						switch (CurrentState)
						{
						case StateEnum::Disabled:
						case StateEnum::WaitingFirstFrame:
							break;
						default:
							Drawable.Draw(frame);
							break;
						}

						return true;
					}
				};

				template<
					typename Layout,
					typename... ViewTypes>
				class CompositeWithFps : public Framework::View::CompositeView<
					ViewTypes...,
					FpsText<Layout>>
				{
				private:
					using FpsViewType = FpsText<Layout>;
					using Base = Framework::View::CompositeView<ViewTypes..., FpsViewType>;

				public:
					CompositeWithFps() = default;

					template<typename... Args>
					explicit CompositeWithFps(Args&&... args)
						: Base(static_cast<Args&&>(args)...)
					{
					}

					FpsViewType& FpsView()
					{
						return this->template view<sizeof...(ViewTypes)>();
					}
				};
			}
		}
	}
}
#endif