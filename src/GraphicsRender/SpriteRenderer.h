// SpriteRenderer.h

#ifndef _EGFX_SPRITE_RENDERER_h
#define _EGFX_SPRITE_RENDERER_h

#include "../Model/ISprite.h"
#include "../Model/IFrameBuffer.h"
#include "../Model/ITransform.h"

namespace Egfx
{
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

			for (pixel_t y = 0; y < height; y++)
			{
				for (pixel_t x = 0; x < width; x++)
				{
					xNew = x;
					yNew = y;
					if (sprite->Get(color, xNew, yNew)
						&& transform->Transform(xNew, yNew))
					{
						frame->Pixel(color, xNew + positionX, yNew + positionY);
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
			if (positionY + line < 0)
			{
				return;
			}

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
			if (positionX + column < 0)
			{
				return;
			}

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
			const pixel_t xStart = MaxValue((pixel_t)0, (pixel_t)(positionX));
			const pixel_t yStart = MaxValue((pixel_t)0, (pixel_t)(positionY));

			const pixel_t xEnd = positionX + sprite->GetWidth();
			const pixel_t yEnd = positionY + sprite->GetHeight();

			rgb_color_t color{};

			for (pixel_t y = yStart; y < yEnd; y++)
			{
				for (pixel_t x = xStart; x < xEnd; x++)
				{
					if (sprite->Get(color, x - positionX, y - positionY))
					{
						frame->Pixel(color, x, y);
					}
				}
			}
		}
	};
}
#endif