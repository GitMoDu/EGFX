// ScaleTransform.h

#ifndef _EGFX_SCALE_TRANSFORM_h
#define _EGFX_SCALE_TRANSFORM_h

#include "../../Model/ITransform.h"

namespace Egfx::SpriteTransform
{
	template<const pixel_t Width,
		typename BaseTransform = ITransform>
	class DownScaleXTransform : public BaseTransform
	{
	private:
		uint8_t DownScale = 0;

	public:
		DownScaleXTransform() : BaseTransform() {}

		void SetDownScaleX(const uint8_t downScale)
		{
			DownScale = downScale;
		}

		const uint8_t GetDownScaleX()
		{
			return DownScale;
		}

		virtual const bool Transform(pixel_t& x, pixel_t& y)
		{
			if (BaseTransform::Transform(x, y))
			{
				const uint8_t skips = ((uint16_t)DownScale * x) / Width;

				if (skips == 0
					|| (x % Width) != skips)
				{
					x = x - skips;
					return true;
				}
			}

			return false;
		}
	};

	template<const pixel_t Height,
		typename BaseTransform = ITransform>
	class DownScaleYTransform : public BaseTransform
	{
	private:
		uint8_t DownScale = 0;

	public:
		DownScaleYTransform() : BaseTransform() {}

		void SetDownScaleY(const uint8_t downScale)
		{
			DownScale = downScale;
		}

		const uint8_t GetDownScaleY()
		{
			return DownScale;
		}

		virtual const bool Transform(pixel_t& x, pixel_t& y)
		{
			if (BaseTransform::Transform(x, y))
			{
				const uint8_t skips = (DownScale * y) / Height;

				if (skips == 0
					|| (y % Height) != skips)
				{
					y = y - skips;
					return true;
				}
			}
			return false;
		}
	};

	template<const pixel_t Width,
		const pixel_t Height,
		typename BaseTransform = ITransform>
	class DownScaleXYTransform : public BaseTransform
	{
	private:
		uint8_t DownScale = 0;

	public:
		DownScaleXYTransform() : BaseTransform() {}

		void SetDownScaleXY(const uint8_t downScale)
		{
			DownScale = downScale;
		}

		const uint8_t GetDownScaleXY()
		{
			return DownScale;
		}

		virtual const bool Transform(pixel_t& x, pixel_t& y)
		{
			if (BaseTransform::Transform(x, y))
			{
				const uint8_t skipsX = (DownScale * x) / Width;
				const uint8_t skipsY = (DownScale * y) / Height;

				if ((skipsX == 0 || (x % Width) != skipsX) && (skipsY == 0 || (y % Height) != skipsY))
				{
					x = x - skipsX;
					y = y - skipsY;

					return true;
				}
			}

			return false;
		}
	};
};
#endif