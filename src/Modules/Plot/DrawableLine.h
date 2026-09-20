#ifndef _INTEGERGLASS_MODULES_PLOT_DRAWABLE_LINE_h
#define _INTEGERGLASS_MODULES_PLOT_DRAWABLE_LINE_h

#include "DrawableBase.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Plot
		{
			namespace Drawable
			{
				template<typename dimension_t, typename ParentLayout, typename PlotStyle,
					typename ColorShaderType, typename TransformShaderType,
					Framework::Shader::Pixel::BlendModeEnum BlendMode, typename PlotValueSourceType>
				class Line : public PlotDrawableBase<dimension_t, ParentLayout, PlotStyle, ColorShaderType, TransformShaderType, BlendMode, PlotValueSourceType>
				{
				private:
					using Base = PlotDrawableBase<dimension_t, ParentLayout, PlotStyle, ColorShaderType, TransformShaderType, BlendMode, PlotValueSourceType>;

				public:
					using Base::Base;

					void Draw(IFrameBuffer* frame)
					{
						if (!Base::PrepareDraw())
							return;

						for (count_t i = 0; i < static_cast<count_t>(Base::SampleCount - 1); i++)
						{
							const dimension_t x1 = Base::SeriesLayout::template GetSampleX<dimension_t>(i);
							const dimension_t x2 = Base::SeriesLayout::template GetSampleX<dimension_t>(static_cast<count_t>(i + 1));
							const auto boundaryMode = i == static_cast<count_t>(Base::SampleCount - 2)
								? Framework::Shader::Geometry::LineBoundaryModeEnum::Inclusive
								: Framework::Shader::Geometry::LineBoundaryModeEnum::ExcludeEnd;

							for (count_t offset = 0; offset < Base::LineThickness; offset++)
							{
								const auto p1 = Base::ToPhysical(x1, static_cast<dimension_t>(Base::Sample(i) + offset));
								const auto p2 = Base::ToPhysical(x2, static_cast<dimension_t>(Base::Sample(static_cast<count_t>(i + 1)) + offset));
								Base::Line(frame, p1.x, p1.y, p2.x, p2.y, boundaryMode);
							}
						}
					}
				};

				template<typename dimension_t, typename ParentLayout, typename PlotStyle,
					typename ColorShaderType, typename TransformShaderType,
					Framework::Shader::Pixel::BlendModeEnum BlendMode, typename PlotValueSourceType>
				class Step : public PlotDrawableBase<dimension_t, ParentLayout, PlotStyle, ColorShaderType, TransformShaderType, BlendMode, PlotValueSourceType>
				{
				private:
					using Base = PlotDrawableBase<dimension_t, ParentLayout, PlotStyle, ColorShaderType, TransformShaderType, BlendMode, PlotValueSourceType>;

				public:
					using Base::Base;

					void Draw(IFrameBuffer* frame)
					{
						if (!Base::PrepareDraw())
							return;

						for (count_t i = 0; i < Base::SampleCount; i++)
						{
							const uint32_t xStart = Base::SeriesLayout::template GetSegmentStartX<dimension_t>(i);
							const uint32_t xEndExclusive = Base::SeriesLayout::template GetSegmentEndXExclusive<dimension_t>(i);
							if (xStart >= xEndExclusive)
								continue;

							const dimension_t xEnd = static_cast<dimension_t>(xEndExclusive - 1u);
							const dimension_t y = Base::Sample(i);
							if (Base::LineThickness > 1)
							{
								const auto p1 = Base::ToPhysical(static_cast<dimension_t>(xStart), y);
								const auto p2 = Base::ToPhysical(xEnd, static_cast<dimension_t>(y + Base::LineThickness - 1));
								Base::RectangleFill(frame, MinValue(p1.x, p2.x), MinValue(p1.y, p2.y), MaxValue(p1.x, p2.x), MaxValue(p1.y, p2.y));
							}
							else
							{
								const auto p1 = Base::ToPhysical(static_cast<dimension_t>(xStart), y);
								const auto p2 = Base::ToPhysical(xEnd, y);
								Base::Line(frame, p1.x, p1.y, p2.x, p2.y);
							}
						}
					}
				};
			}
		}
	}
}
#endif
