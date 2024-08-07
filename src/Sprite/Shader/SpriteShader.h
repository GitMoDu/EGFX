// SpriteShader.h

#ifndef _SPRITE_SHADER_h
#define _SPRITE_SHADER_h

#include "../../Model/RgbColor.h"

/// <summary>
/// Template-chainable shaders, for an underlying SpriteType.
/// </summary>
namespace SpriteShader
{
	template<typename SpriteType>
	class AbstractOneColorShader : public SpriteType
	{
	protected:
		RgbColor ShaderColor{ INT8_MAX, INT8_MAX, INT8_MAX };

	public:
		AbstractOneColorShader()
		{}

	public:
		void SetColor(const uint8_t r, const uint8_t g, const uint8_t b)
		{
			ShaderColor.r = r;
			ShaderColor.g = g;
			ShaderColor.b = b;
		}

		void SetColor(const RgbColor& color)
		{
			ShaderColor.r = color.r;
			ShaderColor.g = color.g;
			ShaderColor.b = color.b;
		}
	};

	template<typename SpriteType>
	class AbstractTwoColorShader : public SpriteType
	{
	protected:
		RgbColor ShaderColor1{ INT8_MAX, INT8_MAX, INT8_MAX };
		RgbColor ShaderColor2{ INT8_MAX, INT8_MAX, INT8_MAX };

	public:
		AbstractTwoColorShader() : SpriteType()
		{}

	public:
		void SetColor1(const uint8_t r, const uint8_t g, const uint8_t b)
		{
			ShaderColor1.r = r;
			ShaderColor1.g = g;
			ShaderColor1.b = b;
		}

		void SetColor1(const RgbColor& color)
		{
			ShaderColor1.r = color.r;
			ShaderColor1.g = color.g;
			ShaderColor1.b = color.b;
		}

		void SetColor2(const uint8_t r, const uint8_t g, const uint8_t b)
		{
			ShaderColor2.r = r;
			ShaderColor2.g = g;
			ShaderColor2.b = b;
		}

		void SetColor2(const RgbColor& color)
		{
			ShaderColor2.r = color.r;
			ShaderColor2.g = color.g;
			ShaderColor2.b = color.b;
		}
	};

	template<typename SpriteType>
	class ColorShader : public AbstractOneColorShader<SpriteType>
	{
	private:
		using AbstractOneColorShader<SpriteType>::ShaderColor;

	public:
		ColorShader() : AbstractOneColorShader<SpriteType>()
		{}

		virtual const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			if (SpriteType::Get(color, x, y))
			{
				color.r = ShaderColor.r;
				color.g = ShaderColor.g;
				color.b = ShaderColor.b;

				return true;
			}
			else
			{
				return false;
			}
		}
	};

	/// <summary>
	/// Shades the pixel by color1 or color2
	/// </summary>
	/// <typeparam name="SpriteType"></typeparam>
	template<typename SpriteType>
	class GridShader : public AbstractTwoColorShader<SpriteType>
	{
	private:
		using AbstractTwoColorShader<SpriteType>::ShaderColor1;
		using AbstractTwoColorShader<SpriteType>::ShaderColor2;

	public:
		GridShader() : AbstractTwoColorShader<SpriteType>()
		{}

		const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			if (SpriteType::Get(color, x, y))
			{
				const bool even = (x + y) % 2 == 1;

				if (even)
				{
					color.r = ShaderColor1.r;
					color.g = ShaderColor1.g;
					color.b = ShaderColor1.b;
				}
				else
				{
					color.r = ShaderColor2.r;
					color.g = ShaderColor2.g;
					color.b = ShaderColor2.b;
				}

				return true;
			}
			else
			{
				return false;
			}
		}
	};

	template<typename SpriteType>
	class HorizontalGradientShader : public AbstractTwoColorShader<SpriteType>
	{
	private:
		using AbstractTwoColorShader<SpriteType>::ShaderColor1;
		using AbstractTwoColorShader<SpriteType>::ShaderColor2;

	public:
		HorizontalGradientShader() : AbstractTwoColorShader<SpriteType>()
		{}

		const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			if (SpriteType::Get(color, x, y))
			{
				RgbColorUtil::InterpolateRgb(color, ShaderColor1, ShaderColor2, (((uint16_t)x * UINT8_MAX) / (SpriteType::Width - 1)));

				return true;
			}
			else
			{
				return false;
			}
		}
	};

	template<typename SpriteType>
	class VerticalGradientShader : public AbstractTwoColorShader<SpriteType>
	{
	private:
		using AbstractTwoColorShader<SpriteType>::ShaderColor1;
		using AbstractTwoColorShader<SpriteType>::ShaderColor2;

	public:
		VerticalGradientShader() : AbstractTwoColorShader<SpriteType>()
		{}

		const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			if (SpriteType::Get(color, x, y))
			{
				RgbColorUtil::InterpolateRgb(color, ShaderColor1, ShaderColor2, (((uint16_t)y * UINT8_MAX) / (SpriteType::Height - 1)));

				return true;
			}
			else
			{
				return false;
			}
		}
	};
}
#endif