// RectangleSprite.h

#ifndef _EGFX_RECTANGLE_SPRITE_h
#define _EGFX_RECTANGLE_SPRITE_h

#include "../../Model/ISprite.h"

namespace Egfx
{
	template<const pixel_t width,
		const pixel_t height>
	class RectangleSprite : public AbstractSprite<width, height>
	{
	public:
		RectangleSprite() : AbstractSprite<width, height>()
		{
		}

		virtual const bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
		{
			color = Rgb::Color(UINT8_MAX, UINT8_MAX, UINT8_MAX);

			return true;
		}
	};
}
#endif