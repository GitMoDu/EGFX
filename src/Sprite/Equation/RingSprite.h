#ifndef _EGFX_RING_SPRITE_h
#define _EGFX_RING_SPRITE_h

#include "../../Model/ISprite.h"

namespace Egfx
{
	class RingSprite : public ISprite
	{
	private:
		pixel_index_t InnerRadiusPow = 0;
		pixel_index_t OuterRadiusPow = 0;
		pixel_t OuterRadius = 0;

	public:
		RingSprite() : ISprite()
		{
		}

		pixel_t GetWidth() const final
		{
			return OuterRadius * 2;
		}

		pixel_t GetHeight() const final
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
					OuterRadiusPow = ((pixel_index_t)outerRadius * outerRadius) - 1;
				}
				else
				{
					OuterRadiusPow = (pixel_index_t)outerRadius * outerRadius;
				}

				InnerRadiusPow = ((pixel_index_t)innerRadius * innerRadius) - 1;
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
			const int16_t dx = -(int16_t)OuterRadius + x;
			const int16_t dy = -(int16_t)OuterRadius + y;
			const pixel_index_t distancePow = (uint32_t)(((int32_t)dx * dx) + ((int32_t)dy * dy));

			return distancePow <= OuterRadiusPow && distancePow >= InnerRadiusPow;
		}

		virtual bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
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