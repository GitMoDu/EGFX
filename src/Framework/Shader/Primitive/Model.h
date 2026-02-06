#ifndef _EGFX_FRAMEWORK_SHADER_PRIMITIVE_MODEL_h
#define _EGFX_FRAMEWORK_SHADER_PRIMITIVE_MODEL_h

#include "../../../EgfxCore.h"

#include "../Source/Sources.h"
#include "../Pixel/Shaders.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Primitive
			{
				namespace Contract
				{ 
					/// <summary>
					/// Defines the contract for primitive shading (rasterization).
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					template<typename dimension_t, typename PixelShaderType>
					struct PrimitiveShader : PixelShaderType
					{
						// PixelShaderType implementation.
						void Prepare(const pixel_t originX, const pixel_t originY) {}
						void Pixel(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y) {}

						// PrimitiveShader implementation.
						void Line(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1, const dimension_t x2, const dimension_t y2) {}
						void RectangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1, const dimension_t x2, const dimension_t y2) {}
						void TriangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1, const dimension_t x2, const dimension_t y2, const dimension_t x3, const dimension_t y3) {}
					};
				}
			}
		}
	}
}
#endif