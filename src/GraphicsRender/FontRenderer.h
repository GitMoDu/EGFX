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
		if (x1 < frameBuffer->GetFrameWidth()
			&& y1 < frameBuffer->GetFrameHeight())
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
					DrawMinus(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'a':
				case (int8_t)'A':
					DrawA(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'b':
				case (int8_t)'B':
					DrawB(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'c':
				case (int8_t)'C':
					DrawC(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'d':
				case (int8_t)'D':
					DrawD(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'e':
				case (int8_t)'E':
					DrawE(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'f':
				case (int8_t)'F':
					DrawF(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'g':
				case (int8_t)'G':
					DrawG(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'h':
				case (int8_t)'H':
					DrawH(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'i':
				case (int8_t)'I':
					DrawI(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'j':
				case (int8_t)'J':
					DrawJ(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'k':
				case (int8_t)'K':
					DrawK(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'l':
				case (int8_t)'L':
					DrawL(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'m':
				case (int8_t)'M':
					DrawM(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'n':
				case (int8_t)'N':
					DrawN(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'o':
				case (int8_t)'O':
					DrawO(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'p':
				case (int8_t)'P':
					DrawP(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'q':
				case (int8_t)'Q':
					DrawQ(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'r':
				case (int8_t)'R':
					DrawR(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'s':
				case (int8_t)'S':
					DrawS(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'t':
				case (int8_t)'T':
					DrawT(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'u':
				case (int8_t)'U':
					DrawU(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'v':
				case (int8_t)'V':
					DrawV(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'w':
				case (int8_t)'W':
					DrawW(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'x':
				case (int8_t)'X':
					DrawX(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'y':
				case (int8_t)'Y':
					DrawY(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				case (int8_t)'z':
				case (int8_t)'Z':
					DrawZ(frameBuffer, font.Color, x1, y1, x1 + font.Width, y1 + font.Height);
					break;
				default:
					break;
				}
			}
		}
	}

private:
	static void DrawMinus(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleY = (((uint16_t)y2 + y1) / 2);

		frameBuffer->Line(color, x1, middleY, x2 - 2, middleY);
	}

	static void DrawA(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t margin = ((x2 - x1) / 4);
		const uint8_t middleX = (((uint16_t)x2 + x1) / 2);
		const uint8_t middleY = (((uint16_t)y2 + y1) / 2) + 1;

		frameBuffer->Line(color, x1, y2, middleX, y1);
		frameBuffer->Line(color, x1 + margin, middleY, x2 - margin, middleY);
		frameBuffer->Line(color, middleX, y1, x2 - 1, y2);
	}

	static void DrawB(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleY = (((uint16_t)y2 + y1) / 2);

		frameBuffer->Line(color, x2 - 1, y1 + 1, x2 - 1, middleY);
		frameBuffer->Line(color, x2 - 1, middleY + 1, x2 - 1, y2 - 1);

		frameBuffer->Line(color, x1, y1, x1, y2 - 1);
		frameBuffer->Line(color, x1, y1, x2 - 1, y1);
		frameBuffer->Line(color, x1, middleY, x2 - 1, middleY);
		frameBuffer->Line(color, x1, y2 - 1, x2 - 1, y2 - 1);
	}

	static void DrawC(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		frameBuffer->Line(color, x1 + 1, y1, x2, y1);
		frameBuffer->Line(color, x1, y1 + 1, x1, y2 - 1);
		frameBuffer->Line(color, x1 + 1, y2 - 1, x2, y2 - 1);
	}

	static void DrawD(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		frameBuffer->Line(color, x1 + 1, y1, x2 - 1, y1);
		frameBuffer->Line(color, x1, y1, x1, y2);
		frameBuffer->Line(color, x2 - 1, y1 + 1, x2 - 1, y2 - 1);
		frameBuffer->Line(color, x1 + 1, y2 - 1, x2 - 1, y2 - 1);
	}

	static void DrawE(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleY = (((uint16_t)y2 + y1) / 2);

		frameBuffer->Line(color, x1, y1, x1, y2 - 1);
		frameBuffer->Line(color, x1, y1, x2, y1);
		frameBuffer->Line(color, x1, middleY, x2 - 1, middleY);
		frameBuffer->Line(color, x1, y2 - 1, x2, y2 - 1);
	}

	static void DrawF(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleY = (((uint16_t)y2 + y1) / 2);

		frameBuffer->Line(color, x1, y1, x1, y2);
		frameBuffer->Line(color, x1, y1, x2, y1);
		frameBuffer->Line(color, x1, middleY, x2 - 1, middleY);
	}

	static void DrawG(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleY = (((uint16_t)y2 + y1) / 2);

		frameBuffer->Line(color, x1 + 1, y1, x2, y1);
		frameBuffer->Line(color, x1, y1 + 1, x1, y2 - 1);
		frameBuffer->Line(color, x1 + 1, y2 - 1, x2 - 1, y2 - 1);

		frameBuffer->Line(color, x2 - 1, middleY, x2 - 1, y2 - 1);
		frameBuffer->Line(color, x1 + 2, middleY, x2 - 1, middleY);
	}

	static void DrawH(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleY = (((uint16_t)y2 + y1) / 2);

		frameBuffer->Line(color, x1, y1, x1, y2);
		frameBuffer->Line(color, x2 - 1, y1, x2 - 1, y2);
		frameBuffer->Line(color, x1 + 1, middleY, x2 - 1, middleY);
	}

	static void DrawI(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleX = ((uint16_t)x2 + x1) / 2;

		frameBuffer->Line(color, x1 + 1, y1, x2 - 1, y1);
		frameBuffer->Line(color, middleX, y1, middleX, y2 - 1);
		frameBuffer->Line(color, x1 + 1, y2 - 1, x2 - 1, y2 - 1);
	}

	static void DrawJ(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleY = (((uint16_t)y2 + y1) / 2);

		frameBuffer->Line(color, x2 - 1, y1, x2 - 1, y2 - 1);
		frameBuffer->Line(color, x1, middleY, x1, y2 - 1);
		frameBuffer->Line(color, x1 + 1, y2 - 1, x2 - 1, y2 - 1);
	}

	static void DrawK(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleY = (((uint16_t)y2 + y1) / 2);

		frameBuffer->Line(color, x1, y1, x1, y2);
		frameBuffer->Line(color, x1, middleY, x2 - 1, y1);
		frameBuffer->Line(color, x1, middleY - 1, x2, y2);
	}

	static void DrawL(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		frameBuffer->Line(color, x1, y1, x1, y2 - 1);
		frameBuffer->Line(color, x1, y2 - 1, x2, y2 - 1);
	}

	static void DrawM(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleX = ((uint16_t)x2 + x1) / 2;
		const uint8_t middleY = (((uint16_t)y2 + y1) / 2);

		frameBuffer->Line(color, x1, y1, x1, y2);
		frameBuffer->Line(color, x1 + 1, y1 + 1, middleX, middleY);
		frameBuffer->Line(color, middleX, middleY, x2 - 1, y1);
		frameBuffer->Line(color, x2 - 1, y1, x2 - 1, y2);
	}

	static void DrawN(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		frameBuffer->Line(color, x1, y1, x1, y2);
		frameBuffer->Line(color, x1 + 1, y1 + 1, x2 - 1, y2);
		frameBuffer->Line(color, x2 - 1, y1, x2 - 1, y2);
	}

	static void DrawO(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		frameBuffer->Line(color, x1 + 1, y1, x2 - 1, y1);
		frameBuffer->Line(color, x1, y1 + 1, x1, y2 - 1);
		frameBuffer->Line(color, x2 - 1, y1 + 1, x2 - 1, y2 - 1);
		frameBuffer->Line(color, x1 + 1, y2 - 1, x2 - 1, y2 - 1);
	}

	static void DrawP(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleY = (((uint16_t)y2 + y1) / 2);

		frameBuffer->Line(color, x1, y1, x1, y2);
		frameBuffer->Line(color, x1 + 1, y1, x2 - 1, y1);
		frameBuffer->Line(color, x2 - 1, y1 + 1, x2 - 1, middleY + 1);
		frameBuffer->Line(color, x1 + 1, middleY, x2 - 1, middleY);
	}

	static void DrawQ(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		frameBuffer->Line(color, x1 + 1, y1, x2 - 1, y1);
		frameBuffer->Line(color, x1, y1 + 1, x1, y2 - 1);
		frameBuffer->Line(color, x2 - 1, y1 + 1, x2 - 1, y2 - 1);
		frameBuffer->Line(color, x1 + 1, y2 - 1, x2 - 1, y2 - 1);
		frameBuffer->Line(color, x2 - 2, y2 - 2, x2, y2);
	}

	static void DrawR(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleY = (((uint16_t)y2 + y1) / 2);

		frameBuffer->Line(color, x1, y1, x1, y2);
		frameBuffer->Line(color, x1 + 1, y1, x2 - 1, y1);
		frameBuffer->Line(color, x2 - 1, y1 + 1, x2 - 1, middleY);
		frameBuffer->Line(color, x1 + 1, middleY, x2 - 1, middleY);
		frameBuffer->Line(color, x1 + 1, middleY, x2 - 1, y2);
	}

	static void DrawS(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleY = (((uint16_t)y2 + y1) / 2);

		frameBuffer->Line(color, x1 + 1, y1, x2, y1);
		frameBuffer->Line(color, x1, y1 + 1, x1, middleY);
		frameBuffer->Line(color, x1 + 1, middleY, x2 - 1, middleY);
		frameBuffer->Line(color, x2 - 1, middleY + 1, x2 - 1, y2 - 1);
		frameBuffer->Line(color, x1, y2 - 1, x2 - 1, y2 - 1);
	}

	static void DrawT(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleX = ((uint16_t)x2 + x1) / 2;

		frameBuffer->Line(color, x1, y1, x2, y1);
		frameBuffer->Line(color, middleX, y1, middleX, y2);
	}

	static void DrawU(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		frameBuffer->Line(color, x1, y1, x1, y2 - 1);
		frameBuffer->Line(color, x1 + 1, y2 - 1, x2 - 1, y2 - 1);
		frameBuffer->Line(color, x2 - 1, y1, x2 - 1, y2 - 1);
	}

	static void DrawV(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleX = (((uint16_t)x2 + x1) / 2);

		frameBuffer->Line(color, x1, y1, middleX, y2);
		frameBuffer->Line(color, middleX, y2 - 1, x2 - 1, y1);
	}

	static void DrawW(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleX = (((uint16_t)x2 + x1) / 2);
		const uint8_t middleY = (((uint16_t)y2 + y1) / 2);

		const uint8_t margin = (((uint16_t)x2 - x1) / 4);

		frameBuffer->Line(color, x1, y1, middleX - margin, y2);
		frameBuffer->Line(color, middleX - margin, y2, middleX, middleY);
		frameBuffer->Line(color, middleX + margin, y2, middleX, middleY);
		frameBuffer->Line(color, x2 - 1, y1, middleX + margin, y2);
	}

	static void DrawX(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		frameBuffer->Line(color, x1, y1, x2 - 1, y2);
		frameBuffer->Line(color, x1, y2, x2 - 1, y1);
	}

	static void DrawY(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		const uint8_t middleX = (((uint16_t)x1 + x2) / 2);
		const uint8_t middleY = (((uint16_t)y1 + y2) / 2) + 2;

		frameBuffer->Line(color, x1, y1, middleX, middleY);
		frameBuffer->Line(color, x2 - 1, y1, middleX, middleY);
		frameBuffer->Line(color, middleX, middleY, middleX, y2);
	}

	static void DrawZ(IFrameBuffer* frameBuffer, const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2)
	{
		frameBuffer->Line(color, x1, y1, x2 - 1, y1);
		frameBuffer->Line(color, x1, y2 - 1, x2 - 1, y1);
		frameBuffer->Line(color, x1, y2 - 1, x2, y2 - 1);
	}
};
#endif