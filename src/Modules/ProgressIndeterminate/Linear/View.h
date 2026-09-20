#ifndef _INTEGERGLASS_MODULES_PROGRESS_INDETERMINATE_LINEAR_VIEW_h
#define _INTEGERGLASS_MODULES_PROGRESS_INDETERMINATE_LINEAR_VIEW_h

#include "Constant.h"
#include "Drawable.h"
#include <Modules/Kit/Include.h>

namespace IntegerGlass
{
	namespace Modules
	{
		namespace ProgressIndeterminate
		{
			namespace Linear
			{
				template<typename dimension_t,
					typename ParentLayout,
					typename IndicatorPixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>,
					typename TrackPixelShaderType = Framework::Shader::Pixel::MockShader<dimension_t>
				>
				class View : public Framework::View::DrawablesView<ParentLayout,
					Drawable::IndicatorRectangle<dimension_t, ParentLayout, IndicatorPixelShaderType>,
					Drawable::Track<dimension_t, ParentLayout, TrackPixelShaderType>
				>
				{
				private:
					using Base = Framework::View::DrawablesView<ParentLayout,
						Drawable::IndicatorRectangle<dimension_t, ParentLayout, IndicatorPixelShaderType>,
						Drawable::Track<dimension_t, ParentLayout, TrackPixelShaderType>
					>;
					using LayoutCalc = typename Layout::Calc<dimension_t, ParentLayout>;

					//using SegmentPair = typename SegmentPair<dimension_t>;

				private:
					static constexpr bool HasTrack = !TypeTraits::TypeDispatch::is_same<TrackPixelShaderType, Framework::Shader::Pixel::MockShader<dimension_t>>::value;

				private:
					uint32_t AnimationPeriod = 3666000;
					AnimationModeEnum AnimationMode = AnimationModeEnum::Progressive;

				public:
					View() : Base() {}
					~View() = default;

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
						SegmentPair<dimension_t> segments;

						switch (AnimationMode)
						{
						case AnimationModeEnum::Test1:
							segments = StepMaterialDesign(frameTime);
							break;
						case AnimationModeEnum::Progressive:
							segments = StepProgressive(frameTime);
							break;
						case AnimationModeEnum::Symmetric:
							segments = StepSymmetric(frameTime);
							break;
						case AnimationModeEnum::PingPong:
							segments = StepPingPong(frameTime);
						default:
							break;
						}

						segments.Coalesce();

						auto& indicator = Base::template drawable<0>();
						indicator.SetSegments(segments);


						if (HasTrack)
						{
							auto& track = Base::template drawable<1>();
							track.SetSegments(segments);
						}

						return true;
					}

				private:
					SegmentPair<dimension_t> StepPingPong(const uint32_t frameTime)
					{
						const uint32_t animationPeriod = AnimationPeriod / 2;
						const auto triangleElapsed = Framework::Animation::TimeCurve::Triangle(frameTime, animationPeriod);
						const auto progress = UFraction16::GetScalar<uint32_t>(triangleElapsed, animationPeriod);
						static constexpr dimension_t length = (int32_t(LayoutCalc::RangeLength()) * 7) / 10;
						const int32_t shift = UFraction16::Fraction<int32_t>(progress, LayoutCalc::RangeLength() + length);
						const dimension_t end = MinValue<int32_t>(LayoutCalc::RangeLength(), shift);
						const dimension_t start = MaxValue<int32_t>(0, int32_t(shift) - length);

						return { start, end, 0, 0 };
					}

					SegmentPair<dimension_t> StepMaterialDesign(const uint32_t frameTime)
					{
						const uint32_t period = AnimationPeriod / 4;
						const auto progress = UFraction16::GetScalar<uint32_t>(frameTime % period, period);

						const int32_t totalLength = static_cast<int32_t>(LayoutCalc::RangeLength());

						// Segment 1: Fast initial stretch and slide
						const int32_t len1 = (totalLength * (progress < 32768 ? (progress * 2) : (65535 - progress) * 2)) / 65535;
						const int32_t shift1 = (totalLength * static_cast<int32_t>(progress)) / 65535;
						const dimension_t start1 = MaxValue<int32_t>(0, shift1 - len1);
						const dimension_t end1 = MinValue<int32_t>(totalLength, shift1);

						// Segment 2: Delayed secondary chase segment
						const uint32_t progress2 = (progress + 32768) % 65535;
						const int32_t len2 = (totalLength * 3) / 10;
						const int32_t shift2 = (totalLength * static_cast<int32_t>(progress2)) / 65535;
						const dimension_t start2 = MaxValue<int32_t>(0, shift2 - len2);
						const dimension_t end2 = MinValue<int32_t>(totalLength, shift2);

						return { start1, end1, start2, end2 };
					}

					SegmentPair<dimension_t> StepSymmetric(const uint32_t frameTime)
					{
						const uint32_t animationPeriod = AnimationPeriod / 4;
						const auto triangleElapsed = Framework::Animation::TimeCurve::Triangle(frameTime, animationPeriod);
						const auto progress = UFraction16::GetScalar<uint32_t>(triangleElapsed, animationPeriod);

						const int32_t totalLength = static_cast<int32_t>(LayoutCalc::RangeLength());
						const int32_t baseSize = totalLength / 4;

						const int32_t centerOffset = (totalLength * static_cast<int32_t>(progress)) / 65535;

						const dimension_t start1 = MaxValue<int32_t>(0, centerOffset - baseSize);
						const dimension_t end1 = MinValue<int32_t>(totalLength, centerOffset + baseSize);

						const dimension_t start2 = MaxValue<int32_t>(0, totalLength - end1);
						const dimension_t end2 = MinValue<int32_t>(totalLength, totalLength - start1);

						return { start1, end1, start2, end2 };
					}

					SegmentPair<dimension_t> StepProgressive(const uint32_t frameTime)
					{
						const uint32_t animationPeriod1 = AnimationPeriod / 4;
						const uint32_t animationPeriod2 = AnimationPeriod;
						const auto progress1 = UFraction16::GetScalar<uint32_t>(frameTime % animationPeriod1, animationPeriod1);
						const auto progress2 = UFraction16::GetScalar<uint32_t>((frameTime + (AnimationPeriod / 2)) % animationPeriod2, animationPeriod2);

						static constexpr dimension_t length = (int32_t(LayoutCalc::RangeLength()) * 7) / 10;
						const int32_t totalRange = static_cast<int32_t>(LayoutCalc::RangeLength()) + length;

						// Segment 1 (fast cycle)
						const int32_t shift1 = UFraction16::Fraction<int32_t>(progress1, totalRange);
						const dimension_t end1 = MinValue<int32_t>(LayoutCalc::RangeLength(), shift1);
						const dimension_t start1 = MaxValue<int32_t>(0, shift1 - length);

						// Segment 2 (long cycle with phase offset)
						const int32_t shift2 = UFraction16::Fraction<int32_t>(progress2, totalRange);
						const dimension_t end2 = MinValue<int32_t>(LayoutCalc::RangeLength(), shift2);
						const dimension_t start2 = MaxValue<int32_t>(0, shift2 - length);

						return { start1, end1, start2, end2 };
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

						using IndicatorPixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>;
					};

					template<typename ParentLayout, bool Monochrome = false>
					using DemoView = View<
						typename Style<ParentLayout>::dimension_t,
						ParentLayout,
						typename Style<ParentLayout>::IndicatorPixelShaderType
					>;
				}
			}
		}
	}
}
#endif