// SpriteRenderer.h

#ifndef _SPRITE_RENDERER_h
#define _SPRITE_RENDERER_h

#include "../Model/ISprite.h"
#include "../Model/IFrameBuffer.h"

struct SpriteRenderer
{
	/// <summary>
	/// Draw sprite with coordinates(?) transform.
	/// </summary>
	/// <typeparam name="TransformType"></typeparam>
	/// <param name="frame"></param>
	/// <param name="sprite"></param>
	/// <param name="positionX"></param>
	/// <param name="positionY"></param>
	/// <param name="parameter"></param>
	template<typename TransformType>
	static void TransformDraw(IFrameBuffer* frame, ISprite* sprite,
		const uint8_t positionX, const uint8_t positionY,
		const int16_t parameter)
	{
		const uint8_t width = sprite->GetWidth();
		const uint8_t height = sprite->GetHeight();

		RgbColor color{};
		uint8_t xNew = 0;
		uint8_t yNew = 0;

		for (uint8_t x = 0; x < width; x++)
		{
			for (uint8_t y = 0; y < height; y++)
			{
				xNew = x;
				yNew = y;
				if (sprite->Get(color, x, y)
					&& TransformType::Transform(xNew, yNew, parameter))
				{
					frame->Pixel(color, positionX + xNew, positionY + yNew);
				}
			}
		}
	}

	/// <summary>
	/// Draw sprite with coordinates(?) transform.
	/// </summary>
	/// <typeparam name="TransformType"></typeparam>
	/// <param name="frame"></param>
	/// <param name="sprite"></param>
	/// <param name="positionX"></param>
	/// <param name="positionY"></param>
	/// <param name="parameter"></param>
	template<typename TransformType>
	static void TransformDrawPartial(IFrameBuffer* frame, ISprite* sprite,
		const uint8_t positionX, const uint8_t positionY,
		const int16_t parameter,
		const uint8_t offsetX, const uint8_t offsetY,
		const uint8_t width, const uint8_t height)
	{
		RgbColor color{};
		uint8_t xNew = 0;
		uint8_t yNew = 0;

		for (uint8_t x = offsetX; x < offsetX + width; x++)
		{
			for (uint8_t y = offsetY; y < offsetY + height; y++)
			{
				xNew = x;
				yNew = y;
				if (sprite->Get(color, x, y)
					&& TransformType::Transform(xNew, yNew, parameter))
				{
					frame->Pixel(color, positionX + xNew, positionY + yNew);
				}
			}
		}
	}

	/// <summary>
	/// Single sprite draw.
	/// </summary>
	/// <param name="frame"></param>
	/// <param name="sprite"></param>
	/// <param name="positionX"></param>
	/// <param name="positionY"></param>
	static void DrawPartial(IFrameBuffer* frame, ISprite* sprite,
		const uint8_t positionX, const uint8_t positionY,
		const uint8_t offsetX, const uint8_t offsetY,
		const uint8_t width, const uint8_t height)
	{
		RgbColor color{};

		for (uint8_t x = offsetX; x < offsetX + width; x++)
		{
			for (uint8_t y = offsetY; y < offsetY + height; y++)
			{
				if (sprite->Get(color, x, y))
				{
					frame->Pixel(color, positionX + x, positionY + y);
				}
			}
		}
	}

	/// <summary>
	/// Single sprite draw.
	/// </summary>
	/// <param name="frame"></param>
	/// <param name="sprite"></param>
	/// <param name="positionX"></param>
	/// <param name="positionY"></param>
	static void Draw(IFrameBuffer* frame, ISprite* sprite,
		const uint8_t positionX, const uint8_t positionY)
	{
		const uint8_t width = sprite->GetWidth();
		const uint8_t height = sprite->GetHeight();

		RgbColor color{};
		for (uint8_t x = 0; x < width; x++)
		{
			for (uint8_t y = 0; y < height; y++)
			{
				if (sprite->Get(color, x, y))
				{
					frame->Pixel(color, positionX + x, positionY + y);
				}
			}
		}
	}

	static void Draw(IFrameBuffer* frame, ISprite* sprite,
		const uint8_t position1X, const uint8_t position1Y,
		const uint8_t position2X, const uint8_t position2Y)
	{
		const uint8_t width = sprite->GetWidth();
		const uint8_t height = sprite->GetHeight();

		RgbColor color{};
		for (uint8_t x = 0; x < width; x++)
		{
			for (uint8_t y = 0; y < height; y++)
			{
				if (sprite->Get(color, x, y))
				{
					frame->Pixel(color, position1X + x, position1Y + y);
					frame->Pixel(color, position2X + x, position2Y + y);
				}
			}
		}
	}

	static void Draw(IFrameBuffer* frame, ISprite* sprite,
		const uint8_t position1X, const uint8_t position1Y,
		const uint8_t position2X, const uint8_t position2Y,
		const uint8_t position3X, const uint8_t position3Y)
	{
		const uint8_t width = sprite->GetWidth();
		const uint8_t height = sprite->GetHeight();

		RgbColor color{};
		uint8_t xNew = 0;
		uint8_t yNew = 0;

		for (uint8_t x = 0; x < width; x++)
		{
			for (uint8_t y = 0; y < height; y++)
			{
				xNew = x;
				yNew = y;
				if (sprite->Get(color, x, y))
				{
					frame->Pixel(color, position1X + xNew, position1Y + yNew);
					frame->Pixel(color, position2X + xNew, position2Y + yNew);
					frame->Pixel(color, position3X + xNew, position3Y + yNew);
				}
			}
		}
	}

	static void Draw(IFrameBuffer* frame, ISprite* sprite, const uint8_t position1X, const uint8_t position1Y,
		const uint8_t position2X, const uint8_t position2Y,
		const uint8_t position3X, const uint8_t position3Y,
		const uint8_t position4X, const uint8_t position4Y)
	{
		const uint8_t width = sprite->GetWidth();
		const uint8_t height = sprite->GetHeight();

		RgbColor color{};
		uint8_t xNew = 0;
		uint8_t yNew = 0;

		for (uint8_t x = 0; x < width; x++)
		{
			for (uint8_t y = 0; y < height; y++)
			{
				xNew = x;
				yNew = y;
				if (sprite->Get(color, x, y))
				{
					frame->Pixel(color, position1X + xNew, position1Y + yNew);
					frame->Pixel(color, position2X + xNew, position2Y + yNew);
					frame->Pixel(color, position3X + xNew, position3Y + yNew);
					frame->Pixel(color, position4X + xNew, position4Y + yNew);
				}
			}
		}
	}

	static void Draw(IFrameBuffer* frame, ISprite* sprite,
		const uint8_t position1X, const uint8_t position1Y,
		const uint8_t position2X, const uint8_t position2Y,
		const uint8_t position3X, const uint8_t position3Y,
		const uint8_t position4X, const uint8_t position4Y,
		const uint8_t position5X, const uint8_t position5Y)
	{
		const uint8_t width = sprite->GetWidth();
		const uint8_t height = sprite->GetHeight();

		RgbColor color{};
		uint8_t xNew = 0;
		uint8_t yNew = 0;

		for (uint8_t x = 0; x < width; x++)
		{
			for (uint8_t y = 0; y < height; y++)
			{
				xNew = x;
				yNew = y;
				if (sprite->Get(color, x, y))
				{
					frame->Pixel(color, position1X + xNew, position1Y + yNew);
					frame->Pixel(color, position2X + xNew, position2Y + yNew);
					frame->Pixel(color, position3X + xNew, position3Y + yNew);
					frame->Pixel(color, position4X + xNew, position4Y + yNew);
					frame->Pixel(color, position5X + xNew, position5Y + yNew);
				}
			}
		}
	}

	static void Draw(IFrameBuffer* frame, ISprite* sprite,
		const uint8_t position1X, const uint8_t position1Y,
		const uint8_t position2X, const uint8_t position2Y,
		const uint8_t position3X, const uint8_t position3Y,
		const uint8_t position4X, const uint8_t position4Y,
		const uint8_t position5X, const uint8_t position5Y,
		const uint8_t position6X, const uint8_t position6Y)
	{
		const uint8_t width = sprite->GetWidth();
		const uint8_t height = sprite->GetHeight();

		RgbColor color{};
		uint8_t xNew = 0;
		uint8_t yNew = 0;

		for (uint8_t x = 0; x < width; x++)
		{
			for (uint8_t y = 0; y < height; y++)
			{
				xNew = x;
				yNew = y;
				if (sprite->Get(color, x, y))
				{
					frame->Pixel(color, position1X + xNew, position1Y + yNew);
					frame->Pixel(color, position2X + xNew, position2Y + yNew);
					frame->Pixel(color, position3X + xNew, position3Y + yNew);
					frame->Pixel(color, position4X + xNew, position4Y + yNew);
					frame->Pixel(color, position5X + xNew, position5Y + yNew);
					frame->Pixel(color, position6X + xNew, position6Y + yNew);
				}
			}
		}
	}

	static void Draw(IFrameBuffer* frame, ISprite* sprite,
		const uint8_t position1X, const uint8_t position1Y,
		const uint8_t position2X, const uint8_t position2Y,
		const uint8_t position3X, const uint8_t position3Y,
		const uint8_t position4X, const uint8_t position4Y,
		const uint8_t position5X, const uint8_t position5Y,
		const uint8_t position6X, const uint8_t position6Y,
		const uint8_t position7X, const uint8_t position7Y)
	{
		const uint8_t width = sprite->GetWidth();
		const uint8_t height = sprite->GetHeight();

		RgbColor color{};
		uint8_t xNew = 0;
		uint8_t yNew = 0;

		for (uint8_t x = 0; x < width; x++)
		{
			for (uint8_t y = 0; y < height; y++)
			{
				xNew = x;
				yNew = y;
				if (sprite->Get(color, x, y))
				{
					frame->Pixel(color, position1X + xNew, position1Y + yNew);
					frame->Pixel(color, position2X + xNew, position2Y + yNew);
					frame->Pixel(color, position3X + xNew, position3Y + yNew);
					frame->Pixel(color, position4X + xNew, position4Y + yNew);
					frame->Pixel(color, position5X + xNew, position5Y + yNew);
					frame->Pixel(color, position6X + xNew, position6Y + yNew);
					frame->Pixel(color, position7X + xNew, position7Y + yNew);
				}
			}
		}
	}

	static void Draw(IFrameBuffer* frame, ISprite* sprite,
		const uint8_t position1X, const uint8_t position1Y,
		const uint8_t position2X, const uint8_t position2Y,
		const uint8_t position3X, const uint8_t position3Y,
		const uint8_t position4X, const uint8_t position4Y,
		const uint8_t position5X, const uint8_t position5Y,
		const uint8_t position6X, const uint8_t position6Y,
		const uint8_t position7X, const uint8_t position7Y,
		const uint8_t position8X, const uint8_t position8Y)
	{
		const uint8_t width = sprite->GetWidth();
		const uint8_t height = sprite->GetHeight();

		RgbColor color{};
		uint8_t xNew = 0;
		uint8_t yNew = 0;

		for (uint8_t x = 0; x < width; x++)
		{
			for (uint8_t y = 0; y < height; y++)
			{
				xNew = x;
				yNew = y;
				if (sprite->Get(color, x, y))
				{
					frame->Pixel(color, position1X + xNew, position1Y + yNew);
					frame->Pixel(color, position2X + xNew, position2Y + yNew);
					frame->Pixel(color, position3X + xNew, position3Y + yNew);
					frame->Pixel(color, position4X + xNew, position4Y + yNew);
					frame->Pixel(color, position5X + xNew, position5Y + yNew);
					frame->Pixel(color, position6X + xNew, position6Y + yNew);
					frame->Pixel(color, position7X + xNew, position7Y + yNew);
					frame->Pixel(color, position8X + xNew, position8Y + yNew);
				}
			}
		}
	}

	static void Draw(IFrameBuffer* frame, ISprite* sprite,
		const uint8_t position1X, const uint8_t position1Y,
		const uint8_t position2X, const uint8_t position2Y,
		const uint8_t position3X, const uint8_t position3Y,
		const uint8_t position4X, const uint8_t position4Y,
		const uint8_t position5X, const uint8_t position5Y,
		const uint8_t position6X, const uint8_t position6Y,
		const uint8_t position7X, const uint8_t position7Y,
		const uint8_t position8X, const uint8_t position8Y,
		const uint8_t position9X, const uint8_t position9Y)
	{
		const uint8_t width = sprite->GetWidth();
		const uint8_t height = sprite->GetHeight();

		RgbColor color{};
		uint8_t xNew = 0;
		uint8_t yNew = 0;

		for (uint8_t x = 0; x < width; x++)
		{
			for (uint8_t y = 0; y < height; y++)
			{
				xNew = x;
				yNew = y;
				if (sprite->Get(color, x, y))
				{
					frame->Pixel(color, position1X + xNew, position1Y + yNew);
					frame->Pixel(color, position2X + xNew, position2Y + yNew);
					frame->Pixel(color, position3X + xNew, position3Y + yNew);
					frame->Pixel(color, position4X + xNew, position4Y + yNew);
					frame->Pixel(color, position5X + xNew, position5Y + yNew);
					frame->Pixel(color, position6X + xNew, position6Y + yNew);
					frame->Pixel(color, position7X + xNew, position7Y + yNew);
					frame->Pixel(color, position8X + xNew, position8Y + yNew);
					frame->Pixel(color, position9X + xNew, position9Y + yNew);
				}
			}
		}
	}

	static void Draw(IFrameBuffer* frame, ISprite* sprite,
		const uint8_t position1X, const uint8_t position1Y,
		const uint8_t position2X, const uint8_t position2Y,
		const uint8_t position3X, const uint8_t position3Y,
		const uint8_t position4X, const uint8_t position4Y,
		const uint8_t position5X, const uint8_t position5Y,
		const uint8_t position6X, const uint8_t position6Y,
		const uint8_t position7X, const uint8_t position7Y,
		const uint8_t position8X, const uint8_t position8Y,
		const uint8_t position9X, const uint8_t position9Y,
		const uint8_t position10X, const uint8_t position10Y)
	{
		const uint8_t width = sprite->GetWidth();
		const uint8_t height = sprite->GetHeight();

		RgbColor color{};
		uint8_t xNew = 0;
		uint8_t yNew = 0;

		for (uint8_t x = 0; x < width; x++)
		{
			for (uint8_t y = 0; y < height; y++)
			{
				xNew = x;
				yNew = y;
				if (sprite->Get(color, x, y))
				{
					frame->Pixel(color, position1X + xNew, position1Y + yNew);
					frame->Pixel(color, position2X + xNew, position2Y + yNew);
					frame->Pixel(color, position3X + xNew, position3Y + yNew);
					frame->Pixel(color, position4X + xNew, position4Y + yNew);
					frame->Pixel(color, position5X + xNew, position5Y + yNew);
					frame->Pixel(color, position6X + xNew, position6Y + yNew);
					frame->Pixel(color, position7X + xNew, position7Y + yNew);
					frame->Pixel(color, position8X + xNew, position8Y + yNew);
					frame->Pixel(color, position9X + xNew, position9Y + yNew);
					frame->Pixel(color, position10X + xNew, position10Y + yNew);
				}
			}
		}
	}
};
#endif