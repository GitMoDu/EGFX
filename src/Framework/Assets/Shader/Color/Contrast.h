#ifndef _EGFX_FRAMEWORK_ASSETS_SHADER_COLOR_CONTRAST_h
#define _EGFX_FRAMEWORK_ASSETS_SHADER_COLOR_CONTRAST_h

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
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Base">The base shader type.</typeparam>
					template<typename dimension_t,
						typename Base = BaseShader<dimension_t>
					>
					class Contrast : public Base
					{
					private:
						static constexpr uint8_t MaxContrastRatio = 6;

					private:
						int8_t Contrast = 0;
						uint8_t Center = INT8_MAX;

					public:
						Contrast() : Base() {}
						~Contrast() {}

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

						rgb_color_t Shade(const rgb_color_t color) const
						{
							if (Contrast == 0)
							{
								return Base::Shade(color);
							}
							else
							{
								rgb_color_t baseColor = Base::Shade(color);
								if (Contrast > 0)
								{
									return Rgb::Color(ConstrastUp(Rgb::R(baseColor), Contrast)
										, ConstrastUp(Rgb::G(baseColor), Contrast)
										, ConstrastUp(Rgb::B(baseColor), Contrast));
								}
								else
								{
									return Rgb::Color(ConstrastDown(Rgb::R(baseColor), -Contrast)
										, ConstrastDown(Rgb::G(baseColor), -Contrast)
										, ConstrastDown(Rgb::B(baseColor), -Contrast));
								}
							}
						}

					private:
						uint8_t ConstrastUp(const uint8_t value, const int8_t scale) const
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

						uint8_t ConstrastDown(const uint8_t value, const int8_t scale) const
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
			}
		}
	}
}
#endif