#ifndef _INTEGERGLASS_FRAMEWORK_SHADER_GEOMETRY_CIRCLE_ARC_h
#define _INTEGERGLASS_FRAMEWORK_SHADER_GEOMETRY_CIRCLE_ARC_h

#include "CircleRing.h"
#include <IntegerTrigonometry8.h>
#include <IntegerTrigonometry16.h>


namespace IntegerGlass
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
				class CircleArcShader : public CircleRingShader<dimension_t, PrimitiveShaderType>
				{
				private:
					using Base = CircleRingShader<dimension_t, PrimitiveShaderType>;

					using signed_t = typename AutoDimension::ByDimension<dimension_t>::signed_t;
					using wide_signed_t = typename AutoDimension::ByDimension<dimension_t>::signed_wide_t;

				public:
					CircleArcShader(const dimension_t left, const dimension_t top,
						const dimension_t right, const dimension_t bottom)
						: Base(left, top, right, bottom) {}
					~CircleArcShader() = default;

					void CircleArcLine(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const angle_t startAngle, const angle_t endAngle,
						const dimension_t radius,
						const ArcBoundaryModeEnum arcBoundaryMode = ArcBoundaryModeEnum::Inclusive)
					{
						if (radius <= 0)
							return;

						const angle_t angleDelta = endAngle - startAngle;
						if (angleDelta == ANGLE_RANGE)
						{
							Base::CircleLine(framebuffer, centerX, centerY, radius);
							return;
						}

						const angle_t start = static_cast<angle_t>(startAngle) % ANGLE_RANGE;
						const angle_t end = static_cast<angle_t>(endAngle) % ANGLE_RANGE;

						if (start == end)
							return;

						const angle_t sweep = (end + ANGLE_RANGE - start) % ANGLE_RANGE;
						if (sweep == 0)
						{
							return;
						}

						angle_t remaining = sweep;
						angle_t segmentStart = start;

						while (remaining > 0)
						{
							const angle_t offsetInOctant = segmentStart % ANGLE_EIGHTH_CIRCLE;
							const angle_t distanceToNextBoundary = (offsetInOctant == 0) ? ANGLE_EIGHTH_CIRCLE : (ANGLE_EIGHTH_CIRCLE - offsetInOctant);
							const angle_t segmentSweep = MinValue(remaining, distanceToNextBoundary);
							const angle_t segmentEnd = (segmentStart + segmentSweep) % ANGLE_RANGE;
							const bool isFirstSegment = segmentStart == start;
							const bool isLastSegment = segmentSweep == remaining;
							const uint8_t octantIndex = static_cast<uint8_t>((segmentStart / ANGLE_EIGHTH_CIRCLE) % 8);
							const OctantEnum octant = static_cast<OctantEnum>(static_cast<uint8_t>((octantIndex + 6) % 8));

							if (segmentSweep == ANGLE_QUARTER_CIRCLE && offsetInOctant == 0 &&
								arcBoundaryMode == ArcBoundaryModeEnum::Inclusive)
							{
								Base::CircleQuadrantLine(framebuffer, centerX, centerY, radius,
									static_cast<QuadrantEnum>((segmentStart / ANGLE_QUARTER_CIRCLE) % 4));
							}
							else if (segmentSweep == ANGLE_EIGHTH_CIRCLE && offsetInOctant == 0 &&
								(arcBoundaryMode == ArcBoundaryModeEnum::Inclusive || (!isFirstSegment && !isLastSegment)))
							{
								Base::CircleOctantLine(framebuffer, centerX, centerY, radius, octant);
							}
							else
							{
								const angle_t localStart = (octantIndex & 1) == 0
									? offsetInOctant
									: static_cast<angle_t>(ANGLE_EIGHTH_CIRCLE - offsetInOctant);
								const angle_t localEndOffset = static_cast<angle_t>(offsetInOctant + segmentSweep);
								const angle_t localEnd = (octantIndex & 1) == 0
									? localEndOffset
									: static_cast<angle_t>(ANGLE_EIGHTH_CIRCLE - localEndOffset);
								dimension_t limitStart;
								dimension_t limitEnd;
								if (sizeof(dimension_t) == 1)
								{
									limitStart = Fraction8::Fraction<dimension_t>(Trigonometry::Sine8(localStart), radius);
									limitEnd = Fraction8::Fraction<dimension_t>(Trigonometry::Sine8(localEnd), radius);
								}
								else
								{
									limitStart = Fraction16::Fraction<dimension_t>(Trigonometry::Sine16(localStart), radius);
									limitEnd = Fraction16::Fraction<dimension_t>(Trigonometry::Sine16(localEnd), radius);
								}
								const bool increasing = limitStart <= limitEnd;
								const bool excludeStart = isFirstSegment &&
									(arcBoundaryMode == ArcBoundaryModeEnum::ExcludeStart || arcBoundaryMode == ArcBoundaryModeEnum::ExcludeStartAndEnd);
								const bool excludeEnd = isLastSegment &&
									(arcBoundaryMode == ArcBoundaryModeEnum::ExcludeEnd || arcBoundaryMode == ArcBoundaryModeEnum::ExcludeStartAndEnd);

								if (excludeStart)
									increasing ? ++limitStart : --limitStart;
								if (excludeEnd)
									increasing ? --limitEnd : ++limitEnd;

								Base::CircleOctantLineLimited(framebuffer, centerX, centerY, radius,
									static_cast<dimension_t>(MinValue(limitStart, limitEnd)),
									static_cast<dimension_t>(MaxValue(limitStart, limitEnd)), octant);
							}

							segmentStart = segmentEnd;
							remaining -= segmentSweep;
						}
					}


					void CircleArcFill(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const angle_t startAngle, const angle_t endAngle,
						const dimension_t outerRadius, const dimension_t innerRadius = 0,
						const ArcBoundaryModeEnum arcBoundaryMode = ArcBoundaryModeEnum::Inclusive,
						const RingBoundaryModeEnum ringBoundaryMode = RingBoundaryModeEnum::Inclusive)
					{
						const angle_t angleDelta = endAngle - startAngle;

						if (outerRadius <= 0 || innerRadius >= outerRadius || angleDelta <= 0)
							return;

						if (angleDelta == ANGLE_RANGE)
						{
							Base::CircleRingFill(framebuffer, centerX, centerY,
								outerRadius, innerRadius,
								ringBoundaryMode);
							return;
						}
						else if ((innerRadius + 1) == outerRadius)
						{
							CircleArcLine(framebuffer, centerX, centerY,
								startAngle, endAngle,
								outerRadius, arcBoundaryMode);
							return;
						}

						const angle_t start = static_cast<angle_t>(startAngle);

						angle_t segmentStart = start;
						angle_t remaining = angleDelta;
						bool leadingSegmentReversed = false;

						const bool excludeStart = arcBoundaryMode == ArcBoundaryModeEnum::ExcludeStart ||
							arcBoundaryMode == ArcBoundaryModeEnum::ExcludeStartAndEnd;

						const bool excludeEnd = arcBoundaryMode == ArcBoundaryModeEnum::ExcludeEnd ||
							arcBoundaryMode == ArcBoundaryModeEnum::ExcludeStartAndEnd;

						const angle_t endExclusive = static_cast<angle_t>(start + angleDelta);
						const uint8_t startOctantIndex = static_cast<uint8_t>((start / ANGLE_EIGHTH_CIRCLE) % 8);
						const uint8_t lastOctantIndex = static_cast<uint8_t>(((endExclusive - 1) / ANGLE_EIGHTH_CIRCLE) % 8);

						if ((angleDelta < ANGLE_EIGHTH_CIRCLE) && startOctantIndex == lastOctantIndex &&
							(start % ANGLE_EIGHTH_CIRCLE) != 0)
						{
							// Entire arc segment is within a single octant. Use the specialized fill function for this case.
							const angle_t endOffsetInOctant = static_cast<angle_t>(endExclusive % ANGLE_EIGHTH_CIRCLE);
							CircleOctantRingFillBetween(framebuffer, centerX, centerY,
								outerRadius, innerRadius,
								startOctantIndex,
								static_cast<angle_t>(start % ANGLE_EIGHTH_CIRCLE),
								endOffsetInOctant == 0 ? ANGLE_EIGHTH_CIRCLE : endOffsetInOctant,
								excludeStart, excludeEnd, ringBoundaryMode);
							return;
						}

						// Leading partial octant.
						// If startAngle is not on an exact octant boundary, consume only the
						// remainder of that octant.
						const angle_t offsetInOctant = static_cast<angle_t>(segmentStart % ANGLE_EIGHTH_CIRCLE);
						if (offsetInOctant != 0)
						{
							const angle_t segmentSweep = MinValue(remaining, static_cast<angle_t>(ANGLE_EIGHTH_CIRCLE - offsetInOctant));

							const bool isLastSegment = segmentSweep == remaining;

							const ArcBoundaryModeEnum segmentBoundary = excludeStart
								? (isLastSegment && excludeEnd
									? ArcBoundaryModeEnum::ExcludeStartAndEnd
									: ArcBoundaryModeEnum::ExcludeStart)
								: (isLastSegment && excludeEnd
									? ArcBoundaryModeEnum::ExcludeEnd
									: ArcBoundaryModeEnum::Inclusive);

							const uint8_t octantIndex = static_cast<uint8_t>((segmentStart / ANGLE_EIGHTH_CIRCLE) % 8);

							const OctantEnum octant = static_cast<OctantEnum>((octantIndex + 6) % 8);
							const ArcBoundaryModeEnum generalizedBoundary = (octantIndex & 1) == 0
								? ReverseArcBoundaryMode(segmentBoundary)
								: segmentBoundary;

							const angle_t localBoundary = (octantIndex & 1) == 0
								? offsetInOctant
								: static_cast<angle_t>(ANGLE_EIGHTH_CIRCLE - offsetInOctant);

							int16_t ratioNumerator;
							int16_t ratioDenominator;
							if (sizeof(dimension_t) == 1)
							{
								ratioNumerator = Trigonometry::Sine8(localBoundary);
								ratioDenominator = Trigonometry::Sine8(localBoundary + ANGLE_QUARTER_CIRCLE);
							}
							else
							{
								ratioNumerator = Trigonometry::Sine16(localBoundary);
								ratioDenominator = Trigonometry::Sine16(localBoundary + ANGLE_QUARTER_CIRCLE);
							}

							CircleOctantRingFillGeneralizedBidirectional(framebuffer, centerX, centerY,
								outerRadius, innerRadius,
								octant,
								ratioNumerator, ratioDenominator,
								generalizedBoundary, ringBoundaryMode,
								true
							);

							leadingSegmentReversed = true;

							segmentStart = static_cast<angle_t>((segmentStart + segmentSweep) % ANGLE_RANGE);
							remaining -= segmentSweep;

							if (remaining == 0)
								return;
						}


						// Full octants / quadrants. At this point segmentStart is on an octant boundary.
						while (remaining >= ANGLE_EIGHTH_CIRCLE)
						{
							const uint8_t octantIndex = static_cast<uint8_t>((segmentStart / ANGLE_EIGHTH_CIRCLE) % 8);
							const bool useQuadrant = (octantIndex & 1) == 0 && remaining >= ANGLE_QUARTER_CIRCLE;
							const angle_t wholeSegmentSweep = useQuadrant ? ANGLE_QUARTER_CIRCLE : ANGLE_EIGHTH_CIRCLE;
							const bool isFirstWholeSegment = segmentStart == start;
							const bool isLastWholeSegment = remaining == wholeSegmentSweep;
							const bool excludeWholeStart = leadingSegmentReversed ||
								(isFirstWholeSegment && excludeStart);
							const bool excludeWholeEnd = leadingSegmentReversed ||
								(!isLastWholeSegment || excludeEnd);
							const ArcBoundaryModeEnum wholeSegmentBoundary = excludeWholeStart
								? (excludeWholeEnd ? ArcBoundaryModeEnum::ExcludeStartAndEnd : ArcBoundaryModeEnum::ExcludeStart)
								: (excludeWholeEnd ? ArcBoundaryModeEnum::ExcludeEnd : ArcBoundaryModeEnum::Inclusive);

							// Every second octant boundary is also a quadrant boundary.
							if (useQuadrant)
							{
								const QuadrantEnum quadrant = static_cast<QuadrantEnum>((octantIndex >> 1) & 3);

								if (innerRadius == 0)
								{
									Base::CircleQuadrantFill(framebuffer, centerX, centerY,
										outerRadius,
										quadrant,
										wholeSegmentBoundary, ringBoundaryMode);
								}
								else
								{
									Base::CircleQuadrantRingFill(framebuffer, centerX, centerY,
										outerRadius, innerRadius,
										quadrant,
										wholeSegmentBoundary, ringBoundaryMode);
								}

								segmentStart = static_cast<angle_t>((segmentStart + ANGLE_QUARTER_CIRCLE) % ANGLE_RANGE);
								remaining -= ANGLE_QUARTER_CIRCLE;
							}
							else
							{
								const OctantEnum octant = static_cast<OctantEnum>((octantIndex + 6) % 8);

								Base::CircleOctantRingFill(framebuffer, centerX, centerY,
									outerRadius, innerRadius,
									octant,
									wholeSegmentBoundary, ringBoundaryMode);

								segmentStart = static_cast<angle_t>((segmentStart + ANGLE_EIGHTH_CIRCLE) % ANGLE_RANGE);
								remaining -= ANGLE_EIGHTH_CIRCLE;
							}

							leadingSegmentReversed = false;
						}

						// Trailing partial octant.
						if (remaining > 0)
						{
							const uint8_t octantIndex = static_cast<uint8_t>((segmentStart / ANGLE_EIGHTH_CIRCLE) % 8);

							const OctantEnum octant = static_cast<OctantEnum>((octantIndex + 6) % 8);

							const angle_t localEnd = (octantIndex & 1) == 0
								? remaining
								: static_cast<angle_t>(ANGLE_EIGHTH_CIRCLE - remaining);

							const ArcBoundaryModeEnum segmentBoundary = leadingSegmentReversed
								? (excludeEnd
									? ArcBoundaryModeEnum::ExcludeStartAndEnd
									: ArcBoundaryModeEnum::ExcludeStart)
								: (excludeEnd
									? ArcBoundaryModeEnum::ExcludeEnd
									: ArcBoundaryModeEnum::Inclusive);

							int16_t ratioNumerator;
							int16_t ratioDenominator;
							if (sizeof(dimension_t) == 1)
							{
								ratioNumerator = Trigonometry::Sine8(localEnd);
								ratioDenominator = Trigonometry::Sine8(localEnd + ANGLE_QUARTER_CIRCLE);
							}
							else
							{
								ratioNumerator = Trigonometry::Sine16(localEnd);
								ratioDenominator = Trigonometry::Sine16(localEnd + ANGLE_QUARTER_CIRCLE);
							}

							CircleOctantRingFillGeneralizedBidirectional(framebuffer, centerX, centerY,
								outerRadius, innerRadius,
								octant,
								ratioNumerator, ratioDenominator,
								segmentBoundary, ringBoundaryMode);
						}
					}


				protected:
					static ArcBoundaryModeEnum ReverseArcBoundaryMode(const ArcBoundaryModeEnum arcBoundaryMode)
					{
						switch (arcBoundaryMode)
						{
						case ArcBoundaryModeEnum::ExcludeStart:
							return ArcBoundaryModeEnum::ExcludeEnd;
						case ArcBoundaryModeEnum::ExcludeEnd:
							return ArcBoundaryModeEnum::ExcludeStart;
						default:
							return arcBoundaryMode;
						}
					}

					void DrawTranslatedOctantSpan(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const OctantEnum octant, const bool horizontal,
						const wide_signed_t minor, const wide_signed_t majorStart,
						const wide_signed_t majorEnd)
					{
						const int8_t xDirection = octant == OctantEnum::VerticalBottomLeft ||
							octant == OctantEnum::HorizontalBottomLeft ||
							octant == OctantEnum::HorizontalTopLeft ||
							octant == OctantEnum::VerticalTopLeft ? -1 : 1;
						const int8_t yDirection = octant == OctantEnum::HorizontalTopLeft ||
							octant == OctantEnum::VerticalTopLeft ||
							octant == OctantEnum::VerticalTopRight ||
							octant == OctantEnum::HorizontalTopRight ? -1 : 1;

						if (horizontal)
						{
							const pixel_t row = static_cast<pixel_t>(static_cast<wide_signed_t>(centerY) + minor * yDirection);
							const pixel_t first = static_cast<pixel_t>(static_cast<wide_signed_t>(centerX) + majorStart * xDirection);
							const pixel_t last = static_cast<pixel_t>(static_cast<wide_signed_t>(centerX) + majorEnd * xDirection);
							Base::RectangleFill(framebuffer, MinValue(first, last), row, MaxValue(first, last), row);
						}
						else
						{
							const pixel_t column = static_cast<pixel_t>(static_cast<wide_signed_t>(centerX) + minor * xDirection);
							const pixel_t first = static_cast<pixel_t>(static_cast<wide_signed_t>(centerY) + majorStart * yDirection);
							const pixel_t last = static_cast<pixel_t>(static_cast<wide_signed_t>(centerY) + majorEnd * yDirection);
							Base::RectangleFill(framebuffer, column, MinValue(first, last), column, MaxValue(first, last));
						}
					}

					void CircleOctantRingFillBetween(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t outerRadius, const dimension_t innerRadius,
						const uint8_t octantIndex,
						const angle_t startOffset, const angle_t endOffset,
						const bool excludeStart, const bool excludeEnd,
						const RingBoundaryModeEnum ringBoundaryMode)
					{
						const OctantEnum octant = static_cast<OctantEnum>((octantIndex + 6) % 8);
						const bool horizontal = octant == OctantEnum::HorizontalBottomRight ||
							octant == OctantEnum::HorizontalBottomLeft ||
							octant == OctantEnum::HorizontalTopLeft ||
							octant == OctantEnum::HorizontalTopRight;
						const bool isEvenOctant = (octantIndex & 1) == 0;
						const bool includeStart = !excludeStart;
						const bool includeEnd = !excludeEnd;
						const bool includeInnerBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeInner &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;
						const bool includeOuterBoundary = ringBoundaryMode != RingBoundaryModeEnum::ExcludeOuter &&
							ringBoundaryMode != RingBoundaryModeEnum::ExcludeInnerAndOuter;

						const angle_t localStart = isEvenOctant
							? startOffset
							: static_cast<angle_t>(ANGLE_EIGHTH_CIRCLE - startOffset);
						const angle_t localEnd = isEvenOctant
							? endOffset
							: static_cast<angle_t>(ANGLE_EIGHTH_CIRCLE - endOffset);

						signed_t startNumerator;
						signed_t startDenominator;
						signed_t endNumerator;
						signed_t endDenominator;
						if (sizeof(dimension_t) == 1)
						{
							startNumerator = Fraction8::Fraction<signed_t>(Trigonometry::Sine8(localStart), outerRadius);
							startDenominator = Fraction8::Fraction<signed_t>(Trigonometry::Sine8(localStart + ANGLE_QUARTER_CIRCLE), outerRadius);
							endNumerator = Fraction8::Fraction<signed_t>(Trigonometry::Sine8(localEnd), outerRadius);
							endDenominator = Fraction8::Fraction<signed_t>(Trigonometry::Sine8(localEnd + ANGLE_QUARTER_CIRCLE), outerRadius);
						}
						else
						{
							startNumerator = Fraction16::Fraction<signed_t>(Trigonometry::Sine16(localStart), outerRadius);
							startDenominator = Fraction16::Fraction<signed_t>(Trigonometry::Sine16(localStart + ANGLE_QUARTER_CIRCLE), outerRadius);
							endNumerator = Fraction16::Fraction<signed_t>(Trigonometry::Sine16(localEnd), outerRadius);
							endDenominator = Fraction16::Fraction<signed_t>(Trigonometry::Sine16(localEnd + ANGLE_QUARTER_CIRCLE), outerRadius);
						}

						const signed_t lowerNumerator = isEvenOctant ? endNumerator : startNumerator;
						const signed_t lowerDenominator = isEvenOctant ? endDenominator : startDenominator;
						const bool includeLower = isEvenOctant ? includeEnd : includeStart;
						const signed_t upperNumerator = isEvenOctant ? startNumerator : endNumerator;
						const signed_t upperDenominator = isEvenOctant ? startDenominator : endDenominator;
						const bool includeUpper = isEvenOctant ? includeStart : includeEnd;

						dimension_t outerX = outerRadius;
						dimension_t innerX = innerRadius;
						dimension_t y = 0;
						signed_t outerD = signed_t(1) - outerRadius;
						signed_t innerD = innerRadius > 0 ? signed_t(1) - innerRadius : 0;

						while (y <= outerX)
						{
							const dimension_t activeInnerX = innerRadius > 0 && y <= innerRadius ? innerX : 0;
							const wide_signed_t innerStart = includeInnerBoundary
								? static_cast<wide_signed_t>(activeInnerX)
								: static_cast<wide_signed_t>(activeInnerX) + 1;
							const wide_signed_t outerEnd = includeOuterBoundary
								? static_cast<wide_signed_t>(outerX)
								: static_cast<wide_signed_t>(outerX) - 1;
							wide_signed_t start = MaxValue<wide_signed_t>(innerStart, static_cast<wide_signed_t>(y));
							wide_signed_t end = outerEnd;

							if (lowerNumerator != 0)
							{
								const wide_signed_t product = static_cast<wide_signed_t>(y) * lowerDenominator;
								const wide_signed_t limit = product / lowerNumerator;
								start = MaxValue<wide_signed_t>(start,
									limit + ((product % lowerNumerator != 0 || !includeLower) ? 1 : 0));
							}

							if (upperNumerator != 0)
							{
								const wide_signed_t product = static_cast<wide_signed_t>(y) * upperDenominator;
								const wide_signed_t limit = product / upperNumerator;
								end = MinValue<wide_signed_t>(end,
									limit - ((!includeUpper && product % upperNumerator == 0) ? 1 : 0));
							}

							if (start <= end)
								DrawTranslatedOctantSpan(framebuffer, centerX, centerY, octant, horizontal,
									static_cast<wide_signed_t>(y), start, end);

							y++;
							if (outerD <= 0)
								outerD += (2 * y) + 1;
							else
							{
								outerX--;
								outerD += (2 * (y - outerX)) + 1;
							}
							if (innerRadius > 0 && y <= innerRadius)
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

					/// <summary>
					/// Partial octant ring fill with a ratio boundary. 
					/// The ratio is defined as the fraction of the minor axis to the major axis, 
					/// and is used to determine the end point of the arc within the octant.
					/// 
					/// </summary>
					/// <param name="framebuffer">The framebuffer to draw into.</param>
					/// <param name="centerX">The X coordinate of the center of the circle.</param>
					/// <param name="centerY">The Y coordinate of the center of the circle.</param>
					/// <param name="outerRadius">The outer radius of the ring.</param>
					/// <param name="innerRadius">The inner radius of the ring.</param>
					/// <param name="octant">The octant to fill.</param>
					/// <param name="ratioNumerator">The numerator of the ratio defining the end point of the arc within the octant.</param>
					/// <param name="ratioDenominator">The denominator of the ratio defining the end point of the arc within the octant.</param>
					/// <param name="arcBoundaryMode">The boundary mode for the arc.</param>
					/// <param name="ringBoundaryMode">The boundary mode for the ring.</param>
					void CircleOctantRingFillGeneralizedBidirectional(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t outerRadius, const dimension_t innerRadius,
						const OctantEnum octant,
						const int16_t ratioNumerator, const int16_t ratioDenominator,
						const ArcBoundaryModeEnum arcBoundaryMode = ArcBoundaryModeEnum::Inclusive,
						const RingBoundaryModeEnum ringBoundaryMode = RingBoundaryModeEnum::Inclusive,
						const bool reverse = false)
					{
						if (outerRadius <= 0 || innerRadius > outerRadius)
							return;

						const bool horizontal = octant == OctantEnum::HorizontalBottomRight ||
							octant == OctantEnum::HorizontalBottomLeft ||
							octant == OctantEnum::HorizontalTopLeft ||
							octant == OctantEnum::HorizontalTopRight;
						const bool includeStart = arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStart &&
							arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd;
						const bool includeEnd = arcBoundaryMode != ArcBoundaryModeEnum::ExcludeEnd &&
							arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd;
						const bool isEvenOctant = (static_cast<uint8_t>(octant) & 1) == 0;
						const bool axisIsStart = isEvenOctant != reverse;
						const bool includeAxis = isEvenOctant
							? (reverse ? false : includeStart)
							: (reverse ? includeStart : includeEnd);
						const bool includeDiagonal = isEvenOctant
							? (reverse ? includeEnd : false)
							: (reverse ? false : includeStart);

						// Even octants start at the axis; odd octants start at the ratio boundary
						const bool axisToRatio = axisIsStart;

						// If axisToRatio is true, ratio is the end boundary; otherwise, it's the start boundary
						const bool includeRatio = axisToRatio ? includeEnd : includeStart;
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

						while (y <= outerX)
						{
							const dimension_t activeInnerX = hasInner && y <= innerRadius ? innerX : 0;
							const wide_signed_t innerStart = includeInnerBoundary
								? static_cast<wide_signed_t>(activeInnerX)
								: static_cast<wide_signed_t>(activeInnerX) + 1;
							const wide_signed_t outerEnd = includeOuterBoundary
								? static_cast<wide_signed_t>(outerX)
								: static_cast<wide_signed_t>(outerX) - 1;

							const int8_t diagonalOffset = includeDiagonal ? 0 : 1;
							const wide_signed_t baseStart = MaxValue<dimension_t>(innerStart, y + diagonalOffset);

							if (horizontal)
							{
								wide_signed_t start = baseStart;
								wide_signed_t end = outerEnd;
								if (ratioNumerator == 0)
								{
									if (y != 0 && axisToRatio)
										start = outerEnd + 1;
								}
								else
								{
									const wide_signed_t product = static_cast<wide_signed_t>(y) * ratioDenominator;
									const wide_signed_t ratio = product / ratioNumerator;
									if (axisToRatio)
										start = MaxValue<wide_signed_t>(baseStart, ratio + ((product % ratioNumerator != 0 || !includeRatio) ? 1 : 0));
									else
										end = MinValue<wide_signed_t>(outerEnd, ratio - ((!includeRatio && product % ratioNumerator == 0) ? 1 : 0));
								}

								if ((includeAxis || y != 0) && start <= end)
								{
									DrawTranslatedOctantSpan(framebuffer, centerX, centerY, octant, true,
										static_cast<wide_signed_t>(y), start, end);
								}
							}
							else
							{
								wide_signed_t start = baseStart;
								wide_signed_t end = outerEnd;

								if (ratioNumerator != 0)
								{
									const wide_signed_t product = static_cast<wide_signed_t>(y) * ratioDenominator;
									const wide_signed_t ratio = product / ratioNumerator;
									if (axisToRatio)
									{
										start = MaxValue<wide_signed_t>(baseStart, ratio + ((product % ratioNumerator != 0 || !includeRatio) ? 1 : 0));
									}
									else
									{
										end = MinValue<wide_signed_t>(outerEnd, ratio - ((!includeRatio && product % ratioNumerator == 0) ? 1 : 0));
									}
								}
								else if (axisToRatio && y != 0)
								{
									start = outerEnd + 1;
								}

								if ((includeAxis || y != 0) && start <= end)
								{
									DrawTranslatedOctantSpan(framebuffer, centerX, centerY, octant, false,
										static_cast<wide_signed_t>(y), start, end);
								}
							}

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