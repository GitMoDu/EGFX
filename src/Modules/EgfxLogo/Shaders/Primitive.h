#ifndef _EGFX_MODULES_EGFX_LOGO_SHADER_PRIMITIVE_h
#define _EGFX_MODULES_EGFX_LOGO_SHADER_PRIMITIVE_h

#include "../Constant.h"
#include "../Layout.h"

namespace Egfx
{
	namespace Modules
	{
		namespace EgfxLogo
		{
			namespace Shader
			{
				namespace Primitive
				{
					using dimension_t = Dimensions::dimension_t;

					/// <summary>
					/// Screen primitive shader, forwards the custom pixel shader for the screen effect.
					/// </summary>
					/// <typeparam name="Monochrome">A boolean flag indicating whether the screen rendering should be in monochrome (true) or color (false).</typeparam>
					template<bool Monochrome>
					using Screen = Framework::Shader::Primitive::TemplateShader<
						dimension_t,
						Pixel::Screen<Monochrome>
					>;

					/// <summary>
					/// Custom RetroLines primitive shader, renders rectangles as sets of spaced out scanlines,.
					/// </summary>
					/// <typeparam name="ColorSourceType">Color source type. Defaults to SingleColor source.</typeparam>
					/// <typeparam name="TransformShaderType">Transform type. Defaults to Translate transform shader.</typeparam>
					/// <typeparam name="FontWidth">The width of the font in pixels, used for layout calculations.</typeparam>
					/// <typeparam name="FontHeight">The height of the font in pixels, used for layout calculations.</typeparam>
					template<dimension_t FontWidth, dimension_t FontHeight,
						typename ColorSourceType = Framework::Shader::Source::SingleColor<dimension_t>,
						typename TransformShaderType = Framework::Assets::Shader::Transform::Translate<dimension_t>
					>
					struct RetroLines
					{
						using RetroLayout = Layout::RetroLinesLayout<FontWidth, FontHeight>;

						ColorSourceType ColorSource{};
						TransformShaderType TransformShader{};

						pixel_point_t Origin{ 0, 0 };

						RetroLines() {}
						~RetroLines() = default;

						void Prepare(const pixel_t originX, const pixel_t originY)
						{
							Origin.x = originX;
							Origin.y = originY;
							TransformShader.Prepare(originX, originY);
						}

						void Pixel(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y)
						{
							// No-op for pixel.
						}

						void Line(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
							const dimension_t x2, const dimension_t y2)
						{
							// No-op for line.
						}

						void TriangleFill(IFrameBuffer* /*framebuffer*/, const dimension_t /*x1*/, const dimension_t /*y1*/,
							const dimension_t /*x2*/, const dimension_t /*y2*/,
							const dimension_t /*x3*/, const dimension_t /*y3*/)
						{
							// No-op for triangle fills.
						}

						void RectangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
							const dimension_t x2, const dimension_t y2)
						{
							dimension_t coordinatesX1 = x1;
							dimension_t coordinatesY1 = y1;
							dimension_t coordinatesX2 = x2;
							dimension_t coordinatesY2 = y2;

							if (!TransformShader.Transform(coordinatesX1, coordinatesY1)
								|| !TransformShader.Transform(coordinatesX2, coordinatesY2))
							{
								return;
							}

							for (uint8_t i = 0; i < RetroLayout::LinesPerRow(); i++)
							{
								const rgb_color_t color = ColorSource.Source(0, y1 + RetroLayout::GetLineYOffset(i));

								const pixel_t y = Origin.y + coordinatesY1 + RetroLayout::GetLineYOffset(i);

								framebuffer->Line(color,
									static_cast<pixel_t>(Origin.x + coordinatesX1),
									static_cast<pixel_t>(y + RetroLayout::LineHeight() / 2),
									static_cast<pixel_t>(Origin.x + coordinatesX2),
									static_cast<pixel_t>(y + RetroLayout::LineHeight() / 2));

								if (RetroLayout::LineHeight() >= (1 + RetroLayout::LineMargin() * 2))
								{
									framebuffer->RectangleFill(color,
										static_cast<pixel_t>(Origin.x + coordinatesX1 + RetroLayout::ScanlineMargin()),
										static_cast<pixel_t>(y + RetroLayout::LineMargin()),
										static_cast<pixel_t>(Origin.x + coordinatesX2 - RetroLayout::ScanlineMargin()),
										static_cast<pixel_t>(y + RetroLayout::LineHeight() / 2 - 1));
									framebuffer->RectangleFill(color,
										static_cast<pixel_t>(Origin.x + coordinatesX1 + RetroLayout::ScanlineMargin()),
										static_cast<pixel_t>(y + RetroLayout::LineHeight() / 2 + 1),
										static_cast<pixel_t>(Origin.x + coordinatesX2 - RetroLayout::ScanlineMargin()),
										static_cast<pixel_t>(y + RetroLayout::LineHeight() - RetroLayout::LineMargin()));
								}
								else if ((RetroLayout::LineHeight() > 2))
								{
									framebuffer->Line(color,
										static_cast<pixel_t>(Origin.x + coordinatesX1 + RetroLayout::ScanlineMargin()),
										static_cast<pixel_t>(y + 1 + RetroLayout::LineHeight() / 2),
										static_cast<pixel_t>(Origin.x + coordinatesX2 - RetroLayout::ScanlineMargin()),
										static_cast<pixel_t>(y + 1 + RetroLayout::LineHeight() / 2));
								}
							}
						}
					};

					/// <summary>
					/// Custom DotMatrix primitive shader, renders rectangles as a grid of dots/squares.
					/// </summary>
					/// <typeparam name="ColorSourceType">Color source type. Defaults to SingleColor source.</typeparam>
					/// <typeparam name="TransformShaderType">Transform type. Defaults to Translate transform shader.</typeparam>
					/// <typeparam name="FontWidth">The width of the font in pixels, used for layout calculations.</typeparam>
					/// <typeparam name="FontHeight">The height of the font in pixels, used for layout calculations.</typeparam>
					template<dimension_t FontWidth, dimension_t FontHeight,
						typename ColorSourceType = Framework::Shader::Source::SingleColor<dimension_t>,
						typename TransformShaderType = Framework::Assets::Shader::Transform::Translate<dimension_t>
					>
					struct DotMatrix
					{
						using DotLayout = Layout::DotMatrixLayout<FontWidth, FontHeight>;

						ColorSourceType ColorSource{};
						TransformShaderType TransformShader{};

						pixel_point_t Origin{ 0, 0 };

						DotMatrix() {}
						~DotMatrix() = default;

						void Prepare(const pixel_t originX, const pixel_t originY)
						{
							Origin.x = originX;
							Origin.y = originY;
							TransformShader.Prepare(originX, originY);
						}

						void Pixel(IFrameBuffer* /*framebuffer*/, const dimension_t /*x*/, const dimension_t /*y*/)
						{
							// No-op for pixel.
						}

						void Line(IFrameBuffer* /*framebuffer*/, const dimension_t /*x1*/, const dimension_t /*y1*/,
							const dimension_t /*x2*/, const dimension_t /*y2*/)
						{
							// No-op for line.
						}

						void TriangleFill(IFrameBuffer* /*framebuffer*/, const dimension_t /*x1*/, const dimension_t /*y1*/,
							const dimension_t /*x2*/, const dimension_t /*y2*/,
							const dimension_t /*x3*/, const dimension_t /*y3*/)
						{
							// No-op for triangle fills.
						}

						void RectangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
							const dimension_t x2, const dimension_t /*y2*/)
						{
							dimension_t coordinatesX1 = x1;
							dimension_t coordinatesY1 = y1;
							dimension_t coordinatesX2 = x2;
							dimension_t coordinatesY2 = y1;

							if (!TransformShader.Transform(coordinatesX1, coordinatesY1)
								|| !TransformShader.Transform(coordinatesX2, coordinatesY2))
							{
								return;
							}

							const pixel_t squareSize = DotLayout::SquareSize();
							if (squareSize == 0)
							{
								return;
							}

							const dimension_t width = coordinatesX2 - coordinatesX1;
							const uint8_t xSquares = static_cast<uint8_t>(width / squareSize);

							for (uint8_t sx = 0; sx < xSquares; sx++)
							{
								for (uint8_t sy = 0; sy < DotLayout::LinesPerRow(); sy++)
								{
									pixel_point_t point = DotLayout::GetSquareOffset(sx, sy);

									const rgb_color_t color = ColorSource.Source(point.x, point.y);

									const pixel_t px = static_cast<pixel_t>(Origin.x + coordinatesX1 + point.x);
									const pixel_t py = static_cast<pixel_t>(Origin.y + coordinatesY1 + point.y);

									if (DotLayout::SquareVisibleSize() > 0)
									{
										framebuffer->RectangleFill(color,
											static_cast<pixel_t>(px + DotLayout::Margin()),
											static_cast<pixel_t>(py + DotLayout::Margin()),
											static_cast<pixel_t>(px + DotLayout::Margin() + DotLayout::SquareVisibleSize()),
											static_cast<pixel_t>(py + DotLayout::Margin() + DotLayout::SquareVisibleSize()));
									}
									else
									{
										framebuffer->Pixel(color,
											static_cast<pixel_t>(px + DotLayout::Margin()),
											static_cast<pixel_t>(py + DotLayout::Margin()));
									}
								}
							}
						}
					};
				}
			}
		}
	}
}
#endif