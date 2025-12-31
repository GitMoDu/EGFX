#ifndef _EGFX_TEMPLATE_FRAME_BUFFER_h
#define _EGFX_TEMPLATE_FRAME_BUFFER_h

#include "../Model/RgbColor.h"
#include "../Model/DisplayOptions.h"

#if defined(ARDUINO_ARCH_RP2040)
#include "hardware/dma.h"
#endif

namespace Egfx
{
	/// <summary>
	/// Generic framebuffer template providing 2D drawing, blending, and buffer management for any color depth and display configuration.
	/// Supports mirroring, inversion, anti-aliasing, and partial buffer clearing via template parameters.
	/// Used as the core implementation for all framebuffer types in Egfx.
	/// </summary>
	/// <typeparam name="FramePainter">The base class providing low-level drawing and buffer access functionality.</typeparam>
	/// <typeparam name="clearDivisorPower">An unsigned 8-bit integer specifying the power-of-two divisor for buffer clearing steps.</typeparam>
	/// <typeparam name="displayOptions">Display configuration options (mirror, rotation, inverted colors, AA).</typeparam>
	template<typename FramePainter
		, uint8_t clearDivisorPower
		, typename displayOptions = DisplayOptions::Default>
	class TemplateFramebuffer : public FramePainter
	{
	private:
		enum class TransformCase : uint8_t
		{
			Identity,
			MirrorX,
			MirrorY,
			MirrorXY,
			Rotate90,
			Rotate90MirrorX,
			Rotate90MirrorY,
			Rotate90MirrorXY,
			Rotate180,
			Rotate180MirrorX,
			Rotate180MirrorY,
		};

	public:
		using FramePainter::BufferSize;
		using FramePainter::FrameWidth;
		using FramePainter::FrameHeight;

		using typename FramePainter::color_t;

		using Configuration = displayOptions;

	protected:
		using FramePainter::IntToFixed;
		using FramePainter::FixedRoundToInt;
		using FramePainter::ClipRectangle;
		using FramePainter::ClipLine;

#if defined(ARDUINO_ARCH_RP2040)
		// Static fill constant lives for the lifetime of the program; safe as DMA source.
		const uint32_t FillZero32 = 0x00000000u;
		const uint32_t FillOnes32 = 0xFFFFFFFFu;

		// Async DMA clear state
		int DmaChan = -1;
		bool DmaActive = false;
#else
		static constexpr uint8_t ClearStepsCount = uint8_t(1) << clearDivisorPower;
#endif

	protected:
		using FramePainter::Buffer;

	private:
		uint8_t ClearIndex = 0;

	public:
		TemplateFramebuffer(uint8_t buffer[BufferSize] = nullptr)
			: FramePainter(buffer)
		{
		}

	public:
		virtual bool Flip()
		{
			// No double-buffering support by default.
			return false;
		}

		uint8_t* GetFrameBuffer() const
		{
			return Buffer;
		}

		bool ClearFrameBuffer() final
		{
#if defined(ARDUINO_ARCH_RP2040)
			return AsyncClearBuffer();
#else
			// The template keyword is required in this context to disambiguate between a static method and a template method.
			FramePainter::template ClearRaw<displayOptions::Inverted, ClearStepsCount>(ClearIndex);

			ClearIndex++;

			if (ClearIndex >= ClearStepsCount)
			{
				ClearIndex = 0; // Ready for next clear.

				return true;
			}
			else
			{
				return false;
			}
#endif
		}

		virtual void SetBuffer(uint8_t buffer[BufferSize])
		{
			Buffer = buffer;
		}

		void Pixel(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			Pixel(color, pixel_point_t{ x, y });
		}

		void Pixel(const rgb_color_t color, const pixel_point_t point) final
		{
			if (point.x >= 0 && point.x < FrameWidth &&
				point.y >= 0 && point.y < FrameHeight)
			{
				const pixel_point_t transformed = TransformCoordinates(point);
				FramePainter::PixelRaw(GetRawColor(color), transformed.x, transformed.y);
			}
		}

		void PixelBlend(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			PixelBlend(color, pixel_point_t{ x, y });
		}

		void PixelBlend(const rgb_color_t color, const pixel_point_t point) final
		{
			if (point.x >= 0 && point.x < FrameWidth &&
				point.y >= 0 && point.y < FrameHeight)
			{
				const pixel_point_t transformed = TransformCoordinates(point);
				FramePainter::PixelRawBlend(GetRawColor(color), transformed.x, transformed.y);
			}
		}

		void PixelBlendAlpha(const rgb_color_t color, const pixel_t x, const pixel_t y, const uint8_t alpha) final
		{
			PixelBlendAlpha(color, pixel_point_t{ x, y }, alpha);
		}

		void PixelBlendAlpha(const rgb_color_t color, const pixel_point_t point, const uint8_t alpha) final
		{
			if (alpha == UINT8_MAX / 2)
			{
				PixelBlend(color, point);
			}
			else if (alpha > 0 &&
				point.x >= 0 && point.x < FrameWidth &&
				point.y >= 0 && point.y < FrameHeight)
			{
				const color_t uninvertedColor = FramePainter::GetRawColor(color);
				const color_t rawColor = displayOptions::Inverted
					? (uninvertedColor & 0xFF000000) | (~uninvertedColor & 0x00FFFFFF)
					: uninvertedColor;

				const pixel_point_t transformed = TransformCoordinates(point);
				FramePainter::PixelRawBlendAlpha(rawColor, transformed.x, transformed.y, alpha);
			}
		}

		void PixelBlendAdd(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			PixelBlendAdd(color, pixel_point_t{ x, y });
		}

		void PixelBlendAdd(const rgb_color_t color, const pixel_point_t point) final
		{
			if (point.x >= 0 && point.x < FrameWidth &&
				point.y >= 0 && point.y < FrameHeight)
			{
				const pixel_point_t transformed = TransformCoordinates(point);
				FramePainter::PixelRawBlendAdd(GetRawColor(color), transformed.x, transformed.y);
			}
		}

		void PixelBlendSubtract(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			PixelBlendSubtract(color, pixel_point_t{ x, y });
		}

		void PixelBlendSubtract(const rgb_color_t color, const pixel_point_t point) final
		{
			if (point.x >= 0 && point.x < FrameWidth &&
				point.y >= 0 && point.y < FrameHeight)
			{
				const pixel_point_t transformed = TransformCoordinates(point);
				FramePainter::PixelRawBlendSubtract(GetRawColor(color), transformed.x, transformed.y);
			}
		}

		void PixelBlendMultiply(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			PixelBlendMultiply(color, pixel_point_t{ x, y });
		}

		void PixelBlendMultiply(const rgb_color_t color, const pixel_point_t point) final
		{
			if (point.x >= 0 && point.x < FrameWidth &&
				point.y >= 0 && point.y < FrameHeight)
			{
				const pixel_point_t transformed = TransformCoordinates(point);
				FramePainter::PixelRawBlendMultiply(GetRawColor(color), transformed.x, transformed.y);
			}
		}

		void PixelBlendScreen(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			PixelBlendScreen(color, pixel_point_t{ x, y });
		}

		void PixelBlendScreen(const rgb_color_t color, const pixel_point_t point) final
		{
			if (point.x >= 0 && point.x < FrameWidth &&
				point.y >= 0 && point.y < FrameHeight)
			{
				const pixel_point_t transformed = TransformCoordinates(point);
				FramePainter::PixelRawBlendScreen(GetRawColor(color), transformed.x, transformed.y);
			}
		}

		void Line(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2) final
		{
			const pixel_line_t line{ {x1, y1}, {x2, y2} };
			Line(color, line);
		}

		void Line(const rgb_color_t color, const pixel_line_t& line) final
		{
			pixel_point_t start = line.start;
			pixel_point_t end = line.end;

			if (!ClipLine(start, end))
			{
				return;
			}

			const color_t rawColor = GetRawColor(color);

			start = TransformCoordinates(start);
			end = TransformCoordinates(end);

			if (start.x == end.x)
			{
				if (start.y == end.y)
				{
					FramePainter::PixelRaw(rawColor, start.x, start.y);
				}
				else
				{
					FramePainter::LineVerticalRaw(rawColor, start.x, start.y, end.y);
				}
			}
			else if (start.y == end.y)
			{
				FramePainter::LineHorizontalRaw(rawColor, start.x, start.y, end.x);
			}
			else
			{
				BresenhamDiagonal(rawColor, start, end);
			}
		}

		void RectangleFill(const rgb_color_t color,
			const pixel_t topLeftX, const pixel_t topLeftY,
			const pixel_t bottomRightX, const pixel_t bottomRightY) final
		{
			RectangleFill(color, pixel_rectangle_t{ {topLeftX, topLeftY}, {bottomRightX, bottomRightY} });
		}

		void RectangleFill(const rgb_color_t color, const pixel_rectangle_t& rectangle) final
		{
			pixel_point_t topLeft = { (rectangle.topLeft.x <= rectangle.bottomRight.x) ? rectangle.topLeft.x : rectangle.bottomRight.x,
				(rectangle.topLeft.y <= rectangle.bottomRight.y) ? rectangle.topLeft.y : rectangle.bottomRight.y };
			pixel_point_t bottomRight = { (rectangle.topLeft.x <= rectangle.bottomRight.x) ? rectangle.bottomRight.x : rectangle.topLeft.x,
				(rectangle.topLeft.y <= rectangle.bottomRight.y) ? rectangle.bottomRight.y : rectangle.topLeft.y };

			if (!ClipRectangle(topLeft, bottomRight))
			{
				return;
			}

			const color_t rawColor = GetRawColor(color);

			// Transform all four corners to find actual bounding box after rotation
			pixel_point_t tl = TransformCoordinates(topLeft);
			pixel_point_t tr = TransformCoordinates({ bottomRight.x, topLeft.y });
			pixel_point_t bl = TransformCoordinates({ topLeft.x, bottomRight.y });
			pixel_point_t br = TransformCoordinates(bottomRight);

			// Find min/max across all four corners
			pixel_t minX = MinValue(MinValue(tl.x, tr.x), MinValue(bl.x, br.x));
			pixel_t maxX = MaxValue(MaxValue(tl.x, tr.x), MaxValue(bl.x, br.x));
			pixel_t minY = MinValue(MinValue(tl.y, tr.y), MinValue(bl.y, br.y));
			pixel_t maxY = MaxValue(MaxValue(tl.y, tr.y), MaxValue(bl.y, br.y));

			if (minX == maxX)
			{
				if (minY == maxY)
				{
					FramePainter::PixelRaw(rawColor, minX, minY);
				}
				else
				{
					FramePainter::LineVerticalRaw(rawColor, minX, minY, maxY);
				}
			}
			else if (minY == maxY)
			{
				FramePainter::LineHorizontalRaw(rawColor, minX, minY, maxX);
			}
			else
			{
				FramePainter::RectangleFillRaw(rawColor, minX, minY, maxX, maxY);
			}
		}

		void Rectangle(const rgb_color_t color, const pixel_t topLeftX, const pixel_t topLeftY,
			const pixel_t bottomRightX, const pixel_t bottomRightY) final
		{
			Rectangle(color, pixel_rectangle_t{ {topLeftX, topLeftY}, {bottomRightX, bottomRightY} });
		}

		void Rectangle(const rgb_color_t color, const pixel_rectangle_t& rectangle) final
		{
			const pixel_t minX = (rectangle.topLeft.x <= rectangle.bottomRight.x) ? rectangle.topLeft.x : rectangle.bottomRight.x;
			const pixel_t maxX = (rectangle.topLeft.x <= rectangle.bottomRight.x) ? rectangle.bottomRight.x : rectangle.topLeft.x;
			const pixel_t minY = (rectangle.topLeft.y <= rectangle.bottomRight.y) ? rectangle.topLeft.y : rectangle.bottomRight.y;
			const pixel_t maxY = (rectangle.topLeft.y <= rectangle.bottomRight.y) ? rectangle.bottomRight.y : rectangle.topLeft.y;

			pixel_point_t topLeft{ minX, minY };
			pixel_point_t bottomRight{ maxX, maxY };

			if (!ClipRectangle(topLeft, bottomRight))
			{
				return;
			}

			// Use logical corners to draw all four edges; Line will handle transform/clipping per edge.
			Line(color, pixel_line_t{ topLeft, { bottomRight.x, topLeft.y } });
			Line(color, pixel_line_t{ { bottomRight.x, topLeft.y }, bottomRight });
			Line(color, pixel_line_t{ bottomRight, { topLeft.x, bottomRight.y } });
			Line(color, pixel_line_t{ { topLeft.x, bottomRight.y }, topLeft });
		}

		void Triangle(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2, const pixel_t x3, const pixel_t y3) final
		{
			Triangle(color, pixel_triangle_t{ {x1, y1}, {x2, y2}, {x3, y3} });
		}

		void Triangle(const rgb_color_t color, const pixel_triangle_t& triangle) final
		{
			Line(color, triangle.a.x, triangle.a.y, triangle.b.x, triangle.b.y);
			Line(color, triangle.b.x, triangle.b.y, triangle.c.x, triangle.c.y);
			Line(color, triangle.a.x, triangle.a.y, triangle.c.x, triangle.c.y);
		}

		void TriangleFill(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2, const pixel_t x3, const pixel_t y3) final
		{
			TriangleFill(color, pixel_triangle_t{ {x1, y1}, {x2, y2}, {x3, y3} });
		}

		void TriangleFill(const rgb_color_t color, const pixel_triangle_t& triangle) final
		{
			pixel_point_t a = TransformCoordinates(triangle.a);
			pixel_point_t b = TransformCoordinates(triangle.b);
			pixel_point_t c = TransformCoordinates(triangle.c);

			const color_t rawColor = GetRawColor(color);

			if (a.y <= b.y && a.y <= c.y)
			{
				if (b.y <= c.y)
				{
					TriangleYOrderedFill(rawColor, a, b, c);
				}
				else
				{
					TriangleYOrderedFill(rawColor, a, c, b);
				}
			}
			else if (b.y <= a.y && b.y <= c.y)
			{
				if (a.y <= c.y)
				{
					TriangleYOrderedFill(rawColor, b, a, c);
				}
				else
				{
					TriangleYOrderedFill(rawColor, b, c, a);
				}
			}
			else
			{
				if (a.y <= b.y)
				{
					TriangleYOrderedFill(rawColor, c, a, b);
				}
				else
				{
					TriangleYOrderedFill(rawColor, c, b, a);
				}
			}
		}

		void Fill(const rgb_color_t color) final
		{
			FramePainter::FillRaw(GetRawColor(color));
		}

	private:
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
				const pixel_t dxTotal = static_cast<pixel_t>(c.x - a.x);
				const pixel_t dyTotal = static_cast<pixel_t>(c.y - a.y);
				const pixel_t dySegment = static_cast<pixel_t>(b.y - a.y);

				if (dyTotal == 0)
					return; // Degenerate triangle

				// Calculate Vi_x in fixed-point.
				const pixel_t Vi_x = FixedRoundToInt(IntToFixed(a.x) + (((IntToFixed(dxTotal) * dySegment) / dyTotal)));
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
				invSlope1 = IntToFixed(b.x - a.x) / (b.y - a.y);
			}
			pixel_index_t invSlope2 = 1;
			if (c.y != a.y)
			{
				invSlope2 = IntToFixed(c.x - a.x) / (c.y - a.y);
			}

			// Starting x positions in fixed-point
			pixel_index_t x1 = IntToFixed(a.x);
			pixel_index_t x2 = x1;

			pixel_t xSide1{};
			pixel_t xSide2{};

			// Loop from a.y to b.y (inclusive)
			for (pixel_t y = a.y; y <= b.y; y++)
			{
				xSide1 = LimitValue<pixel_t>(FixedRoundToInt(x1), 0, FramePainter::PhysicalWidth - 1);
				xSide2 = LimitValue<pixel_t>(FixedRoundToInt(x2), 0, FramePainter::PhysicalWidth - 1);

				if (xSide1 == xSide2)
				{
					FramePainter::PixelRaw(rawColor, xSide1, y);
				}
				else
				{
					FramePainter::LineHorizontalRaw(rawColor, xSide1, y, xSide2);
				}

				switch (displayOptions::AntiAliasing)
				{
				case DisplayOptions::AntiAliasingEnum::EdgeBlend:
				{
					TriangleEdgeAntiAliasingEdgeBlend(rawColor, xSide1, xSide2, y);
				}
				break;
				case DisplayOptions::AntiAliasingEnum::PixelCoverage:
				{
					TriangleEdgeAntiAliasingPixelCoverage(rawColor, x1, x2, xSide1, xSide2, y);
				}
				break;
				case DisplayOptions::AntiAliasingEnum::None:
				default:
					break;
				}

				x1 += invSlope1;
				x2 += invSlope2;
			}
		}

		void BresenhamFlatTopFill(const color_t rawColor, const pixel_point_t a, const pixel_point_t b, const pixel_point_t c)
		{
			// Calculate inverse slopes in fixed-point
			const pixel_index_t invSlope1 = IntToFixed(c.x - a.x) / (c.y - a.y);
			const pixel_index_t invSlope2 = IntToFixed(c.x - b.x) / (c.y - b.y);

			// Starting x positions in fixed-point
			pixel_index_t x1 = IntToFixed(c.x);
			pixel_index_t x2 = x1;

			pixel_t xSide1{};
			pixel_t xSide2{};

			// Loop from c.y down to a.y (inclusive)
			for (pixel_t y = c.y; y >= a.y; y--)
			{
				xSide1 = LimitValue<pixel_t>(FixedRoundToInt(x1), 0, FramePainter::PhysicalWidth - 1);
				xSide2 = LimitValue<pixel_t>(FixedRoundToInt(x2), 0, FramePainter::PhysicalWidth - 1);

				if (xSide1 == xSide2)
				{
					FramePainter::PixelRaw(rawColor, xSide1, y);
				}
				else
				{
					FramePainter::LineHorizontalRaw(rawColor, xSide1, y, xSide2);
				}

				switch (displayOptions::AntiAliasing)
				{
				case DisplayOptions::AntiAliasingEnum::EdgeBlend:
				{
					TriangleEdgeAntiAliasingEdgeBlend(rawColor, xSide1, xSide2, y);
				}
				break;
				case DisplayOptions::AntiAliasingEnum::PixelCoverage:
				{
					TriangleEdgeAntiAliasingPixelCoverage(rawColor, x1, x2, xSide1, xSide2, y);
				}
				break;
				case DisplayOptions::AntiAliasingEnum::None:
				default:
					break;
				}

				x1 -= invSlope1;
				x2 -= invSlope2;
			}
		}

		/// <summary>
		/// Draws a diagonal line between two points using Bresenham's algorithm, selecting the optimal direction based on the line's slope.
		/// Implementation based on https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/
		/// </summary>
		/// <param name="rawColor">The color to use when drawing the line.</param>
		/// <param name="start">The starting point of the line.</param>
		/// <param name="end">The ending point of the line.</param>
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
			const pixel_t scaledWidth = (end.x - start.x) << 1;
			const pixel_index_t slopeMagnitude = AbsValue(end.y - start.y) << 1;
			const int8_t slopeUnit = (end.y >= start.y) ? 1 : -1;
			const int8_t slopeSign = (end.x >= start.x) ? 1 : -1;

			pixel_index_t slopeError = slopeMagnitude - (end.x - start.x);
			pixel_t y = start.y;

			for (pixel_t x = start.x; x != end.x; x += slopeSign)
			{
				FramePainter::PixelRaw(rawColor, x, y);

				slopeError += slopeMagnitude;
				if (slopeError >= 0)
				{
					y += slopeUnit;
					slopeError -= scaledWidth;

					switch (displayOptions::AntiAliasing)
					{
					case DisplayOptions::AntiAliasingEnum::EdgeBlend:
						if (x >= 0 && x < FramePainter::PhysicalWidth &&
							y >= 0 && y < FramePainter::PhysicalHeight)
						{
							FramePainter::PixelRawBlend(rawColor, x, y);
						}
						break;
					default:
						break;
					}
				}
			}

			// Ensure the final endpoint is drawn.
			FramePainter::PixelRaw(rawColor, end.x, end.y);
		}

		void BresenhamUp(const color_t rawColor, const pixel_point_t start, const pixel_point_t end)
		{
			const pixel_t scaledHeight = (end.y - start.y) << 1;
			const pixel_t slopeMagnitude = AbsValue(end.x - start.x) << 1;
			const int8_t slopeUnit = (end.x >= start.x) ? 1 : -1;
			const int8_t slopeSign = (end.y >= start.y) ? 1 : -1;

			pixel_index_t slopeError = (pixel_index_t)slopeMagnitude - (end.y - start.y);
			pixel_t x = start.x;

			for (pixel_t y = start.y; y != end.y; y += slopeSign)
			{
				FramePainter::PixelRaw(rawColor, x, y);

				slopeError += slopeMagnitude;
				if (slopeError >= 0)
				{
					x += slopeUnit;
					slopeError -= scaledHeight;

					switch (displayOptions::AntiAliasing)
					{
					case DisplayOptions::AntiAliasingEnum::EdgeBlend:
						if (x >= 0 && x < FramePainter::PhysicalWidth &&
							y >= 0 && y < FramePainter::PhysicalHeight)
						{
							FramePainter::PixelRawBlend(rawColor, x, y);
						}
						break;
					default:
						break;
					}
				}
			}

			// Ensure the final endpoint is drawn.
			FramePainter::PixelRaw(rawColor, end.x, end.y);
		}

		void TriangleEdgeAntiAliasingEdgeBlend(const color_t rawColor,
			const pixel_t xSide1, const pixel_t xSide2,
			const pixel_t y)
		{
			// Get edge pixels.
			const bool leftToRight = (xSide2 >= xSide1);
			const pixel_t aax1 = leftToRight ? (xSide1 - 1) : (xSide1 + 1);
			const pixel_t aax2 = leftToRight ? (xSide2 + 1) : (xSide2 - 1);

			// Blend the edge pixels.
			if (aax1 >= 0 && aax1 < FramePainter::PhysicalWidth)
				FramePainter::PixelRawBlend(rawColor, aax1, y);
			if (aax2 >= 0 && aax2 < FramePainter::PhysicalWidth)
				FramePainter::PixelRawBlend(rawColor, aax2, y);
		}

		void TriangleEdgeAntiAliasingPixelCoverage(const color_t rawColor,
			const pixel_index_t x1, const pixel_index_t x2,
			const pixel_t xSide1, const pixel_t xSide2,
			const pixel_t y)
		{
			// Get edge pixels.
			const bool leftToRight = (xSide2 >= xSide1);
			const pixel_t aax1 = leftToRight ? (xSide1 - 1) : (xSide1 + 1);
			const pixel_t aax2 = leftToRight ? (xSide2 + 1) : (xSide2 - 1);

			static constexpr int32_t MASK = (int32_t(1) << (FramePainter::BRESENHAM_SCALE - 1)) - 1;

			// For left-to-right scanning:
			// - Left edge: high alpha when x1 fraction is LOW (just entered the shape)
			// - Right edge: high alpha when x2 fraction is HIGH (about to exit the shape)
			if (aax1 >= 0 && aax1 < FramePainter::PhysicalWidth)
			{
				if (leftToRight)
				{
					// Left edge: invert the sub-pixel position for correct coverage
					const uint8_t alpha = 255 - FixedRoundToInt(static_cast<pixel_index_t>(x1 & MASK) * 255);
					FramePainter::PixelRawBlendAlpha(rawColor, aax1, y, alpha);
				}
				else
				{
					// Right-to-left: normal sub-pixel position for correct coverage
					const uint8_t alpha = FixedRoundToInt(static_cast<pixel_index_t>(x1 & MASK) * 255);
					FramePainter::PixelRawBlendAlpha(rawColor, aax1, y, alpha);
				}
			}

			if (aax2 >= 0 && aax2 < FramePainter::PhysicalWidth)
			{
				if (leftToRight) {
					// Right edge: normal sub-pixel position for correct coverage
					const uint8_t alpha = FixedRoundToInt(static_cast<pixel_index_t>(x2 & MASK) * 255);
					FramePainter::PixelRawBlendAlpha(rawColor, aax2, y, alpha);
				}
				else
				{
					// Right-to-left: invert the sub-pixel position for correct coverage
					const uint8_t alpha = 255 - FixedRoundToInt(static_cast<pixel_index_t>(x2 & MASK) * 255);
					FramePainter::PixelRawBlendAlpha(rawColor, aax2, y, alpha);
				}
			}
		}

	private:
		inline constexpr color_t GetRawColor(const rgb_color_t color)
		{
			return displayOptions::Inverted ? ~FramePainter::GetRawColor(color) : FramePainter::GetRawColor(color);
		}

		pixel_point_t TransformCoordinates(const pixel_point_t coordinates) const
		{
			switch (GetTransformCase())
			{
			case TransformCase::Identity:
			default:
				return coordinates;

			case TransformCase::MirrorX:
				return { static_cast<pixel_t>(FrameWidth - 1 - coordinates.x),
						 coordinates.y };

			case TransformCase::MirrorY:
				return { coordinates.x,
						 static_cast<pixel_t>(FrameHeight - 1 - coordinates.y) };

			case TransformCase::MirrorXY:
				return { static_cast<pixel_t>(FrameWidth - 1 - coordinates.x),
						 static_cast<pixel_t>(FrameHeight - 1 - coordinates.y) };

			case TransformCase::Rotate90:
				return { static_cast<pixel_t>(FramePainter::PhysicalWidth - 1 - coordinates.y),
						 coordinates.x };

			case TransformCase::Rotate90MirrorX:
				return { static_cast<pixel_t>(FramePainter::PhysicalWidth - 1 - coordinates.y),
						 static_cast<pixel_t>(FrameWidth - 1 - coordinates.x) };

			case TransformCase::Rotate90MirrorY:
				return { coordinates.y,
						 coordinates.x };

			case TransformCase::Rotate90MirrorXY:
				return { coordinates.y,
						 static_cast<pixel_t>(FramePainter::PhysicalHeight - 1 - coordinates.x) };

			case TransformCase::Rotate180:
				return { static_cast<pixel_t>(FramePainter::PhysicalWidth - 1 - coordinates.x),
						 static_cast<pixel_t>(FramePainter::PhysicalHeight - 1 - coordinates.y) };

			case TransformCase::Rotate180MirrorX:
				return { coordinates.x,
						 static_cast<pixel_t>(FramePainter::PhysicalHeight - 1 - coordinates.y) };

			case TransformCase::Rotate180MirrorY:
				return { static_cast<pixel_t>(FramePainter::PhysicalWidth - 1 - coordinates.x),
						 coordinates.y };
			}
		}

		static constexpr TransformCase GetTransformCase()
		{
			return // Explicitly map each rotation+mirror combination to a named case. C++11 compatible implementation.
				(displayOptions::Rotation == DisplayOptions::RotationEnum::None) ?
				((displayOptions::Mirror == DisplayOptions::MirrorEnum::None) ? TransformCase::Identity :
					(displayOptions::Mirror == DisplayOptions::MirrorEnum::MirrorX) ? TransformCase::MirrorX :
					(displayOptions::Mirror == DisplayOptions::MirrorEnum::MirrorY) ? TransformCase::MirrorY :
					TransformCase::MirrorXY)
				: (displayOptions::Rotation == DisplayOptions::RotationEnum::Rotate90) ?
				((displayOptions::Mirror == DisplayOptions::MirrorEnum::None) ? TransformCase::Rotate90 :
					(displayOptions::Mirror == DisplayOptions::MirrorEnum::MirrorX) ? TransformCase::Rotate90MirrorX :
					(displayOptions::Mirror == DisplayOptions::MirrorEnum::MirrorY) ? TransformCase::Rotate90MirrorY :
					TransformCase::Rotate90MirrorXY)
				: (displayOptions::Rotation == DisplayOptions::RotationEnum::Rotate180) ?
				((displayOptions::Mirror == DisplayOptions::MirrorEnum::None ||
					displayOptions::Mirror == DisplayOptions::MirrorEnum::MirrorXY) ? TransformCase::Rotate180 :
					(displayOptions::Mirror == DisplayOptions::MirrorEnum::MirrorX) ? TransformCase::Rotate180MirrorX :
					TransformCase::Rotate180MirrorY)
				: ((displayOptions::Mirror == DisplayOptions::MirrorEnum::None) ? TransformCase::Rotate90MirrorXY :
					(displayOptions::Mirror == DisplayOptions::MirrorEnum::MirrorX) ? TransformCase::Rotate90MirrorY :
					(displayOptions::Mirror == DisplayOptions::MirrorEnum::MirrorY) ? TransformCase::Rotate90MirrorX :
					TransformCase::Rotate90);
		}

#if defined(ARDUINO_ARCH_RP2040)
	private:
		bool AsyncClearBuffer()
		{
			if (Buffer == nullptr || BufferSize == 0)
				return true;

			if (!DmaActive)
			{
				// Align destination to 32-bit boundary: pre-clear leading bytes until aligned.
				uint8_t* dest = Buffer;
				size_t remaining = BufferSize;

				// Pre-align to 4-byte boundary with byte memset.
				while (((reinterpret_cast<uintptr_t>(dest) & 0x3u) != 0) && remaining > 0)
				{
					*dest++ = displayOptions::Inverted ? UINT8_MAX : 0;
					remaining--;
				}

				// Compute word count for aligned body and leave any tail bytes for after DMA.
				const size_t wordCount = remaining / sizeof(uint32_t);

				DmaChan = dma_claim_unused_channel(true);
				dma_channel_config cfg = dma_channel_get_default_config(DmaChan);
				channel_config_set_read_increment(&cfg, false);
				channel_config_set_write_increment(&cfg, true);
				channel_config_set_transfer_data_size(&cfg, DMA_SIZE_32);

				if (wordCount > 0)
				{
					dma_channel_configure(
						DmaChan,
						&cfg,
						reinterpret_cast<uint32_t*>(dest), // aligned destination
						displayOptions::Inverted ? &FillOnes32 : &FillZero32, // static source constant
						wordCount,                          // number of 32-bit words
						true                                // start immediately
					);
					DmaActive = true;
					return false; // DMA in progress
				}
				else
				{
					// No aligned words; clear any remaining tail bytes now.
					if (remaining)
					{
						memset(dest, displayOptions::Inverted ? UINT8_MAX : 0, remaining);
					}
					return true;
				}
			}

			// Poll DMA completion.
			if (dma_channel_is_busy(DmaChan))
			{
				return false;
			}

			// DMA complete: clear any tail bytes (remaining % 4).
			// Recompute aligned region to find tail safely.
			uint8_t* destAlignedStart = Buffer;
			size_t remaining = BufferSize;

			while (((reinterpret_cast<uintptr_t>(destAlignedStart) & 0x3u) != 0) && remaining > 0)
			{
				// already byte-cleared in start phase
				destAlignedStart++;
				remaining--;
			}

			const size_t alignedBytes = (remaining / sizeof(uint32_t)) * sizeof(uint32_t);
			uint8_t* tail = destAlignedStart + alignedBytes;
			const size_t tailBytes = remaining - alignedBytes;

			if (tailBytes)
			{
				memset(tail, displayOptions::Inverted ? UINT8_MAX : 0, tailBytes);
			}

			dma_channel_unclaim(DmaChan);
			DmaChan = -1;
			DmaActive = false;

			return true;
		}
#endif
	};
}
#endif