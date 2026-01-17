#ifndef _EGFX_FRAMEWORK_ASSETS_EGFX_LOGO_SHADERS_h
#define _EGFX_FRAMEWORK_ASSETS_EGFX_LOGO_SHADERS_h

#include <EgfxDrawer.h>
#include "CodeFonts.h"
#include "Constants.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace EgfxLogo
			{
				namespace Shaders
				{
					// Simple RNG-based ditherer, controlled via chance (0-255)
					class RngDitherer
					{
					private:
						uint16_t Rng = 64;

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

					template<typename dimension_t, bool Monochrome>
					struct Screen
					{
						RngDitherer Ditherer{};

						uint8_t Alpha = 255;
						rgb_color_t Color = Colors::Foreground;

						rgb_color_t ShadeColor()
						{
							if (Monochrome)
							{
								return Ditherer.Dither(UINT8_MAX - Alpha) ? Colors::Background : Color;
							}
							else
							{
								return Rgb::Color(
									(static_cast<uint16_t>(Rgb::R(Color)) * Alpha) >> 8,
									(static_cast<uint16_t>(Rgb::G(Color)) * Alpha) >> 8,
									(static_cast<uint16_t>(Rgb::B(Color)) * Alpha) >> 8);
							}
						}
					};

					template<typename dimension_t, dimension_t FontHeight, bool Monochrome>
					struct RetroLinesAlphaColor
					{
						static constexpr dimension_t LineHeight = (FontHeight / CodeFonts::LogoFontFamily::Dimensions::LineCount);

						RngDitherer Ditherer{};

						uint8_t Alpha = 255;
						rgb_color_t Color1 = Colors::Color1;
						rgb_color_t Color2 = Colors::Color2;

						rgb_color_t ShadeColor(const dimension_t /*x*/, const dimension_t y)
						{
							if (Ditherer.Dither(UINT8_MAX - Alpha))
							{
								return Colors::Background;
							}
							else
							{
								const dimension_t indexY = (y / LineHeight) % CodeFonts::LogoFontFamily::Dimensions::LineCount;

								const ufraction16_t progress = UFraction16::GetScalar<dimension_t>(
									indexY,
									CodeFonts::LogoFontFamily::Dimensions::LineCount);

								const rgb_color_t interpolatedColor = Rgb::InterpolateLinear(
									progress,
									Color1,
									Color2);

								return Rgb::Color(
									(static_cast<uint16_t>(Rgb::R(interpolatedColor)) * Alpha) >> 8,
									(static_cast<uint16_t>(Rgb::G(interpolatedColor)) * Alpha) >> 8,
									(static_cast<uint16_t>(Rgb::B(interpolatedColor)) * Alpha) >> 8);
							}
						}
					};

					template<typename dimension_t, dimension_t FontWidth, dimension_t FontHeight, bool Monochrome>
					struct DotMatrixDitherRgb
					{
						static constexpr dimension_t SquareSize = (FontWidth / CodeFonts::LogoFontFamily::Dimensions::ColumnCount);

						RngDitherer Ditherer{};

						uint8_t Alpha = 255;
						uint16_t FrameCounter = 0;
						uint32_t FrameTime = 0;

						rgb_color_t ShadeColor(const dimension_t x, const dimension_t y)
						{
							const uint8_t indexX = (x / SquareSize) % 3;
							const uint8_t indexY = (y / SquareSize) % 3;

							if (Monochrome)
							{
								return Ditherer.Dither(UINT8_MAX - Alpha) ? Colors::Background : GetColor(indexX, indexY);
							}
							else
							{
								if (Ditherer.Dither(UINT8_MAX - Alpha))
								{
									return Colors::Background;
								}
								else
								{
									const rgb_color_t baseColor = GetColor(indexX, indexY);

									return Rgb::Color(
										(static_cast<uint16_t>(Rgb::R(baseColor)) * Alpha) >> 8,
										(static_cast<uint16_t>(Rgb::G(baseColor)) * Alpha) >> 8,
										(static_cast<uint16_t>(Rgb::B(baseColor)) * Alpha) >> 8);
								}
							}
						}

						rgb_color_t GetColor(const uint8_t x, const uint8_t y) const
						{
							if (Monochrome)
							{
								if ((FrameCounter & 0b1) && (((x + y) + (FrameCounter >> 1)) % 3) != 0)
								{
									return Colors::Background;
								}
								else
								{
									return Colors::Foreground;
								}
							}
							else
							{
								const uint16_t frameCounter = FrameTime / 35000;
								const uint8_t index = ((x + y * 2) + (frameCounter >> 1)) % 3;

								switch (index)
								{
								case 0:
									return Colors::Color1;
								case 1:
									return Colors::Color2;
								case 2:
								default:
									return Colors::Color3;
								}
							}
						}
					};
				}
			}
		}
	}
}
#endif