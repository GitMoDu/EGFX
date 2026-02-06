#ifndef _EGFX_FRAMEWORK_BITMAP_TEMPLATE_DRAWER_h
#define _EGFX_FRAMEWORK_BITMAP_TEMPLATE_DRAWER_h

#include "Model.h"
#include "../Shader/Color/Shaders.h"
#include "../Shader/Transform/Shaders.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Bitmap
		{
			/// <summary>
			/// Bitmap drawer (no scaling).
			///
			/// Draws a row-major bitmap by sampling source pixels, optionally skipping a transparent key color,
			/// optionally transforming coordinates, and applying a color shader before writing to the framebuffer.
			/// </summary>
			/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
			/// <typeparam name="Width">Bitmap width (pixels).</typeparam>
			/// <typeparam name="Height">Bitmap height (pixels).</typeparam>
			/// <typeparam name="BitmapReader">Bitmap reader type providing ReadColorAt(...).</typeparam>
			/// <typeparam name="TransparentColor">Transparent key color (RGB888).</typeparam>
			/// <typeparam name="UseTransparentColor">Enables transparent key color skipping when true.</typeparam>
			/// <typeparam name="ColorShaderType">Color shader type providing Shade(color).</typeparam>
			/// <typeparam name="TransformShaderType">Transform shader type providing Prepare(...) and Transform(x, y).</typeparam>
			template<typename dimension_t,
				dimension_t Width,
				dimension_t Height,
				typename BitmapReader = Reader::Flash,
				rgb_color_t TransparentColor = RGB_COLOR_BLACK,
				bool UseTransparentColor = false,
				typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
				typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>
			>
			class TemplateDrawerNoScale
			{
			public:
				/// <summary>Color shader applied to each sampled bitmap pixel.</summary>
				ColorShaderType ColorShader{};

				/// <summary>Transform shader applied to local-space coordinates.</summary>
				TransformShaderType TransformShader{};

			public:
				TemplateDrawerNoScale() {}
				~TemplateDrawerNoScale() = default;

				/// <summary>
				/// Draws an 8-bit bitmap (indexed/paletted or Color332, depending on usage).
				/// </summary>
				/// <param name="framebuffer">Target framebuffer to draw into.</param>
				/// <param name="x">Pixel-space origin X (pixels).</param>
				/// <param name="y">Pixel-space origin Y (pixels).</param>
				/// <param name="bitmap">Pointer to row-major bitmap pixels.</param>
				void Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const uint8_t* bitmap)
				{
					static constexpr uint8_t TransparentColor8 =
						Rgb::Color332From888(Rgb::R8(TransparentColor), Rgb::G8(TransparentColor), Rgb::B8(TransparentColor));

					TransformShader.Prepare(x, y);

					for (dimension_t row = 0; row < Height; row++)
					{
						for (dimension_t column = 0; column < Width; column++)
						{
							const uint8_t bitmapColor = BitmapReader::template ReadColorAt<dimension_t, Width>(bitmap, column, row);

							if (UseTransparentColor && (bitmapColor == TransparentColor8))
							{
								continue;
							}

							dimension_t tx = column;
							dimension_t ty = row;
							if (TransformShader.Transform(tx, ty))
							{
								framebuffer->Pixel(ColorShader.Shade(bitmapColor),
									static_cast<pixel_t>(x + static_cast<pixel_t>(tx)),
									static_cast<pixel_t>(y + static_cast<pixel_t>(ty)));
							}
						}
					}
				}

				/// <summary>
				/// Draws a 16-bit bitmap (RGB565).
				/// </summary>
				/// <param name="framebuffer">Target framebuffer to draw into.</param>
				/// <param name="x">Pixel-space origin X (pixels).</param>
				/// <param name="y">Pixel-space origin Y (pixels).</param>
				/// <param name="bitmap">Pointer to row-major bitmap pixels.</param>
				void Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const uint16_t* bitmap)
				{
					static constexpr uint16_t TransparentColor16 =
						Rgb::Color565From888(Rgb::R8(TransparentColor), Rgb::G8(TransparentColor), Rgb::B8(TransparentColor));

					TransformShader.Prepare(x, y);

					for (dimension_t row = 0; row < Height; row++)
					{
						for (dimension_t column = 0; column < Width; column++)
						{
							const uint16_t bitmapColor = BitmapReader::template ReadColorAt<dimension_t, Width>(bitmap, column, row);

							if (UseTransparentColor && (bitmapColor == TransparentColor16))
							{
								continue;
							}

							dimension_t tx = column;
							dimension_t ty = row;
							if (TransformShader.Transform(tx, ty))
							{
								framebuffer->Pixel(ColorShader.Shade(Rgb::Color(bitmapColor)),
									static_cast<pixel_t>(x + static_cast<pixel_t>(tx)),
									static_cast<pixel_t>(y + static_cast<pixel_t>(ty)));
							}
						}
					}
				}

				/// <summary>
				/// Draws a 32-bit bitmap (RGB888 or ARGB8888, depending on usage).
				/// </summary>
				/// <param name="framebuffer">Target framebuffer to draw into.</param>
				/// <param name="x">Pixel-space origin X (pixels).</param>
				/// <param name="y">Pixel-space origin Y (pixels).</param>
				/// <param name="bitmap">Pointer to row-major bitmap pixels.</param>
				void Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const uint32_t* bitmap)
				{
					static constexpr uint32_t TransparentColor32 =
						Rgb::Color888From888(Rgb::R8(TransparentColor), Rgb::G8(TransparentColor), Rgb::B8(TransparentColor));

					TransformShader.Prepare(x, y);

					for (dimension_t row = 0; row < Height; row++)
					{
						for (dimension_t column = 0; column < Width; column++)
						{
							const uint32_t bitmapColor = BitmapReader::template ReadColorAt<dimension_t, Width>(bitmap, column, row);

							if (UseTransparentColor && (bitmapColor == TransparentColor32))
							{
								continue;
							}

							dimension_t tx = column;
							dimension_t ty = row;
							if (TransformShader.Transform(tx, ty))
							{
								framebuffer->Pixel(ColorShader.Shade(Rgb::Color(bitmapColor)),
									static_cast<pixel_t>(x + static_cast<pixel_t>(tx)),
									static_cast<pixel_t>(y + static_cast<pixel_t>(ty)));
							}
						}
					}
				}
			};

			/// <summary>
			/// Bitmap drawer (integer scaling).
			///
			/// After optional transparency and transform steps, each source pixel becomes a filled rectangle
			/// of size (ScaleX x ScaleY) in the framebuffer.
			/// </summary>
			/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
			/// <typeparam name="Width">Bitmap width (pixels).</typeparam>
			/// <typeparam name="Height">Bitmap height (pixels).</typeparam>
			/// <typeparam name="BitmapReader">Bitmap reader type providing ReadColorAt(...).</typeparam>
			/// <typeparam name="ScaleX">Horizontal scale factor (integer).</typeparam>
			/// <typeparam name="ScaleY">Vertical scale factor (integer).</typeparam>
			/// <typeparam name="TransparentColor">Transparent key color (RGB888).</typeparam>
			/// <typeparam name="UseTransparentColor">Enables transparent key color skipping when true.</typeparam>
			/// <typeparam name="ColorShaderType">Color shader type providing Shade(color).</typeparam>
			/// <typeparam name="TransformShaderType">Transform shader type providing Prepare(...) and Transform(x, y).</typeparam>
			template<typename dimension_t,
				dimension_t Width,
				dimension_t Height,
				typename BitmapReader = Reader::Flash,
				uint8_t ScaleX = 1,
				uint8_t ScaleY = 1,
				rgb_color_t TransparentColor = RGB_COLOR_BLACK,
				bool UseTransparentColor = false,
				typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
				typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>
			>
			struct TemplateDrawerIntegerScale
			{
			public:
				/// <summary>Color shader applied to each sampled bitmap pixel.</summary>
				ColorShaderType ColorShader{};

				/// <summary>Transform shader applied to local-space coordinates.</summary>
				TransformShaderType TransformShader{};

			public:
				TemplateDrawerIntegerScale() {}
				~TemplateDrawerIntegerScale() = default;

				/// <summary>
				/// Draws an 8-bit bitmap with integer scaling.
				/// </summary>
				/// <param name="framebuffer">Target framebuffer to draw into.</param>
				/// <param name="x">Pixel-space origin X (pixels).</param>
				/// <param name="y">Pixel-space origin Y (pixels).</param>
				/// <param name="bitmap">Pointer to row-major bitmap pixels.</param>
				void Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const uint8_t* bitmap)
				{
					static constexpr uint8_t TransparentColor8 =
						Rgb::Color332From888(Rgb::R8(TransparentColor), Rgb::G8(TransparentColor), Rgb::B8(TransparentColor));

					TransformShader.Prepare(x, y);

					for (dimension_t row = 0; row < Height; row++)
					{
						for (dimension_t column = 0; column < Width; column++)
						{
							const uint8_t bitmapColor = BitmapReader::template ReadColorAt<dimension_t, Width>(bitmap, column, row);

							if (UseTransparentColor && (bitmapColor == TransparentColor8))
							{
								continue;
							}

							dimension_t tx = column;
							dimension_t ty = row;
							if (!TransformShader.Transform(tx, ty))
							{
								continue;
							}

							const pixel_t startX = static_cast<pixel_t>(x + (static_cast<pixel_t>(tx) * static_cast<pixel_t>(ScaleX)));
							const pixel_t startY = static_cast<pixel_t>(y + (static_cast<pixel_t>(ty) * static_cast<pixel_t>(ScaleY)));

							framebuffer->RectangleFill(ColorShader.Shade(bitmapColor),
								startX,
								startY,
								static_cast<pixel_t>(startX + (ScaleX - 1)),
								static_cast<pixel_t>(startY + (ScaleY - 1)));
						}
					}
				}

				/// <summary>
				/// Draws a 16-bit bitmap (RGB565) with integer scaling.
				/// </summary>
				/// <param name="framebuffer">Target framebuffer to draw into.</param>
				/// <param name="x">Pixel-space origin X (pixels).</param>
				/// <param name="y">Pixel-space origin Y (pixels).</param>
				/// <param name="bitmap">Pointer to row-major bitmap pixels.</param>
				void Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const uint16_t* bitmap)
				{
					static constexpr uint16_t TransparentColor16 =
						Rgb::Color565From888(Rgb::R8(TransparentColor), Rgb::G8(TransparentColor), Rgb::B8(TransparentColor));

					TransformShader.Prepare(x, y);

					for (dimension_t row = 0; row < Height; row++)
					{
						for (dimension_t column = 0; column < Width; column++)
						{
							const uint16_t bitmapColor = BitmapReader::template ReadColorAt<dimension_t, Width>(bitmap, column, row);

							if (UseTransparentColor && (bitmapColor == TransparentColor16))
							{
								continue;
							}

							dimension_t tx = column;
							dimension_t ty = row;
							if (!TransformShader.Transform(tx, ty))
							{
								continue;
							}

							const pixel_t startX = static_cast<pixel_t>(x + (static_cast<pixel_t>(tx) * static_cast<pixel_t>(ScaleX)));
							const pixel_t startY = static_cast<pixel_t>(y + (static_cast<pixel_t>(ty) * static_cast<pixel_t>(ScaleY)));

							framebuffer->RectangleFill(ColorShader.Shade(Rgb::Color(bitmapColor)),
								startX,
								startY,
								static_cast<pixel_t>(startX + (ScaleX - 1)),
								static_cast<pixel_t>(startY + (ScaleY - 1)));
						}
					}
				}

				/// <summary>
				/// Draws a 32-bit bitmap with integer scaling.
				/// </summary>
				/// <param name="framebuffer">Target framebuffer to draw into.</param>
				/// <param name="x">Pixel-space origin X (pixels).</param>
				/// <param name="y">Pixel-space origin Y (pixels).</param>
				/// <param name="bitmap">Pointer to row-major bitmap pixels.</param>
				void Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const uint32_t* bitmap)
				{
					static constexpr uint32_t TransparentColor32 =
						Rgb::Color888From888(Rgb::R8(TransparentColor), Rgb::G8(TransparentColor), Rgb::B8(TransparentColor));

					TransformShader.Prepare(x, y);

					for (dimension_t row = 0; row < Height; row++)
					{
						for (dimension_t column = 0; column < Width; column++)
						{
							const uint32_t bitmapColor = BitmapReader::template ReadColorAt<dimension_t, Width>(bitmap, column, row);

							if (UseTransparentColor && (bitmapColor == TransparentColor32))
							{
								continue;
							}

							dimension_t tx = column;
							dimension_t ty = row;
							if (!TransformShader.Transform(tx, ty))
							{
								continue;
							}

							const pixel_t startX = static_cast<pixel_t>(x + (static_cast<pixel_t>(tx) * static_cast<pixel_t>(ScaleX)));
							const pixel_t startY = static_cast<pixel_t>(y + (static_cast<pixel_t>(ty) * static_cast<pixel_t>(ScaleY)));

							framebuffer->RectangleFill(ColorShader.Shade(Rgb::Color(bitmapColor)),
								startX,
								startY,
								static_cast<pixel_t>(startX + (ScaleX - 1)),
								static_cast<pixel_t>(startY + (ScaleY - 1)));
						}
					}
				}
			};

			/// <summary>
			/// Bitmap drawer alias that selects the non-scaled or scaled implementation.
			/// </summary>
			/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
			/// <typeparam name="Width">Bitmap width (pixels).</typeparam>
			/// <typeparam name="Height">Bitmap height (pixels).</typeparam>
			/// <typeparam name="BitmapReader">Bitmap reader type providing ReadColorAt(...).</typeparam>
			/// <typeparam name="ScaleX">Horizontal scale factor (integer).</typeparam>
			/// <typeparam name="ScaleY">Vertical scale factor (integer).</typeparam>
			/// <typeparam name="TransparentColor">Transparent key color (RGB888).</typeparam>
			/// <typeparam name="UseTransparentColor">Enables transparent key color skipping when true.</typeparam>
			/// <typeparam name="ColorShaderType">Color shader type providing Shade(color).</typeparam>
			/// <typeparam name="TransformShaderType">Transform shader type providing Prepare(...) and Transform(x, y).</typeparam>
			template<typename dimension_t,
				dimension_t Width,
				dimension_t Height,
				typename BitmapReader = Reader::Flash,
				uint8_t ScaleX = 1,
				uint8_t ScaleY = 1,
				rgb_color_t TransparentColor = RGB_COLOR_BLACK,
				bool UseTransparentColor = false,
				typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
				typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>
			>
			using TemplateDrawer = typename TypeTraits::TypeConditional::conditional_type<
				TemplateDrawerNoScale<dimension_t, Width, Height, BitmapReader, TransparentColor, UseTransparentColor, ColorShaderType, TransformShaderType>,
				TemplateDrawerIntegerScale<dimension_t, Width, Height, BitmapReader, ScaleX, ScaleY, TransparentColor, UseTransparentColor, ColorShaderType, TransformShaderType>,
				(ScaleX == 1 && ScaleY == 1)>::type;
		}
	}
}
#endif