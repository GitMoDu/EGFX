// MonochromeFrameBuffer.h

#ifndef _MONOCHROME_FRAME_BUFFER_h
#define _MONOCHROME_FRAME_BUFFER_h

#include "AbstractFrameBuffer.h"

/// <summary>
/// 1 bit monochrome frame buffer.
/// </summary>
/// <typeparam name="frameWidth">Frame buffer width [0;254].</typeparam>
/// <typeparam name="frameHeight">Frame buffer height [0;254].</typeparam>
/// <typeparam name="ColorConverter">Must be an implementation of AbstractColorConverter8.</typeparam>
/// <typeparam name="displayAxis">Display mirror option.</typeparam>
/// <typeparam name="displayRotation">Display rotation option.</typeparam>
template<const uint8_t frameWidth, const uint8_t frameHeight
	, typename ColorConverter = BinaryColorConverter1
	, DisplayMirrorEnum displayMirror = DisplayMirrorEnum::NoMirror
	, DisplayRotationEnum displayRotation = DisplayRotationEnum::NoRotation>
class MonochromeFrameBuffer final
	: public AbstractFrameBuffer<ColorConverter, frameWidth, frameHeight, displayMirror, displayRotation>
{
private:
	using BaseClass = AbstractFrameBuffer<ColorConverter, frameWidth, frameHeight, displayMirror, displayRotation>;

public:
	using BaseClass::BufferSize;

protected:
	using BaseClass::Buffer;
	using BaseClass::Inverted;
	using BaseClass::DisplayMirror;
	using typename BaseClass::color_t;

public:
	MonochromeFrameBuffer(uint8_t buffer[BufferSize] = nullptr)
		: BaseClass(buffer)
	{}

	const uint32_t GetColorDepth() const  final
	{
		return 1;
	}

	void Fill(const RgbColor& color) final
	{
		const uint8_t rawColor = ColorConverter::GetRawColor(color);
		if (((bool)(rawColor > 0) ^ (bool)Inverted))
		{
			memset(Buffer, UINT8_MAX, BufferSize);
		}
		else
		{
			memset(Buffer, 0, BufferSize);
		}
	}

protected:
	void WritePixel(const color_t rawColor, const uint8_t x, const uint8_t y) final
	{
		const uint8_t yByte = y / 8;
		const uint8_t yBit = y % 8;

		const size_t offset = ((sizeof(color_t) * frameWidth) * yByte) + x;

		if (((bool)(rawColor > 0) ^ (bool)Inverted))
		{
			Buffer[offset] |= 1 << yBit;
		}
		else
		{
			Buffer[offset] &= ~(1 << yBit);
		}
	}

	/// <summary>
	/// Optimized version.
	/// </summary>
	/// <param name="rawColor"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="width"></param>
	void LineHorizontal(const color_t rawColor, const uint8_t x, const uint8_t y, const uint8_t width) final
	{
		uint8_t x1 = 0;
		uint8_t yByte = 0;
		uint8_t yBit = 0;

		//TODO: Handle rotation. This switches Width and Height around.
		switch (DisplayMirror)
		{
		case DisplayMirrorEnum::MirrorY:
			x1 = x;
			yByte = (frameHeight - 1 - y) / 8;
			yBit = (frameHeight - 1 - y) % 8;
			break;
		case DisplayMirrorEnum::MirrorXY:
			x1 = frameWidth - width - x;
			yByte = (frameHeight - 1 - y) / 8;
			yBit = (frameHeight - 1 - y) % 8;
			break;
		case DisplayMirrorEnum::MirrorX:
			x1 = frameWidth - width - x;
			yByte = y / 8;
			yBit = y % 8;
			break;
		case DisplayMirrorEnum::NoMirror:
		default:
			x1 = x;
			yByte = y / 8;
			yBit = y % 8;
			break;
		}

		const size_t offset = ((sizeof(color_t) * frameWidth) * yByte) + x1;

		if (((bool)(rawColor > 0) ^ (bool)Inverted))
		{
			for (uint_fast8_t i = 0; i < width; i++)
			{
				Buffer[offset + i] |= 1 << yBit;
			}
		}
		else
		{
			for (uint_fast8_t i = 0; i < width; i++)
			{
				Buffer[offset + i] &= ~(1 << yBit);
			}
		}
	}

	/// <summary>
	/// Optimized version.
	/// TODO: fix address issues and rename to LineVertical.
	/// </summary>
	void LineVerticalBuggy(const color_t rawColor, const uint8_t x, const uint8_t y, const uint8_t height)
	{
		if (x >= frameWidth
			|| y >= frameHeight
			|| height > frameHeight - y)
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

		uint8_t x1 = 0;
		uint8_t y1 = 0;

		//TODO: Handle rotation. This switches Width and Height around.
		switch (DisplayMirror)
		{
		case DisplayMirrorEnum::MirrorY:
			x1 = x;
			y1 = frameHeight - height - y;
			break;
		case DisplayMirrorEnum::MirrorXY:
			x1 = frameWidth - 1 - x;
			y1 = frameHeight - height - y;
			break;
		case DisplayMirrorEnum::MirrorX:
			x1 = frameWidth - 1 - x;
			y1 = y;
			break;
		case DisplayMirrorEnum::NoMirror:
		default:
			x1 = x;
			y1 = y;
			break;
		}

		if (((bool)(rawColor > 0) ^ (bool)Inverted))
		{
			if (height >= 8)
			{
				const uint8_t byte0 = y1 / 8;
				uint8_t byte1;
				if (y1 == (byte0 * 8))
				{
					byte1 = byte0;
				}
				else
				{
					byte1 = byte0 + 1;

					const uint8_t yBitStart = ((byte1 * 8) - y1);
					for (uint_fast8_t i = 0; i < yBitStart; i++)
					{
						const uint8_t yByte = (y1 + i) / 8;
						const uint8_t yBit = (y1 + i) % 8;

						const size_t offset = (sizeof(color_t) * ((size_t)yByte * frameWidth)) + x1;

						Buffer[offset] |= 1 << yBit;
					}
				}

				const uint8_t byteHeight = (height / 8) - byte1;
				for (uint_fast8_t i = 0; i < byteHeight; i++)
				{
					const uint8_t yByte = byte1 + i;
					const size_t offset = (sizeof(color_t) * ((size_t)yByte * frameWidth)) + x1;

					Buffer[offset] = UINT8_MAX;
				}

				const uint8_t broadHeight = byteHeight * 8;
				if (height > broadHeight)
				{
					for (uint_fast8_t i = broadHeight; i < height; i++)
					{
						const uint8_t yByte = ((y1 + i) / 8);
						const uint8_t yBit = (y1 + i) % 8;

						const size_t offset = (sizeof(color_t) * ((size_t)yByte * frameWidth)) + x1;

						Buffer[offset] |= 1 << yBit;
					}
				}
			}
			else
			{
				for (uint_fast8_t i = 0; i < height; i++)
				{
					const uint8_t yByte = (y1 + i) / 8;
					const uint8_t yBit = (y1 + i) % 8;

					const size_t offset = (sizeof(color_t) * ((size_t)yByte * frameWidth)) + x1;

					Buffer[offset] |= 1 << yBit;
				}
			}
		}
		else
		{
			for (uint_fast8_t i = 0; i < height; i++)
			{
				const uint8_t yByte = (y1 + i) / 8;
				const uint8_t yBit = (y1 + i) % 8;

				const size_t offset = (sizeof(color_t) * ((size_t)yByte * frameWidth)) + x1;

				Buffer[offset] &= ~(1 << yBit);
			}
		}
}
};
#endif