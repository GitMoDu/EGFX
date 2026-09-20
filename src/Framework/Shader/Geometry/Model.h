#ifndef _INTEGERGLASS_FRAMEWORK_SHADER_GEOMETRY_MODEL_h
#define _INTEGERGLASS_FRAMEWORK_SHADER_GEOMETRY_MODEL_h

#include "../Pixel/Model.h"

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Geometry
			{
				struct LineClipPoint
				{
					int16_t x;
					int16_t y;
				};

				class LineClipper
				{
				public:
					/// <summary>
					/// Clips an integer line segment to half-open bounds: [left, right) and [top, bottom).
					/// </summary>
					template<typename dimension_t>
					static bool Clip(LineClipPoint& start, LineClipPoint& end,
						const dimension_t left, const dimension_t top,
						const dimension_t right, const dimension_t bottom)
					{
						if (left >= right || top >= bottom)
							return false;

						const int16_t clipLeft = static_cast<int16_t>(left);
						const int16_t clipTop = static_cast<int16_t>(top);
						const int16_t clipRight = static_cast<int16_t>(right - 1);
						const int16_t clipBottom = static_cast<int16_t>(bottom - 1);

						for (;;)
						{
							const uint8_t startCode = OutCode(start, clipLeft, clipTop, clipRight, clipBottom);
							const uint8_t endCode = OutCode(end, clipLeft, clipTop, clipRight, clipBottom);
							if ((startCode | endCode) == 0) return true;
							if ((startCode & endCode) != 0) return false;

							const uint8_t code = startCode != 0 ? startCode : endCode;
							LineClipPoint point{};
							if ((code & 8) != 0)
							{
								point.y = clipBottom;
								point.x = Interpolate(start.x, end.x, clipBottom - start.y, end.y - start.y);
							}
							else if ((code & 4) != 0)
							{
								point.y = clipTop;
								point.x = Interpolate(start.x, end.x, clipTop - start.y, end.y - start.y);
							}
							else if ((code & 2) != 0)
							{
								point.x = clipRight;
								point.y = Interpolate(start.y, end.y, clipRight - start.x, end.x - start.x);
							}
							else
							{
								point.x = clipLeft;
								point.y = Interpolate(start.y, end.y, clipLeft - start.x, end.x - start.x);
							}

							if (code == startCode) start = point;
							else end = point;
						}
					}

				private:
					static uint8_t OutCode(const LineClipPoint point,
						const int16_t left, const int16_t top,
						const int16_t right, const int16_t bottom)
					{
						return static_cast<uint8_t>((point.x < left ? 1 : 0) | (point.x > right ? 2 : 0) |
							(point.y < top ? 4 : 0) | (point.y > bottom ? 8 : 0));
					}

					static int16_t Interpolate(const int16_t start, const int16_t end,
						const int16_t numerator, const int16_t denominator)
					{
						using wide_t = typename IntegerSignal::TypeTraits::TypeNext::next_int_type<int16_t>::type;
						return static_cast<int16_t>(start + static_cast<wide_t>(end - start) * numerator / denominator);
					}
				};

				enum class LineBoundaryModeEnum : uint8_t
				{
					Inclusive,
					ExcludeStart,
					ExcludeEnd,
					ExcludeStartAndEnd
				};

				enum class ArcBoundaryModeEnum : uint8_t
				{
					Inclusive,
					ExcludeStart,
					ExcludeEnd,
					ExcludeStartAndEnd
				};

				enum class RingBoundaryModeEnum : uint8_t
				{
					Inclusive,
					ExcludeInner,
					ExcludeOuter,
					ExcludeInnerAndOuter
				};

				enum class HalfEnum : uint8_t
				{
					Top,
					Right,
					Bottom,
					Left
				};


				enum class QuadrantEnum : uint8_t
				{
					TopRight,
					BottomRight,
					BottomLeft,
					TopLeft
				};

				/// <summary>
				/// Identifies one of the eight screen-space octants of a circle.
				/// Angles use the library convention: 0 degrees is up and angles increase clockwise.
				/// The suffix identifies the midpoint branch used by the octant rasterizer.
				/// </summary>
				enum class OctantEnum : uint8_t
				{
					/// <summary>Bottom-right, approximately 90 to 135 degrees; horizontal branch.</summary>
					HorizontalBottomRight = 0,

					/// <summary>Bottom-right, approximately 135 to 180 degrees; vertical branch.</summary>
					VerticalBottomRight = 1,

					/// <summary>Bottom-left, approximately 180 to 225 degrees; vertical branch.</summary>
					VerticalBottomLeft = 2,

					/// <summary>Bottom-left, approximately 225 to 270 degrees; horizontal branch.</summary>
					HorizontalBottomLeft = 3,

					/// <summary>Top-left, approximately 270 to 315 degrees; horizontal branch.</summary>
					HorizontalTopLeft = 4,

					/// <summary>Top-left, approximately 315 to 360 degrees; vertical branch.</summary>
					VerticalTopLeft = 5,

					/// <summary>Top-right, approximately 0 to 45 degrees; vertical branch.</summary>
					VerticalTopRight = 6,

					/// <summary>Top-right, approximately 45 to 90 degrees; horizontal branch.</summary>
					HorizontalTopRight = 7
				};
			}
		}
	}


}
#endif