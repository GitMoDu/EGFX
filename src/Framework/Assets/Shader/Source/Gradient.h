#ifndef _EGFX_FRAMEWORK_ASSETS_SHADER_SOURCE_GRADIENT_h
#define _EGFX_FRAMEWORK_ASSETS_SHADER_SOURCE_GRADIENT_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace Shader
			{
				namespace Source
				{
					/// <summary>
					/// Horizontal gradient color source (left->right) between ColorLeft and ColorRight.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Gradient width in pixels.</typeparam>
					template<typename dimension_t,
						dimension_t Width>
					class GradientX
					{
					public:
						rgb_color_t ColorLeft = RGB_COLOR_WHITE;
						rgb_color_t ColorRight = RGB_COLOR_BLACK;

					public:
						GradientX() = default;
						~GradientX() = default;

						rgb_color_t Source(const dimension_t x, const dimension_t /*y*/)
						{
							if (Width <= 1)
							{
								return ColorLeft;
							}

							const ufraction16_t fraction = UFraction16::GetScalar<dimension_t>(x, static_cast<dimension_t>(Width - 1));
							return Rgb::Interpolate(fraction, ColorLeft, ColorRight);
						}
					};

					/// <summary>
					/// Vertical gradient color source (top->bottom) between ColorTop and ColorBottom.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Height">Gradient height in pixels.</typeparam>
					template<typename dimension_t,
						dimension_t Height>
					class GradientY
					{
					public:
						rgb_color_t ColorTop = RGB_COLOR_WHITE;
						rgb_color_t ColorBottom = RGB_COLOR_BLACK;

					public:
						GradientY() = default;
						~GradientY() = default;

						rgb_color_t Source(const dimension_t /*x*/, const dimension_t y)
						{
							if (Height <= 1)
							{
								return ColorTop;
							}

							const ufraction16_t fraction = UFraction16::GetScalar<dimension_t>(y, static_cast<dimension_t>(Height - 1));
							return Rgb::Interpolate(fraction, ColorTop, ColorBottom);
						}
					};

					/// <summary>
					/// Combined 2D gradient source (bilinear blend).
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Gradient width in pixels.</typeparam>
					/// <typeparam name="Height">Gradient height in pixels.</typeparam>
					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height>
					class GradientXY
					{
					public:
						rgb_color_t ColorTopLeft = RGB_COLOR_BLACK;
						rgb_color_t ColorTopRight = RGB_COLOR_WHITE;
						rgb_color_t ColorBottomLeft = RGB_COLOR_WHITE;
						rgb_color_t ColorBottomRight = RGB_COLOR_BLACK;

					public:
						GradientXY() = default;
						~GradientXY() = default;

						rgb_color_t Source(const dimension_t x, const dimension_t y)
						{
							const ufraction16_t fx = (Width <= 1)
								? static_cast<ufraction16_t>(0)
								: UFraction16::GetScalar<dimension_t>(x, static_cast<dimension_t>(Width - 1));

							const ufraction16_t fy = (Height <= 1)
								? static_cast<ufraction16_t>(0)
								: UFraction16::GetScalar<dimension_t>(y, static_cast<dimension_t>(Height - 1));

							const rgb_color_t top = Rgb::Interpolate(fx, ColorTopLeft, ColorTopRight);
							const rgb_color_t bot = Rgb::Interpolate(fx, ColorBottomLeft, ColorBottomRight);
							return Rgb::Interpolate(fy, top, bot);
						}
					};

					/// <summary>
					/// Radial gradient source (center -> edge) between ColorCenter and ColorEdge.
					/// Inspired by `CropCircle` distance math (uses squared distance).
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Source width in pixels.</typeparam>
					/// <typeparam name="Height">Source height in pixels.</typeparam>
					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height>
					class GradientRadial
					{
					private:
						using radius_t = pixel_t;
						using power_t = pixel_index_t;

					public:
						rgb_color_t ColorCenter = RGB_COLOR_WHITE;
						rgb_color_t ColorEdge = RGB_COLOR_BLACK;

					private:
						// Center offset (local coordinate space).
						radius_t OffsetX = Width / 2;
						radius_t OffsetY = Height / 2;

						// Radius^2 in pixels^2 (cached).
						power_t RadiusPow = 0;

						// Cached radius for readback.
						radius_t Radius = 0;

					public:
						GradientRadial()
						{
							SetRadius(static_cast<pixel_t>(MinValue(Width, Height) / 2));
						}

						~GradientRadial() = default;

						/// <summary>
						/// Set the gradient center offset in pixels (local coords).
						/// </summary>
						void SetCenter(const radius_t offsetX, const radius_t offsetY)
						{
							OffsetX = LimitValue<radius_t, -static_cast<radius_t>(Width - 1), static_cast<radius_t>(Width - 1)>(offsetX);
							OffsetY = LimitValue<radius_t, -static_cast<radius_t>(Height - 1), static_cast<radius_t>(Height - 1)>(offsetY);
						}

						/// <summary>
						/// Set the gradient radius in pixels.
						/// Radius 0 collapses to ColorCenter.
						/// </summary>
						void SetRadius(const radius_t radius)
						{
							RadiusPow = static_cast<power_t>(radius) * radius;
							Radius = radius;
						}

						radius_t GetRadius() const
						{
							return Radius;
						}

						rgb_color_t Source(const dimension_t x, const dimension_t y) const
						{
							if (RadiusPow == 0)
							{
								return ColorCenter;
							}

							const radius_t xx = x - OffsetX;
							const radius_t yy = y - OffsetY;

							const power_t distancePow =
								(static_cast<power_t>(xx) * xx) +
								(static_cast<power_t>(yy) * yy);

							// Use distance^2 to avoid sqrt: scalar ~= clamped / RadiusPow.
							const ufraction16_t fraction = UFraction16::GetScalar<power_t>(MinValue(RadiusPow, distancePow), RadiusPow);

							return Rgb::Interpolate(fraction, ColorCenter, ColorEdge);
						}
					};
				}
			}
		}
	}
}
#endif