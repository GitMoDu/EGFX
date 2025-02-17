// AbstractScreenDriverI2C.h

#ifndef _ABSTRACT_SCREEN_DRIVER_I2C_h
#define _ABSTRACT_SCREEN_DRIVER_I2C_h

#include "../Model/IScreenDriver.h"

#include <Wire.h>

using namespace Egfx;

namespace Egfx
{
	template<const pixel_t width,
		const pixel_t height,
		const uint8_t i2cAddress>
	class AbstractScreenDriverI2C : public virtual IScreenDriver
	{
	public:
		static constexpr uint8_t ScreenWidth = width;
		static constexpr uint8_t ScreenHeight = height;

	public:
		static constexpr uint8_t I2CAddress = i2cAddress;

	protected:
		TwoWire& WireInstance;

	public:
		AbstractScreenDriverI2C(TwoWire& wire)
			: IScreenDriver()
			, WireInstance(wire)
		{
		}

		virtual const bool Start()
		{
			return true;
		}

		virtual const pixel_t GetScreenWidth() const final
		{
			return ScreenWidth;
		}

		virtual const pixel_t GetScreenHeight() const final
		{
			return ScreenHeight;
		}
	};
}
#endif