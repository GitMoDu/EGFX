// FontRenderer.h

#ifndef _FONT_RENDERER_h
#define _FONT_RENDERER_h

#include "../../IFrame/IFramePrimitives.h"
#include "../FontStyle.h"

/// <summary>
/// Draws ASCII code fonts using IFramePrimitives primitives.
/// </summary>
class FontRenderer
{
protected:
	static void WriteCharacter(IFramePrimitives* frameBuffer, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const RgbColor& color, const uint8_t stroke, const uint8_t character)
	{
		if (x1 < frameBuffer->GetWidth()
			&& y1 < frameBuffer->GetHeight())
		{
			switch (character)
			{
			case '0':
				Draw0(frameBuffer, color, x1, y1, x2, y2, stroke);
				break;
			case '1':
				Draw1(frameBuffer, color, x1, y1, x2, y2, stroke);
				break;
			case '2':
				Draw2(frameBuffer, color, x1, y1, x2, y2, stroke);
				break;
			case '3':
				Draw3(frameBuffer, color, x1, y1, x2, y2, stroke);
				break;
			case '4':
				Draw4(frameBuffer, color, x1, y1, x2, y2, stroke);
				break;
			case '5':
				Draw5(frameBuffer, color, x1, y1, x2, y2, stroke);
				break;
			case '6':
				Draw6(frameBuffer, color, x1, y1, x2, y2, stroke);
				break;
			case '7':
				Draw7(frameBuffer, color, x1, y1, x2, y2, stroke);
				break;
			case '8':
				Draw8(frameBuffer, color, x1, y1, x2, y2, stroke);
				break;
			case '9':
				Draw9(frameBuffer, color, x1, y1, x2, y2, stroke);
				break;
				//case 'a':
				//	Drawa(frameBuffer, color, x1, y1, x2, y2, stroke);
				//	break;
				//	//case ...
			case 'z':
				Drawz(frameBuffer, color, x1, y1, x2, y2, stroke);
				break;
			case 'A':
				DrawA(frameBuffer, color, x1, y1, x2, y2, stroke);
				break;
				//case ...
			case 'Z':
				DrawZ(frameBuffer, color, x1, y1, x2, y2, stroke);
				break;
			default:
				frameBuffer->Rectangle(x1, y1, x2, y2, color, stroke);
				break;
			}
		}
	}

private:
	static void Draw0(IFramePrimitives* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const uint8_t stroke)
	{
		for (uint_fast8_t i = 0; i < stroke; i++)
		{
			frameBuffer->Line(x1 + stroke, y1 + i, x2 - stroke - 1, y1 + i, color);
			frameBuffer->Line(x1 + stroke, y2 - 1 - i, x2 - stroke - 1, y2 - 1 - i, color);
			frameBuffer->Line(x1, y1 + stroke + i, x1 + stroke, y1 + 1 + i, color);
			frameBuffer->Line(x2 - stroke - 0, y1 + i, x2, y1 + stroke + i, color);
			frameBuffer->Line(x1, y2 - stroke - 1 - i, x1 + stroke, y2 - 1 - i, color);
			frameBuffer->Line(x2 - stroke - 1, y2 - 1 - i, x2, y2 - stroke - 1 - i, color);
			frameBuffer->Line(x1 + i, y1 + stroke, x1 + i, y2 - 1 - stroke, color);
			frameBuffer->Line(x2 - i, y1 + stroke, x2 - i, y2 - 1 - stroke, color);
		}

		if (stroke > 1)
		{
			frameBuffer->CircleFill(((uint16_t)x1 + x2) / 2, ((uint16_t)y1 + y2) / 2, stroke - 1, color);
		}
		else
		{
			frameBuffer->Pixel(((uint16_t)x1 + x2) / 2, ((uint16_t)y1 + y2) / 2, color);
		}
	}

	static void Draw1(IFramePrimitives* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const uint8_t stroke)
	{
		const uint8_t middleX = stroke + (((uint16_t)x2 + x1) / 2);
		const uint8_t doubleStroke = stroke * 2;
		for (uint_fast8_t i = 0; i < stroke; i++)
		{
			frameBuffer->Line(middleX - doubleStroke, y1 + stroke + i, middleX, y1 + i, color);
			frameBuffer->Line(middleX + i, y1, middleX + i, y2 - stroke, color);
			frameBuffer->Line(middleX - stroke, y2 - 1 - i, x2 - stroke + 1, y2 - 1 - i, color);
		}
	}

	static void Draw2(IFramePrimitives* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const uint8_t stroke)
	{
		const uint8_t doubleStroke = stroke * 2;
		const uint8_t tripleStroke = stroke * 3;
		for (uint_fast8_t i = 0; i < stroke; i++)
		{
			frameBuffer->Line(x1 + stroke, y1 + i, x2 - doubleStroke + 1, y1 + i, color);
			frameBuffer->Line(x2 - i, y1 + doubleStroke - 1, x2 - 1 - i, y1 + tripleStroke, color);
			frameBuffer->Line(x2 - doubleStroke + 1, y1 + i, x2 - 1, y1 + stroke + 1 + i, color);
			frameBuffer->Line(x1 + 1, y2 - doubleStroke + 1 - i, x2 - 1, y1 + doubleStroke - i, color);
			frameBuffer->Line(x1, y2 - 1 - i, x2 - 1, y2 - 1 - i, color);
		}
	}

	static void Draw3(IFramePrimitives* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const uint8_t stroke)
	{
		const uint8_t doubleStroke = stroke * 2;
		const uint8_t tripleStroke = stroke * 3;
		const uint8_t middleY = ((uint16_t)y2 + y1) / 2;
		for (uint_fast8_t i = 0; i < stroke; i++)
		{
			frameBuffer->Line(x1 + stroke, y1 + i, x2 - doubleStroke + 1, y1 + i, color);
			frameBuffer->Line(x2 - doubleStroke + 1, y1 + i, x2 - 1, y1 + stroke + 1 + i, color);
			frameBuffer->Line(x2 - tripleStroke, middleY + 1 - i, x2 - 1, middleY - doubleStroke - i, color);
			frameBuffer->Line(x2 - doubleStroke - 1, middleY + i, x2 - 1, middleY + doubleStroke - 1 + i, color);
			frameBuffer->Line(x2 - stroke - i, y1 + stroke, x2 - stroke - i, middleY - stroke, color);
			frameBuffer->Line(x2 - doubleStroke + 1, middleY - i, x1 + stroke, middleY - i, color);
			frameBuffer->Line(x2 - doubleStroke, y2 - 1 - i, x2 - 1, y2 - doubleStroke - 1 - i, color);
			frameBuffer->Line(x1 + stroke, y2 - 1 - i, x2 - stroke - 1, y2 - 1 - i, color);
			frameBuffer->Line(x1, y2 - stroke - i, x1 + doubleStroke - 1, y2 - 1 - i, color);
		}
	}

	static void Draw4(IFramePrimitives* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const uint8_t stroke)
	{
		const uint8_t middleY = ((uint16_t)y2 + y1) / 2;
		const uint8_t doubleStroke = stroke * 2;
		for (uint_fast8_t i = 0; i < stroke; i++)
		{
			frameBuffer->Line(x2 - stroke - i, y1, x2 - stroke - i, y2 - 1, color);

			frameBuffer->Line(x1, middleY + stroke + i, x2 - 1, middleY + stroke + i, color);

			frameBuffer->Line(x1 + i, middleY + stroke, x1 + doubleStroke - 1 + i, y1, color);
		}
	}

	static void Draw5(IFramePrimitives* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const uint8_t stroke)
	{
		const uint8_t middleY = ((uint16_t)y2 + y1) / 2;
		const uint8_t doubleStroke = stroke * 2;
		for (uint_fast8_t i = 0; i < stroke; i++)
		{
			frameBuffer->Line(x1, y1 + i, x2 - 1, y1 + i, color);
			frameBuffer->Line(x1 + i, y1, x1 + i, middleY, color);
			frameBuffer->Line(x1 + stroke, middleY - stroke + i, x2 - doubleStroke, middleY - stroke + i, color);
			frameBuffer->Line(x2 - doubleStroke - 1, middleY - stroke + i, x2, middleY + 1 + i, color);
			frameBuffer->Line(x2 - 1 - i, middleY, x2 - 1 - i, y2 - doubleStroke + 1, color);
			frameBuffer->Line(x2 - doubleStroke, y2 - 1 - i, x2 - 1, y2 - doubleStroke - 1 - i, color);
			frameBuffer->Line(x1 + stroke, y2 - 1 - i, x2 - stroke - 1, y2 - 1 - i, color);
			frameBuffer->Line(x1, y2 - stroke - i, x1 + doubleStroke - 1, y2 - 1 - i, color);
		}
	}

	static void Draw6(IFramePrimitives* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const uint8_t stroke)
	{
		const uint8_t middleY = ((uint16_t)y2 + y1) / 2;
		const uint8_t doubleStroke = stroke * 2;
		for (uint_fast8_t i = 0; i < stroke; i++)
		{
			frameBuffer->Line(x2 - doubleStroke, y1 + i, x2 - 1, y1 + i, color);
			frameBuffer->Line(x1 + stroke, y1 + 1 + i, x2 - doubleStroke, y1 + i, color);
			frameBuffer->Line(x1 + i, middleY, x1 + stroke - 1 + i, y1 + 1, color);

			frameBuffer->Line(x1 + stroke + 1, middleY - stroke + i, x2 - doubleStroke, middleY - stroke + i, color);
			frameBuffer->Line(x2 - doubleStroke - 1, middleY - stroke + i, x2, middleY + i, color);


			frameBuffer->Line(x1 + i, middleY - doubleStroke - 1, x1 + i, y2 - doubleStroke + 1, color);
			frameBuffer->Line(x2 - 1 - i, middleY, x2 - 1 - i, y2 - doubleStroke + 1, color);
			frameBuffer->Line(x2 - doubleStroke, y2 - 1 - i, x2 - doubleStroke, y2 - doubleStroke - 1 - i, color);
			frameBuffer->Line(x1 + stroke, y2 - 1 - i, x2 - stroke - 1, y2 - 1 - i, color);
			frameBuffer->Line(x1, y2 - stroke - i, x1 + doubleStroke - 1, y2 - 1 - i, color);
		}
	}

	static void Draw7(IFramePrimitives* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const uint8_t stroke)
	{
		const uint8_t doubleStroke = stroke * 2;
		const uint8_t middleX = ((uint16_t)x2 + x1) / 2;
		for (uint_fast8_t i = 0; i < stroke; i++)
		{
			frameBuffer->Line(x1, y1 + i, x2 - 1, y1 + i, color);
			frameBuffer->Line(x1 + i, y1 + stroke, x1 + i, y1 + doubleStroke - 1, color);
			frameBuffer->Line(x2 - i, y1, middleX - stroke - i, y2 - 1, color);
		}
	}
	static void Draw8(IFramePrimitives* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const uint8_t stroke)
	{
		const uint8_t middleY = ((uint16_t)y2 + y1) / 2;
		const uint8_t doubleStroke = stroke * 2;
		for (uint_fast8_t i = 0; i < stroke; i++)
		{
			frameBuffer->Line(x1 + i, y1 + doubleStroke - 1, x1 + i, middleY, color);
			frameBuffer->Line(x1 + stroke, y1 + i, x2 - stroke - 1, y1 + i, color);
			frameBuffer->Line(x2 - stroke + i, y1 + doubleStroke - 1, x2 - stroke + i, middleY, color);
			frameBuffer->Line(x1 + stroke, middleY - stroke + 1 + i, x2 - stroke, middleY - stroke + 1 + i, color);

			frameBuffer->Line(x1 + i, middleY, x1 + i, y2 - doubleStroke + 1, color);
			frameBuffer->Line(x2 - 1 - i, middleY, x2 - 1 - i, y2 - doubleStroke + 1, color);
			frameBuffer->Line(x2 - doubleStroke, y2 - 1 - i, x2 - doubleStroke, y2 - doubleStroke - 1 - i, color);
			frameBuffer->Line(x1 + stroke, y2 - 1 - i, x2 - stroke - 1, y2 - 1 - i, color);
			frameBuffer->Line(x1, y2 - stroke - i, x1 + doubleStroke - 1, y2 - 1 - i, color);
		}
	}
	static void Draw9(IFramePrimitives* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const uint8_t stroke)
	{
		const uint8_t middleY = ((uint16_t)y2 + y1) / 2;
		const uint8_t doubleStroke = stroke * 2;
		for (uint_fast8_t i = 0; i < stroke; i++)
		{
			frameBuffer->Line(x1 + i, y1 + doubleStroke - 1, x1 + i, middleY - stroke, color);
			frameBuffer->Line(x1 + stroke, y1 + i, x2 - stroke - 1, y1 + i, color);
			frameBuffer->Line(x2 - stroke + i, y1 + doubleStroke - 1, x2 - stroke + i, middleY, color);
			frameBuffer->Line(x1 + stroke, middleY - stroke + i, x2 - stroke, middleY - stroke + i, color);

			frameBuffer->Line(x2 - doubleStroke - i, y2 - 1, x2 - i - 1, middleY, color);
		}
	}

	static void Drawa(IFramePrimitives* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const uint8_t stroke)
	{
		DrawA(frameBuffer, color, x1, y1, x2, y2, stroke);
	}

	static void Drawz(IFramePrimitives* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const uint8_t stroke)
	{
		const uint8_t middleY = (((uint16_t)y2 + y1) / 2);
		const uint8_t doubleStroke = stroke * 2;
		for (uint_fast8_t i = 0; i < stroke; i++)
		{
			frameBuffer->Line(x1, middleY - doubleStroke + i, x2 - 1, middleY - doubleStroke + i, color);
			frameBuffer->Line(x1 + i, y2 - stroke, x2 - stroke + i, middleY - doubleStroke, color);
			frameBuffer->Line(x1, y2 - 1 - i, x2 - 1, y2 - 1 - i, color);
		}
	}

	static void DrawA(IFramePrimitives* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const uint8_t stroke)
	{
		const uint8_t middleX = ((uint16_t)x2 + x1) / 2;
		const uint8_t middleY = ((uint16_t)y2 + y1) / 2;
		const uint8_t halfStroke = stroke / 2;
		for (uint_fast8_t i = 0; i < stroke; i++)
		{
			frameBuffer->Line(x1 + i, y2 - 1, middleX - halfStroke + i, y1, color);
			frameBuffer->Line(middleX - stroke - i + 1, middleY + stroke + i, middleX + stroke + i, middleY + stroke + i, color);
			frameBuffer->Line(middleX + stroke + i, y1, x2 - stroke + i, y2 - 1, color);
		}
	}
	static void DrawZ(IFramePrimitives* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const uint8_t stroke)
	{
		for (uint_fast8_t i = 0; i < stroke; i++)
		{
			frameBuffer->Line(x1, y1 + i, x2 - 1, y1 + i, color);
			frameBuffer->Line(x1, y2 - 1 + i, x2 - stroke, y1 + i, color);
			frameBuffer->Line(x1, y2 - 1 - i, x2 - 1, y2 - 1 - i, color);
		}
	}
};
#endif