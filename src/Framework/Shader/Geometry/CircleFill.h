#ifndef _EGFX_FRAMEWORK_SHADER_GEOMETRY_CIRCLE_FILL_h
#define _EGFX_FRAMEWORK_SHADER_GEOMETRY_CIRCLE_FILL_h

#include "CircleLine.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Geometry
			{
				template<typename dimension_t,
					typename PrimitiveShaderType = Framework::Shader::Geometry::RectangleShader<dimension_t>
				>
				class CircleFillShader : public CircleLineShader<dimension_t, PrimitiveShaderType>
				{
				private:
					using Base = CircleLineShader<dimension_t, PrimitiveShaderType>;

					using signed_t = typename AutoDimension::ByDimension<dimension_t>::signed_t;
					using wide_signed_t = typename AutoDimension::ByDimension<dimension_t>::signed_wide_t;

				public:
					CircleFillShader(const dimension_t left, const dimension_t top,
						const dimension_t right, const dimension_t bottom)
						: Base(left, top, right, bottom) {}
					~CircleFillShader() = default;

					void CircleFill(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t outerRadius,
						const RingBoundaryModeEnum ringBoundaryMode = RingBoundaryModeEnum::Inclusive)
					{
						const bool includeOuterBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeOuter &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;
						const bool includeInnerBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeInner &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;

						// Handle and dispatch edge cases for small circles.
						switch (outerRadius)
						{
						case 0:
							// A radius of 0 is a single pixel at the center.
							if (includeOuterBoundary || includeInnerBoundary)
								Base::Pixel(framebuffer, centerX, centerY);
							return;
						case 1:
							// A radius of 1 is a cross shape of 4 pixels with no fill space.
							if (includeOuterBoundary)
							{
								Base::Pixel(framebuffer, centerX, centerY - 1);
								Base::Pixel(framebuffer, centerX + 1, centerY);
								Base::Pixel(framebuffer, centerX, centerY + 1);
								Base::Pixel(framebuffer, centerX - 1, centerY);
							}
							// The inner boundary is a single pixel at the center.
							if (includeInnerBoundary)
								Base::Pixel(framebuffer, centerX, centerY);
							return;
						default:
							break;
						}

						const dimension_t boundedOuterRadius = (includeOuterBoundary ? outerRadius : static_cast<dimension_t>(outerRadius - 1));
						if (includeInnerBoundary)
						{
							Base::Line(framebuffer, centerX - boundedOuterRadius, centerY,
								centerX + boundedOuterRadius, centerY);
						}
						else
						{
							Base::Line(framebuffer, centerX - boundedOuterRadius, centerY, centerX - 1, centerY);
							Base::Line(framebuffer, centerX + 1, centerY, centerX + boundedOuterRadius, centerY);
						}

						dimension_t x = outerRadius;
						dimension_t y = 0;
						signed_t error = static_cast<signed_t>(1) - outerRadius;

						const bool radiusOdd = (outerRadius & 1) != 0;
						const dimension_t radiusOffset = radiusOdd ? 0 : 1;
						while ((y + 1) < x)
						{
							y++;
							if (error <= 0)
							{
								error += (static_cast<signed_t>(y) * 2) + 1;
							}
							else
							{
								x--;
								error += ((static_cast<signed_t>(y) - x) * 2) + 1;

								if (y + radiusOffset < x)
								{
									// x is finished – emit its outer-band spans exactly once.
									const dimension_t outerCoordinate = (y - 1);
									Base::Line(framebuffer,
										centerX - outerCoordinate, centerY - x,
										centerX + outerCoordinate, centerY - x);
									Base::Line(framebuffer,
										centerX - outerCoordinate, centerY + x,
										centerX + outerCoordinate, centerY + x);
								}
							}

							const dimension_t horizontalCoordinate = (includeOuterBoundary ? x : (x - 1)) - 0;
							const dimension_t verticalCoordinate = y;

							// Central band – every y is unique.
							Base::Line(framebuffer, centerX - horizontalCoordinate, centerY - verticalCoordinate,
								centerX + horizontalCoordinate, centerY - verticalCoordinate);
							Base::Line(framebuffer, centerX - horizontalCoordinate, centerY + verticalCoordinate,
								centerX + horizontalCoordinate, centerY + verticalCoordinate);
						}
					}

					void CircleFillOutside(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t outerRadius,
						const RingBoundaryModeEnum ringBoundaryMode = RingBoundaryModeEnum::Inclusive)
					{
						if (outerRadius == 0)
							return;

						const bool includeOuterBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeOuter &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;
						const wide_signed_t radius = static_cast<wide_signed_t>(outerRadius);
						const wide_signed_t radiusSquared = radius * radius;

						for (wide_signed_t y = -radius; y <= radius; y++)
						{
							const wide_signed_t ySquared = y * y;
							wide_signed_t firstOutside = 0;

							while (firstOutside <= radius)
							{
								const wide_signed_t distanceSquared = firstOutside * firstOutside + ySquared;
								if (distanceSquared > radiusSquared ||
									(distanceSquared == radiusSquared && !includeOuterBoundary))
								{
									break;
								}

								firstOutside++;
							}

							if (firstOutside > radius)
								continue;

							const wide_signed_t leftStart = static_cast<wide_signed_t>(centerX) - radius;
							const wide_signed_t leftEnd = static_cast<wide_signed_t>(centerX) - firstOutside;
							const wide_signed_t rightStart = static_cast<wide_signed_t>(centerX) + firstOutside;
							const wide_signed_t rightEnd = static_cast<wide_signed_t>(centerX) + radius;
							const wide_signed_t row = static_cast<wide_signed_t>(centerY) + y;

							if (leftStart <= leftEnd)
								Base::Line(framebuffer,
									static_cast<dimension_t>(leftStart), static_cast<dimension_t>(row),
									static_cast<dimension_t>(leftEnd), static_cast<dimension_t>(row));

							if (rightStart <= rightEnd)
								Base::Line(framebuffer,
									static_cast<dimension_t>(rightStart), static_cast<dimension_t>(row),
									static_cast<dimension_t>(rightEnd), static_cast<dimension_t>(row));
						}
					}

					void CircleQuadrantFill(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t outerRadius,
						const QuadrantEnum quadrant,
						const ArcBoundaryModeEnum arcBoundaryMode = ArcBoundaryModeEnum::Inclusive,
						const RingBoundaryModeEnum ringBoundaryMode = RingBoundaryModeEnum::Inclusive)
					{
						bool includeOuterBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeOuter &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;
						bool includeInnerBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeInner &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;

						bool includeStart = arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStart
							&& arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd;
						bool includeEnd = (arcBoundaryMode != ArcBoundaryModeEnum::ExcludeEnd) && (arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd);

						if (quadrant == QuadrantEnum::BottomRight || quadrant == QuadrantEnum::TopLeft)
						{
							const bool temp = includeStart;
							includeStart = includeEnd;
							includeEnd = temp;
						}

						// Handle and dispatch edge cases for small circles.
						switch (outerRadius)
						{
						case 0:
							// A radius of 0 is a single pixel at the center.
							if (includeOuterBoundary || includeInnerBoundary)
								Base::Pixel(framebuffer, centerX, centerY);
							return;
						case 1:
							// A radius of 1 is a cross shape of 4 pixels with no fill space.
							if (includeOuterBoundary)
							{
								switch (quadrant)
								{
								case QuadrantEnum::TopRight:
									if (includeEnd) Base::Pixel(framebuffer, centerX, centerY - 1);
									if (includeStart) Base::Pixel(framebuffer, centerX + 1, centerY);
									break;
								case QuadrantEnum::BottomRight:
									if (includeStart) Base::Pixel(framebuffer, centerX + 1, centerY);
									if (includeEnd) Base::Pixel(framebuffer, centerX, centerY + 1);
									break;
								case QuadrantEnum::BottomLeft:
									if (includeStart) Base::Pixel(framebuffer, centerX - 1, centerY);
									if (includeEnd) Base::Pixel(framebuffer, centerX, centerY + 1);
									break;
								case QuadrantEnum::TopLeft:
									if (includeStart) Base::Pixel(framebuffer, centerX - 1, centerY);
									if (includeEnd) Base::Pixel(framebuffer, centerX, centerY - 1);
									break;
								default:
									break;
								};
							}
							if (includeInnerBoundary)
								Base::Pixel(framebuffer, centerX, centerY);
							return;
						default:
							break;
						}

						if (includeEnd)
						{
							const dimension_t outerRow = (includeOuterBoundary ? outerRadius : (outerRadius - 1));
							const dimension_t innerRow = includeInnerBoundary ? 0 : 1;
							QuadrantSpanLine(framebuffer, quadrant, centerX, centerY, innerRow, outerRow, 0);
						}

						dimension_t x = outerRadius;
						dimension_t y = 0;
						signed_t error = static_cast<signed_t>(1) - outerRadius;

						const dimension_t innerStart = includeStart ? 0 : 1;

						// Walk the circle's octants until the diagonal is reached, not inclusive with the (y + 1).
						while ((y + 1) < x)
						{
							y++;
							if (error <= 0)
							{
								error += (static_cast<signed_t>(y) * 2) + 1;
							}
							else
							{
								// Advance x and error after the outer-band spans are emitted to avoid double-counting the diagonal.
								x--;
								error += (static_cast<signed_t>(y - x) * 2) + 1;

								// x is finished – emit its outer-band spans. Check includeEnd if vertical hits the orthogonal axis limit.
								const dimension_t outer = (includeOuterBoundary ? y : (y - 1));
								const dimension_t vertical = x;
								QuadrantSpanLine(framebuffer, quadrant, centerX, centerY, innerStart, outer, vertical);
							}

							const dimension_t outerCoordinate = (includeOuterBoundary ? (x) : (x - 1)) - 1;
							const dimension_t verticalCoordinate = y;

							// Central band – every y is unique.
							if ((y + 1) < x)
								QuadrantSpanLine(framebuffer, quadrant, centerX, centerY, innerStart, outerCoordinate, verticalCoordinate);
						}

						if (y + 1 == x)
						{
							const dimension_t outerCoordinate = (includeOuterBoundary ? (x) : (x - 1)) - 1;
							const dimension_t verticalCoordinate = y;

							QuadrantSpanLine(framebuffer, quadrant, centerX, centerY, innerStart, outerCoordinate, verticalCoordinate);
						}
					}

					void CircleQuadrantFillOutside(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t outerRadius,
						const QuadrantEnum quadrant,
						const ArcBoundaryModeEnum arcBoundaryMode = ArcBoundaryModeEnum::Inclusive,
						const RingBoundaryModeEnum ringBoundaryMode = RingBoundaryModeEnum::Inclusive)
					{
						const bool includeOuterBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeOuter &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;
						const bool includeInnerBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeInner &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;
						const bool includeStart = arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStart &&
							arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd;
						const bool includeEnd = arcBoundaryMode != ArcBoundaryModeEnum::ExcludeEnd &&
							arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd;

						bool includeHorizontalBoundary;
						bool includeVerticalBoundary;
						switch (quadrant)
						{
						case QuadrantEnum::TopRight:
							includeHorizontalBoundary = includeEnd;
							includeVerticalBoundary = includeStart;
							break;
						case QuadrantEnum::BottomRight:
							includeHorizontalBoundary = includeStart;
							includeVerticalBoundary = includeEnd;
							break;
						case QuadrantEnum::BottomLeft:
							includeHorizontalBoundary = includeEnd;
							includeVerticalBoundary = includeStart;
							break;
						case QuadrantEnum::TopLeft:
						default:
							includeHorizontalBoundary = includeStart;
							includeVerticalBoundary = includeEnd;
							break;
						}

						if (outerRadius == 0)
						{
							if (includeInnerBoundary || includeOuterBoundary)
								Base::Pixel(framebuffer, centerX, centerY);
							return;
						}

						const wide_signed_t radius = static_cast<wide_signed_t>(outerRadius);
						const wide_signed_t radiusSquared = radius * radius;
						const wide_signed_t maxOuter = includeOuterBoundary ? radius : radius - 1;

						for (wide_signed_t y = 0; y <= maxOuter; y++)
						{
							const wide_signed_t ySquared = y * y;
							wide_signed_t firstOutside = 0;
							while (firstOutside <= maxOuter)
							{
								const wide_signed_t distanceSquared = firstOutside * firstOutside + ySquared;
								if (distanceSquared > radiusSquared ||
									(distanceSquared == radiusSquared && includeInnerBoundary))
									break;
								firstOutside++;
							}

							if (y == 0 && !includeHorizontalBoundary)
								firstOutside = maxOuter + 1;
							else if (y == radius && !includeVerticalBoundary && firstOutside == 0)
								firstOutside = 1;

							if (firstOutside <= maxOuter)
								QuadrantSpanLine(framebuffer, quadrant, centerX, centerY,
									static_cast<dimension_t>(firstOutside), static_cast<dimension_t>(maxOuter),
									static_cast<dimension_t>(y));
						}
					}

				protected:
					void QuadrantSpanLine(IFrameBuffer* framebuffer,
						const QuadrantEnum quadrant,
						const dimension_t centerX,
						const dimension_t centerY,
						const dimension_t inner,
						const dimension_t outer,
						const dimension_t y)
					{
						switch (quadrant)
						{
						case QuadrantEnum::TopRight:
							Base::Line(framebuffer, centerX + inner, centerY - y, centerX + outer, centerY - y);
							break;
						case QuadrantEnum::BottomRight:
							Base::Line(framebuffer, centerX + inner, centerY + y, centerX + outer, centerY + y);
							break;
						case QuadrantEnum::BottomLeft:
							Base::Line(framebuffer, centerX - outer, centerY + y, centerX - inner, centerY + y);
							break;
						case QuadrantEnum::TopLeft:
						default:
							Base::Line(framebuffer, centerX - outer, centerY - y, centerX - inner, centerY - y);
							break;
						}
					}
				};
			}
		}
	}
}

#endif