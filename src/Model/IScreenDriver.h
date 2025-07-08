#ifndef _I_SCREEN_DRIVER_h
#define _I_SCREEN_DRIVER_h

#include "Model/RgbColor.h"
#include "Model/GraphicsBuffer.h"

namespace Egfx
{
	struct IScreenDriver
	{
		virtual bool CanPushBuffer() { return  false; }
		virtual void StartBuffer() {}
		virtual uint32_t PushBuffer(const uint8_t* frameBuffer) { return 0; }
		virtual bool PushingBuffer(const uint8_t* frameBuffer) { return false; }
		virtual void EndBuffer() {}

		virtual pixel_t GetScreenWidth() const { return 0; }
		virtual pixel_t GetScreenHeight() const { return 0; }

		virtual bool Start() { return false; }

		virtual void Stop() {}

		virtual void SetBrightness(const uint8_t brightness) {}

#if defined(ARDUINO_ARCH_NRF52)
		virtual void SetBufferTaskCallback(SchedulerRTOS::taskfunc_t taskCallback) {}
#else
		virtual void SetBufferTaskCallback(void (*taskCallback)(void* parameter)) {}
#endif

		virtual void BufferTaskCallback(void* parameter) {}
	};
}
#endif