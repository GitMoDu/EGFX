// SkewTransform.h

#ifndef _SKEW_TRANSFORM_h
#define _SKEW_TRANSFORM_h

#include "../../Model/ITransform.h"

namespace SpriteTransform
{
	template<const pixel_t Height,
		typename BaseTransform = ITransform>
	class SkewHorizontalTransform : public BaseTransform
	{
	private:
		pixel_t ReferenceY = Height / 2;
		int8_t Skew = 0;

	public:
		SkewHorizontalTransform() : BaseTransform() {}

		virtual const bool Transform(pixel_t& x, pixel_t& y)
		{
			if (BaseTransform::Transform(x, y)
				&& y < Height)
			{
				x += ((((int16_t)ReferenceY - 1) - y) * Skew) / (Height - 1);

				return true;
			}

			return false;
		}

		void SetSkewX(const int8_t skewPixels)
		{
			Skew = skewPixels;
		}

		const int8_t GetSkewX() const
		{
			return Skew;
		}

		void SetReferenceY(const pixel_t reference)
		{
			ReferenceY = reference;
		}

		const pixel_t GetReferenceY() const
		{
			return ReferenceY;
		}
	};

	template<const pixel_t Width,
		typename BaseTransform = ITransform>
	class SkewVerticalTransform : public BaseTransform
	{
	private:
		pixel_t ReferenceX = Width / 2;
		int8_t Skew = 0;

	public:
		SkewVerticalTransform() : BaseTransform() {}

		virtual const bool Transform(pixel_t& x, pixel_t& y)
		{
			if (BaseTransform::Transform(x, y)
				&& x < Width)
			{
				y += ((((int16_t)ReferenceX - 1) - x) * Skew) / (Width - 1);

				return true;
			}

			return false;
		}

		void SetSkewY(const int8_t skewPixels)
		{
			Skew = skewPixels;
		}

		const int8_t GetSkewY() const
		{
			return Skew;
		}

		void SetReferenceX(const pixel_t reference)
		{
			ReferenceX = reference;
		}

		const pixel_t GetReferenceX() const
		{
			return ReferenceX;
		}
	};
}
#endif