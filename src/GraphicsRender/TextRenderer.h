// TextRenderer.h

#ifndef _TEXT_RENDERER_h
#define _TEXT_RENDERER_h

#include "FontRenderer.h"
#include <WString.h>

/// <summary>
/// Draws text to screen using FontRenderer.
/// </summary>
class TextRenderer
{
public:
	static void TextTopLeft(IFrameBuffer* frame, const FontStyle& font, const uint8_t x1, const uint8_t y1, const __FlashStringHelper* ifsh)
	{
#if defined(ARDUINO_ARCH_AVR)
		if (ifsh != NULL
			&& x1 < frame->GetWidth() - font.Width
			&& y1 < frame->GetHeight() - font.Height)
		{
			char* ptr = (char*)reinterpret_cast<const char*>(ifsh);
			uint8_t offset = 0;
			while (x1 < frame->GetWidth() - offset - font.Width - font.Kerning)
			{
				const int8_t character = pgm_read_byte(ptr++);
				if (character == Character::Break)
				{
					break;
				}
				else
				{
					FontRenderer::Write(frame, font, x1 + offset, y1, character);
					offset += font.Width + font.Kerning;
				}
			}
		}
#else
		TextTopLeft(frame, font, x1, y1, reinterpret_cast<const char*>(ifsh));
#endif		
	}

	static void TextTopRight(IFrameBuffer* frame, const FontStyle& font, const uint8_t x2, const uint8_t y1, const __FlashStringHelper* ifsh)
	{
#if defined(ARDUINO_ARCH_AVR)
		if (ifsh != NULL
			&& x2 < frame->GetWidth()
			&& y1 < frame->GetHeight() - font.Height)
		{
			const uint8_t x1 = x2 - font.Width;

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
				&& x2 > offset)
			{
				const int8_t character = pgm_read_byte(ptr--);
				FontRenderer::Write(frame, font, x1 - offset, y1, character);
				offset += font.Width + font.Kerning;
			}
		}
#else
		TextTopRight(frame, font, x2, y1, reinterpret_cast<const char*>(ifsh));
#endif		
	}

	static void TextBottomRight(IFrameBuffer* frame, const FontStyle& font, const uint8_t x2, const uint8_t y1, const __FlashStringHelper* ifsh)
	{
		TextTopLeft(frame, font, x2, y1 - font.Height, ifsh);
	}

	static void TextBottomLeft(IFrameBuffer* frame, const FontStyle& font, const uint8_t x1, const uint8_t y2, const __FlashStringHelper* ifsh)
	{
		TextTopLeft(frame, font, x1, y2 - font.Height, ifsh);
	}

	static void TextBottomRight(IFrameBuffer* frame, const FontStyle& font, const uint8_t x2, const uint8_t y1, const char* str)
	{
		TextTopRight(frame, font, x2, y1 - font.Height, str);
	}

	static void TextBottomLeft(IFrameBuffer* frame, const FontStyle& font, const uint8_t x1, const uint8_t y1, const char* str)
	{
		TextTopLeft(frame, font, x1, y1 - font.Height, str);
	}

	static void TextTopLeft(IFrameBuffer* frame, const FontStyle& font, const uint8_t x1, const uint8_t y1, const char* str)
	{
		if (str != NULL
			&& x1 < frame->GetWidth()
			&& y1 < frame->GetHeight() - font.Height)
		{
			size_t size = strlen(str);
			uint8_t* ch = (uint8_t*)str;
			uint8_t offset = 0;
			while (size--
				&& x1 < frame->GetWidth() - offset - font.Width - font.Kerning)
			{
				FontRenderer::Write(frame, font, x1 + offset, y1, (const uint8_t)*ch++);
				offset += font.Width + font.Kerning;
			}
		}
	}

	static void TextTopRight(IFrameBuffer* frame, const FontStyle& font, const uint8_t x2, const uint8_t y1, const char* str)
	{
		if (str != NULL
			&& x2 < frame->GetWidth()
			&& y1 < frame->GetHeight() - font.Height)
		{
			const uint8_t x1 = x2 - font.Width;

			size_t size = strlen(str);
			uint8_t* ch = (uint8_t*)str + size - 1;
			uint8_t offset = 0;
			while (size--
				&& x1 < frame->GetWidth() - offset)
			{
#if defined(GRAPHICS_ENGINE_DEBUG)
				if (offset > x1)
				{
					break;
				}
#endif
				FontRenderer::Write(frame, font, x1 - offset, y1, (const char)*ch--);
				offset += font.Width + font.Kerning;
			}
		}
	}
};
#endif