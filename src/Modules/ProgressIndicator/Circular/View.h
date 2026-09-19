#ifndef _EGFX_MODULES_PROGRESS_INDICATOR_CIRCULAR_VIEW_h
#define _EGFX_MODULES_PROGRESS_INDICATOR_CIRCULAR_VIEW_h

#include "Constant.h"
#include "Drawable.h"

namespace Egfx
{
	namespace Modules
	{
		namespace ProgressIndicator
		{
			namespace Circular
			{
				template<typename dimension_t,
					typename ParentLayout,
					typename Style = TemplateCircularStyle<>,
					typename FillPixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>,
					typename BorderPixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>,
					typename TrackPixelShaderType = Framework::Shader::Pixel::MockShader<dimension_t>
				>
				class View :public Framework::View::DrawablesView<ParentLayout,
					Drawable::CircularTrack<dimension_t, ParentLayout, Style, TrackPixelShaderType>,
					Drawable::CircularIndicator<dimension_t, ParentLayout, Style, FillPixelShaderType>,
					Drawable::CircularBorder<dimension_t, ParentLayout, Style, BorderPixelShaderType>
				>
				{
				private:
					using Base = Framework::View::DrawablesView<ParentLayout,
						Drawable::CircularTrack<dimension_t, ParentLayout, Style, TrackPixelShaderType>,
						Drawable::CircularIndicator<dimension_t, ParentLayout, Style, FillPixelShaderType>,
						Drawable::CircularBorder<dimension_t, ParentLayout, Style, BorderPixelShaderType>
					>;

					static constexpr bool HasTrack = !TypeTraits::TypeDispatch::is_same<TrackPixelShaderType, Framework::Shader::Pixel::MockShader<dimension_t>>::value;

				private:
					ufraction16_t Progress = 0;

				public:
					View() : Base() {}
					~View() = default;

					void SetProgress(const ufraction16_t progress)
					{
						const angle_t angleSweep = UFraction16::Fraction<angle_t>(progress, Style::EndAngle - Style::StartAngle);
						auto& indicator = Base::template drawable<1>();
						indicator.SetFilledAngleSweep(angleSweep);

						if (HasTrack)
						{
							auto& track = Base::template drawable<0>();
							track.SetFilledAngleSweep(angleSweep);
						}

						Progress = progress;
					}

					ufraction16_t GetProgress() const
					{
						return Progress;
					}

					static constexpr angle_t GetStartAngle()
					{
						return Style::StartAngle;
					}

					static constexpr angle_t GetEndAngle()
					{
						return Style::EndAngle;
					}

					static constexpr angle_t GetAngleRange()
					{
						return Style::EndAngle - Style::StartAngle;
					}
				};

				namespace Demo
				{
					static constexpr uint32_t CyclePeriod = 4333000 * 3;

					template<typename ParentLayout>
					static constexpr uint16_t ArcThickness()
					{
						return (MinValue<pixel_t>(ParentLayout::Width(), ParentLayout::Height()) / 4) + 1;
					}

					template<typename ParentLayout>
					static constexpr uint16_t BorderThickness()
					{
						return MaxValue<uint16_t>(1, (MinValue<pixel_t>(ParentLayout::Width(), ParentLayout::Height()) / 32));
					}

					template<typename ParentLayout>
					static constexpr uint16_t TrackMargin()
					{
						return MaxValue<int16_t>(1, int16_t(BorderThickness<ParentLayout>()) + 1);
					}

					template<typename ParentLayout>
					using DemoStyle = TemplateCircularStyle<
						ArcThickness<ParentLayout>(),
						BorderThickness<ParentLayout>(), BorderThickness<ParentLayout>(),
						0, TrackMargin<ParentLayout>()
					>;


					template<typename ParentLayout>
					using IndicatorPixelShaderColor = Framework::Shader::Pixel::TemplateShader<
						typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t
						, Modules::Kit::Shader::Source::GradientAngular<
						typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
						ParentLayout::Width(), ParentLayout::Height(), true>,
						Framework::Shader::Color::NoShader<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>,
						Framework::Shader::Transform::NoTransform<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>,
						::Egfx::Framework::Shader::Pixel::BlendModeEnum::Add
					>;

					template<typename ParentLayout>
					using IndicatorPixelShaderMonochrome = Framework::Shader::Pixel::TemplateShader<
						typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>;

					template<typename ParentLayout>
					using BorderPixelShader = Framework::Shader::Pixel::TemplateShader<
						typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>;

					template<typename ParentLayout>
					class DemoCircularMonochrome : public View<
						typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t, ParentLayout,
						Demo::DemoStyle<ParentLayout>,
						Demo::IndicatorPixelShaderMonochrome<ParentLayout>,
						Demo::BorderPixelShader<ParentLayout>
					>
					{
					private:
						using dimension_t = typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t;

						using Base = View<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
							ParentLayout,
							Demo::DemoStyle<ParentLayout>,
							Demo::IndicatorPixelShaderMonochrome<ParentLayout>,
							Demo::BorderPixelShader<ParentLayout>
						>;

					public:
						DemoCircularMonochrome() : Base() {}
						~DemoCircularMonochrome() = default;

					protected:
						bool ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
						{
							// Use a triangle wave to animate the progress indicator back and forth.
							const auto triangleElapsed = Framework::Animation::TimeCurve::Triangle<Demo::CyclePeriod>(frameTime);

							// Set the progress of progress indicator in ufraction16 format (0 to Fraction16::Max).
							const auto triangleProgress = UFraction16::GetScalar<uint32_t>(triangleElapsed, Demo::CyclePeriod);

							Base::SetProgress(triangleProgress);

							return true;
						}
					};


					template<typename ParentLayout>
					class DemoCircularColor : public View<
						typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t, ParentLayout,
						Demo::DemoStyle<ParentLayout>,
						Demo::IndicatorPixelShaderColor<ParentLayout>,
						Demo::BorderPixelShader<ParentLayout>
					>
					{
					private:
						using Base = View<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
							ParentLayout,
							Demo::DemoStyle<ParentLayout>,
							Demo::IndicatorPixelShaderColor<ParentLayout>,
							Demo::BorderPixelShader<ParentLayout>
						>;

					public:
						DemoCircularColor() : Base()
						{
							auto& fill = Base::template colorSource<1>();
							fill.SetStartAngle(Demo::DemoStyle<ParentLayout>::StartAngle);
							fill.SetEndAngle(Demo::DemoStyle<ParentLayout>::EndAngle);
							fill.SetColors(Rgb::Color(uint32_t(0x4E4E4E)), Rgb::Color(uint32_t(0xFFFFFF)));
						}

					protected:
						bool ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
						{
							// Use a triangle wave to animate the progress indicator back and forth.
							const auto triangleElapsed = Framework::Animation::TimeCurve::Triangle<Demo::CyclePeriod>(frameTime);

							// Set the progress of progress indicator in ufraction16 format (0 to Fraction16::Max).
							const auto triangleProgress = UFraction16::GetScalar<uint32_t>(triangleElapsed, Demo::CyclePeriod);
							Base::SetProgress(triangleProgress);

							return true;
						}
					};

					template<typename ParentLayout, bool Monochrome = true>
					using DemoView = typename TypeTraits::TypeConditional::conditional_type<
						DemoCircularMonochrome<ParentLayout>,
						DemoCircularColor<ParentLayout>,
						Monochrome
					>::type;
				}
			}
		}
	}
}
#endif