#ifndef _EGFX_FRAMEWORK_CODE_FONT_ABSTRACT_DRAWER_h
#define _EGFX_FRAMEWORK_CODE_FONT_ABSTRACT_DRAWER_h

#include "Model.h"
#include "../../Shader/Primitive/Shaders.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Code
		{
			namespace Font
			{
				/// <summary>
				/// Base class for code-driven font drawers.
				/// Holds common font metrics (width, height, kerning) and provides default monospace
				/// spacing behavior (advance/space/tab).
				///
				/// Derived font drawers must implement:
				/// - pixel_t Draw(IFrameBuffer*, pixel_t x, pixel_t y, char printableChar)
				///   which calls the inherited pixel shader Prepare(x, y) and then renders the glyph
				///   using shader primitives (Pixel/Line/RectangleFill).
				///
				/// The SetFontWidth/SetFontHeight methods are virtual so derived classes can
				/// recompute cached/intermediate metrics (e.g. midpoints) when the font size changes.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				/// <typeparam name="PrimitiveShaderType">Primitive shader type providing Prepare(...) and primitive draw calls.</typeparam>
				template<typename dimension_t,
					typename PrimitiveShaderType = Shader::Primitive::TemplateShader<dimension_t>
				>
				class AbstractDrawer : public PrimitiveShaderType
				{
				private:
					using Base = PrimitiveShaderType;

				protected:
					/// <summary>Nominal glyph width in pixels (used by monospace defaults).</summary>
					dimension_t Width;

					/// <summary>Nominal glyph height in pixels.</summary>
					dimension_t Height;

					/// <summary>Additional horizontal spacing between glyphs in pixels.</summary>
					dimension_t KerningWidth = 1;

				public:
					/// <summary>
					/// Constructs the drawer with initial font metrics.
					/// </summary>
					/// <param name="width">Nominal glyph width in pixels.</param>
					/// <param name="height">Nominal glyph height in pixels.</param>
					/// <param name="kerningWidth">Extra spacing between glyphs in pixels.</param>
					AbstractDrawer(const dimension_t width, const dimension_t height, const dimension_t kerningWidth)
						: Base()
						, Width(width)
						, Height(height)
						, KerningWidth(kerningWidth)
					{
					}

					~AbstractDrawer() = default;

					/// <summary>
					/// Returns the horizontal advance for a glyph.
					/// Default is monospace: returns the nominal glyph width.
					/// </summary>
					/// <param name="printableChar">ASCII printable character to measure.</param>
					/// <returns>Glyph advance (pixels).</returns>
					virtual dimension_t Advance(const char /*printableChar*/) const
					{
						return Width;
					}

					/// <summary>
					/// Example glyph rendering implementation.
					/// Demonstrates preparing the primitive shader and drawing with primitives.
					/// </summary>
					/*pixel_t Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char printableChar)
					{
						Base::Prepare(x, y);
						Base::Line(1, 1, Width -1, Height - 1);
						return Width;
					}*/

					//// <summary>
					/// Returns the width of a space character (pixels).
					/// Default equals the nominal glyph width.
					/// </summary>
					virtual dimension_t GetSpaceWidth() const
					{
						return Width;
					}

					/// <summary>
					/// Returns the width of a tab character (pixels).
					/// Default is 3 spaces.
					/// </summary>
					virtual dimension_t GetTabWidth() const
					{
						return Width * 3;
					}

					/// <summary>
					/// Sets the nominal glyph width (pixels).
					/// Virtual so derived drawers can update cached/intermediate metrics.
					/// </summary>
					virtual void SetFontWidth(const dimension_t width)
					{
						Width = width;
					}

					/// <summary>
					/// Sets the nominal glyph height (pixels).
					/// Virtual so derived drawers can update cached/intermediate metrics.
					/// </summary>
					virtual void SetFontHeight(const dimension_t height)
					{
						Height = height;
					}

					/// <summary>Sets extra spacing between glyphs (pixels).</summary>
					void SetKerningWidth(const dimension_t kerningWidth)
					{
						KerningWidth = kerningWidth;
					}

					/// <summary>Sets the nominal glyph width and height (pixels).</summary>
					void SetFontDimensions(const dimension_t width, const dimension_t height)
					{
						SetFontWidth(width);
						SetFontHeight(height);
					}

					/// <summary>Sets the nominal glyph width/height and kerning (pixels).</summary>
					void SetFontDimensions(const dimension_t width, const dimension_t height, const dimension_t kerningWidth)
					{
						SetFontWidth(width);
						SetFontHeight(height);
						SetKerningWidth(kerningWidth);
					}

					/// <summary>Gets the nominal glyph width (pixels).</summary>
					dimension_t GetFontWidth() const
					{
						return Width;
					}

					/// <summary>Gets the nominal glyph height (pixels).</summary>
					dimension_t GetFontHeight() const
					{
						return Height;
					}

					/// <summary>Gets extra spacing between glyphs (pixels).</summary>
					dimension_t GetKerningWidth() const
					{
						return KerningWidth;
					}
				};
			}
		}
	}
}
#endif