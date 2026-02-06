#ifndef _EGFX_MODULES_EGFX_LOGO_SHADER_COLOR_h
#define _EGFX_MODULES_EGFX_LOGO_SHADER_COLOR_h

#include "../Constant.h"
#include "Dither.h"

namespace Egfx
{
	namespace Modules
	{
		namespace EgfxLogo
		{
			namespace Shader
			{
				namespace Color
				{
					using dimension_t = Dimensions::dimension_t;

					/// <summary>
					/// Custom screen shader for monochrome rendering, applies a dither to the output color based on the alpha value.
					/// </summary>
					class ScreenDitherAlpha : public Framework::Shader::Color::BaseShader<dimension_t>
					{
					private:
						using Base = Framework::Shader::Color::BaseShader<dimension_t>;

					private:
						Dither::RngDitherer Ditherer{};

					public:
						uint8_t Alpha = UINT8_MAX;

					public:
						ScreenDitherAlpha() : Base() {}
						~ScreenDitherAlpha() {}

						rgb_color_t Shade(const rgb_color_t /*color*/)
						{
							if (Ditherer.Dither(Fraction(Dimensions::ScreenAlphaScale, Alpha)))
							{
								return Colors::Foreground;
							}
							else
							{
								return Colors::Background;
							}
						}
					};

					/// <summary>
					/// Custom screen color shader, applies an alpha and dither to the input color.
					/// </summary>
					class ScreenColorAlpha
					{
					private:
						Dither::RngDitherer Ditherer{};

					public:
						uint8_t Alpha = 255;

					public:
						ScreenColorAlpha() = default;
						~ScreenColorAlpha() = default;

						rgb_color_t Shade(const rgb_color_t color)
						{
							if (Ditherer.Dither(Fraction(Dimensions::ScreenAlphaScale, Alpha)))
							{
								return Rgb::Color(
									(static_cast<uint16_t>(Rgb::R(color)) * Alpha) >> 8,
									(static_cast<uint16_t>(Rgb::G(color)) * Alpha) >> 8,
									(static_cast<uint16_t>(Rgb::B(color)) * Alpha) >> 8);
							}
							else
							{
								return Colors::Background;
							}
						}
					};

					/// <summary>
					/// Forwards either the ScreenDitherAlpha shader or the ScreenColorAlpha shader based on the Monochrome template parameter.
					/// </summary>
					/// <typeparam name="Base">The base shader type to use. Defaults to Framework::Shader::Color::BaseShader<dimension_t>.</typeparam>
					/// <typeparam name="Monochrome">A boolean flag indicating whether to use monochrome rendering.</typeparam>
					template<bool Monochrome,
						typename Base = Framework::Shader::Color::BaseShader<dimension_t>
					>
					using ScreenAlpha = typename TypeTraits::TypeConditional::conditional_type<
						ScreenDitherAlpha,
						ScreenColorAlpha,
						Monochrome>::type;
				}
			}
		}
	}
}
#endif