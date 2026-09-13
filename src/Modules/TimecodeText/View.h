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
			template<typename ParentLayout,
				typename FontType = TimecodeText::DefaultFont,
				typename ColorShaderType = Framework::Shader::Color::NoShader<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>,
				typename TransformShaderType = Framework::Shader::Transform::NoTransform<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>
			>
			class View : public Egfx::Framework::View::AbstractView
			{
			private:
				using Base = Egfx::Framework::View::AbstractView;
				using dimension_t = typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t;
				using DrawableLayout = Framework::Layout::Align<ParentLayout,
					Layout::Time<ParentLayout>,
					Framework::Layout::AlignmentEnum::MiddleCenter>;
				using DrawableType = Drawable::Timecode<dimension_t, DrawableLayout,
					FontType, ColorShaderType, TransformShaderType>;

				DrawableType TimecodeDrawable{};

			public:
				View() : Base() {}
				~View() = default;

				DrawableType& Drawable()
				{
					return TimecodeDrawable;
				}

				void SetTextColor(const Egfx::rgb_color_t color)
				{
					Drawable().GetTextDrawer().ColorSource.Color = color;
				}

				void SetMinutes(const uint32_t totalMilliseconds)
				{
					Drawable().SetMinutes(totalMilliseconds);
				}

				void SetHours(const uint32_t totalSeconds)
				{
					Drawable().SetHours(totalSeconds);
				}

				void SetMode(const PresentModeEnum mode)
				{
					Drawable().CurrentMode = mode;
				}

				void SetBounds(const pixel_t left, const pixel_t top,
					const pixel_t right, const pixel_t bottom)
				{
					Drawable().SetBounds(left, top, right, bottom);
				}

				void SetTranslation(const int16_t x, const int16_t y)
				{
					Drawable().SetTranslation(x, y);
				}

			protected:
				bool ViewStep(const uint32_t /*frameTime*/, const uint16_t /*frameCounter*/) override
				{
					return Drawable().CurrentMode != PresentModeEnum::Invisible;
				}

				bool Draw(IFrameBuffer* frame) override
				{
					return Drawable().Draw(frame);
				}
			};

			namespace Demo
			{
				template<typename ParentLayout,
					bool UseHours = false,
					typename ColorShaderType = Framework::Shader::Color::NoShader<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>,
					typename TransformShaderType = Framework::Shader::Transform::NoTransform<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>
				>
				class DemoView : public View<ParentLayout, TimecodeText::DefaultFont, ColorShaderType, TransformShaderType>
				{
				private:
					using Base = View<ParentLayout, TimecodeText::DefaultFont, ColorShaderType, TransformShaderType>;

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
					DemoView() : Base()
					{
						auto& timecodeDrawable = this->Drawable();
						timecodeDrawable.CurrentMode = PresentModeEnum::NoDuration;
					}

					~DemoView() = default;

				protected:
					bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
					{
						auto& timecodeDrawable = this->Drawable();

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