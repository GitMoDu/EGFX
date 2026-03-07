#ifndef _EGFX_FRAMEWORK_ASSETS_SHADER_TRANSFORM_SCALE_DOWN_h
#define _EGFX_FRAMEWORK_ASSETS_SHADER_TRANSFORM_SCALE_DOWN_h

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
					/// X-axis scale-down transform (drop-and-pack).
					/// Drops (Width - TargetWidth) source pixels and compacts the remaining X coordinates.
					/// Transform returns false for source pixels that should be skipped (dropped).
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Source width in pixels (transform-space).</typeparam>
					/// <typeparam name="Height">Source height in pixels (transform-space).</typeparam>
					/// <typeparam name="Base">Base transform type in the chain (defaults to BaseTransform).</typeparam>
					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>
					>
					class ScaleDownX : public Base
					{
					private:
						using Selector = Framework::Shader::Transform::FractionSelector<Width, Height>;
						using ufraction_t = typename Selector::ufraction_t;
						using signed_t = typename TypeTraits::TypeNext::next_int_type<dimension_t>::type;

					private:
						/// <summary>Target width (pixels) produced by the transform.</summary>
						dimension_t TargetWidth = Width;

						/// <summary>Number of source pixels to drop across the width.</summary>
						dimension_t SkipCount = 0;

						/// <summary>Cached scalar: SkipCount / Width (fixed-point).</summary>
						ufraction_t SkipScalar = {}; // SkipCount / Width

					public:
						ScaleDownX() : Base() {}
						~ScaleDownX() = default;

						/// <summary>
						/// Sets the target width (pixels). Clamped to [0..Width].
						/// Updates internal skip parameters used by Transform().
						/// </summary>
						void SetWidth(const dimension_t width)
						{
							const dimension_t target = (width > Width) ? Width : width;

							TargetWidth = (target < 0) ? 0 : target;
							SkipCount = Width - TargetWidth;

							if (Selector::Use16)
								SkipScalar = UFraction16::GetScalar<dimension_t>(SkipCount, Width);
							else
								SkipScalar = UFraction8::GetScalar<dimension_t>(SkipCount, Width);
						}

						/// <summary>Gets the current target width (pixels).</summary>
						dimension_t GetWidth() const
						{
							return static_cast<dimension_t>(TargetWidth);
						}

						/// <summary>
						/// Transforms a source coordinate in-place.
						/// Returns false to indicate the source pixel should be skipped (dropped) when scaling down.
						/// </summary>
						/// <param name="x">X coordinate to transform in-place.</param>
						/// <param name="y">Y coordinate to transform in-place.</param>
						/// <returns>True if the pixel is kept; false if it should be skipped.</returns>
						bool Transform(dimension_t& x, dimension_t& y)
						{
							if (!Base::Transform(x, y))
							{
								return false;
							}

							if (x < 0 || x >= Width)
							{
								return false;
							}

							if (SkipCount == 0)
							{
								return true;
							}

							const dimension_t skippedBefore = Fraction(SkipScalar, x);

							// Skip the seam pixel.
							if (skippedBefore != 0 && x == skippedBefore)
							{
								return false;
							}

							const signed_t destX = x - skippedBefore;
							if (destX < 0 || destX >= TargetWidth)
							{
								return false;
							}

							x = destX;
							return true;
						}
					};

					/// <summary>
					/// Y-axis scale-down transform (drop-and-pack).
					/// Drops (Height - TargetHeight) source pixels and compacts the remaining Y coordinates.
					/// Transform returns false for source pixels that should be skipped (dropped).
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Source width in pixels (transform-space).</typeparam>
					/// <typeparam name="Height">Source height in pixels (transform-space).</typeparam>
					/// <typeparam name="Base">Base transform type in the chain (defaults to BaseTransform).</typeparam>
					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>
					>
					class ScaleDownY : public Base
					{
					private:
						using Selector = Framework::Shader::Transform::FractionSelector<Width, Height>;
						using ufraction_t = typename Selector::ufraction_t;
						using signed_t = typename TypeTraits::TypeNext::next_int_type<dimension_t>::type;

					private:
						/// <summary>Target height (pixels) produced by the transform.</summary>
						dimension_t TargetHeight = Height;

						/// <summary>Number of source pixels to drop across the height.</summary>
						dimension_t SkipCount = 0;

						/// <summary>Cached scalar: SkipCount / Height (fixed-point).</summary>
						ufraction_t SkipScalar = {}; // SkipCount / Height

					public:
						ScaleDownY() : Base() {}
						~ScaleDownY() = default;

						/// <summary>
						/// Sets the target height (pixels). Clamped to [0..Height].
						/// Updates internal skip parameters used by Transform().
						/// </summary>
						void SetHeight(const dimension_t height)
						{
							const dimension_t target = (height > Height) ? Height : height;

							TargetHeight = (target < 0) ? 0 : target;
							SkipCount = Height - TargetHeight;

							if (Selector::Use16)
								SkipScalar = UFraction16::GetScalar<dimension_t>(SkipCount, Height);
							else
								SkipScalar = UFraction8::GetScalar<dimension_t>(SkipCount, Height);
						}

						/// <summary>Gets the current target height (pixels).</summary>
						dimension_t GetHeight() const
						{
							return static_cast<dimension_t>(TargetHeight);
						}

						/// <summary>
						/// Transforms a source coordinate in-place.
						/// Returns false to indicate the source pixel should be skipped (dropped) when scaling down.
						/// </summary>
						/// <param name="x">X coordinate to transform in-place.</param>
						/// <param name="y">Y coordinate to transform in-place.</param>
						/// <returns>True if the pixel is kept; false if it should be skipped.</returns>
						bool Transform(dimension_t& x, dimension_t& y)
						{
							if (!Base::Transform(x, y))
							{
								return false;
							}

							if (y < 0 || y >= Height)
							{
								return false;
							}

							if (SkipCount == 0)
							{
								return true;
							}

							const dimension_t skippedBefore = Fraction(SkipScalar, y);

							// Skip the seam pixel.
							if (skippedBefore != 0 && y == skippedBefore)
							{
								return false;
							}

							const signed_t destY = y - skippedBefore;
							if (destY < 0 || destY >= TargetHeight)
							{
								return false;
							}

							y = destY;

							return true;
						}
					};

					/// <summary>
					/// Combined scale-down transform (Y scale-down, then X scale-down) using the provided Base transform chain.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Source width in pixels (transform-space).</typeparam>
					/// <typeparam name="Height">Source height in pixels (transform-space).</typeparam>
					/// <typeparam name="Base">Base transform type in the chain (defaults to BaseTransform).</typeparam>
					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>>
						using ScaleDownXY = ScaleDownY<dimension_t, Width, Height, ScaleDownX<dimension_t, Width, Height, Base>>;
				}
			}
		}
	}
}
#endif