#ifndef _INTEGERGLASS_FRAMEWORK_ICON_VECTOR_VIEW_h
#define _INTEGERGLASS_FRAMEWORK_ICON_VECTOR_VIEW_h

#include "Drawable.h"

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Icon
		{
			namespace Vector
			{
				template<typename dimension_t, typename ParentLayout, typename IconSetType,
					typename Style = Framework::Image::TemplateImageStyle<>,
					typename PaletteType = Framework::Shader::Image::Vector::StaticColorPalette<RGB_COLOR_WHITE>,
					typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
					typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
					Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
				>
				class IconView : public Framework::View::DrawablesView<ParentLayout,
					Drawable::Icon<dimension_t, ParentLayout, IconSetType, Style, PaletteType, ColorShaderType, TransformShaderType, BlendMode>>
				{
				private:
					using DrawableType = Drawable::Icon<dimension_t, ParentLayout, IconSetType, Style, PaletteType, ColorShaderType, TransformShaderType, BlendMode>;
					using Base = Framework::View::DrawablesView<ParentLayout, DrawableType>;

				public:
					using ViewLayout = ParentLayout;
					using icon_enum_t = Framework::Icon::IconEnum;
					using signed_t = typename DrawableType::coordinate_t;

					IconView() : Base() {}
					~IconView() = default;

					DrawableType& Drawable() { return Base::template drawable<0>(); }
					const DrawableType& Drawable() const { return Base::template drawable<0>(); }

					void SetIcon(const icon_enum_t icon) { Drawable().SetIcon(icon); }
					icon_enum_t GetIcon() const { return Drawable().GetIcon(); }
					void SetDimensions(const dimension_t width, const dimension_t height) { Drawable().SetDimensions(width, height); }
					void SetOffset(const signed_t x, const signed_t y) { Drawable().SetOffset(x, y); }
					void SetInset(const dimension_t inset) { Drawable().SetInset(inset); }
				};
			}
		}
	}
}
#endif
