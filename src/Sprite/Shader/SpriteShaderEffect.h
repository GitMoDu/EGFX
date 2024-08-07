// SpriteShaderEffect.h

#ifndef _SPRITE_SHADER_EFFECT_h
#define _SPRITE_SHADER_EFFECT_h

#include "../../Model/RgbColor.h"

namespace SpriteShaderEffect
{
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
		{}

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
	/// </summary>
	/// <typeparam name="BaseClass">The underlying SpriteShader.</typeparam>
	template<typename BaseClass>
	class TransparentColorEffect : public BaseClass
	{
	private:
		RgbColor Transparent{};

	public:
		TransparentColorEffect() : BaseClass()
		{}

		void SetTransparentColor(const uint8_t r, const uint8_t g, const uint8_t b)
		{
			Transparent.r = r;
			Transparent.g = g;
			Transparent.b = b;
		}

		void SetTransparentColor(RgbColor& color)
		{
			Transparent.r = color.r;
			Transparent.g = color.g;
			Transparent.b = color.b;
		}

	protected:
		const bool GetColor(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			return BaseClass::GetColor(color, x, y)
				&& (color.r != Transparent.r || color.g != Transparent.g || color.b != Transparent.b);
		}
	};

	/// <summary>
	/// Applies a brightness lighten/darken effect to the sprite.
	/// I.e. Shifts RGB values up/down.
	/// </summary>
	/// <typeparam name="BaseClass">The underlying SpriteShader.</typeparam>
	template<typename BaseClass>
	class BrightnessEffect : public BaseClass
	{
	private:
		int8_t Brightness = 0;

	public:
		BrightnessEffect() : BaseClass()
		{}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="brightness">Absolute shift in RGB space.</param>
		void SetBrightness(const int8_t brightness)
		{
			Brightness = brightness;
		}

		virtual const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			if (BaseClass::Get(color, x, y))
			{
				if (Brightness > 0)
				{
					color.r = LimitedAdd(color.r);
					color.g = LimitedAdd(color.g);
					color.b = LimitedAdd(color.b);
				}
				else if (Brightness < 0)
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
		const uint8_t LimitedAdd(const uint8_t value)
		{
			if (value < (UINT8_MAX - Brightness))
			{
				return value + Brightness;
			}
			else
			{
				return UINT8_MAX;
			}
		}

		const uint8_t LimitedSubtract(const uint8_t value)
		{
			if (value > -Brightness)
			{
				return value + Brightness;
			}
			else
			{
				return 0;
			}
		}
	};

	/// <summary>
	/// Applies a constrast adjust effect to the sprite.
	/// </summary>
	/// <typeparam name="BaseClass">The underlying SpriteShader.</typeparam>
	template<typename BaseClass>
	class ConstrastEffect : public BaseClass
	{
	private:
		int8_t Contrast = 0;

	public:
		ConstrastEffect() : BaseClass()
		{}

		/// <summary>
		/// </summary>
		/// <param name="contrast">[INT8_MIN, -1] Scales down to 0x. [1;INT8_MAX] Scales up to 2x.</param>
		void SetContrast(const int8_t contrast)
		{
			Contrast = contrast;
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
					const uint8_t upScale = (uint8_t)Contrast * 2;
					color.r = LimitedScaleUp(color.r, upScale);
					color.g = LimitedScaleUp(color.g, upScale);
					color.b = LimitedScaleUp(color.b, upScale);
				}
				else
				{
					const uint8_t downScale = (uint8_t)(INT8_MIN - Contrast) * 2;
					color.r = LimitedScaleUp(color.r, downScale);
					color.g = LimitedScaleUp(color.g, downScale);
					color.b = LimitedScaleUp(color.b, downScale);
				}
			}

			return false;
		}

	private:
		static const uint8_t LimitedScaleUp(const uint8_t value, const uint8_t scale)
		{
			if (value == 0)
			{
				return value;
			}
			else
			{
				const uint16_t scaled = value + (((uint16_t)value * scale) / UINT8_MAX);

				if (scaled < (UINT8_MAX - value))
				{
					return scaled + value;
				}
				else
				{
					return UINT8_MAX;
				}
			}
		}

		static const uint8_t LimitedScaleDown(const uint8_t value, const uint8_t scale)
		{
			if (value == INT8_MAX)
			{
				return value;
			}
			else
			{
				return ((uint16_t)value * scale) / UINT8_MAX;
			}
		}
	};
}
#endif