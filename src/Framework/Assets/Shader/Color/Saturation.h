#ifndef _EGFX_FRAMEWORK_ASSETS_SHADER_COLOR_SATURATION_h
#define _EGFX_FRAMEWORK_ASSETS_SHADER_COLOR_SATURATION_h

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
					/// Saturation shader.
					/// Negative values decrease saturation towards grayscale.
					/// Positive values increase saturation (oversaturate, clamped).
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Base">The base shader type.</typeparam>
					template<typename dimension_t,
						typename Base = BaseShader<dimension_t>
					>
					class Saturation : public Base
					{
					private:
						ufraction16_t Progress = 0;
						int8_t Sign = 0;

					public:
						Saturation() : Base() {}
						~Saturation() {}

						/// <summary>
						/// Set saturation shift. Values lower than 0 desaturate, values higher than 0 oversaturate.
						/// </summary>
						/// <param name="saturation">[FRACTION16_NEGATIVE_1X ; FRACTION16_1X]</param>
						void SetSaturation(const fraction16_t saturation)
						{
							// Mirror Brightness: store direction in Sign and magnitude in unsigned Progress.
							Sign = (saturation >= 0) ? 1 : -1;
							if (Sign >= 0)
							{
								Progress = saturation << 1;
							}
							else
							{
								// Maps [-1x;0) to (UFRACTION16_1X;0] while keeping Progress unsigned.
								Progress = UFRACTION16_1X - (-(saturation + 1) << 1);
							}
						}

						rgb_color_t Shade(const rgb_color_t color) const
						{
							// Match Brightness: if no-op, skip Base and just return input.
							if (Progress == 0)
							{
								return color;
							}

							const rgb_color_t baseColor = Base::Shade(color);

							// Luma pivot (BT.601-ish), on 8-bit RGB components:
							// Y ~= 0.299R + 0.587G + 0.114B
							// Approximated as (77*R + 150*G + 29*B) / 256.
							const uint8_t y = static_cast<uint8_t>(
								(static_cast<uint16_t>(77) * Rgb::R(baseColor)
									+ static_cast<uint16_t>(150) * Rgb::G(baseColor)
									+ static_cast<uint16_t>(29) * Rgb::B(baseColor)) >> 8);

							const rgb_color_t gray = Rgb::Color(y, y, y);

							if (Sign < 0)
							{
								// Down-saturation:
								// Interpolate from the original color towards grayscale.
								return Rgb::InterpolateLinear(Progress, baseColor, gray);
							}
							else
							{
								// Up-saturation:
								// Push channels away from the luma pivot.
								//
								// Let delta = (c - y).
								// c' = c + delta * Progress
								//    = y + delta * (1 + Progress)
								//
								// Progress is in [0..UFRACTION16_1X] due to SetSaturation mapping.
								return Rgb::Color(
									OversaturateChannel(y, Rgb::R(baseColor)),
									OversaturateChannel(y, Rgb::G(baseColor)),
									OversaturateChannel(y, Rgb::B(baseColor)));
							}
						}

					private:
						/// <summary>
						/// Scales a single channel further away from the luma pivot by Progress and clamps to [0;255].
						/// </summary>
						uint8_t OversaturateChannel(const uint8_t y, const uint8_t c) const
						{
							const int16_t delta = static_cast<int16_t>(c) - static_cast<int16_t>(y);
							const int16_t deltaScaled = Fraction<int16_t>(Progress, delta);
							const int16_t out = static_cast<int16_t>(c) + deltaScaled;

							if (out < 0)
							{
								return 0;
							}
							else if (out > UINT8_MAX)
							{
								return UINT8_MAX;
							}
							else
							{
								return static_cast<uint8_t>(out);
							}
						}
					};
				}
			}
		}
	}
}
#endif