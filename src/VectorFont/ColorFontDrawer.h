#ifndef _EGFX_VECTOR_FONT_COLOR_FONT_DRAWER_h
#define _EGFX_VECTOR_FONT_COLOR_FONT_DRAWER_h

#include "AbstractFontDrawer.h"
#include "../Text/FontText.h"

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
			typename FontColorSourceType = FontText::SingleColorSource,
			typename DimensionType = uint8_t,
			typename vector_count_t = uint_fast8_t,
			typename VectorShaderType = typename VectorGraphics::VectorShaders::SingleColorShader<DimensionType>>
			class TemplateColorFontDrawer : public AbstractFontDrawer<VectorFontType, DimensionType, vector_count_t, VectorShaderType>
		{
		public:
			using dimension_t = DimensionType;

		private:
			using Base = AbstractFontDrawer<VectorFontType, dimension_t, vector_count_t, VectorShaderType>;
			using VectorModel = typename VectorFontType::CharacterSetType::VectorModel;
			using axis_t = typename VectorModel::axis_t;

		protected:
			using Base::VectorDrawer;

		public:
			FontColorSourceType FontColor{};

		public:
			TemplateColorFontDrawer(const dimension_t width = VectorModel::AxisMax,
				const dimension_t height = VectorModel::AxisMax,
				const axis_t kerning = 1)
				: Base(width, height, kerning)
			{
			}

			pixel_t Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char printableChar)
			{
				const auto set = VectorFontType::GetCharacterSet(printableChar);
				VectorDrawer.DrawVectors(framebuffer, set.Nodes, set.Count, x, y, FontColor.GetColor());

				return VectorModel::GetScaled(VectorDrawer.Width, set.Width) + 1;
			}

			// Get only character advance.
			pixel_t Advance(const char printableChar) const
			{
				const auto set = VectorFontType::GetCharacterSet(printableChar);

				return VectorModel::GetScaled(VectorDrawer.Width, set.Width) + 1;
			}
		};
	}
}
#endif