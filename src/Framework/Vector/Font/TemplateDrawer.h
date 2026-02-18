#ifndef _EGFX_FRAMEWORK_VECTOR_FONT_TEMPLATE_DRAWER_h
#define _EGFX_FRAMEWORK_VECTOR_FONT_TEMPLATE_DRAWER_h

#include "Model.h"

#include "../../Text/Model.h"
#include "../../Shader/Primitive/Shaders.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Vector
		{
			namespace Font
			{
				/// <summary>
				/// Vector font drawer.
				///
				/// Wraps a vector font definition and renders glyph node streams via the generic vector stream drawer.
				/// Provides spacing metrics (space/tab/kerning) in pixel space for use by the text framework.
				/// </summary>
				/// <typeparam name="VectorFontType">Vector font type providing CharacterSetType, dimension_t, SpaceWidth, TabWidth, and GetCharacterSet().</typeparam>
				/// <typeparam name="vector_count_t">Vector node index type.</typeparam>
				/// <typeparam name="PrimitiveShaderType">Primitive shader type providing Prepare(...) and primitive draw calls.</stypeparam>
				template<typename VectorFontType,
					typename vector_count_t = uint_fast8_t,
					typename PrimitiveShaderType = Shader::Primitive::TemplateShader<typename VectorFontType::dimension_t>
				>
				class TemplateDrawer : public Vector::TemplateDrawer<
					typename VectorFontType::CharacterSetType::VectorModel,
					typename VectorFontType::dimension_t,
					vector_count_t,
					PrimitiveShaderType>
				{
				private:
					using Base = Vector::TemplateDrawer<
						typename VectorFontType::CharacterSetType::VectorModel,
						typename VectorFontType::dimension_t,
						vector_count_t,
						PrimitiveShaderType>;

				public:
					/// <summary>The shader's intrinsic dimension type.</summary>
					using dimension_t = typename VectorFontType::dimension_t;

				private:
					using VectorModel = typename VectorFontType::CharacterSetType::VectorModel;
					using axis_t = typename VectorModel::axis_t;

				private:
					/// <summary>Scaled tab width (pixels).</summary>
					dimension_t TabWidth;

					/// <summary>Scaled space width (pixels).</summary>
					dimension_t SpaceWidth;

					/// <summary>Scaled kerning (pixels).</summary>
					dimension_t KerningWidth;

					/// <summary>Kerning in local vector model units.</summary>
					axis_t Kerning;

				public:
					/// <summary>
					/// Constructs the drawer and computes scaled spacing metrics.
					/// </summary>
					/// <param name="width">Target glyph width used when scaling local-space X coordinates (pixels).</param>
					/// <param name="height">Target glyph height used when scaling local-space Y coordinates (pixels).</param>
					/// <param name="kerning">Kerning in local vector model units.</param>
					TemplateDrawer(const dimension_t width = VectorModel::AxisMax,
						const dimension_t height = VectorModel::AxisMax,
						const axis_t kerning = 1)
						: Base(width, height)
						, TabWidth(VectorModel::GetScaled(width, VectorFontType::TabWidth))
						, SpaceWidth(VectorModel::GetScaled(width, VectorFontType::SpaceWidth))
						, KerningWidth(GetScaledKerning(kerning))
						, Kerning(kerning)
					{
					}

					~TemplateDrawer() = default;

					/// <summary>
					/// Draws a single glyph and returns its advance.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="x">Pixel-space origin X (pixels).</param>
					/// <param name="y">Pixel-space origin Y (pixels).</param>
					/// <param name="printableChar">ASCII printable character to draw.</param>
					/// <returns>Glyph advance (pixels).</returns>
					pixel_t Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char printableChar)
					{
						const auto set = VectorFontType::GetCharacterSet(printableChar);
						Base::Draw(framebuffer, x, y, set.Nodes, set.Count);

						// Advance is the scaled glyph width plus a trailing pixel for spacing.
						return VectorModel::GetScaled(Base::Width, set.Width) + 1;
					}

					/// <summary>
					/// Returns the glyph advance without drawing.
					/// </summary>
					/// <param name="printableChar">ASCII printable character to measure.</param>
					/// <returns>Glyph advance (pixels).</returns>
					pixel_t Advance(const char printableChar) const
					{
						const auto set = VectorFontType::GetCharacterSet(printableChar);
						return VectorModel::GetScaled(Base::Width, set.Width) + 1;
					}

					/// <summary>
					/// Sets font pixel dimensions and recomputes scaled spacing metrics.
					/// </summary>
					/// <param name="width">Target glyph width used when scaling local-space X coordinates (pixels).</param>
					/// <param name="height">Target glyph height used when scaling local-space Y coordinates (pixels).</param>
					void SetFontDimensions(const dimension_t width, const dimension_t height)
					{
						Base::Width = width;
						Base::Height = height;
						TabWidth = VectorModel::GetScaled(Base::Width, VectorFontType::TabWidth);
						SpaceWidth = VectorModel::GetScaled(Base::Width, VectorFontType::SpaceWidth);
						KerningWidth = GetScaledKerning(Kerning);
					}

					/// <summary>
					/// Sets font pixel dimensions and kerning (local units).
					/// </summary>
					/// <param name="width">Target glyph width used when scaling local-space X coordinates (pixels).</param>
					/// <param name="height">Target glyph height used when scaling local-space Y coordinates (pixels).</param>
					/// <param name="kerning">Kerning in local vector model units.</param>
					void SetFontDimensions(const dimension_t width, const dimension_t height, const axis_t kerning)
					{
						Kerning = kerning;
						SetFontDimensions(width, height);
					}

					/// <summary>
					/// Sets the font pixel width and recomputes scaled spacing metrics.
					/// </summary>
					/// <param name="width">Target glyph width used when scaling local-space X coordinates (pixels).</param>
					void SetFontWidth(const dimension_t width)
					{
						Base::Width = width;
						TabWidth = VectorModel::GetScaled(Base::Width, VectorFontType::TabWidth);
						SpaceWidth = VectorModel::GetScaled(Base::Width, VectorFontType::SpaceWidth);
						KerningWidth = GetScaledKerning(Kerning);
					}

					/// <summary>
					/// Sets the font pixel height.
					/// </summary>
					/// <param name="height">Target glyph height used when scaling local-space Y coordinates (pixels).</param>
					void SetFontHeight(const dimension_t height)
					{
						Base::Height = height;
					}

					/// <summary>
					/// Sets kerning (local units) and recomputes scaled kerning.
					/// </summary>
					/// <param name="kerning">Kerning in local vector model units.</param>
					void SetKerning(const axis_t kerning)
					{
						Kerning = kerning;
						KerningWidth = GetScaledKerning(kerning);
					}

					/// <summary>
					/// Gets kerning in local vector model units.
					/// </summary>
					axis_t GetKerning() const
					{
						return Kerning;
					}

					/// <summary>Gets the space width (pixels).</summary>
					dimension_t GetSpaceWidth() const { return SpaceWidth; }

					/// <summary>Gets the tab width (pixels).</summary>
					dimension_t GetTabWidth() const { return TabWidth; }

					/// <summary>Gets kerning (pixels).</summary>
					dimension_t GetKerningWidth() const { return KerningWidth; }

					/// <summary>Gets the font width used for scaling (pixels).</summary>
					dimension_t GetFontWidth() const { return Base::Width; }

					/// <summary>Gets the font height used for scaling (pixels).</summary>
					dimension_t GetFontHeight() const { return Base::Height; }

				private:
					/// <summary>
					/// Scales kerning from local vector model units into pixels.
					/// </summary>
					dimension_t GetScaledKerning(const dimension_t kerning)
					{
						Kerning = kerning;

						// Scale kerning to font width.
						return Kerning <= 0 ? 0 : (1 + ((uint16_t(Base::Width) * Kerning) >> 2) / (uint16_t(VectorModel::AxisMax)));
					}
				};
			}
		}
	}
}
#endif