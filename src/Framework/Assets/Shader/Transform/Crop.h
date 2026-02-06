#ifndef _EGFX_FRAMEWORK_ASSETS_SHADER_TRANSFORM_CROP_h
#define _EGFX_FRAMEWORK_ASSETS_SHADER_TRANSFORM_CROP_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace Shader
			{
				namespace Transform
				{
					/// <summary>
					/// Rectangle crop transform.
					/// Returns false when the pixel is outside the crop rectangle.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Base">Optional base transform to chain.</typeparam>
					template<typename dimension_t,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>
					>
					class CropRectangle : public Base
					{
					private:
						dimension_t X = 0;
						dimension_t Y = 0;
						dimension_t CropWidth = 0;
						dimension_t CropHeight = 0;

					public:
						CropRectangle() : Base() {}
						~CropRectangle() = default;

						/// <summary>
						/// Set crop rectangle in local coordinates.
						/// </summary>
						void SetRectangleCrop(const dimension_t x, const dimension_t y, const dimension_t width, const dimension_t height)
						{
							X = x;
							Y = y;
							CropWidth = width;
							CropHeight = height;
						}

						bool Transform(dimension_t& x, dimension_t& y)
						{
							if (!Base::Transform(x, y))
							{
								return false;
							}

							return x >= X && x < (X + CropWidth)
								&& y >= Y && y < (Y + CropHeight);
						}
					};

					/// <summary>
					/// Circle crop transform.
					/// Returns false when the pixel is outside the crop circle.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Base">Optional base transform to chain.</typeparam>
					template<typename dimension_t,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>
					>
					class CropCircle : public Base
					{
					private:
						pixel_index_t RadiusPow = 0;
						int16_t OffsetX = 0;
						int16_t OffsetY = 0;

					public:
						CropCircle() : Base() {}
						~CropCircle() = default;

						/// <summary>
						/// Set crop circle center offset and radius in pixels.
						/// </summary>
						void SetCircleCrop(const int16_t offsetX, const int16_t offsetY, const pixel_t radius)
						{
							OffsetX = offsetX;
							OffsetY = offsetY;

							// Matches SpriteShaderEffect::CropCircleEffect seam tweak for radius > 1.
							if (radius > 1)
							{
								RadiusPow = (static_cast<pixel_index_t>(radius) * radius) - 1;
							}
							else
							{
								RadiusPow = (static_cast<pixel_index_t>(radius) * radius);
							}
						}

						bool Transform(dimension_t& x, dimension_t& y)
						{
							if (!Base::Transform(x, y))
							{
								return false;
							}

							return IsInsideCircle(x, y);
						}

					private:
						bool IsInsideCircle(const dimension_t x, const dimension_t y) const
						{
							const int16_t xx = static_cast<int16_t>(x) - OffsetX;
							const int16_t yy = static_cast<int16_t>(y) - OffsetY;

							const pixel_index_t distancePow =
								(static_cast<pixel_index_t>(xx) * xx)
								+ (static_cast<pixel_index_t>(yy) * yy);

							return distancePow <= RadiusPow;
						}
					};
				}
			}
		}
	}
}
#endif