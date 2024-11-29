// AbstractScreenDriverSPI.h

#ifndef _ABSTRACT_SCREEN_DRIVER_SPI_h
#define _ABSTRACT_SCREEN_DRIVER_SPI_h

#include "../Model/IScreenDriver.h"

#include <SPI.h>

namespace Egfx
{
#if defined(ARDUINO_ARCH_RP2040)
	/// <summary>
	/// Platform SPI type. Required to access DMA functions.
	/// </summary>
	using SpiType = SPIClassRP2040;
#else
	/// <summary>
	/// Platform SPI type. Required to access DMA functions.
	/// </summary>
	using SpiType = SPIClass;
#endif
}

template<size_t bufferSize,
	const uint8_t width,
	const uint8_t height,
	const uint8_t pinCS,
	const uint8_t pinDC,
	const uint8_t pinRST>
class AbstractScreenDriverSPI : public virtual IScreenDriver
{
public:
	static constexpr size_t BufferSize = bufferSize;
	static constexpr uint8_t ScreenWidth = width;
	static constexpr uint8_t ScreenHeight = height;

protected:
	Egfx::SpiType& SpiInstance;

public:
	AbstractScreenDriverSPI(Egfx::SpiType& spi)
		: IScreenDriver()
		, SpiInstance(spi)
	{
	}

	virtual const bool Start()
	{
		if (pinDC == UINT8_MAX)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	virtual void Stop()
	{
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

	const uint8_t GetScreenWidth() const final
	{
		return ScreenWidth;
	}

	const uint8_t GetScreenHeight() const final
	{
		return ScreenHeight;
	}

protected:
	void PinReset(const uint32_t waitPeriod = 10)
	{
		pinMode(pinDC, OUTPUT);
		digitalWrite(pinDC, LOW);
		if (pinCS != UINT8_MAX)
		{
			digitalWrite(pinCS, HIGH);
			pinMode(pinCS, OUTPUT);
		}

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
		if (pinCS != UINT8_MAX)
		{
			digitalWrite(pinCS, LOW);
		}
		digitalWrite(pinDC, LOW);
		SpiInstance.beginTransaction(settings);
	}

	void CommandEnd()
	{
		SpiInstance.endTransaction();
		if (pinCS != UINT8_MAX)
		{
			digitalWrite(pinCS, HIGH);
		}
		digitalWrite(pinDC, LOW);
	}
};

#endif