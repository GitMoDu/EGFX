#ifndef _EGFX_FRAMEWORK_ASSETS_EGFX_LOGO_CODE_FONTS_h
#define _EGFX_FRAMEWORK_ASSETS_EGFX_LOGO_CODE_FONTS_h

#include <EgfxDrawer.h>

namespace Egfx
{
	namespace CodeFonts
	{
		namespace LogoFontFamily
		{

			namespace Dimensions
			{
				using font_dimension_t = uint8_t;

				static constexpr font_dimension_t ColumnCount = 6;
				static constexpr font_dimension_t LineCount = 6;

				template<pixel_t FontWidth, pixel_t FontHeight>
				struct FontLayout
				{
					static constexpr uint8_t Margin() { return 1; }

					static constexpr pixel_t RowHeight()
					{
						return FontHeight / Dimensions::LineCount;
					}

					static constexpr pixel_t EffectiveHeight()
					{
						return RowHeight() * (Dimensions::LineCount - 1);
					}

					static constexpr uint8_t LinesPerRow() { return RowHeight() >= (3 + Margin()) ? 2 : 1; }
				};
			}

			namespace Shaders
			{
				namespace RetroLines
				{
					template<typename dimension_t,
						dimension_t FontWidth, dimension_t FontHeight,
						typename ColorShaderType = VectorGraphics::Shaders::Color::Single<dimension_t>
					>
					struct PrimitiveShaderType
					{
						struct Layout
						{
							static constexpr pixel_t RowHeight()
							{
								return Dimensions::FontLayout<FontWidth, FontHeight>::RowHeight();
							}

							static constexpr uint8_t ScanlineMargin() { return 1 + (FontWidth / 3 / 3); }
							static constexpr uint8_t ExtraMargin() { return 0; }
							static constexpr uint8_t Margin() { return Dimensions::FontLayout<FontWidth, FontHeight>::Margin(); }

							static constexpr uint8_t LinesPerRow() { return Dimensions::FontLayout<FontWidth, FontHeight>::LinesPerRow(); }

							static constexpr pixel_t LineHeight()
							{
								return RowHeight() / LinesPerRow();
							}

							static constexpr pixel_t LineMargin()
							{
								return 1 + (LineHeight() / 3);
							}

							static constexpr pixel_t GetLineYOffset(const uint8_t lineIndex)
							{
								return static_cast<pixel_t>(pixel_t(lineIndex) * LineHeight());
							}
						};

						ColorShaderType ColorShader{};
						pixel_point_t Origin{ 0, 0 };

						PrimitiveShaderType() {}
						~PrimitiveShaderType() = default;

						void PrimitivePrepare(const pixel_t originX, const pixel_t originY)
						{
							Origin.x = originX;
							Origin.y = originY;
						}

						void PrimitiveRectangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
							const dimension_t x2, const dimension_t /*y2*/)
						{
							const auto color = ColorShader.ShadeColor(x1, y1);

							for (uint8_t i = 0; i < Layout::LinesPerRow(); i++)
							{
								const pixel_t y = Origin.y + y1 + Layout::GetLineYOffset(i);

								framebuffer->Line(color,
									static_cast<pixel_t>(Origin.x + x1),
									static_cast<pixel_t>(y + Layout::LineHeight() / 2),
									static_cast<pixel_t>(Origin.x + x2),
									static_cast<pixel_t>(y + Layout::LineHeight() / 2));

								if (Layout::LineHeight() >= (1 + Layout::LineMargin() * 2))
								{
									framebuffer->RectangleFill(color,
										static_cast<pixel_t>(Origin.x + x1 + Layout::ScanlineMargin()),
										static_cast<pixel_t>(y + Layout::LineMargin()),
										static_cast<pixel_t>(Origin.x + x2 - Layout::ScanlineMargin()),
										static_cast<pixel_t>(y + Layout::LineHeight() / 2 - 1));
									framebuffer->RectangleFill(color,
										static_cast<pixel_t>(Origin.x + x1 + Layout::ScanlineMargin()),
										static_cast<pixel_t>(y + Layout::LineHeight() / 2 + 1),
										static_cast<pixel_t>(Origin.x + x2 - Layout::ScanlineMargin()),
										static_cast<pixel_t>(y + Layout::LineHeight() - Layout::LineMargin()));
								}
								else if ((Layout::LineHeight() > 2))
								{
									framebuffer->Line(color,
										static_cast<pixel_t>(Origin.x + x1 + Layout::ScanlineMargin()),
										static_cast<pixel_t>(y + 1 + Layout::LineHeight() / 2),
										static_cast<pixel_t>(Origin.x + x2 - Layout::ScanlineMargin()),
										static_cast<pixel_t>(y + 1 + Layout::LineHeight() / 2));
								}
							}
						}
					};
				}

				namespace DotMatrix
				{
					template<typename dimension_t,
						dimension_t FontWidth, dimension_t FontHeight,
						typename ColorShaderType = VectorGraphics::Shaders::Color::Single<dimension_t>
					>
					struct PrimitiveShaderType
					{
						struct Layout
						{
							static constexpr uint8_t Margin() { return Dimensions::FontLayout<FontWidth, FontHeight>::Margin(); }

							static constexpr pixel_t RowHeight()
							{
								return Dimensions::FontLayout<FontWidth, FontHeight>::RowHeight();
							}

							static constexpr uint8_t LinesPerRow() { return Dimensions::FontLayout<FontWidth, FontHeight>::LinesPerRow(); }


							static constexpr pixel_t SquareSize()
							{
								return (FontWidth / Dimensions::ColumnCount);
							}

							static constexpr pixel_t SquareVisibleSize()
							{
								return SquareSize() - (Margin() * 2);
							}

							static constexpr uint8_t SquaresPerRow()
							{
								return Dimensions::ColumnCount / 2;
							}

							static constexpr pixel_point_t GetSquareOffset(const uint8_t squareX, const uint8_t squareY)
							{
								return pixel_point_t{
									static_cast<pixel_t>(pixel_t(squareX) * SquareSize()),
									static_cast<pixel_t>(pixel_t(squareY) * SquareSize())
								};
							}
						};

						ColorShaderType ColorShader{};
						pixel_point_t Origin{ 0, 0 };

						PrimitiveShaderType() {}
						~PrimitiveShaderType() = default;


						void PrimitivePrepare(const pixel_t originX, const pixel_t originY)
						{
							Origin.x = originX;
							Origin.y = originY;
						}

						void PrimitiveRectangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
							const dimension_t x2, const dimension_t /*y2*/)
						{
							const uint8_t xSquares = (x2 - x1) / (Layout::SquareSize() + 0);

							for (uint8_t x = 0; x < xSquares; x++)
							{
								for (uint8_t y = 0; y < Layout::LinesPerRow(); y++)
								{
									pixel_point_t point = Layout::GetSquareOffset(x, y);

									const auto color = ColorShader.ShadeColor(point.x, point.y);

									point.x += Origin.x + x1;
									point.y += Origin.y + y1;

									if (Layout::SquareVisibleSize() > 0)
									{
										framebuffer->RectangleFill(color,
											static_cast<pixel_t>(point.x + Layout::Margin()),
											static_cast<pixel_t>(point.y + Layout::Margin()),
											static_cast<pixel_t>(point.x + Layout::Margin() + Layout::SquareVisibleSize()),
											static_cast<pixel_t>(point.y + Layout::Margin() + Layout::SquareVisibleSize()));
									}
									else
									{
										framebuffer->Pixel(color,
											static_cast<pixel_t>(point.x + Layout::Margin()),
											static_cast<pixel_t>(point.y + Layout::Margin()));
									}
								}
							}
						}
					};
				}
			}

			template<const pixel_t FontWidth, const pixel_t FontHeight,
				typename PrimitiveShaderType = VectorGraphics::Shaders::Primitive::SingleColor<Dimensions::font_dimension_t>>
				struct TemplateFont
			{
			public:
				using dimension_t = Dimensions::font_dimension_t;

			private:
				static constexpr pixel_t LineHeight = FontHeight / Dimensions::LineCount;
				static constexpr pixel_t ColumnWidth = FontWidth / Dimensions::ColumnCount;
				static constexpr pixel_t GShift = ColumnWidth / 2;

			public:
				PrimitiveShaderType PrimitiveShader{};

			public:
				TemplateFont() {}

				~TemplateFont() = default;

				// Get only character advance.
				dimension_t Advance(const char /*printableChar*/) const
				{
					// Monospaced font.
					return FontWidth;
				}

				pixel_t Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char printableChar)
				{
					PrimitiveShader.PrimitivePrepare(x, y);

					switch (static_cast<uint8_t>(printableChar))
					{
					case uint8_t(AsciiDefinition::Printable::E):
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, 0, 0, FontWidth, LineHeight);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, 0, LineHeight, ColumnWidth * 2, LineHeight * 2);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, 0, LineHeight * 2, FontWidth - ColumnWidth * 1, LineHeight * 3);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, 0, LineHeight * 3, ColumnWidth * 2, LineHeight * 4);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, 0, LineHeight * 4, FontWidth, LineHeight * 5);
						break;
					case uint8_t(AsciiDefinition::Printable::F):
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, 0, 0, FontWidth, LineHeight);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, 0, LineHeight, ColumnWidth * 2, LineHeight * 2);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, 0, LineHeight * 2, FontWidth - ColumnWidth, LineHeight * 3);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, 0, LineHeight * 3, ColumnWidth * 2, LineHeight * 4);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, 0, LineHeight * 4, ColumnWidth * 2, LineHeight * 5);
						break;
					case uint8_t(AsciiDefinition::Printable::G):
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, GShift, 0, FontWidth - GShift, LineHeight);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, 0, LineHeight, ColumnWidth * 2, LineHeight * 2);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, 0, LineHeight * 2, ColumnWidth * 2, LineHeight * 3);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, FontWidth - ColumnWidth * 3, LineHeight * 2, FontWidth, LineHeight * 3);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, 0, LineHeight * 3, ColumnWidth * 2, LineHeight * 4);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, FontWidth - ColumnWidth * 2, LineHeight * 3, FontWidth, LineHeight * 4);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, GShift, LineHeight * 4, FontWidth - GShift, LineHeight * 5);
						break;
					case uint8_t(AsciiDefinition::Printable::X):
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, 0, 0, ColumnWidth * 2, LineHeight);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, FontWidth - ColumnWidth * 2, 0, FontWidth, LineHeight);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, ColumnWidth, LineHeight * 1, ColumnWidth * 3, LineHeight * 2);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, FontWidth - ColumnWidth * 3, LineHeight * 1, FontWidth - ColumnWidth, LineHeight * 2);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, ColumnWidth * 2, LineHeight * 2, FontWidth - ColumnWidth * 2, LineHeight * 3);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, FontWidth - ColumnWidth * 3, LineHeight * 3, FontWidth - ColumnWidth, LineHeight * 4);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, ColumnWidth, LineHeight * 3, ColumnWidth * 3, LineHeight * 4);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, 0, LineHeight * 4, ColumnWidth * 2, LineHeight * 5);
						PrimitiveShader.PrimitiveRectangleFill(framebuffer, FontWidth - ColumnWidth * 2, LineHeight * 4, FontWidth, LineHeight * 5);
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
#endif