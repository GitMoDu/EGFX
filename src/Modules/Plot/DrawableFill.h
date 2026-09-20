#ifndef _INTEGERGLASS_MODULES_PLOT_DRAWABLE_FILL_h
#define _INTEGERGLASS_MODULES_PLOT_DRAWABLE_FILL_h

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
				class Fill : public PlotDrawableBase<dimension_t, ParentLayout, PlotStyle, ColorShaderType, TransformShaderType, BlendMode, PlotValueSourceType>
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
							DrawSegment(frame, x1, Base::Sample(i), x2, Base::Sample(static_cast<count_t>(i + 1)));
						}
					}

				private:
					void FillToBottom(IFrameBuffer* frame, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2, const dimension_t bottom)
					{
						const dimension_t xMin = MinValue(x1, x2);
						const dimension_t xMax = MaxValue(x1, x2);
						if (xMin == xMax)
						{
							const auto p1 = Base::ToPhysical(xMin, MaxValue(y1, y2));
							const auto p2 = Base::ToPhysical(xMin, bottom);
							Base::Line(frame, p1.x, p1.y, p2.x, p2.y);
							return;
						}

						const dimension_t yHigh = MaxValue(y1, y2);
						const auto rectangleFirst = Base::ToPhysical(xMin, yHigh);
						const auto rectangleLast = Base::ToPhysical(xMax, bottom);
						Base::RectangleFill(frame, MinValue(rectangleFirst.x, rectangleLast.x), MinValue(rectangleFirst.y, rectangleLast.y),
							MaxValue(rectangleFirst.x, rectangleLast.x), MaxValue(rectangleFirst.y, rectangleLast.y));

						const bool firstIsHigher = y1 < y2;
						const auto p1 = Base::ToPhysical(x1, y1);
						const auto p2 = Base::ToPhysical(x2, y2);
						const auto p3 = Base::ToPhysical(firstIsHigher ? x1 : x2, firstIsHigher ? y2 : y1);
						Base::TriangleFill(frame, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
					}

					void FillToTop(IFrameBuffer* frame, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2, const dimension_t top)
					{
						const dimension_t xMin = MinValue(x1, x2);
						const dimension_t xMax = MaxValue(x1, x2);
						if (xMin == xMax)
						{
							const auto p1 = Base::ToPhysical(xMin, top);
							const auto p2 = Base::ToPhysical(xMin, MinValue(y1, y2));
							Base::Line(frame, p1.x, p1.y, p2.x, p2.y);
							return;
						}

						const dimension_t yLow = MinValue(y1, y2);
						const auto rectangleFirst = Base::ToPhysical(xMin, top);
						const auto rectangleLast = Base::ToPhysical(xMax, yLow);
						Base::RectangleFill(frame, MinValue(rectangleFirst.x, rectangleLast.x), MinValue(rectangleFirst.y, rectangleLast.y),
							MaxValue(rectangleFirst.x, rectangleLast.x), MaxValue(rectangleFirst.y, rectangleLast.y));

						const bool firstIsHigher = y1 < y2;
						const auto p1 = Base::ToPhysical(x1, y1);
						const auto p2 = Base::ToPhysical(x2, y2);
						const auto p3 = Base::ToPhysical(firstIsHigher ? x2 : x1, firstIsHigher ? y1 : y2);
						Base::TriangleFill(frame, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
					}

					void DrawSegment(IFrameBuffer* frame, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2)
					{
						if (PlotStyle::PlotType == PlotTypeEnum::FillHigh)
						{
							FillToTop(frame, x1, y1, x2, y2, Base::SeriesLayout::template TopBaseline<dimension_t>());
							return;
						}

						if (PlotStyle::PlotType == PlotTypeEnum::FillLow)
						{
							FillToBottom(frame, x1, y1, x2, y2, Base::SeriesLayout::template BottomBaseline<dimension_t>());
							return;
						}

						const dimension_t center = Base::SeriesLayout::template CenterBaseline<dimension_t>();
						if (y1 <= center && y2 <= center)
						{
							FillToBottom(frame, x1, y1, x2, y2, center);
						}
						else if (y1 >= center && y2 >= center)
						{
							FillToTop(frame, x1, y1, x2, y2, center);
						}
						else
						{
							const int32_t dx = static_cast<int32_t>(x2) - static_cast<int32_t>(x1);
							const int32_t dy = static_cast<int32_t>(y2) - static_cast<int32_t>(y1);
							const int32_t xCross = static_cast<int32_t>(x1) + (dx * (static_cast<int32_t>(center) - static_cast<int32_t>(y1))) / dy;

							if (y1 < center)
								FillToBottom(frame, x1, y1, static_cast<dimension_t>(xCross), center, center);
							else
								FillToTop(frame, x1, y1, static_cast<dimension_t>(xCross), center, center);

							if (y2 < center)
								FillToBottom(frame, static_cast<dimension_t>(xCross), center, x2, y2, center);
							else
								FillToTop(frame, static_cast<dimension_t>(xCross), center, x2, y2, center);
						}
					}
				};
			}
		}
	}
}
#endif
