// AbstractFrameBuffer.h

#ifndef _ABSTRACT_FRAME_BUFFER_h
#define _ABSTRACT_FRAME_BUFFER_h

#include "../Model/RgbColor.h"
#include "../Model/DisplayOrientation.h"
#include "../Model/ColorConverter.h"
#include "../Model/IFrameBuffer.h"

/// <summary>
/// Abstract frame buffer, with primitive implementation.
/// Diagonal (Bresenham) Line implementation based on https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/
/// </summary>
/// <typeparam name="ColorConverter">Must be an implementation of AbstractColorConverter.</typeparam>
/// <typeparam name="clearDivisorPower">Frame buffer clear will be divided into sections. The divisor is set by the power of 2, keeping it a multiple of 2.</typeparam>
/// <typeparam name="frameWidth">Frame buffer width [0;254].</typeparam>
/// <typeparam name="frameHeight">Frame buffer height [0;254].</typeparam>
/// <typeparam name="displayAxis">Display mirror option.</typeparam>
/// <typeparam name="displayRotation">Display rotation option.</typeparam>
template<typename ColorConverter
	, const uint8_t clearDivisorPower
	, const uint8_t frameWidth
	, const uint8_t frameHeight
	, DisplayMirrorEnum displayMirror = DisplayMirrorEnum::NoMirror
	, DisplayRotationEnum displayRotation = DisplayRotationEnum::NoRotation>
class AbstractFrameBuffer : public virtual IFrameBuffer
{
public:
	static constexpr uint8_t FrameWidth = frameWidth;
	static constexpr uint8_t FrameHeight = frameHeight;
	static constexpr DisplayMirrorEnum DisplayMirror = displayMirror;
	static constexpr DisplayRotationEnum DisplayRotation = displayRotation;
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
	virtual void WritePixel(const color_t rawColor, const uint8_t x, const uint8_t y) {}

public:
	AbstractFrameBuffer(uint8_t buffer[BufferSize] = nullptr)
		: IFrameBuffer()
		, Buffer(buffer)
	{}

public:
	virtual const uint8_t GetWidth() const final
	{
		return FrameWidth;
	}

	virtual const uint8_t GetHeight() const final
	{
		return FrameHeight;
	}

	virtual void SetInverted(const bool inverted) final
	{
		Inverted = inverted;
	}

	virtual const uint8_t* GetFrameBuffer() final
	{
		return (const uint8_t*)Buffer;
	}

	virtual const bool ClearFrameBuffer() final
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

private:
	void Pixel(const color_t rawColor, const uint8_t x, const uint8_t y)
	{
		if (x >= FrameWidth
			|| y >= FrameHeight)
		{
#if defined(GRAPHICS_ENGINE_DEBUG)
			Serial.print(F("x,y "));
			Serial.print(x);
			Serial.print(',');
			Serial.println(y);
#endif
			return;
		}
		//TODO: Handle rotation. This switches Width and Height around.
		switch (DisplayMirror)
		{
		case DisplayMirrorEnum::MirrorX:
			WritePixel(rawColor, FrameWidth - 1 - x, y);
			break;
		case DisplayMirrorEnum::MirrorY:
			WritePixel(rawColor, x, FrameHeight - 1 - y);
			break;
		case DisplayMirrorEnum::MirrorXY:
			WritePixel(rawColor, FrameWidth - 1 - x, FrameHeight - 1 - y);
			break;
		case DisplayMirrorEnum::NoMirror:
		default:
			WritePixel(rawColor, x, y);
			break;
		}
	}

protected:
	/// <summary>
	/// Optimizeable but requires orientation and rotation awareness.
	/// </summary>
	virtual void LineVertical(const color_t rawColor, const uint8_t x, const uint8_t y, const uint8_t height)
	{
		if (x >= FrameWidth
			|| y >= FrameHeight
			|| height > FrameHeight - y)
		{
#if defined(GRAPHICS_ENGINE_DEBUG)
			Serial.println(F("LV x,y "));
			Serial.print(x);
			Serial.print(',');
			Serial.print(y);
			Serial.print('\t');
			Serial.println(height);
#endif
			return;
		}
		for (uint_fast8_t i = 0; i < height; i++)
		{
			Pixel(rawColor, x, y + i);
		}
	}

	/// <summary>
	/// Optimizeable but requires orientation and rotation awareness.
	/// </summary>
	virtual void LineHorizontal(const color_t rawColor, const uint8_t x, const uint8_t y, const uint8_t width)
	{
		if (x >= FrameWidth
			|| y >= FrameHeight
			|| width > FrameWidth - x)
		{
#if defined(GRAPHICS_ENGINE_DEBUG)
			Serial.println(F("LH x,y "));
			Serial.print(x);
			Serial.print(',');
			Serial.print(y);
			Serial.print('\t');
			Serial.println(width);
#endif
			return;
		}
		for (uint_fast8_t i = 0; i < width; i++)
		{
			Pixel(rawColor, x + i, y);
		}
	}

public:
	virtual void Fill(const RgbColor& color)
	{
		RectangleFill(color, 0, 0, FrameWidth - 1, FrameHeight - 1);
	}

	virtual void Pixel(const RgbColor& color, const uint8_t x, const uint8_t y) final
	{
		Pixel(ColorConverter::GetRawColor(color), x, y);
	}

	virtual void Line(const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2) final
	{
		if (x1 == x2)
		{
			if (y1 == y2)
			{
				Pixel(ColorConverter::GetRawColor(color), x1, y1);
			}
			else if (y2 > y1)
			{
				LineVertical(ColorConverter::GetRawColor(color), x1, y1, y2 - y1);
			}
			else
			{
				LineVertical(ColorConverter::GetRawColor(color), x1, y2 - 1, y1 - y2);
			}
		}
		else if (y1 == y2)
		{
			if (x2 > x1)
			{
				LineHorizontal(ColorConverter::GetRawColor(color), x1, y1, x2 - x1);
			}
			else
			{
				LineHorizontal(ColorConverter::GetRawColor(color), x2 + 1, y1, x1 - x2);
			}
		}
		else if (x2 > x1)
		{
			if (y2 > y1)
			{
				if (x2 - x1 > y2 - y1)
				{
					BresenhamRightUp(ColorConverter::GetRawColor(color), x1, y1, y2, x2 - x1);
				}
				else
				{
					BresenhamUpRight(ColorConverter::GetRawColor(color), x1, y1, x2, y2 - y1);
				}
			}
			else if (x2 - x1 > y1 - y2)
			{
				BresenhamRightDown(ColorConverter::GetRawColor(color), x1, y1, y2, x2 - x1);
			}
			else
			{
				BresenhamUpLeft(ColorConverter::GetRawColor(color), x2, y2, x1, y1 - y2);
			}
		}
		else if (y2 > y1)
		{
			if (x1 - x2 > y2 - y1)
			{
				BresenhamRightDown(ColorConverter::GetRawColor(color), x2, y2, y1, x1 - x2);
			}
			else
			{
				BresenhamUpLeft(ColorConverter::GetRawColor(color), x1, y1, x2, y2 - y1);
			}
		}
		else
		{
			if (x1 - x2 > y1 - y2)
			{
				BresenhamRightUp(ColorConverter::GetRawColor(color), x2, y2, y1, x1 - x2);
			}
			else
			{
				BresenhamUpRight(ColorConverter::GetRawColor(color), x2, y2, x1, y1 - y2);
			}
		}
	}

	virtual void RectangleFill(const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		if (x1 >= x2
			|| x2 >= FrameWidth
			|| y1 >= y2
			|| y2 >= FrameHeight)
		{
#if defined(GRAPHICS_ENGINE_DEBUG)
			Serial.println(F("RF x,y "));
			Serial.print(x1);
			Serial.print(',');
			Serial.print(y1);
			Serial.print('\t');
			Serial.print(x2);
			Serial.print(',');
			Serial.println(y2);
#endif
			return;
		}

		const color_t rawColor = ColorConverter::GetRawColor(color);
		for (uint_fast8_t y = y1; y <= y2; y++)
		{
			LineHorizontal(rawColor, x1, y, x2 - x1 + 1);
		}
	}

	virtual void Rectangle(const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2) final
	{
		if (x1 >= x2
			|| x2 >= FrameWidth
			|| y1 >= y2
			|| y2 >= FrameHeight)
		{
#if defined(GRAPHICS_ENGINE_DEBUG)
			Serial.println(F("R x,y"));
			Serial.print(x1);
			Serial.print(',');
			Serial.print(y1);
			Serial.print('\t');
			Serial.print(x2);
			Serial.print(',');
			Serial.println(y2);
#endif
			return;
		}

		const color_t rawColor = ColorConverter::GetRawColor(color);
		LineHorizontal(rawColor, x1, y1, x2 - x1);
		LineHorizontal(rawColor, x1, y2 - 1, x2 - x1);
		LineVertical(rawColor, x1, y1 + 1, y2 - y1 - 2);
		LineVertical(rawColor, x2 - 1, y1 + 1, y2 - y1 - 2);
	}

private:
	void BresenhamRightUp(const color_t rawColor, const uint8_t x1, const uint8_t y1, const uint8_t y2, const uint8_t width)
	{
		const uint16_t slopeMagnitude = 2 * (y2 - y1);

		int16_t slopeError = slopeMagnitude - width;
		uint8_t y = y1;

		for (uint8_t x = x1; x < x1 + width; x++)
		{
			Pixel(rawColor, x, y);

			slopeError += slopeMagnitude;
			if (slopeError >= 0)
			{
				y++;
				slopeError -= 2 * width;
			}
		}
	}

	void BresenhamRightDown(const color_t rawColor, const uint8_t x1, const uint8_t y1, const uint8_t y2, const uint8_t width)
	{
		const uint16_t slopeMagnitude = 2 * (y1 - y2);

		int16_t slopeError = slopeMagnitude - width;
		uint8_t y = y1;

		for (uint8_t x = x1; x < x1 + width; x++)
		{
			Pixel(rawColor, x, y);

			slopeError += slopeMagnitude;
			if (slopeError >= 0)
			{
				y--;
				slopeError -= 2 * width;
			}
		}
	}

	void BresenhamUpRight(const color_t rawColor, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t height)
	{
		const uint16_t slopeMagnitude = 2 * (x2 - x1);

		int16_t slopeError = slopeMagnitude - height;
		uint8_t x = x1;

		for (uint8_t y = y1; y < y1 + height; y++)
		{
			Pixel(rawColor, x, y);

			slopeError += slopeMagnitude;
			if (slopeError >= 0)
			{
				x++;
				slopeError -= 2 * height;
			}
		}
	}

	void BresenhamUpLeft(const color_t rawColor, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t height)
	{
		const uint16_t slopeMagnitude = 2 * (x1 - x2);

		int16_t slopeError = slopeMagnitude - height;
		uint8_t x = x1;

		for (uint8_t y = y1; y < y1 + height; y++)
		{
			Pixel(rawColor, x, y);

			slopeError += slopeMagnitude;
			if (slopeError >= 0)
			{
				x--;
				slopeError -= 2 * height;
			}
		}
	}
};
#endif