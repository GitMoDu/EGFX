#ifndef _EGFX_MODULES_PLOT_DRAWABLE_BASE_h
#define _EGFX_MODULES_PLOT_DRAWABLE_BASE_h

#include "Constant.h"
#include "Layout.h"
#include "Data.h"

namespace Egfx
{
	namespace Modules
	{
		namespace Plot
		{
			namespace Drawable
			{
				template<typename dimension_t,
					typename ParentLayout,
					typename PlotStyle,
					typename ColorShaderType,
					typename TransformShaderType,
					Framework::Shader::Pixel::BlendModeEnum BlendMode,
					typename PlotValueSourceType>
				class PlotDrawableBase : public Framework::Shader::Geometry::TriangleShader<dimension_t,
					Shader::Pixel::Line<dimension_t, ColorShaderType, TransformShaderType, BlendMode>, false>
				{
				protected:
					using Base = Framework::Shader::Geometry::TriangleShader<dimension_t,
						Shader::Pixel::Line<dimension_t, ColorShaderType, TransformShaderType, BlendMode>, false>;
					using SeriesLayout = Layout::SeriesHorizontal<ParentLayout, PlotStyle>;
					using LineBoundaryModeEnum = Framework::Shader::Geometry::LineBoundaryModeEnum;

					static constexpr count_t SampleCount = PlotStyle::SampleCount;
					static constexpr count_t LineThickness = PlotStyle::LineThickness;

					PlotValueSourceType* Samples = nullptr;

				private:
					int16_t RangeMin = 0;
					int16_t RangeMax = UINT8_MAX;

				public:
					using DrawableLayout = ParentLayout;

					PlotDrawableBase() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height()) {}
					explicit PlotDrawableBase(PlotValueSourceType& dataSource) : PlotDrawableBase()
					{
						Bind(dataSource);
					}
					~PlotDrawableBase() = default;

					void Bind(PlotValueSourceType& dataSource)
					{
						Samples = &dataSource;
					}

					void SetRange(const int16_t minValue, const int16_t maxValue)
					{
						if (maxValue == minValue)
						{
							RangeMin = (minValue == INT16_MIN) ? INT16_MIN : static_cast<int16_t>(minValue - 1);
							RangeMax = (maxValue == INT16_MAX) ? INT16_MAX : static_cast<int16_t>(maxValue + 1);
						}
						else if (maxValue > minValue)
						{
							RangeMin = minValue;
							RangeMax = maxValue;
						}
						else
						{
							RangeMin = maxValue;
							RangeMax = minValue;
						}
					}

					void SetAll(const typename PlotStyle::value_t value)
					{
						if (Samples == nullptr)
							return;

						for (count_t i = 0; i < SampleCount; i++)
							Samples->Set(i, value);
					}

					void Push(const typename PlotStyle::value_t value)
					{
						if (Samples != nullptr)
							Samples->Push(value);
					}

				protected:
					bool PrepareDraw()
					{
						if (Samples == nullptr)
							return false;

						Base::Prepare(ParentLayout::X(), ParentLayout::Y());
						return true;
					}

					dimension_t Sample(const count_t index) const
					{
						const int16_t height = SeriesLayout::ValueAxisLength();
						const int16_t bottom = EffectiveBottom(height);
						const int16_t range = MaxValue<int16_t>(1, static_cast<int16_t>(RangeMax - RangeMin));
						const int16_t clamped = LimitValue<int16_t>(static_cast<int16_t>(Samples->Get(index)), RangeMin, RangeMax);
						const int32_t scaled = (static_cast<int32_t>(clamped - RangeMin) * bottom) / range;
						return static_cast<dimension_t>(static_cast<int32_t>(bottom) - scaled);
					}

					static int16_t EffectiveBottom(const int16_t height)
					{
						if (PlotStyle::PlotType == PlotTypeEnum::Line || PlotStyle::PlotType == PlotTypeEnum::Step)
						{
							const int16_t strokeThickness = (LineThickness > 0) ? static_cast<int16_t>(LineThickness) : 1;
							return static_cast<int16_t>(height - MinValue<int16_t>(strokeThickness, height));
						}

						return static_cast<int16_t>(height - 1);
					}

					static pixel_point_t ToPhysical(const dimension_t x, const dimension_t y)
					{
						const dimension_t sample = SeriesLayout::IsReversed()
							? static_cast<dimension_t>(SeriesLayout::WidthMinus1() - x)
							: x;

						return SeriesLayout::IsVertical()
							? pixel_point_t{ static_cast<pixel_t>(y), static_cast<pixel_t>(sample) }
						: pixel_point_t{ static_cast<pixel_t>(sample), static_cast<pixel_t>(y) };
					}
				};
			}
		}
	}
}
#endif
