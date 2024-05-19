// ScreenDriverSSD1331_SPI.h

#ifndef _SCREEN_DRIVER_SSD1331_SPI_h
#define _SCREEN_DRIVER_SSD1331_SPI_h

#include "AbstractScreenDriverSPI.h"
#include "TemplateScreenDriverRtos.h"
#include "SSD1331/SSD1331.h"

template<typename pixel_t,
	const uint8_t pinDC,
	const uint8_t pinCS,
	const uint8_t pinRST,
	const uint8_t pinCLK,
	const uint8_t pinMOSI,
	const uint8_t spiChannel,
	const uint32_t spiSpeed>
class AbstractScreenDriverSSD1331_SPI : public AbstractScreenDriverSPI<SSD1331::Width, SSD1331::Height, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>
{
private:
	using BaseClass = AbstractScreenDriverSPI<SSD1331::Width, SSD1331::Height, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>;

public:
	using BaseClass::ScreenWidth;
	using BaseClass::ScreenHeight;

protected:
	using BaseClass::SpiInstance;
	using BaseClass::CommandStart;
	using BaseClass::CommandEnd;
	using BaseClass::PinReset;

public:
	static constexpr size_t BufferSize = GraphicsBuffer::GetBufferSize<pixel_t>(SSD1331::Width, SSD1331::Height);

private:
	SPISettings Settings;

public:
	AbstractScreenDriverSSD1331_SPI() : IScreenDriver()
		, Settings(spiSpeed, MSBFIRST, SPI_MODE0)
	{}

public:
	virtual const bool CanPushBuffer()
	{
		return true;
	}

	virtual void StartBuffer()
	{
		CommandStart(Settings);
		SpiInstance.transfer((void*)SSD1331::PointConfig, sizeof(SSD1331::PointConfig));

		digitalWrite(pinDC, HIGH);
	}

	virtual const uint32_t PushBuffer(const uint8_t* frameBuffer)
	{
		SpiInstance.transfer((void*)frameBuffer, BufferSize);

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

protected:
	const bool Initialize(const uint8_t* configBatch, const size_t configBatchSize)
	{
		PinReset(SSD1331::RESET_WAIT_MICROS);
		delayMicroseconds(SSD1331::RESET_DELAY_MICROS);

		CommandStart(Settings);
		SpiInstance.transfer((void*)configBatch, configBatchSize);
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
			&& BaseClass::Initialize(SSD1331::Rgb332::ConfigBatch, sizeof(SSD1331::Rgb332::ConfigBatch));
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
			&& BaseClass::Initialize(SSD1331::Rgb565::ConfigBatch, sizeof(SSD1331::Rgb565::ConfigBatch));
	}
};

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
class ScreenDriverSSD1331_96x64x8_SPI_Rtos : public TemplateScreenDriverRtos<ScreenDriverSSD1331_96x64x16_SPI<pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>, stackHeight, priority>
{};
#endif

#endif