// IScreenDriver.h

#ifndef _I_SCREEN_DRIVER_h
#define _I_SCREEN_DRIVER_h

#include <stdint.h>

#include "GraphicsBuffer.h"

class IScreenDriver
{
public:
	virtual const bool CanPushBuffer() { return  false; }
	virtual void StartBuffer() {}
	virtual const uint32_t PushBuffer(const uint8_t* frameBuffer) { return 0; }
	virtual const bool PushingBuffer(const uint8_t* frameBuffer) { return false; }
	virtual void EndBuffer() {}

	virtual const uint8_t GetWidth() const { return 0; }
	virtual const uint8_t GetHeight() const { return 0; }

	virtual const bool Start() { return false; }

	virtual void Stop() {}

	virtual void SetBrightness(const uint8_t brightness) {}

	virtual void SetBufferTaskCallback(void (*taskCallback)(void* parameter)) {}

	virtual void BufferTaskCallback(void* parameter) {}
};
#endif