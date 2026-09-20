#ifndef _INTEGERGLASS_MODULES_LOGO_SHADER_h
#define _INTEGERGLASS_MODULES_LOGO_SHADER_h

#include "Constant.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Logo
		{
			namespace Shader
			{
				namespace Dither
				{
					/// <summary>
					/// Simple statefull RNG-based ditherer, controlled via chance (0-255).
					/// </summary>
					class RngDitherer
					{
					private:
						uint16_t Rng = 67;

					public:
						RngDitherer() = default;
						~RngDitherer() = default;

						bool Dither(const uint8_t chance)
						{
							Rng ^= Rng << 7;
							Rng ^= Rng >> 9;
							Rng ^= Rng << 8;

							return chance == 0 ? false : chance >= static_cast<uint8_t>(Rng);
						}
					};
				}

				namespace Color
				{
					template<typename dimension_t>
					class ScreenDitherAlpha : public Framework::Shader::Color::BaseShader<dimension_t>
					{
					private:
						Dither::RngDitherer Ditherer{};
						uint8_t Alpha = UINT8_MAX;

					public:
						ScreenDitherAlpha() = default;
						~ScreenDitherAlpha() = default;

						void SetAlpha(const uint8_t alpha)
						{
							Alpha = alpha;
						}

						rgb_color_t Shade(const rgb_color_t color)
						{
							return Ditherer.Dither(Alpha) ? color : RGB_COLOR_BLACK;
						}
					};
				}

				namespace Source
				{
					template<typename dimension_t>
					class MatrixColor
					{
					private:
						rgb_color_t Pallete[Dimensions::FontRowCount]{};
						dimension_t SquareOuterSize = 1;

					public:
						MatrixColor() = default;
						~MatrixColor() = default;

						void SetSquareOuterSize(const dimension_t squareOuterSize)
						{
							SquareOuterSize = MaxValue<dimension_t>(1, squareOuterSize);
						}

						void SetColors(const rgb_color_t color1, const rgb_color_t color2)
						{
							for (dimension_t y = 0; y < Dimensions::FontRowCount; y++)
							{
								const ufraction16_t progress = UFraction16::GetScalar<dimension_t>(y % Dimensions::FontRowCount, Dimensions::FontRowCount - 1);

								Pallete[y] = Rgb::Interpolate(
									progress,
									color1,
									color2);
							}
						}

						rgb_color_t Source(const dimension_t /*x*/, const dimension_t y)
						{
							return Pallete[(y / SquareOuterSize) % Dimensions::FontRowCount];
						}
					};
				}

				namespace Transform
				{
					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height,
						dimension_t CornerRadius,
						typename TransformBase = Framework::Shader::Transform::BaseTransform<dimension_t>
					>
					class RoundedCornerCrop : public Modules::Kit::Shader::Transform::Translate<dimension_t, TransformBase>
					{
					private:
						using Base = Modules::Kit::Shader::Transform::Translate<dimension_t, TransformBase>;
						using power_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;

					private:
						static constexpr dimension_t CornerEffectiveRadius = CornerRadius - 1;
						static constexpr power_t CornerPower = static_cast<power_t>(CornerEffectiveRadius) * CornerEffectiveRadius;

					public:
						RoundedCornerCrop() : Base() {}
						~RoundedCornerCrop() = default;

						bool Transform(dimension_t& x, dimension_t& y)
						{
							if (!Base::Transform(x, y)
								|| (x <= 1) || (x >= Width - 2)
								|| (y <= 1) || (y >= Height - 2)
								)
							{
								return false;
							}

							// Reduce the comparison area by folding the coordinates into a quarter of the area.
							dimension_t xCropped = x;
							dimension_t yCropped = y;
							if (xCropped >= Width / 2)
							{
								xCropped = Width - 1 - xCropped;
							}
							if (yCropped >= Height / 2)
							{
								yCropped = Height - 1 - yCropped;
							}

							// If the point is within the corner radius area, it should be cropped out to create rounded corners.
							if (xCropped < CornerEffectiveRadius && yCropped < CornerEffectiveRadius)
							{
								// Check if the point is within the quarter-circle corner radius.
								const power_t dx = CornerEffectiveRadius - xCropped + 1;
								const power_t dy = CornerEffectiveRadius - yCropped + 1;
								if ((dx * dx) + (dy * dy) >= CornerPower)
								{
									return false;
								}
							}

							return true;
						}
					};

					template<typename dimension_t,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>
					>
					class AlphaDither : public Base
					{
					private:
						Dither::RngDitherer Ditherer{};

						uint8_t Alpha = UINT8_MAX;

					public:
						AlphaDither() : Base() {}
						~AlphaDither() {}

						void SetAlpha(const uint8_t alpha)
						{
							Alpha = alpha;
						}

						bool Transform(dimension_t& x, dimension_t& y)
						{
							if (!Ditherer.Dither(Alpha) || !Base::Transform(x, y))
							{
								return false;
							}

							return true;
						}
					};

					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height>
					using ReflectionMonochrome = Modules::Kit::Shader::Transform::Translate<dimension_t,
						RoundedCornerCrop<dimension_t, Width, Height, Fraction(Dimensions::CornerRadiusRatio, Width),
						Modules::Kit::Shader::Transform::CheckerboardMask<dimension_t, false, AlphaDither<dimension_t>>
						>
					>;

					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height>
					using ReflectionColor = Modules::Kit::Shader::Transform::Translate<dimension_t,
						RoundedCornerCrop<dimension_t, Width, Height, Fraction(Dimensions::CornerRadiusRatio, Width)>
					>;

					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height,
						bool Monochrome>
					using Reflection = typename TypeTraits::TypeConditional::conditional_type<
						ReflectionMonochrome<dimension_t, Width, Height>,
						ReflectionColor<dimension_t, Width, Height>,
						Monochrome
					>::type;
				}

				namespace Pixel
				{
					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height>
					using ReflectionMonochrome = Framework::Shader::Pixel::TemplateShader<
						dimension_t,
						Framework::Shader::Source::StaticColor<dimension_t, Colors::Foreground>,
						Framework::Shader::Color::NoShader<dimension_t>,
						Transform::Reflection<dimension_t, Width, Height, true>,
						Framework::Shader::Pixel::BlendModeEnum::Replace
					>;

					template<typename dimension_t>
					using GlassTextMonochrome = Framework::Shader::Pixel::TemplateShader<
						dimension_t,
						Framework::Shader::Source::StaticColor<dimension_t, Colors::Foreground>,
						Framework::Shader::Color::NoShader<dimension_t>,
						Modules::Kit::Shader::Transform::Translate<dimension_t>
					>;

					template<typename dimension_t>
					using GlassTextColor = Framework::Shader::Pixel::TemplateShader<
						dimension_t,
						Source::MatrixColor<dimension_t>,
						Framework::Shader::Color::NoShader<dimension_t>,
						Modules::Kit::Shader::Transform::Translate<dimension_t>
					>;

					template<typename dimension_t, bool Monochrome>
					using GlassText = typename TypeTraits::TypeConditional::conditional_type<
						GlassTextMonochrome<dimension_t>,
						GlassTextColor<dimension_t>,
						Monochrome
					>::type;

					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height>
					using ReflectionColor = Framework::Shader::Pixel::TemplateShader<
						dimension_t,
						Framework::Shader::Source::SingleColor<dimension_t, Colors::Glass>,
						Framework::Shader::Color::NoShader<dimension_t>,
						Transform::Reflection<dimension_t, Width, Height, false>,
						Framework::Shader::Pixel::BlendModeEnum::Add
					>;

					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height,
						bool Monochrome>
					using Reflection = typename TypeTraits::TypeConditional::conditional_type<
						ReflectionMonochrome<dimension_t, Width, Height>,
						ReflectionColor<dimension_t, Width, Height>,
						Monochrome
					>::type;

					template<typename dimension_t>
					class GlassMonochrome : public Framework::Shader::Pixel::TemplateShader<
						dimension_t,
						Framework::Shader::Source::StaticColor<dimension_t, Colors::Foreground>,
						Modules::Kit::Shader::Color::NoShader<dimension_t>,
						Modules::Kit::Shader::Transform::Translate<dimension_t, Transform::AlphaDither<dimension_t>>
					>
					{
					private:
						using Base = Framework::Shader::Pixel::TemplateShader<
							dimension_t,
							Framework::Shader::Source::StaticColor<dimension_t, Colors::Foreground>,
							Modules::Kit::Shader::Color::NoShader<dimension_t>,
							Modules::Kit::Shader::Transform::Translate<dimension_t, Transform::AlphaDither<dimension_t>>
						>;

					public:
						GlassMonochrome(const dimension_t left, const dimension_t top,
							const dimension_t right, const dimension_t bottom)
							: Base(left, top, right, bottom) {}
						~GlassMonochrome() = default;

						void SetAlpha(const uint8_t alpha)
						{
							Base::TransformShader.SetAlpha(alpha);
						}
					};

					template<typename dimension_t>
					class GlassColor : public Framework::Shader::Pixel::TemplateShader<
						dimension_t,
						Framework::Shader::Source::SingleColor<dimension_t, Colors::Glass>,
						Modules::Kit::Shader::Color::NoShader<dimension_t>,
						Modules::Kit::Shader::Transform::Translate<dimension_t>
					>
					{
					private:
						using Base = Framework::Shader::Pixel::TemplateShader<
							dimension_t,
							Framework::Shader::Source::SingleColor<dimension_t, Colors::Glass>,
							Modules::Kit::Shader::Color::NoShader<dimension_t>,
							Modules::Kit::Shader::Transform::Translate<dimension_t>
						>;

					public:
						using Base::ColorSource;

					public:
						GlassColor(const dimension_t left, const dimension_t top,
							const dimension_t right, const dimension_t bottom)
							: Base(left, top, right, bottom) {}
						~GlassColor() = default;

						void SetAlpha(const uint8_t alpha)
						{
							ColorSource.Color = Rgb::Color(
								(static_cast<uint16_t>(Rgb::R(Colors::Glass)) * alpha) >> 8,
								(static_cast<uint16_t>(Rgb::G(Colors::Glass)) * alpha) >> 8,
								(static_cast<uint16_t>(Rgb::B(Colors::Glass)) * alpha) >> 8);
						}
					};

					template<typename dimension_t,
						bool Monochrome>
					using Glass = typename TypeTraits::TypeConditional::conditional_type<
						GlassMonochrome<dimension_t>,
						GlassColor<dimension_t>,
						Monochrome
					>::type;

					template<typename dimension_t>
					class LabelMonochrome : public Framework::Shader::Pixel::TemplateShader<dimension_t,
						Framework::Shader::Source::StaticColor<dimension_t, Colors::Foreground>,
						Framework::Shader::Color::NoShader<dimension_t>,
						Modules::Kit::Shader::Transform::Translate<dimension_t, Transform::AlphaDither<dimension_t>>
					>
					{
					private:
						using Base = Framework::Shader::Pixel::TemplateShader<dimension_t,
							Framework::Shader::Source::StaticColor<dimension_t, Colors::Foreground>,
							Framework::Shader::Color::NoShader<dimension_t>,
							Modules::Kit::Shader::Transform::Translate<dimension_t, Transform::AlphaDither<dimension_t>>
						>;

					public:
						LabelMonochrome(const dimension_t left, const dimension_t top,
							const dimension_t right, const dimension_t bottom)
							: Base(left, top, right, bottom) {}
						~LabelMonochrome() = default;

						void SetAlpha(const uint8_t alpha)
						{
							Base::TransformShader.SetAlpha(alpha);
						}
					};

					template<typename dimension_t>
					class LabelColor : public Framework::Shader::Pixel::TemplateShader<dimension_t,
						Framework::Shader::Source::SingleColor<dimension_t, Colors::Foreground>,
						Framework::Shader::Color::NoShader<dimension_t>,
						Modules::Kit::Shader::Transform::Translate<dimension_t, Transform::AlphaDither<dimension_t>>
					>
					{
					private:
						using Base = Framework::Shader::Pixel::TemplateShader<dimension_t,
							Framework::Shader::Source::SingleColor<dimension_t, Colors::Foreground>,
							Framework::Shader::Color::NoShader<dimension_t>,
							Modules::Kit::Shader::Transform::Translate<dimension_t, Transform::AlphaDither<dimension_t>>
						>;

					public:
						LabelColor(const dimension_t left, const dimension_t top,
							const dimension_t right, const dimension_t bottom)
							: Base(left, top, right, bottom) {}
						~LabelColor() = default;

						void SetAlpha(const uint8_t alpha)
						{
							Base::ColorSource.Color = Rgb::Color(
								(static_cast<uint16_t>(Rgb::R(Colors::Foreground)) * alpha) >> 8,
								(static_cast<uint16_t>(Rgb::G(Colors::Foreground)) * alpha) >> 8,
								(static_cast<uint16_t>(Rgb::B(Colors::Foreground)) * alpha) >> 8);
						}
					};

					template<typename dimension_t,
						bool Monochrome>
					using Label = typename TypeTraits::TypeConditional::conditional_type<
						LabelMonochrome<dimension_t>,
						LabelColor<dimension_t>,
						Monochrome
					>::type;
				}

				namespace Primitive
				{
					template<typename dimension_t, bool Monochrome>
					using Glass = Framework::Shader::Geometry::TriangleShader<
						dimension_t,
						Pixel::Glass<dimension_t, Monochrome>
					>;

					template<typename dimension_t, bool Monochrome>
					using Label = Framework::Shader::Geometry::TriangleShader<dimension_t, Pixel::Label<dimension_t, Monochrome>>;


					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height,
						bool Monochrome
					>
					using Reflection = Framework::Shader::Geometry::TriangleShader<dimension_t,
						Pixel::Reflection<dimension_t, Width, Height, Monochrome>
					>;

					template<typename dimension_t, bool Monochrome>
					using GlassText = Framework::Shader::Geometry::RectangleShader<
						dimension_t,
						Pixel::GlassText<dimension_t, Monochrome>>;
				}
			}
		}
	}
}
#endif