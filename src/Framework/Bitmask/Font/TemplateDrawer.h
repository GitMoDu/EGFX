#ifndef _EGFX_FRAMEWORK_BITMASK_FONT_TEMPLATE_DRAWER_h
#define _EGFX_FRAMEWORK_BITMASK_FONT_TEMPLATE_DRAWER_h

#include "Model.h"
#include "../../Shader/Primitive/Shaders.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Bitmask
		{
			namespace Font
			{
				/// <summary>
				/// Bitmask font drawer (no scaling).
				///
				/// Turns a bitmask font definition into a glyph drawer compatible with the text framework.
				/// Draws glyphs by delegating to Bitmask::TemplateDrawer using the provided primitive shader.
				/// </summary>
				/// <typeparam name="BitmaskFontType">Bitmask font type providing dimension_t, Width, Height, ReaderType, and GetCharacter().</typeparam>
				/// <typeparam name="PrimitiveShaderType">Primitive shader type providing Prepare(...) and primitive draw calls.</typeparam>
				template<typename BitmaskFontType,
					typename PrimitiveShaderType = Shader::Primitive::NoShader<typename BitmaskFontType::dimension_t>
				>
				class TemplateDrawerNoScale : public Bitmask::TemplateDrawer<
					typename BitmaskFontType::dimension_t,
					BitmaskFontType::Width,
					BitmaskFontType::Height,
					typename BitmaskFontType::ReaderType,
					1, 1,
					PrimitiveShaderType>
				{
				private:
					using Base = Bitmask::TemplateDrawer<
						typename BitmaskFontType::dimension_t,
						BitmaskFontType::Width,
						BitmaskFontType::Height,
						typename BitmaskFontType::ReaderType,
						1, 1,
						PrimitiveShaderType>;

				public:
					/// <summary>The shader's intrinsic dimension type.</summary>
					using dimension_t = typename BitmaskFontType::dimension_t;

				protected:
					/// <summary>Tab width override (pixels).</summary>
					dimension_t TabWidth;

					/// <summary>Space width override (pixels).</summary>
					dimension_t SpaceWidth;

					/// <summary>Extra spacing between glyphs (pixels).</summary>
					dimension_t KerningWidth;

				public:
					/// <summary>
					/// Constructs the drawer with spacing metrics.
					/// </summary>
					/// <param name="tabWidth">Tab width (pixels).</param>
					/// <param name="spaceWidth">Space width (pixels).</param>
					/// <param name="kerning">Extra spacing between glyphs (pixels).</param>
					TemplateDrawerNoScale(const dimension_t tabWidth = 5,
						const dimension_t spaceWidth = 3,
						const dimension_t kerning = 1)
						: Base()
						, TabWidth(tabWidth), SpaceWidth(spaceWidth), KerningWidth(kerning)
					{
					}

					~TemplateDrawerNoScale() = default;

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
						const auto character = BitmaskFontType::GetCharacter(printableChar);

						if (character.Advance > 0)
						{
							Base::Draw(framebuffer, x, y, character.Bitmask);
							return character.Advance;
						}

						return 0;
					}

					/// <summary>
					/// Returns the glyph advance without drawing.
					/// </summary>
					/// <param name="printableChar">ASCII printable character to measure.</param>
					/// <returns>Glyph advance (pixels).</returns>
					dimension_t Advance(const char printableChar) const
					{
						const auto character = BitmaskFontType::GetCharacter(printableChar);
						return character.Advance;
					}

					/// <summary>Gets the space width (pixels).</summary>
					dimension_t GetSpaceWidth() const { return SpaceWidth; }

					/// <summary>Gets the tab width (pixels).</summary>
					dimension_t GetTabWidth() const { return TabWidth; }

					/// <summary>Gets extra spacing between glyphs (pixels).</summary>
					dimension_t GetKerningWidth() const { return KerningWidth; }

					/// <summary>Sets extra spacing between glyphs (pixels).</summary>
					void SetKerningWidth(const dimension_t kerning) { KerningWidth = kerning; }

					/// <summary>Sets the space width (pixels).</summary>
					void SetSpaceWidth(const dimension_t spaceWidth) { SpaceWidth = spaceWidth; }

					/// <summary>Sets the tab width (pixels).</summary>
					void SetTabWidth(const dimension_t tabWidth) { TabWidth = tabWidth; }

					/// <summary>Gets the nominal glyph width (pixels).</summary>
					static constexpr dimension_t GetFontWidth() { return BitmaskFontType::Width; }

					/// <summary>Gets the nominal glyph height (pixels).</summary>
					static constexpr dimension_t GetFontHeight() { return BitmaskFontType::Height; }
				};

				/// <summary>
				/// Bitmask font drawer (integer scaling).
				///
				/// Draws glyphs by delegating to Bitmask::TemplateDrawer with integer scaling. Advance and
				/// spacing metrics are scaled in X (and height in Y).
				/// </summary>
				/// <typeparam name="BitmaskFontType">Bitmask font type providing dimension_t, Width, Height, ReaderType, and GetCharacter().</typeparam>
				/// <typeparam name="ScaleX">Horizontal scale factor (integer).</typeparam>
				/// <typeparam name="ScaleY">Vertical scale factor (integer).</typeparam>
				/// <typeparam name="PrimitiveShaderType">Primitive shader type providing Prepare(...) and primitive draw calls.</typeparam>
				template<typename BitmaskFontType,
					uint8_t ScaleX = 1,
					uint8_t ScaleY = ScaleX,
					typename PrimitiveShaderType = Shader::Primitive::NoShader<typename BitmaskFontType::dimension_t>
				>
				class TemplateDrawerIntegerScale : public Bitmask::TemplateDrawer<
					typename BitmaskFontType::dimension_t,
					BitmaskFontType::Width,
					BitmaskFontType::Height,
					typename BitmaskFontType::ReaderType,
					ScaleX,
					ScaleY,
					PrimitiveShaderType
				>
				{
				private:
					using Base = Bitmask::TemplateDrawer<
						typename BitmaskFontType::dimension_t,
						BitmaskFontType::Width,
						BitmaskFontType::Height,
						typename BitmaskFontType::ReaderType,
						ScaleX,
						ScaleY,
						PrimitiveShaderType
					>;

				public:
					/// <summary>The shader's intrinsic dimension type.</summary>
					using dimension_t = typename BitmaskFontType::dimension_t;

				protected:
					/// <summary>Tab width override (pixels).</summary>
					dimension_t TabWidth;

					/// <summary>Space width override (pixels).</summary>
					dimension_t SpaceWidth;

					/// <summary>Extra spacing between glyphs (pixels).</summary>
					dimension_t KerningWidth;

				public:
					/// <summary>
					/// Constructs the drawer with scaled spacing metrics.
					/// </summary>
					/// <param name="tabWidth">Tab width (pixels).</param>
					/// <param name="spaceWidth">Space width (pixels).</param>
					/// <param name="kerning">Extra spacing between glyphs (pixels).</param>
					TemplateDrawerIntegerScale(const dimension_t tabWidth = 5 * ScaleX,
						const dimension_t spaceWidth = 3 * ScaleX,
						const dimension_t kerning = 1 * ScaleX)
						: Base()
						, TabWidth(tabWidth), SpaceWidth(spaceWidth), KerningWidth(kerning)
					{
					}

					~TemplateDrawerIntegerScale() = default;

					/// <summary>
					/// Draws a single glyph and returns its scaled advance.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="x">Pixel-space origin X (pixels).</param>
					/// <param name="y">Pixel-space origin Y (pixels).</param>
					/// <param name="printableChar">ASCII printable character to draw.</param>
					/// <returns>Glyph advance (pixels).</returns>
					pixel_t Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char printableChar)
					{
						const auto character = BitmaskFontType::GetCharacter(printableChar);

						if (character.Advance > 0)
						{
							Base::Draw(framebuffer, x, y, character.Bitmask);
							return character.Advance * ScaleX;
						}

						return 0;
					}

					/// <summary>
					/// Returns the scaled glyph advance without drawing.
					/// </summary>
					/// <param name="printableChar">ASCII printable character to measure.</param>
					/// <returns>Glyph advance (pixels).</returns>
					dimension_t Advance(const char printableChar) const
					{
						const auto character = BitmaskFontType::GetCharacter(printableChar);
						return character.Advance * ScaleX;
					}

					/// <summary>Gets the space width (pixels).</summary>
					dimension_t GetSpaceWidth() const { return SpaceWidth; }

					/// <summary>Gets the tab width (pixels).</summary>
					dimension_t GetTabWidth() const { return TabWidth; }

					/// <summary>Gets extra spacing between glyphs (pixels).</summary>
					dimension_t GetKerningWidth() const { return KerningWidth; }

					/// <summary>Sets extra spacing between glyphs (pixels).</summary>
					void SetKerningWidth(const dimension_t kerning) { KerningWidth = kerning; }

					/// <summary>Sets the space width (pixels).</summary>
					void SetSpaceWidth(const dimension_t spaceWidth) { SpaceWidth = spaceWidth; }

					/// <summary>Sets the tab width (pixels).</summary>
					void SetTabWidth(const dimension_t tabWidth) { TabWidth = tabWidth; }

					/// <summary>Gets the nominal glyph width (pixels).</summary>
					static constexpr dimension_t GetFontWidth() { return BitmaskFontType::Width * ScaleX; }

					/// <summary>Gets the nominal glyph height (pixels).</summary>
					static constexpr dimension_t GetFontHeight() { return BitmaskFontType::Height * ScaleY; }
				};

				/// <summary>
				/// Bitmask font drawer alias that selects the non-scaled or scaled implementation.
				/// </summary>
				/// <typeparam name="BitmaskFontType">Bitmask font type providing dimension_t, Width, Height, ReaderType, and GetCharacter().</typeparam>
				/// <typeparam name="ScaleX">Horizontal scale factor (integer).</typeparam>
				/// <typeparam name="ScaleY">Vertical scale factor (integer).</typeparam>
				/// <typeparam name="PrimitiveShaderType">Primitive shader type providing Prepare(...) and primitive draw calls.</typeparam>
				template<typename BitmaskFontType,
					uint8_t ScaleX = 1,
					uint8_t ScaleY = ScaleX,
					typename PrimitiveShaderType = Shader::Primitive::NoShader<typename BitmaskFontType::dimension_t>
				>
				using TemplateDrawer = typename TypeTraits::TypeConditional::conditional_type<
					TemplateDrawerNoScale<BitmaskFontType, PrimitiveShaderType>,
					TemplateDrawerIntegerScale<BitmaskFontType, ScaleX, ScaleY, PrimitiveShaderType>,
					(ScaleX == 1 && ScaleY == 1)>::type;
			}
		}
	}
}
#endif