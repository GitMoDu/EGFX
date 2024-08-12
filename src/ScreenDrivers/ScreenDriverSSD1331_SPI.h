// ScreenDriverSSD1331_SPI.h

#ifndef _SCREEN_DRIVER_SSD1331_SPI_h
#define _SCREEN_DRIVER_SSD1331_SPI_h

#include "AbstractScreenDriverSPI.h"
#include "TemplateScreenDriverRtos.h"
#include "TemplateScreenDriverSpiAsync.h"
#include "TemplateScreenDriverSpiDma.h"
#include "SSD1331/SSD1331.h"

template<typename pixel_t,
	const uint8_t pinDC,
	const uint8_t pinCS,
	const uint8_t pinRST,
	const uint8_t pinCLK,
	const uint8_t pinMOSI,
	const uint8_t spiChannel,
	const uint32_t spiSpeed>
class AbstractScreenDriverSSD1331_SPI : public AbstractScreenDriverSPI<GraphicsBuffer::GetBufferSize<pixel_t>(SSD1331::Width, SSD1331::Height), SSD1331::Width, SSD1331::Height, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>
{
private:
	using BaseClass = AbstractScreenDriverSPI<GraphicsBuffer::GetBufferSize<pixel_t>(SSD1331::Width, SSD1331::Height), SSD1331::Width, SSD1331::Height, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>;

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
	AbstractScreenDriverSSD1331_SPI() : BaseClass()
		, Settings(spiSpeed, MSBFIRST, SPI_MODE0)
	{}

public:
	virtual void StartBuffer()
	{
		CommandStart(Settings);
		digitalWrite(pinDC, HIGH);
	}

protected:
	const bool Initialize(const uint8_t rgbMode)
	{
		PinReset(SSD1331::RESET_WAIT_MICROS);
		delayMicroseconds(SSD1331::RESET_DELAY_MICROS);

		CommandStart(Settings);
		SpiInstance.transfer((uint8_t)SSD1331::CommandEnum::DisplayOff);
		SpiInstance.transfer((uint8_t)SSD1331::CommandEnum::RemapAndColor);
		SpiInstance.transfer(rgbMode);

#if defined(ARDUINO_ARCH_STM32F4)
		SpiInstance.transfer((uint8_t*)SSD1331::CommonConfigBatch, sizeof(SSD1331::CommonConfigBatch));
#elif defined(ARDUINO_ARCH_STM32)
		SpiInstance.transfer((void*)SSD1331::CommonConfigBatch, sizeof(SSD1331::CommonConfigBatch), true);
#else
		SpiInstance.transfer((void*)SSD1331::CommonConfigBatch, sizeof(SSD1331::CommonConfigBatch));
#endif
		CommandEnd();

		return true;
	}
};

template<const uint8_t pinDC = UINT8_MAX,
	const uint8_t pinCS = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t pinCLK = UINT8_MAX,
	const uint8_t pinMOSI = UINT8_MAX,
	const uint8_t spiChannel = 0,
	const uint32_t spiSpeed = 4000000>
class ScreenDriverSSD1331_96x64x8_SPI : public AbstractScreenDriverSSD1331_SPI<uint8_t, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>
{
private:
	using BaseClass = AbstractScreenDriverSSD1331_SPI<uint8_t, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>;

public:
	ScreenDriverSSD1331_96x64x8_SPI() : BaseClass() {}

	virtual const bool Start()
	{
		return BaseClass::Start()
			&& BaseClass::Initialize(SSD1331::MapRgb332);
	}
};

template<const uint8_t pinDC = UINT8_MAX,
	const uint8_t pinCS = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t pinCLK = UINT8_MAX,
	const uint8_t pinMOSI = UINT8_MAX,
	const uint8_t spiChannel = 0,
	const uint32_t spiSpeed = 4000000>
class ScreenDriverSSD1331_96x64x16_SPI : public AbstractScreenDriverSSD1331_SPI<uint16_t, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>
{
private:
	using BaseClass = AbstractScreenDriverSSD1331_SPI<uint16_t, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>;

public:
	ScreenDriverSSD1331_96x64x16_SPI() : BaseClass() {}

	virtual const bool Start()
	{
		return BaseClass::Start()
			&& BaseClass::Initialize(SSD1331::MapRgb565);
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
using ScreenDriverSSD1331_96x64x8_SPI_Async = TemplateScreenDriverSpiAsync<ScreenDriverSSD1331_96x64x8_SPI<pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>, spiChunkDivisor>;

template<const uint8_t pinDC = UINT8_MAX,
	const uint8_t pinCS = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t pinCLK = UINT8_MAX,
	const uint8_t pinMOSI = UINT8_MAX,
	const uint8_t spiChannel = 0,
	const uint32_t spiSpeed = 4000000,
	const uint8_t spiChunkDivisor = 2>
using ScreenDriverSSD1331_96x64x16_SPI_Async = TemplateScreenDriverSpiAsync<ScreenDriverSSD1331_96x64x16_SPI<pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>, spiChunkDivisor>;

#if defined(TEMPLATE_SCREEN_DRIVER_SPI_DMA)
template<const uint8_t pinDC = UINT8_MAX,
	const uint8_t pinCS = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t pinCLK = UINT8_MAX,
	const uint8_t pinMOSI = UINT8_MAX,
	const uint8_t spiChannel = 0,
	const uint32_t spiSpeed = 4000000,
	const uint32_t pushSleepDuration = 0>
using ScreenDriverSSD1331_96x64x8_SPI_Dma = TemplateScreenDriverSpiDma<ScreenDriverSSD1331_96x64x8_SPI<pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>, pushSleepDuration>;

template<const uint8_t pinDC = UINT8_MAX,
	const uint8_t pinCS = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t pinCLK = UINT8_MAX,
	const uint8_t pinMOSI = UINT8_MAX,
	const uint8_t spiChannel = 0,
	const uint32_t spiSpeed = 4000000,
	const uint32_t pushSleepDuration = 0>
using ScreenDriverSSD1331_96x64x16_SPI_Dma = TemplateScreenDriverSpiDma<ScreenDriverSSD1331_96x64x16_SPI<pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>, pushSleepDuration>;
#endif

#if defined(TEMPLATE_SCREEN_DRIVER_RTOS)
template<const uint8_t pinDC = UINT8_MAX,
	const uint8_t pinCS = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t pinCLK = UINT8_MAX,
	const uint8_t pinMOSI = UINT8_MAX,
	const uint8_t spiChannel = 0,
	const uint32_t spiSpeed = 4000000,
	uint32_t stackHeight = 1500,
	portBASE_TYPE priority = 1>
using ScreenDriverSSD1331_96x64x8_SPI_Rtos = TemplateScreenDriverRtos<ScreenDriverSSD1331_96x64x8_SPI<pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>, stackHeight, priority>;

template<const uint8_t pinDC = UINT8_MAX,
	const uint8_t pinCS = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t pinCLK = UINT8_MAX,
	const uint8_t pinMOSI = UINT8_MAX,
	const uint8_t spiChannel = 0,
	const uint32_t spiSpeed = 4000000,
	uint32_t stackHeight = 1500,
	portBASE_TYPE priority = 1>
using ScreenDriverSSD1331_96x64x16_SPI_Rtos = TemplateScreenDriverRtos<ScreenDriverSSD1331_96x64x16_SPI<pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>, stackHeight, priority>;
#endif
#endif