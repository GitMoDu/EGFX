// AbstractFrameBuffer.h

#ifndef _ABSTRACT_FRAME_BUFFER_h
#define _ABSTRACT_FRAME_BUFFER_h

#include "../Model/RgbColor.h"
#include "../Model/DisplayOrientation.h"
#include "../Model/ColorConverter.h"
#include "../Model/IFrameBuffer.h"
#include <IntegerSignal.h>

namespace Egfx
{
	/// <summary>
	/// Abstract frame buffer, with primitive implementation.
	/// Diagonal (Bresenham) Line implementation based on https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/
	/// </summary>
	/// <typeparam name="ColorConverter">Must be an implementation of AbstractColorConverter.</typeparam>
	/// <typeparam name="clearDivisorPower">Frame buffer clear will be divided into sections. The divisor is set by the power of 2, keeping it a multiple of 2.</typeparam>
	/// <typeparam name="frameWidth">Frame buffer width [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="frameHeight">Frame buffer height [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="displayAxis">Display mirror option.</typeparam>
	/// <typeparam name="displayRotation">Display rotation option.</typeparam>
	template<typename ColorConverter
		, const uint8_t clearDivisorPower
		, const pixel_t frameWidth
		, const pixel_t frameHeight
		, DisplayMirrorEnum displayMirror = DisplayMirrorEnum::NoMirror>
	class AbstractFrameBuffer : public IFrameBuffer
	{
	public:
		static constexpr pixel_t FrameWidth = frameWidth;
		static constexpr pixel_t FrameHeight = frameHeight;
		static constexpr DisplayMirrorEnum DisplayMirror = displayMirror;
		static constexpr size_t BufferSize = ColorConverter::GetBufferSize(FrameWidth, FrameHeight);

	protected:
		using color_t = typename ColorConverter::color_t;

		static constexpr size_t ClearStepByteCount()
		{
			return BufferSize / ClearStepsCount();
		}

		static constexpr uint8_t ClearStepsCount()
		{
			return (uint32_t)(1) << clearDivisorPower;
		}

	protected:
		uint8_t* Buffer;

	protected:
		uint8_t ClearIndex = 0;

		bool Inverted = 0;

	protected:
		virtual void PixelRaw(const color_t rawColor, const pixel_t x, const pixel_t y) {}

	public:
		AbstractFrameBuffer(uint8_t buffer[BufferSize] = nullptr)
			: IFrameBuffer()
			, Buffer(buffer)
		{
		}

	public:
		pixel_t GetFrameWidth() const final
		{
			return FrameWidth;
		}

		pixel_t GetFrameHeight() const final
		{
			return FrameHeight;
		}

		void SetInverted(const bool inverted) final
		{
			Inverted = inverted;
		}

		const uint8_t* GetFrameBuffer() final
		{
			return (const uint8_t*)Buffer;
		}

		bool ClearFrameBuffer() final
		{
			// Background color full, when inverted.
			if (Inverted)
			{
				memset(&Buffer[ClearStepByteCount() * ClearIndex], UINT8_MAX, ClearStepByteCount());
			}
			else
			{
				memset(&Buffer[ClearStepByteCount() * ClearIndex], 0, ClearStepByteCount());
			}

			ClearIndex++;

			if (ClearIndex >= ClearStepsCount())
			{
				ClearIndex = 0; // Ready for next clear.

				return true;
			}
			else
			{
				return false;
			}
		}

		void SetBuffer(uint8_t buffer[BufferSize])
		{
			Buffer = buffer;
		}

		void Pixel(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			if (x >= 0 && x < FrameWidth &&
				y >= 0 && y < FrameHeight)
			{
				const color_t rawColor = ColorConverter::GetRawColor(color);

				switch (DisplayMirror)
				{
				case DisplayMirrorEnum::MirrorX:
					PixelRaw(rawColor, MirrorX(x), y);
					break;
				case DisplayMirrorEnum::MirrorY:
					PixelRaw(rawColor, x, MirrorY(y));
					break;
				case DisplayMirrorEnum::MirrorXY:
					PixelRaw(rawColor, MirrorX(x), MirrorY(y));
					break;
				case DisplayMirrorEnum::NoMirror:
				default:
					PixelRaw(rawColor, x, y);
					break;
				}
			}
		}

		void Pixel(const rgb_color_t color, const pixel_point_t point) final
		{
			Pixel(color, point.x, point.y);
		}

		void Fill(const rgb_color_t color) final
		{
			const color_t rawColor = ColorConverter::GetRawColor(color);
			FillRaw(rawColor);
		}

		void Line(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2) final
		{
			const pixel_line_t line{ {x1, y1}, {x2, y2} };

			Line(color, line);
		}

		void Line(const rgb_color_t color, const pixel_line_t& line) final
		{
			const color_t rawColor = ColorConverter::GetRawColor(color);

			pixel_t startX;
			pixel_t startY;
			pixel_t endX;
			pixel_t endY;

			// Apply mirroring.
			switch (DisplayMirror)
			{
			case DisplayMirrorEnum::MirrorX:
				startX = MirrorX(line.start.x);
				endX = MirrorX(line.end.x);
				startY = line.start.y;
				endY = line.end.y;
				break;
			case DisplayMirrorEnum::MirrorY:
				startX = line.start.x;
				startY = MirrorY(line.start.y);
				endX = line.end.x;
				endY = MirrorY(line.end.y);
				break;
			case DisplayMirrorEnum::MirrorXY:
				startX = MirrorX(line.start.x);
				startY = MirrorY(line.start.y);
				endX = MirrorX(line.end.x);
				endY = MirrorY(line.end.y);
				break;
			case DisplayMirrorEnum::NoMirror:
			default:
				startX = line.start.x;
				startY = line.start.y;
				endX = line.end.x;
				endY = line.end.y;
				break;
			}

			if (startX == endX)
			{
				if (startX >= 0 && startX < FrameWidth)
				{
					if (startY == endY)
					{
						if (startY >= 0 && startY < FrameHeight)
						{
							PixelRaw(rawColor, startX, startY);
						}
					}
					else if (endY > startY)
					{
						startY = MaxValue((pixel_t)0, (pixel_t)(startY));
						endY = MinValue((pixel_t)(FrameHeight - 1), (pixel_t)(endY));
						LineVerticalRaw(rawColor, startX, startY, endY);
					}
					else
					{
						endY = MaxValue((pixel_t)0, (pixel_t)(endY));
						startY = MinValue((pixel_t)(FrameHeight - 1), (pixel_t)(startY));
						LineVerticalRaw(rawColor, startX, endY, startY);
					}
				}
			}
			else if (startY == endY)
			{
				if (startY >= 0 && startY < FrameHeight)
				{
					if (endX > startX)
					{
						startX = MaxValue((pixel_t)0, (pixel_t)(startX));
						endX = MinValue((pixel_t)(FrameWidth - 1), (pixel_t)(endX));
						if (endX > startX)
						{
							LineHorizontalRaw(rawColor, startX, startY, endX);
						}
						else if (endX == startX)
						{
							PixelRaw(rawColor, startX, startY);
						}
					}
					else
					{
						endX = MaxValue((pixel_t)0, (pixel_t)(endX));
						startX = MinValue((pixel_t)(FrameWidth - 1), (pixel_t)(startX));
						if (startX > endX)
						{
							LineHorizontalRaw(rawColor, endX, startY, startX);
						}
						else if (endX == startX)
						{
							PixelRaw(rawColor, startX, startY);
						}
					}
				}
			}
			else
			{
				// No bounds checking for BresenhamDiagonal
				BresenhamDiagonal(rawColor, { startX, startY }, { endX, endY });
			}
		}

		void RectangleFill(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2) final
		{
			// Integrity and bounds check.
			if (x1 > x2 || y1 > y2
				|| x2 < 0 || x1 >= FrameWidth
				|| y2 < 0 || y1 >= FrameHeight)
			{
				return;
			}

			const color_t rawColor = ColorConverter::GetRawColor(color);

			pixel_t mx1, mx2, my1, my2;

			// Apply mirroring.
			switch (DisplayMirror)
			{
			case DisplayMirrorEnum::MirrorX:
				mx1 = MirrorX(x2);
				mx2 = MirrorX(x1);
				my1 = y1;
				my2 = y2;
				break;
			case DisplayMirrorEnum::MirrorY:
				mx1 = x1;
				mx2 = x2;
				my1 = MirrorY(y2);
				my2 = MirrorY(y1);
				break;
			case DisplayMirrorEnum::MirrorXY:
				mx1 = MirrorX(x2);
				mx2 = MirrorX(x1);
				my1 = MirrorY(y2);
				my2 = MirrorY(y1);
				break;
			case DisplayMirrorEnum::NoMirror:
			default:
				mx1 = x1;
				mx2 = x2;
				my1 = y1;
				my2 = y2;
				break;
			}

			if (mx1 == mx2)
			{
				if (my1 == my2)
				{
					// Degenerate line, single pixel.
					PixelRaw(rawColor, mx1, my1);
				}
				else
				{
					// Apply bounds limiting for line.
					const pixel_t yStart = MaxValue((pixel_t)0, (pixel_t)(my1));
					const pixel_t yEnd = MinValue((pixel_t)(FrameHeight - 1), (pixel_t)(my2));
					if (yStart < yEnd)
					{
						LineVerticalRaw(rawColor, mx1, yStart, yEnd);
					}
				}
			}
			else if (my1 == my2)
			{
				// Apply bounds limiting for line.
				const pixel_t xStart = MaxValue((pixel_t)0, (pixel_t)(mx1));
				const pixel_t xEnd = MinValue((pixel_t)(FrameWidth - 1), (pixel_t)(mx2));
				if (xStart < xEnd)
				{
					LineHorizontalRaw(rawColor, xStart, my1, xEnd);
				}
			}
			else
			{
				// Apply bounds limiting for rectangle.
				const pixel_t xStart = MaxValue((pixel_t)0, (pixel_t)(mx1));
				const pixel_t yStart = MaxValue((pixel_t)0, (pixel_t)(my1));

				const pixel_t xEnd = MinValue((pixel_t)(FrameWidth - 1), (pixel_t)(mx2));
				const pixel_t yEnd = MinValue((pixel_t)(FrameHeight - 1), (pixel_t)(my2));

				if (xStart < xEnd
					&& yStart < yEnd)
				{
					RectangleFillRaw(rawColor, xStart, yStart, xEnd, yEnd);
				}
			}
		}

		void RectangleFill(const rgb_color_t color, const pixel_rectangle_t& rectangle) final
		{
			RectangleFill(color, rectangle.topLeft.x, rectangle.topLeft.y, rectangle.bottomRight.x, rectangle.bottomRight.y);
		}

		void Rectangle(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2) final
		{
			// Integrity and bounds check.
			if (x1 > x2 || y1 > y2
				|| x2 < 0 || x1 >= FrameWidth
				|| y2 < 0 || y1 >= FrameHeight)
			{
				return;
			}

			const color_t rawColor = ColorConverter::GetRawColor(color);

			pixel_t mx1, mx2, my1, my2;

			// Apply mirroring.
			switch (DisplayMirror)
			{
			case DisplayMirrorEnum::MirrorX:
				mx1 = MirrorX(x2);
				mx2 = MirrorX(x1);
				my1 = y1;
				my2 = y2;
				break;
			case DisplayMirrorEnum::MirrorY:
				mx1 = x1;
				mx2 = x2;
				my1 = MirrorY(y2);
				my2 = MirrorY(y1);
				break;
			case DisplayMirrorEnum::MirrorXY:
				mx1 = MirrorX(x2);
				mx2 = MirrorX(x1);
				my1 = MirrorY(y2);
				my2 = MirrorY(y1);
				break;
			case DisplayMirrorEnum::NoMirror:
			default:
				mx1 = x1;
				mx2 = x2;
				my1 = y1;
				my2 = y2;
				break;
			}

			// Apply bounds limiting for rectangle.
			const pixel_t xStart = MaxValue((pixel_t)0, (pixel_t)(mx1));
			const pixel_t yStart = MaxValue((pixel_t)0, (pixel_t)(my1 + 1));
			const pixel_t xEnd = MinValue((pixel_t)(FrameWidth - 1), (pixel_t)(mx2));
			const pixel_t yEnd = MinValue((pixel_t)(FrameHeight - 1), (pixel_t)(my2 - 1));

			if (xEnd >= xStart)
			{
				// Draw top horizontal line.
				if (my1 >= 0 && my1 < FrameHeight)
				{
					LineHorizontalRaw(rawColor, xStart, my1, xEnd);
				}

				// Draw bottom horizontal line.
				if (my2 >= 0 && my2 < FrameHeight)
				{
					LineHorizontalRaw(rawColor, xStart, my2, xEnd);
				}
			}

			if (yEnd >= yStart)
			{
				// Draw left vertical line.
				if (mx1 >= 0 && mx1 < FrameWidth)
				{
					LineVerticalRaw(rawColor, mx1, yStart, yEnd);
				}

				// Draw right vertical line.
				if (mx2 >= 0 && mx2 < FrameWidth)
				{
					LineVerticalRaw(rawColor, mx2, yStart, yEnd);
				}
			}
		}

		void Rectangle(const rgb_color_t color, const pixel_rectangle_t& rectangle) final
		{
			Rectangle(color, rectangle.topLeft.x, rectangle.topLeft.y, rectangle.bottomRight.x, rectangle.bottomRight.y);
		}

		void Triangle(const rgb_color_t color, const pixel_triangle_t& triangle) final
		{
			pixel_line_t line{ {triangle.a.x, triangle.a.y}, {triangle.b.x, triangle.b.y } };
			Line(color, line);
			line = { {triangle.b.x, triangle.b.y}, {triangle.c.x, triangle.c.y } };
			Line(color, line);
			line = { {triangle.a.x, triangle.a.y}, {triangle.c.x, triangle.c.y } };
			Line(color, line);
		}

		void Triangle(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2, const pixel_t x3, const pixel_t y3) final
		{
			const pixel_triangle_t triangle{ {x1,y1},{x2,y2},{x3,y3} };

			Triangle(color, triangle);
		}

		void TriangleFill(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2, const pixel_t x3, const pixel_t y3) final
		{
			const pixel_triangle_t triangle{ {x1,y1},{x2,y2},{x3,y3} };

			TriangleFill(color, triangle);
		}

		void TriangleFill(const rgb_color_t color, const pixel_triangle_t& triangle) final
		{
			const color_t rawColor = ColorConverter::GetRawColor(color);

			pixel_t ax;
			pixel_t ay;
			pixel_t bx;
			pixel_t by;
			pixel_t cx;
			pixel_t cy;

			// Apply mirroring
			switch (DisplayMirror)
			{
			case DisplayMirrorEnum::MirrorX:
				ax = MirrorX(triangle.a.x);
				ay = triangle.a.y;
				bx = MirrorX(triangle.b.x);
				by = triangle.b.y;
				cx = MirrorX(triangle.c.x);
				cy = triangle.c.y;
				break;
			case DisplayMirrorEnum::MirrorY:
				ax = triangle.a.x;
				ay = MirrorY(triangle.a.y);
				bx = triangle.b.x;
				by = MirrorY(triangle.b.y);
				cx = triangle.c.x;
				cy = MirrorY(triangle.c.y);
				break;
			case DisplayMirrorEnum::MirrorXY:
				ax = MirrorX(triangle.a.x);
				ay = MirrorY(triangle.a.y);
				bx = MirrorX(triangle.b.x);
				by = MirrorY(triangle.b.y);
				cx = MirrorX(triangle.c.x);
				cy = MirrorY(triangle.c.y);
				break;
			case DisplayMirrorEnum::NoMirror:
			default:
				ax = triangle.a.x;
				ay = triangle.a.y;
				bx = triangle.b.x;
				by = triangle.b.y;
				cx = triangle.c.x;
				cy = triangle.c.y;
				break;
			}

			if (ay <= by && ay <= cy)
			{
				// Vertex A is at the top.
				if (by <= cy)
				{
					TriangleYOrderedFill(rawColor, { ax, ay }, { bx, by }, { cx, cy });
				}
				else
				{
					TriangleYOrderedFill(rawColor, { ax, ay }, { cx, cy }, { bx, by });
				}
			}
			else if (by <= ay && by <= cy)
			{
				// Vertex B is at the top.
				if (ay <= cy)
				{
					TriangleYOrderedFill(rawColor, { bx, by }, { ax, ay }, { cx, cy });
				}
				else
				{
					TriangleYOrderedFill(rawColor, { bx, by }, { cx, cy }, { ax, ay });
				}
			}
			else
			{
				// Vertex C is at the top.
				if (ay <= by)
				{
					TriangleYOrderedFill(rawColor, { cx, cy }, { ax, ay }, { bx, by });
				}
				else
				{
					TriangleYOrderedFill(rawColor, { cx, cy }, { bx, by }, { ax, ay });
				}
			}
		}

	protected:
		virtual void LineVerticalRaw(const color_t rawColor, const pixel_t x, const pixel_t y1, const pixel_t y2)
		{
			for (pixel_t y = y1; y <= y2; y++)
			{
				PixelRaw(rawColor, x, y);
			}
		}

		virtual void LineHorizontalRaw(const color_t rawColor, const pixel_t x1, const pixel_t y, const pixel_t x2)
		{
			for (pixel_t x = x1; x <= x2; x++)
			{
				PixelRaw(rawColor, x, y);
			}
		}

		virtual void FillRaw(const color_t rawColor)
		{
			RectangleFillRaw(rawColor, 0, 0, FrameWidth - 1, FrameHeight - 1);
		}

		virtual void RectangleFillRaw(const color_t rawColor, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
		{
			for (pixel_t y = y1; y <= y2; y++)
			{
				LineHorizontalRaw(rawColor, x1, y, x2);
			}
		}

	private:
		pixel_t MirrorX(const pixel_t x) const { return FrameWidth - 1 - x; }
		pixel_t MirrorY(const pixel_t y) const { return FrameHeight - 1 - y; }

		void TriangleYOrderedFill(const color_t rawColor, const pixel_point_t a, const pixel_point_t b, const pixel_point_t c)
		{
			if (b.y == c.y) // Flat bottom.
			{
				BresenhamFlatBottomFill(rawColor, a, b, c);
			}
			else if (a.y == b.y) // Flat top.
			{
				BresenhamFlatTopFill(rawColor, a, b, c);
			}
			else // General triangle: split it.
			{
				// Calculate splitting vertex Vi.
				const pixel_t dxTotal = (pixel_t)c.x - (pixel_t)a.x;
				const pixel_t dyTotal = (pixel_t)c.y - (pixel_t)a.y;
				const pixel_t dySegment = (pixel_t)b.y - (pixel_t)a.y;

				if (dyTotal == 0)
					return; // Degenerate triangle

				// Calculate Vi_x in fixed-point.
				const pixel_t Vi_x = (((pixel_index_t)a.x << BRESENHAM_SCALE) + ((((pixel_index_t)dxTotal << BRESENHAM_SCALE) * dySegment) / dyTotal)) >> BRESENHAM_SCALE;
				const pixel_point_t Vi = { Vi_x, b.y };

				// Draw the two sub-triangles
				BresenhamFlatBottomFill(rawColor, a, b, Vi);
				BresenhamFlatTopFill(rawColor, b, Vi, c);
			}
		}

		void BresenhamFlatBottomFill(const color_t rawColor, const pixel_point_t a, const pixel_point_t b, const pixel_point_t c)
		{
			// Calculate inverse slopes in fixed-point
			pixel_index_t invSlope1 = 1;
			if (b.y != a.y)
			{
				invSlope1 = ((pixel_index_t)(b.x - a.x) << BRESENHAM_SCALE) / (b.y - a.y);
			}
			pixel_index_t invSlope2 = 1;
			if (c.y != a.y)
			{
				invSlope2 = ((pixel_index_t)(c.x - a.x) << BRESENHAM_SCALE) / (c.y - a.y);
			}

			// Starting x positions in fixed-point
			pixel_index_t x1 = (pixel_index_t)a.x << BRESENHAM_SCALE;
			pixel_index_t x2 = x1;

			// Loop from a.y to b.y (inclusive)
			const pixel_t startY = a.y;
			const pixel_t endY = min((pixel_t)(FrameHeight - 1), (pixel_t)b.y);

			for (pixel_t y = startY; y < endY; y++)
			{
				if (y >= 0 && y < FrameHeight)
				{
					pixel_t xStart{};
					pixel_t xEnd{};

					if (x1 > x2)
					{
						xStart = x2 >> BRESENHAM_SCALE;
						xEnd = x1 >> BRESENHAM_SCALE;
					}
					else
					{
						xStart = x1 >> BRESENHAM_SCALE;
						xEnd = x2 >> BRESENHAM_SCALE;
					}

					xStart = MaxValue(pixel_t(0), xStart);
					xEnd = MinValue(pixel_t(FrameWidth - 1), xEnd);

					if (xStart < xEnd)
					{
						LineHorizontalRaw(rawColor, xStart, y, xEnd);
					}
					else if (xStart == xEnd)
					{
						PixelRaw(rawColor, xStart, y);
					}
				}

				x1 += invSlope1;
				x2 += invSlope2;
			}
		}

		void BresenhamFlatTopFill(const color_t rawColor, const pixel_point_t a, const pixel_point_t b, const pixel_point_t c)
		{
			// Calculate inverse slopes in fixed-point
			const pixel_index_t invSlope1 = ((pixel_index_t)(c.x - a.x) << BRESENHAM_SCALE) / (c.y - a.y);
			const pixel_index_t invSlope2 = ((pixel_index_t)(c.x - b.x) << BRESENHAM_SCALE) / (c.y - b.y);

			// Starting x positions in fixed-point
			pixel_index_t x1 = (pixel_index_t)c.x << BRESENHAM_SCALE;
			pixel_index_t x2 = x1;

			// Loop from c.y down to a.y (inclusive)
			const pixel_t startY = c.y;
			const pixel_t endY = MaxValue(pixel_t(0), a.y);

			for (pixel_t y = startY; y >= endY; y--)
			{
				if (y >= 0 && y < FrameHeight)
				{
					pixel_t xStart{};
					pixel_t xEnd{};

					if (x1 > x2)
					{
						xStart = x2 >> BRESENHAM_SCALE;
						xEnd = x1 >> BRESENHAM_SCALE;
					}
					else
					{
						xStart = x1 >> BRESENHAM_SCALE;
						xEnd = x2 >> BRESENHAM_SCALE;
					}

					xStart = MaxValue(pixel_t(0), xStart);
					xEnd = MinValue(pixel_t(FrameWidth - 1), xEnd);

					if (xStart < xEnd)
					{
						LineHorizontalRaw(rawColor, xStart, y, xEnd);
					}
					else if (xStart == xEnd)
					{
						PixelRaw(rawColor, xStart, y);
					}
				}

				x1 -= invSlope1;
				x2 -= invSlope2;
			}
		}

		void BresenhamDiagonal(const color_t rawColor, const pixel_point_t start, const pixel_point_t end)
		{
			if (end.x > start.x)
			{
				if (end.y > start.y)
				{
					if (end.x - start.x > end.y - start.y)
					{
						BresenhamRight(rawColor, start, end);
					}
					else
					{
						BresenhamUp(rawColor, start, end);
					}
				}
				else if (end.x - start.x > start.y - end.y)
				{
					BresenhamRight(rawColor, start, end);
				}
				else
				{
					BresenhamUp(rawColor, end, start);
				}
			}
			else if (end.y > start.y)
			{
				if (start.x - end.x > end.y - start.y)
				{
					BresenhamRight(rawColor, end, start);
				}
				else
				{
					BresenhamUp(rawColor, start, end);
				}
			}
			else
			{
				if (start.x - end.x > start.y - end.y)
				{
					BresenhamRight(rawColor, end, start);
				}
				else
				{
					BresenhamUp(rawColor, end, start);
				}
			}
		}

		void BresenhamRight(const color_t rawColor, const pixel_point_t start, const pixel_point_t end)
		{
			const pixel_t xEnd = MinValue((pixel_t)(FrameWidth - 1), (pixel_t)(end.x));

			const pixel_t scaledWidth = (end.x - start.x) << 1;
			const pixel_index_t slopeMagnitude = AbsValue(end.y - start.y) << 1;
			const int8_t slopeUnit = (end.y >= start.y) ? 1 : -1;

			pixel_index_t slopeError = slopeMagnitude - (end.x - start.x);
			pixel_t y = start.y;


			for (pixel_t x = start.x; x <= xEnd; x++)
			{
				if (x >= 0 && x < FrameWidth &&
					y >= 0 && y < FrameHeight)
				{
					PixelRaw(rawColor, x, y);
				}

				slopeError += slopeMagnitude;
				if (slopeError >= 0)
				{
					y += slopeUnit;
					slopeError -= scaledWidth;
				}
			}
		}

		void BresenhamUp(const color_t rawColor, const pixel_point_t start, const pixel_point_t end)
		{
			const pixel_t yEnd = MinValue((pixel_t)(FrameHeight - 1), (pixel_t)(end.y));

			const pixel_t scaledHeight = (end.y - start.y) << 1;
			const pixel_t slopeMagnitude = AbsValue(end.x - start.x) << 1;
			const int8_t slopeUnit = (end.x >= start.x) ? 1 : -1;

			pixel_index_t slopeError = (pixel_index_t)slopeMagnitude - (end.y - start.y);
			pixel_t x = start.x;

			for (pixel_t y = start.y; y <= yEnd; y++)
			{
				if (x >= 0 && x < FrameWidth &&
					y >= 0 && y < FrameHeight)
				{
					PixelRaw(rawColor, x, y);
				}

				slopeError += slopeMagnitude;
				if (slopeError >= 0)
				{
					x += slopeUnit;
					slopeError -= scaledHeight;
				}
			}
		}
	};
}
#endif