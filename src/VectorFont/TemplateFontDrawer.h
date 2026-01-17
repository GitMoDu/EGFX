#ifndef _EGFX_VECTOR_FONT_TEMPLATE_FONT_DRAWER_h
#define _EGFX_VECTOR_FONT_TEMPLATE_FONT_DRAWER_h

#include "Model.h"
#include "../Model/IFrameBuffer.h"

#include "../VectorGraphics/Shaders.h"
#include "../VectorGraphics/TemplateVectorDrawer.h"

namespace Egfx
{
	namespace VectorFont
	{
		template<typename VectorFontType,
			typename font_dimension_t = uint8_t,
			typename vector_count_t = uint_fast8_t,
			typename VectorShaderType = typename VectorGraphics::Shaders::Primitive::SingleColor<font_dimension_t>
		>
		class TemplateFontDrawer : public VectorGraphics::TemplateVectorDrawer<
			typename VectorFontType::CharacterSetType::VectorModel,
			font_dimension_t,
			vector_count_t,
			VectorShaderType>
		{
		public:
			using dimension_t = font_dimension_t;

		private:
			using VectorModel = typename VectorFontType::CharacterSetType::VectorModel;

			using Base = VectorGraphics::TemplateVectorDrawer<VectorModel, dimension_t, vector_count_t, VectorShaderType>;

			using axis_t = typename VectorModel::axis_t;

		private:
			// Pre-scaled space and tab widths.
			dimension_t TabWidth;
			dimension_t SpaceWidth;

			// Pre-scaled kerning (pixels).
			dimension_t KerningWidth;

			// Kerning in scaled units.
			axis_t Kerning;

		public:
			TemplateFontDrawer(const dimension_t width = VectorModel::AxisMax,
				const dimension_t height = VectorModel::AxisMax,
				const axis_t kerning = 1)
				: Base(width, height)
				, TabWidth(VectorModel::GetScaled(width, VectorFontType::TabWidth))
				, SpaceWidth(VectorModel::GetScaled(width, VectorFontType::SpaceWidth))
				, KerningWidth(GetScaledKerning(kerning))
				, Kerning(kerning)
			{
			}

			pixel_t Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char printableChar)
			{
				const auto set = VectorFontType::GetCharacterSet(printableChar);
				Base::DrawVectors(framebuffer, set.Nodes, set.Count, x, y);

				return VectorModel::GetScaled(Base::Width, set.Width) + 1;
			}

			// Get only character advance.
			pixel_t Advance(const char printableChar) const
			{
				const auto set = VectorFontType::GetCharacterSet(printableChar);

				return VectorModel::GetScaled(Base::Width, set.Width) + 1;
			}

			/// <summary>
			/// Set font pixel dimensions and recompute scaled widths.
			/// </summary>
			void SetFontDimensions(const dimension_t width, const dimension_t height)
			{
				Base::Width = width;
				Base::Height = height;
				TabWidth = VectorModel::GetScaled(Base::Width, VectorFontType::TabWidth);
				SpaceWidth = VectorModel::GetScaled(Base::Width, VectorFontType::SpaceWidth);
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
				Base::Width = width;
				TabWidth = VectorModel::GetScaled(Base::Width, VectorFontType::TabWidth);
				SpaceWidth = VectorModel::GetScaled(Base::Width, VectorFontType::SpaceWidth);
				KerningWidth = GetScaledKerning(Kerning);
			}

			/// <summary>
			/// Set font pixel height (does not affect horizontal advances).
			/// </summary>
			void SetFontHeight(const dimension_t height)
			{
				Base::Height = height;
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
				return Base::Width;
			}

			/// <summary>
			/// Gets the pixel height of the font.
			/// </summary>
			dimension_t GetFontHeight() const
			{
				return Base::Height;
			}

		private:
			dimension_t GetScaledKerning(const dimension_t kerning)
			{
				Kerning = kerning;

				// Scale kerning to font width.
				return Kerning <= 0 ? 0 : (1 + ((uint16_t(Base::Width) * Kerning) >> 2) / (uint16_t(VectorModel::AxisMax)));
			}
		};
	}
}
#endif