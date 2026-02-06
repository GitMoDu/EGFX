#ifndef _EGFX_MODULES_EGFX_LOGO_SHADER_SOURCE_h
#define _EGFX_MODULES_EGFX_LOGO_SHADER_SOURCE_h

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
				namespace Source
				{
					using dimension_t = Dimensions::dimension_t;

					/// <summary>
					/// Scanline pattern with alpha control.
					/// </summary>
					/// <typeparam name="FontHeight"></typeparam>
					/// <typeparam name="Monochrome"></typeparam>
					template<dimension_t FontHeight, bool Monochrome>
					class RetroLinesColorAlpha
					{
					private:
						static constexpr dimension_t LineHeight = (FontHeight / Dimensions::LineCount);

						Dither::RngDitherer Ditherer{};

					public:
						rgb_color_t Color1 = Colors::Color1;
						rgb_color_t Color2 = Colors::Color2;
						uint8_t Alpha = 255;

					public:
						RetroLinesColorAlpha() = default;
						~RetroLinesColorAlpha() = default;

						rgb_color_t Source(const dimension_t /*x*/, const dimension_t y)
						{
							if (Monochrome)
							{
								return Ditherer.Dither(Alpha) ? Colors::Foreground : Colors::Background;
							}
							else
							{
								if (Ditherer.Dither(Alpha))
								{
									const dimension_t indexY = (y / LineHeight) % Dimensions::LineCount;

									const ufraction16_t progress = UFraction16::GetScalar<dimension_t>(indexY, Dimensions::LineCount);

									const rgb_color_t interpolatedColor = Rgb::InterpolateLinear(
										progress,
										Color1,
										Color2);

									return Rgb::Color(
										(static_cast<uint16_t>(Rgb::R(interpolatedColor)) * Alpha) >> 8,
										(static_cast<uint16_t>(Rgb::G(interpolatedColor)) * Alpha) >> 8,
										(static_cast<uint16_t>(Rgb::B(interpolatedColor)) * Alpha) >> 8);
								}
								else
								{
									return Colors::Background;
								}
							}
						}
					};

					/// <summary>
					/// Dot matrix animated pattern with alpha control.
					/// </summary>
					template<dimension_t FontWidth, dimension_t FontHeight, bool Monochrome>
					class DotMatrixRgbAlpha
					{
					private:
						static constexpr dimension_t SquareSize = (FontWidth / Dimensions::ColumnCount);

						Dither::RngDitherer Ditherer{};

					public:
						uint16_t FrameCounter = 0;
						uint32_t FrameTime = 0;
						uint8_t Alpha = 255;

					public:
						DotMatrixRgbAlpha() = default;
						~DotMatrixRgbAlpha() = default;

						rgb_color_t Source(const dimension_t x, const dimension_t y)
						{
							const uint8_t indexX = (x / SquareSize) % 3;
							const uint8_t indexY = (y / SquareSize) % 3;

							if (Monochrome)
							{
								return Ditherer.Dither(Alpha) ? GetMonochrome(indexX, indexY) : Colors::Background;
							}
							else
							{
								if (Ditherer.Dither(Alpha))
								{
									const rgb_color_t baseColor = GetColor(indexX, indexY);

									return Rgb::Color(
										(static_cast<uint16_t>(Rgb::R(baseColor)) * Alpha) >> 8,
										(static_cast<uint16_t>(Rgb::G(baseColor)) * Alpha) >> 8,
										(static_cast<uint16_t>(Rgb::B(baseColor)) * Alpha) >> 8);
								}
								else
								{
									return Colors::Background;

								}
							}
						}

					private:
						rgb_color_t GetMonochrome(const uint8_t x, const uint8_t y) const
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

						rgb_color_t GetColor(const uint8_t x, const uint8_t y) const
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
					};
				}
			}
		}
	}
}
#endif