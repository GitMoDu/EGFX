#ifndef _INTEGERGLASS_FRAMEWORK_IMAGE_BITMAP_VIEW_h
#define _INTEGERGLASS_FRAMEWORK_IMAGE_BITMAP_VIEW_h

#include "Drawable.h"

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Image
		{
			namespace Bitmap
			{
				template<typename dimension_t,
					typename ParentLayout,
					typename ImageSourceType, // Contract::StaticBitmapSource,
					typename Style = Framework::Image::TemplateImageStyle<>,
					typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
					typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
					Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
				>
				class StaticImageView : public Framework::View::DrawablesView<ParentLayout,
					Drawable::StaticImage<dimension_t, ParentLayout, ImageSourceType, Style, ColorShaderType, TransformShaderType, BlendMode>
				>
				{
				public:
					using ViewLayout = ParentLayout;

				private:
					using DrawableType = Drawable::StaticImage<dimension_t, ParentLayout, ImageSourceType, Style, ColorShaderType, TransformShaderType, BlendMode>;
					using Base = Framework::View::DrawablesView<ParentLayout, DrawableType>;


				public:
					StaticImageView() : Base() {}
					~StaticImageView() = default;

					DrawableType& Drawable()
					{
						return Base::template drawable<0>();
					}
				};

				template<typename dimension_t,
					typename ParentLayout,
					typename color_t,
					Framework::DataSourceTypeEnum DataSourceType = Framework::DataSourceTypeEnum::Ram,
					typename Style = Framework::Image::TemplateImageStyle<>,
					typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
					typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
					Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
				>
					class ImageView : public Framework::View::DrawablesView<ParentLayout,
					Drawable::Image<dimension_t, ParentLayout, color_t, DataSourceType, Style, ColorShaderType, TransformShaderType, BlendMode>
				>
				{
				public:
					using ViewLayout = ParentLayout;

				private:
					using DrawableType = Drawable::Image<dimension_t, ParentLayout, color_t, DataSourceType, Style, ColorShaderType, TransformShaderType, BlendMode>;
					using Base = Framework::View::DrawablesView<ParentLayout, DrawableType>;

				public:
					ImageView() : Base() {}
					~ImageView() = default;

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