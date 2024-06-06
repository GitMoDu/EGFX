// NumberRenderer.h

#ifndef _NUMBER_RENDERER_h
#define _NUMBER_RENDERER_h

#include "DigitRenderer.h"
#include "../Model/Character.h"


/// <summary>
/// Draws numbers to screen using DigitRenderer.
/// </summary>
class NumberRenderer
{
public:
	static void NumberTopLeft(IFrameBuffer* frame, const FontStyle& font, const uint8_t x1, const uint8_t y1, const uint8_t number)
	{
		if (number > 99)
		{
			const uint8_t offset = font.Width + font.Kerning;
			const uint8_t tens = number / 10;

			DigitRenderer::Digit(frame, font, x1 + (offset * 2), y1, number % 10);
			DigitRenderer::Digit(frame, font, x1 + offset, y1, tens % 10);
			DigitRenderer::Digit(frame, font, x1, y1, tens / 10);
		}
		else if (number > 9)
		{
			const uint8_t offset = font.Width + font.Kerning;
			DigitRenderer::Digit(frame, font, x1, y1, number / 10);
			DigitRenderer::Digit(frame, font, x1 + offset, y1, number % 10);
		}
		else
		{
			DigitRenderer::Digit(frame, font, x1, y1, number % 10);
		}
	}

	static void NumberTopRight(IFrameBuffer* frame, const FontStyle& font, const uint8_t x2, const uint8_t y1, const uint8_t number)
	{
		const uint8_t x1 = x2 - font.Width;
		const uint8_t offset = font.Width + font.Kerning;

		DigitRenderer::Digit(frame, font, x1, y1, number % 10);
		if (number >= 10
			&& (x2 >= offset))
		{
			uint16_t remainder = number / 10;
			DigitRenderer::Digit(frame, font, x1 - offset, y1, remainder % 10);

			if (number >= 100
				&& (x2 >= (offset * 2)))
			{
				DigitRenderer::Digit(frame, font, x1 - (offset * 2), y1, remainder / 10);
			}
		}
	}

	static void NumberBottomRight(IFrameBuffer* frame, const FontStyle& font, const uint8_t x2, const uint8_t y2, const uint8_t number)
	{
		NumberTopRight(frame, font, x2, y2 - font.Height, number);
	}

	static void NumberBottomLeft(IFrameBuffer* frame, const FontStyle& font, const uint8_t x1, const uint8_t y2, const uint8_t number)
	{
		NumberTopLeft(frame, font, x1, y2 - font.Height, number);
	}

public:
	static void NumberTopLeft(IFrameBuffer* frame, const FontStyle& font, const uint8_t x1, const uint8_t y1, const uint16_t number)
	{
		if (number > UINT8_MAX)
		{
			if (x1 < frame->GetWidth() - font.Width
				&& y1 < frame->GetHeight())
			{
				uint8_t offset = 0;
				for (int_fast8_t i = 3; i >= 0; i--)
				{
					const uint16_t power = (uint16_t)Character::PowerOfTen[i];

					if (number >= power)
					{
						DigitRenderer::Digit(frame, font, x1 + offset, y1, ((number / power) % 10));
						offset += font.Width + font.Kerning;
					}
				}
				DigitRenderer::Digit(frame, font, x1 + offset, y1, number % 10);
			}
		}
		else
		{
			NumberTopLeft(frame, font, x1, y1, (const uint8_t)number);
		}
	}

	static void NumberTopRight(IFrameBuffer* frame, const FontStyle& font, const uint8_t x2, const uint8_t y1, const uint16_t number)
	{
		if (number > UINT8_MAX)
		{
			if (x2 < frame->GetWidth()
				&& y1 < frame->GetHeight())
			{
				const uint8_t x1 = x2 - font.Width;

				DigitRenderer::Digit(frame, font, x1, y1, number % 10);

				uint16_t remainder = number / 10;
				uint8_t offset = font.Width + font.Kerning;
				for (uint_fast8_t i = 0; i < 4; i++)
				{
					const uint16_t power = (uint16_t)Character::PowerOfTen[i];
					if (number >= power)
					{
						DigitRenderer::Digit(frame, font, x1 - offset, y1, remainder % 10);

						offset += font.Width + font.Kerning;
						remainder /= 10;
					}
					else
					{
						return;
					}
				}
			}
		}
		else
		{
			NumberTopRight(frame, font, x2, y1, (const uint8_t)number);
		}
	}

public:
	static void NumberTopLeft(IFrameBuffer* frame, const FontStyle& font, const uint8_t x1, const uint8_t y1, const uint32_t number)
	{
		if (number > UINT16_MAX)
		{
			if (x1 < frame->GetWidth() - font.Width
				&& y1 < frame->GetHeight())
			{
				uint8_t offset = 0;
				for (int_fast8_t i = 8; i >= 0; i--)
				{
					const uint32_t power = Character::PowerOfTen[i];
					if (number >= power)
					{
						DigitRenderer::Digit(frame, font, x1 + offset, y1, ((number / power) % 10));
						offset += font.Width + font.Kerning;
					}
				}
				DigitRenderer::Digit(frame, font, x1 + offset, y1, number % 10);
			}
		}
		else
		{
			NumberTopLeft(frame, font, x1, y1, (const uint16_t)number);
		}
	}

	static void NumberTopRight(IFrameBuffer* frame, const FontStyle& font, const uint8_t x2, const uint8_t y1, const uint32_t number)
	{
		if (number > UINT16_MAX)
		{
			if (x2 < frame->GetWidth()
				&& y1 < frame->GetHeight())
			{
				const uint8_t x1 = x2 - font.Width;

				DigitRenderer::Digit(frame, font, x1, y1, number % 10);

				uint32_t remainder = number / 10;
				uint8_t offset = font.Width + font.Kerning;
				for (uint_fast8_t i = 0; i < 9; i++)
				{
					if (number >= Character::PowerOfTen[i])
					{
						DigitRenderer::Digit(frame, font, x1 - offset, y1, remainder % 10);

						offset += font.Width + font.Kerning;
						remainder /= 10;
					}
					else
					{
						return;
					}
				}
			}
		}
		else
		{
			NumberTopRight(frame, font, x2, y1, (const uint16_t)number);
		}
	}

	static void NumberBottomRight(IFrameBuffer* frame, const FontStyle& font, const uint8_t x2, const uint8_t y2, const uint32_t number)
	{
		NumberTopRight(frame, font, x2, y2 - font.Height, number);
	}

	static void NumberBottomLeft(IFrameBuffer* frame, const FontStyle& font, const uint8_t x1, const uint8_t y2, const uint32_t number)
	{
		NumberTopLeft(frame, font, x1, y2 - font.Height, number);
	}

public:
	static void NumberTopLeft(IFrameBuffer* frame, const FontStyle& font, const uint8_t x1, const uint8_t y1, const int number)
	{
		NumberTopLeft(frame, font, x1, y1, (const uint32_t)number);
	}

	static void NumberTopRight(IFrameBuffer* frame, const FontStyle& font, const uint8_t x2, const uint8_t y1, const int number)
	{
		NumberTopRight(frame, font, x2, y1, (const uint32_t)number);
	}

	static void NumberBottomRight(IFrameBuffer* frame, const FontStyle& font, const uint8_t x2, const uint8_t y2, const int number)
	{
		NumberTopRight(frame, font, x2, y2 - font.Height, (const uint32_t)number);
	}

	static void NumberBottomLeft(IFrameBuffer* frame, const FontStyle& font, const uint8_t x1, const uint8_t y2, const int number)
	{
		NumberTopLeft(frame, font, x1, y2 - font.Height, (const uint32_t)number);
	}
};
#endif