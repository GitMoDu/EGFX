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

						/// <summary>Source width (pixels) as the working index type.</summary>
						static constexpr pixel_index_t SourceWidth = static_cast<pixel_index_t>(Width);

						/// <summary>Target width (pixels) produced by the transform.</summary>
						pixel_index_t TargetWidth = SourceWidth;

						/// <summary>Number of source pixels to drop across the width.</summary>
						pixel_index_t SkipCount = 0;

						/// <summary>Cached scalar: SkipCount / SourceWidth (fixed-point).</summary>
						ufraction_t SkipScalar = {}; // SkipCount / SourceWidth

					public:
						ScaleDownX() : Base() {}
						~ScaleDownX() = default;

						/// <summary>
						/// Sets the target width (pixels). Clamped to [0..Width].
						/// Updates internal skip parameters used by Transform().
						/// </summary>
						void SetWidth(const dimension_t width)
						{
							const pixel_index_t target =
								(static_cast<pixel_index_t>(width) > SourceWidth) ? SourceWidth : static_cast<pixel_index_t>(width);

							TargetWidth = (target < 0) ? 0 : target;
							SkipCount = SourceWidth - TargetWidth;

							if (Selector::Use16)
								SkipScalar = UFraction16::GetScalar<pixel_index_t>(SkipCount, SourceWidth);
							else
								SkipScalar = UFraction8::GetScalar<pixel_index_t>(SkipCount, SourceWidth);
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

							const pixel_index_t sourceX = static_cast<pixel_index_t>(x);
							if (sourceX < 0 || sourceX >= SourceWidth)
							{
								return false;
							}

							if (SkipCount == 0)
							{
								return true;
							}

							const pixel_index_t skippedBefore = Fraction(SkipScalar, sourceX);

							// Skip the seam pixel.
							if (skippedBefore != 0 && sourceX == skippedBefore)
							{
								return false;
							}

							const pixel_index_t destX = sourceX - skippedBefore;
							if (destX < 0 || destX >= TargetWidth)
							{
								return false;
							}

							x = static_cast<dimension_t>(destX);
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

						/// <summary>Source height (pixels) as the working index type.</summary>
						static constexpr pixel_index_t SourceHeight = static_cast<pixel_index_t>(Height);

						/// <summary>Target height (pixels) produced by the transform.</summary>
						pixel_index_t TargetHeight = SourceHeight;

						/// <summary>Number of source pixels to drop across the height.</summary>
						pixel_index_t SkipCount = 0;

						/// <summary>Cached scalar: SkipCount / SourceHeight (fixed-point).</summary>
						ufraction_t SkipScalar = {}; // SkipCount / SourceHeight

					public:
						ScaleDownY() : Base() {}
						~ScaleDownY() = default;

						/// <summary>
						/// Sets the target height (pixels). Clamped to [0..Height].
						/// Updates internal skip parameters used by Transform().
						/// </summary>
						void SetHeight(const dimension_t height)
						{
							const pixel_index_t target =
								(static_cast<pixel_index_t>(height) > SourceHeight) ? SourceHeight : static_cast<pixel_index_t>(height);

							TargetHeight = (target < 0) ? 0 : target;
							SkipCount = SourceHeight - TargetHeight;

							if (Selector::Use16)
								SkipScalar = UFraction16::GetScalar<pixel_index_t>(SkipCount, SourceHeight);
							else
								SkipScalar = UFraction8::GetScalar<pixel_index_t>(SkipCount, SourceHeight);
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

							const pixel_index_t sourceY = static_cast<pixel_index_t>(y);
							if (sourceY < 0 || sourceY >= SourceHeight)
							{
								return false;
							}

							if (SkipCount == 0)
							{
								return true;
							}

							const pixel_index_t skippedBefore = Fraction(SkipScalar, sourceY);

							// Skip the seam pixel.
							if (skippedBefore != 0 && sourceY == skippedBefore)
							{
								return false;
							}

							const pixel_index_t destY = sourceY - skippedBefore;
							if (destY < 0 || destY >= TargetHeight)
							{
								return false;
							}

							y = static_cast<dimension_t>(destY);
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