// SpriteFontRenderer.h

#ifndef _SPRITE_FONT_RENDERER_h
#define _SPRITE_FONT_RENDERER_h

#include <String.h>

template<const uint8_t fontWidth,
	const uint8_t fontHeight>
class AbstractSpriteFontRenderer
{
public:
	static constexpr uint8_t FontWidth = fontWidth;
	static constexpr uint8_t FontHeight = fontHeight;

private:
	static constexpr uint8_t BitScale = ((fontWidth - 1) / 8) + 1;

protected:
	virtual const uint8_t* GetMask(const int8_t character) { return nullptr; }

public:
	AbstractSpriteFontRenderer()
	{}

public:
	void TextBottomLeft(IFrameBuffer* frame, const RgbColor& color, const uint8_t x, const uint8_t y, const char* str)
	{
		TextTopLeft(frame, color, x, y - FontHeight, str);
	}

	void TextBottomRight(IFrameBuffer* frame, const RgbColor& color, const uint8_t x, const uint8_t y, const char* str)
	{
		TextTopRight(frame, x, y - FontHeight, str);
	}

	void TextTopLeft(IFrameBuffer* frame, const RgbColor& color, const uint8_t x, const uint8_t y, const char* str)
	{
		const uint8_t width = frame->GetWidth();
		const uint8_t height = frame->GetHeight();

		if (str != NULL
			&& y < height)
		{
			size_t size = strlen(str);
			uint8_t* ch = (uint8_t*)str;
			uint8_t offset = 0;
			while (size--
				&& (x < (width - offset)))
			{
				Write(frame, color, x + offset, y, (const uint8_t)*ch++);
				offset += FontWidth + 1;
			}
		}
	}

	void TextTopRight(IFrameBuffer* frame, const RgbColor& color, const uint8_t x, const uint8_t y, const char* str)
	{
		const uint8_t width = frame->GetWidth();
		const uint8_t height = frame->GetHeight();

		if (str != NULL
			&& y < height)
		{
			size_t size = strlen(str);
			uint8_t* ch = (uint8_t*)str + size - 1;
			uint8_t offset = 0;
			while (size--
				&& (x < (width - offset)))
			{
				Write(frame, color, x - offset, y, (const uint8_t)*ch--);
				offset += FontWidth + 1;
			}
		}
	}

	void Write(IFrameBuffer* frame, const RgbColor& color, const uint8_t positionX, const uint8_t positionY, const char character)
	{
		const uint8_t* mask = GetMask(character);

		if (mask == nullptr)
		{
			return;
		}

		uint8_t yByte = 0;
		uint8_t xByte = 0;
		uint8_t offset = 0;

		uint8_t xBit = 0;

		for (uint8_t x = 0; x < FontWidth; x++)
		{
			for (uint8_t y = 0; y < FontHeight; y++)
			{
				yByte = y * BitScale;
				xByte = (x / 8);
				offset = yByte + xByte;
				xBit = 7 - (x % 8);

#if defined(ARDUINO_ARCH_AVR)
				if (((pgm_read_byte(&mask[offset]) >> xBit) & 0x01) != 0)
#else
				if (((mask[offset] >> xBit) & 0x01) != 0)
#endif
				{
					frame->Pixel(color, positionX + x, positionY + y);
				}
			}
		}
	}
};

#endif