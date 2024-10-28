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
#if defined(ARDUINO_ARCH_NRF52)
	TwoWire& WireInstance;
#else
	TwoWire WireInstance;
#endif

public:
	AbstractScreenDriverI2C()
		: IScreenDriver()
#if defined(ARDUINO_ARCH_ESP32)
		, WireInstance(i2cChannel)
#elif defined(ARDUINO_ARCH_STM32)
		, WireInstance(pinSDA, pinSCL)
#elif defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F4)
		, WireInstance(i2cChannel)
#elif defined(ARDUINO_ARCH_RP2040)
		, WireInstance(GetI2cHost(), pinSDA, pinSCL)
#elif defined(ARDUINO_ARCH_NRF52)
		, WireInstance(GetI2cHost())
#else
		, WireInstance()
#endif
	{}

	//TODO: Optional in-driver I2C begin.
	virtual const bool Start()
	{
#if defined(ARDUINO_ARCH_RP2040)
		if (GetI2cHost() == nullptr)
		{
			return false;
		}
#endif

		PinReset();

#if defined(ARDUINO_ARCH_ESP32)
		return WireInstance.begin(pinSDA, pinSCL, I2CSpeed);
#else

		WireInstance.begin();

		if (I2CSpeed > 0)
		{
			WireInstance.setClock(i2cSpeed);
		}

#if defined(ARDUINO_ARCH_NRF52)
		if (pinSDA != UINT8_MAX
			&& pinSCL != UINT8_MAX)
		{
			Wire.setPins(pinSDA, pinSCL);
		}

#endif

		return true;
#endif
	}

	virtual const uint8_t GetWidth() const final
	{
		return ScreenWidth;
	}

	virtual const uint8_t GetHeight() const final
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

#if defined(ARDUINO_ARCH_RP2040)
private:
	i2c_inst_t* GetI2cHost()
	{
		if (i2cChannel == 0)
		{
			return i2c0;
		}
		else if (i2cChannel == 1)
		{
			return i2c1;
		}
		else
		{
			return nullptr;
		}
	}
#elif defined(ARDUINO_ARCH_NRF52)
private:
	TwoWire& GetI2cHost()
	{
		if (i2cChannel == 1)
		{
			return Wire1;
		}
		else
		{
			return Wire;
		}
	}
#endif
};
#endif