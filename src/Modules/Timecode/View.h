#ifndef _INTEGERGLASS_MODULES_TIMECODE_VIEW_h
#define _INTEGERGLASS_MODULES_TIMECODE_VIEW_h

#include "Constant.h"
#include "Layout.h"
#include "Drawable.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Timecode
		{
			template<typename ParentLayout,
				typename FontType = Styles::DefaultFont,
				typename ColorShaderType = Framework::Shader::Color::NoShader<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>,
				typename TransformShaderType = Framework::Shader::Transform::NoTransform<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>
			>
			class View : public IntegerGlass::Framework::View::DrawablesView<ParentLayout,
				Drawable::Timecode<
					typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
					Framework::Layout::Align<ParentLayout,
						Layout::Time<ParentLayout>,
						Framework::Layout::AlignmentEnum::MiddleCenter>,
					FontType, ColorShaderType, TransformShaderType>>
			{
			public:
				using ViewLayout = ParentLayout;

			private:
				using dimension_t = typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t;
				using DrawableLayout = Framework::Layout::Align<ParentLayout,
					Layout::Time<ParentLayout>,
					Framework::Layout::AlignmentEnum::MiddleCenter>;
				using DrawableType = Drawable::Timecode<dimension_t, DrawableLayout,
					FontType, ColorShaderType, TransformShaderType>;
				using Base = IntegerGlass::Framework::View::DrawablesView<ParentLayout, DrawableType>;

			public:
				View() : Base() {}
				~View() = default;

				DrawableType& Drawable()
				{
					return Base::template drawable<0>();
				}

				void SetTextColor(const IntegerGlass::rgb_color_t color)
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

			protected:
				bool ViewStep(const uint32_t /*frameTime*/, const uint16_t /*frameCounter*/) override
				{
					return Drawable().CurrentMode != PresentModeEnum::Invisible;
				}
			};

			namespace Demo
			{
				template<typename ParentLayout,
					bool UseHours = false,
					typename ColorShaderType = Framework::Shader::Color::NoShader<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>,
					typename TransformShaderType = Framework::Shader::Transform::NoTransform<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>
				>
				class DemoView : public View<ParentLayout, Timecode::Styles::DefaultFont, ColorShaderType, TransformShaderType>
				{
				private:
					using Base = View<ParentLayout, Timecode::Styles::DefaultFont, ColorShaderType, TransformShaderType>;

					enum class DemoStateEnum : uint8_t
					{
						StartAnimation,
						StartDelay,
						StartFlash,
						Running,
						EnumCount
					};

				private:
					static constexpr uint32_t StartDelay = 1000000;
					static constexpr uint32_t FlashDuration = 200000;
					static constexpr uint32_t FlashDelay = 1000000;

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