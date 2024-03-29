// AbstractScreenDriverSPI.h

#ifndef _ABSTRACT_SCREEN_DRIVER_SPI_h
#define _ABSTRACT_SCREEN_DRIVER_SPI_h

#include "../Model/IScreenDriver.h"

#include <SPI.h>

template<const uint8_t width,
	const uint8_t height,
	const uint8_t pinDC,
	const uint8_t pinCS,
	const uint8_t pinCLK,
	const uint8_t pinMOSI,
	const uint8_t pinRST,
	const uint8_t spiChannel,
	const uint32_t spiSpeed>
class AbstractScreenDriverSPI : public virtual IScreenDriver
{
public:
	static constexpr uint8_t ScreenWidth = width;
	static constexpr uint8_t ScreenHeight = height;

protected:
	static constexpr uint8_t SPIChannel = spiChannel;
	static constexpr uint32_t SPISpeed = spiSpeed;

	static constexpr uint32_t CLOCK_DIVIDER = F_CPU / SPISpeed;


protected:
	SPIClass SpiInstance;

public:
	AbstractScreenDriverSPI()
		: IScreenDriver()
#if defined(ARDUINO_ARCH_ESP32)
		, SpiInstance(pinCLK, pinMOSI, -1)
#elif defined(ARDUINO_ARCH_STM32F1)
		, SpiInstance(SPIChannel)
#else
		, SpiInstance()
#endif
	{}

	//TODO: Optional in-driver I2C begin.
	virtual const bool Start()
	{
		digitalWrite(pinRST, LOW);
		digitalWrite(pinDC, LOW);
		digitalWrite(pinCS, LOW);
		pinMode(pinRST, OUTPUT);
		pinMode(pinDC, OUTPUT);
		pinMode(pinCS, OUTPUT);

		PinReset();

		//TODO: Optional in-driver SPI begin.
#if defined(ARDUINO_ARCH_ESP32)
		return SpiInstance.begin(pinCS, pinCLK, pinMOSI, -1);
#elif defined(ARDUINO_ARCH_STM32F1)
		SpiInstance.begin();
		//SpiInstance.begin(BitOrder::LSBFIRST, 0);
		/*	if (spiSpeed > 0)
			{
				SpiInstance.setClockDivider(CLOCK_DIVIDER);
			}*/

		return true;
#else
		SpiInstance.begin();
		if (spiSpeed > 0)
		{
			SpiInstance.setClock(spiSpeed);
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