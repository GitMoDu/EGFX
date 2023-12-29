// IFramePrimitives.h

#ifndef _I_FRAME_PRIMITIVES_h
#define _I_FRAME_PRIMITIVES_h

#include "IFrameBuffer.h"

class IFramePrimitives : public virtual IFrameBuffer
{
public:
	IFramePrimitives() : IFrameBuffer() {}

public:
	virtual void Pixel(const uint8_t x, const uint8_t y, const RgbColor& color) {}
	virtual void Line(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const RgbColor& color) {}
	virtual void RectangleFill(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const RgbColor& color) {}
	virtual void Rectangle(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const RgbColor& color, const uint8_t width) {}
	virtual void CircleFill(const uint8_t xCenter, const uint8_t yCenter, const uint8_t radius, const RgbColor& color) {}
	virtual void Circle(const uint8_t xCenter, const uint8_t yCenter, const uint8_t radius, const RgbColor& color) {}
};
#endif