// RotateTransform.h

#ifndef _ROTATE_TRANSFORM_h
#define _ROTATE_TRANSFORM_h

#include "SkewTransform.h"
#include "InvertTransform.h"

namespace SpriteTransform
{
	namespace Rotation
	{
		static constexpr uint8_t SinTable[] PROGMEM
		{
			18, 22, 27, 31, 35, 40, 44, 49, 53, 57, 62, 66, 70, 75, 79, 83, 87 ,91 ,96, 100, 104, 108, 112, 116, 120, 124, 128, 131, 135, 139, 143, 146, 150, 153, 157, 160, 164, 167, 171, 174, 177, 180, 183, 186, 190, 192
		};

		static constexpr uint8_t TanTable[] PROGMEM
		{
			18, 22, 27, 31, 36, 41, 45, 50, 54, 59, 64, 68, 73, 78, 83, 88, 93, 98, 103, 108, 114, 119, 124
		};
	}

	/// <summary>
	/// Pixel-retaining rotate done with 3 consecutive skews.
	/// </summary>
	/// <typeparam name="Width"></typeparam>
	/// <typeparam name="Height"></typeparam>
	template<const uint8_t Width,
		const uint8_t Height,
		typename BaseTransform = ITransform>
	class RotateTransform : public BaseTransform
	{
	private:
		static constexpr uint8_t GetDimension()
		{
			return ((Width >= Height) * (Width)) | ((Width < Height) * (Height));
		}

	private:
		using Invert = SpriteTransform::InvertTransform<GetDimension(), GetDimension()>;
		using FlipInvertX = SpriteTransform::FlipInvertXTransform<GetDimension()>;
		using FlipInvertY = SpriteTransform::FlipInvertYTransform<GetDimension()>;

		SpriteTransform::SkewHorizontalTransform<GetDimension()> SkewHorizontal{};
		SpriteTransform::SkewVerticalTransform<GetDimension()> SkewVertical{};

	private:
		int16_t Angle = 0;

	public:
		RotateTransform() : BaseTransform() {}

		void SetRotation(const int16_t angleDegrees)
		{
			Angle = angleDegrees % 360;
		}

		const int16_t GetRotation() const
		{
			return Angle;
		}

		virtual const bool Transform(uint8_t& x, uint8_t& y)
		{
			return Transform(x, y, Angle);
		}

		virtual const bool Transform(uint8_t& x, uint8_t& y, const int16_t angle)
		{
			if (!BaseTransform::Transform(x, y))
			{
				return false;
			}

			switch (angle)
			{
			case 0:
			case 360:
			case -360:
				return true;

			case 90:
			case -90:
				FlipInvertX::TransformStatic(x, y);
				return true;

			case 180:
			case -180:
				Invert::TransformStatic(x, y);
				return true;
			default:
				if (angle > 360
					|| angle < -360)
				{
					return Transform(x, y, angle % 360);
				}
				else if (angle > 180)
				{
					Invert::TransformStatic(x, y);

					return Transform(x, y, angle - 180);
				}
				else if (angle < -180)
				{
					Invert::TransformStatic(x, y);

					return Transform(x, y, angle + 180);
				}
				else if (angle > 90)
				{
					FlipInvertX::TransformStatic(x, y);

					return Transform(x, y, (angle - 90));
				}
				else if (angle < -90)
				{
					FlipInvertY::TransformStatic(x, y);

					return Transform(x, y, (angle + 90));
				}
				else if (angle > 45)
				{
					if (Transform(x, y, angle - 90))
					{
						FlipInvertX::TransformStatic(x, y);
						return true;
					}
				}
				else if (angle < -45)
				{
					if (Transform(x, y, angle + 90))
					{
						FlipInvertY::TransformStatic(x, y);
						return true;
					}
				}
				else  if (angle > 0)
				{
#if defined(ARDUINO_ARCH_AVR)
					const int8_t h = (((int16_t)pgm_read_byte(&Rotation::TanTable[angle / 2]) * Height) / UINT8_MAX);
					const int8_t v = ((int16_t)pgm_read_byte(&Rotation::SinTable[angle]) * Width) / UINT8_MAX;
#else
					const int8_t h = (((int16_t)Rotation::TanTable[angle / 2] * Height) / UINT8_MAX);
					const int8_t v = ((int16_t)Rotation::SinTable[angle] * Width) / UINT8_MAX;
#endif
					SkewHorizontal.SetSkewX(h);
					SkewVertical.SetSkewY(-v);

					return SkewHorizontal.Transform(x, y)
						&& SkewVertical.Transform(x, y)
						&& SkewHorizontal.Transform(x, y);
				}
				else
				{
#if defined(ARDUINO_ARCH_AVR)
					const int8_t h = ((int16_t)pgm_read_byte(&Rotation::TanTable[-(angle / 2)]) * Height) / UINT8_MAX;
					const int8_t v = (((int16_t)pgm_read_byte(&Rotation::SinTable[-angle]) * Width) / UINT8_MAX);
#else
					const int8_t h = ((int16_t)Rotation::TanTable[-(angle / 2)] * Height) / UINT8_MAX;
					const int8_t v = (((int16_t)Rotation::SinTable[-angle] * Width) / UINT8_MAX);
#endif
					SkewHorizontal.SetSkewX(-h);
					SkewVertical.SetSkewY(v);

					return SkewHorizontal.Transform(x, y)
						&& SkewVertical.Transform(x, y)
						&& SkewHorizontal.Transform(x, y);
					break;
				}

				return false;
			}
		}
	};
}
#endif