// TextRenderer.h

#ifndef _TEXT_RENDERER_h
#define _TEXT_RENDERER_h

#include "FontRenderer.h"
#include <String.h>

/// <summary>
/// Draws text to screen using FontRenderer.
/// </summary>
class TextRenderer
{
public:
	static void TextTopLeft(IFrameBuffer* frame, const FontStyle& font, const uint8_t x1, const uint8_t y1, const __FlashStringHelper* ifsh)
	{
		TextTopLeft(frame, font, x1, y1, reinterpret_cast<const char*>(ifsh));
	}

	static void TextTopRight(IFrameBuffer* frame, const FontStyle& font, const uint8_t x2, const uint8_t y1, const __FlashStringHelper* ifsh)
	{
		TextTopRight(frame, font, x2, y1, reinterpret_cast<const char*>(ifsh));
	}

	static void TextBottomRight(IFrameBuffer* frame, const FontStyle& font, const uint8_t x2, const uint8_t y1, const __FlashStringHelper* ifsh)
	{
		TextTopLeft(frame, font, x2, y1 - font.Height, ifsh);
	}

	static void TextBottomLeft(IFrameBuffer* frame, const FontStyle& font, const uint8_t x1, const uint8_t y2, const __FlashStringHelper* ifsh)
	{
		TextTopLeft(frame, font, x1, y2 - font.Height, ifsh);
	}

	static void TextTopLeft(IFrameBuffer* frame, const FontStyle& font, const uint8_t x1, const uint8_t y1, const char* str)
	{
		if (str != NULL)
		{
#if defined(GRAPHICS_ENGINE_DEBUG)
			const uint8_t x2 = x1 + font.Width;
			const uint8_t y2 = y1 + font.Height;
			const uint8_t width = frame->GetWidth();
			const uint8_t height = frame->GetHeight();

			if ((x2 >= width)
				|| (y2 >= height))
			{
				return;
			}
#endif
			size_t size = strlen(str);
			uint8_t* ch = (uint8_t*)str;
			uint8_t offset = 0;
			while (size--)
			{
#if defined(GRAPHICS_ENGINE_DEBUG)
				if ((x2 + offset) >= width)
				{
					break;
				}
#endif
				FontRenderer::Write(frame, font, x1 + offset, y1, (const uint8_t)*ch++);
				offset += font.Width + font.Kerning;
			}
		}
	}

	static void TextTopRight(IFrameBuffer* frame, const FontStyle& font, const uint8_t x2, const uint8_t y1, const char* str)
	{
		if (str != NULL)
		{
			const uint8_t x1 = x2 - font.Width;

#if defined(GRAPHICS_ENGINE_DEBUG)
			const uint8_t y2 = y1 + font.Height;
			const uint8_t width = frame->GetWidth();
			const uint8_t height = frame->GetHeight();

			if ((x2 >= width)
				|| (y2 >= height))
			{
				return;
			}
#endif
			size_t size = strlen(str);
			uint8_t* ch = (uint8_t*)str + size - 1;
			uint8_t offset = 0;
			while (size--)
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

	static void TextBottomRight(IFrameBuffer* frame, const FontStyle& font, const uint8_t x2, const uint8_t y1, const char* str)
	{
		TextTopRight(frame, font, x2, y1 - font.Height, str);
	}

	static void TextBottomLeft(IFrameBuffer* frame, const FontStyle& font, const uint8_t x1, const uint8_t y1, const char* str)
	{
		TextTopLeft(frame, font, x1, y1 - font.Height, str);
	}
};
#endif