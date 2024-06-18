// IFrameBuffer.h

#ifndef _I_FRAME_BUFFER_h
#define _I_FRAME_BUFFER_h

#include <stdint.h>
#include "RgbColor.h"
#include "DisplayOrientation.h"

class IFrameBuffer
{
public:
	virtual const uint8_t GetWidth() { return 0; }
	virtual const uint8_t GetHeight() { return 0; }

	/// <summary>
	/// Exposes the frame buffer for pushing to IScreenDriver.
	/// </summary>
	/// <returns></returns>
	virtual const uint8_t* GetFrameBuffer() { return nullptr; }

	/// <summary>
	/// Clear the frame buffer.
	/// </summary>
	virtual void ClearFrameBuffer() {}

	/// <summary>
	/// Set inverted color for frame-buffer.
	/// </summary>
	/// <param name="inverted"></param>
	virtual void SetInverted(const bool inverted) {}

	// Drawing primitives.

	/// <summary>
	/// Fill the whole framebuffer with the provided color.
	/// </summary>
	/// <param name="color"></param>
	virtual void Fill(const RgbColor& color) {}

	/// <summary>
	/// Draw a single pixel.
	/// </summary>
	/// <param name="color"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	virtual void Pixel(const RgbColor& color, const uint8_t x, const uint8_t y) {}

	/// <summary>
	/// Draw a line. 
	/// Special orientations (horizontal and vertical) are internally optimized.
	/// </summary>
	/// <param name="color"></param>
	/// <param name="x1"></param>
	/// <param name="y1"></param>
	/// <param name="x2"></param>
	/// <param name="y2"></param>
	virtual void Line(const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2) {}

	/// <summary>
	/// Draw a rectangle outline.
	/// </summary>
	/// <param name="color"></param>
	/// <param name="x1"></param>
	/// <param name="y1"></param>
	/// <param name="x2"></param>
	/// <param name="y2"></param>
	virtual void Rectangle(const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2) {}

	/// <summary>
	/// Draw a full rectangle.
	/// </summary>
	/// <param name="color"></param>
	/// <param name="x1"></param>
	/// <param name="y1"></param>
	/// <param name="x2"></param>
	/// <param name="y2"></param>
	virtual void RectangleFill(const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2) {}
	
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
	virtual void Triangle(const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const uint8_t x3, const uint8_t y3) {}

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
	virtual void TriangleFill(const RgbColor& color, const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const uint8_t x3, const uint8_t y3) {}
};
#endif