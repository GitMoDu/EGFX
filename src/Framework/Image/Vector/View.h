#ifndef _EGFX_FRAMEWORK_IMAGE_VECTOR_VIEW_h
#define _EGFX_FRAMEWORK_IMAGE_VECTOR_VIEW_h

#include "Drawable.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Image
		{
			namespace Vector
			{
				using namespace Framework::Vector;

				template<typename dimension_t, typename ParentLayout,
					typename ImageSourceType,
					typename Style = Framework::Image::TemplateImageStyle<>,
					typename PaletteType = Framework::Shader::Image::Vector::StaticColorPalette<RGB_COLOR_WHITE>,
					typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
					typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>>
					class StaticImageView : public Framework::View::DrawablesView<ParentLayout,
					Drawable::StaticImage<dimension_t, ParentLayout, ImageSourceType, Style, PaletteType, ColorShaderType, TransformShaderType>>
				{
				public:
					using ViewLayout = ParentLayout;

				private:
					using DrawableType = Drawable::StaticImage<dimension_t, ParentLayout, ImageSourceType, Style, PaletteType, ColorShaderType, TransformShaderType>;
					using Base = Framework::View::DrawablesView<ParentLayout, DrawableType>;

				public:
					StaticImageView() : Base() {}
					~StaticImageView() = default;

					DrawableType& Drawable()
					{
						return Base::template drawable<0>();
					}
				};

				template<typename dimension_t, typename ParentLayout,
					typename Style = Framework::Image::TemplateImageStyle<>,
					typename PaletteType = Framework::Shader::Image::Vector::StaticColorPalette<RGB_COLOR_WHITE>,
					typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
					typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>>
					class ImageView : public Framework::View::DrawablesView<ParentLayout,
					Drawable::Image<dimension_t, ParentLayout, Style, PaletteType, ColorShaderType, TransformShaderType>>
				{
				public:
					using ViewLayout = ParentLayout;

				private:
					using DrawableType = Drawable::Image<dimension_t, ParentLayout, Style, PaletteType, ColorShaderType, TransformShaderType>;
					using Base = Framework::View::DrawablesView<ParentLayout, DrawableType>;

				public:
					ImageView() : Base() {}
					~ImageView() = default;

					DrawableType& Drawable()
					{
						return Base::template drawable<0>();
					}
					const DrawableType& Drawable() const
					{
						return Base::template drawable<0>();
					}

					void SetImage(const Image15x15::packed_t* data, const size_t count,
						const dimension_t canvasWidth, const dimension_t canvasHeight,
						const ufraction8_t thicknessScale, const dimension_t inset = 0,
						const Framework::DataSourceTypeEnum dataSource = Framework::DataSourceTypeEnum::Ram)
					{
						Drawable().SetImage(data, count, canvasWidth, canvasHeight, thicknessScale, inset, dataSource);
					}

					void SetImage(const Image255x255::packed_t* data, const size_t count,
						const dimension_t canvasWidth, const dimension_t canvasHeight,
						const ufraction8_t thicknessScale, const dimension_t inset = 0,
						const Framework::DataSourceTypeEnum dataSource = Framework::DataSourceTypeEnum::Ram)
					{
						Drawable().SetImage(data, count, canvasWidth, canvasHeight, thicknessScale, inset, dataSource);
					}

					void SetOffset(const dimension_t x, const dimension_t y)
					{
						Drawable().SetOffset(x, y);
					}

					void SetDimensions(const dimension_t width, const dimension_t height)
					{
						Drawable().SetDimensions(width, height);
					}

					void SetInset(const dimension_t inset)
					{
						Drawable().SetInset(inset);
					}

					dimension_t GetWidth() const { return Drawable().GetWidth(); }
					dimension_t GetHeight() const { return Drawable().GetHeight(); }
					dimension_t GetOffsetX() const { return Drawable().GetOffsetX(); }
					dimension_t GetOffsetY() const { return Drawable().GetOffsetY(); }
				};
			}
		}
	}
}
#endif