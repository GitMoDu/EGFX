// AbstractScreenDriverSPI.h

#ifndef _ABSTRACT_SCREEN_DRIVER_SPI_h
#define _ABSTRACT_SCREEN_DRIVER_SPI_h

#include "../Model/IScreenDriver.h"

#include <SPI.h>

template<const uint8_t width,
	const uint8_t height,
	const uint8_t pinDC,
	const uint8_t pinCS,
	const uint8_t pinRST,
	const uint8_t pinCLK,
	const uint8_t pinMOSI,
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

protected:
#if defined(ARDUINO_ARCH_RP2040)
	SPIClassRP2040 SpiInstance;
#else
	SPIClass SpiInstance;
#endif


public:
	AbstractScreenDriverSPI()
		: IScreenDriver()
#if defined(ARDUINO_ARCH_ESP32)
		, SpiInstance(spiChannel)
#elif defined(ARDUINO_ARCH_STM32F1)
		, SpiInstance(spiChannel)
#elif defined(ARDUINO_ARCH_RP2040)
		, SpiInstance(GetSpiHost(), UINT8_MAX, pinCS, pinCLK, pinMOSI)
#else
		, SpiInstance()
#endif
	{}

	//TODO: Optional in-driver I2C begin.
	virtual const bool Start()
	{
		if (pinDC == UINT8_MAX
			|| pinCS == UINT8_MAX)
		{
			return false;
		}

		PinReset();

		//TODO: Optional in-driver SPI begin.
#if defined(ARDUINO_ARCH_ESP32)
		if (pinCS != UINT8_MAX
			&& pinCLK != UINT8_MAX
			&& pinMOSI != UINT8_MAX)
		{
			SpiInstance.begin((int8_t)pinCLK, (int8_t)-1, (int8_t)pinMOSI, (int8_t)pinCS);
		}
		else if (pinCS != UINT8_MAX && pinCLK != UINT8_MAX)
		{
			SpiInstance.begin((int8_t)pinCLK, (int8_t)-1, (int8_t)-1, (int8_t)pinCS);
		}
		else if (pinCS != UINT8_MAX)
		{
			SpiInstance.begin((int8_t)-1, (int8_t)-1, (int8_t)-1, (int8_t)pinCS);
		}
#else
		SpiInstance.begin();
#endif
		return true;
	}

	virtual void Stop()
	{
		SpiInstance.end();
		if (pinDC != UINT8_MAX)
		{
			digitalWrite(pinDC, LOW);
		}

		if (pinCS != UINT8_MAX)
		{
			digitalWrite(pinCS, HIGH);
		}

		if (pinRST != UINT8_MAX)
		{
			digitalWrite(pinRST, LOW);
		}
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
		pinMode(pinDC, OUTPUT);
		digitalWrite(pinDC, LOW);
		digitalWrite(pinCS, HIGH);
		pinMode(pinCS, OUTPUT);

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

	void CommandStart(SPISettings& settings)
	{
		digitalWrite(pinCS, LOW);
		digitalWrite(pinDC, LOW);
		SpiInstance.beginTransaction(settings);
	}

	void CommandEnd()
	{
		SpiInstance.endTransaction();
		digitalWrite(pinCS, HIGH);
		digitalWrite(pinDC, LOW);
	}

#if defined(ARDUINO_ARCH_RP2040)
private:
	spi_inst_t* GetSpiHost()
	{
		if (spiChannel == 0)
		{
			return spi0;
		}
		else if (spiChannel == 1)
		{
			return spi1;
		}
		else
		{
			return nullptr;
		}
	}
#endif
};
#endif