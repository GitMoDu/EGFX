// SpriteRenderer.h

#ifndef _SPRITE_RENDERER_h
#define _SPRITE_RENDERER_h

#include "../Model/ISprite.h"
#include "../Model/IFrameBuffer.h"
#include "../Model/ITransform.h"

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
	//template<typename TransformType>
	static void TransformDraw(IFrameBuffer* frame, ISprite* sprite, ITransform* transform,
		const pixel_t positionX, const pixel_t positionY)
	{
		const pixel_t width = sprite->GetWidth();
		const pixel_t height = sprite->GetHeight();

		rgb_color_t color{};
		pixel_t xNew = 0;
		pixel_t yNew = 0;

		for (pixel_t x = 0; x < width; x++)
		{
			for (pixel_t y = 0; y < height; y++)
			{
				xNew = x;
				yNew = y;

				if (sprite->Get(color, x, y)
					&& transform->Transform(xNew, yNew))
				{
					frame->Pixel(color, positionX + xNew, positionY + yNew);
				}
			}
		}
	}

	/// <summary>
	/// Draw sprite with coordinates(?) transform.
	/// </summary>
	/// <param name="frame"></param>
	/// <param name="sprite"></param>
	/// <param name="positionX"></param>
	/// <param name="positionY"></param>
	/// <param name="parameter"></param>
	static void TransformDrawPartial(IFrameBuffer* frame, ISprite* sprite, ITransform* transform,
		const pixel_t positionX, const pixel_t positionY,
		const pixel_t offsetX, const pixel_t offsetY,
		const pixel_t width, const pixel_t height)
	{
		rgb_color_t color{};
		pixel_t xNew = 0;
		pixel_t yNew = 0;

		for (pixel_t x = offsetX; x < offsetX + width; x++)
		{
			for (pixel_t y = offsetY; y < offsetY + height; y++)
			{
				xNew = x;
				yNew = y;
				if (sprite->Get(color, x, y)
					&& transform->Transform(xNew, yNew))
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
	/// <param name="offsetX"></param>
	/// <param name="offsetY"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	static void DrawPartial(IFrameBuffer* frame, ISprite* sprite,
		const pixel_t positionX, const pixel_t positionY,
		const pixel_t offsetX, const pixel_t offsetY,
		const pixel_t width, const pixel_t height)
	{
		rgb_color_t color{};

		for (pixel_t x = offsetX; x < offsetX + width; x++)
		{
			for (pixel_t y = offsetY; y < offsetY + height; y++)
			{
				if (sprite->Get(color, x, y))
				{
					frame->Pixel(color, positionX + x, positionY + y);
				}
			}
		}
	}

	/// <summary>
	/// Draw a single line of a sprite
	/// </summary>
	/// <param name="frame"></param>
	/// <param name="sprite"></param>
	/// <param name="positionX"></param>
	/// <param name="positionY"></param>
	/// <param name="line">Y index of the line to be rendered.</param>
	static void DrawRow(IFrameBuffer* frame, ISprite* sprite,
		const pixel_t positionX, const pixel_t positionY,
		const pixel_t line)
	{
		const pixel_t width = sprite->GetWidth();

		rgb_color_t color{};

		for (pixel_t x = 0; x < width; x++)
		{
			if (sprite->Get(color, x, line))
			{
				frame->Pixel(color, positionX + x, positionY + line);
			}
		}
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="frame"></param>
	/// <param name="sprite"></param>
	/// <param name="positionX"></param>
	/// <param name="positionY"></param>
	/// <param name="column">X index of the column to be rendered.</param>
	static void DrawColumn(IFrameBuffer* frame, ISprite* sprite,
		const pixel_t positionX, const pixel_t positionY,
		const pixel_t column)
	{
		const pixel_t height = sprite->GetHeight();

		rgb_color_t color{};

		for (pixel_t y = 0; y < height; y++)
		{
			if (sprite->Get(color, column, y))
			{
				frame->Pixel(color, positionX + column, positionY + y);
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
		const pixel_t positionX, const pixel_t positionY)
	{
		const pixel_t width = sprite->GetWidth();
		const pixel_t height = sprite->GetHeight();

		rgb_color_t color{};
		for (pixel_t x = 0; x < width; x++)
		{
			for (pixel_t y = 0; y < height; y++)
			{
				if (sprite->Get(color, x, y))
				{
					frame->Pixel(color, positionX + x, positionY + y);
				}
			}
		}
	}

	static void Draw(IFrameBuffer* frame, ISprite* sprite,
		const pixel_t position1X, const pixel_t position1Y,
		const pixel_t position2X, const pixel_t position2Y)
	{
		const pixel_t width = sprite->GetWidth();
		const pixel_t height = sprite->GetHeight();

		rgb_color_t color{};
		for (pixel_t x = 0; x < width; x++)
		{
			for (pixel_t y = 0; y < height; y++)
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
		const pixel_t position1X, const pixel_t position1Y,
		const pixel_t position2X, const pixel_t position2Y,
		const pixel_t position3X, const pixel_t position3Y)
	{
		const pixel_t width = sprite->GetWidth();
		const pixel_t height = sprite->GetHeight();

		rgb_color_t color{};
		pixel_t xNew = 0;
		pixel_t yNew = 0;

		for (pixel_t x = 0; x < width; x++)
		{
			for (pixel_t y = 0; y < height; y++)
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

	static void Draw(IFrameBuffer* frame, ISprite* sprite, const pixel_t position1X, const pixel_t position1Y,
		const pixel_t position2X, const pixel_t position2Y,
		const pixel_t position3X, const pixel_t position3Y,
		const pixel_t position4X, const pixel_t position4Y)
	{
		const pixel_t width = sprite->GetWidth();
		const pixel_t height = sprite->GetHeight();

		rgb_color_t color{};
		pixel_t xNew = 0;
		pixel_t yNew = 0;

		for (pixel_t x = 0; x < width; x++)
		{
			for (pixel_t y = 0; y < height; y++)
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
		const pixel_t position1X, const pixel_t position1Y,
		const pixel_t position2X, const pixel_t position2Y,
		const pixel_t position3X, const pixel_t position3Y,
		const pixel_t position4X, const pixel_t position4Y,
		const pixel_t position5X, const pixel_t position5Y)
	{
		const pixel_t width = sprite->GetWidth();
		const pixel_t height = sprite->GetHeight();

		rgb_color_t color{};
		pixel_t xNew = 0;
		pixel_t yNew = 0;

		for (pixel_t x = 0; x < width; x++)
		{
			for (pixel_t y = 0; y < height; y++)
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
		const pixel_t position1X, const pixel_t position1Y,
		const pixel_t position2X, const pixel_t position2Y,
		const pixel_t position3X, const pixel_t position3Y,
		const pixel_t position4X, const pixel_t position4Y,
		const pixel_t position5X, const pixel_t position5Y,
		const pixel_t position6X, const pixel_t position6Y)
	{
		const pixel_t width = sprite->GetWidth();
		const pixel_t height = sprite->GetHeight();

		rgb_color_t color{};
		pixel_t xNew = 0;
		pixel_t yNew = 0;

		for (pixel_t x = 0; x < width; x++)
		{
			for (pixel_t y = 0; y < height; y++)
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
		const pixel_t position1X, const pixel_t position1Y,
		const pixel_t position2X, const pixel_t position2Y,
		const pixel_t position3X, const pixel_t position3Y,
		const pixel_t position4X, const pixel_t position4Y,
		const pixel_t position5X, const pixel_t position5Y,
		const pixel_t position6X, const pixel_t position6Y,
		const pixel_t position7X, const pixel_t position7Y)
	{
		const pixel_t width = sprite->GetWidth();
		const pixel_t height = sprite->GetHeight();

		rgb_color_t color{};
		pixel_t xNew = 0;
		pixel_t yNew = 0;

		for (pixel_t x = 0; x < width; x++)
		{
			for (pixel_t y = 0; y < height; y++)
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
		const pixel_t position1X, const pixel_t position1Y,
		const pixel_t position2X, const pixel_t position2Y,
		const pixel_t position3X, const pixel_t position3Y,
		const pixel_t position4X, const pixel_t position4Y,
		const pixel_t position5X, const pixel_t position5Y,
		const pixel_t position6X, const pixel_t position6Y,
		const pixel_t position7X, const pixel_t position7Y,
		const pixel_t position8X, const pixel_t position8Y)
	{
		const pixel_t width = sprite->GetWidth();
		const pixel_t height = sprite->GetHeight();

		rgb_color_t color{};
		pixel_t xNew = 0;
		pixel_t yNew = 0;

		for (pixel_t x = 0; x < width; x++)
		{
			for (pixel_t y = 0; y < height; y++)
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
		const pixel_t position1X, const pixel_t position1Y,
		const pixel_t position2X, const pixel_t position2Y,
		const pixel_t position3X, const pixel_t position3Y,
		const pixel_t position4X, const pixel_t position4Y,
		const pixel_t position5X, const pixel_t position5Y,
		const pixel_t position6X, const pixel_t position6Y,
		const pixel_t position7X, const pixel_t position7Y,
		const pixel_t position8X, const pixel_t position8Y,
		const pixel_t position9X, const pixel_t position9Y)
	{
		const pixel_t width = sprite->GetWidth();
		const pixel_t height = sprite->GetHeight();

		rgb_color_t color{};
		pixel_t xNew = 0;
		pixel_t yNew = 0;

		for (pixel_t x = 0; x < width; x++)
		{
			for (pixel_t y = 0; y < height; y++)
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
		const pixel_t position1X, const pixel_t position1Y,
		const pixel_t position2X, const pixel_t position2Y,
		const pixel_t position3X, const pixel_t position3Y,
		const pixel_t position4X, const pixel_t position4Y,
		const pixel_t position5X, const pixel_t position5Y,
		const pixel_t position6X, const pixel_t position6Y,
		const pixel_t position7X, const pixel_t position7Y,
		const pixel_t position8X, const pixel_t position8Y,
		const pixel_t position9X, const pixel_t position9Y,
		const pixel_t position10X, const pixel_t position10Y)
	{
		const pixel_t width = sprite->GetWidth();
		const pixel_t height = sprite->GetHeight();

		rgb_color_t color{};
		pixel_t xNew = 0;
		pixel_t yNew = 0;

		for (pixel_t x = 0; x < width; x++)
		{
			for (pixel_t y = 0; y < height; y++)
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