#ifndef _EGFX_SPRITE_SHADER_h
#define _EGFX_SPRITE_SHADER_h

#include "../../Model/RgbColor.h"

/// <summary>
/// Template-chainable shaders, for an underlying SpriteType.
/// </summary>
namespace Egfx
{
	namespace SpriteShader
	{
		using namespace IntegerSignal::FixedPoint::ScalarFraction;

		template<typename SpriteType>
		class AbstractOneColorShader : public SpriteType
		{
		protected:
			rgb_color_t ShaderColor = Rgb::Color(INT8_MAX, INT8_MAX, INT8_MAX);

		public:
			AbstractOneColorShader() : SpriteType()
			{
			}

		public:
			void SetColor(const rgb_color_t color)
			{
				ShaderColor = color;
			}

			rgb_color_t GetColor()
			{
				return ShaderColor;
			}
		};

		template<typename SpriteType>
		class AbstractTwoColorShader : public SpriteType
		{
		protected:
			rgb_color_t ShaderColor1 = Rgb::Color(INT8_MAX, INT8_MAX, INT8_MAX);
			rgb_color_t ShaderColor2 = Rgb::Color(INT8_MAX, INT8_MAX, INT8_MAX);

		public:
			AbstractTwoColorShader() : SpriteType()
			{
			}

		public:
			void SetColor1(const rgb_color_t color)
			{
				ShaderColor1 = color;
			}

			void SetColor2(const rgb_color_t color)
			{
				ShaderColor2 = color;
			}
		};

		template<typename SpriteType>
		class ColorShader : public AbstractOneColorShader<SpriteType>
		{
		private:
			using AbstractOneColorShader<SpriteType>::ShaderColor;

		public:
			ColorShader() : AbstractOneColorShader<SpriteType>()
			{
			}

			virtual bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
			{
				if (SpriteType::Get(color, x, y))
				{
					color = ShaderColor;

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
			{
			}

			bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
			{
				if (SpriteType::Get(color, x, y))
				{
					const bool even = (x + y) % 2 == 1;

					if (even)
					{
						color = ShaderColor1;
					}
					else
					{
						color = ShaderColor2;
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
			{
			}

			bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
			{
				if (SpriteType::Get(color, x, y))
				{
					const ufraction16_t fraction = UFraction16::GetScalar<pixel_t>(x, SpriteType::Width - 1);

					color = Rgb::Interpolate(fraction, ShaderColor1, ShaderColor2);

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
			{
			}

			bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
			{
				if (SpriteType::Get(color, x, y))
				{
					const ufraction16_t fraction = UFraction16::GetScalar<pixel_t>(y, SpriteType::Height - 1);

					color = Rgb::Interpolate(fraction, ShaderColor1, ShaderColor2);

					return true;
				}
				else
				{
					return false;
				}
			}
		};

		template<typename SpriteType>
		class AxisLimitTwoColorShader : public AbstractTwoColorShader<SpriteType>
		{
		private:
			using AbstractTwoColorShader<SpriteType>::ShaderColor1;
			using AbstractTwoColorShader<SpriteType>::ShaderColor2;

		private:
			pixel_t LimitX = 0;
			pixel_t LimitY = 0;

		public:
			AxisLimitTwoColorShader() : AbstractTwoColorShader<SpriteType>()
			{
			}

			void SetColorSwitchLimits(const pixel_t limitX = SpriteType::Width, const pixel_t limitY = SpriteType::Height)
			{
				LimitX = limitX;
				LimitY = limitY;
			}

			virtual bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
			{
				if (SpriteType::Get(color, x, y))
				{
					if (x >= LimitX
						|| y >= LimitY)
					{
						color = ShaderColor2;
					}
					else
					{
						color = ShaderColor1;
					}

					return true;
				}
				else
				{
					return false;
				}
			}
		};
	}
}
#endif