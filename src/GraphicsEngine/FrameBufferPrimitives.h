// FrameBufferPrimitives.h

#ifndef _FRAME_BUFFER_EFFECTS_h
#define _FRAME_BUFFER_EFFECTS_h

#include "IFrame/IFramePrimitives.h"
#include "GraphicsEngine/Geometry.h"

template<typename pixel_t>
class FrameBufferPrimitives : public virtual IFramePrimitives
{
public:
	const size_t GetOffset(const uint8_t x, const uint8_t y)
	{
		switch (DisplayAxis)
		{
		case IFrame::DisplayAxisEnum::MirrorX:
			return ((uint32_t)(((uint16_t)y * ScreenWidth) + (ScreenWidth - 1 - x))) * sizeof(pixel_t);
			break;
		case IFrame::DisplayAxisEnum::MirrorY:
			return ((uint32_t)(((uint16_t)(ScreenHeight - 1 - y) * ScreenWidth) + x)) * sizeof(pixel_t);
			break;
		case IFrame::DisplayAxisEnum::MirrorXY:
			return ((uint32_t)(((uint16_t)(ScreenHeight - 1 - y) * ScreenWidth) + (ScreenWidth - 1 - x))) * sizeof(pixel_t);
			break;
		case IFrame::DisplayAxisEnum::NoMirror:
		default:
			return ((uint32_t)(((uint16_t)y * ScreenWidth) + x)) * sizeof(pixel_t);
			break;
		}
	}

private:
	pixel_t* Frame;
	IColorConverter<pixel_t>* ColorConverter;

	const uint32_t BufferSize;

	const uint8_t ScreenWidth;
	const uint8_t ScreenHeight;
	const IFrame::DisplayAxisEnum DisplayAxis;
	const IFrame::FrameRotationEnum DisplayRotation;//TODO:
	const pixel_t BlankColor = 0;

public:
	FrameBufferPrimitives(pixel_t* frame,
		const uint8_t width,
		const uint8_t height,
		IColorConverter<pixel_t>* colorConverter,
		IFrame::DisplayAxisEnum displayAxis = IFrame::DisplayAxisEnum::NoMirror,
		IFrame::FrameRotationEnum displayRotation = IFrame::FrameRotationEnum::NoRotation)
		: IFramePrimitives()
		, Frame(frame)
		, ColorConverter(colorConverter)
		, BufferSize(IFrame::GetBufferSize<pixel_t>(width, height))
		, ScreenWidth(width)
		, ScreenHeight(height)
		, DisplayAxis(displayAxis)
		, DisplayRotation(displayRotation)
	{}

	void SetFrameBuffer(pixel_t* frame)
	{
		Frame = frame;
	}

	virtual const uint8_t GetWidth() final
	{
		return ScreenWidth;
	}

	virtual const uint8_t GetHeight() final
	{
		return ScreenHeight;
	}

	virtual const uint8_t* GetFrameBuffer() final
	{
		return (const uint8_t*)Frame;
	}

	virtual void ClearFrame() final
	{
		for (uint32_t i = 0; i < BufferSize; i++)
		{
			((uint8_t*)Frame)[i] = BlankColor;
		}
	}

	virtual void Pixel(const uint8_t x, const uint8_t y, const RgbColor& color) final
	{
		Frame[GetOffset(x, y)] = ColorConverter->GetRawColor(color);
	}

	virtual void Line(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const RgbColor& color)
	{
		if (x1 == x2)
		{
			VerticalLine(x1, y1, y2, ColorConverter->GetRawColor(color));
		}
		else if (y1 == y2)
		{
			HorizontalLine(x1, x2, y1, ColorConverter->GetRawColor(color));
		}
		else
		{
			DiagonalLine(x1, y1, x2, y2, ColorConverter->GetRawColor(color));
		}
	}

	virtual void RectangleFill(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const RgbColor& color) final
	{
#if defined(DEBUG)
		if (x2 < x1
			|| y2 < y1)
		{
			Serial.println(F("Invalid parameters."));
			Serial.print(F("x1: "));
			Serial.println(x1);
			Serial.print(F("y1: "));
			Serial.println(y1);
			Serial.print(F("x2: "));
			Serial.println(x2);
			Serial.print(F("y2: "));
			Serial.println(y2);
			Serial.println();
			Serial.print(F("Screen: "));
			Serial.print(ScreenWidth);
			Serial.print('x');
			Serial.println(ScreenHeight);

			return;
		}
#endif
		if ((x1 < ScreenWidth)
			&& (y1 < ScreenHeight))
		{
			const pixel_t rawColor = ColorConverter->GetRawColor(color);
			for (uint_fast8_t x = x1; x <= x2; x++)
			{
				if (x < ScreenWidth)
				{
					for (uint_fast8_t y = y1; y <= y2; y++)
					{
						if (y < ScreenHeight)
						{
							Frame[GetOffset(x, y)] = rawColor;
						}
					}
				}
			}
		}
	}
	virtual void Rectangle(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const RgbColor& color, const uint8_t width) final
	{
#if defined(DEBUG)
		if (x2 < x1
			|| y2 < y1)
		{
			Serial.println(F("Invalid parameters."));
			Serial.print(F("x1: "));
			Serial.println(x1);
			Serial.print(F("y1: "));
			Serial.println(y1);
			Serial.print(F("x2: "));
			Serial.println(x2);
			Serial.print(F("y2: "));
			Serial.println(y2);
			Serial.println();
			Serial.print(F("Screen: "));
			Serial.print(ScreenWidth);
			Serial.print('x');
			Serial.println(ScreenHeight);

			return;
		}
#endif

		if (x1 < ScreenWidth)
		{
			const pixel_t rawColor = ColorConverter->GetRawColor(color);
			for (uint_fast8_t x = x1; x < x2; x++)
			{
				if (x < ScreenWidth)
				{
					if (y1 < ScreenHeight)
					{
						Frame[GetOffset(x, y1)] = rawColor;
					}
					if (y2 < ScreenHeight)
					{
						Frame[GetOffset(x, y2 - 1)] = rawColor;
					}
				}

			}
			for (uint_fast8_t y = y1; y < y2; y++)
			{
				if (y < ScreenHeight)
				{
					Frame[GetOffset(x1, y)] = rawColor;
					Frame[GetOffset(x2 - 1, y)] = rawColor;
				}
			}
		}
	}

	/// <summary>
	/// Based on https://github.com/Sarah-Hesham-2022/ComputerGraphics-Algorithms-With-CPP
	/// </summary>
	virtual void CircleFill(const uint8_t xCenter, const uint8_t yCenter, const uint8_t radius, const RgbColor& color) final
	{
		if ((xCenter < ScreenWidth)
			&& (yCenter < ScreenHeight))
		{
			const pixel_t rawColor = ColorConverter->GetRawColor(color);
			const uint8_t isEven = (radius & 0x01) == 0;
			const uint8_t effectiveRadius = radius - isEven;
			uint_fast8_t x = 0;
			uint_fast8_t y = effectiveRadius;
			int16_t d1 = 1 - effectiveRadius;
			int_fast8_t c1 = 3;
			uint_fast8_t c2 = 5 - 2 * effectiveRadius;

			// Fill center rectangle.
			const uint_fast8_t squareDimension = Geometry::GetSquareInsideCircleDimension(radius);

			const uint_fast8_t squareStartX = (xCenter - (squareDimension / 2));
			const uint_fast8_t squareStartY = (yCenter - (squareDimension / 2));
			const uint_fast8_t squareEndX = (xCenter + (squareDimension / 2));
			const uint_fast8_t squareEndY = (yCenter + (squareDimension / 2));

			for (uint_fast8_t x = squareStartX; x <= squareEndX; x++)
			{
				for (uint_fast8_t y = squareStartY; y <= squareEndY; y++)
				{
					SafeWrite(x, y, rawColor);
				}
			}

			// Fill circle lines.
			while (x < y)
			{
				if (d1 < 0)
				{
					d1 += c1;
					c2 += 2;
				}
				else
				{
					d1 += c2;
					c2 += 4;
					y--;
				}
				c1 += 2;
				x++;
				DrawCircleLines(xCenter, yCenter, x, y, rawColor);
			}
		}
	}

	/// <summary>
	/// Based on https://github.com/Sarah-Hesham-2022/ComputerGraphics-Algorithms-With-CPP
	/// </summary>
	virtual void Circle(const uint8_t xCenter, const uint8_t yCenter, const uint8_t radius, const RgbColor& color)  final
	{
		if ((xCenter < ScreenWidth)
			&& (yCenter < ScreenHeight))
		{
			const pixel_t rawColor = ColorConverter->GetRawColor(color);
			const uint8_t isEven = (radius & 0x01) == 0;
			const uint8_t effectiveRadius = radius - isEven;


			uint_fast8_t x = 0;
			uint_fast8_t y = effectiveRadius;
			int16_t d1 = 1 - effectiveRadius;
			int_fast8_t c1 = 3;
			uint_fast8_t c2 = 5 - (2 * effectiveRadius);
			DrawCirclePoints(xCenter, yCenter, x, y, rawColor);
			while (x < y)
			{
				if (d1 < 0)
				{
					d1 += c1;
					c2 += 2;
				}
				else
				{
					d1 += c2;
					c2 += 4;
					y--;
				}
				c1 += 2;
				x++;
				DrawCirclePoints(xCenter, yCenter, x, y, rawColor);
			}
		}
	}

private:
	void HorizontalLine(const uint8_t x1, const uint8_t x2, const uint8_t y, const pixel_t rawColor)
	{
		if (y < ScreenHeight)
		{
			for (uint_fast8_t x = x1; x <= x2; x++)
			{
				if (x < ScreenWidth)
				{
					Frame[GetOffset(x, y)] = rawColor;
				}
			}
		}
	}

	void VerticalLine(const uint8_t x, const uint8_t y1, const uint8_t y2, const pixel_t rawColor)
	{
		if (x < ScreenWidth)
		{
			for (uint_fast8_t y = y1; y <= y2; y++)
			{
				if (y < ScreenHeight)
				{
					Frame[GetOffset(x, y)] = rawColor;
				}
			}
		}
	}

	/// <summary>
	/// Based on https://github.com/Sarah-Hesham-2022/ComputerGraphics-Algorithms-With-CPP
	/// </summary>
	void DiagonalLine(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const pixel_t rawColor)
	{
		if (Absolute(y2 - y1) < Absolute(x2 - x1))
		{
			if (x1 > x2)
			{
				PlotLineLow(x2, y2, x1, y1, rawColor);
			}
			else
			{
				PlotLineLow(x1, y1, x2, y2, rawColor);
			}
		}
		else
		{
			if (y1 > y2)
			{
				PlotLineHigh(x2, y2, x1, y1, rawColor);
			}
			else
			{
				PlotLineHigh(x1, y1, x2, y2, rawColor);
			}
		}
	}

private:
	void SafeWrite(const uint8_t x, const uint8_t y, const pixel_t rawColor)
	{
		if ((x < ScreenWidth)
			&& (y < ScreenHeight))
		{
			Frame[GetOffset(x, y)] = rawColor;
		}
	}

	void PlotLineLow(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const pixel_t rawColor)
	{
		const int_fast8_t dx = x2 - x1;
		int_fast8_t dy = y2 - y1;
		int_fast8_t yi = 1;
		int_fast16_t D = (int_fast16_t)(2 * dy) - dx;
		uint_fast8_t y = y1;

		if (dy < 0)
		{
			yi = -1;
			dy = -dy;
		}

		for (uint_fast8_t x = x1; x < x2; x++)
		{
			SafeWrite(x, y, rawColor);
			if (D > 0)
			{
				y = y + yi;
				D = D + (2 * (dy - dx));
			}
			else
			{
				D = D + 2 * dy;
			}
		}
	}

	void PlotLineHigh(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const pixel_t rawColor)
	{
		const int_fast8_t dy = y2 - y1;
		int_fast8_t dx = x2 - x1;
		int_fast8_t xi = 1;
		int_fast16_t D = (int_fast16_t)(2 * dx) - dy;
		uint_fast8_t x = x1;

		if (dx < 0)
		{
			xi = -1;
			dx = -dx;
		}

		for (uint_fast8_t y = y1; y < y2; y++)
		{
			SafeWrite(x, y, rawColor);
			if (D > 0)
			{
				x = x + xi;
				D = D + (2 * (dx - dy));
			}
			else
			{
				D = D + 2 * dx;
			}
		}
	}

	void DrawCirclePoints(const uint8_t xCenter, const uint8_t yCenter, const uint8_t a, const uint8_t b, const pixel_t rawColor)
	{
		SafeWrite(xCenter + a, yCenter + b, rawColor);
		SafeWrite(xCenter + a, yCenter - b, rawColor);
		SafeWrite(xCenter - a, yCenter + b, rawColor);
		SafeWrite(xCenter - a, yCenter - b, rawColor);

		SafeWrite(xCenter + b, yCenter + a, rawColor);
		SafeWrite(xCenter - b, yCenter + a, rawColor);
		SafeWrite(xCenter + b, yCenter - a, rawColor);
		SafeWrite(xCenter - b, yCenter - a, rawColor);
	}

	void DrawCircleLines(const uint8_t xCenter, const uint8_t yCenter, const uint8_t a, const uint8_t b, const pixel_t rawColor)
	{
		HorizontalLine(xCenter - a, xCenter + a, yCenter + b, rawColor);
		HorizontalLine(xCenter - a, xCenter + a, yCenter - b, rawColor);

		VerticalLine(xCenter - b, yCenter - a, yCenter + a, rawColor);
		VerticalLine(xCenter + b, yCenter - a, yCenter + a, rawColor);
	}

	static const uint8_t Absolute(const int8_t value)
	{
		if (value >= 0)
		{
			return value;
		}
		else
		{
			return -value;
		}
	}
};
#endif