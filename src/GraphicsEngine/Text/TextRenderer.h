// TextRenderer.h

#ifndef _TEXT_RENDERER_h
#define _TEXT_RENDERER_h

#include "NumberRenderer.h"
#include <String.h>

/// <summary>
/// Draws text to screen using FontRenderer.
/// </summary>
class TextRenderer : public NumberRenderer
{
public:
	static void TextTopLeft(IFramePrimitives* frame, const uint8_t x1, const uint8_t y1, const FontStyle& font, const __FlashStringHelper* ifsh)
	{
		TextTopLeft(frame, x1, y1, font, reinterpret_cast<const char*>(ifsh));
	}

	static void TextTopRight(IFramePrimitives* frame, const uint8_t x2, const uint8_t y1, const FontStyle& font, const __FlashStringHelper* ifsh)
	{
		TextTopRight(frame, x2, y1, font, reinterpret_cast<const char*>(ifsh));
	}

	static void TextBottomRight(IFramePrimitives* frame, const uint8_t x2, const uint8_t y1, const FontStyle& font, const __FlashStringHelper* ifsh)
	{
		TextTopLeft(frame, x2, y1 - font.Height, font, ifsh);
	}

	static void TextBottomLeft(IFramePrimitives* frame, const uint8_t x1, const uint8_t y2, const FontStyle& font, const __FlashStringHelper* ifsh)
	{
		TextTopLeft(frame, x1, y2 - font.Height, font, ifsh);
	}

	static void TextTopLeft(IFramePrimitives* frame, const uint8_t x1, const uint8_t y1, const FontStyle& font, const char* str)
	{
		if (str != NULL)
		{
			const uint8_t x2 = x1 + font.Width;
			const uint8_t y2 = y1 + font.Height;

			uint8_t size = strlen(str);
			uint8_t* ch = (uint8_t*)str;
			uint8_t offset = 0;
			while (size--)
			{
				WriteCharacter(frame, x1 + offset, y1, x2 + offset, y2, font.Color, font.Stroke, (const uint8_t)*ch++);
				offset += font.Width + font.Kerning;
			}
		}
	}

	static void TextTopRight(IFramePrimitives* frame, const uint8_t x2, const uint8_t y1, const FontStyle& font, const char* str)
	{
		if (str != NULL)
		{
			const uint8_t x1 = x2 - font.Width;
			const uint8_t y2 = y1 + font.Height;

			uint8_t size = strlen(str);
			uint8_t* ch = (uint8_t*)str + size - 1;
			uint8_t offset = 0;
			while (size--)
			{
				WriteCharacter(frame, x1 - offset, y1, x2 - offset, y2, font.Color, font.Stroke, (const char)*ch--);
				offset += font.Width + font.Kerning;
			}
		}
	}

	static void TextBottomRight(IFramePrimitives* frame, const uint8_t x2, const uint8_t y1, const FontStyle& font, const char* str)
	{
		TextTopRight(frame, x2, y1 - font.Height, font, str);
	}	

	static void TextBottomLeft(IFramePrimitives* frame, const uint8_t x1, const uint8_t y1, const FontStyle& font, const char* str)
	{
		TextTopLeft(frame, x1, y1 - font.Height, font, str);
	}
};
#endif