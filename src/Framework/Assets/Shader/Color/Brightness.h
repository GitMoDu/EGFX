#ifndef _EGFX_FRAMEWORK_ASSETS_SHADER_COLOR_BRIGHTNESS_h
#define _EGFX_FRAMEWORK_ASSETS_SHADER_COLOR_BRIGHTNESS_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace Shader
			{
				namespace Color
				{
					using namespace Framework::Shader::Color;

					/// <summary>
					/// Brightness shader that adjusts the brightness of a color.
					/// Applies a RGB boost/dampen effect by scaling RGB values up/down.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Base">The base shader type.</typeparam>
					template<typename dimension_t,
						typename Base = BaseShader<dimension_t>
					>
					class Brightness : public Base
					{
					private:
						ufraction16_t Progress = 0;
						int8_t Sign = 0;

					public:
						Brightness() : Base() {}
						~Brightness() {}

						/// <summary>
						/// Set brightness shift. Values lower than 0 darken, values higher than 0 lighten.
						/// </summary>
						/// <param name="brightness">[FRACTION16_NEGATIVE_1X ; FRACTION16_1X] Absolute shift in RGB space.</param>
						void SetBrightness(const fraction16_t brightness)
						{
							Sign = (brightness >= 0) ? 1 : -1;
							if (Sign >= 0)
							{
								Progress = brightness << 1;
							}
							else
							{
								Progress = UFRACTION16_1X - (-(brightness + 1) << 1);
							}
						}

						rgb_color_t Shade(const rgb_color_t color) const
						{
							if (Progress != 0)
							{
								const rgb_color_t baseColor = Base::Shade(color);
								if (Sign >= 0)
								{
									return Rgb::InterpolateLinear(Progress, baseColor, RGB_COLOR_WHITE);
								}
								else
								{
									return Rgb::Color(Fraction(Progress, Rgb::R(baseColor)),
										Fraction(Progress, Rgb::G(baseColor)),
										Fraction(Progress, Rgb::B(baseColor)));
								}
							}
							else
							{
								return color;
							}
						}
					};

					/// <summary>
					/// Applies a lighten/darken effect to the color by shifting RGB values up/down.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Base">The base shader type.</typeparam>
					template<typename dimension_t,
						typename Base = BaseShader<dimension_t>
					>
					class LightenDarken : public Base
					{
					private:
						int8_t Shift = 0;

#if !defined(EGFX_PLATFORM_HDR)
					private:
						static constexpr uint8_t ADD_LIMIT_6 = 0x3F;
						static constexpr uint8_t ADD_LIMIT_5 = 0x1F;
#endif

					public:
						LightenDarken() : Base() {}
						~LightenDarken() {}

						/// <summary>
						/// Set lighten/darken shift. Values lower than 0 darken, values higher than 0 lighten.
						/// </summary>
						/// <param name="shift">[INT8_MIN+1; INT8_MAX] Absolute shift in RGB space.</param>
						void SetLightenDarken(const int8_t shift)
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

						rgb_color_t Shade(const rgb_color_t color) const
						{
							const rgb_color_t baseColor = Base::Shade(color);

							if (Shift > 0)
							{
#if defined(EGFX_PLATFORM_HDR)
								return Rgb::Color888(
									LimitedAdd<UINT8_MAX>(Rgb::R(baseColor)),
									LimitedAdd<UINT8_MAX>(Rgb::G(baseColor)),
									LimitedAdd<UINT8_MAX>(Rgb::B(baseColor)));
#else
								return Rgb::Color565From565(
									LimitedAdd<ADD_LIMIT_5>(Rgb::R5(baseColor)),
									LimitedAdd<ADD_LIMIT_6>(Rgb::G6(baseColor)),
									LimitedAdd<ADD_LIMIT_5>(Rgb::B5(baseColor)));
#endif
							}
							else if (Shift < 0)
							{
#if defined(EGFX_PLATFORM_HDR)
								return Rgb::Color888(
									LimitedSubtract(Rgb::R(baseColor)),
									LimitedSubtract(Rgb::G(baseColor)),
									LimitedSubtract(Rgb::B(baseColor)));
#else
								return Rgb::Color888(
									LimitedSubtract(Rgb::R5(baseColor)),
									LimitedSubtract(Rgb::G6(baseColor)),
									LimitedSubtract(Rgb::B5(baseColor)));
#endif
							}
						}

					private:
						template<uint8_t AddLimit>
						uint8_t LimitedAdd(const uint8_t value) const
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

						uint8_t LimitedSubtract(const uint8_t value) const
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

				}
			}
		}
	}
}
#endif