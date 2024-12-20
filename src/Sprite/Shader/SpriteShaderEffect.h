// SpriteShaderEffect.h

#ifndef _SPRITE_SHADER_EFFECT_h
#define _SPRITE_SHADER_EFFECT_h

#include "../../Model/RgbColor.h"

namespace SpriteShaderEffect
{
	/// <summary>
	/// Masks the pixels with a rectangle crop.
	/// </summary>
	/// <typeparam name="BaseClass"></typeparam>
	template<typename BaseClass>
	class CropRectangleEffect : public BaseClass
	{
	private:
		uint8_t X = 0;
		uint8_t Y = 0;
		uint8_t CropWidth = 0;
		uint8_t CropHeight = 0;

	public:
		CropRectangleEffect() : BaseClass()
		{
			CropWidth = BaseClass::GetWidth();
			CropHeight = BaseClass::GetHeight();
		}

		void SetRectangleCrop(const uint8_t x, const uint8_t y, const uint8_t width, const uint8_t height)
		{
			X = x;
			Y = y;
			CropWidth = width;
			CropHeight = height;
		}

		virtual const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			return x >= X && x < (X + CropWidth) && y >= Y && y < (Y + CropHeight)
				&& BaseClass::Get(color, x, y);
		}
	};

	/// <summary>
	/// Masks the pixels with a circle crop.
	/// </summary>
	/// <typeparam name="BaseClass"></typeparam>
	template<typename BaseClass>
	class CropCircleEffect : public BaseClass
	{
	private:
		uint16_t RadiusPow = 0;
		int8_t OffsetX = 0;
		int8_t OffsetY = 0;

	public:
		CropCircleEffect() : BaseClass()
		{
		}

		void SetCircleCrop(const int8_t offsetX, const int8_t offsetY, const uint8_t radius)
		{
			OffsetX = offsetX;
			OffsetY = offsetY;
			if (radius > 1)
			{
				RadiusPow = ((uint16_t)radius * radius) - 1;
			}
			else
			{
				RadiusPow = ((uint16_t)radius * radius);
			}
		}

		virtual const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			return IsInsideCircle(x, y) && BaseClass::Get(color, x, y);
		}

	private:
		const bool IsInsideCircle(const uint8_t x, const uint8_t y)
		{
			const int16_t xx = x - OffsetX;
			const int16_t yy = y - OffsetY;
			const uint16_t distancePow = (((uint16_t)xx * xx) + ((uint16_t)yy * yy));

			return distancePow <= RadiusPow;
		}
	};

	/// <summary>
	/// Applies a dithered transparency effect to the sprite.
	/// </summary>
	/// <typeparam name="BaseClass">The underlying SpriteShader.</typeparam>
	template<typename BaseClass>
	class TransparentGridEffect : public BaseClass
	{
	private:
		uint8_t Modulus = 0;

	public:
		TransparentGridEffect() : BaseClass()
		{
		}

		/// <summary>
		/// How many pixels sipped for each quadrant.
		/// </summary>
		/// <param name="pixelSkip">[0;min(SpriteWidth, SpriteHeight]</param>
		void SetTransparency(const uint8_t pixelSkip)
		{
			Modulus = pixelSkip + 1;
		}

		virtual const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			return BaseClass::Get(color, x, y)
				&& ((Modulus < 2) ||
					((x % Modulus == 0) && ((y + 1) % Modulus == 0)));
		}
	};

	/// <summary>
	/// Applies a transparency effect to the sprite when the color matches.
	/// Get(color) is rejected on transparent pixels.
	/// </summary>
	/// <typeparam name="BaseClass">The underlying SpriteShader.</typeparam>
	template<typename BaseClass>
	class TransparentColorEffect : public BaseClass
	{
	private:
		RgbColor Transparent{};

	public:
		TransparentColorEffect() : BaseClass()
		{
		}

		void SetTransparentColor(const uint8_t r, const uint8_t g, const uint8_t b)
		{
			Transparent.r = r;
			Transparent.g = g;
			Transparent.b = b;
		}

		void SetTransparentColor(const RgbColor& color)
		{
			Transparent.r = color.r;
			Transparent.g = color.g;
			Transparent.b = color.b;
		}

		virtual const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			return BaseClass::Get(color, x, y)
				&& (color.r != Transparent.r || color.g != Transparent.g || color.b != Transparent.b);
		}
	};

	/// <summary>
	/// Applies a lighten/darken effect to the sprite by shifting RGB values up/down.
	/// </summary>
	/// <typeparam name="BaseClass">The underlying SpriteShader.</typeparam>
	template<typename BaseClass>
	class BrightnessShiftEffect : public BaseClass
	{
	private:
		int8_t Shift = 0;

	public:
		BrightnessShiftEffect() : BaseClass()
		{
		}

		/// <summary>
		/// Brightness shift.
		/// </summary>
		/// <param name="shift">[INT8_MIN+1; INT8_MAX] Absolute shift in RGB space.</param>
		void SetBrightnessShift(const int8_t shift)
		{
			Shift = shift;
			if (Shift == INT8_MIN)
			{
				Shift = INT8_MIN + 1;
			}
		}

		virtual const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			if (BaseClass::Get(color, x, y))
			{
				if (Shift > 0)
				{
					color.r = LimitedAdd(color.r);
					color.g = LimitedAdd(color.g);
					color.b = LimitedAdd(color.b);
				}
				else if (Shift < 0)
				{
					color.r = LimitedSubtract(color.r);
					color.g = LimitedSubtract(color.g);
					color.b = LimitedSubtract(color.b);
				}

				return true;
			}

			return false;
		}

	private:
		const uint8_t LimitedAdd(const uint8_t value) const
		{
			if (value < (UINT8_MAX - Shift))
			{
				return value + Shift;
			}
			else
			{
				return UINT8_MAX;
			}
		}

		const uint8_t LimitedSubtract(const uint8_t value) const
		{
			if (value > -Shift)
			{
				return value + Shift;
			}
			else
			{
				return 0;
			}
		}
	};

	/// <summary>
	/// Applies a RGB boost/dampen effect to the sprite by scaling RGB values up/down.
	/// </summary>
	/// <typeparam name="BaseClass">The underlying SpriteShader.</typeparam>
	template<typename BaseClass>
	class BrightnessEffect : public BaseClass
	{
	private:
		int8_t Brightness = 0;

	public:
		BrightnessEffect() : BaseClass()
		{
		}

		/// <summary>
		/// Brightness shift.
		/// </summary>
		/// <param name="brightness">[INT8_MIN+1; INT8_MAX] Absolute shift in RGB space.</param>
		void SetBrightness(const int8_t brightness)
		{
			Brightness = brightness;
		}

		virtual const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			if (BaseClass::Get(color, x, y))
			{
				color.r = Scale(color.r);
				color.g = Scale(color.g);
				color.b = Scale(color.b);

				return true;
			}

			return false;
		}

	private:
		const uint8_t Scale(const uint8_t value) const
		{
			if (Brightness == INT8_MIN)
			{
				return 0;
			}
			else if (Brightness < 0)
			{
				return ((uint16_t)value * (INT8_MAX + Brightness)) / (INT8_MAX - 0);
			}
			else if (Brightness > 0)
			{
				const uint16_t unlimited = ((uint16_t)value * (INT8_MAX + Brightness)) / INT8_MAX;
				if (unlimited >= UINT8_MAX)
				{
					return UINT8_MAX;
				}
				else
				{
					return unlimited;
				}
			}
			else
			{
				return value;
			}
		}
	};

	/// <summary>
	/// Applies a constrast adjust effect to the sprite.
	/// I.e. Shifts RGB values away/towards center.
	/// </summary>
	/// <typeparam name="BaseClass">The underlying SpriteShader.</typeparam>
	template<typename BaseClass>
	class ConstrastEffect : public BaseClass
	{
	private:
		static constexpr uint8_t MaxContrastRatio = 6;

	private:
		int8_t Contrast = 0;
		uint8_t Center = INT8_MAX;

	public:
		ConstrastEffect() : BaseClass()
		{
		}

		/// <summary>
		/// Set relative Contrast.
		/// </summary>
		/// <param name="contrast">[INT8_MIN+1, -1] Scales down to 0x. [1;INT8_MAX] Scales up to MaxContrastRatio = 4x.</param>
		void SetContrast(const int8_t contrast)
		{
			Contrast = contrast;
			if (Contrast == INT8_MIN)
			{
				Contrast = INT8_MIN + 1;
			}
		}

		void SetContrastCenter(const uint8_t center = INT8_MAX)
		{
			Center = center;
		}

		virtual const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			if (BaseClass::Get(color, x, y))
			{
				if (Contrast == 0)
				{
					return true;
				}
				else if (Contrast > 0)
				{
					color.r = ConstrastUp(color.r, Contrast);
					color.g = ConstrastUp(color.g, Contrast);
					color.b = ConstrastUp(color.b, Contrast);

					return true;
				}
				else
				{
					color.r = ConstrastDown(color.r, -Contrast);
					color.g = ConstrastDown(color.g, -Contrast);
					color.b = ConstrastDown(color.b, -Contrast);

					return true;
				}
			}

			return false;
		}

	private:
		const uint8_t ConstrastUp(const uint8_t value, const int8_t scale)
		{
			const int16_t delta = value - Center;

			if (delta == 0)
			{
				return value;
			}
			else if (delta > 0)
			{
				const uint16_t deltaScaled = ((((uint16_t)delta * MaxContrastRatio) * scale) >> 7);

				if ((deltaScaled < UINT8_MAX)
					&& value < (UINT8_MAX - deltaScaled))
				{
					return value + deltaScaled;
				}
				else
				{
					return UINT8_MAX;
				}
			}
			else
			{
				const uint16_t deltaScaled = (((uint16_t)(-(int16_t)delta) * MaxContrastRatio) * scale) >> 7;

				if (value > deltaScaled)
				{
					return value - deltaScaled;
				}
				else
				{
					return 0;
				}
			}
		}

		const uint8_t ConstrastDown(const uint8_t value, const int8_t scale)
		{
			const int16_t delta = value - Center;

			if (delta == 0)
			{
				return value;
			}
			else if (delta > 0)
			{
				const uint8_t deltaScaled = (((uint16_t)delta) * scale) >> 7;

				return value - deltaScaled;
			}
			else
			{
				const uint8_t deltaScaled = ((uint16_t)(-((int16_t)delta * scale))) >> 7;

				return value + deltaScaled;
			}
		}
	};
}
#endif