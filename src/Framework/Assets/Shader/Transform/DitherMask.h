#ifndef _EGFX_FRAMEWORK_ASSETS_SHADER_TRANSFORM_DITHER_MASK_h
#define _EGFX_FRAMEWORK_ASSETS_SHADER_TRANSFORM_DITHER_MASK_h

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
					/// Grid dither mask with configurable modulus and phase offsets.
					/// Keeps a pixel only when both (x + OffsetX) and (y + OffsetY) land on a modulus boundary.
					/// For a classic 1x1 checkerboard pattern, use CheckerboardMask for better performance.
					/// Uses two modulo operations per pixel when enabled.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="UseGlobalCoordinates">When true, uses Origin+x/y so the pattern is stable across nested transforms.</typeparam>
					/// <typeparam name="Base">Base transform type in the chain (defaults to BaseTransform).</typeparam>
					template<typename dimension_t,
						bool UseGlobalCoordinates = true,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>
					>
					class DitherMask : public Base
					{
					private:
						/// <summary>
						/// Grid modulus (step). Values &lt; 2 disable the mask (all pixels are kept).
						/// </summary>
						uint8_t Modulus = 2;

						/// <summary>Phase offset added to the mask X coordinate before applying the modulus.</summary>
						uint8_t OffsetX = 0;

						/// <summary>Phase offset added to the mask Y coordinate before applying the modulus.</summary>
						uint8_t OffsetY = 0;

					protected:
						using Base::Origin;

					public:
						DitherMask() : Base() {}
						~DitherMask() = default;

						/// <summary>Disables the mask (keeps all pixels).</summary>
						void DisableDitherMask()
						{
							Modulus = 0;
						}

						/// <summary>Gets the X phase offset (mask-space).</summary>
						uint8_t GetMaskPhaseX() const
						{
							return OffsetX;
						}

						/// <summary>Gets the Y phase offset (mask-space).</summary>
						uint8_t GetMaskPhaseY() const
						{
							return OffsetY;
						}

						/// <summary>Sets the X phase offset (mask-space).</summary>
						void SetMaskPhaseX(const uint8_t offsetX)
						{
							OffsetX = offsetX;
						}

						/// <summary>Sets the Y phase offset (mask-space).</summary>
						void SetMaskPhaseY(const uint8_t offsetY)
						{
							OffsetY = offsetY;
						}

						/// <summary>Sets the X/Y phase offsets (mask-space).</summary>
						void SetMaskPhase(const uint8_t offsetX, const uint8_t offsetY)
						{
							OffsetX = offsetX;
							OffsetY = offsetY;
						}

						/// <summary>Sets the grid modulus (step). Values &lt; 2 disable the mask.</summary>
						void SetModulus(const uint8_t modulus)
						{
							Modulus = modulus;
						}

						/// <summary>Gets the grid modulus (step).</summary>
						uint8_t GetModulus() const { return Modulus; }

						/// <summary>
						/// Applies the base transform chain and then applies the mask.
						/// Returns false when the pixel should be rejected by the dither mask.
						/// </summary>
						/// <param name="x">X coordinate to transform in-place.</param>
						/// <param name="y">Y coordinate to transform in-place.</param>
						/// <returns>True if the pixel is kept; false if it should be masked out.</returns>
						bool Transform(dimension_t& x, dimension_t& y)
						{
							if (!Base::Transform(x, y))
							{
								return false;
							}

							// Disabled for values < 2 (also avoids modulo-by-zero).
							if (Modulus < 2)
							{
								return true;
							}

							const pixel_index_t maskX = static_cast<pixel_index_t>(
								UseGlobalCoordinates ? (Origin.x + x) : x);

							const pixel_index_t maskY = static_cast<pixel_index_t>(
								UseGlobalCoordinates ? (Origin.y + y) : y);

							return (((maskX + OffsetX) % Modulus) == 0)
								&& (((maskY + OffsetY) % Modulus) == 0);
						}
					};

					/// <summary>
					/// Fast 1x1 checkerboard mask with configurable phase offset.
					/// This is the dedicated fast-path type: no modulus support by design.
					/// Uses only an AND with 1 (no division).
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="UseGlobalCoordinates">When true, uses Origin+x/y so the pattern is stable across nested transforms.</typeparam>
					/// <typeparam name="Base">Base transform type in the chain (defaults to BaseTransform).</typeparam>
					template<typename dimension_t,
						bool UseGlobalCoordinates = true,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>
					>
					class CheckerboardMask : public Base
					{
					private:
						/// <summary>Phase offset added to (x + y) before evaluating the checkerboard parity.</summary>
						uint8_t Offset = 0;

						/// <summary>Enables/disables the mask without changing the phase.</summary>
						bool Enabled = true;

					protected:
						using Base::Origin;

					public:
						CheckerboardMask() : Base() {}
						~CheckerboardMask() = default;

						/// <summary>Enables or disables the checkerboard mask without changing the phase.</summary>
						void SetCheckerboardMaskEnabled(const bool enabled)
						{
							Enabled = enabled;
						}

						/// <summary>Sets the pattern phase/shift (does not change enabled state).</summary>
						void SetMaskPhase(const uint8_t offset)
						{
							Offset = offset;
						}

						/// <summary>Gets the pattern phase/shift.</summary>
						uint8_t GetMaskPhase() const
						{
							return Offset;
						}

						/// <summary>
						/// Applies the base transform chain and then applies the 1x1 checkerboard mask.
						/// Returns false when the pixel should be rejected by the mask.
						/// </summary>
						/// <param name="x">X coordinate to transform in-place.</param>
						/// <param name="y">Y coordinate to transform in-place.</param>
						/// <returns>True if the pixel is kept; false if it should be masked out.</returns>
						bool Transform(dimension_t& x, dimension_t& y)
						{
							if (!Base::Transform(x, y))
							{
								return false;
							}

							if (!Enabled)
							{
								return true;
							}

							const pixel_index_t maskX = static_cast<pixel_index_t>(
								UseGlobalCoordinates ? (Origin.x + x) : x);

							const pixel_index_t maskY = static_cast<pixel_index_t>(
								UseGlobalCoordinates ? (Origin.y + y) : y);

							// Keep every other pixel in a 1x1 checkerboard pattern.
							return (((maskX + maskY + Offset) & 1) == 0);
						}
					};
				}
			}
		}
	}
}
#endif