#ifndef _EGFX_FRAMEWORK_SHADER_IMAGE_VECTOR_IMAGE_SHADER_h
#define _EGFX_FRAMEWORK_SHADER_IMAGE_VECTOR_IMAGE_SHADER_h

#include "VectorShader.h"

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
					/// <summary>
					/// Combines decoder with vector shader to provide a complete image rendering solution for vector graphics.
					/// </summary>
					template<typename dimension_t,
						typename PaletteType = Framework::Shader::Image::Vector::StaticColorPalette<RGB_COLOR_WHITE>,
						typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
						Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
					>
					class Image : public Framework::Vector::Decoder<dimension_t,
						VectorShader<dimension_t, PaletteType, ColorShaderType, TransformShaderType, BlendMode>>
					{
					private:
						using Base = Framework::Vector::Decoder<dimension_t,
							VectorShader<dimension_t, PaletteType, ColorShaderType, TransformShaderType, BlendMode>>;
						using signed_t = typename AutoDimension::ByDimension<dimension_t>::signed_t;

					public:
						using Base::Base;
						using Base::GetPalette;

						template<Framework::Vector::PackedVectorEnum vectorType,
							Framework::DataSourceTypeEnum dataSourceType = Framework::DataSourceTypeEnum::Ram>
						void Draw(IFrameBuffer* frame, const void* data, const size_t count,
							const dimension_t canvasWidth, const dimension_t canvasHeight,
							const ufraction8_t thicknessScale, const dimension_t width, const dimension_t height,
							const signed_t offsetX, const signed_t offsetY, const dimension_t inset = 0)
						{
							Base::SetOffset(offsetX, offsetY);
							Base::template Decode<vectorType, dataSourceType>(frame, data, count, canvasWidth, canvasHeight,
								thicknessScale, width, height, inset);
						}

						template<Framework::DataSourceTypeEnum dataSourceType = Framework::DataSourceTypeEnum::Ram>
						void Draw(IFrameBuffer* frame, const Framework::Vector::PackedVectorEnum vectorType,
							const void* data, const size_t count,
							const dimension_t canvasWidth, const dimension_t canvasHeight,
							const ufraction8_t thicknessScale, const dimension_t width, const dimension_t height,
							const signed_t offsetX, const signed_t offsetY, const dimension_t inset = 0)
						{
							switch (vectorType)
							{
							case Framework::Vector::PackedVectorEnum::Image15x15:
								Draw<Framework::Vector::PackedVectorEnum::Image15x15, dataSourceType>(frame, data, count,
									canvasWidth, canvasHeight, thicknessScale, width, height, offsetX, offsetY, inset);
								break;
							case Framework::Vector::PackedVectorEnum::Image255x255:
								Draw<Framework::Vector::PackedVectorEnum::Image255x255, dataSourceType>(frame, data, count,
									canvasWidth, canvasHeight, thicknessScale, width, height, offsetX, offsetY, inset);
								break;
							default:
								break;
							}
						}
					};
				}
			}
		}
	}
}
#endif