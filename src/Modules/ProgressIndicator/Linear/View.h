#ifndef _EGFX_MODULES_PROGRESS_INDICATOR_LINEAR_VIEW_h
#define _EGFX_MODULES_PROGRESS_INDICATOR_LINEAR_VIEW_h

#include "Constant.h"
#include "Drawable.h"

namespace Egfx
{
	namespace Modules
	{
		namespace ProgressIndicator
		{
			namespace Linear
			{
				template<typename dimension_t,
					typename ParentLayout,
					typename Style = TemplateLinearProgressStyle<>,
					typename IndicatorPixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>,
					typename BorderPixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>,
					typename TrackPixelShaderType = Framework::Shader::Pixel::MockShader<dimension_t>
				>
				class View : public Framework::View::DrawablesView<
					Drawable::Track<dimension_t, ParentLayout, Style, TrackPixelShaderType>,
					Drawable::Indicator<dimension_t, ParentLayout, Style, IndicatorPixelShaderType>,
					Drawable::Border<dimension_t, ParentLayout, Style, BorderPixelShaderType>
				>
				{
				private:
					using Base = Framework::View::DrawablesView<
						Drawable::Track<dimension_t, ParentLayout, Style, TrackPixelShaderType>,
						Drawable::Indicator<dimension_t, ParentLayout, Style, IndicatorPixelShaderType>,
						Drawable::Border<dimension_t, ParentLayout, Style, BorderPixelShaderType>
					>;

					using LayoutCalc = Layout::Linear<ParentLayout, Style>;
					using LayoutFill = typename LayoutCalc::RoundedRectangleFill;

				private:
					static constexpr bool HasTrack = !TypeTraits::TypeDispatch::is_same<TrackPixelShaderType, Framework::Shader::Pixel::MockShader<dimension_t>>::value;

				public:
					View() {}
					~View() = default;

					void SetBounds(const dimension_t left, const dimension_t top,
						const dimension_t right, const dimension_t bottom)
					{
						Base::template drawable<0>().SetBounds(left, top, right, bottom);
						Base::template drawable<1>().SetBounds(left, top, right, bottom);
						Base::template drawable<2>().SetBounds(left, top, right, bottom);
					}

					void SetTranslation(const int16_t x, const int16_t y)
					{
						Base::template drawable<0>().SetTranslation(x, y);
						Base::template drawable<1>().SetTranslation(x, y);
						Base::template drawable<2>().SetTranslation(x, y);
					}

					void SetProgress(const UFraction16::scalar_t progress)
					{
						const dimension_t filledDimension = UFraction16::Fraction(progress, LayoutFill::RangeLength());
						auto& indicator = Base::template drawable<1>();
						indicator.SetFilledDimension(filledDimension);

						if (HasTrack)
						{
							auto& track = Base::template drawable<0>();
							track.SetFilledDimension(filledDimension);
						}
					}

					UFraction16::scalar_t GetProgress() const
					{
						return Base::template drawable<0>().GetProgress();
					}
				};


				namespace Demo
				{
					template<typename ParentLayout>
					struct Style
					{
						using dimension_t = typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t;

						static constexpr dimension_t CornerRadius()
						{
							return MaxValue<int16_t>(1, (MinValue(ParentLayout::Width(), ParentLayout::Height()) / 2) - 1);
						}

						using IndicatorPixelShaderColor = Framework::Shader::Pixel::TemplateShader<dimension_t,
							Framework::Assets::Shader::Source::GradientY<dimension_t, ParentLayout::Height(), true>>;

						using IndicatorPixelShaderMonochrome = Framework::Shader::Pixel::TemplateShader<dimension_t>;

						template<bool Monochrome>
						using IndicatorPixelShader = typename TypeTraits::TypeConditional::conditional_type<
							IndicatorPixelShaderMonochrome,
							IndicatorPixelShaderColor,
							Monochrome
						>::type;

						using BorderPixelShader = Framework::Shader::Pixel::TemplateShader<
							dimension_t>;

						using TrackPixelShader = Framework::Shader::Pixel::TemplateShader<
							dimension_t
							, Framework::Shader::Source::StaticColor<dimension_t>
							, Framework::Shader::Color::NoShader<dimension_t>
							, Framework::Assets::Shader::Transform::CheckerboardMask<dimension_t, true>
						>;

						using DemoStyle = TemplateLinearProgressStyle<OrientationEnum::LeftToRight, 1, 1, CornerRadius()>;
					};

					template<typename ParentLayout, bool Monochrome = true>
					class DemoView : public View<typename Style<ParentLayout>::dimension_t, ParentLayout,
						typename Style<ParentLayout>::DemoStyle,
						typename Style<ParentLayout>::template IndicatorPixelShader<Monochrome>,
						typename Style<ParentLayout>::BorderPixelShader,
						typename Style<ParentLayout>::TrackPixelShader
					>
					{
					private:
						using dimension_t = typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t;

						using Base = View <
							dimension_t,
							ParentLayout,
							typename Style<ParentLayout>::DemoStyle,
							typename Style<ParentLayout>::template IndicatorPixelShader<Monochrome>,
							typename Style<ParentLayout>::BorderPixelShader,
							typename Style<ParentLayout>::TrackPixelShader
						>;

						static constexpr uint32_t CyclePeriod = 4100000;
						static constexpr uint32_t HalfPeriod = CyclePeriod / 2;

					public:
						DemoView() : Base() {}
						~DemoView() = default;

						static constexpr auto progress1percent = UFraction16::GetScalar<int8_t>(1, 100);
						static constexpr auto progress99percent = UFraction16::GetScalar<int8_t>(99, 100);

					protected:
						bool ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
						{
							// Use a triangle wave to animate the progress indicator back and forth.
							const auto triangleElapsed = Framework::Animation::TimeCurve::Triangle<CyclePeriod>(frameTime);

							// Set the progress of progress indicator in ufraction16 format (0 to Fraction16::Max).
							const auto triangleProgress = UFraction16::GetScalar<uint32_t>(triangleElapsed, CyclePeriod);

							if (triangleProgress <= progress1percent)
								Base::SetProgress(0);
							else if (triangleProgress > progress99percent)
								Base::SetProgress(UFraction16::FRACTION_1X);
							else
								Base::SetProgress(triangleProgress);

							return true;
						}
					};
				}
			}
		}
	}
}

#endif
