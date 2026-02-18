#ifndef _EGFX_I_FRAME_BUFFER_h
#define _EGFX_I_FRAME_BUFFER_h

#include "../Platform/Platform.h"
#include "RgbColor.h"
#include "DisplayOptions.h"

namespace Egfx
{
	class IFrameBuffer
	{
	public:
		/// <summary>
		/// Exposes the frame buffer for pushing to IScreenDriver.
		/// </summary>
		/// <returns>Pointer to framebuffer.</returns>
		virtual uint8_t* GetFrameBuffer() const = 0;

		/// <summary>
		/// Flips the underlying buffer. No-op for single framesbuffers.
		/// </summary>
		virtual bool Flip() = 0;

		/// <summary>
		/// Clear the frame buffer.
		/// </summary>
		virtual bool ClearFrameBuffer() = 0;

		/// <summary>
		/// Fill the whole frame with the provided color.
		/// </summary>
		/// <param name="color">The color for filling.</param>
		virtual void Fill(const rgb_color_t color) = 0;

		/// <summary>
		/// Draw a single pixel.
		/// </summary>
		/// <param name="color">The pixel color.</param>
		virtual void Pixel(const rgb_color_t color, const pixel_t x, const pixel_t y) = 0;
		virtual void Pixel(const rgb_color_t color, const pixel_point_t point) = 0;

		/// <summary>
		/// Blend a pixel color with 50/50 split.
		/// </summary>
		/// <param name="color">The color to blend with the pixel.</param>
		virtual void PixelBlend(const rgb_color_t color, const pixel_t x, const pixel_t y) = 0;
		virtual void PixelBlend(const rgb_color_t color, const pixel_point_t point) = 0;

		/// <summary>
		/// Blend a pixel color with given alpha split.
		/// </summary>
		/// <param name="color">The color to blend with the pixel.</param>
		/// <param name="alpha">The alpha value (opacity) to use for blending, typically in the range 0 (fully transparent) to 255 (fully opaque).</param>
		virtual void PixelBlendAlpha(const rgb_color_t color, const pixel_t x, const pixel_t y, const uint8_t alpha) = 0;
		virtual void PixelBlendAlpha(const rgb_color_t color, const pixel_point_t point, const uint8_t alpha) = 0;

		/// <summary>
		/// Blend a pixel color using color add.
		/// </summary>
		/// <param name="color">The color to add into the pixel.</param>
		virtual void PixelBlendAdd(const rgb_color_t color, const pixel_t x, const pixel_t y) = 0;
		virtual void PixelBlendAdd(const rgb_color_t color, const pixel_point_t point) = 0;

		/// <summary>
		/// Blend a pixel color using color subtract.
		/// </summary>
		/// <param name="color">The color to subtract from the pixel.</param>
		virtual void PixelBlendSubtract(const rgb_color_t color, const pixel_t x, const pixel_t y) = 0;
		virtual void PixelBlendSubtract(const rgb_color_t color, const pixel_point_t point) = 0;

		/// <summary>
		/// Blend a pixel color using color multiply.
		/// </summary>
		/// <param name="color">The color to multiply with the pixel.</param>
		virtual void PixelBlendMultiply(const rgb_color_t color, const pixel_t x, const pixel_t y) = 0;
		virtual void PixelBlendMultiply(const rgb_color_t color, const pixel_point_t point) = 0;

		/// <summary>
		/// Blend a pixel color using color screen.
		/// Screen blending is a special case of color addition that produces a lighter color.
		/// </summary>
		/// <param name="color">The color to screen with the pixel.</param>
		virtual void PixelBlendScreen(const rgb_color_t color, const pixel_t x, const pixel_t y) = 0;
		virtual void PixelBlendScreen(const rgb_color_t color, const pixel_point_t point) = 0;

		/// <summary>
		/// Draw a horizontal line between two points.
		/// </summary>
		/// <param name="color">The pixel color.</param>
		virtual void LineHorizontal(const rgb_color_t color, const pixel_t x1, const pixel_t x2, const pixel_t y) = 0;

		/// <summary>
		/// Draw a vertical line between two points.
		/// </summary>
		/// <param name="color">The pixel color.</param>
		virtual void LineVertical(const rgb_color_t color, const pixel_t x, const pixel_t y1, const pixel_t y2) = 0;

		/// <summary>
		/// Draw a full rectangle.
		/// </summary>
		/// <param name="color">The fill color.</param>
		virtual void RectangleFill(const rgb_color_t color, const pixel_t topLeftX, const pixel_t topLeftY, const pixel_t bottomRightX, const pixel_t bottomRightY) = 0;
		virtual void RectangleFill(const rgb_color_t color, const pixel_rectangle_t& rectangle) = 0;
	};
}
#endif