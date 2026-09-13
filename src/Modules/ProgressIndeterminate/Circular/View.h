#ifndef _EGFX_MODULES_PROGRESS_INDETERMINATE_CIRCULAR_VIEW_h
#define _EGFX_MODULES_PROGRESS_INDETERMINATE_CIRCULAR_VIEW_h

#include "Constant.h"
#include "Drawable.h"
#include <EgfxAssets.h>

namespace Egfx
{
	namespace Modules
	{
		namespace ProgressIndeterminate
		{
			namespace Circular
			{
				template<typename dimension_t,
					typename ParentLayout,
					dimension_t ArcThickness,
					typename IndicatorPixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>,
					typename TrackPixelShaderType = Framework::Shader::Pixel::MockShader<dimension_t>
				>
				class View : public Framework::View::DrawablesView<
					Drawable::Indicator<dimension_t, ParentLayout, ArcThickness, IndicatorPixelShaderType>,
					Drawable::Track<dimension_t, ParentLayout, ArcThickness, TrackPixelShaderType>
				>
				{
				private:
					using Base = Framework::View::DrawablesView <
						Drawable::Indicator<dimension_t, ParentLayout, ArcThickness, IndicatorPixelShaderType>,
						Drawable::Track<dimension_t, ParentLayout, ArcThickness, TrackPixelShaderType>
					>;

					struct AnglePair
					{
						angle_t StartAngle;
						angle_t EndAngle;
					};

				private:
					static constexpr bool HasTrack = !TypeTraits::TypeDispatch::is_same<TrackPixelShaderType, Framework::Shader::Pixel::MockShader<dimension_t>>::value;
					static constexpr angle_t SweepDeadBand = GetAngle(25);
					static constexpr angle_t SweepMin = GetAngle(120);
					static constexpr angle_t SweepVariance = (SweepMin - SweepDeadBand) / 4;

				private:
					uint32_t AnimationPeriod = 3666000;
					AnimationModeEnum AnimationMode = AnimationModeEnum::Outrun;

				public:
					View() : Base() {}
					~View() = default;

					void SetBounds(const dimension_t left, const dimension_t top,
						const dimension_t right, const dimension_t bottom)
					{
						Base::template drawable<0>().SetBounds(left, top, right, bottom);
						Base::template drawable<1>().SetBounds(left, top, right, bottom);
					}

					void SetTranslation(const int16_t x, const int16_t y)
					{
						Base::template drawable<0>().SetTranslation(x, y);
						Base::template drawable<1>().SetTranslation(x, y);
					}

					void SetAnimationPeriod(const uint32_t period)
					{
						AnimationPeriod = period;
					}

					void SetAnimationMode(const AnimationModeEnum mode)
					{
						AnimationMode = mode;
					}

				protected:
					bool ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
					{
						AnglePair angles;

						switch (AnimationMode)
						{
						case AnimationModeEnum::Outrun:
							angles = StepOutrun(frameTime);
							break;
						case AnimationModeEnum::BottomBounce:
							angles = StepBottomBounce(frameTime);
							break;
						case AnimationModeEnum::GapRotate:
							angles = StepGapRotate(frameTime);
							break;
						case AnimationModeEnum::ChaseBounce:
						default:
							angles = StepChaseBounce(frameTime);
							break;
						}

						auto& indicator = Base::template drawable<0>();
						indicator.SetFilledAngles(angles.StartAngle, angles.EndAngle);

						if (HasTrack)
						{
							auto& track = Base::template drawable<1>();
							track.SetFilledAngles(angles.StartAngle, angles.EndAngle);
						}

						return true;
					}

				private:
					AnglePair StepChaseBounce(const uint32_t frameTime)
					{
						const uint32_t animationPeriod2 = AnimationPeriod / 3;
						const uint32_t animationPeriod3 = AnimationPeriod / 2;

						const auto progress1 = UFraction16::GetScalar<uint32_t>(frameTime % AnimationPeriod, AnimationPeriod);

						const auto triangleElapsed1 = Framework::Animation::TimeCurve::Triangle(frameTime, animationPeriod2);
						const auto progress2 = UFraction16::GetScalar<uint32_t>(triangleElapsed1, animationPeriod2);

						const auto triangleElapsed2 = Framework::Animation::TimeCurve::Triangle(frameTime, animationPeriod3);
						const auto progress3 = UFraction16::GetScalar<uint32_t>(triangleElapsed2, animationPeriod3);

						const angle_t middleAngle = UFraction16::Fraction<angle_t>(progress1, ANGLE_RANGE);
						const angle_t startAngle = middleAngle + UFraction16::Fraction<angle_t>(progress2, SweepVariance);
						const angle_t endAngle = static_cast<angle_t>(middleAngle + SweepMin + UFraction16::Fraction<angle_t>(progress3, ANGLE_RANGE - SweepMin - SweepDeadBand));

						return { startAngle, endAngle };
					}

					AnglePair StepOutrun(const uint32_t frameTime)
					{
						const uint32_t animationPeriod2 = AnimationPeriod / 5;

						const auto progress1 = UFraction16::GetScalar<uint32_t>(frameTime % animationPeriod2, animationPeriod2);
						const auto triangleElapsed2 = Framework::Animation::TimeCurve::Triangle(frameTime, AnimationPeriod);
						const auto progress2 = UFraction16::GetScalar<uint32_t>(triangleElapsed2, AnimationPeriod);

						const angle_t startAngle = UFraction16::Fraction<angle_t>(progress1, ANGLE_RANGE);
						const angle_t endAngle = startAngle + UFraction16::Fraction<angle_t>(progress2, ANGLE_RANGE - SweepDeadBand);

						return { static_cast<angle_t>(startAngle), static_cast<angle_t>(endAngle) };
					}

					AnglePair StepBottomBounce(const uint32_t frameTime)
					{
						const uint32_t animationPeriod = AnimationPeriod / 2;
						const auto triangleElapsed = Framework::Animation::TimeCurve::Triangle(frameTime, animationPeriod);
						const auto progress = UFraction16::GetScalar<uint32_t>(triangleElapsed, animationPeriod);

						const angle_t startAngle = UFraction16::Fraction<angle_t>(progress, ANGLE_HALF_CIRCLE - 1);
						const angle_t endAngle = ANGLE_RANGE - startAngle;

						return { startAngle, endAngle };
					}

					AnglePair StepGapRotate(const uint32_t frameTime)
					{
						const uint32_t animationPeriod = AnimationPeriod / 2;
						const auto progress = UFraction16::GetScalar<uint32_t>(frameTime % animationPeriod, animationPeriod);
						const angle_t startAngle = UFraction16::Fraction<angle_t>(progress, ANGLE_RANGE);
						const angle_t endAngle = startAngle + (ANGLE_RANGE - (SweepMin / 2));
						return { startAngle, endAngle };
					}
				};

				namespace Demo
				{
					template<typename ParentLayout>
					struct Style
					{
						using dimension_t = typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t;

						static constexpr dimension_t Size()
						{
							return MinValue<pixel_t>(ParentLayout::Width(), ParentLayout::Height());
						}

						static constexpr dimension_t ArcThickness()
						{
							return MaxValue<pixel_t>(Size() / 10, 1);
						}
					};

					template<typename ParentLayout, bool Monochrome = false>
					using DemoView = View<
						typename Style<ParentLayout>::dimension_t,
						ParentLayout,
						Style<ParentLayout>::ArcThickness()
					>;
				}
			}
		}
	}
}
#endif