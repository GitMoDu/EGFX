#ifndef _EGFX_INVERT_TRANSFORM_h
#define _EGFX_INVERT_TRANSFORM_h

#include "../../Model/ITransform.h"

namespace Egfx
{
	namespace SpriteTransform
	{
		/// <summary>
		/// Invert X Axis only.
		/// </summary>
		/// <typeparam name="Width"></typeparam>
		/// <typeparam name="Height"></typeparam>
		/// <typeparam name="BaseTransform"></typeparam>
		template<const pixel_t Width,
			typename BaseTransform = ITransform>
		struct InvertXTransform : public BaseTransform
		{
			InvertXTransform() : BaseTransform() {}

			virtual bool Transform(pixel_t& x, pixel_t& y)
			{
				if (BaseTransform::Transform(x, y))
				{
					TransformStatic(x, y);

					return true;
				}

				return false;
			}

			static void TransformStatic(pixel_t& x, pixel_t& /*y*/)
			{
				x = Width - 1 - x;
			}
		};

		/// <summary>
		/// Invert Y axis only.
		/// </summary>
		/// <typeparam name="Height"></typeparam>
		/// <typeparam name="BaseTransform"></typeparam>
		template<const pixel_t Height,
			typename BaseTransform = ITransform>
		struct InvertYTransform : public BaseTransform
		{
			InvertYTransform() : BaseTransform() {}

			virtual bool Transform(pixel_t& x, pixel_t& y)
			{
				if (BaseTransform::Transform(x, y))
				{
					TransformStatic(x, y);

					return true;
				}

				return false;
			}

			static void TransformStatic(pixel_t& /*x*/, pixel_t& y)
			{
				y = Height - 1 - y;
			}
		};

		/// <summary>
		/// Equivalent to a +-180 degree rotation.
		/// </summary>
		/// <typeparam name="Width"></typeparam>
		/// <typeparam name="Height"></typeparam>
		template<const pixel_t Width, const pixel_t Height,
			typename BaseTransform = ITransform>
		struct InvertTransform : public BaseTransform
		{
			InvertTransform() : BaseTransform() {}

			virtual bool Transform(pixel_t& x, pixel_t& y)
			{
				if (BaseTransform::Transform(x, y))
				{
					TransformStatic(x, y);

					return true;
				}

				return false;
			}

			static void TransformStatic(pixel_t& x, pixel_t& y)
			{
				x = Width - 1 - x;
				y = Height - 1 - y;
			}
		};

		/// <summary>
		/// Flip X axis with Y axis.
		/// </summary>
		template<typename BaseTransform = ITransform>
		struct FlipTransform : public BaseTransform
		{
			FlipTransform() : BaseTransform() {}

			virtual bool Transform(pixel_t& x, pixel_t& y)
			{
				if (BaseTransform::Transform(x, y))
				{
					TransformStatic(x, y);

					return true;
				}

				return false;
			}

			static void TransformStatic(pixel_t& x, pixel_t& y)
			{
				const pixel_t xx = x;
				x = y;
				y = xx;
			}
		};

		/// <summary>
		/// Flip axis and invert X axis.
		/// Equivalent to a +90 degree rotation.
		/// </summary>
		/// <typeparam name="Height"></typeparam>
		template<const pixel_t Height,
			typename BaseTransform = ITransform>
		struct FlipInvertXTransform : public BaseTransform
		{
			FlipInvertXTransform() : BaseTransform() {}

			virtual bool Transform(pixel_t& x, pixel_t& y)
			{
				if (BaseTransform::Transform(x, y))
				{
					TransformStatic(x, y);

					return true;
				}

				return false;
			}

			static void TransformStatic(pixel_t& x, pixel_t& y)
			{
				const pixel_t xx = x;
				x = Height - 1 - y;
				y = xx;
			}
		};

		/// <summary>
		/// Flip axis and invert Y axis.
		/// Equivalent to a 270 degree (-90 degree) rotation.
		/// </summary>
		/// <typeparam name="Width"></typeparam>
		template<const pixel_t Width,
			typename BaseTransform = ITransform>
		struct FlipInvertYTransform : public BaseTransform
		{
			FlipInvertYTransform() : BaseTransform() {}

			virtual bool Transform(pixel_t& x, pixel_t& y)
			{
				if (BaseTransform::Transform(x, y))
				{
					TransformStatic(x, y);

					return true;
				}

				return false;
			}

			static void TransformStatic(pixel_t& x, pixel_t& y)
			{
				const pixel_t xx = x;
				x = y;
				y = Width - 1 - xx;
			}
		};
	}
}
#endif