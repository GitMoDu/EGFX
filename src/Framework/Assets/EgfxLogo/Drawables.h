#ifndef _EGFX_FRAMEWORK_ASSETS_EGFX_LOGO_DRAWABLES_h
#define _EGFX_FRAMEWORK_ASSETS_EGFX_LOGO_DRAWABLES_h

#include <EgfxDrawer.h>
#include "CodeFonts.h"
#include "Constants.h"
#include "Shaders.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace EgfxLogo
			{
				namespace Drawables
				{
					/// <summary>
					/// Draws screen border rectangle.
					/// </summary>
					template<typename Layout, bool Monochrome>
					struct Screen
					{
						pixel_point_t* Translation = nullptr;

						Shaders::Screen<CodeFonts::LogoFontFamily::Dimensions::font_dimension_t, Monochrome> Shader{};

						void Draw(IFrameBuffer* frame)
						{
							if (Translation != nullptr)
							{
								frame->Rectangle(Shader.ShadeColor(),
									Layout::X() + Translation->x, Layout::Y() + Translation->y,
									Layout::X() + Layout::Width() + Translation->x,
									Layout::Y() + Layout::Height() + Translation->y);
							}
							else
							{
								frame->Rectangle(Shader.ShadeColor(),
									Layout::X(), Layout::Y(),
									Layout::X() + Layout::Width(),
									Layout::Y() + Layout::Height());
							}
						}
					};

					using dimension_t = CodeFonts::LogoFontFamily::Dimensions::font_dimension_t;

					template<uint8_t FontHeight, bool Monochrome>
					using RetroLinesColorShaderType = Shaders::RetroLinesAlphaColor<dimension_t, FontHeight, Monochrome>;

					template<uint8_t FontWidth, uint8_t FontHeight, bool Monochrome>
					using RetroLinesPrimitiveShaderType = CodeFonts::LogoFontFamily::Shaders::RetroLines::PrimitiveShaderType<dimension_t, FontWidth, FontHeight, RetroLinesColorShaderType<FontHeight, Monochrome>>;

					template<uint8_t FontWidth, uint8_t FontHeight, bool Monochrome>
					using RetroLinesFontDrawerType = CodeFonts::LogoFontFamily::TemplateFont<FontWidth, FontHeight, RetroLinesPrimitiveShaderType<FontWidth, FontHeight, Monochrome>>;

					template<uint8_t FontWidth, uint8_t FontHeight, bool Monochrome>
					using DotMatrixColorShaderType = Shaders::DotMatrixDitherRgb<dimension_t, FontWidth, FontHeight, Monochrome>;

					template<uint8_t FontWidth, uint8_t FontHeight, bool Monochrome>
					using DotMatrixPrimitiveShaderType = CodeFonts::LogoFontFamily::Shaders::DotMatrix::PrimitiveShaderType<dimension_t, FontWidth, FontHeight, DotMatrixColorShaderType<FontWidth, FontHeight, Monochrome>>;

					template<uint8_t FontWidth, uint8_t FontHeight, bool Monochrome>
					using DotMatrixFontDrawerType = CodeFonts::LogoFontFamily::TemplateFont<FontWidth, FontHeight, DotMatrixPrimitiveShaderType<FontWidth, FontHeight, Monochrome>>;

					template<typename Layout, bool Monochrome>
					struct LetterE
					{
						pixel_point_t* Translation = nullptr;

						static constexpr auto FontWidth = Layout::Width();
						static constexpr auto FontHeight = Layout::Height();

						RetroLinesFontDrawerType<FontWidth, FontHeight, Monochrome>* FontDrawer = nullptr;

						void Draw(IFrameBuffer* frame)
						{
							if (FontDrawer != nullptr)
							{
								if (Translation != nullptr)
								{
									FontDrawer->Draw(frame, Layout::X() + Translation->x, Layout::Y() + Translation->y, char(AsciiDefinition::Printable::E));
								}
								else
								{
									FontDrawer->Draw(frame, Layout::X(), Layout::Y(), char(AsciiDefinition::Printable::E));
								}
							}
						}
					};

					template<typename Layout, bool Monochrome>
					struct LetterG
					{
						pixel_point_t* Translation = nullptr;

						static constexpr auto FontWidth = Layout::Width();
						static constexpr auto FontHeight = Layout::Height();

						RetroLinesFontDrawerType<FontWidth, FontHeight, Monochrome>* FontDrawer = nullptr;

						void Draw(IFrameBuffer* frame)
						{
							if (FontDrawer != nullptr)
							{
								if (Translation != nullptr)
								{
									FontDrawer->Draw(frame, Layout::X() + Translation->x, Layout::Y() + Translation->y, char(AsciiDefinition::Printable::G));
								}
								else
								{
									FontDrawer->Draw(frame, Layout::X(), Layout::Y(), char(AsciiDefinition::Printable::G));
								}
							}
						}
					};

					template<typename Layout, bool Monochrome>
					struct LetterF
					{
						pixel_point_t* Translation = nullptr;

						static constexpr auto FontWidth = Layout::Width();
						static constexpr auto FontHeight = Layout::Height();

						DotMatrixFontDrawerType<FontWidth, FontHeight, Monochrome>* FontDrawer = nullptr;

						void Draw(IFrameBuffer* frame)
						{
							if (FontDrawer != nullptr)
							{
								if (Translation != nullptr)
								{
									FontDrawer->Draw(frame, Layout::X() + Translation->x, Layout::Y() + Translation->y, char(AsciiDefinition::Printable::F));
								}
								else
								{
									FontDrawer->Draw(frame, Layout::X(), Layout::Y(), char(AsciiDefinition::Printable::F));
								}
							}
						}
					};

					template<typename Layout, bool Monochrome>
					struct LetterX
					{
						pixel_point_t* Translation = nullptr;

						static constexpr auto FontWidth = Layout::Width();
						static constexpr auto FontHeight = Layout::Height();

						DotMatrixFontDrawerType<FontWidth, FontHeight, Monochrome>* FontDrawer = nullptr;

						void Draw(IFrameBuffer* frame)
						{
							if (FontDrawer != nullptr)
							{
								if (Translation != nullptr)
								{
									FontDrawer->Draw(frame, Layout::X() + Translation->x, Layout::Y() + Translation->y, char(AsciiDefinition::Printable::X));
								}
								else
								{
									FontDrawer->Draw(frame, Layout::X(), Layout::Y(), char(AsciiDefinition::Printable::X));
								}
							}
						}
					};
				}
			}
		}
	}
}
#endif