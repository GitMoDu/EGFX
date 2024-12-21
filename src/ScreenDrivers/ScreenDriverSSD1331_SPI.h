// ScreenDriverSSD1331_SPI.h

#ifndef _SCREEN_DRIVER_SSD1331_SPI_h
#define _SCREEN_DRIVER_SSD1331_SPI_h

#include "AbstractScreenDriverSPI.h"
#include "TemplateScreenDriverRtos.h"
#include "TemplateScreenDriverSpiAsync.h"
#include "TemplateScreenDriverSpiDma.h"
#include "SSD1331/SSD1331.h"

template<typename pixel_t,
	const uint8_t pinCS,
	const uint8_t pinDC,
	const uint8_t pinRST,
	const uint32_t spiSpeed>
class AbstractScreenDriverSSD1331_SPI : public AbstractScreenDriverSPI<GraphicsBuffer::GetBufferSize<pixel_t>(SSD1331::Width, SSD1331::Height), SSD1331::Width, SSD1331::Height, pinCS, pinDC, pinRST>
{
private:
	using BaseClass = AbstractScreenDriverSPI<GraphicsBuffer::GetBufferSize<pixel_t>(SSD1331::Width, SSD1331::Height), SSD1331::Width, SSD1331::Height, pinCS, pinDC, pinRST>;

public:
	using BaseClass::ScreenWidth;
	using BaseClass::ScreenHeight;
	using BaseClass::BufferSize;

protected:
	using BaseClass::SpiInstance;
	using BaseClass::SpiTransfer;
	using BaseClass::CommandStart;
	using BaseClass::CommandEnd;
	using BaseClass::PinReset;

private:
	SPISettings Settings;

public:
	AbstractScreenDriverSSD1331_SPI(Egfx::SpiType& spi)
		: BaseClass(spi)
		, Settings(spiSpeed, MSBFIRST, SPI_MODE0)
	{
	}

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

		SpiTransfer(SSD1331::CommonConfigBatch, sizeof(SSD1331::CommonConfigBatch));
		CommandEnd();

		return true;
	}
};

template<const uint8_t pinCS = UINT8_MAX,
	const uint8_t pinDC = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint32_t spiSpeed = 4000000>
class ScreenDriverSSD1331_96x64x8_SPI : public AbstractScreenDriverSSD1331_SPI<uint8_t, pinCS, pinDC, pinRST, spiSpeed>
{
private:
	using BaseClass = AbstractScreenDriverSSD1331_SPI<uint8_t, pinCS, pinDC, pinRST, spiSpeed>;

public:
	ScreenDriverSSD1331_96x64x8_SPI(Egfx::SpiType& spi) : BaseClass(spi) {}

	virtual const bool Start()
	{
		return BaseClass::Start() && BaseClass::Initialize(SSD1331::MapRgb332);
	}
};

template<const uint8_t pinCS = UINT8_MAX,
	const uint8_t pinDC = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint32_t spiSpeed = 4000000>
class ScreenDriverSSD1331_96x64x16_SPI : public AbstractScreenDriverSSD1331_SPI<uint16_t, pinCS, pinDC, pinRST, spiSpeed>
{
private:
	using BaseClass = AbstractScreenDriverSSD1331_SPI<uint16_t, pinCS, pinDC, pinRST, spiSpeed>;

public:
	ScreenDriverSSD1331_96x64x16_SPI(Egfx::SpiType& spi) : BaseClass(spi) {}

	virtual const bool Start()
	{
		return BaseClass::Start() && BaseClass::Initialize(SSD1331::MapRgb565);
	}
};

template<const uint8_t pinCS = UINT8_MAX,
	const uint8_t pinDC = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint32_t spiSpeed = 4000000,
	const uint8_t spiChunkDivisor = 2>
using ScreenDriverSSD1331_96x64x8_SPI_Async = TemplateScreenDriverSpiAsync<ScreenDriverSSD1331_96x64x8_SPI<pinCS, pinDC, pinRST, spiSpeed>, spiChunkDivisor>;

template<const uint8_t pinCS = UINT8_MAX,
	const uint8_t pinDC = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint32_t spiSpeed = 4000000,
	const uint8_t spiChunkDivisor = 2>
using ScreenDriverSSD1331_96x64x16_SPI_Async = TemplateScreenDriverSpiAsync<ScreenDriverSSD1331_96x64x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, spiChunkDivisor>;

#if defined(TEMPLATE_SCREEN_DRIVER_SPI_DMA)
template<const uint8_t pinCS = UINT8_MAX,
	const uint8_t pinDC = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint32_t spiSpeed = 4000000,
	const uint32_t pushSleepDuration = 0>
using ScreenDriverSSD1331_96x64x8_SPI_Dma = TemplateScreenDriverSpiDma<ScreenDriverSSD1331_96x64x8_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration>;

template<const uint8_t pinCS = UINT8_MAX,
	const uint8_t pinDC = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint32_t spiSpeed = 4000000,
	const uint32_t pushSleepDuration = 0>
using ScreenDriverSSD1331_96x64x16_SPI_Dma = TemplateScreenDriverSpiDma<ScreenDriverSSD1331_96x64x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration>;
#endif

#if defined(TEMPLATE_SCREEN_DRIVER_RTOS)
template<const uint8_t pinCS,
	const uint8_t pinDC,
	const uint8_t pinRST,
	const uint32_t spiSpeed = 4000000,
	const uint32_t pushSleepDuration = 0,
	uint32_t stackHeight = 1500,
	portBASE_TYPE priority = 1
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
	, const uint32_t coreAffinity = tskNO_AFFINITY
#endif
>
using ScreenDriverSSD1331_96x64x8_SPI_Rtos = TemplateScreenDriverRtos<Egfx::SpiType, ScreenDriverSSD1331_96x64x8_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration, stackHeight, priority>;

template<const uint8_t pinCS,
	const uint8_t pinDC,
	const uint8_t pinRST,
	const uint32_t spiSpeed = 4000000,
	const uint32_t pushSleepDuration = 0,
	uint32_t stackHeight = 1500,
	portBASE_TYPE priority = 1
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
	, const uint32_t coreAffinity = tskNO_AFFINITY
#endif
>
using ScreenDriverSSD1331_96x64x16_SPI_Rtos = TemplateScreenDriverRtos<Egfx::SpiType, ScreenDriverSSD1331_96x64x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration, stackHeight, priority
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
	, coreAffinity
#endif
>;
#endif
#endif