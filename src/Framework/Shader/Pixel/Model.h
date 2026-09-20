#ifndef _INTEGERGLASS_FRAMEWORK_SHADER_PIXEL_MODEL_h
#define _INTEGERGLASS_FRAMEWORK_SHADER_PIXEL_MODEL_h

#include "../Color/Model.h"
#include "../Transform/Model.h"

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Pixel
			{
				/// <summary>
				/// Blend modes for pixel shaders.
				/// Selects which framebuffer write/blend operation is used when emitting the final shaded color.
				/// </summary>
				enum class BlendModeEnum : uint8_t
				{
					// Replace the framebuffer pixel with the shader's output color.
					Replace,

					// Add the shader's output color to the framebuffer pixel.
					Add,

					// Subtract the shader's output color from the framebuffer pixel.
					Subtract,

					// Multiply the shader's output color with the framebuffer pixel.
					Multiply,

					// Divide the framebuffer pixel by the shader's output color.
					Screen,

					// Invert the framebuffer pixel and multiply by the shader's output color.
					Xor,

					// Blend the shader's output color with the framebuffer pixel using half alpha.
					BlendHalfAlpha,

					// Blend the shader's output color with the framebuffer pixel using the configured alpha.
					BlendAlpha
				};

				namespace Contract
				{
					/// <summary>
					/// Pixel shader contract.
					///
					/// A pixel shader is responsible for emitting pixels to a framebuffer using a prepared origin
					/// and local-space coordinates. Implementations may compose a color source, a color shader,
					/// and a transform shader, but this contract only defines the callable surface.
					/// This class serves as documentation only, and is not intended to be used directly.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					template<typename dimension_t>
					struct PixelShader
					{
						/// <summary>
						/// Prepares the shader for a draw cycle by setting the origin in pixel-space.
						/// </summary>
						/// <param name="originX">Pixel-space origin X (pixels).</param>
						/// <param name="originY">Pixel-space origin Y (pixels).</param>
						void Prepare(const dimension_t /*originX*/, const dimension_t /*originY*/) {}

						/// <summary>
						/// Shades and writes a single pixel at the given local-space coordinate.
						/// </summary>
						/// <param name="framebuffer">Target framebuffer to draw into.</param>
						/// <param name="x">Local-space X coordinate.</param>
						/// <param name="y">Local-space Y coordinate.</param>
						void Pixel(IFrameBuffer* /*framebuffer*/, const dimension_t /*x*/, const dimension_t /*y*/) {}
					};
				}
			}
		}
	}
}
#endif