#ifndef _EGFX_SKEW_TRANSFORM_h
#define _EGFX_SKEW_TRANSFORM_h

#include "../../Model/ITransform.h"

namespace Egfx
{
	namespace SpriteTransform
	{
		/// <summary>
		///  Pixel perfect horizontal skew transform.
		/// </summary>
		/// <typeparam name="BaseTransform">The base transform class to inherit from, defaults to ITransform. Allows chaining of multiple transformations.</typeparam>
		/// <typeparam name="Height">The total height in pixels of the coordinate space being transformed.</typeparam>
		template<pixel_t Height,
			typename BaseTransform = ITransform>
		class SkewHorizontalTransform : public BaseTransform
		{
		private:
			pixel_t ReferenceY = Height / 2;
			pixel_t Skew = 0;

		public:
			SkewHorizontalTransform() : BaseTransform() {}

			virtual bool Transform(pixel_t& x, pixel_t& y)
			{
				if (BaseTransform::Transform(x, y))
				{
					if (Skew != 0)
					{
						x += (((static_cast<pixel_index_t>(y) - ReferenceY)) * Skew) / (Height - 1);
					}
					return true;
				}

				return false;
			}

			void SetSkewX(const pixel_t skewPixels)
			{
				Skew = skewPixels;
			}

			pixel_t GetSkewX() const
			{
				return Skew;
			}

			void SetReferenceY(const pixel_t reference)
			{
				ReferenceY = reference;
			}

			pixel_t GetReferenceY() const
			{
				return ReferenceY;
			}
		};

		/// <summary>
		/// Pixel perfect vertical skew transform.
		/// </summary>
		/// <typeparam name="BaseTransform">The base transformation class to inherit from. Defaults to ITransform.</typeparam>
		/// <typeparam name="Width">The width in pixels used for skew calculations and default reference point.</typeparam>
		template<pixel_t Width,
			typename BaseTransform = ITransform>
		class SkewVerticalTransform : public BaseTransform
		{
		private:
			pixel_t ReferenceX = Width / 2;
			pixel_t Skew = 0;

		public:
			SkewVerticalTransform() : BaseTransform() {}

			virtual bool Transform(pixel_t& x, pixel_t& y)
			{
				if (BaseTransform::Transform(x, y))
				{
					if (Skew != 0)
					{
						y += (((static_cast<pixel_index_t>(x) - ReferenceX)) * Skew) / (Width - 1);
					}

					return true;
				}

				return false;
			}

			void SetSkewY(const pixel_t skewPixels)
			{
				Skew = skewPixels;
			}

			pixel_t GetSkewY() const
			{
				return Skew;
			}

			void SetReferenceX(const pixel_t reference)
			{
				ReferenceX = reference;
			}

			pixel_t GetReferenceX() const
			{
				return ReferenceX;
			}
		};
	}
}
#endif