#ifndef _EGFX_ROTATE_TRANSFORM_h
#define _EGFX_ROTATE_TRANSFORM_h

#include "SkewTransform.h"
#include "InvertTransform.h"
#include <IntegerTrigonometry8.h>
#include <IntegerTrigonometry16.h>

namespace Egfx
{
	namespace SpriteTransform
	{
		using namespace IntegerSignal::FixedPoint;
		using namespace IntegerSignal::Trigonometry;

		namespace Rotation
		{
			template<bool use16>
			struct fraction_selector { using type = fraction16_t; };
			template<>
			struct fraction_selector<false> { using type = fraction8_t; };

			template<int32_t width, int32_t height>
			static constexpr bool Use16BitFraction()
			{
				return (width > 128) || (height > 128);
			}
		}

		/// <summary>
		/// A fast rotation transformation class for fixed-size images that rotates pixels around the center point using precomputed sine and cosine values.
		/// </summary>
		/// <typeparam name="BaseTransform">The base transformation class to inherit from. Defaults to ITransform and allows chaining multiple transformations.</typeparam>
		/// <typeparam name="Width">The width of the image in pixels. Used to calculate the center point for rotation.</typeparam>
		/// <typeparam name="Height">The height of the image in pixels. Used to calculate the center point for rotation.</typeparam>
		template<const pixel_t Width,
			const pixel_t Height,
			typename BaseTransform = ITransform>
		class RotateTransformFast : public BaseTransform
		{
		private:
			using fraction_t = typename Rotation::fraction_selector<Rotation::Use16BitFraction<Width, Height>()>::type;

		private:
			static constexpr uint8_t BresehamScale = 8;
			static constexpr pixel_t CenterX = static_cast<pixel_t>(Width - 1) / 2;
			static constexpr pixel_t CenterY = static_cast<pixel_t>(Height - 1) / 2;

		private:
			fraction_t RotateCosine{};
			fraction_t RotateSine{};

		public:
			RotateTransformFast() : BaseTransform() {}

			void SetRotation(const angle_t angle)
			{
				if (Rotation::Use16BitFraction<Width, Height>())
				{
					RotateCosine = Cosine16(angle);
					RotateSine = Sine16(angle);
				}
				else
				{
					RotateCosine = Cosine8(angle);
					RotateSine = Sine8(angle);
				}
			}

			virtual bool Transform(pixel_t& x, pixel_t& y) override
			{
				if (!BaseTransform::Transform(x, y))
				{
					return false;
				}

				const int16_t dx = SignedLeftShift(static_cast<int32_t>(x), BresehamScale) - SignedLeftShift(static_cast<int32_t>(CenterX), BresehamScale);
				const int16_t dy = SignedLeftShift(static_cast<int32_t>(y), BresehamScale) - SignedLeftShift(static_cast<int32_t>(CenterY), BresehamScale);

				const int16_t cosDx = Fraction<int16_t>(RotateCosine, dx);
				const int16_t sinDy = Fraction<int16_t>(RotateSine, dy);
				const int16_t sinDx = Fraction<int16_t>(RotateSine, dx);
				const int16_t cosDy = Fraction<int16_t>(RotateCosine, dy);

				x = static_cast<pixel_t>(SignedRightShift(cosDx - sinDy, BresehamScale) + CenterX);
				y = static_cast<pixel_t>(SignedRightShift(sinDx + cosDy, BresehamScale) + CenterY);

				return true;
			}
		};

		/// <summary>
		/// Pixel-accurate rotation transformation class for fixed-size images that rotates pixels around the center point using skew and flip transformations.
		/// </summary>
		/// <typeparam name="Width">The width of the image in pixels. Used to calculate the center point for rotation.</typeparam>
		/// <typeparam name="Height">The height of the image in pixels. Used to calculate the center point for rotation.</typeparam>
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
			SpriteTransform::SkewHorizontalTransform<Height + 1> SkewHorizontal{};
			SpriteTransform::SkewVerticalTransform<Width + 1> SkewVertical{};
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
				case Trigonometry::ANGLE_90:
					// Apply 90-degree rotation transformation.
					SkewHorizontal.SetSkewX(0);
					SkewVertical.SetSkewY(0);
					Flip = FlippingEnum::FlipInvertX;
					return;
				case Trigonometry::ANGLE_180:
					// Apply 180-degree rotation transformation.
					SkewHorizontal.SetSkewX(0);
					SkewVertical.SetSkewY(0);
					Flip = FlippingEnum::Invert;
					return;
				case Trigonometry::ANGLE_270:
					// Apply 270-degree rotation transformation.
					SkewHorizontal.SetSkewX(0);
					SkewVertical.SetSkewY(0);
					Flip = FlippingEnum::FlipInvertY;
					return;
				}

				// Adjust angle for transformations beyond 180 degrees.
				if (adjustedAngle > Trigonometry::ANGLE_180)
				{
					// Apply 180-degree inversion transformation.
					Flip = FlippingEnum::Invert;
					adjustedAngle -= Trigonometry::ANGLE_180;

					// Adjust angle for transformations beyond 270 degrees.
					if (adjustedAngle > Trigonometry::ANGLE_90)
					{
						// Apply 90-degree flip inversion transformation.
						Flip = FlippingEnum::FlipInvertY;
						adjustedAngle -= Trigonometry::ANGLE_90;
					}
				}
				// Adjust angle for transformations beyond 90 degrees.
				else if (adjustedAngle > Trigonometry::ANGLE_90)
				{
					// Apply 90-degree flip inversion transformation.
					Flip = FlippingEnum::FlipInvertX;
					adjustedAngle -= Trigonometry::ANGLE_90;
				}

				// Calculate skew factors based on the adjusted angle.
				pixel_t skewX, skewY;
				if (Rotation::Use16BitFraction<Width, Height>()) // Compile-time branch for higher precision.
				{
					skewX = Fraction<pixel_t>(static_cast<fraction16_t>(-Tangent16(adjustedAngle >> 1)), Width);
					skewY = Fraction<pixel_t>(Sine16(adjustedAngle), Height);
				}
				else
				{
					skewX = Fraction<pixel_t>(static_cast<fraction8_t>(-Tangent8(adjustedAngle >> 1)), Width);
					skewY = Fraction<pixel_t>(Sine8(adjustedAngle), Height);
				}

				SkewHorizontal.SetSkewX(skewX);
				SkewVertical.SetSkewY(skewY);
			}

			angle_t GetRotation() const
			{
				// Return the current rotation angle.
				return Angle;
			}

			virtual bool Transform(pixel_t& x, pixel_t& y)
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
}
#endif