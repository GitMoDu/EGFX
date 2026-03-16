#ifndef _EGFX_MODULES_PLOT_VIEW_h
#define _EGFX_MODULES_PLOT_VIEW_h

#include "Constant.h"
#include "Layout.h"
#include "Drawable.h"
#include <IntegerTrigonometry8.h>

namespace Egfx
{
	namespace Modules
	{
		namespace Plot
		{
			namespace View
			{

				template<typename dimension_t,
					typename ParentLayout,
					typename PlotStyle = TemplatePlotStyle<>
				>
				class PlotHorizontal : public Framework::View::DrawablesView<
					Drawable::SeriesHorizontal<dimension_t, ParentLayout, PlotStyle>
				>
				{
				private:
					using Base = Framework::View::DrawablesView<
						Drawable::SeriesHorizontal<dimension_t, ParentLayout, PlotStyle>
					>;

				public:
					bool Visible = true;

				public:
					PlotHorizontal() : Base() {}
					~PlotHorizontal() = default;

					void SetRange(const int16_t minValue, const int16_t maxValue)
					{
						Base::template drawable<0>().SetRange(minValue, maxValue);
					}

					void SetAll(const int16_t value)
					{
						Base::template drawable<0>().SetAll(value);
					}

					void Push(const int16_t newValue)
					{
						Base::template drawable<0>().Push(newValue);
					}

					void SetSeriesColor(const rgb_color_t color)
					{
						Base::template drawable<0>().ColorSource.Color = color;
					}

				protected:
					bool ViewStep(const uint32_t /*frameTime*/, const uint16_t /*frameCounter*/) override
					{
						return (ParentLayout::Width() >= 1 || ParentLayout::Height() >= 1) && Visible;
					}
				};


				template<typename ParentLayout,
					bool Monochrome = true,
					count_t sampleCount = 32
				>
				class PlotHorizontalDemoView : public Framework::View::DrawablesView<
					Drawable::SeriesHorizontal<uint8_t, ParentLayout, TemplatePlotStyle<sampleCount, PlotTypeEnum::Step, false>>,
					Drawable::SeriesHorizontal<uint8_t, ParentLayout, TemplatePlotStyle<sampleCount, PlotTypeEnum::FillLow, false>, Shader::Pixel::PlotDither<uint8_t>>
				>
				{
				private:
					using Base = Framework::View::DrawablesView<
						Drawable::SeriesHorizontal<uint8_t, ParentLayout, TemplatePlotStyle<sampleCount, PlotTypeEnum::Step, false>>,
						Drawable::SeriesHorizontal<uint8_t, ParentLayout, TemplatePlotStyle<sampleCount, PlotTypeEnum::FillLow, false>, Shader::Pixel::PlotDither<uint8_t>>
					>;

				private:
					static constexpr uint8_t MID = static_cast<uint8_t>((static_cast<uint16_t>(UINT8_MAX) + 1u) / 2u);

				public:
					PlotHorizontalDemoView() : Base()
					{
						Base::template drawable<0>().SetRange(0, UINT8_MAX);
						Base::template drawable<1>().SetRange(0, UINT8_MAX);
						/*	Base::SetRange(0, UINT8_MAX);
							Base::SetSeriesColor(Rgb::Color(uint32_t(0x00FF00)));*/
					}
					~PlotHorizontalDemoView() = default;

				protected:
					bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
					{
						if (!Base::ViewStep(frameTime, frameCounter))
							return false;

						const uint64_t big = (frameTime * sampleCount);

						const angle_t angle = (big * 3) >> 11;
						fraction8_t sine = Trigonometry::Sine8(angle);
						int16_t signedSample = Fraction<int16_t>(sine, static_cast<int16_t>(MID));
						int16_t biased = static_cast<int16_t>(MID) + signedSample;

						Base::template drawable<0>().Push(LimitValue<int16_t, 0, UINT8_MAX>(biased));

						const angle_t angle2 = big >> 10;
						sine = Trigonometry::Sine8(angle2);
						signedSample = Fraction<int16_t>(sine, static_cast<int16_t>(MID / 2));
						biased = static_cast<int16_t>(MID / 2) + 1 + signedSample;

						Base::template drawable<1>().Push(LimitValue<int16_t, 0, UINT8_MAX>(biased));

						return true;
					}
				};
			}
		}
	}
}
#endif