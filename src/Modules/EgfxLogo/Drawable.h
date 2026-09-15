#ifndef _EGFX_MODULES_EGFX_LOGO_DRAWABLE_h
#define _EGFX_MODULES_EGFX_LOGO_DRAWABLE_h

#include "Constant.h"
#include "Layout.h"

#include "Shaders/Source.h"
#include "Shaders/Color.h"
#include "Shaders/Pixel.h"

namespace Egfx
{
	namespace Modules
	{
		namespace EgfxLogo
		{
			namespace Drawable
			{
				using dimension_t = pixel_t;

				/// <summary>
				/// Rectangle outline with custom Pixel shader.
				/// </summary>
				template<typename ParentLayout, bool Monochrome>
				using Screen = Framework::Assets::Drawable::Shape::Rectangle<
					dimension_t,
					typename Layout::Calc<ParentLayout>::LogoLayout,
					1,
					Shader::Pixel::Screen<Monochrome>
				>;


				/// <summary>
				/// Draws letters "EG" using Logo font with RetroLines shader.
				/// </summary>
				template<typename ParentLayout, bool Monochrome>
				class LettersEG : public Framework::Shader::Geometry::RectangleShader<
					typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
					Framework::Shader::Pixel::TemplateShader<
					typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
					Shader::Source::RetroLinesColorAlpha<Layout::Calc<ParentLayout>::FontHeight(), Monochrome>>
					>
				{
				private:
					using Base = Framework::Shader::Geometry::RectangleShader<
						typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
						Framework::Shader::Pixel::TemplateShader<
						typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
						Shader::Source::RetroLinesColorAlpha<Layout::Calc<ParentLayout>::FontHeight(), Monochrome>>>;

					static constexpr dimension_t FontWidth = Layout::Calc<ParentLayout>::FontWidth();
					static constexpr dimension_t FontHeight = Layout::Calc<ParentLayout>::FontHeight();
					static constexpr dimension_t Kerning = Layout::Calc<ParentLayout>::Kerning();

					using LettersLayout = Layout::LettersEG<ParentLayout>;

					using RenderLayout = Layout::Letters::ScanLineLetterLayout<FontWidth, FontHeight>;
					using ScanLine = Layout::Letters::ScanLine;

				private:
					static constexpr int16_t ScanLineHeight = RenderLayout::LineHeight;
					static constexpr int16_t OverscanHeight = RenderLayout::OverscanHeight;
					static constexpr int16_t ScanlineOffset = (RenderLayout::SquareHeight() / 2);

				public:
					LettersEG() : Base(0, 0, LettersLayout::Width(), LettersLayout::Height()) {}
					~LettersEG() = default;

					void Draw(IFrameBuffer* framebuffer)
					{
						Base::Prepare(LettersLayout::X(), LettersLayout::Y());

						// Draw E
						{
							DrawScanline(framebuffer, RenderLayout::Row(0, 0, Dimensions::ColumnCount - 1, 0));
							DrawScanline(framebuffer, RenderLayout::Row(1, 0, 1, 0));
							DrawScanline(framebuffer, RenderLayout::Row(2, 0, Dimensions::ColumnCount / 2, 0));
							DrawScanline(framebuffer, RenderLayout::Row(3, 0, 1, 0));
							DrawScanline(framebuffer, RenderLayout::Row(4, 0, Dimensions::ColumnCount - 1, 0));
						}

						// Draw G
						{
							constexpr dimension_t gOffset = LetterOffset(1);
							DrawScanline(framebuffer, RenderLayout::Row(0, 1, Dimensions::ColumnCount - 2, gOffset));
							DrawScanline(framebuffer, RenderLayout::Row(1, 0, 1, gOffset));
							DrawScanline(framebuffer, RenderLayout::Row(2, 0, 1, gOffset));
							DrawScanline(framebuffer, RenderLayout::Row(2, (Dimensions::ColumnCount / 2) + 1, Dimensions::ColumnCount - 1, gOffset));
							DrawScanline(framebuffer, RenderLayout::Row(3, 0, 1, gOffset));
							DrawScanline(framebuffer, RenderLayout::Row(3, Dimensions::ColumnCount - 2, Dimensions::ColumnCount - 1, gOffset));
							DrawScanline(framebuffer, RenderLayout::Row(4, 1, Dimensions::ColumnCount - 2, gOffset));
						}
					}

				private:
					static constexpr dimension_t LetterOffset(const dimension_t letterIndex)
					{
						return (FontWidth + Kerning) * letterIndex;
					}

					void DrawScanline(IFrameBuffer* framebuffer, const ScanLine scanline)
					{
						Base::RectangleFill(framebuffer, scanline.Line.topLeft.x, scanline.Line.topLeft.y, scanline.Line.bottomRight.x, scanline.Line.bottomRight.y);
						Base::RectangleFill(framebuffer, scanline.Overscan.topLeft.x, scanline.Overscan.topLeft.y, scanline.Overscan.bottomRight.x, scanline.Overscan.bottomRight.y);

						if (ScanlineOffset > 0)
						{
							Base::RectangleFill(framebuffer, scanline.Line.topLeft.x, scanline.Line.topLeft.y + ScanlineOffset, scanline.Line.bottomRight.x, scanline.Line.bottomRight.y + ScanlineOffset);
							Base::RectangleFill(framebuffer, scanline.Overscan.topLeft.x, scanline.Overscan.topLeft.y + ScanlineOffset, scanline.Overscan.bottomRight.x, scanline.Overscan.bottomRight.y + ScanlineOffset);
						}
					}
				};

				/// <summary>
				/// Draws letters "FX" using Logo font with DotMatrix shader.
				/// </summary>
				template<typename ParentLayout, bool Monochrome>
				class LettersFX : public Framework::Shader::Geometry::RectangleShader<
					typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
					Framework::Shader::Pixel::TemplateShader<
					typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
					Shader::Source::DotMatrixRgbAlpha<Layout::Calc<ParentLayout>::FontWidth(), Layout::Calc<ParentLayout>::FontHeight(), Monochrome>>
					>
				{
				private:
					using Base = Framework::Shader::Geometry::RectangleShader<
						typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
						Framework::Shader::Pixel::TemplateShader<
						typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
						Shader::Source::DotMatrixRgbAlpha<Layout::Calc<ParentLayout>::FontWidth(), Layout::Calc<ParentLayout>::FontHeight(), Monochrome>>>;

					static constexpr dimension_t FontWidth = Layout::Calc<ParentLayout>::FontWidth();
					static constexpr dimension_t FontHeight = Layout::Calc<ParentLayout>::FontHeight();
					static constexpr dimension_t Kerning = Layout::Calc<ParentLayout>::Kerning();

					using LettersLayout = Layout::LettersFX<ParentLayout>;

					using RenderLayout = Layout::Letters::SquareDotLetterLayout<FontWidth, FontHeight>;

				public:
					LettersFX() : Base(0, 0, LettersLayout::Width(), LettersLayout::Height()) {}
					~LettersFX() = default;

					void Draw(IFrameBuffer* framebuffer)
					{
						Base::Prepare(LettersLayout::X(), LettersLayout::Y());

						// Draw F
						const dimension_t fOffset = 0;
						DrawRow(framebuffer, 0, 0, Dimensions::ColumnCount - 1, fOffset);
						DrawRow(framebuffer, 1, 0, 1, fOffset);
						DrawRow(framebuffer, 2, 0, Dimensions::ColumnCount / 2, fOffset);
						DrawRow(framebuffer, 3, 0, 1, fOffset);
						DrawRow(framebuffer, 4, 0, 1, fOffset);

						// Draw X
						const dimension_t xOffset = FontWidth + Kerning;
						DrawRow(framebuffer, 0, 0, 1, xOffset);
						DrawRow(framebuffer, 0, Dimensions::ColumnCount - 2, Dimensions::ColumnCount - 1, xOffset);
						DrawRow(framebuffer, 1, 1, 2, xOffset);
						DrawRow(framebuffer, 1, Dimensions::ColumnCount - 3, Dimensions::ColumnCount - 2, xOffset);

						DrawRow(framebuffer, 2, (Dimensions::ColumnCount / 2) - 1, (Dimensions::ColumnCount / 2) + 1, xOffset);

						DrawRow(framebuffer, 3, Dimensions::ColumnCount - 3, Dimensions::ColumnCount - 2, xOffset);
						DrawRow(framebuffer, 3, 1, 2, xOffset);
						DrawRow(framebuffer, 4, Dimensions::ColumnCount - 2, Dimensions::ColumnCount - 1, xOffset);
						DrawRow(framebuffer, 4, 0, 1, xOffset);
					}

				private:
					static constexpr dimension_t LetterOffset(const dimension_t letterIndex)
					{
						return (FontWidth + Kerning) * letterIndex;
					}

					void DrawRow(IFrameBuffer* framebuffer, const uint8_t rowIndex, const uint8_t columnStartIndex, const uint8_t columnEndIndex, const dimension_t xOffset)
					{
						for (uint8_t columnIndex = columnStartIndex; columnIndex <= columnEndIndex; ++columnIndex)
						{
							const auto cell = RenderLayout::Cell(rowIndex, columnIndex, xOffset);
							Base::RectangleFill(framebuffer,
								cell.topLeft.x,
								cell.topLeft.y,
								cell.topLeft.x + RenderLayout::DotSize() - 1,
								cell.topLeft.y + RenderLayout::DotSize() - 1);

							Base::RectangleFill(framebuffer,
								cell.topLeft.x,
								cell.topLeft.y + RenderLayout::DotOffset(),
								cell.topLeft.x + RenderLayout::DotSize() - 1,
								cell.topLeft.y + RenderLayout::DotOffset() + RenderLayout::DotSize() - 1);
						}
					}
				};
			}
		}
	}
}
#endif