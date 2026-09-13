#ifndef _EGFX_MODULES_PLOT_DRAWABLE_BAR_h
#define _EGFX_MODULES_PLOT_DRAWABLE_BAR_h

#include "DrawableBase.h"

namespace Egfx
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
				class Bar : public PlotDrawableBase<dimension_t, ParentLayout, PlotStyle, ColorShaderType, TransformShaderType, BlendMode, PlotValueSourceType>
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
							DrawBar(frame, static_cast<dimension_t>(xStart), xEnd, y);
						}
					}

				private:
					void DrawBar(IFrameBuffer* frame, const dimension_t x1, const dimension_t x2, const dimension_t y)
					{
						if (PlotStyle::PlotType == PlotTypeEnum::BarHigh)
						{
							const auto p1 = Base::ToPhysical(x1, Base::SeriesLayout::template TopBaseline<dimension_t>());
							const auto p2 = Base::ToPhysical(x2, y);
							Base::RectangleFill(frame, MinValue(p1.x, p2.x), MinValue(p1.y, p2.y), MaxValue(p1.x, p2.x), MaxValue(p1.y, p2.y));
						}
						else if (PlotStyle::PlotType == PlotTypeEnum::BarCenter)
						{
							const dimension_t center = Base::SeriesLayout::template CenterBaseline<dimension_t>();
							const auto p1 = Base::ToPhysical(x1, MinValue(y, center));
							const auto p2 = Base::ToPhysical(x2, MaxValue(y, center));
							Base::RectangleFill(frame, MinValue(p1.x, p2.x), MinValue(p1.y, p2.y), MaxValue(p1.x, p2.x), MaxValue(p1.y, p2.y));
						}
						else
						{
							const auto p1 = Base::ToPhysical(x1, y);
							const auto p2 = Base::ToPhysical(x2, Base::SeriesLayout::template BottomBaseline<dimension_t>());
							Base::RectangleFill(frame, MinValue(p1.x, p2.x), MinValue(p1.y, p2.y), MaxValue(p1.x, p2.x), MaxValue(p1.y, p2.y));
						}
					}
				};
			}
		}
	}
}
#endif
