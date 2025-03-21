// IFrameBuffer.h

#ifndef _EGFX_I_FRAME_BUFFER_h
#define _EGFX_I_FRAME_BUFFER_h

#include <stdint.h>
#include "RgbColor.h"
#include "DisplayOrientation.h"

namespace Egfx
{
	/// <summary>
	/// Point defined by two pixel coordinates.
	/// </summary>
	struct pixel_point_t
	{
		pixel_t x;
		pixel_t y;
	};

	/// <summary>
	/// Line segment defined by start and end points.
	/// </summary>
	struct pixel_line_t
	{
		pixel_point_t start;
		pixel_point_t end;
	};

	/// <summary>
	/// Triangle defined by three points.
	/// </summary>
	struct pixel_triangle_t
	{
		pixel_point_t a;
		pixel_point_t b;
		pixel_point_t c;
	};

	/// <summary>
	/// Rectangle defined by top-left and bottom-right points.
	/// </summary>
	struct pixel_rectangle_t
	{
		pixel_point_t topLeft;
		pixel_point_t bottomRight;
	};

	class IFrameBuffer
	{
		//Experimental, optional fast direct copy of bitmap to framebuffer.
	private:
		virtual void Bitmap565(const uint16_t* bitmap565, const pixel_t x, const pixel_t y, const pixel_t width, const pixel_t height) {}
		virtual void Bitmap332(const uint8_t* bitmap332, const pixel_t x, const pixel_t y, const pixel_t width, const pixel_t height) {}
		virtual void Bitmap1x8(const uint8_t* bitmask1x8, const pixel_t x, const pixel_t y, const pixel_t width, const pixel_t height) {}
		virtual void Bitmap8x1(const uint8_t* bitmask8x1, const pixel_t x, const pixel_t y, const pixel_t width, const pixel_t height) {}


	public:
		virtual const pixel_t GetFrameWidth() const { return 0; }
		virtual const pixel_t GetFrameHeight() const { return 0; }

		virtual const bool IsMonochrome() const { return false; }
		virtual const uint8_t GetColorDepth() const { return 0; }

		/// <summary>
		/// Exposes the frame buffer for pushing to IScreenDriver.
		/// </summary>
		/// <returns>Pointer to framebuffer.</returns>
		virtual const uint8_t* GetFrameBuffer() { return nullptr; }

		/// <summary>
		/// Clear the frame buffer.
		/// </summary>
		virtual const bool ClearFrameBuffer() { return false; }

		/// <summary>
		/// Set inverted color for frame-buffer.
		/// </summary>
		/// <param name="inverted"></param>
		virtual void SetInverted(const bool inverted) {}

		/// <summary>
		/// Fill the whole frame with the provided color.
		/// </summary>
		/// <param name="color"></param>
		virtual void Fill(const rgb_color_t color) {}

		/// <summary>
		/// Draw a single pixel.
		/// </summary>
		/// <param name="color"></param>
		/// <param name="x"></param>
		/// <param name="y"></param>
		virtual void Pixel(const rgb_color_t color, const pixel_t x, const pixel_t y) {}
		virtual void Pixel(const rgb_color_t color, const pixel_point_t point) {}

		/// <summary>
		/// Draw a line. 
		/// Special orientations (horizontal and vertical) are internally optimized.
		/// </summary>
		/// <param name="color"></param>
		/// <param name="x1"></param>
		/// <param name="y1"></param>
		/// <param name="x2"></param>
		/// <param name="y2"></param>
		virtual void Line(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2) {}
		virtual void Line(const rgb_color_t color, const pixel_line_t& line) {}

		/// <summary>
		/// Draw a rectangle outline.
		/// </summary>
		/// <param name="color"></param>
		/// <param name="x1"></param>
		/// <param name="y1"></param>
		/// <param name="x2"></param>
		/// <param name="y2"></param>
		virtual void Rectangle(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2) {}
		virtual void Rectangle(const rgb_color_t color, const pixel_rectangle_t& rectangle) {}

		/// <summary>
		/// Draw a full rectangle.
		/// </summary>
		/// <param name="color"></param>
		/// <param name="x1"></param>
		/// <param name="y1"></param>
		/// <param name="x2"></param>
		/// <param name="y2"></param>
		virtual void RectangleFill(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2) {}
		virtual void RectangleFill(const rgb_color_t color, const pixel_rectangle_t& rectangle) {}

		/// <summary>
		/// Draw a triangle outline.
		/// </summary>
		/// <param name="color"></param>
		/// <param name="x1"></param>
		/// <param name="y1"></param>
		/// <param name="x2"></param>
		/// <param name="y2"></param>
		/// <param name="x3"></param>
		/// <param name="y3"></param>
		virtual void Triangle(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2, const pixel_t x3, const pixel_t y3) {}
		virtual void Triangle(const rgb_color_t color, const pixel_triangle_t& triangle) {}


		/// <summary>
		/// Draw a full triangle.
		/// </summary>
		/// <param name="color"></param>
		/// <param name="x1"></param>
		/// <param name="y1"></param>
		/// <param name="x2"></param>
		/// <param name="y2"></param>
		/// <param name="x3"></param>
		/// <param name="y3"></param>
		virtual void TriangleFill(const rgb_color_t color, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2, const pixel_t x3, const pixel_t y3) {}
		virtual void TriangleFill(const rgb_color_t color, const pixel_triangle_t& triangle) {}

	};
}
#endif