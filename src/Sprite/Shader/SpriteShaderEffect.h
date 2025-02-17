// SpriteShaderEffect.h

#ifndef _EGFX_SPRITE_SHADER_EFFECT_h
#define _EGFX_SPRITE_SHADER_EFFECT_h

#include "../../Model/RgbColor.h"

namespace Egfx::SpriteShaderEffect
{
	/// <summary>
	/// Masks the pixels with a rectangle crop.
	/// </summary>
	/// <typeparam name="BaseClass"></typeparam>
	template<typename BaseClass>
	class CropRectangleEffect : public BaseClass
	{
	private:
		pixel_t X = 0;
		pixel_t Y = 0;
		pixel_t CropWidth = 0;
		pixel_t CropHeight = 0;

	public:
		CropRectangleEffect() : BaseClass()
		{
			CropWidth = BaseClass::GetWidth();
			CropHeight = BaseClass::GetHeight();
		}

		void SetRectangleCrop(const pixel_t x, const pixel_t y, const pixel_t width, const pixel_t height)
		{
			X = x;
			Y = y;
			CropWidth = width;
			CropHeight = height;
		}

		virtual const bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
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
		coordinate_t RadiusPow = 0;
		pixel_signed_t OffsetX = 0;
		pixel_signed_t OffsetY = 0;

	public:
		CropCircleEffect() : BaseClass()
		{
		}

		void SetCircleCrop(const pixel_signed_t offsetX, const pixel_signed_t offsetY, const pixel_t radius)
		{
			OffsetX = offsetX;
			OffsetY = offsetY;
			if (radius > 1)
			{
				RadiusPow = ((coordinate_t)radius * radius) - 1;
			}
			else
			{
				RadiusPow = ((coordinate_t)radius * radius);
			}
		}

		virtual const bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
		{
			return IsInsideCircle(x, y) && BaseClass::Get(color, x, y);
		}

	private:
		const bool IsInsideCircle(const pixel_t x, const pixel_t y)
		{
			const coordinate_signed_t xx = x - OffsetX;
			const coordinate_signed_t yy = y - OffsetY;
			const coordinate_t distancePow = (((coordinate_t)xx * xx) + ((coordinate_t)yy * yy));

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

		virtual const bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
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
		rgb_color_t Transparent = 0;

	public:
		TransparentColorEffect() : BaseClass()
		{
		}

		void SetTransparentColor(const rgb_color_t color)
		{
			Transparent = color;
		}

		virtual const bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
		{
			return BaseClass::Get(color, x, y) && color != Transparent;
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

#if !defined(EGFX_PLATFORM_HDR)
	private:
		static constexpr uint8_t ADD_LIMIT_6 = 0x3F;
		static constexpr uint8_t ADD_LIMIT_5 = 0x1F;
#endif

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
#if defined(EGFX_PLATFORM_HDR)
			Shift = shift;
#else
			Shift = shift / 4;
#endif
			if (Shift == INT8_MIN)
			{
				Shift = INT8_MIN + 1;
			}
		}

		virtual const bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
		{
			if (BaseClass::Get(color, x, y))
			{
				if (Shift > 0)
				{
#if defined(EGFX_PLATFORM_HDR)
					color = Rgb::Color888(LimitedAdd<UINT8_MAX>(Rgb::R(color)), LimitedAdd<UINT8_MAX>(Rgb::G(color)), LimitedAdd<UINT8_MAX>(Rgb::B(color)));
#else
					color = Rgb::Color565From565(LimitedAdd<ADD_LIMIT_5>(Rgb::R5(color)), LimitedAdd<ADD_LIMIT_6>(Rgb::G6(color)), LimitedAdd<ADD_LIMIT_5>(Rgb::B5(color)));
#endif
				}
				else if (Shift < 0)
				{
#if defined(EGFX_PLATFORM_HDR)
					color = Rgb::Color888(LimitedSubtract(Rgb::R(color)), LimitedSubtract(Rgb::G(color)), LimitedSubtract(Rgb::B(color)));
#else
					color = Rgb::Color888(LimitedSubtract(Rgb::R5(color)), LimitedSubtract(Rgb::G6(color)), LimitedSubtract(Rgb::B5(color)));
#endif
				}

				return true;
			}

			return false;
		}

	private:
		template<const uint8_t AddLimit>
		const uint8_t LimitedAdd(const uint8_t value) const
		{
			if (value < (AddLimit - Shift))
			{
				return value + Shift;
			}
			else
			{
				return AddLimit;
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

		virtual const bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
		{
			if (BaseClass::Get(color, x, y))
			{
				color = Rgb::Color(Scale(Rgb::R(color)), Scale(Rgb::G(color)), Scale(Rgb::B(color)));

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

		virtual const bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
		{
			if (BaseClass::Get(color, x, y))
			{
				if (Contrast == 0)
				{
					return true;
				}
				else if (Contrast > 0)
				{
					color = Rgb::Color(ConstrastUp(Rgb::R(color), Contrast)
						, ConstrastUp(Rgb::G(color), Contrast)
						, ConstrastUp(Rgb::B(color), Contrast));

					return true;
				}
				else
				{
					color = Rgb::Color(ConstrastDown(Rgb::R(color), -Contrast)
						, ConstrastDown(Rgb::G(color), -Contrast)
						, ConstrastDown(Rgb::B(color), -Contrast));

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