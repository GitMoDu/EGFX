#ifndef _EGFX_FRAMEWORK_SHADER_SOURCE_MODEL_h
#define _EGFX_FRAMEWORK_SHADER_SOURCE_MODEL_h

#include "../../../EgfxCore.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Source
			{
				namespace Contract
				{
					/// <summary>
					/// Defines the contract for color sourcing, uses only coordinate inputs to determine the color output.
					/// This class serves as documentation only, and is not intended to be used directly.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					template<typename dimension_t>
					struct ColorSource
					{
						// Color source contract.
						rgb_color_t Source(const dimension_t /*x*/, const dimension_t /*y*/)
						{
							// Color sourcing.
							return RGB_COLOR_BLACK;
						}
					};
				}
			}
		}
	}
}
#endif