// SpriteFontRenderer.h

#ifndef _SPRITE_FONT_RENDERER_h
#define _SPRITE_FONT_RENDERER_h

#include <WString.h>
#include "../Model/Character.h"

template<typename SpriteType>
class AbstractSpriteFontRenderer
{
public:
	static constexpr uint8_t FontWidth = SpriteType::Width;
	static constexpr uint8_t FontHeight = SpriteType::Height;

private:
	class ColorSprite : public SpriteType
	{
	private:
		RgbColor Color{ UINT8_MAX ,UINT8_MAX ,UINT8_MAX };

	public:
		ColorSprite() : SpriteType()
		{}

		void SetColor(const RgbColor& color)
		{
			Color.r = color.r;
			Color.g = color.g;
			Color.b = color.b;
		}

	protected:
		virtual const bool GetColor(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			color.r = Color.r;
			color.g = Color.g;
			color.b = Color.b;

			return true;
		}
	};

private:
	ColorSprite SpriteSource{};

protected:
	virtual void SetCharacter(SpriteType& spriteSource, const int8_t character) { }

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

	void TextBottomLeft(IFrameBuffer* frame, const RgbColor& color, const uint8_t x, const uint8_t y, const __FlashStringHelper* ifsh)
	{
		TextTopLeft(frame, color, x, y - FontHeight, ifsh);
	}

	void TextBottomRight(IFrameBuffer* frame, const RgbColor& color, const uint8_t x, const uint8_t y, const __FlashStringHelper* ifsh)
	{
		TextTopRight(frame, color, x, y - FontHeight, ifsh);
	}

	void TextTopLeft(IFrameBuffer* frame, const RgbColor& color, const uint8_t x, const uint8_t y, const __FlashStringHelper* ifsh)
	{
#if defined(ARDUINO_ARCH_AVR)
		if (ifsh != NULL
			&& x < frame->GetWidth() - FontWidth
			&& y < frame->GetHeight() - FontHeight)
		{
			char* ptr = (char*)reinterpret_cast<const char*>(ifsh);
			uint8_t offset = 0;
			while (x < frame->GetWidth() - offset)
			{
				const int8_t character = pgm_read_byte(ptr++);
				if (character == Character::Break)
				{
					break;
				}
				else
				{
					Write(frame, color, x + offset, y, character);
					offset += FontWidth + 1;
				}
			}
		}
#else
		TextTopLeft(frame, color, x, y, reinterpret_cast<const char*>(ifsh));
#endif	
	}

	void TextTopRight(IFrameBuffer* frame, const RgbColor& color, const uint8_t x, const uint8_t y, const __FlashStringHelper* ifsh)
	{
#if defined(ARDUINO_ARCH_AVR)
		if (ifsh != NULL
			&& x < frame->GetWidth()
			&& y < frame->GetHeight() - FontHeight)
		{
			const uint8_t x1 = x - FontHeight;

			size_t size = 0;
			char* ptr = (char*)reinterpret_cast<const char*>(ifsh);
			while (true)
			{
				const int8_t character = pgm_read_byte(ptr + size);
				if (character == Character::Break)
				{
					break;
				}
				else
				{
					size++;
				}
			}
			ptr += size - 1;
			uint8_t offset = 0;
			while (size--
				&& x > offset)
			{
				const int8_t character = pgm_read_byte(ptr--);
				Write(frame, color, x1 - offset, y, character);
				offset += FontWidth + 1;
			}
		}
#else
		TextTopRight(frame, color, x, y, reinterpret_cast<const char*>(ifsh));
#endif		
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

	void Write(IFrameBuffer* frame, const RgbColor& color, const uint8_t x, const uint8_t y, const char character)
	{
		SetCharacter(SpriteSource, character);
		SpriteSource.SetColor(color);

		if (character == '\t'
			|| character == ' ')
		{
			return;
		}

		SpriteRenderer::Draw(frame, &SpriteSource, x, y);
	}
};
#endif