// RingSprite.h

#ifndef _EGFX_RING_SPRITE_h
#define _EGFX_RING_SPRITE_h

#include "../../Model/ISprite.h"

namespace Egfx
{
	class RingSprite : public ISprite
	{
	private:
		coordinate_t InnerRadiusPow = 0;
		coordinate_t OuterRadiusPow = 0;
		pixel_t OuterRadius = 0;

	public:
		RingSprite() : ISprite()
		{
		}

		const pixel_t GetWidth() const final
		{
			return OuterRadius * 2;
		}

		const pixel_t GetHeight() const final
		{
			return GetWidth();
		}

		void SetRadius(const pixel_t innerRadius, const pixel_t outerRadius)
		{
			if (innerRadius < outerRadius)
			{
				OuterRadius = outerRadius;

				if (outerRadius > 1)
				{
					OuterRadiusPow = ((coordinate_t)outerRadius * outerRadius) - 1;
				}
				else
				{
					OuterRadiusPow = (coordinate_t)outerRadius * outerRadius;
				}

				InnerRadiusPow = ((coordinate_t)innerRadius * innerRadius) - 1;
			}
			else
			{
				OuterRadius = 0;
				InnerRadiusPow = 0;
				OuterRadiusPow = 0;
			}
		}

		const bool IsInsideRing(const pixel_t x, const pixel_t y)
		{
			const coordinate_signed_t dx = -(coordinate_signed_t)OuterRadius + x;
			const coordinate_signed_t dy = -(coordinate_signed_t)OuterRadius + y;
			const coordinate_t distancePow = (coordinate_t)((dx * dx) + (dy * dy));

			return distancePow <= OuterRadiusPow
				&& distancePow >= InnerRadiusPow;
		}

		virtual const bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
		{
			if (IsInsideRing(x, y))
			{
				color = Rgb::Color(UINT8_MAX, UINT8_MAX, UINT8_MAX);

				return true;
			}
			else
			{
				return false;
			}
		}
	};
}
#endif