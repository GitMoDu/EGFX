#ifndef _EGFX_FRAMEWORK_CODE_FONT_MODEL_h
#define _EGFX_FRAMEWORK_CODE_FONT_MODEL_h

#include "../Model.h"
#include "../../Text/Model.h"
#include "AbstractDrawer.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Code
		{
			/// <summary>
			/// Code-driven font rendering.
			///
			/// This namespace contains font drawers that procedurally render glyphs using shader primitives.
			/// The text framework (for example `Text::TemplateWriter`) handles layout, kerning, spacing, and
			/// alignment while delegating per-glyph rendering to a drawer in this namespace.
			/// </summary>
			namespace Font
			{
				namespace Contract
				{
					/// <summary>
					/// Font drawer contract for code-driven fonts.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="PrimitiveShaderType">Primitive shader type providing Prepare(...) and primitive draw calls.</typeparam>
					template<typename dimension_t,
						typename PrimitiveShaderType = Shader::Primitive::NoShader<dimension_t>
					>
					struct FontDrawer : AbstractDrawer<dimension_t, PrimitiveShaderType>
					{
						/// <summary>
						/// Optional glyph advance measurement for non-monospace fonts.
						/// </summary>
						/// <param name="printableChar">ASCII printable character to measure.</param>
						/// <returns>Character advance (pixels).</returns>
						virtual dimension_t Advance(const char /*printableChar*/) const
						{
							return 0;
						}

						/// <summary>
						/// Draws a single glyph and returns its advance.
						/// </summary>
						/// <param name="framebuffer">Target framebuffer to draw into.</param>
						/// <param name="x">Pixel-space X position (pixels).</param>
						/// <param name="y">Pixel-space Y position (pixels).</param>
						/// <param name="printableChar">ASCII printable character to draw.</param>
						/// <returns>Character advance (pixels).</returns>
						pixel_t Draw(IFrameBuffer* /*framebuffer*/, const pixel_t /*x*/, const pixel_t /*y*/, const char /*printableChar*/)
						{
							return 0;
						}
					};
				}
			}
		}
	}
}
#endif