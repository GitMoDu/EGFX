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
	public:
		using FramePainter::BufferSize;
		using FramePainter::FrameWidth;
		using FramePainter::FrameHeight;

		using typename FramePainter::color_t;

		using Configuration = displayOptions;

	protected:
		using FramePainter::IntToFixed;
		using FramePainter::FixedRoundToInt;

	private:
		enum class OutcodeEnum : uint8_t
		{
			OUT_LEFT = 1,
			OUT_RIGHT = 2,
			OUT_BOTTOM = 4,
			OUT_TOP = 8
		};

		static constexpr uint8_t BRESENHAM_SCALE = 8;

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
			if (x >= 0 && x < FrameWidth &&
				y >= 0 && y < FrameHeight)
			{
				const color_t rawColor = GetRawColor(color);

				switch (displayOptions::Mirror)
				{
				case DisplayOptions::MirrorEnum::MirrorX:
					FramePainter::PixelRaw(rawColor, MirrorX(x), y);
					break;
				case DisplayOptions::MirrorEnum::MirrorY:
					FramePainter::PixelRaw(rawColor, x, MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::MirrorXY:
					FramePainter::PixelRaw(rawColor, MirrorX(x), MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::None:
				default:
					FramePainter::PixelRaw(rawColor, x, y);
					break;
				}
			}
		}

		void Pixel(const rgb_color_t color, const pixel_point_t point) final
		{
			Pixel(color, point.x, point.y);
		}

		void PixelBlend(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			if (x >= 0 && x < FrameWidth &&
				y >= 0 && y < FrameHeight)
			{
				const color_t rawColor = GetRawColor(color);
				switch (displayOptions::Mirror)
				{
				case DisplayOptions::MirrorEnum::MirrorX:
					FramePainter::PixelRawBlend(rawColor, MirrorX(x), y);
					break;
				case DisplayOptions::MirrorEnum::MirrorY:
					FramePainter::PixelRawBlend(rawColor, x, MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::MirrorXY:
					FramePainter::PixelRawBlend(rawColor, MirrorX(x), MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::None:
				default:
					FramePainter::PixelRawBlend(rawColor, x, y);
					break;
				}
			}
		}

		void PixelBlendAlpha(const rgb_color_t color, const pixel_t x, const pixel_t y, const uint8_t alpha) final
		{
			if (alpha == UINT8_MAX / 2)
			{
				PixelBlend(color, x, y);
			}
			else if (alpha > 0 &&
				x >= 0 && x < FrameWidth &&
				y >= 0 && y < FrameHeight)
			{
				const color_t uninvertedColor = FramePainter::GetRawColor(color);
				const color_t rawColor = displayOptions::Inverted
					? (uninvertedColor & 0xFF000000) | (~uninvertedColor & 0x00FFFFFF)
					: uninvertedColor;

				switch (displayOptions::Mirror)
				{
				case DisplayOptions::MirrorEnum::MirrorX:
					FramePainter::PixelRawBlendAlpha(rawColor, MirrorX(x), y, alpha);
					break;
				case DisplayOptions::MirrorEnum::MirrorY:
					FramePainter::PixelRawBlendAlpha(rawColor, x, MirrorY(y), alpha);
					break;
				case DisplayOptions::MirrorEnum::MirrorXY:
					FramePainter::PixelRawBlendAlpha(rawColor, MirrorX(x), MirrorY(y), alpha);
					break;
				case DisplayOptions::MirrorEnum::None:
				default:
					FramePainter::PixelRawBlendAlpha(rawColor, x, y, alpha);
					break;
				}
			}
		}

		void PixelBlendAdd(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			if (x >= 0 && x < FrameWidth &&
				y >= 0 && y < FrameHeight)
			{
				const color_t rawColor = GetRawColor(color);
				switch (displayOptions::Mirror)
				{
				case DisplayOptions::MirrorEnum::MirrorX:
					FramePainter::PixelRawBlendAdd(rawColor, MirrorX(x), y);
					break;
				case DisplayOptions::MirrorEnum::MirrorY:
					FramePainter::PixelRawBlendAdd(rawColor, x, MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::MirrorXY:
					FramePainter::PixelRawBlendAdd(rawColor, MirrorX(x), MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::None:
				default:
					FramePainter::PixelRawBlendAdd(rawColor, x, y);
					break;
				}
			}
		}

		void PixelBlendSubtract(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			if (x >= 0 && x < FrameWidth &&
				y >= 0 && y < FrameHeight)
			{
				const color_t rawColor = GetRawColor(color);
				switch (displayOptions::Mirror)
				{
				case DisplayOptions::MirrorEnum::MirrorX:
					FramePainter::PixelRawBlendSubtract(rawColor, MirrorX(x), y);
					break;
				case DisplayOptions::MirrorEnum::MirrorY:
					FramePainter::PixelRawBlendSubtract(rawColor, x, MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::MirrorXY:
					FramePainter::PixelRawBlendSubtract(rawColor, MirrorX(x), MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::None:
				default:
					FramePainter::PixelRawBlendSubtract(rawColor, x, y);
					break;
				}
			}
		}

		void PixelBlendMultiply(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			if (x >= 0 && x < FrameWidth &&
				y >= 0 && y < FrameHeight)
			{
				const color_t rawColor = GetRawColor(color);
				switch (displayOptions::Mirror)
				{
				case DisplayOptions::MirrorEnum::MirrorX:
					FramePainter::PixelRawBlendMultiply(rawColor, MirrorX(x), y);
					break;
				case DisplayOptions::MirrorEnum::MirrorY:
					FramePainter::PixelRawBlendMultiply(rawColor, x, MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::MirrorXY:
					FramePainter::PixelRawBlendMultiply(rawColor, MirrorX(x), MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::None:
				default:
					FramePainter::PixelRawBlendMultiply(rawColor, x, y);
					break;
				}
			}
		}

		void PixelBlendScreen(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			if (x >= 0 && x < FrameWidth &&
				y >= 0 && y < FrameHeight)
			{
				const color_t rawColor = GetRawColor(color);
				switch (displayOptions::Mirror)
				{
				case DisplayOptions::MirrorEnum::MirrorX:
					FramePainter::PixelRawBlendScreen(rawColor, MirrorX(x), y);
					break;
				case DisplayOptions::MirrorEnum::MirrorY:
					FramePainter::PixelRawBlendScreen(rawColor, x, MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::MirrorXY:
					FramePainter::PixelRawBlendScreen(rawColor, MirrorX(x), MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::None:
				default:
					FramePainter::PixelRawBlendScreen(rawColor, x, y);
					break;
				}
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

			// Apply mirroring.
			switch (displayOptions::Mirror)
			{
			case DisplayOptions::MirrorEnum::MirrorX:
				start = { MirrorX(start.x), start.y };
				end = { MirrorX(end.x), end.y };
				break;
			case DisplayOptions::MirrorEnum::MirrorY:
				start = { start.x, MirrorY(start.y) };
				end = { end.x, MirrorY(end.y) };
				break;
			case DisplayOptions::MirrorEnum::MirrorXY:
				start = { MirrorX(start.x), MirrorY(start.y) };
				end = { MirrorX(end.x), MirrorY(end.y) };
				break;
			case DisplayOptions::MirrorEnum::None:
			default:
				break;
			}

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
			pixel_point_t topLeft = { (topLeftX <= bottomRightX) ? topLeftX : bottomRightX,
					(topLeftY <= bottomRightY) ? topLeftY : bottomRightY };
			pixel_point_t bottomRight = { (topLeftX <= bottomRightX) ? bottomRightX : topLeftX,
				(topLeftY <= bottomRightY) ? bottomRightY : topLeftY };

			if (!ClipRectangle(topLeft, bottomRight))
			{
				return;
			}

			const color_t rawColor = GetRawColor(color);

			// Apply mirroring.
			switch (displayOptions::Mirror)
			{
			case DisplayOptions::MirrorEnum::MirrorX:
				topLeft = { MirrorX(topLeft.x), topLeft.y };
				bottomRight = { MirrorX(bottomRight.x), bottomRight.y };
				break;
			case DisplayOptions::MirrorEnum::MirrorY:
				topLeft = { topLeft.x, MirrorY(topLeft.y) };
				bottomRight = { bottomRight.x, MirrorY(bottomRight.y) };
				break;
			case DisplayOptions::MirrorEnum::MirrorXY:
				topLeft = { MirrorX(topLeft.x), MirrorY(topLeft.y) };
				bottomRight = { MirrorX(bottomRight.x), MirrorY(bottomRight.y) };
				break;
			case DisplayOptions::MirrorEnum::None:
			default:
				break;
			}

			if (topLeft.x == bottomRight.x)
			{
				if (topLeft.y == bottomRight.y)
				{
					FramePainter::PixelRaw(rawColor, topLeft.x, topLeft.y);
				}
				else
				{
					FramePainter::LineVerticalRaw(rawColor, topLeft.x, topLeft.y, bottomRight.y);
				}
			}
			else if (topLeft.y == bottomRight.y)
			{
				FramePainter::LineHorizontalRaw(rawColor, topLeft.x, topLeft.y, bottomRight.x);
			}
			else
			{
				FramePainter::RectangleFillRaw(rawColor, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
			}
		}

		void RectangleFill(const rgb_color_t color, const pixel_rectangle_t& rectangle) final
		{
			RectangleFill(color, rectangle.topLeft.x, rectangle.topLeft.y, rectangle.bottomRight.x, rectangle.bottomRight.y);
		}

		void Rectangle(const rgb_color_t color, const pixel_t topLeftX, const pixel_t topLeftY,
			const pixel_t bottomRightX, const pixel_t bottomRightY) final
		{
			pixel_point_t topLeft = { topLeftX, topLeftY };
			pixel_point_t bottomRight = { bottomRightX , bottomRightY };

			// Apply mirroring.
			switch (displayOptions::Mirror)
			{
			case DisplayOptions::MirrorEnum::MirrorX:
				topLeft = { MirrorX(topLeft.x), topLeft.y };
				bottomRight = { MirrorX(bottomRight.x), bottomRight.y };
				break;
			case DisplayOptions::MirrorEnum::MirrorY:
				topLeft = { topLeft.x, MirrorY(topLeft.y) };
				bottomRight = { bottomRight.x, MirrorY(bottomRight.y) };
				break;
			case DisplayOptions::MirrorEnum::MirrorXY:
				topLeft = { MirrorX(topLeft.x), MirrorY(topLeft.y) };
				bottomRight = { MirrorX(bottomRight.x), MirrorY(bottomRight.y) };
				break;
			case DisplayOptions::MirrorEnum::None:
			default:
				break;
			}

			const color_t rawColor = GetRawColor(color);
			const pixel_point_t topLeftLimited = { LimitValue<pixel_t>(topLeft.x, 0, FrameWidth - 1), LimitValue<pixel_t>(topLeft.y, 0, FrameHeight - 1) };
			const pixel_point_t bottomRightLimited = { LimitValue<pixel_t>(bottomRight.x, 0, FrameWidth - 1), LimitValue<pixel_t>(bottomRight.y, 0, FrameHeight - 1) };

			if (((topLeft.x >= 0 && topLeft.x < FrameWidth) ||
				(bottomRight.x >= 0 && bottomRight.x < FrameWidth))
				&& bottomRightLimited.x >= topLeftLimited.x)
			{
				// Draw top horizontal line.
				if (topLeft.y >= 0 && topLeft.y < FrameHeight)
				{
					FramePainter::LineHorizontalRaw(rawColor, topLeftLimited.x, topLeft.y, bottomRightLimited.x);
				}

				// Draw bottom horizontal line.
				if (bottomRight.y >= 0 && bottomRight.y < FrameHeight)
				{
					FramePainter::LineHorizontalRaw(rawColor, topLeftLimited.x, bottomRight.y, bottomRightLimited.x);
				}
			}

			if (((topLeft.y >= 0 && topLeft.y < FrameHeight) ||
				(bottomRight.y >= 0 && bottomRight.y < FrameHeight))
				&& bottomRightLimited.y >= topLeftLimited.y)
			{
				// Draw left vertical line.
				if (topLeft.x >= 0 && topLeft.x < FrameWidth)
				{
					FramePainter::LineVerticalRaw(rawColor, topLeft.x, topLeftLimited.y + 1, bottomRightLimited.y - 1);
				}

				// Draw right vertical line.
				if (bottomRight.x >= 0 && bottomRight.x < FrameWidth)
				{
					FramePainter::LineVerticalRaw(rawColor, bottomRight.x, topLeftLimited.y + 1, bottomRightLimited.y - 1);
				}
			}
		}

		void Rectangle(const rgb_color_t color, const pixel_rectangle_t& rectangle) final
		{
			Rectangle(color, rectangle.topLeft.x, rectangle.topLeft.y, rectangle.bottomRight.x, rectangle.bottomRight.y);
		}

		void Triangle(const rgb_color_t color, const pixel_triangle_t& triangle) final
		{
			Line(color, triangle.a.x, triangle.a.y, triangle.b.x, triangle.b.y);
			Line(color, triangle.b.x, triangle.b.y, triangle.c.x, triangle.c.y);
			Line(color, triangle.a.x, triangle.a.y, triangle.c.x, triangle.c.y);
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
			pixel_t ax;
			pixel_t ay;
			pixel_t bx;
			pixel_t by;
			pixel_t cx;
			pixel_t cy;

			// Apply mirroring
			switch (displayOptions::Mirror)
			{
			case DisplayOptions::MirrorEnum::MirrorX:
				ax = MirrorX(triangle.a.x);
				ay = triangle.a.y;
				bx = MirrorX(triangle.b.x);
				by = triangle.b.y;
				cx = MirrorX(triangle.c.x);
				cy = triangle.c.y;
				break;
			case DisplayOptions::MirrorEnum::MirrorY:
				ax = triangle.a.x;
				ay = MirrorY(triangle.a.y);
				bx = triangle.b.x;
				by = MirrorY(triangle.b.y);
				cx = triangle.c.x;
				cy = MirrorY(triangle.c.y);
				break;
			case DisplayOptions::MirrorEnum::MirrorXY:
				ax = MirrorX(triangle.a.x);
				ay = MirrorY(triangle.a.y);
				bx = MirrorX(triangle.b.x);
				by = MirrorY(triangle.b.y);
				cx = MirrorX(triangle.c.x);
				cy = MirrorY(triangle.c.y);
				break;
			case DisplayOptions::MirrorEnum::None:
			default:
				ax = triangle.a.x;
				ay = triangle.a.y;
				bx = triangle.b.x;
				by = triangle.b.y;
				cx = triangle.c.x;
				cy = triangle.c.y;
				break;
			}

			const color_t rawColor = GetRawColor(color);

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
				const pixel_t dxTotal = (pixel_t)c.x - (pixel_t)a.x;
				const pixel_t dyTotal = (pixel_t)c.y - (pixel_t)a.y;
				const pixel_t dySegment = (pixel_t)b.y - (pixel_t)a.y;

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
				xSide1 = LimitValue<pixel_t>(FixedRoundToInt(x1), 0, FrameWidth - 1);
				xSide2 = LimitValue<pixel_t>(FixedRoundToInt(x2), 0, FrameWidth - 1);

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
				xSide1 = LimitValue<pixel_t>(FixedRoundToInt(x1), 0, FrameWidth - 1);
				xSide2 = LimitValue<pixel_t>(FixedRoundToInt(x2), 0, FrameWidth - 1);

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
						if (x >= 0 && x < FrameWidth &&
							y >= 0 && y < FrameHeight)
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
						if (x >= 0 && x < FrameWidth &&
							y >= 0 && y < FrameHeight)
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
			if (aax1 >= 0 && aax1 < FrameWidth)
				FramePainter::PixelRawBlend(rawColor, aax1, y);
			if (aax2 >= 0 && aax2 < FrameWidth)
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

			static constexpr int32_t MASK = (int32_t(1) << (BRESENHAM_SCALE - 1)) - 1;
			//static constexpr uint8_t MASK = 127;

			// For left-to-right scanning:
			// - Left edge: high alpha when x1 fraction is LOW (just entered the shape)
			// - Right edge: high alpha when x2 fraction is HIGH (about to exit the shape)
			if (aax1 >= 0 && aax1 < FrameWidth)
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

			if (aax2 >= 0 && aax2 < FrameWidth)
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
		pixel_t MirrorX(const pixel_t x) const { return FrameWidth - 1 - x; }
		pixel_t MirrorY(const pixel_t y) const { return FrameHeight - 1 - y; }

		inline constexpr color_t GetRawColor(const rgb_color_t color)
		{
			return displayOptions::Inverted ? ~FramePainter::GetRawColor(color) : FramePainter::GetRawColor(color);
		}

		/// <summary>
		/// Clips a rectangle defined by its top-left and bottom-right points to fit within the frame boundaries.
		/// </summary>
		/// <param name="topLeft">Reference to the top-left corner of the rectangle. May be modified to fit within the frame.</param>
		/// <param name="bottomRight">Reference to the bottom-right corner of the rectangle. May be modified to fit within the frame.</param>
		/// <returns>True if the rectangle is valid and was clipped; false if the rectangle is invalid or completely outside the frame.</returns>
		bool ClipRectangle(pixel_point_t& topLeft, pixel_point_t& bottomRight)
		{
			if ((topLeft.x < 0 && bottomRight.x < 0)
				|| (topLeft.y < 0 && bottomRight.y < 0)
				|| (topLeft.x >= FrameWidth && bottomRight.x >= FrameWidth)
				|| (topLeft.y >= FrameHeight && bottomRight.y >= FrameHeight)
				|| (topLeft.x > bottomRight.x || topLeft.y > bottomRight.y))
			{
				return false; // Invalid rectangle
			}

			topLeft.x = LimitValue<pixel_t>(topLeft.x, 0, FrameWidth - 1);
			topLeft.y = LimitValue<pixel_t>(topLeft.y, 0, FrameHeight - 1);
			bottomRight.x = LimitValue<pixel_t>(bottomRight.x, 0, FrameWidth - 1);
			bottomRight.y = LimitValue<pixel_t>(bottomRight.y, 0, FrameHeight - 1);

			return true;
		}

		/// <summary>
		/// Clips a line segment to the screen boundaries using the Cohen-Sutherland algorithm.
		/// </summary>
		/// <param name="p0">Reference to the first endpoint of the line segment. On return, may be modified to the clipped position.</param>
		/// <param name="p1">Reference to the second endpoint of the line segment. On return, may be modified to the clipped position.</param>
		/// <returns>true if the line segment is at least partially within the screen and has been clipped (if necessary); false if the line segment lies entirely outside the screen.</returns>
		bool ClipLine(pixel_point_t& p0, pixel_point_t& p1)
		{
			pixel_t x0 = p0.x, y0 = p0.y, x1 = p1.x, y1 = p1.y;
			uint8_t outcode0 = ComputeOutCode(x0, y0);
			uint8_t outcode1 = ComputeOutCode(x1, y1);
			pixel_t x = 0;
			pixel_t y = 0;
			while (true)
			{
				if (!(outcode0 | outcode1))
				{
					// Both endpoints inside
					p0 = { x0 , y0 };
					p1 = { x1 , y1 };
					return true;

				}
				else if (outcode0 & outcode1)
				{
					// Both endpoints share an outside zone
					return false;
				}
				else
				{
					// At least one endpoint is outside
					uint8_t outcodeOut = outcode0 ? outcode0 : outcode1;
					x = 0;
					y = 0;

					if (outcodeOut & (uint8_t)OutcodeEnum::OUT_TOP)
					{
						if (y1 == y0) return false; // Parallel to top
						x = x0 + int32_t(x1 - x0) * (0 - y0) / (y1 - y0);
						y = 0;
					}
					else if (outcodeOut & (uint8_t)OutcodeEnum::OUT_BOTTOM)
					{
						if (y1 == y0) return false; // Parallel to bottom
						x = x0 + int32_t(x1 - x0) * (FrameHeight - 1 - y0) / (y1 - y0);
						y = FrameHeight - 1;
					}
					else if (outcodeOut & (uint8_t)OutcodeEnum::OUT_RIGHT)
					{
						if (x1 == x0) return false; // Parallel to right
						y = y0 + int32_t(y1 - y0) * (FrameWidth - 1 - x0) / (x1 - x0);
						x = FrameWidth - 1;
					}
					else // OUT_LEFT
					{
						if (x1 == x0) return false; // Parallel to left
						y = y0 + int32_t(y1 - y0) * (0 - x0) / (x1 - x0);
						x = 0;
					}

					if (outcodeOut == outcode0)
					{
						x0 = x; y0 = y;
						outcode0 = ComputeOutCode(x0, y0);
					}
					else
					{
						x1 = x; y1 = y;
						outcode1 = ComputeOutCode(x1, y1);
					}
				}
			}
		}

		/// <summary>
		/// Computes the outcode for a point based on its position relative to the frame boundaries.
		/// </summary>
		/// <param name="x">The x-coordinate of the point.</param>
		/// <param name="y">The y-coordinate of the point.</param>
		/// <returns>A uint8_t value representing the outcode, indicating which sides of the frame (left, right, top, bottom) the point is outside of.</returns>
		inline uint8_t ComputeOutCode(const pixel_t x, const pixel_t y)
		{
			uint8_t code = 0;
			if (x < 0) code |= (uint8_t)OutcodeEnum::OUT_LEFT;
			else if (x >= FrameWidth) code |= (uint8_t)OutcodeEnum::OUT_RIGHT;
			if (y < 0) code |= (uint8_t)OutcodeEnum::OUT_TOP;
			else if (y >= FrameHeight) code |= (uint8_t)OutcodeEnum::OUT_BOTTOM;

			return code;
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