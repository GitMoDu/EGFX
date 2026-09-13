#ifndef _EGFX_FRAMEWORK_SHADER_IMAGE_VECTOR_SHADER_h
#define _EGFX_FRAMEWORK_SHADER_IMAGE_VECTOR_SHADER_h

#include "Model.h"
#include "ColorSource.h"
#include "../../../Vector/Decoder.h"
#include "../../Color/Shaders.h"
#include "../../Transform/Shaders.h"
#include "../../Pixel/Shaders.h"
#include "../../Geometry/CircleArc.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Image
			{
				namespace Vector
				{
					using namespace Framework::Vector;
					using namespace IntegerSignal::FixedPoint::FactorScale;

					template<typename dimension_t,
						typename PaletteType,
						typename ColorShaderType,
						typename TransformShaderType
					>
					class VectorShader : public Geometry::CircleArcShader<dimension_t,
						Geometry::TriangleShader<dimension_t, Shader::Pixel::TemplateShader<dimension_t,
						Vector::ColorSource<dimension_t>, ColorShaderType, TransformShaderType>>>
					{
					private:
						using Base = Geometry::CircleArcShader<dimension_t,
							Geometry::TriangleShader<dimension_t, Shader::Pixel::TemplateShader<dimension_t,
							Vector::ColorSource<dimension_t>, ColorShaderType, TransformShaderType>>>;
						using signed_t = typename AutoDimension::ByDimension<dimension_t>::signed_t;
						using signed_wide_t = typename AutoDimension::ByDimension<dimension_t>::signed_wide_t;

						struct coordinates_t
						{
							signed_t x;
							signed_t y;
						};

						PaletteType Palette{};
						signed_t OffsetX = 0;
						signed_t OffsetY = 0;

					public:
						VectorShader(const dimension_t left, const dimension_t top,
							const dimension_t right, const dimension_t bottom)
							: Base(left, top, right, bottom) {}

						PaletteType& GetPalette() { return Palette; }

						void SetOffset(const signed_t offsetX, const signed_t offsetY)
						{
							OffsetX = offsetX;
							OffsetY = offsetY;
						}

						void OnColorSet(const uint8_t colorIndex)
						{
							Base::ColorSource.Color = Palette.GetColor(colorIndex);
						}

						void OnDrawPoint(IFrameBuffer* frame, const dimension_t x, const dimension_t y, const dimension_t thickness)
						{
							DrawPoint(frame, OffsetPoint(x, y), thickness);
						}

						void OnDrawLine(IFrameBuffer* frame, const dimension_t x1, const dimension_t y1,
							const dimension_t x2, const dimension_t y2, const dimension_t thickness)
						{
							DrawLine(frame, OffsetPoint(x1, y1), OffsetPoint(x2, y2), thickness);
						}

						void OnDrawRectangle(IFrameBuffer* frame, const dimension_t x1, const dimension_t y1,
							const dimension_t x2, const dimension_t y2, const bool fill, const dimension_t thickness)
						{
							DrawRectangle(frame, OffsetPoint(x1, y1), OffsetPoint(x2, y2), fill, thickness);
						}

						void OnDrawTriangle(IFrameBuffer* frame, const dimension_t x1, const dimension_t y1,
							const dimension_t x2, const dimension_t y2, const dimension_t x3, const dimension_t y3,
							const bool fill, const dimension_t thickness)
						{
							DrawTriangle(frame, OffsetPoint(x1, y1), OffsetPoint(x2, y2), OffsetPoint(x3, y3), fill, thickness);
						}

						void OnDrawCircle(IFrameBuffer* frame, const dimension_t centerX, const dimension_t centerY,
							const dimension_t radius, const bool fill, const dimension_t thickness)
						{
							const coordinates_t center = OffsetPoint(centerX, centerY);
							if (fill)
								Base::CircleFill(frame, static_cast<dimension_t>(center.x), static_cast<dimension_t>(center.y), radius);
							else if (thickness <= 1)
								Base::CircleLine(frame, static_cast<dimension_t>(center.x), static_cast<dimension_t>(center.y), radius);
							else
							{
								const signed_t outer = static_cast<signed_t>(radius);
								const signed_t inner = MaxValue<signed_t>(0, outer - static_cast<signed_t>(thickness) + 1);
								Base::CircleRingFill(frame, static_cast<dimension_t>(center.x), static_cast<dimension_t>(center.y),
									static_cast<dimension_t>(outer), static_cast<dimension_t>(inner));
							}
						}

						void OnDrawCircleArc(IFrameBuffer* frame, const dimension_t centerX, const dimension_t centerY,
							const angle_t startAngle, const angle_t endAngle, const dimension_t outerRadius,
							const dimension_t innerRadius, const bool fill, const dimension_t thickness)
						{
							const coordinates_t center = OffsetPoint(centerX, centerY);
							const dimension_t x = static_cast<dimension_t>(center.x);
							const dimension_t y = static_cast<dimension_t>(center.y);
							if (fill)
								Base::CircleArcFill(frame, x, y, startAngle, endAngle, outerRadius, innerRadius,
									Geometry::ArcBoundaryModeEnum::Inclusive, Geometry::RingBoundaryModeEnum::Inclusive);
							else if (thickness == 1)
								Base::CircleArcLine(frame, x, y, startAngle, endAngle, outerRadius,
									Geometry::ArcBoundaryModeEnum::Inclusive);
							else
							{
								const signed_t outerHalf = static_cast<signed_t>(thickness) / 2;
								const signed_t innerHalf = (static_cast<signed_t>(thickness) - 1) / 2;
								Base::CircleArcFill(frame, x, y, startAngle, endAngle,
									static_cast<dimension_t>(static_cast<signed_t>(outerRadius) + outerHalf),
									static_cast<dimension_t>(MaxValue<signed_t>(0, static_cast<signed_t>(outerRadius) - innerHalf)),
									Geometry::ArcBoundaryModeEnum::Inclusive, Geometry::RingBoundaryModeEnum::Inclusive);
							}
						}

						void OnDrawCircleRing(IFrameBuffer* frame, const dimension_t centerX, const dimension_t centerY,
							const dimension_t outerRadius, const dimension_t innerRadius, const bool /*fill*/, const dimension_t /*thickness*/)
						{
							const coordinates_t center = OffsetPoint(centerX, centerY);
							Base::CircleRingFill(frame, static_cast<dimension_t>(center.x), static_cast<dimension_t>(center.y), outerRadius, innerRadius);
						}

						void OnDrawCircleQuadrant(IFrameBuffer* frame, const dimension_t centerX, const dimension_t centerY,
							const dimension_t radius, const QuadrantEnum quadrant, const bool fill, const dimension_t thickness)
						{
							const coordinates_t center = OffsetPoint(centerX, centerY);
							const dimension_t x = static_cast<dimension_t>(center.x);
							const dimension_t y = static_cast<dimension_t>(center.y);
							if (fill || thickness == 1)
							{
								if (fill)
									Base::CircleQuadrantFill(frame, x, y, radius, quadrant,
										Geometry::ArcBoundaryModeEnum::Inclusive, Geometry::RingBoundaryModeEnum::Inclusive);
								else
									Base::CircleQuadrantLine(frame, x, y, radius, quadrant, Geometry::ArcBoundaryModeEnum::Inclusive);
								return;
							}

							const signed_t outerHalf = static_cast<signed_t>(thickness) / 2;
							const signed_t innerHalf = (static_cast<signed_t>(thickness) - 1) / 2;
							Base::CircleQuadrantRingFill(frame, x, y,
								static_cast<dimension_t>(static_cast<signed_t>(radius) + outerHalf),
								static_cast<dimension_t>(MaxValue<signed_t>(0, static_cast<signed_t>(radius) - innerHalf)),
								quadrant, Geometry::ArcBoundaryModeEnum::Inclusive, Geometry::RingBoundaryModeEnum::Inclusive);
							DrawQuadrantEdges(frame, center, static_cast<signed_t>(radius) + outerHalf, quadrant,
								static_cast<dimension_t>(outerHalf + innerHalf), static_cast<dimension_t>(innerHalf));
						}

					private:
						coordinates_t OffsetPoint(const dimension_t x, const dimension_t y) const
						{
							return {
								static_cast<signed_t>(static_cast<signed_t>(x) + OffsetX),
								static_cast<signed_t>(static_cast<signed_t>(y) + OffsetY)
							};
						}

						void DrawPoint(IFrameBuffer* frame, const coordinates_t point, const dimension_t thickness)
						{
							const signed_t radius = (static_cast<signed_t>(thickness) - 1) / 2;
							const signed_t left = MaxValue<signed_t>(point.x - radius, static_cast<signed_t>(Base::GetBoundsLeft()));
							const signed_t top = MaxValue<signed_t>(point.y - radius, static_cast<signed_t>(Base::GetBoundsTop()));
							const signed_t right = MinValue<signed_t>(point.x + radius + 1, static_cast<signed_t>(Base::GetBoundsRight()));
							const signed_t bottom = MinValue<signed_t>(point.y + radius + 1, static_cast<signed_t>(Base::GetBoundsBottom()));
							if (left < right && top < bottom)
								Base::RectangleFill(frame, static_cast<dimension_t>(left), static_cast<dimension_t>(top),
									static_cast<dimension_t>(right - 1), static_cast<dimension_t>(bottom - 1));
						}

						void DrawLine(IFrameBuffer* frame, const coordinates_t start, const coordinates_t end, const dimension_t thickness)
						{
							if (thickness == 1)
							{
								Geometry::LineClipPoint clippedStart{ start.x, start.y };
								Geometry::LineClipPoint clippedEnd{ end.x, end.y };
								if (Geometry::LineClipper::Clip(clippedStart, clippedEnd,
									Base::GetBoundsLeft(), Base::GetBoundsTop(),
									Base::GetBoundsRight(), Base::GetBoundsBottom()))
									Base::Line(frame, static_cast<dimension_t>(clippedStart.x), static_cast<dimension_t>(clippedStart.y),
										static_cast<dimension_t>(clippedEnd.x), static_cast<dimension_t>(clippedEnd.y));
								return;
							}

							signed_t x = start.x;
							signed_t y = start.y;
							const signed_t dx = AbsValue(end.x - start.x);
							const signed_t sx = start.x < end.x ? 1 : -1;
							const signed_t dy = -AbsValue(end.y - start.y);
							const signed_t sy = start.y < end.y ? 1 : -1;
							signed_wide_t error = static_cast<signed_wide_t>(dx) + dy;

							while (true)
							{
								DrawPoint(frame, { x, y }, thickness);
								if (x == end.x && y == end.y)
									break;

								const signed_wide_t twice = error * 2;
								if (twice >= dy)
								{
									error += dy;
									x += sx;
								}
								if (twice <= dx)
								{
									error += dx;
									y += sy;
								}
							}
						}

						void DrawRectangle(IFrameBuffer* frame, const coordinates_t first, const coordinates_t second,
							const bool fill, const dimension_t thickness)
						{
							if (fill)
							{
								const signed_t left = MaxValue(MinValue(first.x, second.x), static_cast<signed_t>(Base::GetBoundsLeft()));
								const signed_t top = MaxValue(MinValue(first.y, second.y), static_cast<signed_t>(Base::GetBoundsTop()));
								const signed_t right = MinValue<signed_t>(MaxValue(first.x, second.x), Base::GetBoundsRight() - 1);
								const signed_t bottom = MinValue<signed_t>(MaxValue(first.y, second.y), Base::GetBoundsBottom() - 1);
								if (left <= right && top <= bottom)
									Base::RectangleFill(frame, static_cast<dimension_t>(left), static_cast<dimension_t>(top),
										static_cast<dimension_t>(right), static_cast<dimension_t>(bottom));
							}
							else
							{
								DrawLine(frame, first, { second.x, first.y }, thickness);
								DrawLine(frame, { second.x, first.y }, second, thickness);
								DrawLine(frame, second, { first.x, second.y }, thickness);
								DrawLine(frame, { first.x, second.y }, first, thickness);
							}
						}

						void DrawTriangle(IFrameBuffer* frame, const coordinates_t first, const coordinates_t second,
							const coordinates_t third, const bool fill, const dimension_t thickness)
						{
							if (fill)
							{
								Base::TriangleFill(frame, static_cast<dimension_t>(first.x), static_cast<dimension_t>(first.y),
									static_cast<dimension_t>(second.x), static_cast<dimension_t>(second.y),
									static_cast<dimension_t>(third.x), static_cast<dimension_t>(third.y));
							}
							else
							{
								DrawLine(frame, first, second, thickness);
								DrawLine(frame, second, third, thickness);
								DrawLine(frame, third, first, thickness);
							}
						}

						void DrawQuadrantEdges(IFrameBuffer* frame, const coordinates_t center,
							const signed_t outerRadius, const QuadrantEnum quadrant,
							const dimension_t capLong, const dimension_t capShort)
						{
							if (capShort == 0)
								return;

							if (capShort == 1)
							{
								switch (quadrant)
								{
								case QuadrantEnum::TopRight:
									DrawLine(frame, { center.x, static_cast<signed_t>(center.y - outerRadius) },
										{ center.x, static_cast<signed_t>(center.y - outerRadius + capLong) }, 1);
									DrawLine(frame, { static_cast<signed_t>(center.x + outerRadius - capLong), center.y },
										{ static_cast<signed_t>(center.x + outerRadius), center.y }, 1);
									break;
								case QuadrantEnum::BottomRight:
									DrawLine(frame, { static_cast<signed_t>(center.x + outerRadius - capLong), center.y },
										{ static_cast<signed_t>(center.x + outerRadius), center.y }, 1);
									DrawLine(frame, { center.x, static_cast<signed_t>(center.y + outerRadius - capLong) },
										{ center.x, static_cast<signed_t>(center.y + outerRadius) }, 1);
									break;
								case QuadrantEnum::BottomLeft:
									DrawLine(frame, { static_cast<signed_t>(center.x - outerRadius), center.y },
										{ static_cast<signed_t>(center.x - outerRadius + capLong), center.y }, 1);
									DrawLine(frame, { center.x, static_cast<signed_t>(center.y + outerRadius - capLong) },
										{ center.x, static_cast<signed_t>(center.y + outerRadius) }, 1);
									break;
								case QuadrantEnum::TopLeft:
									DrawLine(frame, { static_cast<signed_t>(center.x - outerRadius), center.y },
										{ static_cast<signed_t>(center.x - outerRadius + capLong), center.y }, 1);
									DrawLine(frame, { center.x, static_cast<signed_t>(center.y - outerRadius) },
										{ center.x, static_cast<signed_t>(center.y - outerRadius + capLong) }, 1);
									break;
								default:
									break;
								}
								return;
							}

							switch (quadrant)
							{
							case QuadrantEnum::TopRight:
								Base::RectangleFill(frame, center.x - capShort + 1, center.y - outerRadius,
									center.x - 1, center.y - outerRadius + capLong);
								Base::RectangleFill(frame, center.x + outerRadius - capLong, center.y + 1,
									center.x + outerRadius, center.y + capShort);
								break;
							case QuadrantEnum::BottomRight:
								Base::RectangleFill(frame, center.x + outerRadius - capLong, center.y - capShort + 1,
									center.x + outerRadius, center.y - 1);
								Base::RectangleFill(frame, center.x + 1, center.y + outerRadius - capLong,
									center.x + capShort, center.y + outerRadius);
								break;
							case QuadrantEnum::BottomLeft:
								Base::RectangleFill(frame, center.x - outerRadius, center.y - capShort + 1,
									center.x - outerRadius + capLong, center.y - 1);
								Base::RectangleFill(frame, center.x - capShort + 1, center.y + outerRadius - capLong,
									center.x - 1, center.y + outerRadius);
								break;
							case QuadrantEnum::TopLeft:
								Base::RectangleFill(frame, center.x + 1, center.y - outerRadius,
									center.x + capShort, center.y - outerRadius + capLong);
								Base::RectangleFill(frame, center.x - outerRadius, center.y + 1,
									center.x - outerRadius + capLong, center.y + 1 + capShort);
								break;
							default:
								break;
							}
						}
					};
				}
			}
		}
	}
}
#endif