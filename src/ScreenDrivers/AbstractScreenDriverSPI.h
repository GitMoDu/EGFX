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

	template<size_t bufferSize,
		const pixel_t width,
		const pixel_t height,
		const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST>
	class AbstractScreenDriverSPI : public IScreenDriver
	{
	public:
		static constexpr size_t BufferSize = bufferSize;
		static constexpr pixel_t ScreenWidth = width;
		static constexpr pixel_t ScreenHeight = height;

	protected:
		Egfx::SpiType& SpiInstance;

	public:
		AbstractScreenDriverSPI(Egfx::SpiType& spi)
			: IScreenDriver()
			, SpiInstance(spi)
		{
		}

		virtual void BufferTaskCallback(void* parameter) {}
		virtual void SetBufferTaskCallback(void (*taskCallback)(void* parameter)) {}

		virtual bool Start()
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

		virtual bool CanPushBuffer()
		{
			return true;
		}

		virtual uint32_t PushBuffer(const uint8_t* frameBuffer)
		{
			SpiTransfer(frameBuffer, bufferSize);

			return 0;
		}

		virtual bool PushingBuffer(const uint8_t* frameBuffer)
		{
			return false;
		}

		virtual void EndBuffer()
		{
			CommandEnd();
		}

		pixel_t GetScreenWidth() const final
		{
			return ScreenWidth;
		}

		pixel_t GetScreenHeight() const final
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

		void SpiTransfer(const uint8_t* data, const size_t size)
		{
#if defined(ARDUINO_ARCH_STM32F4)
			SpiInstance.transfer((uint8_t*)data, (uint32_t)size);
#elif defined(ARDUINO_ARCH_STM32)
			SpiInstance.transfer((void*)data, size, true);
#else
			SpiInstance.transfer((void*)data, size);
#endif
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
}
#endif