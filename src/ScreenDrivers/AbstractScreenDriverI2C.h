// AbstractScreenDriverI2C.h

#ifndef _ABSTRACT_SCREEN_DRIVER_I2C_h
#define _ABSTRACT_SCREEN_DRIVER_I2C_h

#include "../Model/IScreenDriver.h"

#include <Wire.h>

template<const uint8_t width,
	const uint8_t height,
	const uint8_t pinSCL,
	const uint8_t pinSDA,
	const uint8_t pinRST,
	const uint8_t i2cAddress,
	const uint8_t i2cChannel,
	const uint32_t i2cSpeed>
class AbstractScreenDriverI2C : public virtual IScreenDriver
{
public:
	static constexpr uint8_t ScreenWidth = width;
	static constexpr uint8_t ScreenHeight = height;

public:
	static constexpr uint8_t I2CAddress = i2cAddress;
	static constexpr uint8_t I2CChannel = i2cChannel;
	static constexpr uint32_t I2CSpeed = i2cSpeed;

protected:
	TwoWire WireInstance;

public:
	AbstractScreenDriverI2C()
		: IScreenDriver()
#if defined(ARDUINO_ARCH_ESP32)
		, WireInstance(i2cChannel)
#elif defined(ARDUINO_ARCH_STM32F1)
		, WireInstance(i2cChannel)
#else
		, WireInstance()
#endif
	{}

	//TODO: Optional in-driver I2C begin.
	virtual const bool Start()
	{
		PinReset();

#if defined(ARDUINO_ARCH_ESP32)
		return WireInstance.begin(pinSDA, pinSCL, I2CSpeed);
#else
		WireInstance.begin();
		if (I2CSpeed > 0)
		{
			WireInstance.setClock(i2cSpeed);
		}

		return true;
#endif
	}

	virtual const uint8_t GetWidth() final
	{
		return ScreenWidth;
	}

	virtual const uint8_t GetHeight() final
	{
		return ScreenHeight;
	}

protected:
	void PinReset()
	{
		if (pinRST != UINT8_MAX)
		{
			digitalWrite(pinRST, HIGH);
			pinMode(pinRST, OUTPUT);
			delayMicroseconds(10);
			digitalWrite(pinRST, LOW);
			delayMicroseconds(10);
			digitalWrite(pinRST, HIGH);
		}
	}
};
#endif