// AbstractScreenDriverSPI.h

#ifndef _ABSTRACT_SCREEN_DRIVER_SPI_h
#define _ABSTRACT_SCREEN_DRIVER_SPI_h

#include "../Model/IScreenDriver.h"

#include <SPI.h>

template<
	size_t bufferSize,
	const uint8_t width,
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
	static constexpr size_t BufferSize = bufferSize;
	static constexpr uint8_t ScreenWidth = width;
	static constexpr uint8_t ScreenHeight = height;

protected:
	static constexpr uint32_t SPISpeed = spiSpeed;
	static constexpr uint8_t SPIChannel = spiChannel;

protected:
#if defined(ARDUINO_ARCH_RP2040)
	SPIClassRP2040 SpiInstance;
#elif defined(ARDUINO_ARCH_NRF52)
	SPIClass& SpiInstance;
#else
	SPIClass SpiInstance;
#endif

public:
	AbstractScreenDriverSPI()
		: IScreenDriver()
#if defined(ARDUINO_ARCH_STM32)
		, SpiInstance(pinMOSI, PNUM_NOT_DEFINED, pinCLK, pinCS)
#elif defined(ARDUINO_ARCH_ESP32)
		, SpiInstance(spiChannel)
#elif defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F4)
		, SpiInstance(spiChannel)
#elif defined(ARDUINO_ARCH_RP2040)
		, SpiInstance(GetSpiHost(), UINT8_MAX, pinCS, pinCLK, pinMOSI)
#elif defined(ARDUINO_ARCH_NRF52)
		, SpiInstance(GetSpiHost())
#else
		, SpiInstance()
#endif
	{}

	virtual const bool Start()
	{
		if (pinDC == UINT8_MAX
			|| pinCS == UINT8_MAX)
		{
			return false;
		}

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
#elif defined(ARDUINO_ARCH_NRF52)
		if (pinCLK != UINT8_MAX
			&& pinMOSI != UINT8_MAX)
		{
			SpiInstance.begin();
			SpiInstance.setPins(UINT8_MAX, pinCLK, pinMOSI);
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

	virtual const bool CanPushBuffer()
	{
		return true;
	}

	virtual const uint32_t PushBuffer(const uint8_t* frameBuffer)
	{
#if defined(ARDUINO_ARCH_STM32F4)
		SpiInstance.transfer((uint8_t*)frameBuffer, (uint32_t)bufferSize);
#elif defined(ARDUINO_ARCH_STM32)
		SpiInstance.transfer((void*)frameBuffer, bufferSize, true);
#else
		SpiInstance.transfer((void*)frameBuffer, bufferSize);
#endif

		return 0;
	}

	virtual const bool PushingBuffer(const uint8_t* frameBuffer)
	{
		return false;
	}

	virtual void EndBuffer()
	{
		CommandEnd();
	}

	const uint8_t GetWidth() const final
	{
		return ScreenWidth;
	}

	const uint8_t GetHeight() const final
	{
		return ScreenHeight;
	}

protected:
	void PinReset(const uint32_t waitPeriod = 10)
	{
		pinMode(pinDC, OUTPUT);
		digitalWrite(pinDC, LOW);
		digitalWrite(pinCS, HIGH);
		pinMode(pinCS, OUTPUT);

		if (pinRST != UINT8_MAX)
		{
			digitalWrite(pinRST, HIGH);
			pinMode(pinRST, OUTPUT);
			delayMicroseconds(waitPeriod);
			digitalWrite(pinRST, LOW);
			delayMicroseconds(waitPeriod);
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

private:
#if defined(ARDUINO_ARCH_RP2040)
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
#elif defined(ARDUINO_ARCH_NRF52)
	SPIClass& GetSpiHost()
	{
		if (spiChannel == 1)
		{
			return SPI1;
		}
		else
		{
			return SPI;
		}
	}
#endif
};

#endif