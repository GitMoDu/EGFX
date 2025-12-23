#ifndef _ABSTRACT_SCREEN_DRIVER_I2C_h
#define _ABSTRACT_SCREEN_DRIVER_I2C_h

#include "../Model/IScreenDriver.h"

#include <Wire.h>

namespace Egfx
{
	/// <summary>
	/// Platform I2C type.
	/// </summary>
	using WireType = TwoWire;

	// Selector for the screen communications hardware instance.
	template<> struct InterfaceSelector<InterfaceTypeEnum::I2C>
	{
		using Type = WireType;
	};

	template<const pixel_t width,
		const pixel_t height,
		const uint8_t i2cAddress>
	class AbstractScreenDriverI2C : public IScreenDriver
	{

	public:
		using InterfaceType = WireType;

	public:
		static constexpr uint8_t ScreenWidth = width;
		static constexpr uint8_t ScreenHeight = height;

	public:
		static constexpr uint8_t I2CAddress = i2cAddress;

	protected:
		WireType& WireInstance;

	public:
		AbstractScreenDriverI2C(WireType& wire)
			: IScreenDriver()
			, WireInstance(wire)
		{
		}

		virtual void BufferTaskCallback(void* /*parameter*/) {}
		virtual void SetBufferTaskCallback(void (*/*taskCallback*/)(void* parameter)) {}

		virtual bool Start()
		{
			return true;
		}

		virtual pixel_t GetScreenWidth() const final
		{
			return ScreenWidth;
		}

		virtual pixel_t GetScreenHeight() const final
		{
			return ScreenHeight;
		}
	};
}
#endif