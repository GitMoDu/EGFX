// DigitRenderer.h

#ifndef _DIGIT_RENDERER_h
#define _DIGIT_RENDERER_h

#include "../Model/IFrameBuffer.h"
#include "../Model/FontStyle.h"

/// <summary>
/// Draws digits using IFrameBuffer primitives.
/// </summary>
class DigitRenderer
{
public:
	static void Digit(IFrameBuffer* frameBuffer, const FontStyle& font, const pixel_t x1, const pixel_t y1, const uint8_t digit)
	{
		if ((x1 < (frameBuffer->GetFrameWidth() - font.Width))
			&& (y1 < (frameBuffer->GetFrameHeight() - font.Height)))
		{
			switch (digit)
			{
			case 0:
				Draw0(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
				break;
			case 1:
				Draw1(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
				break;
			case 2:
				Draw2(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
				break;
			case 3:
				Draw3(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
				break;
			case 4:
				Draw4(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
				break;
			case 5:
				Draw5(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
				break;
			case 6:
				Draw6(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
				break;
			case 7:
				Draw7(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
				break;
			case 8:
				Draw8(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
				break;
			case 9:
				Draw9(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
				break;
			default:
				frameBuffer->Rectangle(font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
				break;
			}
		}
	}

private:
	static void Draw0(IFrameBuffer* frameBuffer, const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
	{
		frameBuffer->Line(color, x1 + 1, y1, x2 - 1, y1);
		frameBuffer->Line(color, x1 + 1, y2 - 1, x2 - 1, y2 - 1);

		frameBuffer->Line(color, x1, y1 + 1, x1, y2 - 1);
		frameBuffer->Line(color, x2 - 1, y1 + 1, x2 - 1, y2 - 1);

		frameBuffer->Line(color, x1 + 1, y2 - 1, x2 - 2, y1 + 1);
	}

	static void Draw1(IFrameBuffer* frameBuffer, const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
	{
		const pixel_t middleX = (((coordinate_t)x2 + x1) / 2);

		if ((x2 - x1) > 2)
		{
			frameBuffer->Line(color, x1, y1, middleX, y1);
			frameBuffer->Line(color, middleX - 1, y1 + 1, middleX - 1, y2 - 1);
			frameBuffer->Line(color, x1, y2 - 1, x2 - 2, y2 - 1);
		}
		else
		{
			frameBuffer->Line(color, middleX, y1, middleX, y2 - 1);
			frameBuffer->Line(color, x1, y2 - 1, x2 - 1, y2 - 1);
		}
	}

	static void Draw2(IFrameBuffer* frameBuffer, const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
	{
		const pixel_t referenceY = (((coordinate_t)y2 + y1) / 2) - 1;

		frameBuffer->Line(color, x1, y1, x2 - 1, y1);
		frameBuffer->Line(color, x2 - 1, y1 + 1, x2 - 1, referenceY);

		frameBuffer->Line(color, x1, y2 - 1, x2 - 1, referenceY);

		frameBuffer->Line(color, x1, y2 - 1, x2, y2 - 1);
	}

	static void Draw3(IFrameBuffer* frameBuffer, const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
	{
		const pixel_t middleX = (((coordinate_t)x2 + x1) / 2);
		const pixel_t middleY = (((coordinate_t)y2 + y1) / 2);

		frameBuffer->Line(color, x2 - 1, y1 + 1, x2 - 1, y2 - 1);

		frameBuffer->Line(color, x1, y1, x2 - 1, y1);
		frameBuffer->Line(color, middleX, middleY - 1, x2 - 1, middleY - 1);
		frameBuffer->Line(color, x1, y2 - 1, x2 - 1, y2 - 1);
	}

	static void Draw4(IFrameBuffer* frameBuffer, const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
	{
		const pixel_t middleY = (((coordinate_t)y2 + y1) / 2) - 1;

		frameBuffer->Line(color, x2 - 1, y1, x2 - 1, y2);
		frameBuffer->Line(color, x1, middleY + 1, x2 - 1, middleY + 1);

		if (x2 - x1 > 7)
		{
			const pixel_t referenceX = x1 - 1 + ((x2 - x1) / 4);
			frameBuffer->Line(color, referenceX, y1, x1, middleY + 1);
		}
		else
		{
			frameBuffer->Line(color, x1 + 1, y1, x1, middleY + 1);
		}
	}

	static void Draw5(IFrameBuffer* frameBuffer, const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
	{
		const pixel_t middleY = ((coordinate_t)y2 + y1) / 2;

		frameBuffer->Line(color, x1, y1, x2, y1);
		frameBuffer->Line(color, x1, y1 + 1, x1, middleY - 1);
		frameBuffer->Line(color, x1 + 1, middleY - 1, x2 - 1, middleY - 1);
		frameBuffer->Line(color, x2 - 1, middleY, x2 - 1, y2 - 1);
		frameBuffer->Line(color, x1, y2 - 1, x2 - 1, y2 - 1);
	}

	static void Draw6(IFrameBuffer* frameBuffer, const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
	{
		const pixel_t middleY = ((coordinate_t)y2 + y1) / 2;

		frameBuffer->Line(color, x1 + 1, y1, x2, y1);
		frameBuffer->Line(color, x1, y1 + 1, x1, y2 - 1);
		frameBuffer->Line(color, x1 + 1, middleY - 1, x2 - 1, middleY - 1);
		frameBuffer->Line(color, x1 + 1, y2 - 1, x2 - 1, y2 - 1);
		frameBuffer->Line(color, x2 - 1, middleY, x2 - 1, y2 - 1);
	}

	static void Draw7(IFrameBuffer* frameBuffer, const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
	{
		frameBuffer->Line(color, x1, y1, x2, y1);
		frameBuffer->Line(color, x1, y2, x2 - 1, y1 + 1);
	}

	static void Draw8(IFrameBuffer* frameBuffer, const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
	{
		const pixel_t middleY = (((coordinate_t)y2 + y1) / 2) - 1;

		frameBuffer->Line(color, x1 + 1, y1, x2 - 1, y1);
		frameBuffer->Line(color, x1, y1 + 1, x1, middleY);
		frameBuffer->Line(color, x2 - 1, y1 + 1, x2 - 1, middleY);
		frameBuffer->Line(color, x1 + 1, middleY, x2 - 1, middleY);
		frameBuffer->Line(color, x1, middleY + 1, x1, y2 - 1);
		frameBuffer->Line(color, x2 - 1, middleY + 1, x2 - 1, y2 - 1);
		frameBuffer->Line(color, x1 + 1, y2 - 1, x2 - 1, y2 - 1);
	}

	static void Draw9(IFrameBuffer* frameBuffer, const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
	{
		const pixel_t middleY = (((coordinate_t)y2 + y1) / 2) - 1;

		frameBuffer->Line(color, x1 + 1, y1, x2 - 1, y1);
		frameBuffer->Line(color, x1, y1 + 1, x1, middleY);
		frameBuffer->Line(color, x1 + 1, middleY, x2 - 1, middleY);
		frameBuffer->Line(color, x2 - 1, y1 + 1, x2 - 1, y2 - 1);
		frameBuffer->Line(color, x1 + 1, y2 - 1, x2 - 1, y2 - 1);
	}
};
#endif