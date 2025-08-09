#ifndef _ABSTRACT_FRAME_BUFFER_h
#define _ABSTRACT_FRAME_BUFFER_h

#include "../Model/RgbColor.h"
#include "../Model/ColorConverter.h"
#include "../Model/DisplayOptions.h"
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
	/// <typeparam name="displayOptions">Display configuration options (mirror, rotation, inverted colors, AA).</typeparam>
	template<typename ColorConverter
		, uint8_t clearDivisorPower
		, pixel_t frameWidth
		, pixel_t frameHeight
		, typename displayOptions = DisplayOptions::Default>
	class AbstractFrameBuffer : public IFrameBuffer
	{
	public:
		using Configuration = displayOptions;

		static constexpr pixel_t FrameWidth = frameWidth;
		static constexpr pixel_t FrameHeight = frameHeight;
		static constexpr size_t BufferSize = ColorConverter::BufferSize(FrameWidth, FrameHeight);

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

		enum class OutcodeEnum : uint8_t
		{
			OUT_LEFT = 1,
			OUT_RIGHT = 2,
			OUT_BOTTOM = 4,
			OUT_TOP = 8
		};

	protected:
		uint8_t* Buffer;

	protected:
		uint8_t ClearIndex = 0;

	protected:
		virtual void PixelRaw(const color_t rawColor, const pixel_t x, const pixel_t y) = 0;

		virtual void PixelRawBlend(const color_t rawColor, const pixel_t x, const pixel_t y) {}
		virtual void PixelRawBlendAlpha(const color_t rawColor, const pixel_t x, const pixel_t y, const uint8_t alpha) {}
		virtual void PixelRawBlendAdd(const color_t rawColor, const pixel_t x, const pixel_t y) {}
		virtual void PixelRawBlendSubtract(const color_t rawColor, const pixel_t x, const pixel_t y) {}
		virtual void PixelRawBlendMultiply(const color_t rawColor, const pixel_t x, const pixel_t y) {}
		virtual void PixelRawBlendScreen(const color_t rawColor, const pixel_t x, const pixel_t y) {}

	public:
		AbstractFrameBuffer(uint8_t buffer[BufferSize] = nullptr)
			: IFrameBuffer()
			, Buffer(buffer)
		{
		}

	public:
		bool IsMonochrome() const final
		{
			return ColorConverter::Monochrome();
		}

		uint8_t GetColorDepth() const final
		{
			return ColorConverter::ColorDepth();
		}

		pixel_t GetFrameWidth() const final
		{
			return FrameWidth;
		}

		pixel_t GetFrameHeight() const final
		{
			return FrameHeight;
		}

		const uint8_t* GetFrameBuffer() final
		{
			return (const uint8_t*)Buffer;
		}

		bool ClearFrameBuffer() final
		{
			// Background color full, when inverted.
			if (displayOptions::Inverted)
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
				const color_t rawColor = displayOptions::Inverted ? ~ColorConverter::GetRawColor(color) : ColorConverter::GetRawColor(color);

				switch (displayOptions::Mirror)
				{
				case DisplayOptions::MirrorEnum::MirrorX:
					PixelRaw(rawColor, MirrorX(x), y);
					break;
				case DisplayOptions::MirrorEnum::MirrorY:
					PixelRaw(rawColor, x, MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::MirrorXY:
					PixelRaw(rawColor, MirrorX(x), MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::None:
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
			const color_t rawColor = displayOptions::Inverted ? ~ColorConverter::GetRawColor(color) : ColorConverter::GetRawColor(color);
			FillRaw(rawColor);
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

			const color_t rawColor = displayOptions::Inverted ? ~ColorConverter::GetRawColor(color) : ColorConverter::GetRawColor(color);

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
					PixelRaw(rawColor, start.x, start.y);
				}
				else
				{
					LineVerticalRaw(rawColor, start.x, start.y, end.y);
				}
			}
			else if (start.y == end.y)
			{
				LineHorizontalRaw(rawColor, start.x, start.y, end.x);
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
			pixel_point_t topLeft = { topLeftX, topLeftY };
			pixel_point_t bottomRight = { bottomRightX , bottomRightY };

			if (!ClipRectangle(topLeft, bottomRight))
			{
				return;
			}

			const color_t rawColor = displayOptions::Inverted ? ~ColorConverter::GetRawColor(color) : ColorConverter::GetRawColor(color);

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
					PixelRaw(rawColor, topLeft.x, topLeft.y);
				}
				else
				{
					LineVerticalRaw(rawColor, topLeft.x, topLeft.y, bottomRight.y);
				}
			}
			else if (topLeft.y == bottomRight.y)
			{
				LineHorizontalRaw(rawColor, topLeft.x, topLeft.y, bottomRight.x);
			}
			else
			{
				RectangleFillRaw(rawColor, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
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

			const color_t rawColor = displayOptions::Inverted ? ~ColorConverter::GetRawColor(color) : ColorConverter::GetRawColor(color);
			const pixel_point_t topLeftLimited = { LimitValue<pixel_t>(topLeft.x, 0, FrameWidth - 1), LimitValue<pixel_t>(topLeft.y, 0, FrameHeight - 1) };
			const pixel_point_t bottomRightLimited = { LimitValue<pixel_t>(bottomRight.x, 0, FrameWidth - 1), LimitValue<pixel_t>(bottomRight.y, 0, FrameHeight - 1) };

			if (((topLeft.x >= 0 && topLeft.x < FrameWidth) ||
				(bottomRight.x >= 0 && bottomRight.x < FrameWidth))
				&& bottomRightLimited.x >= topLeftLimited.x)
			{
				// Draw top horizontal line.
				if (topLeft.y >= 0 && topLeft.y < FrameHeight)
				{
					LineHorizontalRaw(rawColor, topLeftLimited.x, topLeft.y, bottomRightLimited.x);
				}

				// Draw bottom horizontal line.
				if (bottomRight.y >= 0 && bottomRight.y < FrameHeight)
				{
					LineHorizontalRaw(rawColor, topLeftLimited.x, bottomRight.y, bottomRightLimited.x);
				}
			}

			if (((topLeft.y >= 0 && topLeft.y < FrameHeight) ||
				(bottomRight.y >= 0 && bottomRight.y < FrameHeight))
				&& bottomRightLimited.y >= topLeftLimited.y)
			{
				// Draw left vertical line.
				if (topLeft.x >= 0 && topLeft.x < FrameWidth)
				{
					LineVerticalRaw(rawColor, topLeft.x, topLeftLimited.y + 1, bottomRightLimited.y - 1);
				}

				// Draw right vertical line.
				if (bottomRight.x >= 0 && bottomRight.x < FrameWidth)
				{
					LineVerticalRaw(rawColor, bottomRight.x, topLeftLimited.y + 1, bottomRightLimited.y - 1);
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
			const color_t rawColor = displayOptions::Inverted ? ~ColorConverter::GetRawColor(color) : ColorConverter::GetRawColor(color);

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

		void PixelBlend(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			if (x >= 0 && x < FrameWidth &&
				y >= 0 && y < FrameHeight)
			{
				const color_t rawColor = displayOptions::Inverted ? ~ColorConverter::GetRawColor(color) : ColorConverter::GetRawColor(color);
				switch (displayOptions::Mirror)
				{
				case DisplayOptions::MirrorEnum::MirrorX:
					PixelRawBlend(rawColor, MirrorX(x), y);
					break;
				case DisplayOptions::MirrorEnum::MirrorY:
					PixelRawBlend(rawColor, x, MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::MirrorXY:
					PixelRawBlend(rawColor, MirrorX(x), MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::None:
				default:
					PixelRawBlend(rawColor, x, y);
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
				const color_t uninvertedColor = ColorConverter::GetRawColor(color);
				const color_t rawColor = displayOptions::Inverted
					? (uninvertedColor & 0xFF000000) | (~uninvertedColor & 0x00FFFFFF)
					: uninvertedColor;

				switch (displayOptions::Mirror)
				{
				case DisplayOptions::MirrorEnum::MirrorX:
					PixelRawBlendAlpha(rawColor, MirrorX(x), y, alpha);
					break;
				case DisplayOptions::MirrorEnum::MirrorY:
					PixelRawBlendAlpha(rawColor, x, MirrorY(y), alpha);
					break;
				case DisplayOptions::MirrorEnum::MirrorXY:
					PixelRawBlendAlpha(rawColor, MirrorX(x), MirrorY(y), alpha);
					break;
				case DisplayOptions::MirrorEnum::None:
				default:
					PixelRawBlendAlpha(rawColor, x, y, alpha);
					break;
				}
			}
		}

		void PixelBlendAdd(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			if (x >= 0 && x < FrameWidth &&
				y >= 0 && y < FrameHeight)
			{
				const color_t rawColor = displayOptions::Inverted ? ~ColorConverter::GetRawColor(color) : ColorConverter::GetRawColor(color);
				switch (displayOptions::Mirror)
				{
				case DisplayOptions::MirrorEnum::MirrorX:
					PixelRawBlendAdd(rawColor, MirrorX(x), y);
					break;
				case DisplayOptions::MirrorEnum::MirrorY:
					PixelRawBlendAdd(rawColor, x, MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::MirrorXY:
					PixelRawBlendAdd(rawColor, MirrorX(x), MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::None:
				default:
					PixelRawBlendAdd(rawColor, x, y);
					break;
				}
			}
		}

		void PixelBlendSubtract(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			if (x >= 0 && x < FrameWidth &&
				y >= 0 && y < FrameHeight)
			{
				const color_t rawColor = displayOptions::Inverted ? ~ColorConverter::GetRawColor(color) : ColorConverter::GetRawColor(color);
				switch (displayOptions::Mirror)
				{
				case DisplayOptions::MirrorEnum::MirrorX:
					PixelRawBlendSubtract(rawColor, MirrorX(x), y);
					break;
				case DisplayOptions::MirrorEnum::MirrorY:
					PixelRawBlendSubtract(rawColor, x, MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::MirrorXY:
					PixelRawBlendSubtract(rawColor, MirrorX(x), MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::None:
				default:
					PixelRawBlendSubtract(rawColor, x, y);
					break;
				}
			}
		}

		void PixelBlendMultiply(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			if (x >= 0 && x < FrameWidth &&
				y >= 0 && y < FrameHeight)
			{
				const color_t rawColor = displayOptions::Inverted ? ~ColorConverter::GetRawColor(color) : ColorConverter::GetRawColor(color);
				switch (displayOptions::Mirror)
				{
				case DisplayOptions::MirrorEnum::MirrorX:
					PixelRawBlendMultiply(rawColor, MirrorX(x), y);
					break;
				case DisplayOptions::MirrorEnum::MirrorY:
					PixelRawBlendMultiply(rawColor, x, MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::MirrorXY:
					PixelRawBlendMultiply(rawColor, MirrorX(x), MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::None:
				default:
					PixelRawBlendMultiply(rawColor, x, y);
					break;
				}
			}
		}

		void PixelBlendScreen(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			if (x >= 0 && x < FrameWidth &&
				y >= 0 && y < FrameHeight)
			{
				const color_t rawColor = displayOptions::Inverted ? ~ColorConverter::GetRawColor(color) : ColorConverter::GetRawColor(color);
				switch (displayOptions::Mirror)
				{
				case DisplayOptions::MirrorEnum::MirrorX:
					PixelRawBlendScreen(rawColor, MirrorX(x), y);
					break;
				case DisplayOptions::MirrorEnum::MirrorY:
					PixelRawBlendScreen(rawColor, x, MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::MirrorXY:
					PixelRawBlendScreen(rawColor, MirrorX(x), MirrorY(y));
					break;
				case DisplayOptions::MirrorEnum::None:
				default:
					PixelRawBlendScreen(rawColor, x, y);
					break;
				}
			}
		}

	protected:
		virtual void LineVerticalRaw(const color_t rawColor, const pixel_t x, const pixel_t y1, const pixel_t y2)
		{
			const int8_t sign = (y2 >= y1) ? 1 : -1;
			const pixel_t endY = (y2 + sign);
			for (pixel_t y = y1; y != endY; y += sign)
			{
				PixelRaw(rawColor, x, y);
			}
		}

		virtual void LineHorizontalRaw(const color_t rawColor, const pixel_t x1, const pixel_t y, const pixel_t x2)
		{
			const int8_t sign = (x2 >= x1) ? 1 : -1;
			const pixel_t endX = (x2 + sign);
			for (pixel_t x = x1; x != endX; x += sign)
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

			pixel_t xSide1{};
			pixel_t xSide2{};

			// Loop from a.y to b.y (inclusive)
			for (pixel_t y = a.y; y <= b.y; y++)
			{
				xSide1 = LimitValue<pixel_t>(x1 >> BRESENHAM_SCALE, 0, FrameWidth - 1);
				xSide2 = LimitValue<pixel_t>(x2 >> BRESENHAM_SCALE, 0, FrameWidth - 1);

				if (xSide1 == xSide2)
				{
					PixelRaw(rawColor, xSide1, y);
				}
				else
				{
					LineHorizontalRaw(rawColor, xSide1, y, xSide2);
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
			const pixel_index_t invSlope1 = ((pixel_index_t)(c.x - a.x) << BRESENHAM_SCALE) / (c.y - a.y);
			const pixel_index_t invSlope2 = ((pixel_index_t)(c.x - b.x) << BRESENHAM_SCALE) / (c.y - b.y);

			// Starting x positions in fixed-point
			pixel_index_t x1 = (pixel_index_t)c.x << BRESENHAM_SCALE;
			pixel_index_t x2 = x1;

			pixel_t xSide1{};
			pixel_t xSide2{};

			// Loop from c.y down to a.y (inclusive)
			for (pixel_t y = c.y; y >= a.y; y--)
			{
				xSide1 = LimitValue<pixel_t>(x1 >> BRESENHAM_SCALE, 0, FrameWidth - 1);
				xSide2 = LimitValue<pixel_t>(x2 >> BRESENHAM_SCALE, 0, FrameWidth - 1);

				if (xSide1 == xSide2)
				{
					PixelRaw(rawColor, xSide1, y);
				}
				else
				{
					LineHorizontalRaw(rawColor, xSide1, y, xSide2);
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
				PixelRaw(rawColor, x, y);

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
							PixelRawBlend(rawColor, x, y);
						}
						break;
					default:
						break;
					}
				}
			}
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
				PixelRaw(rawColor, x, y);

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
							PixelRawBlend(rawColor, x, y);
						}
						break;
					default:
						break;
					}
				}
			}
		}

		/// <summary>
		/// Clips a rectangle defined by its top-left and bottom-right points to fit within the frame boundaries.
		/// </summary>
		/// <param name="topLeft">Reference to the top-left corner of the rectangle. May be modified to fit within the frame.</param>
		/// <param name="bottomRight">Reference to the bottom-right corner of the rectangle. May be modified to fit within the frame.</param>
		/// <returns>True if the rectangle is valid and was clipped; false if the rectangle is invalid or completely outside the frame.</returns>
		static bool ClipRectangle(pixel_point_t& topLeft, pixel_point_t& bottomRight)
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
		/// Clips a line segment to the screen boundaries using the Cohen–Sutherland algorithm.
		/// </summary>
		/// <param name="p0">Reference to the first endpoint of the line segment. On return, may be modified to the clipped position.</param>
		/// <param name="p1">Reference to the second endpoint of the line segment. On return, may be modified to the clipped position.</param>
		/// <returns>true if the line segment is at least partially within the screen and has been clipped (if necessary); false if the line segment lies entirely outside the screen.</returns>
		static bool ClipLine(pixel_point_t& p0, pixel_point_t& p1)
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
						x = x0 + (x1 - x0) * (0 - y0) / (y1 - y0);
						y = 0;
					}
					else if (outcodeOut & (uint8_t)OutcodeEnum::OUT_BOTTOM)
					{
						if (y1 == y0) return false; // Parallel to bottom
						x = x0 + (x1 - x0) * (FrameHeight - 1 - y0) / (y1 - y0);
						y = FrameHeight - 1;
					}
					else if (outcodeOut & (uint8_t)OutcodeEnum::OUT_RIGHT)
					{
						if (x1 == x0) return false; // Parallel to right
						y = y0 + (y1 - y0) * (FrameWidth - 1 - x0) / (x1 - x0);
						x = FrameWidth - 1;
					}
					else // OUT_LEFT
					{
						if (x1 == x0) return false; // Parallel to left
						y = y0 + (y1 - y0) * (0 - x0) / (x1 - x0);
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
		static uint8_t ComputeOutCode(const pixel_t x, const pixel_t y)
		{
			uint8_t code = 0;
			if (x < 0) code |= (uint8_t)OutcodeEnum::OUT_LEFT;
			else if (x >= FrameWidth) code |= (uint8_t)OutcodeEnum::OUT_RIGHT;
			if (y < 0) code |= (uint8_t)OutcodeEnum::OUT_TOP;
			else if (y >= FrameHeight) code |= (uint8_t)OutcodeEnum::OUT_BOTTOM;

			return code;
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
				PixelRawBlend(rawColor, aax1, y);
			if (aax2 >= 0 && aax2 < FrameWidth)
				PixelRawBlend(rawColor, aax2, y);
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

			// For left-to-right scanning:
			// - Left edge: high alpha when x1 fraction is LOW (just entered the shape)
			// - Right edge: high alpha when x2 fraction is HIGH (about to exit the shape)
			if (aax1 >= 0 && aax1 < FrameWidth)
			{
				if (leftToRight)
				{
					// Left edge: invert the sub-pixel position for correct coverage
					const uint8_t alpha = (255 - (((x1 & ((int32_t(1) << BRESENHAM_SCALE) - 1)) * 255) >> BRESENHAM_SCALE)) >> 0;
					PixelRawBlendAlpha(rawColor, aax1, y, alpha);
				}
				else
				{
					// Right-to-left: normal sub-pixel position for correct coverage
					const uint8_t alpha = ((x1 & ((int32_t(1) << BRESENHAM_SCALE) - 1)) * 255) >> (BRESENHAM_SCALE + 0);
					PixelRawBlendAlpha(rawColor, aax1, y, alpha);
				}
			}

			if (aax2 >= 0 && aax2 < FrameWidth)
			{
				if (leftToRight) {
					// Right edge: normal sub-pixel position for correct coverage
					const uint8_t alpha = ((x2 & ((int32_t(1) << BRESENHAM_SCALE) - 1)) * 255) >> (BRESENHAM_SCALE + 0);
					PixelRawBlendAlpha(rawColor, aax2, y, alpha);
				}
				else
				{
					// Right-to-left: invert the sub-pixel position for correct coverage
					const uint8_t alpha = (255 - (((x2 & ((int32_t(1) << BRESENHAM_SCALE) - 1)) * 255) >> BRESENHAM_SCALE)) >> 0;
					PixelRawBlendAlpha(rawColor, aax2, y, alpha);
				}
			}
		}
	};
}
#endif