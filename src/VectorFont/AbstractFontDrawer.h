#ifndef _EGFX_VECTOR_FONT_ABSTRACT_FONT_DRAWER_h
#define _EGFX_VECTOR_FONT_ABSTRACT_FONT_DRAWER_h

#include "Model.h"
#include "../Model/IFrameBuffer.h"

#include "../VectorGraphics/Shaders.h"
#include "../VectorGraphics/TemplateShadeDecoder.h"
#include "../VectorGraphics/SingleColorVectorDrawer.h"


namespace Egfx
{
	/// <summary>
	/// Vector font utilities.
	/// - Glyphs are defined on an 8×8 local grid (VectorGraphics Model8) using compact vector nodes.
	/// - Rendering scales local coordinates to the configured font Width/Height.
	/// - KerningWidth is applied between glyphs (or space/tab), not after the last glyph.
	/// </summary>
	namespace VectorFont
	{
		template<typename VectorFontType,
			typename dimension_t = uint8_t,
			typename vector_count_t = uint_fast8_t,
			typename VectorShaderType = typename VectorGraphics::VectorShaders::SingleColorShader<dimension_t>>
			class AbstractFontDrawer
		{
		private:
			using VectorModel = typename VectorFontType::CharacterSetType::VectorModel;
			using axis_t = typename VectorModel::axis_t;

		protected:
			VectorGraphics::SingleColorVectorDrawer<VectorModel, dimension_t, vector_count_t, VectorShaderType> VectorDrawer;

		private:
			// Pre-scaled space and tab widths.
			dimension_t TabWidth;
			dimension_t SpaceWidth;

			// Pre-scaled kerning (pixels).
			dimension_t KerningWidth;

			// Kerning in scaled units.
			axis_t Kerning;

		public:
			AbstractFontDrawer(const dimension_t width = VectorModel::AxisMax,
				const dimension_t height = VectorModel::AxisMax,
				const axis_t kerning = 1)
				: VectorDrawer(width, height)
				, TabWidth(VectorModel::GetScaled(VectorDrawer.Width, VectorFontType::TabWidth))
				, SpaceWidth(VectorModel::GetScaled(VectorDrawer.Width, VectorFontType::SpaceWidth))
				, KerningWidth(GetScaledKerning(kerning))
				, Kerning(kerning)
			{
			}

			/// <summary>
			/// Set font pixel dimensions and recompute scaled widths.
			/// </summary>
			void SetFontDimensions(const dimension_t width, const dimension_t height)
			{
				VectorDrawer.Width = width;
				VectorDrawer.Height = height;
				TabWidth = VectorModel::GetScaled(VectorDrawer.Width, VectorFontType::TabWidth);
				SpaceWidth = VectorModel::GetScaled(VectorDrawer.Width, VectorFontType::SpaceWidth);
				KerningWidth = GetScaledKerning(Kerning);
			}

			void SetFontDimensions(const dimension_t width, const dimension_t height, const axis_t kerning)
			{
				Kerning = kerning;
				SetFontDimensions(width, height);
			}

			/// <summary>
			/// Set font pixel width (recomputes scaled widths).
			/// </summary>
			void SetFontWidth(const dimension_t width)
			{
				VectorDrawer.Width = width;
				TabWidth = VectorModel::GetScaled(VectorDrawer.Width, VectorFontType::TabWidth);
				SpaceWidth = VectorModel::GetScaled(VectorDrawer.Width, VectorFontType::SpaceWidth);
				KerningWidth = GetScaledKerning(Kerning);
			}

			/// <summary>
			/// Set font pixel height (does not affect horizontal advances).
			/// </summary>
			void SetFontHeight(const dimension_t height)
			{
				VectorDrawer.Height = height;
			}

			/// <summary>
			/// Set kerning in local units and recompute scaled kerning.
			/// </summary>
			void SetKerning(const axis_t kerning)
			{
				Kerning = kerning;
				KerningWidth = GetScaledKerning(kerning);
			}

			/// <summary>
			/// Get kerning in local vector model units.
			/// </summary>
			axis_t GetKerning() const
			{
				return Kerning;
			}

			/// <summary>
			/// Gets the pixel width of a space character.
			/// </summary>
			dimension_t GetSpaceWidth() const
			{
				return SpaceWidth;
			}

			/// <summary>
			/// Gets the pixel width of a tab character.
			/// </summary>
			dimension_t GetTabWidth() const
			{
				return TabWidth;
			}

			/// <summary>
			/// Gets the kerning pixel width.
			/// </summary>
			dimension_t GetKerningWidth() const
			{
				return KerningWidth;
			}

			/// <summary>
			/// Gets the pixel width of the font.
			/// </summary>
			dimension_t GetFontWidth() const
			{
				return VectorDrawer.Width;
			}

			/// <summary>
			/// Gets the pixel height of the font.
			/// </summary>
			dimension_t GetFontHeight() const
			{
				return VectorDrawer.Height;
			}

		private:
			dimension_t GetScaledKerning(const dimension_t kerning)
			{
				Kerning = kerning;

				// Scale kerning to font width.
				return Kerning <= 0 ? 0 : (1 + ((uint16_t(VectorDrawer.Width) * Kerning) >> 2) / (uint16_t(VectorModel::AxisMax)));
			}
		};
	}
}
#endif