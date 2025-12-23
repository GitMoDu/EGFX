#ifndef _EGFX_I_SCREEN_DRIVER_h
#define _EGFX_I_SCREEN_DRIVER_h

#include "Model/RgbColor.h"
#include "Model/GraphicsBuffer.h"

namespace Egfx
{
	/// <summary>
	/// Platform display communications interface type enumeration.
	/// </summary>
	enum class InterfaceTypeEnum : uint8_t
	{
		I2C,
		SPI,
		SPI_QUAD,
		SPI_OCTAL,
		PARALLEL,
		EnumCount
	};

	/// <summary>
	/// Abstract compile time type selector for display communications interface type.
	/// Implementations provided for each InterfaceTypeEnum value.
	/// </summary>
	/// <typeparam name=""></typeparam>
	template<InterfaceTypeEnum> struct InterfaceSelector;

	struct IScreenDriver
	{
		virtual bool CanPushBuffer() = 0;
		virtual void StartBuffer() = 0;
		virtual uint32_t PushBuffer(const uint8_t* frameBuffer) = 0;
		virtual bool PushingBuffer(const uint8_t* frameBuffer) = 0;
		virtual void EndBuffer() = 0;

		virtual pixel_t GetScreenWidth() const = 0;
		virtual pixel_t GetScreenHeight() const = 0;

		virtual bool Start() = 0;

		virtual void Stop() = 0;

		//virtual void SetBrightness(const uint8_t brightness) {} //TODO:

		virtual void SetBufferTaskCallback(void (*taskCallback)(void* parameter)) = 0;

		virtual void BufferTaskCallback(void* parameter) = 0;
	};
}
#endif