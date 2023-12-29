// NumberRenderer.h

#ifndef _NUMBER_RENDERER_h
#define _NUMBER_RENDERER_h

#include "FontRenderer.h"
#include <String.h>

class NumberRenderer : public FontRenderer
{
private:
	static constexpr uint8_t NumberOffset = (uint8_t)'0';

private:
	static const uint32_t PowerOfTen(const uint8_t power)
	{
		if (power > 0)
		{
			uint32_t value = 10;
			for (uint8_t i = 0; i < power - 1; i++)
			{
				value *= 10;
			}

			return value;
		}
		else
		{
			return 1;
		}
	}

public:
	static void NumberTopLeft(IFramePrimitives* frame, const uint8_t x1, const uint8_t y1, const FontStyle& font, const uint8_t number)
	{
		const uint8_t x2 = x1 + font.Width - 1;
		const uint8_t y2 = y1 + font.Height;

		if (number > 99)
		{
			const uint8_t offset = font.Width + font.Kerning;
			const uint8_t tens = number / 10;

			WriteCharacter(frame, x1 + (offset * 2), y1, x2 + (offset * 2), y2, font.Color, font.Stroke, (const uint8_t)(NumberOffset + (number % 10)));
			WriteCharacter(frame, x1 + offset, y1, x2 + offset, y2, font.Color, font.Stroke, (const uint8_t)(NumberOffset + (tens % 10)));
			WriteCharacter(frame, x1, y1, x2, y2, font.Color, font.Stroke, (const uint8_t)(NumberOffset + ((tens / 10) % 10)));
		}
		else if (number > 9)
		{
			const uint8_t offset = font.Width + font.Kerning;
			WriteCharacter(frame, x1, y1, x2, y2, font.Color, font.Stroke, (const uint8_t)(NumberOffset + ((number / 10) % 10)));
			WriteCharacter(frame, x1 + offset, y1, x2 + offset, y2, font.Color, font.Stroke, (const uint8_t)(NumberOffset + (number % 10)));
		}
		else
		{
			WriteCharacter(frame, x1, y1, x2, y2, font.Color, font.Stroke, (const uint8_t)(NumberOffset + (number % 10)));
		}
	}

	static void NumberTopRight(IFramePrimitives* frame, const uint8_t x2, const uint8_t y1, const FontStyle& font, const uint8_t number)
	{
		const uint8_t x1 = x2 - font.Width;
		const uint8_t y2 = y1 + font.Height;
		const uint8_t offset = font.Width + font.Kerning;

		WriteCharacter(frame, x1, y1, x2, y2, font.Color, font.Stroke, (const uint8_t)(NumberOffset + (number % 10)));
		if (number > 9
			&& (x2 >= offset))
		{
			uint16_t remainder = number / 10;
			WriteCharacter(frame, x1 - offset, y1, x2 - offset, y2, font.Color, font.Stroke, (const uint8_t)(NumberOffset + (remainder % 10)));

			if (number > 99
				&& (x2 >= (offset * 2)))
			{
				remainder /= 10;
				WriteCharacter(frame, x1 - (offset * 2), y1, x2 - (offset * 2), y2, font.Color, font.Stroke, (const uint8_t)(NumberOffset + (remainder % 10)));
			}
		}
	}

	static void NumberBottomRight(IFramePrimitives* frame, const uint8_t x2, const uint8_t y2, const FontStyle& font, const uint8_t number)
	{
		NumberTopRight(frame, x2, y2 - font.Height, font, number);
	}

	static void NumberBottomLeft(IFramePrimitives* frame, const uint8_t x1, const uint8_t y2, const FontStyle& font, const uint8_t number)
	{
		NumberTopLeft(frame, x1, y2 - font.Height, font, number);
	}

public:
	static void NumberTopLeft(IFramePrimitives* frame, const uint8_t x1, const uint8_t y1, const FontStyle& font, const uint16_t number)
	{
		if (number > UINT8_MAX)
		{
			const uint8_t x2 = x1 + font.Width - 1;
			const uint8_t y2 = y1 + font.Height;

			uint8_t offset = 0;
			bool significantFound = false;
			for (uint_fast8_t i = 0; i < 4; i++)
			{
				const uint16_t near = PowerOfTen(4 - i) - 1;

				if (!significantFound)
				{
					significantFound = number > near;
				}

				if (significantFound)
				{
					if ((x2 + offset) < frame->GetWidth())
					{
						const uint8_t remainder = (number / (near + 1)) % 10;
						WriteCharacter(frame, x1 + offset, y1, x2 + offset, y2, font.Color, font.Stroke, (const uint8_t)(NumberOffset + (remainder)));
						offset += font.Width + font.Kerning;
					}
					else
					{
						return;
					}
				}
			}

			WriteCharacter(frame, x1 + offset, y1, x2 + offset, y2, font.Color, font.Stroke, (const uint8_t)(NumberOffset + (number % 10)));
		}
		else
		{
			NumberTopLeft(frame, x1, y1, font, (const uint8_t)number);
		}
	}

	static void NumberTopRight(IFramePrimitives* frame, const uint8_t x2, const uint8_t y1, const FontStyle& font, const uint16_t number)
	{
		if (number > UINT8_MAX)
		{
			const uint8_t x1 = x2 - font.Width;
			const uint8_t y2 = y1 + font.Height;
			const uint8_t offset = font.Width + font.Kerning;

			WriteCharacter(frame, x1, y1, x2, y2, font.Color, font.Stroke, (const uint8_t)(NumberOffset + (number % 10)));
			uint16_t remainder = number / 10;
			for (uint_fast8_t i = 1; i < 5; i++)
			{
				const uint16_t near = PowerOfTen(i) - 1;

				if (number > near
					&& x2 >= (offset * i))
				{
					WriteCharacter(frame, x1 - (offset * i), y1, x2 - (offset * i), y2, font.Color, font.Stroke, (const uint8_t)(NumberOffset + (remainder % 10)));
					remainder /= 10;
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			NumberTopRight(frame, x2, y1, font, (const uint8_t)number);
		}
	}

	static void NumberBottomRight(IFramePrimitives* frame, const uint8_t x2, const uint8_t y2, const FontStyle& font, const uint16_t number)
	{
		NumberTopRight(frame, x2, y2 - font.Height, font, number);
	}

	static void NumberBottomLeft(IFramePrimitives* frame, const uint8_t x1, const uint8_t y2, const FontStyle& font, const uint16_t number)
	{
		NumberTopLeft(frame, x1, y2 - font.Height, font, number);
	}

public:
	static void NumberTopLeft(IFramePrimitives* frame, const uint8_t x1, const uint8_t y1, const FontStyle& font, const uint32_t number)
	{
		if (number > UINT16_MAX)
		{
			if (y1 < frame->GetHeight())
			{
				const uint8_t x2 = x1 + font.Width - 1;
				const uint8_t y2 = y1 + font.Height;

				uint8_t offset = 0;
				bool significantFound = false;
				for (uint_fast8_t i = 0; i < 9; i++)
				{
					const uint32_t near = PowerOfTen(9 - i) - 1;

					if (!significantFound)
					{
						significantFound = number > near;
					}

					if (significantFound)
					{
						if ((x2 + offset) < frame->GetWidth())
						{
							const uint8_t remainder = (number / (near + 1)) % 10;
							WriteCharacter(frame, x1 + offset, y1, x2 + offset, y2, font.Color, font.Stroke, (const uint8_t)(NumberOffset + (remainder)));
							offset += font.Width + font.Kerning;
						}
						else
						{
							return;
						}
					}
				}

				WriteCharacter(frame, x1 + offset, y1, x2 + offset, y2, font.Color, font.Stroke, (const uint8_t)(NumberOffset + (number % 10)));
			}
		}
		else
		{
			NumberTopLeft(frame, x1, y1, font, (const uint16_t)number);
		}
	}

	static void NumberTopRight(IFramePrimitives* frame, const uint8_t x2, const uint8_t y1, const FontStyle& font, const uint32_t number)
	{
		if (number > UINT16_MAX)
		{
			if (y1 < frame->GetHeight())
			{
				const uint8_t x1 = x2 - font.Width;
				const uint8_t y2 = y1 + font.Height;
				const uint8_t offset = font.Width + font.Kerning;

				WriteCharacter(frame, x1, y1, x2, y2, font.Color, font.Stroke, (const uint8_t)(NumberOffset + (number % 10)));
				uint32_t remainder = number / 10;
				uint64_t digit = 10;
				for (uint_fast8_t i = 1; i < 11; i++)
				{
					if (number > (digit - 1)
						&& x2 >= (offset * i))
					{
						WriteCharacter(frame, x1 - (offset * i), y1, x2 - (offset * i), y2, font.Color, font.Stroke, (const uint8_t)(NumberOffset + (remainder % 10)));
						remainder /= 10;
						digit *= 10;
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
			NumberTopRight(frame, x2, y1, font, (const uint16_t)number);
		}
	}

	static void NumberBottomRight(IFramePrimitives* frame, const uint8_t x2, const uint8_t y2, const FontStyle& font, const uint32_t number)
	{
		NumberTopRight(frame, x2, y2 - font.Height, font, number);
	}

	static void NumberBottomLeft(IFramePrimitives* frame, const uint8_t x1, const uint8_t y2, const FontStyle& font, const uint32_t number)
	{
		NumberTopLeft(frame, x1, y2 - font.Height, font, number);
	}

public:
	static void NumberTopLeft(IFramePrimitives* frame, const uint8_t x1, const uint8_t y1, const FontStyle& font, const int number)
	{
		NumberTopLeft(frame, x1, y1, font, (const uint32_t)number);
	}

	static void NumberTopRight(IFramePrimitives* frame, const uint8_t x2, const uint8_t y1, const FontStyle& font, const int number)
	{
		NumberTopRight(frame, x2, y1, font, (const uint32_t)number);
	}

	static void NumberBottomRight(IFramePrimitives* frame, const uint8_t x2, const uint8_t y2, const FontStyle& font, const int number)
	{
		NumberTopRight(frame, x2, y2 - font.Height, font, (const uint32_t)number);
	}

	static void NumberBottomLeft(IFramePrimitives* frame, const uint8_t x1, const uint8_t y2, const FontStyle& font, const int number)
	{
		NumberTopLeft(frame, x1, y2 - font.Height, font, (const uint32_t)number);
	}
};
#endif