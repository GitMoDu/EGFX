// CircleSprite.h

#ifndef _EGFX_CIRCLE_SPRITE_h
#define _EGFX_CIRCLE_SPRITE_h

#include "../../Model/ISprite.h"

namespace Egfx
{
	template<const pixel_t Radius>
	class CircleFixedSprite : public AbstractSprite<Radius * 2, Radius * 2>
	{
	private:
		static constexpr pixel_index_t GetRadiusPower(const pixel_t radius)
		{
			return ((radius > 1) * (((int32_t)radius * radius) - 1)) | ((radius <= 1) * (((int32_t)radius * radius)));
		}

	private:
		static constexpr pixel_index_t RadiusPow = GetRadiusPower(Radius);

	public:
		CircleFixedSprite() : AbstractSprite<Radius * 2, Radius * 2>()
		{
		}

		const bool IsInsideCircle(const pixel_t x, const pixel_t y)
		{
			const int16_t dx = -(int16_t)Radius + x + 1;
			const int16_t dy = -(int16_t)Radius + y + 1;
			const uint32_t distancePow = (uint32_t)(((int32_t)dx * dx) + ((int32_t)dy * dy));

			return distancePow <= RadiusPow;
		}

		virtual bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
		{
			if (IsInsideCircle(x, y))
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

	class CircleSprite : public ISprite
	{
	private:
		pixel_index_t RadiusPow = 0;
		pixel_t Radius = 0;

	public:
		CircleSprite() : ISprite()
		{
		}

		pixel_t GetWidth() const final
		{
			return GetRadius() * 2;
		}

		pixel_t GetHeight() const final
		{
			return GetWidth();
		}

		pixel_t GetRadius() const
		{
			return Radius;
		}

		void SetRadius(const pixel_t radius)
		{
			Radius = radius;
			if (radius > 1)
			{
				RadiusPow = ((pixel_index_t)radius * radius) - 1;
			}
			else
			{
				RadiusPow = ((pixel_index_t)radius * radius);
			}
		}

		const bool IsInsideCircle(const pixel_t x, const pixel_t y)
		{
			const int16_t dx = -(int16_t)Radius + x + 1;
			const int16_t dy = -(int16_t)Radius + y + 1;
			const pixel_index_t distancePow = (pixel_index_t)(((int32_t)dx * dx) + ((int32_t)dy * dy));

			return distancePow <= RadiusPow;
		}

		virtual bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
		{
			if (IsInsideCircle(x, y))
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