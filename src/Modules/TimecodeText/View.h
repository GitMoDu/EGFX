#ifndef _EGFX_MODULES_TIMECODE_TEXT_VIEW_h
#define _EGFX_MODULES_TIMECODE_TEXT_VIEW_h

#include "Constant.h"
#include "Layout.h"
#include "Drawable.h"

namespace Egfx
{
	namespace Modules
	{
		namespace TimecodeText
		{
			namespace View
			{
				template<typename ParentLayout, bool Monochrome>
				class TimecodeView : public Egfx::Framework::View::DrawablesView<
					Drawable::Timecode<Framework::Layout::Align<ParentLayout,
					Layout::Time<ParentLayout>,
					Framework::Layout::AlignmentEnum::MiddleCenter>>
					>
				{
				private:
					using Base = Egfx::Framework::View::DrawablesView<
						Drawable::Timecode<Framework::Layout::Align<ParentLayout,
						Layout::Time<ParentLayout>,
						Framework::Layout::AlignmentEnum::MiddleCenter>>
						>;

				public:
					TimecodeView() : Base() {}
					~TimecodeView() = default;

					void SetTextColor(const Egfx::rgb_color_t color)
					{
						auto& timecodeDrawable = this->template drawable<0>();
						timecodeDrawable.GetTextDrawer().SetColor(color);
					}

					void SetMinutes(const uint32_t totalMilliseconds)
					{
						auto& timecodeDrawable = this->template drawable<0>();
						timecodeDrawable.SetMinutes(totalMilliseconds);
					}

					void SetHours(const uint32_t totalSeconds)
					{
						auto& timecodeDrawable = this->template drawable<0>();
						timecodeDrawable.SetHours(totalSeconds);
					}

					void SetMode(const PresentModeEnum mode)
					{
						auto& timecodeDrawable = this->template drawable<0>();
						timecodeDrawable.CurrentMode = mode;
					}
				};

				template<typename ParentLayout, bool Monochrome, bool UseHours = false>
				class TimecodeDemoView : public TimecodeView<ParentLayout, Monochrome>
				{
				private:
					using Base = TimecodeView<ParentLayout, Monochrome>;

					enum class DemoStateEnum : uint8_t
					{
						StartAnimation,
						StartDelay,
						StartFlash,
						Running,
						EnumCount
					};

				private:
					static constexpr uint32_t StartDelay = 2500000;
					static constexpr uint32_t FlashDuration = 200000;
					static constexpr uint32_t FlashDelay = 3000000;

				private:
					uint32_t StartAnimation = 0;
					uint32_t StartDuration = 0;
					uint32_t LastTotal = UINT32_MAX;
					DemoStateEnum State = DemoStateEnum::StartAnimation;

				public:
					TimecodeDemoView() : Base()
					{
						auto& timecodeDrawable = this->template drawable<0>();
						timecodeDrawable.CurrentMode = PresentModeEnum::NoDuration;
					}

					~TimecodeDemoView() = default;

				protected:
					bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
					{
						auto& timecodeDrawable = this->template drawable<0>();

						switch (State)
						{
						case DemoStateEnum::StartAnimation:
							State = DemoStateEnum::StartDelay;
							StartAnimation = frameTime;
							timecodeDrawable.CurrentMode = PresentModeEnum::NoDuration;
							break;
						case DemoStateEnum::StartDelay:
							if ((frameTime - StartAnimation) >= StartDelay)
							{
								State = DemoStateEnum::StartFlash;
								timecodeDrawable.CurrentMode = PresentModeEnum::Duration;
								StartAnimation = frameTime; // Set start reference to now for the flash timing.
								StartDuration = millis(); // Set start reference to now for the duration counting.
							}
							break;
						case DemoStateEnum::StartFlash:
							if ((frameTime - StartAnimation) >= FlashDelay)
							{
								State = DemoStateEnum::Running;
								timecodeDrawable.CurrentMode = PresentModeEnum::Duration;
							}
							else
							{
								if ((frameTime - StartAnimation) % (FlashDuration + 1) >= ((FlashDuration * 3) / 4))
								{
									timecodeDrawable.CurrentMode = PresentModeEnum::Invisible;
								}
								else
								{
									timecodeDrawable.CurrentMode = PresentModeEnum::Duration;
								}
							}
							break;
						case DemoStateEnum::Running:
							// Stay in this state indefinitely.
						default:
							break;
						}

						uint32_t timestamp = millis() - StartDuration;
						if (UseHours)
						{
							timestamp = timestamp / 1000;
						}

						if (timestamp != LastTotal)
						{
							LastTotal = timestamp;
							if (UseHours)
							{
								timecodeDrawable.SetHours(timestamp);
							}
							else
							{
								timecodeDrawable.SetMinutes(timestamp);
							}
						}

						return Base::ViewStep(frameTime, frameCounter);
					}
				};
			}
		}
	}
}
#endif