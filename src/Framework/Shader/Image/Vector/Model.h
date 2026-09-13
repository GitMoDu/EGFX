#ifndef _EGFX_FRAMEWORK_SHADER_IMAGE_VECTOR_MODEL_h
#define _EGFX_FRAMEWORK_SHADER_IMAGE_VECTOR_MODEL_h

#include "../../../Image/Model.h"
#include "../../../Vector/Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Image
			{
				namespace Vector
				{
					namespace Contract
					{
						struct PaletteSource
						{
							static constexpr rgb_color_t GetColor(const uint8_t /*colorIndex*/)
							{
								return RGB_COLOR_BLACK;
							}
						};
					}
				}
			}
		}
	}
}
#endif