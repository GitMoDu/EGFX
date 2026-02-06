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
					/// Full classic alternating checkerboard tiles with configurable modulus and phase offset.
					/// For simple 1x1 checkerboard patterns, use CheckerboardMask instead for better performance.
					/// Uses two modulo operations per pixel.
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
						/// <summary>Tile modulus. Values &lt; 2 disable the mask.</summary>
						uint8_t Modulus = 0;

						/// <summary>Phase offset applied to the mask X coordinate.</summary>
						uint8_t OffsetX = 0;

						/// <summary>Phase offset applied to the mask Y coordinate.</summary>
						uint8_t OffsetY = 0;

					protected:
						using Base::Origin;

					public:
						DitherMask() : Base() {}
						~DitherMask() = default;

						/// <summary>Disables the dither mask.</summary>
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

						/// <summary>Sets the tile modulus. Values &lt; 2 disable the mask.</summary>
						void SetModulus(const uint8_t modulus)
						{
							Modulus = modulus;
						}

						/// <summary>Gets the tile modulus.</summary>
						uint8_t GetModulus() const { return Modulus; }

						/// <summary>
						/// Applies the base transform chain and then applies the mask.
						/// Returns false when the pixel should be rejected by the checkerboard mask.
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

							if (Modulus < 2)
							{
								return true; // disabled
							}

							const pixel_index_t maskX = static_cast<pixel_index_t>(
								UseGlobalCoordinates ? (Origin.x + x) : x) + OffsetX;

							const pixel_index_t maskY = static_cast<pixel_index_t>(
								UseGlobalCoordinates ? (Origin.y + y) : y) + OffsetY;

							// Classic alternating tiles without division:
							// Determine which half of the modulus we're in for each axis, XOR for checkerboard.
							const uint8_t half = static_cast<uint8_t>(Modulus >> 1);
							if (half == 0)
							{
								return true;
							}

							const uint8_t rx = static_cast<uint8_t>(maskX % Modulus);
							const uint8_t ry = static_cast<uint8_t>(maskY % Modulus);

							const bool a = (rx < half);
							const bool b = (ry < half);

							return (a ^ b);
						}
					};

					/// <summary>
					/// Fast dither mask: per-pixel 1x1 checkerboard with configurable phase offset.
					/// Modulus is intentionally not supported here; this is the dedicated fast-path type.
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
						/// <summary>Phase offset applied to (x+y) (mask-space).</summary>
						uint8_t Offset = 0;

						/// <summary>Enables/disables the mask without changing the phase.</summary>
						bool Enabled = true;

					protected:
						using Base::Origin;

					public:
						CheckerboardMask() : Base() {}
						~CheckerboardMask() = default;

						/// <summary>Disables the dither mask.</summary>
						void DisableDitherMask()
						{
							Enabled = false;
						}

						/// <summary>Sets the pattern phase/shift and enables the mask.</summary>
						void SetMaskPhase(const uint8_t offset)
						{
							Offset = offset;
							Enabled = true;
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