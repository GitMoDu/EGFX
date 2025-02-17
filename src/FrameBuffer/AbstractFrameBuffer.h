// AbstractFrameBuffer.h

#ifndef _ABSTRACT_FRAME_BUFFER_h
#define _ABSTRACT_FRAME_BUFFER_h

#include "../Model/RgbColor.h"
#include "../Model/DisplayOrientation.h"
#include "../Model/ColorConverter.h"
#include "../Model/IFrameBuffer.h"

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
		, DisplayMirrorEnum displayMirror = DisplayMirrorEnum::NoMirror
		, DisplayRotationEnum displayRotation = DisplayRotationEnum::NoRotation>
	class AbstractFrameBuffer : public virtual IFrameBuffer
	{
	public:
		static constexpr pixel_t FrameWidth = frameWidth;
		static constexpr pixel_t FrameHeight = frameHeight;
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
		virtual void WritePixel(const color_t rawColor, const pixel_t x, const pixel_t y) {}

	public:
		AbstractFrameBuffer(uint8_t buffer[BufferSize] = nullptr)
			: IFrameBuffer()
			, Buffer(buffer)
		{
		}

	public:
		virtual const pixel_t GetFrameWidth() const final
		{
			return FrameWidth;
		}

		virtual const pixel_t GetFrameHeight() const final
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

		virtual void Pixel(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			PixelRaw(ColorConverter::GetRawColor(color), x, y);
		}

	private:
		void PixelRaw(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
#if defined(GRAPHICS_ENGINE_DEBUG)
			if (x >= FrameWidth
				|| y >= FrameHeight)
			{
				Serial.print(F("x,y "));
				Serial.print(x);
				Serial.print(',');
				Serial.println(y);
				return;
			}
#endif
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
		virtual void LineVertical(const color_t rawColor, const pixel_t x, const pixel_t y, const pixel_t height)
		{
#if defined(GRAPHICS_ENGINE_DEBUG)
			if (x >= FrameWidth
				|| y >= FrameHeight
				|| height > FrameHeight - y)
			{
				Serial.println(F("LV x,y "));
				Serial.print(x);
				Serial.print(',');
				Serial.print(y);
				Serial.print('\t');
				Serial.println(height);
				return;
			}
#endif
			for (pixel_t i = 0; i < height; i++)
			{
				PixelRaw(rawColor, x, y + i);
			}
		}

		/// <summary>
		/// Optimizeable but requires orientation and rotation awareness.
		/// </summary>
		virtual void LineHorizontal(const color_t rawColor, const pixel_t x, const pixel_t y, const pixel_t width)
		{
#if defined(GRAPHICS_ENGINE_DEBUG)
			if (x >= FrameWidth
				|| y >= FrameHeight
				|| width > FrameWidth - x)
			{
				Serial.println(F("LH x,y "));
				Serial.print(x);
				Serial.print(',');
				Serial.print(y);
				Serial.print('\t');
				Serial.println(width);
				return;
			}
#endif

			for (pixel_t i = 0; i < width; i++)
			{
				PixelRaw(rawColor, x + i, y);
			}
		}

	public:
		virtual void Fill(const rgb_color_t color)
		{
			RectangleFill(color, 0, 0, FrameWidth - 1, FrameHeight - 1);
		}

		virtual void Line(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2) final
		{
			if (x1 == x2)
			{
				if (y1 == y2)
				{
					PixelRaw(ColorConverter::GetRawColor(color), x1, y1);
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

		virtual void RectangleFill(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
		{
#if defined(GRAPHICS_ENGINE_DEBUG)
			if (x1 >= x2
				|| x2 >= FrameWidth
				|| y1 >= y2
				|| y2 >= FrameHeight)
			{
				Serial.println(F("RF x,y "));
				Serial.print(x1);
				Serial.print(',');
				Serial.print(y1);
				Serial.print('\t');
				Serial.print(x2);
				Serial.print(',');
				Serial.println(y2);
				return;
			}
#endif

			const color_t rawColor = ColorConverter::GetRawColor(color);
			for (pixel_t y = y1; y <= y2; y++)
			{
				LineHorizontal(rawColor, x1, y, x2 - x1 + 1);
			}
		}

		virtual void Rectangle(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2) final
		{
#if defined(GRAPHICS_ENGINE_DEBUG)
			if (x1 >= x2
				|| x2 >= FrameWidth
				|| y1 >= y2
				|| y2 >= FrameHeight)
			{
				Serial.println(F("R x,y"));
				Serial.print(x1);
				Serial.print(',');
				Serial.print(y1);
				Serial.print('\t');
				Serial.print(x2);
				Serial.print(',');
				Serial.println(y2);
				return;
			}
#endif

			const color_t rawColor = ColorConverter::GetRawColor(color);
			LineHorizontal(rawColor, x1, y1, x2 - x1);
			LineHorizontal(rawColor, x1, y2 - 1, x2 - x1);
			LineVertical(rawColor, x1, y1 + 1, y2 - y1 - 2);
			LineVertical(rawColor, x2 - 1, y1 + 1, y2 - y1 - 2);
		}

	private:
		void BresenhamRightUp(const color_t rawColor, const pixel_t x1, const pixel_t y1, const pixel_t y2, const pixel_t width)
		{
			const uint16_t slopeMagnitude = 2 * (y2 - y1);

			int16_t slopeError = slopeMagnitude - width;
			pixel_t y = y1;

			for (pixel_t x = x1; x < x1 + width; x++)
			{
				PixelRaw(rawColor, x, y);

				slopeError += slopeMagnitude;
				if (slopeError >= 0)
				{
					y++;
					slopeError -= 2 * width;
				}
			}
		}

		void BresenhamRightDown(const color_t rawColor, const pixel_t x1, const pixel_t y1, const pixel_t y2, const pixel_t width)
		{
			const uint16_t slopeMagnitude = 2 * (y1 - y2);

			int16_t slopeError = slopeMagnitude - width;
			pixel_t y = y1;

			for (pixel_t x = x1; x < x1 + width; x++)
			{
				PixelRaw(rawColor, x, y);

				slopeError += slopeMagnitude;
				if (slopeError >= 0)
				{
					y--;
					slopeError -= 2 * width;
				}
			}
		}

		void BresenhamUpRight(const color_t rawColor, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t height)
		{
			const uint16_t slopeMagnitude = 2 * (x2 - x1);

			int16_t slopeError = slopeMagnitude - height;
			pixel_t x = x1;

			for (pixel_t y = y1; y < y1 + height; y++)
			{
				PixelRaw(rawColor, x, y);

				slopeError += slopeMagnitude;
				if (slopeError >= 0)
				{
					x++;
					slopeError -= 2 * height;
				}
			}
		}

		void BresenhamUpLeft(const color_t rawColor, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t height)
		{
			const uint16_t slopeMagnitude = 2 * (x1 - x2);

			int16_t slopeError = slopeMagnitude - height;
			pixel_t x = x1;

			for (pixel_t y = y1; y < y1 + height; y++)
			{
				PixelRaw(rawColor, x, y);

				slopeError += slopeMagnitude;
				if (slopeError >= 0)
				{
					x--;
					slopeError -= 2 * height;
				}
			}
		}
	};
}
#endif