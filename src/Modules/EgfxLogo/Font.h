#ifndef _EGFX_MODULES_EGFX_LOGO_CODE_FONT_h
#define _EGFX_MODULES_EGFX_LOGO_CODE_FONT_h

#include "Constant.h"

namespace Egfx
{
	namespace Modules
	{
		namespace EgfxLogo
		{
			namespace Code
			{
				namespace Font
				{
					namespace LogoFontFamily
					{
						namespace AsciiDefinition = Framework::AsciiDefinition;
						using dimension_t = Dimensions::dimension_t;

						/// <summary>
						/// Custom monospaced font for the Egfx logo.
						/// </summary>
						/// <typeparam name="PrimitiveShaderType">The shader type to use for rendering primitives. Defaults to NoShader if not specified.</typeparam>
						/// <typeparam name="FontWidth">The width of each character in pixels.</typeparam>
						/// <typeparam name="FontHeight">The height of each character in pixels.</typeparam>
						template<dimension_t FontWidth, dimension_t FontHeight,
							typename PrimitiveShaderType = Framework::Shader::Primitive::TemplateShader<dimension_t>
						>
						struct TemplateFont : Framework::Code::Font::AbstractDrawer<dimension_t, PrimitiveShaderType>
						{
						private:
							using Base = Framework::Code::Font::AbstractDrawer<dimension_t, PrimitiveShaderType>;

						private:
							static constexpr dimension_t LineHeight = FontHeight / Dimensions::LineCount;
							static constexpr dimension_t ColumnWidth = FontWidth / Dimensions::ColumnCount;
							static constexpr dimension_t GShift = ColumnWidth / 2;

						public:
							PrimitiveShaderType PrimitiveShader{};

						public:
							TemplateFont() : Base(FontWidth, FontHeight, 1)
							{
							}

							~TemplateFont() = default;

							// Get only character advance.
							dimension_t Advance(const char /*printableChar*/) const
							{
								// Monospaced font.
								return FontWidth;
							}

							dimension_t Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char printableChar)
							{
								Base::Prepare(x, y);

								switch (static_cast<uint8_t>(printableChar))
								{
								case uint8_t(AsciiDefinition::Printable::E):
									Base::RectangleFill(framebuffer, 0, 0, FontWidth, LineHeight);
									Base::RectangleFill(framebuffer, 0, LineHeight, ColumnWidth * 2, LineHeight * 2);
									Base::RectangleFill(framebuffer, 0, LineHeight * 2, FontWidth - ColumnWidth * 1, LineHeight * 3);
									Base::RectangleFill(framebuffer, 0, LineHeight * 3, ColumnWidth * 2, LineHeight * 4);
									Base::RectangleFill(framebuffer, 0, LineHeight * 4, FontWidth, LineHeight * 5);
									break;
								case uint8_t(AsciiDefinition::Printable::F):
									Base::RectangleFill(framebuffer, 0, 0, FontWidth, LineHeight);
									Base::RectangleFill(framebuffer, 0, LineHeight, ColumnWidth * 2, LineHeight * 2);
									Base::RectangleFill(framebuffer, 0, LineHeight * 2, FontWidth - ColumnWidth, LineHeight * 3);
									Base::RectangleFill(framebuffer, 0, LineHeight * 3, ColumnWidth * 2, LineHeight * 4);
									Base::RectangleFill(framebuffer, 0, LineHeight * 4, ColumnWidth * 2, LineHeight * 5);
									break;
								case uint8_t(AsciiDefinition::Printable::G):
									Base::RectangleFill(framebuffer, GShift, 0, FontWidth - GShift, LineHeight);
									Base::RectangleFill(framebuffer, 0, LineHeight, ColumnWidth * 2, LineHeight * 2);
									Base::RectangleFill(framebuffer, 0, LineHeight * 2, ColumnWidth * 2, LineHeight * 3);
									Base::RectangleFill(framebuffer, FontWidth - ColumnWidth * 3, LineHeight * 2 - 1, FontWidth, LineHeight * 3 - 1);
									Base::RectangleFill(framebuffer, 0, LineHeight * 3, ColumnWidth * 2, LineHeight * 4);
									Base::RectangleFill(framebuffer, FontWidth - ColumnWidth * 2, LineHeight * 3, FontWidth, LineHeight * 4);
									Base::RectangleFill(framebuffer, GShift, LineHeight * 4, FontWidth - GShift, LineHeight * 5);
									break;
								case uint8_t(AsciiDefinition::Printable::X):
									Base::RectangleFill(framebuffer, 0, 0, ColumnWidth * 2, LineHeight);
									Base::RectangleFill(framebuffer, FontWidth - ColumnWidth * 2, 0, FontWidth, LineHeight);
									Base::RectangleFill(framebuffer, ColumnWidth, LineHeight * 1, ColumnWidth * 3, LineHeight * 2);
									Base::RectangleFill(framebuffer, FontWidth - ColumnWidth * 3, LineHeight * 1, FontWidth - ColumnWidth, LineHeight * 2);
									Base::RectangleFill(framebuffer, ColumnWidth * 2, LineHeight * 2, FontWidth - ColumnWidth * 2, LineHeight * 3);
									Base::RectangleFill(framebuffer, FontWidth - ColumnWidth * 3, LineHeight * 3, FontWidth - ColumnWidth, LineHeight * 4);
									Base::RectangleFill(framebuffer, ColumnWidth, LineHeight * 3, ColumnWidth * 3, LineHeight * 4);
									Base::RectangleFill(framebuffer, 0, LineHeight * 4, ColumnWidth * 2, LineHeight * 5);
									Base::RectangleFill(framebuffer, FontWidth - ColumnWidth * 2, LineHeight * 4, FontWidth, LineHeight * 5);
									break;
								default:
									break;
								};

								// Monospaced font.
								return FontWidth;
							}
						};
					}
				}
			}
		}
	}
}
#endif