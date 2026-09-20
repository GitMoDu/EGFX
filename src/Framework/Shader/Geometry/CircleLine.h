#ifndef _INTEGERGLASS_FRAMEWORK_SHADER_GEOMETRY_CIRCLE_LINE_h
#define _INTEGERGLASS_FRAMEWORK_SHADER_GEOMETRY_CIRCLE_LINE_h

#include "Rectangle.h"

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
				class CircleLineShader : public PrimitiveShaderType
				{
				private:
					using Base = PrimitiveShaderType;

					// Wide enough to sum the squares of the maximum dimension_t values without overflow.
					using signed_t = typename AutoDimension::ByDimension<dimension_t>::signed_wide_t;

				public:
					CircleLineShader(const dimension_t left, const dimension_t top,
						const dimension_t right, const dimension_t bottom)
						: Base(left, top, right, bottom) {}
					~CircleLineShader() = default;


					/// <summary>
					/// Draws a circle line using Bresenham's circle algorithm.
					/// Reference algorithm for the rest of the Circle*Shader classes.
					/// </summary>
					/// <param name="framebuffer">The framebuffer to draw on.</param>
					/// <param name="centerX">The x-coordinate of the circle's center.</param>
					/// <param name="centerY">The y-coordinate of the circle's center.</param>
					/// <param name="radius">The radius of the circle.</param>
					void CircleLine(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t radius)
					{
						// Handle and dispatch edge cases for small circles.
						switch (radius)
						{
						case 0:
							// A radius of 0 is a single pixel at the center.
							Base::Pixel(framebuffer, centerX, centerY);
							return;
						case 1:
							// A radius of 1 is a cross shape of 4 pixels.
							Base::Pixel(framebuffer, centerX, centerY - 1);
							Base::Pixel(framebuffer, centerX + 1, centerY);
							Base::Pixel(framebuffer, centerX, centerY + 1);
							Base::Pixel(framebuffer, centerX - 1, centerY);
							return;
						default:
							break;
						}

						// Draw the four cardinal points of the circle.
						Base::Pixel(framebuffer, centerX + radius, centerY);
						Base::Pixel(framebuffer, centerX - radius, centerY);
						Base::Pixel(framebuffer, centerX, centerY + radius);
						Base::Pixel(framebuffer, centerX, centerY - radius);

						dimension_t x = radius;
						dimension_t y = 0;
						signed_t error = static_cast<signed_t>(1) - radius;

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
								x--;
								error += ((static_cast<signed_t>(y) - x) * 2) + 1;
							}

							// Emits the 8 symmetric outline points for a circle step at(x, y) relative to the center.
							if (y < x)
							{
								Base::Pixel(framebuffer, centerX + x, centerY + y);
								Base::Pixel(framebuffer, centerX - x, centerY + y);
								Base::Pixel(framebuffer, centerX + x, centerY - y);
								Base::Pixel(framebuffer, centerX - x, centerY - y);
							}

							Base::Pixel(framebuffer, centerX + y, centerY + x);
							Base::Pixel(framebuffer, centerX - y, centerY + x);
							Base::Pixel(framebuffer, centerX + y, centerY - x);
							Base::Pixel(framebuffer, centerX - y, centerY - x);
						}

					}

					void CircleOctantLine(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t radius, const OctantEnum octant,
						const ArcBoundaryModeEnum arcBoundaryMode = ArcBoundaryModeEnum::Inclusive)
					{
						if (radius <= 0)
							return;

						const bool swapCoordinates = octant == OctantEnum::VerticalBottomRight ||
							octant == OctantEnum::VerticalBottomLeft ||
							octant == OctantEnum::VerticalTopLeft ||
							octant == OctantEnum::VerticalTopRight;
						const int8_t xDirection =
							octant == OctantEnum::VerticalBottomLeft ||
							octant == OctantEnum::HorizontalBottomLeft ||
							octant == OctantEnum::HorizontalTopLeft ||
							octant == OctantEnum::VerticalTopLeft ? -1 : 1;
						const int8_t yDirection =
							octant == OctantEnum::HorizontalTopLeft ||
							octant == OctantEnum::VerticalTopLeft ||
							octant == OctantEnum::VerticalTopRight ||
							octant == OctantEnum::HorizontalTopRight ? -1 : 1;
						const bool includeStart = arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStart &&
							arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd;
						const bool includeEnd = arcBoundaryMode != ArcBoundaryModeEnum::ExcludeEnd &&
							arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd;
						const bool isEvenOctant = (static_cast<uint8_t>(octant) & 1) == 0;
						const bool includeAxis = isEvenOctant ? includeStart : includeEnd;

						dimension_t x = radius;
						dimension_t y = 0;
						signed_t error = static_cast<signed_t>(1) - radius;

						// Draw the axis/start point separately from the midpoint walk.
						if (includeAxis)
						{
							if (swapCoordinates)
							{
								Base::Pixel(framebuffer, centerX + (static_cast<signed_t>(y) * xDirection),
									centerY + (static_cast<signed_t>(x) * yDirection));
							}
							else
							{
								Base::Pixel(framebuffer, centerX + (static_cast<signed_t>(x) * xDirection),
									centerY + (static_cast<signed_t>(y) * yDirection));
							}
						}

						while (y < x)
						{
							y++;
							if (error <= 0)
								error += (static_cast<signed_t>(y) * 2) + 1;
							else
							{
								x--;
								error += (static_cast<signed_t>(y - x) * 2) + 1;
							}

							if (y < x)
							{
								if (swapCoordinates)
								{
									Base::Pixel(framebuffer, centerX + (static_cast<signed_t>(y) * xDirection),
										centerY + (static_cast<signed_t>(x) * yDirection));
								}
								else
								{
									Base::Pixel(framebuffer, centerX + (static_cast<signed_t>(x) * xDirection),
										centerY + (static_cast<signed_t>(y) * yDirection));
								}
							}
						}

						// The midpoint walk can finish on the diagonal or pass it for very small radii.
						const bool includeDiagonal = isEvenOctant ? includeEnd : includeStart;
						if (includeDiagonal && y == x)
						{
							Base::Pixel(framebuffer, centerX + (static_cast<signed_t>(x) * xDirection),
								centerY + (static_cast<signed_t>(y) * yDirection));
						}
					}

					void CircleQuadrantLine(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t radius, const QuadrantEnum quadrant,
						const ArcBoundaryModeEnum arcBoundaryMode = ArcBoundaryModeEnum::Inclusive)
					{
						if (radius <= 0)
							return;

						const bool swapCoordinates = quadrant == QuadrantEnum::TopRight ||
							quadrant == QuadrantEnum::BottomLeft;
						const int8_t xDirection =
							quadrant == QuadrantEnum::BottomLeft || quadrant == QuadrantEnum::TopLeft ? -1 : 1;
						const int8_t yDirection =
							quadrant == QuadrantEnum::TopRight || quadrant == QuadrantEnum::TopLeft ? -1 : 1;
						const bool firstBranchIsStart = quadrant != QuadrantEnum::BottomRight;

						// Draw the quadrant endpoints separately from the midpoint walk.

						bool includeStart = arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStart
							&& arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd;
						bool includeEnd = arcBoundaryMode != ArcBoundaryModeEnum::ExcludeEnd
							&& arcBoundaryMode != ArcBoundaryModeEnum::ExcludeStartAndEnd;

						const bool quadrantFlip = quadrant == QuadrantEnum::BottomRight || quadrant == QuadrantEnum::TopLeft;
						if (quadrantFlip)
						{
							bool temp = includeStart;
							includeStart = includeEnd;
							includeEnd = temp;
						}

						if (firstBranchIsStart ? includeStart : includeEnd)
							Base::Pixel(framebuffer,
								centerX + (swapCoordinates ? 0 : (static_cast<signed_t>(radius) * xDirection)),
								centerY + (swapCoordinates ? (static_cast<signed_t>(radius) * yDirection) : 0));
						if (firstBranchIsStart ? includeEnd : includeStart)
							Base::Pixel(framebuffer,
								centerX + (swapCoordinates ? (static_cast<signed_t>(radius) * xDirection) : 0),
								centerY + (swapCoordinates ? 0 : (static_cast<signed_t>(radius) * yDirection)));

						dimension_t x = radius;
						dimension_t y = 0;
						signed_t error = static_cast<signed_t>(1) - radius;

						while (y < x)
						{
							y++;
							if (error <= 0)
								error += (static_cast<signed_t>(y) * 2) + 1;
							else
							{
								x--;
								error += (static_cast<signed_t>(y - x) * 2) + 1;
							}

							if (y < x)
							{
								Base::Pixel(framebuffer,
									centerX + (static_cast<signed_t>(swapCoordinates ? y : x) * xDirection),
									centerY + (static_cast<signed_t>(swapCoordinates ? x : y) * yDirection));
								Base::Pixel(framebuffer,
									centerX + (static_cast<signed_t>(swapCoordinates ? x : y) * xDirection),
									centerY + (static_cast<signed_t>(swapCoordinates ? y : x) * yDirection));
							}
						}

						// The two quadrant branches share one diagonal pixel.
						if (y == x)
						{
							Base::Pixel(framebuffer, centerX + (static_cast<signed_t>(x) * xDirection),
								centerY + (static_cast<signed_t>(y) * yDirection));
						}
					}

				protected:
					void CircleOctantLineLimited(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t radius,
						const dimension_t startLimit, const dimension_t endLimit,
						const OctantEnum octant)
					{
						if (radius <= 0 || startLimit > endLimit)
						{
							return;
						}
						const dimension_t actualEndLimit = endLimit < radius ? endLimit : radius;
						const dimension_t actualStartLimit = startLimit < radius ? startLimit : radius;

						if (actualEndLimit < actualStartLimit)
						{
							// No pixels to draw.
							return;
						}

						dimension_t x = radius;
						dimension_t y = 0;
						signed_t error = static_cast<signed_t>(1) - radius;

						// Fast-forward the midpoint algorithm to the first requested step.
						while (y < actualStartLimit)
						{
							y++;
							if (error <= 0)
								error += (static_cast<signed_t>(y) * 2) + 1;
							else
							{
								x--;
								error += (static_cast<signed_t>(y - x) * 2) + 1;
							}
						}

						const bool swapCoordinates = octant == OctantEnum::VerticalBottomRight ||
							octant == OctantEnum::VerticalBottomLeft ||
							octant == OctantEnum::VerticalTopLeft ||
							octant == OctantEnum::VerticalTopRight;
						const int8_t xDirection =
							octant == OctantEnum::VerticalBottomLeft ||
							octant == OctantEnum::HorizontalBottomLeft ||
							octant == OctantEnum::HorizontalTopLeft ||
							octant == OctantEnum::VerticalTopLeft ? -1 : 1;
						const int8_t yDirection =
							octant == OctantEnum::HorizontalTopLeft ||
							octant == OctantEnum::VerticalTopLeft ||
							octant == OctantEnum::VerticalTopRight ||
							octant == OctantEnum::HorizontalTopRight ? -1 : 1;

						// Draw the first requested step, including the axis or diagonal when selected.
						Base::Pixel(framebuffer,
							centerX + (static_cast<signed_t>(swapCoordinates ? y : x) * xDirection),
							centerY + (static_cast<signed_t>(swapCoordinates ? x : y) * yDirection));

						while (y < actualEndLimit)
						{
							y++;
							if (error <= 0)
								error += (static_cast<signed_t>(y) * 2) + 1;
							else
							{
								x--;
								error += (static_cast<signed_t>(y - x) * 2) + 1;
							}

							Base::Pixel(framebuffer,
								centerX + (static_cast<signed_t>(swapCoordinates ? y : x) * xDirection),
								centerY + (static_cast<signed_t>(swapCoordinates ? x : y) * yDirection));
						}
					}
				};
			}
		}
	}
}

#endif