#ifndef _EGFX_SCALE_TRANSFORM_h
#define _EGFX_SCALE_TRANSFORM_h

#include "../../Model/ITransform.h"

namespace Egfx
{
	namespace SpriteTransform
	{
		template<pixel_t Width,
			typename BaseTransform = ITransform>
		class DownScaleXTransform : public BaseTransform
		{
		private:
			pixel_t DownScale = 0;

		public:
			DownScaleXTransform() : BaseTransform() {}

			void SetDownScaleX(const pixel_t downScale)
			{
				DownScale = downScale;
			}

			pixel_t GetDownScaleX() const
			{
				return DownScale;
			}

			virtual bool Transform(pixel_t& x, pixel_t& y)
			{
				if (BaseTransform::Transform(x, y))
				{
					const pixel_t skips = ((pixel_index_t)DownScale * x) / Width;

					if (skips >= 0 &&
						(skips == 0
							|| (x % Width) != skips))
					{
						x = x - skips;
						return true;
					}
				}

				return false;
			}
		};

		template<pixel_t Height,
			typename BaseTransform = ITransform>
		class DownScaleYTransform : public BaseTransform
		{
		private:
			pixel_t DownScale = 0;

		public:
			DownScaleYTransform() : BaseTransform() {}

			void SetDownScaleY(const pixel_t downScale)
			{
				DownScale = downScale;
			}

			pixel_t GetDownScaleY() const
			{
				return DownScale;
			}

			virtual bool Transform(pixel_t& x, pixel_t& y)
			{
				if (BaseTransform::Transform(x, y))
				{
					const pixel_t skips = ((pixel_index_t)DownScale * y) / Height;

					if (skips >= 0 &&
						(skips == 0
							|| (y % Height) != skips))
					{
						y = y - skips;
						return true;
					}
				}
				return false;
			}
		};

		template<pixel_t Width,
			pixel_t Height,
			typename BaseTransform = ITransform>
		class DownScaleXYTransform : public BaseTransform
		{
		private:
			pixel_t DownScale = 0;

		public:
			DownScaleXYTransform() : BaseTransform() {}

			void SetDownScaleXY(const pixel_t downScale)
			{
				DownScale = downScale;
			}

			pixel_t GetDownScaleXY() const
			{
				return DownScale;
			}

			virtual bool Transform(pixel_t& x, pixel_t& y)
			{
				if (BaseTransform::Transform(x, y))
				{
					const pixel_t skipsX = ((pixel_index_t)DownScale * x) / Width;
					const pixel_t skipsY = ((pixel_index_t)DownScale * y) / Height;

					bool skipped = false;

					if (skipsY >= 0 &&
						(skipsY == 0
							|| (y % Height) != skipsY))
					{
						y = y - skipsY;
						skipped = true;
					}

					if (skipsX >= 0 &&
						(skipsX == 0
							|| (x % Width) != skipsX))
					{
						x = x - skipsX;

						return skipped;
					}
				}

				return false;
			}
		};
	}
}
#endif