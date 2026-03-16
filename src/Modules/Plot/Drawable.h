#ifndef _EGFX_MODULES_PLOT_DRAWABLE_h
#define _EGFX_MODULES_PLOT_DRAWABLE_h

#include "Constant.h"
#include "Layout.h"
#include "Shader.h"
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
					typename PlotStyle = TemplatePlotStyle<>,
					typename PixelShaderType = Shader::Pixel::PlotSeries<dimension_t>
				>
				class SeriesHorizontal : public Framework::Shader::Geometry::TriangleShader<dimension_t, PixelShaderType, false>
				{
				private:
					using Base = Framework::Shader::Geometry::TriangleShader<dimension_t, PixelShaderType, false>;
					using SeriesLayout = Layout::SeriesHorizontal<ParentLayout, PlotStyle>;

					using FlipTag = typename TypeTraits::TypeConditional::conditional_type<
						TypeTraits::TypeDispatch::TrueType,
						TypeTraits::TypeDispatch::FalseType,
						PlotStyle::FlipOrientation
					>::type;

					using FillTag = typename TypeTraits::TypeConditional::conditional_type<
						TypeTraits::TypeDispatch::TrueType,
						TypeTraits::TypeDispatch::FalseType,
						(PlotStyle::PlotType == PlotTypeEnum::FillLow)
						|| (PlotStyle::PlotType == PlotTypeEnum::FillCenter)
						|| (PlotStyle::PlotType == PlotTypeEnum::FillHigh)
					>::type;

					using StepTag = typename TypeTraits::TypeConditional::conditional_type<
						TypeTraits::TypeDispatch::TrueType,
						TypeTraits::TypeDispatch::FalseType,
						PlotStyle::PlotType == PlotTypeEnum::Step
					>::type;

					using FillHighTag = typename TypeTraits::TypeConditional::conditional_type<
						TypeTraits::TypeDispatch::TrueType,
						TypeTraits::TypeDispatch::FalseType,
						PlotStyle::PlotType == PlotTypeEnum::FillHigh
					>::type;

					using FillCenterTag = typename TypeTraits::TypeConditional::conditional_type<
						TypeTraits::TypeDispatch::TrueType,
						TypeTraits::TypeDispatch::FalseType,
						PlotStyle::PlotType == PlotTypeEnum::FillCenter
					>::type;

				public:
					static constexpr count_t SampleCount = PlotStyle::SampleCount;
					static_assert(SampleCount >= 2, "Plot::Drawable::SeriesHorizontal requires PlotStyle::SampleCount >= 2.");

				public:
					Data::CircularBuffer<dimension_t, SampleCount> Samples{};

				private:
					int16_t RangeMin = 0;
					int16_t RangeMax = UINT8_MAX;

				public:
					SeriesHorizontal() : Base()
					{
						// Origin is applied by TemplateShader via Prepare() and added on framebuffer writes.
						Base::Prepare(ParentLayout::X(), ParentLayout::Y());
					}
					~SeriesHorizontal() = default;


					void SetRange(const int16_t minValue, const int16_t maxValue)
					{
						if (maxValue == minValue)
						{
							// Expand by 1 on both sides when possible (avoid int16 wrap).
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

					void SetAll(const int16_t value)
					{
						if (ParentLayout::Width() <= 1 || ParentLayout::Height() <= 1)
							return;

						const int16_t h = ParentLayout::Height();
						const int16_t bottom = static_cast<int16_t>(h - 1);
						const int16_t range = MaxValue<int16_t>(1, static_cast<int16_t>(RangeMax - RangeMin));

						const int16_t clamped = LimitValue<int16_t>(value, RangeMin, RangeMax);
						const int32_t s = static_cast<int32_t>(clamped - RangeMin);
						const int32_t scaled = (s * static_cast<int32_t>(h - 1)) / range;

						// Map RangeMin -> bottom, RangeMax -> top (typical plot orientation).
						const dimension_t pixelY = static_cast<dimension_t>(static_cast<int32_t>(bottom) - scaled);

						for (count_t i = 0; i < SampleCount; i++)
						{
							Samples.Push(pixelY);
						}
					}

					void Push(const int16_t newValue)
					{
						if (ParentLayout::Width() <= 1 || ParentLayout::Height() <= 1)
							return;

						const int16_t h = ParentLayout::Height();
						const int16_t bottom = static_cast<int16_t>(h - 1);
						const int16_t range = MaxValue<int16_t>(1, static_cast<int16_t>(RangeMax - RangeMin));

						const int16_t clamped = LimitValue<int16_t>(newValue, RangeMin, RangeMax);
						const int32_t s = static_cast<int32_t>(clamped - RangeMin);
						const int32_t scaled = (s * static_cast<int32_t>(h - 1)) / range;

						const dimension_t pixelY = static_cast<dimension_t>(static_cast<int32_t>(bottom) - scaled);
						Samples.Push(pixelY);
					}

					void Draw(IFrameBuffer* frame)
					{
						Draw(frame, StepTag{});
					}

				private:
					inline void Draw(IFrameBuffer* frame, TypeTraits::TypeDispatch::FalseType)
					{
						// Use width-1 so the last point lands inside the drawable region.

						for (count_t i = 0; i < static_cast<count_t>(SampleCount - 1); i++)
						{
                            const dimension_t x1 = SeriesLayout::template GetSampleX<dimension_t>(i);
							const dimension_t x2 = SeriesLayout::template GetSampleX<dimension_t>(static_cast<count_t>(i + 1));

							DrawSegment(frame, x1, x2, i, FillTag{}, FlipTag{});
						}
					}

					inline void Draw(IFrameBuffer* frame, TypeTraits::TypeDispatch::TrueType)
					{
						for (count_t i = 0; i < SampleCount; i++)
						{
                            const uint32_t xStart = SeriesLayout::template GetStepStartX<dimension_t>(i);
							const uint32_t xEndExclusive = SeriesLayout::template GetStepEndXExclusive<dimension_t>(i);

							// Draw only spans that own at least one pixel.
							if (xStart < xEndExclusive)
							{
								DrawStepSegment(frame,
									static_cast<dimension_t>(xStart),
									static_cast<dimension_t>(xEndExclusive - 1u),
									i,
									FlipTag{});
							}
						}
					}

					void FillToBottom(IFrameBuffer* frame,
						const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2,
						const dimension_t bottom)
					{
						const dimension_t xMin = MinValue(x1, x2);
						const dimension_t xMax = MaxValue(x1, x2);

						// Avoid underflow when xMax == xMin (can happen when SampleCount is high vs width).
						const dimension_t xRectEnd = (xMax > xMin) ? static_cast<dimension_t>(xMax - 1) : xMax;

						const dimension_t yHigh = MaxValue(y1, y2);
						Base::RectangleFill(frame, xMin, yHigh, xRectEnd, bottom);

						// Corner must be at X of the higher endpoint and Y of the lower endpoint.
						const bool firstIsHigher = (y1 < y2); // smaller y is higher on screen
						const dimension_t cx = firstIsHigher ? x1 : x2;
						const dimension_t cy = firstIsHigher ? y2 : y1;

						Base::TriangleFill(frame,
							x1, y1,
							x2, y2,
							cx, cy);
					}

					void FillToTop(IFrameBuffer* frame,
						const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2,
						const dimension_t top)
					{
						const dimension_t xMin = MinValue(x1, x2);
						const dimension_t xMax = MaxValue(x1, x2);

						// Avoid underflow when xMax == xMin (can happen when SampleCount is high vs width).
						const dimension_t xRectEnd = (xMax > xMin) ? static_cast<dimension_t>(xMax - 1) : xMax;

						const dimension_t yLow = MinValue(y1, y2);
						Base::RectangleFill(frame, xMin, top, xRectEnd, yLow);

						// For FillToTop the corner choice is mirrored vs FillToBottom.
						const bool firstIsHigher = (y1 < y2);
						const dimension_t cx = firstIsHigher ? x2 : x1;
						const dimension_t cy = firstIsHigher ? y1 : y2;

						Base::TriangleFill(frame,
							x1, y1,
							x2, y2,
							cx, cy);
					}

					inline void DrawSegmentFill(IFrameBuffer* frame,
						const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2,
						TypeTraits::TypeDispatch::FalseType /*FillHighTag*/,
						TypeTraits::TypeDispatch::FalseType /*FillCenterTag*/)
					{
						// FillLow: fill towards the bottom.
                        const dimension_t bottom = SeriesLayout::template BottomBaseline<dimension_t>();
						FillToBottom(frame, x1, y1, x2, y2, bottom);
					}

					void DrawSegmentFill(IFrameBuffer* frame,
						const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2,
						TypeTraits::TypeDispatch::TrueType /*FillHighTag*/,
						TypeTraits::TypeDispatch::FalseType /*FillCenterTag*/)
					{
						// FillHigh: fill towards the top.
                      const dimension_t top = SeriesLayout::template TopBaseline<dimension_t>();
						FillToTop(frame, x1, y1, x2, y2, top);
					}

					void DrawSegmentFill(IFrameBuffer* frame,
						const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2,
						TypeTraits::TypeDispatch::FalseType /*FillHighTag*/,
						TypeTraits::TypeDispatch::TrueType /*FillCenterTag*/)
					{
						// FillCenter: fill towards the vertical center line of the plot.
                       const dimension_t yBase = SeriesLayout::template CenterBaseline<dimension_t>();

						// Entirely on/above center -> fill down to center.
						if (y1 <= yBase && y2 <= yBase)
						{
							FillToBottom(frame, x1, y1, x2, y2, yBase);
							return;
						}

						// Entirely on/below center -> fill up to center.
						if (y1 >= yBase && y2 >= yBase)
						{
							FillToTop(frame, x1, y1, x2, y2, yBase);
							return;
						}

						// Crosses center: split at intersection with y = yBase and fill each half towards center.
						const int32_t ix1 = static_cast<int32_t>(x1);
						const int32_t ix2 = static_cast<int32_t>(x2);
						const int32_t iy1 = static_cast<int32_t>(y1);
						const int32_t iy2 = static_cast<int32_t>(y2);
						const int32_t iyb = static_cast<int32_t>(yBase);

						const int32_t dy = (iy2 - iy1);
						if (dy == 0)
						{
							// Shouldn't happen in the crossing case, but keep it safe.
							if (y1 < yBase)
								FillToBottom(frame, x1, y1, x2, y2, yBase);
							else
								FillToTop(frame, x1, y1, x2, y2, yBase);
							return;
						}

						const int32_t dx = (ix2 - ix1);
						const int32_t tNum = (iyb - iy1);
						const int32_t xCross32 = ix1 + (dx * tNum) / dy;
						const dimension_t xCross = static_cast<dimension_t>(xCross32);

						// First half
						if (y1 < yBase)
							FillToBottom(frame, x1, y1, xCross, yBase, yBase);
						else
							FillToTop(frame, x1, y1, xCross, yBase, yBase);

						// Second half
						if (y2 < yBase)
							FillToBottom(frame, xCross, yBase, x2, y2, yBase);
						else
							FillToTop(frame, xCross, yBase, x2, y2, yBase);
					}

					// (Unreachable for valid PlotTypeEnum, but keeps overload resolution well-formed.)
					inline void DrawSegmentFill(IFrameBuffer* frame,
						const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2,
						TypeTraits::TypeDispatch::TrueType /*FillHighTag*/,
						TypeTraits::TypeDispatch::TrueType /*FillCenterTag*/)
					{
						DrawSegmentFill(frame, x1, y1, x2, y2,
							TypeTraits::TypeDispatch::FalseType{}, TypeTraits::TypeDispatch::TrueType{});
					}

					inline void DrawSegment(IFrameBuffer* frame,
						const dimension_t x1, const dimension_t x2, const count_t sampleIndex,
						TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::FalseType)
					{
						const dimension_t y1 = Samples.Get(sampleIndex);
						const dimension_t y2 = Samples.Get(sampleIndex + 1);
						DrawSegmentFill(frame, x1, y1, x2, y2, FillHighTag{}, FillCenterTag{});
					}

					inline void DrawSegment(IFrameBuffer* frame,
						const dimension_t x1, const dimension_t x2, const count_t sampleIndex,
						TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::TrueType)
					{
						const dimension_t y1 = Samples.Get(sampleIndex);
						const dimension_t y2 = Samples.Get(sampleIndex + 1);

						const dimension_t widthMinus1 = static_cast<dimension_t>(ParentLayout::Width() - 1);
						const dimension_t fx1 = static_cast<dimension_t>(widthMinus1 - x1);
						const dimension_t fx2 = static_cast<dimension_t>(widthMinus1 - x2);

						DrawSegmentFill(frame, fx1, y1, fx2, y2, FillHighTag{}, FillCenterTag{});
					}

					inline void DrawSegment(IFrameBuffer* frame,
						const dimension_t x1, const dimension_t x2, const count_t sampleIndex,
						TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::TrueType)
					{
						Base::Line(frame,
							ParentLayout::Width() - 1 - x1, Samples.Get(sampleIndex),
							ParentLayout::Width() - 1 - x2, Samples.Get(sampleIndex + 1));
					}

					inline void DrawSegment(IFrameBuffer* frame,
						const dimension_t x1, const dimension_t x2, const count_t sampleIndex,
						TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::FalseType)
					{
						Base::Line(frame,
							x1, Samples.Get(sampleIndex),
							x2, Samples.Get(sampleIndex + 1));
					}

					inline void DrawStepSegment(IFrameBuffer* frame,
						const dimension_t x1, const dimension_t x2, const count_t sampleIndex,
						TypeTraits::TypeDispatch::FalseType)
					{
						const dimension_t y = Samples.Get(sampleIndex);
						Base::Line(frame, x1, y, x2 - 1, y);
					}

					inline void DrawStepSegment(IFrameBuffer* frame,
						const dimension_t x1, const dimension_t x2, const count_t sampleIndex,
						TypeTraits::TypeDispatch::TrueType)
					{
						const dimension_t y = Samples.Get(sampleIndex);
						const dimension_t widthMinus1 = static_cast<dimension_t>(ParentLayout::Width() - 1);

						Base::Line(frame,
							static_cast<dimension_t>(widthMinus1 - x2 + 1), y,
							static_cast<dimension_t>(widthMinus1 - x1), y);
					}
				};
			}
		}
	}
}
#endif