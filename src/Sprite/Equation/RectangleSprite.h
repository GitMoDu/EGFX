#ifndef _EGFX_RECTANGLE_SPRITE_h
#define _EGFX_RECTANGLE_SPRITE_h

#include "../../Model/ISprite.h"

namespace Egfx
{
	template<const pixel_t width,
		const pixel_t height>
	class RectangleFixedSprite : public AbstractSprite<width, height>
	{
	public:
		RectangleFixedSprite() : AbstractSprite<width, height>()
		{
		}

		virtual bool Get(rgb_color_t& color, const pixel_t /*x*/, const pixel_t /*y*/)
		{
			color = Rgb::Color(UINT8_MAX, UINT8_MAX, UINT8_MAX);

			return true;
		}
	};

	class RectangleSprite : public ISprite
	{
	public:
		pixel_t Width = 0;
		pixel_t Height = 0;

	public:
		RectangleSprite() : ISprite()
		{
		}

		virtual bool Get(rgb_color_t& color, const pixel_t /*x*/, const pixel_t /*y*/)
		{
			color = Rgb::Color(UINT8_MAX, UINT8_MAX, UINT8_MAX);

			return true;
		}

		virtual pixel_t GetWidth() const
		{
			return Width;
		}

		virtual pixel_t GetHeight() const
		{
			return Height;
		}
	};
}
#endif