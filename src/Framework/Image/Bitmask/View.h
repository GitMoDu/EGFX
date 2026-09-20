#ifndef _INTEGERGLASS_FRAMEWORK_IMAGE_BITMASK_VIEW_h
#define _INTEGERGLASS_FRAMEWORK_IMAGE_BITMASK_VIEW_h

#include "Drawable.h"

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Image
		{
			namespace Bitmask
			{
				template<typename dimension_t,
					typename ParentLayout,
					typename ImageSourceType, // Contract::StaticBitmaskSource,
					typename Style = Framework::Image::TemplateImageStyle<>,
					typename SourceShaderType = Shader::Source::StaticColor<dimension_t>,
					typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
					typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
					Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
				>
				class StaticImageView : public Framework::View::DrawablesView<ParentLayout,
					Drawable::StaticImage<dimension_t, ParentLayout, ImageSourceType, Style, SourceShaderType, ColorShaderType, TransformShaderType, BlendMode>
				>
				{
				public:
					using ViewLayout = ParentLayout;

				private:
					using DrawableType = Drawable::StaticImage<dimension_t, ParentLayout, ImageSourceType, Style, SourceShaderType, ColorShaderType, TransformShaderType, BlendMode>;
					using Base = Framework::View::DrawablesView<ParentLayout, DrawableType>;

				public:
					StaticImageView() : Base() {}
					~StaticImageView() = default;

					DrawableType& Drawable()
					{
						return Base::template drawable<0>();
					}
				};
			}
		}
	}
}
#endif