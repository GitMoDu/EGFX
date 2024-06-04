// ScreenDriverSSD1306_SPI.h

#ifndef _SCREEN_DRIVER_SSD1306_SPI_h
#define _SCREEN_DRIVER_SSD1306_SPI_h

#include "AbstractScreenDriverSPI.h"
#include "TemplateScreenDriverRtos.h"
#include "TemplateScreenDriverSpiAsync.h"
#include "TemplateScreenDriverSpiDma.h"
#include "SSD1306\SSD1306.h"

template<const uint8_t width,
	const uint8_t height,
	const uint8_t pinDC,
	const uint8_t pinCS,
	const uint8_t pinRST,
	const uint8_t pinCLK,
	const uint8_t pinMOSI,
	const uint8_t spiChannel,
	const uint32_t spiSpeed>
class AbstractScreenDriverSSD1306_SPI : public AbstractScreenDriverSPI<GraphicsBuffer::GetMonochromeBufferSize(width, height), width, height, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>
{
private:
	using BaseClass = AbstractScreenDriverSPI<GraphicsBuffer::GetMonochromeBufferSize(width, height), width, height, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>;

public:
	using BaseClass::ScreenWidth;
	using BaseClass::ScreenHeight;
	using BaseClass::BufferSize;

protected:
	using BaseClass::SpiInstance;
	using BaseClass::CommandStart;
	using BaseClass::CommandEnd;
	using BaseClass::PinReset;

private:
	SPISettings Settings;

public:
	AbstractScreenDriverSSD1306_SPI()
		: BaseClass()
		, Settings(spiSpeed, MSBFIRST, SPI_MODE0)
	{}

	virtual void StartBuffer()
	{
		CommandStart(Settings);
		SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::CommandStart);

		SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::Column);
		SpiInstance.transfer((uint8_t)0);
		SpiInstance.transfer((uint8_t)(ScreenWidth - 1));

		SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::Page);
		SpiInstance.transfer((uint8_t)0);
		SpiInstance.transfer((uint8_t)((SSD1306::Height / 8) - 1));

		digitalWrite(pinDC, HIGH);
	}

protected:
	const bool Initialize(const bool backlightInternal = false)
	{
		PinReset(SSD1306::RESET_WAIT_MICROS);

		CommandStart(Settings);
		CommandEnd();
		CommandStart(Settings);
		CommandEnd();

		delayMicroseconds(SSD1306::RESET_DELAY_MICROS);
		CommandReset();

		CommandStart(Settings);
#if defined(ARDUINO_ARCH_STM32F4)
		SpiInstance.transfer((uint8_t*)SSD1306::ConfigBatch, (uint32_t)SSD1306::ConfigBatchSize);
#else
		SpiInstance.transfer((void*)SSD1306::ConfigBatch, SSD1306::ConfigBatchSize);
#endif
		CommandEnd();

		SetBacklightMode(backlightInternal);

		return true;
	}

private:
	void CommandReset()
	{
		CommandStart(Settings);
		SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::CommandStart);
		SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::Reset);
		CommandEnd();
	}

	void SetBacklightMode(const bool backlightInternal)
	{
		CommandStart(Settings);
		SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::CommandStart);
		SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::Backlight);
		if (backlightInternal)
		{
			SpiInstance.transfer((uint8_t)SSD1306::BacklightEnum::Internal);
		}
		else
		{
			SpiInstance.transfer((uint8_t)SSD1306::BacklightEnum::External);
		}
		CommandEnd();
	}
};

template<const uint8_t pinDC = UINT8_MAX,
	const uint8_t pinCS = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t pinCLK = UINT8_MAX,
	const uint8_t pinMOSI = UINT8_MAX,
	const uint8_t spiChannel = 0,
	const uint32_t spiSpeed = 4000000>
class ScreenDriverSSD1306_128x64x1_SPI : public AbstractScreenDriverSSD1306_SPI<SSD1306_128x64::Width, SSD1306_128x64::Height, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>
{
private:
	using BaseClass = AbstractScreenDriverSSD1306_SPI<SSD1306_128x64::Width, SSD1306_128x64::Height, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>;

public:
	ScreenDriverSSD1306_128x64x1_SPI() : BaseClass() {}

	virtual const bool Start()
	{
		return BaseClass::Start() && BaseClass::Initialize(false);
	}
};

template<const uint8_t pinDC = UINT8_MAX,
	const uint8_t pinCS = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t pinCLK = UINT8_MAX,
	const uint8_t pinMOSI = UINT8_MAX,
	const uint8_t spiChannel = 0,
	const uint32_t spiSpeed = 4000000,
	const uint8_t spiChunkDivisor = 2>
class ScreenDriverSSD1306_128x64x1_SPI_Async : public TemplateScreenDriverSpiAsync<ScreenDriverSSD1306_128x64x1_SPI<pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>, spiChunkDivisor>
{};

#if defined(TEMPLATE_SCREEN_DRIVER_SPI_DMA)
template<const uint8_t pinDC = UINT8_MAX,
	const uint8_t pinCS = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t pinCLK = UINT8_MAX,
	const uint8_t pinMOSI = UINT8_MAX,
	const uint8_t spiChannel = 0,
	const uint32_t spiSpeed = 4000000,
	const uint32_t pushSleepDuration = 0>
class ScreenDriverSSD1306_128x64x1_SPI_Dma : public TemplateScreenDriverSpiDma<ScreenDriverSSD1306_128x64x1_SPI<pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>, pushSleepDuration>
{};
#endif

#if defined(TEMPLATE_SCREEN_DRIVER_RTOS)
template<const uint8_t pinDC = UINT8_MAX,
	const uint8_t pinCS = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t pinCLK = UINT8_MAX,
	const uint8_t pinMOSI = UINT8_MAX,
	const uint8_t spiChannel = 0,
	const uint32_t spiSpeed = 4000000,
	const uint32_t pushSleepDuration = 0,
	uint32_t stackHeight = 1500,
	portBASE_TYPE priority = 1>
class ScreenDriverSSD1306_128x64x1_SPI_Rtos : public TemplateScreenDriverRtos<ScreenDriverSSD1306_128x64x1_SPI<pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>, pushSleepDuration, stackHeight, priority>
{};
#endif

#endif