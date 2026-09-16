#ifndef _EGFX_FRAMEWORK_SHADER_GEOMETRY_CIRCLE_RING_h
#define _EGFX_FRAMEWORK_SHADER_GEOMETRY_CIRCLE_RING_h

#include "CircleFill.h"

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
				class CircleRingShader : public CircleFillShader<dimension_t, PrimitiveShaderType>
				{
				private:
					using Base = CircleFillShader<dimension_t, PrimitiveShaderType>;

					using signed_t = typename AutoDimension::ByDimension<dimension_t>::signed_t;
					using wide_signed_t = typename AutoDimension::ByDimension<dimension_t>::signed_wide_t;

				protected:
					using Base::QuadrantSpanLine;

				public:
					CircleRingShader(const dimension_t left, const dimension_t top,
						const dimension_t right, const dimension_t bottom)
						: Base(left, top, right, bottom) {}
					~CircleRingShader() = default;

					void CircleRingFill(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t outerRadius, const dimension_t innerRadius,
						const RingBoundaryModeEnum ringBoundaryMode = RingBoundaryModeEnum::Inclusive)
					{
						const bool includeInnerBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeInner &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;
						const bool includeOuterBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeOuter &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;

						if (outerRadius <= 0 || innerRadius > outerRadius)
						{
							return; // Nothing to draw.
						}
						else if (outerRadius == 1)
						{
							if (includeOuterBoundary && (innerRadius == 0 || includeInnerBoundary))
								Base::Pixel(framebuffer, centerX, centerY);
							return;
						}
						else if (innerRadius == 0)
						{
							Base::CircleFill(framebuffer, centerX, centerY, outerRadius, ringBoundaryMode);
						}
						else if (innerRadius == (outerRadius - 1))
						{
							if (includeOuterBoundary)
								Base::CircleLine(framebuffer, centerX, centerY, outerRadius);
							else if (includeInnerBoundary)
								Base::CircleLine(framebuffer, centerX, centerY, innerRadius);
						}
						else
						{
							CircleRingFillImpl(framebuffer, centerX, centerY, outerRadius, innerRadius, ringBoundaryMode);
							//CircleRingFillNew(framebuffer, centerX, centerY, outerRadius, innerRadius, ringBoundaryMode);
						}
					}

					void CircleQuadrantRingFill(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t outerRadius, const dimension_t innerRadius,
						const QuadrantEnum quadrant,
						const ArcBoundaryModeEnum arcBoundaryMode = ArcBoundaryModeEnum::Inclusive,
						const RingBoundaryModeEnum ringBoundaryMode = RingBoundaryModeEnum::Inclusive)
					{
						if (outerRadius <= 0 || innerRadius >= outerRadius)
							return;

						else if (innerRadius == (outerRadius - 1))
						{
							if (ringBoundaryMode != RingBoundaryModeEnum::Inclusive)
								return;

							const OctantEnum firstOctant = quadrant == QuadrantEnum::BottomRight
								? OctantEnum::HorizontalBottomRight
								: quadrant == QuadrantEnum::BottomLeft
								? OctantEnum::VerticalBottomLeft
								: quadrant == QuadrantEnum::TopLeft
								? OctantEnum::HorizontalTopLeft
								: OctantEnum::HorizontalTopRight;
							const OctantEnum secondOctant = quadrant == QuadrantEnum::BottomRight
								? OctantEnum::VerticalBottomRight
								: quadrant == QuadrantEnum::BottomLeft
								? OctantEnum::HorizontalBottomLeft
								: quadrant == QuadrantEnum::TopLeft
								? OctantEnum::VerticalTopLeft
								: OctantEnum::VerticalTopRight;
							const bool includeStart = arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStart &&
								arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd;
							const bool includeEnd = arcBoundaryMode != ArcBoundaryModeEnum::ExcludeEnd &&
								arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd;
							const bool firstOctantEndsAtStart = quadrant != QuadrantEnum::TopRight;
							const dimension_t firstStartLimit =
								(firstOctantEndsAtStart ? includeStart : includeEnd) ? 0 : 1;
							const dimension_t secondStartLimit =
								(firstOctantEndsAtStart ? includeEnd : includeStart) ? 0 : 1;
							Base::CircleOctantLineLimited(framebuffer, centerX, centerY, outerRadius,
								firstStartLimit, outerRadius, firstOctant);
							Base::CircleOctantLineLimited(framebuffer, centerX, centerY, outerRadius,
								secondStartLimit, outerRadius, secondOctant);
							return;
						}
						else if (innerRadius == 0)
						{
							Base::CircleQuadrantFill(framebuffer, centerX, centerY, outerRadius, quadrant, arcBoundaryMode);
							return;
						}

						CircleQuadrantRingFillImpl(framebuffer, centerX, centerY, outerRadius, innerRadius, quadrant, arcBoundaryMode, ringBoundaryMode);
					}

					void CircleOctantRingFill(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t outerRadius, const dimension_t innerRadius,
						const OctantEnum octant,
						const ArcBoundaryModeEnum arcBoundaryMode = ArcBoundaryModeEnum::Inclusive,
						const RingBoundaryModeEnum ringBoundaryMode = RingBoundaryModeEnum::Inclusive)
					{
						if (outerRadius <= 0 || innerRadius > outerRadius)
							return;

						const bool includeStart = arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStart &&
							arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd;
						const bool includeEnd = arcBoundaryMode != ArcBoundaryModeEnum::ExcludeEnd &&
							arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd;

						if (innerRadius == (outerRadius - 1))
						{
							if (ringBoundaryMode != RingBoundaryModeEnum::Inclusive)
								return;

							const dimension_t startLimit = (static_cast<uint8_t>(octant) & 1) == 0
								? (includeStart ? 0 : 1)
								: (includeEnd ? 0 : 1);
							Base::CircleOctantLineLimited(framebuffer, centerX, centerY, outerRadius,
								startLimit, outerRadius, octant);
							return;
						}

						const bool horizontal = octant == OctantEnum::HorizontalBottomRight ||
							octant == OctantEnum::HorizontalBottomLeft ||
							octant == OctantEnum::HorizontalTopLeft ||
							octant == OctantEnum::HorizontalTopRight;
						const int8_t xDirection = octant == OctantEnum::VerticalBottomLeft ||
							octant == OctantEnum::HorizontalBottomLeft ||
							octant == OctantEnum::HorizontalTopLeft ||
							octant == OctantEnum::VerticalTopLeft ? -1 : 1;
						const int8_t yDirection = octant == OctantEnum::HorizontalTopLeft ||
							octant == OctantEnum::VerticalTopLeft ||
							octant == OctantEnum::VerticalTopRight ||
							octant == OctantEnum::HorizontalTopRight ? -1 : 1;

						// Even octants start on the axis and end on the diagonal.
						// Odd octants start on the diagonal and end on the axis.
						const bool isEvenOctant = (static_cast<uint8_t>(octant) & 1) == 0;
						const bool includeAxis = isEvenOctant ? includeStart : includeEnd;
						const bool includeDiagonal = isEvenOctant ? includeEnd : includeStart;

						const bool includeInnerBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeInner &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;
						const bool includeOuterBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeOuter &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;
						const bool hasInner = innerRadius > 0;

						dimension_t outerX = outerRadius;
						dimension_t innerX = hasInner ? innerRadius : 0;
						dimension_t y = 0;
						signed_t outerD = signed_t(1) - outerRadius;
						signed_t innerD = hasInner ? signed_t(1) - innerRadius : 0;


						if (!includeAxis)
						{
							// Fast forward to the first row that is not the axis.
							y++;
							if (outerD <= 0)
							{
								outerD += (2 * y) + 1;
							}
							else
							{
								outerX--;
								outerD += (2 * (y - outerX)) + 1;
							}
							if (hasInner && y <= innerRadius)
							{
								if (innerD <= 0)
								{
									innerD += (2 * y) + 1;
								}
								else
								{
									innerX--;
									innerD += (2 * (y - innerX)) + 1;
								}
							}
						}

						while (y <= outerX)
						{
							const dimension_t activeInnerX = hasInner && y <= innerRadius ? innerX : 0;
							const dimension_t innerStart = includeInnerBoundary
								? activeInnerX
								: static_cast<dimension_t>(activeInnerX + 1);
							const dimension_t outerEnd = includeOuterBoundary
								? outerX
								: static_cast<dimension_t>(outerX - 1);

							const int8_t diagonalOffset = includeDiagonal ? 0 : 1;
							const dimension_t start = MaxValue<dimension_t>(innerStart, y + diagonalOffset);
							if (horizontal)
							{
								if (start <= outerEnd)
								{
									const pixel_t row = static_cast<pixel_t>(static_cast<wide_signed_t>(centerY) +
										static_cast<wide_signed_t>(y) * yDirection);
									const pixel_t first = static_cast<pixel_t>(static_cast<wide_signed_t>(centerX) +
										static_cast<wide_signed_t>(start) * xDirection);
									const pixel_t last = static_cast<pixel_t>(static_cast<wide_signed_t>(centerX) +
										static_cast<wide_signed_t>(outerEnd) * xDirection);
									EmitHorizontalSpan(framebuffer, first, last, row);
								}
							}
							else
							{
								if (start <= outerEnd)
								{
									const pixel_t column = static_cast<pixel_t>(static_cast<wide_signed_t>(centerX) +
										static_cast<wide_signed_t>(y) * xDirection);
									const pixel_t first = static_cast<pixel_t>(static_cast<wide_signed_t>(centerY) +
										static_cast<wide_signed_t>(start) * yDirection);
									const pixel_t last = static_cast<pixel_t>(static_cast<wide_signed_t>(centerY) +
										static_cast<wide_signed_t>(outerEnd) * yDirection);
									EmitVerticalSpan(framebuffer, column, first, last);
								}
							}

							y++;
							if (outerD <= 0)
							{
								outerD += (2 * y) + 1;
							}
							else
							{
								outerX--;
								outerD += (2 * (y - outerX)) + 1;
							}
							if (hasInner && y <= innerRadius)
							{
								if (innerD <= 0)
								{
									innerD += (2 * y) + 1;
								}
								else
								{
									innerX--;
									innerD += (2 * (y - innerX)) + 1;
								}
							}
						}
					}

				protected:
					void EmitHorizontalSpan(IFrameBuffer* framebuffer,
						const wide_signed_t first, const wide_signed_t last, const wide_signed_t row)
					{
						const wide_signed_t left = MaxValue<wide_signed_t>(
							MinValue(first, last), static_cast<wide_signed_t>(Base::GetBoundsLeft()));
						const wide_signed_t right = MinValue<wide_signed_t>(
							MaxValue(first, last), static_cast<wide_signed_t>(Base::GetBoundsRight() - 1));
						if (left > right || row < static_cast<wide_signed_t>(Base::GetBoundsTop()) ||
							row >= static_cast<wide_signed_t>(Base::GetBoundsBottom()))
							return;

						Base::RectangleFill(framebuffer,
							static_cast<dimension_t>(left), static_cast<dimension_t>(row),
							static_cast<dimension_t>(right), static_cast<dimension_t>(row));
					}

					void EmitVerticalSpan(IFrameBuffer* framebuffer,
						const wide_signed_t column, const wide_signed_t first, const wide_signed_t last)
					{
						const wide_signed_t top = MaxValue<wide_signed_t>(
							MinValue(first, last), static_cast<wide_signed_t>(Base::GetBoundsTop()));
						const wide_signed_t bottom = MinValue<wide_signed_t>(
							MaxValue(first, last), static_cast<wide_signed_t>(Base::GetBoundsBottom() - 1));
						const wide_signed_t left = static_cast<wide_signed_t>(Base::GetBoundsLeft());
						const wide_signed_t right = static_cast<wide_signed_t>(Base::GetBoundsRight() - 1);
						if (top > bottom || column < left || column > right)
							return;

						Base::RectangleFill(framebuffer,
							static_cast<dimension_t>(column), static_cast<dimension_t>(top),
							static_cast<dimension_t>(column), static_cast<dimension_t>(bottom));
					}

					void CircleRingFillNew(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t outerRadius, const dimension_t innerRadius,
						const RingBoundaryModeEnum ringBoundaryMode = RingBoundaryModeEnum::Inclusive)
					{
						const bool includeOuterBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeOuter &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;
						const bool includeInnerBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeInner &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;

						const bool radiusOddO = (outerRadius & 1) != 0;
						const dimension_t radiusOffsetO = radiusOddO ? 0 : 1;
						const dimension_t boundedOuterRadius = includeOuterBoundary ? outerRadius : static_cast<dimension_t>(outerRadius - 1);
						const dimension_t boundedInnerRadius = includeInnerBoundary ? innerRadius : static_cast<dimension_t>(innerRadius + 1);
						const dimension_t radiusRange = boundedOuterRadius - boundedInnerRadius;

						// Initial center horizontal lines.
						if (boundedInnerRadius <= boundedOuterRadius)
						{
							//Base::Line(framebuffer, centerX + boundedInnerRadius, centerY, centerX + boundedOuterRadius, centerY);
							//Base::Line(framebuffer, centerX - boundedOuterRadius, centerY, centerX - boundedInnerRadius, centerY);
						}

						dimension_t xo = boundedOuterRadius;
						dimension_t xi = boundedInnerRadius;
						dimension_t y = 0;

						signed_t error_o = static_cast<signed_t>(1) - boundedOuterRadius;
						signed_t error_i = boundedInnerRadius > 0 ? static_cast<signed_t>(1) - boundedInnerRadius : 0;

						while ((y + 1) < xi)
						{
							// Emit the central band spans for the current y.
							const dimension_t currentOuter = (includeOuterBoundary ? xo : (xo - 1)) + 0;
							const dimension_t currentInner = (includeInnerBoundary ? xi : (xi + 1));
							Base::Line(framebuffer, centerX + currentInner, centerY + y, centerX + currentOuter, centerY + y);
							Base::Line(framebuffer, centerX + currentInner, centerY - y, centerX + currentOuter, centerY - y);
							Base::Line(framebuffer, centerX - currentOuter, centerY + y, centerX - currentInner, centerY + y);
							Base::Line(framebuffer, centerX - currentOuter, centerY - y, centerX - currentInner, centerY - y);


							y++;
							// Advance outer walker
							if (error_o <= 0)
							{
								error_o += (static_cast<signed_t>(y) * 2) + 1;
							}
							else
							{
								xo--;
								error_o += ((static_cast<signed_t>(y) - xo) * 2) + 1;


								// x is finished – emit its outer-band spans exactly once.
								if (xo >= (boundedInnerRadius))
								{
									const dimension_t currentOuter = (includeOuterBoundary ? y : (y - 1)) - 1;
									const dimension_t vertical = xo;
									Base::Line(framebuffer,
										centerX - currentOuter, centerY - vertical,
										centerX + currentOuter, centerY - vertical);
									Base::Line(framebuffer,
										centerX - currentOuter, centerY + vertical,
										centerX + currentOuter, centerY + vertical);
								}
								else// if (xo >= (boundedInnerRadius - 2))
								{
									// Inner transition span at row x (xo).
									const dimension_t currentOuter = (includeOuterBoundary ? y : (y - 1));
									//const dimension_t currentInner = (xo - y);
									const dimension_t currentInner = currentOuter - radiusRange - (includeInnerBoundary ? 0 : 1);
									const dimension_t vertical = xo;
									Base::Line(framebuffer, centerX + currentInner, centerY + vertical, centerX + currentOuter, centerY + vertical);
									Base::Line(framebuffer, centerX + currentInner, centerY - vertical, centerX + currentOuter, centerY - vertical);
									Base::Line(framebuffer, centerX - currentOuter, centerY + vertical, centerX - currentInner, centerY + vertical);
									Base::Line(framebuffer, centerX - currentOuter, centerY - vertical, centerX - currentInner, centerY - vertical);
								}
							}

							// Advance inner walker if active
							if (innerRadius > 0 && y <= xo)
							{
								if (error_i <= 0)
								{
									error_i += (static_cast<signed_t>(y) * 2) + 1;
								}
								else
								{
									xi--;
									error_i += ((static_cast<signed_t>(y) - xi) * 2) + 1;

								}
							}
						}
					}


					void CircleRingFillImpl(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t outerRadius, const dimension_t innerRadius,
						const RingBoundaryModeEnum ringBoundaryMode = RingBoundaryModeEnum::Inclusive)
					{
						const bool hasInner = innerRadius > 0 && innerRadius < outerRadius;
						const bool includeInnerBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeInner &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;
						const bool includeOuterBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeOuter &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;

						dimension_t outerX = outerRadius;
						dimension_t innerX = hasInner ? innerRadius : 0;
						dimension_t y = 0;
						signed_t outerD = signed_t(1) - outerRadius;
						signed_t innerD = hasInner ? signed_t(1) - innerRadius : 0;

						while (y <= outerX)
						{
							const dimension_t activeInnerX = hasInner && y <= innerRadius ? innerX : 0;
							const dimension_t innerStart = includeInnerBoundary
								? activeInnerX
								: static_cast<dimension_t>(activeInnerX + 1);
							const dimension_t outerEnd = includeOuterBoundary
								? outerX
								: static_cast<dimension_t>(outerX - 1);

							const dimension_t horizontalStart = MaxValue<dimension_t>(innerStart,
								static_cast<dimension_t>(y + 1));
							if (horizontalStart <= outerEnd)
							{
								const wide_signed_t rowBottom = static_cast<wide_signed_t>(centerY) +
									static_cast<wide_signed_t>(y);
								const wide_signed_t rowTop = static_cast<wide_signed_t>(centerY) -
									static_cast<wide_signed_t>(y);
								const wide_signed_t rightStart = static_cast<wide_signed_t>(centerX) +
									static_cast<wide_signed_t>(horizontalStart);
								const wide_signed_t rightEnd = static_cast<wide_signed_t>(centerX) +
									static_cast<wide_signed_t>(outerEnd);
								const wide_signed_t leftStart = static_cast<wide_signed_t>(centerX) -
									static_cast<wide_signed_t>(outerEnd);
								const wide_signed_t leftEnd = static_cast<wide_signed_t>(centerX) -
									static_cast<wide_signed_t>(horizontalStart);

								EmitHorizontalSpan(framebuffer, rightStart, rightEnd, rowBottom);
								EmitHorizontalSpan(framebuffer, leftStart, leftEnd, rowBottom);
								if (y != 0)
								{
									EmitHorizontalSpan(framebuffer, rightStart, rightEnd, rowTop);
									EmitHorizontalSpan(framebuffer, leftStart, leftEnd, rowTop);
								}
							}

							const dimension_t verticalStart = MaxValue<dimension_t>(y, innerStart);
							if (verticalStart <= outerEnd)
							{
								const wide_signed_t columnRight = static_cast<wide_signed_t>(centerX) +
									static_cast<wide_signed_t>(y);
								const wide_signed_t columnLeft = static_cast<wide_signed_t>(centerX) -
									static_cast<wide_signed_t>(y);
								const wide_signed_t bottomStart = static_cast<wide_signed_t>(centerY) +
									static_cast<wide_signed_t>(verticalStart);
								const wide_signed_t bottomEnd = static_cast<wide_signed_t>(centerY) +
									static_cast<wide_signed_t>(outerEnd);
								const wide_signed_t topStart = static_cast<wide_signed_t>(centerY) -
									static_cast<wide_signed_t>(outerEnd);
								const wide_signed_t topEnd = static_cast<wide_signed_t>(centerY) -
									static_cast<wide_signed_t>(verticalStart);

								EmitVerticalSpan(framebuffer, columnRight, bottomStart, bottomEnd);
								EmitVerticalSpan(framebuffer, columnRight, topStart, topEnd);
								if (y != 0)
								{
									EmitVerticalSpan(framebuffer, columnLeft, bottomStart, bottomEnd);
									EmitVerticalSpan(framebuffer, columnLeft, topStart, topEnd);
								}
							}

							if (y == outerX)
								break;

							y++;
							if (outerD <= 0)
								outerD += (2 * y) + 1;
							else
							{
								outerX--;
								outerD += (2 * (y - outerX)) + 1;
							}

							if (hasInner && y <= innerRadius)
							{
								if (innerD <= 0)
									innerD += (2 * y) + 1;
								else
								{
									innerX--;
									innerD += (2 * (y - innerX)) + 1;
								}
							}
						}
					}

					void CircleQuadrantRingFillImpl(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t outerRadius, const dimension_t innerRadius,
						const QuadrantEnum quadrant,
						const ArcBoundaryModeEnum arcBoundaryMode,
						const RingBoundaryModeEnum ringBoundaryMode)
					{

						// 2 octant calls for testing.
						OctantEnum firstOctant;
						OctantEnum secondOctant;

						switch (quadrant)
						{
						case QuadrantEnum::TopRight:
							firstOctant = OctantEnum::VerticalTopRight;
							secondOctant = OctantEnum::HorizontalTopRight;
							break;
						case QuadrantEnum::BottomRight:
							firstOctant = OctantEnum::HorizontalBottomRight;
							secondOctant = OctantEnum::VerticalBottomRight;
							break;
						case QuadrantEnum::BottomLeft:
							firstOctant = OctantEnum::VerticalBottomLeft;
							secondOctant = OctantEnum::HorizontalBottomLeft;
							break;
						case QuadrantEnum::TopLeft:
						default:
							firstOctant = OctantEnum::HorizontalTopLeft;
							secondOctant = OctantEnum::VerticalTopLeft;
							break;
						}

						const bool includeStart = arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStart &&
							arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd;
						const bool includeEnd = arcBoundaryMode != ArcBoundaryModeEnum::ExcludeEnd &&
							arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd;

						const ArcBoundaryModeEnum firstArcBoundaryMode = includeStart ? ArcBoundaryModeEnum::ExcludeEnd : ArcBoundaryModeEnum::ExcludeStartAndEnd;
						const ArcBoundaryModeEnum secondArcBoundaryMode = includeEnd ? ArcBoundaryModeEnum::Inclusive : ArcBoundaryModeEnum::ExcludeEnd;

						CircleOctantRingFill(framebuffer, centerX, centerY,
							outerRadius, innerRadius, firstOctant,
							firstArcBoundaryMode, ringBoundaryMode);
						CircleOctantRingFill(framebuffer, centerX, centerY,
							outerRadius, innerRadius, secondOctant,
							secondArcBoundaryMode, ringBoundaryMode);
						return;

						const bool right = quadrant == QuadrantEnum::TopRight || quadrant == QuadrantEnum::BottomRight;
						const bool bottom = quadrant == QuadrantEnum::BottomLeft || quadrant == QuadrantEnum::BottomRight;
						const int8_t xDirection = right ? 1 : -1;
						const int8_t yDirection = bottom ? 1 : -1;
						const bool hasInner = innerRadius > 0 && innerRadius < outerRadius;
						//const bool includeStart = arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStart &&
						//	arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd;
						//const bool includeEnd = arcBoundaryMode != ArcBoundaryModeEnum::ExcludeEnd &&
						//	arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd;

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

						const bool includeInnerBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeInner &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;
						const bool includeOuterBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeOuter &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;

						dimension_t outerX = outerRadius;
						dimension_t innerX = hasInner ? innerRadius : 0;
						dimension_t y = 0;
						signed_t outerD = signed_t(1) - outerRadius;
						signed_t innerD = hasInner ? signed_t(1) - innerRadius : 0;

						while (y <= outerX)
						{
							const dimension_t activeInnerX = hasInner && y <= innerRadius ? innerX : 0;
							const dimension_t innerStart = includeInnerBoundary
								? activeInnerX
								: static_cast<dimension_t>(activeInnerX + 1);
							const dimension_t outerEnd = includeOuterBoundary
								? outerX
								: static_cast<dimension_t>(outerX - 1);
							const dimension_t horizontalStart = MaxValue<dimension_t>(
								innerStart, static_cast<dimension_t>(y + 1));
							const dimension_t verticalStart = MaxValue<dimension_t>(y,
								innerStart);

							if ((includeHorizontalBoundary || y != 0) && horizontalStart <= outerEnd)
							{
								const pixel_t rightStart = (static_cast<pixel_t>(static_cast<wide_signed_t>(centerX) +
									static_cast<wide_signed_t>(horizontalStart) * xDirection));
								const pixel_t rightEnd = static_cast<pixel_t>(static_cast<wide_signed_t>(centerX) +
									static_cast<wide_signed_t>(outerEnd) * xDirection);
								const pixel_t bottomRow = static_cast<pixel_t>(static_cast<wide_signed_t>(centerY) +
									static_cast<wide_signed_t>(y) * yDirection);
								Base::Line(framebuffer, MinValue(rightStart, rightEnd), bottomRow,
									MaxValue(rightStart, rightEnd), bottomRow);
							}

							if ((includeVerticalBoundary || y != 0) && verticalStart <= outerEnd)
							{
								const pixel_t rightColumn = static_cast<pixel_t>(static_cast<wide_signed_t>(centerX) +
									static_cast<wide_signed_t>(y) * xDirection) - 0;
								const pixel_t firstRow = (static_cast<pixel_t>(static_cast<wide_signed_t>(centerY) +
									static_cast<wide_signed_t>(verticalStart) * yDirection));
								const pixel_t lastRow = static_cast<pixel_t>(static_cast<wide_signed_t>(centerY) +
									static_cast<wide_signed_t>(outerEnd) * yDirection);
								Base::Line(framebuffer, rightColumn, MinValue(firstRow, lastRow),
									rightColumn, MaxValue(firstRow, lastRow));
							}


							if (y == outerX)
								break;

							y++;
							if (outerD <= 0)
								outerD += (2 * y) + 1;
							else
							{
								outerX--;
								outerD += (2 * (y - outerX)) + 1;


							}
							if (hasInner && y <= innerRadius)
							{
								if (innerD <= 0)
									innerD += (2 * y) + 1;
								else
								{
									innerX--;
									innerD += (2 * (y - innerX)) + 1;
								}
							}
						}
					}

				};
			}
		}
	}
}

#endif