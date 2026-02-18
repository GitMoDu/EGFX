#ifndef _EGFX_FRAMEWORK_SHADER_PRIMITIVE_SHADERS_h
#define _EGFX_FRAMEWORK_SHADER_PRIMITIVE_SHADERS_h

#include "Model.h"
#include "../Geometry/Triangle.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Primitive
			{
				/// <summary>
				/// Default primitive shader pipeline.
				/// Layers primitive rasterization (Line, RectangleFill, TriangleFill) on top of a pixel shader's Pixel(...) implementation.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				/// <typeparam name="PixelShaderType">Pixel shader type providing Pixel(...) and Prepare(...).</typeparam>
				template<typename dimension_t,
					typename PixelShaderType = Shader::Pixel::TemplateShader<dimension_t>
				>
				using TemplateShader = Geometry::TriangleShader<dimension_t, PixelShaderType>;
			}
		}
	}
}
#endif