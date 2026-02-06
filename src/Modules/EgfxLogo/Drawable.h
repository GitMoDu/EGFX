#ifndef _EGFX_MODULES_EGFX_LOGO_DRAWABLE_h
#define _EGFX_MODULES_EGFX_LOGO_DRAWABLE_h

#include "Constant.h"
#include "Font.h"
#include "Layout.h"

#include "Shaders/Source.h"
#include "Shaders/Color.h"
#include "Shaders/Pixel.h"
#include "Shaders/Primitive.h"

namespace Egfx
{
	namespace Modules
	{
		namespace EgfxLogo
		{
			namespace Drawable
			{
				using dimension_t = pixel_t;
				namespace AsciiDefinition = Framework::AsciiDefinition;

				/// <summary>
				/// Rectangle outline with custom Pixel shader.
				/// </summary>
				template<typename ParentLayout, bool Monochrome>
				using Screen = Framework::Assets::Drawable::Code::Rectangle::TemplateDrawable<
					typename Layout::LogoLayout<ParentLayout>::ScreenMargin,
					Shader::Primitive::Screen<Monochrome>
				>;

				/// <summary>
				/// Draws letters "EG" using Logo font with RetroLines shader.
				/// </summary>
				template<typename ParentLayout, bool Monochrome>
				class LettersEG
				{
				private:
					static constexpr dimension_t FontWidth = Layout::LogoLayout<ParentLayout>::Logo::FontWidth();
					static constexpr dimension_t FontHeight = Layout::LogoLayout<ParentLayout>::Logo::FontHeight();

					using LettersLayout = Layout::LogoLayout<ParentLayout>;

					using PrimitiveShaderType = Shader::Primitive::RetroLines<FontWidth, FontHeight,
						Shader::Source::RetroLinesColorAlpha<FontHeight, Monochrome>,
						Framework::Assets::Shader::Transform::Translate<dimension_t>
					>;

				public:
					Code::Font::LogoFontFamily::TemplateFont<FontWidth, FontHeight, PrimitiveShaderType> FontDrawer{};

				public:
					LettersEG() = default;
					~LettersEG() = default;

					void Draw(IFrameBuffer* frame)
					{
						FontDrawer.Draw(frame, LettersLayout::LetterE::X(), LettersLayout::LetterE::Y(), char(AsciiDefinition::Printable::E));
						FontDrawer.Draw(frame, LettersLayout::LetterG::X(), LettersLayout::LetterG::Y(), char(AsciiDefinition::Printable::G));
					}
				};

				/// <summary>
				/// Draws letters "FX" using Logo font with DotMatrix shader.
				/// </summary>
				template<typename ParentLayout, bool Monochrome>
				class LettersFX
				{
				private:
					static constexpr dimension_t FontWidth = Layout::LogoLayout<ParentLayout>::Logo::FontWidth();
					static constexpr dimension_t FontHeight = Layout::LogoLayout<ParentLayout>::Logo::FontHeight();

					using LettersLayout = Layout::LogoLayout<ParentLayout>;

					using PrimitiveShaderType = Shader::Primitive::DotMatrix < FontWidth, FontHeight,
						Shader::Source::DotMatrixRgbAlpha<FontWidth, FontHeight, Monochrome>,
						Framework::Assets::Shader::Transform::Translate<dimension_t>
					>;

				public:
					Code::Font::LogoFontFamily::TemplateFont<FontWidth, FontHeight, PrimitiveShaderType> FontDrawer{};

				public:
					LettersFX() = default;
					~LettersFX() = default;

					void Draw(IFrameBuffer* frame)
					{
						FontDrawer.Draw(frame, LettersLayout::LetterF::X(), LettersLayout::LetterF::Y(), char(AsciiDefinition::Printable::F));
						FontDrawer.Draw(frame, LettersLayout::LetterX::X(), LettersLayout::LetterX::Y(), char(AsciiDefinition::Printable::X));
					}
				};
			}
		}
	}
}
#endif