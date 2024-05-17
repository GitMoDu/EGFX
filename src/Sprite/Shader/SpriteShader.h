// SpriteShader.h

#ifndef _SPRITE_SHADER_h
#define _SPRITE_SHADER_h

#include "../../Model/RgbColor.h"

namespace SpriteShader
{
	template<typename SpriteType>
	class AbstractOneColorShader : public SpriteType
	{
	protected:
		RgbColor ShaderColor;

	public:
		AbstractOneColorShader()
			: ShaderColor(INT8_MAX, INT8_MAX, INT8_MAX)
		{}

	public:
		void SetColor(const uint8_t r, const uint8_t g, const uint8_t b)
		{
			ShaderColor.r = r;
			ShaderColor.g = g;
			ShaderColor.b = b;
		}

		void SetColor(RgbColor& color)
		{
			ShaderColor.r = color.r;
			ShaderColor.g = color.g;
			ShaderColor.b = color.b;
		}
	};

	template<typename SpriteType>
	class AbstractTwoColorShader : public AbstractOneColorShader<SpriteType>
	{
	protected:
		RgbColor ShaderColor2;

	public:
		AbstractTwoColorShader() : AbstractOneColorShader<SpriteType>()
			, ShaderColor2(0, 0, 0)
		{}

	public:
		void SetColor2(const uint8_t r, const uint8_t g, const uint8_t b)
		{
			ShaderColor2.r = r;
			ShaderColor2.g = g;
			ShaderColor2.b = b;
		}

		void SetColor2(RgbColor& color)
		{
			ShaderColor2.r = color.r;
			ShaderColor2.g = color.g;
			ShaderColor2.b = color.b;
		}
	};

	template<typename SpriteType>
	class SingleColorShader : public AbstractOneColorShader<SpriteType>
	{
	private:
		using AbstractOneColorShader<SpriteType>::ShaderColor;

	public:
		SingleColorShader() : AbstractOneColorShader<SpriteType>()
		{}

	protected:
		const bool GetColor(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			color.r = ShaderColor.r;
			color.g = ShaderColor.g;
			color.b = ShaderColor.b;

			return true;
		}
	};

	template<typename SpriteType>
	class GridShader : public AbstractTwoColorShader<SpriteType>
	{
	private:
		using AbstractTwoColorShader<SpriteType>::ShaderColor;
		using AbstractTwoColorShader<SpriteType>::ShaderColor2;

	public:
		GridShader() : AbstractTwoColorShader<SpriteType>()
		{}

	protected:
		const bool GetColor(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			const bool even = (x + y) % 2 == 1;

			if (even)
			{
				color.r = ShaderColor.r;
				color.g = ShaderColor.g;
				color.b = ShaderColor.b;
			}
			else
			{
				color.r = ShaderColor2.r;
				color.g = ShaderColor2.g;
				color.b = ShaderColor2.b;
			}

			return true;
		}
	};


	template<typename SpriteType>
	class HorizontalGradientShader : public AbstractTwoColorShader<SpriteType>
	{
	private:
		using AbstractTwoColorShader<SpriteType>::ShaderColor;
		using AbstractTwoColorShader<SpriteType>::ShaderColor2;

	public:
		HorizontalGradientShader() : AbstractTwoColorShader<SpriteType>()
		{}

	protected:
		const bool GetColor(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			color.r = (((uint16_t)x * ShaderColor2.r) / (SpriteType::Width - 1))
				+ (((uint16_t)(SpriteType::Width - 1 - x) * ShaderColor.r) / (SpriteType::Width - 1));
			color.g = (((uint16_t)x * ShaderColor2.g) / (SpriteType::Width - 1))
				+ (((uint16_t)(SpriteType::Width - 1 - x) * ShaderColor.g) / (SpriteType::Width - 1));
			color.b = (((uint16_t)x * ShaderColor2.b) / (SpriteType::Width - 1))
				+ (((uint16_t)(SpriteType::Width - 1 - x) * ShaderColor.b) / (SpriteType::Width - 1));

			return true;
		}
	};

	template<typename SpriteType>
	class VerticalGradientShader : public AbstractTwoColorShader<SpriteType>
	{
	private:
		using AbstractTwoColorShader<SpriteType>::ShaderColor;
		using AbstractTwoColorShader<SpriteType>::ShaderColor2;

	public:
		VerticalGradientShader() : AbstractTwoColorShader<SpriteType>()
		{}

	protected:
		const bool GetColor(RgbColor& color, const uint8_t x, const uint8_t y)
		{
			color.r = (((uint16_t)y * ShaderColor2.r) / (SpriteType::Height - 1))
				+ (((uint16_t)(SpriteType::Height - 1 - y) * ShaderColor.r) / (SpriteType::Height - 1));
			color.g = (((uint16_t)y * ShaderColor2.g) / (SpriteType::Height - 1))
				+ (((uint16_t)(SpriteType::Height - 1 - y) * ShaderColor.g) / (SpriteType::Height - 1));
			color.b = (((uint16_t)y * ShaderColor2.b) / (SpriteType::Height - 1))
				+ (((uint16_t)(SpriteType::Height - 1 - y) * ShaderColor.b) / (SpriteType::Height - 1));

			return true;
		}
	};
}
#endif