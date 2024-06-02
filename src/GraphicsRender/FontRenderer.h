// FontRenderer.h

#ifndef _FONT_RENDERER_h
#define _FONT_RENDERER_h

#include "../Model/IFrameBuffer.h"
#include "../Model/FontStyle.h"
#include "../Model/Character.h"
#include "DigitRenderer.h"

/// <summary>
/// Draws ASCII code fonts using IFrameBuffer primitives.
/// </summary>
class FontRenderer
{
public:
	static void Write(IFrameBuffer* frameBuffer, const FontStyle& font, const uint8_t x1, const uint8_t y1, const int8_t character)
	{
		if (x1 < frameBuffer->GetWidth()
			&& y1 < frameBuffer->GetHeight())
		{
			if (character >= Character::NumbersStart
				&& character < Character::NumbersEnd)
			{
				DigitRenderer::Digit(frameBuffer, font, x1, y1, (uint8_t)(character - Character::NumbersStart));
			}
			else
			{
				switch (character)
				{
				case (int8_t)'-':
					DrawDash(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'A':
					DrawA(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'T':
					DrawT(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'X':
					DrawX(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'Z':
					DrawZ(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'a':
					Drawa(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'t':
					Drawt(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'x':
					Drawx(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'z':
					Drawz(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				default:
					frameBuffer->Rectangle(font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				}
			}
		}
	}

private:
	static void DrawDash(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleY = (((uint16_t)y2 + y1) / 2);

		frameBuffer->Line(color, x1, middleY, x2 - 2, middleY);
	}

	static void Drawa(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		DrawA(frameBuffer, color, x1, y1, x2, y2);
	}

	static void Drawt(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t height = y2 - y1;
		const uint8_t crossY = y1 + (height / 4);
		const uint8_t middleX = ((uint16_t)x2 + x1) / 2;

		frameBuffer->Line(color, x1 + 1, crossY, x2 - 1, crossY);
		frameBuffer->Line(color, middleX, crossY - 1, middleX, y2 - 1);
	}

	static void Drawx(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t height = y2 - y1;
		const uint8_t crossY = y1 + (height / 4);
		frameBuffer->Line(color, x1, crossY, x2 - 1, y2);
		frameBuffer->Line(color, x1, y2, x2 - 1, crossY);
	}

	static void Drawz(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t height = y2 - y1;
		const uint8_t crossY = y1 + (height / 4);

		frameBuffer->Line(color, x1 + 1, crossY, x2 - 1, crossY);
		frameBuffer->Line(color, x1, y2 - 2, x2 - 1, crossY);
		frameBuffer->Line(color, x1, y2 - 1, x2 - 1, y2 - 1);
	}

	static void DrawA(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t width = x2 - x1;
		const uint8_t middleX = ((uint16_t)x2 + x1) / 2;
		const uint8_t middleY = ((uint16_t)y2 + y1) / 2;

		frameBuffer->Line(color, x1, y2, middleX, y1 + 1);
		frameBuffer->Line(color, middleX - (width / 7), middleY, x2 - 1 - (width / 5), middleY);
		frameBuffer->Line(color, middleX + 1, y1, x2 - 1, y2);
	}

	static void DrawT(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleX = ((uint16_t)x2 + x1) / 2;
		frameBuffer->Line(color, x1, y1, x2 - 1, y1);
		frameBuffer->Line(color, middleX, y1, middleX, y2 - 1);
	}

	static void DrawX(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		frameBuffer->Line(color, x1, y1, x2 - 1, y2);
		frameBuffer->Line(color, x1, y2, x2 - 1, y1);
	}

	static void DrawZ(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		frameBuffer->Line(color, x1, y1, x2 - 1, y1);
		frameBuffer->Line(color, x1, y2 - 1, x2 - 1, y1);
		frameBuffer->Line(color, x1, y2 - 1, x2 - 1, y2 - 1);
	}
};
#endif