// RotateTransform.h

#ifndef _EGFX_ROTATE_TRANSFORM_h
#define _EGFX_ROTATE_TRANSFORM_h

#include "SkewTransform.h"
#include "InvertTransform.h"
#if defined(EGFX_PLATFORM_BIG)
#include <IntegerTrigonometry16.h>
#else
#include <IntegerTrigonometry8.h>
#endif

namespace Egfx::SpriteTransform
{
	/// <summary>
	/// Pixel-retaining rotate done with 3 consecutive skews.
	/// </summary>
	/// <typeparam name="Width"></typeparam>
	/// <typeparam name="Height"></typeparam>
	template<const pixel_t Width,
		const pixel_t Height,
		typename BaseTransform = ITransform>
	class RotateTransform : public BaseTransform
	{
	private:
		enum class FlippingEnum : uint8_t
		{
			NoFlip,
			Invert,
			FlipInvertX,
			FlipInvertY,
			InvertX,
			InvertY,
		};

	private:
		SpriteTransform::SkewHorizontalTransform<Height> SkewHorizontal{};
		SpriteTransform::SkewVerticalTransform<Width> SkewVertical{};
		FlippingEnum Flip = FlippingEnum::NoFlip;

	private:
		angle_t Angle = 0;

	public:
		RotateTransform() : BaseTransform() {}

		void SetRotation(const angle_t angle)
		{
			// Set the rotation angle.
			Angle = angle;
			angle_t adjustedAngle = angle;
			Flip = FlippingEnum::NoFlip;

			// Handle direct static transformations for specific angles.
			switch (adjustedAngle)
			{
			case 0:
				// No transformation needed for 0 degrees.
				SkewHorizontal.SetSkewX(0);
				SkewVertical.SetSkewY(0);
				return;
			case ANGLE_90:
				// Apply 90-degree rotation transformation.
				SkewHorizontal.SetSkewX(0);
				SkewVertical.SetSkewY(0);
				Flip = FlippingEnum::FlipInvertX;
				return;
			case ANGLE_180:
				// Apply 180-degree rotation transformation.
				SkewHorizontal.SetSkewX(0);
				SkewVertical.SetSkewY(0);
				Flip = FlippingEnum::Invert;
				return;
			case ANGLE_270:
				// Apply 270-degree rotation transformation.
				SkewHorizontal.SetSkewX(0);
				SkewVertical.SetSkewY(0);
				Flip = FlippingEnum::FlipInvertY;
				return;
			}

			// Adjust angle for transformations beyond 180 degrees.
			if (adjustedAngle > ANGLE_180)
			{
				// Apply 180-degree inversion transformation.
				Flip = FlippingEnum::Invert;
				adjustedAngle -= ANGLE_180;

				// Adjust angle for transformations beyond 270 degrees.
				if (adjustedAngle > ANGLE_90)
				{
					// Apply 90-degree flip inversion transformation.
					Flip = FlippingEnum::FlipInvertY;
					adjustedAngle -= ANGLE_90;
				}
			}
			// Adjust angle for transformations beyond 90 degrees.
			else if (adjustedAngle > ANGLE_90)
			{
				// Apply 90-degree flip inversion transformation.
				Flip = FlippingEnum::FlipInvertX;
				adjustedAngle -= ANGLE_90;
			}

			// Calculate skew factors based on the adjusted angle.
#if defined(EGFX_PLATFORM_BIG)
			const pixel_t skewX = Fraction::Scale((fraction16_t)-Tangent16(adjustedAngle / 2), (pixel_t)(Width));
			const pixel_t skewY = Fraction::Scale(Sine16(adjustedAngle), pixel_t(Height));
#else
			const pixel_t skewX = Fraction::Scale((fraction8_t)-Tangent8(adjustedAngle / 2), (pixel_t)(Width));
			const pixel_t skewY = Fraction::Scale(Sine8(adjustedAngle), pixel_t(Height));
#endif
			SkewHorizontal.SetSkewX(skewX);
			SkewVertical.SetSkewY(skewY);
		}

		const angle_t GetRotation() const
		{
			// Return the current rotation angle.
			return Angle;
		}

		virtual const bool Transform(pixel_t& x, pixel_t& y)
		{
			if (!BaseTransform::Transform(x, y))
			{
				return false;
			}

			// Apply the appropriate flip transformation based on the set rotation.
			switch (Flip)
			{
			case FlippingEnum::FlipInvertX:
				FlipInvertXTransform<Height>::TransformStatic(x, y);
				break;
			case FlippingEnum::FlipInvertY:
				FlipInvertYTransform<Width>::TransformStatic(x, y);
				break;
			case FlippingEnum::InvertX:
				InvertXTransform<Width>::TransformStatic(x, y);
				break;
			case FlippingEnum::InvertY:
				InvertYTransform<Height>::TransformStatic(x, y);
				break;
			case FlippingEnum::Invert:
				InvertTransform<Width, Height>::TransformStatic(x, y);
				break;
			case FlippingEnum::NoFlip:
			default:
				break;
			}

			// Apply horizontal and vertical skew transformations.
			if (SkewHorizontal.Transform(x, y)
				&& SkewVertical.Transform(x, y)
				&& SkewHorizontal.Transform(x, y))
			{
				return true;
			}

			return false;
		}
	};
}
#endif