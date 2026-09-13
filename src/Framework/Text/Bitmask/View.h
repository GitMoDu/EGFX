#ifndef _EGFX_FRAMEWORK_TEXT_BITMASK_VIEW_h
#define _EGFX_FRAMEWORK_TEXT_BITMASK_VIEW_h

#include "Drawable.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Text
		{
			namespace Bitmask
			{
				template<typename dimension_t,
					typename ParentLayout,
					typename GlyphSourceType,
					typename GlyphStyle = Framework::Image::TemplateImageStyle<>,
					typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
					typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
					Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
				>
				class TextView : public Framework::View::DrawablesView<ParentLayout,
					Drawable::Text<dimension_t, ParentLayout, GlyphSourceType, GlyphStyle,
					ColorShaderType, TransformShaderType, BlendMode>
				>
				{
				public:
					using DrawableType = Drawable::Text<dimension_t, ParentLayout, GlyphSourceType,
						GlyphStyle, ColorShaderType, TransformShaderType, BlendMode>;

					using ViewLayout = ParentLayout;

				private:
					using Base = Framework::View::DrawablesView<ParentLayout, DrawableType>;

				public:
					TextView() : Base() {}
					~TextView() = default;

					DrawableType& Drawable()
					{
						return Base::template drawable<0>();
					}

					void SetFontSize(const dimension_t width, const dimension_t height) { Drawable().SetFontSize(width, height); }
					void SetTabWidth(const dimension_t width) { Drawable().SetTabWidth(width); }
					void SetSpaceWidth(const dimension_t width) { Drawable().SetSpaceWidth(width); }
					void SetKerningWidth(const dimension_t width) { Drawable().SetKerningWidth(width); }
					void SetLineSpacing(const dimension_t spacing) { Drawable().SetLineSpacing(spacing); }
				};

				template<typename dimension_t,
					typename ParentLayout,
					typename number_t,
					typename GlyphSourceType,
					typename GlyphStyle = Framework::Image::TemplateImageStyle<>,
					typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
					typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
					Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
				>
				class NumberView : public Framework::View::DrawablesView<ParentLayout,
					Drawable::Number<dimension_t, ParentLayout, number_t,
					GlyphSourceType, GlyphStyle,
					ColorShaderType, TransformShaderType, BlendMode>
				>
				{
				public:
					using DrawableType = Drawable::Number<dimension_t, ParentLayout, number_t,
						GlyphSourceType, GlyphStyle, ColorShaderType, TransformShaderType, BlendMode>;

					using ViewLayout = ParentLayout;

				private:
					using Base = Framework::View::DrawablesView<ParentLayout, DrawableType>;

				public:
					NumberView() : Base() {}
					~NumberView() = default;

					DrawableType& Drawable()
					{
						return Base::template drawable<0>();
					}

					void SetFontSize(const dimension_t width, const dimension_t height) { Drawable().SetFontSize(width, height); }
					void SetKerningWidth(const dimension_t width) { Drawable().SetKerningWidth(width); }

				};
			}
		}
	}
}
#endif