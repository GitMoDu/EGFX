// ScreenDriverSSD1306_SPI.h

#ifndef _SCREEN_DRIVER_SSD1306_SPI_h
#define _SCREEN_DRIVER_SSD1306_SPI_h

#include "AbstractScreenDriverSPI.h"
#include "SSD1306\SSD1306.h"
#include "TemplateScreenDriverRtos.h"

template<const uint8_t width,
	const uint8_t height,
	const uint8_t pinDC,
	const uint8_t pinCS,
	const uint8_t pinRST,
	const uint8_t pinCLK,
	const uint8_t pinMOSI,
	const uint8_t spiChannel,
	const uint32_t spiSpeed>
class AbstractScreenDriverSSD1306_SPI : public AbstractScreenDriverSPI<width, height, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>
{
public:
	static constexpr size_t BufferSize = GraphicsBuffer::GetMonochromeBufferSize(width, height);

private:
	using BaseClass = AbstractScreenDriverSPI<width, height, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>;

public:
	using BaseClass::ScreenWidth;
	using BaseClass::ScreenHeight;

protected:
	using BaseClass::SpiInstance;
	using BaseClass::CommandStart;
	using BaseClass::CommandEnd;

private:
	SPISettings Settings;

public:
	AbstractScreenDriverSSD1306_SPI()
		: BaseClass()
		, Settings(spiSpeed, MSBFIRST, SPI_MODE1)
	{}

	virtual const bool Start()
	{
		return BaseClass::Start();
	}

	virtual void Stop()
	{
		CommandReset();
		SpiInstance.end();
	}

	virtual const bool CanPushBuffer()
	{
		return true;
	}

	virtual void StartBuffer()
	{
		CommandStartBuffer();
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
	const bool Initialize(const bool backlightInternal = false)
	{
		CommandReset();

		delayMicroseconds(SSD1306::RESET_DELAY_MICROS);

		CommandStart(Settings);
		SpiInstance.transfer((void*)SSD1306::ConfigBatch, SSD1306::ConfigBatchSize);
		CommandEnd();

		SetBacklightMode(backlightInternal);

		return true;
	}

private:
	void CommandReset()
	{
		CommandStart(Settings);
		CommandEnd();
		CommandStart(Settings);
		CommandEnd();

		CommandStart(Settings);
		SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::CommandStart);
		SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::Reset);
		CommandEnd();
	}

	void CommandStartBuffer()
	{
		CommandStart(Settings);
		SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::CommandStart);

		SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::Column);
		SpiInstance.transfer((uint8_t)0);
		SpiInstance.transfer((uint8_t)(ScreenWidth - 1));

		SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::Page);
		SpiInstance.transfer((uint8_t)0);
		SpiInstance.transfer((uint8_t)(SSD1306::Height / 8) - 1);

		digitalWrite(pinDC, HIGH);
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

template<const uint8_t width,
	const uint8_t height,
	const uint8_t pinDC,
	const uint8_t pinCS,
	const uint8_t pinRST,
	const uint8_t pinCLK,
	const uint8_t pinMOSI,
	const uint8_t spiChannel,
	const uint32_t spiSpeed,
	const uint8_t spiChunkDivisor = 2>
class AbstractScreenDriverSSD1306_SPI_Async : public AbstractScreenDriverSSD1306_SPI<width, height, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>
{
private:
	using BaseClass = AbstractScreenDriverSSD1306_SPI<width, height, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>;

public:
	using BaseClass::BufferSize;

protected:
	using BaseClass::SpiInstance;

private:
	static constexpr size_t CHUNK_SIZE = BufferSize / spiChunkDivisor;
	static constexpr size_t WHOLE_SIZE = (BufferSize / CHUNK_SIZE) * CHUNK_SIZE;
	static constexpr size_t REMAINDER_SIZE = BufferSize - WHOLE_SIZE;
	static constexpr size_t REMAINDER_START = WHOLE_SIZE;

private:
	size_t PushIndex = 0;

public:
	AbstractScreenDriverSSD1306_SPI_Async() : BaseClass() {}

	virtual const uint32_t PushBuffer(const uint8_t* frameBuffer) final
	{
		PushIndex = 0;

		if (WHOLE_SIZE > 0)
		{
			SpiInstance.transfer((void*)frameBuffer, CHUNK_SIZE);
			PushIndex += CHUNK_SIZE;
		}

		return 0;
	}

	virtual const bool PushingBuffer(const uint8_t* frameBuffer) final
	{
		if (PushIndex < WHOLE_SIZE)
		{
			SpiInstance.transfer((void*)&frameBuffer[PushIndex], CHUNK_SIZE);
			PushIndex += CHUNK_SIZE;

			return true;
		}
		else
		{
			if (REMAINDER_SIZE > 0)
			{
				SpiInstance.transfer((void*)&frameBuffer[REMAINDER_START], REMAINDER_SIZE);
			}

			return false;
		}
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
		return BaseClass::Start() && BaseClass::Initialize();
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
class ScreenDriverSSD1306_128x64x1_SPI_Async : public AbstractScreenDriverSSD1306_SPI_Async<SSD1306_128x64::Width, SSD1306_128x64::Height, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed, spiChunkDivisor>
{
private:
	using BaseClass = AbstractScreenDriverSSD1306_SPI_Async<SSD1306_128x64::Width, SSD1306_128x64::Height, pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed, spiChunkDivisor>;

public:
	ScreenDriverSSD1306_128x64x1_SPI_Async() : BaseClass() {}

	virtual const bool Start() final
	{
		return BaseClass::Start() && BaseClass::Initialize(false);
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
	const uint32_t pushSleepDuration = 0,
	uint32_t stackHeight = 1500,
	portBASE_TYPE priority = 1>
class ScreenDriverSSD1306_128x64x1_SPI_Rtos : public TemplateScreenDriverRtos<ScreenDriverSSD1306_128x64x1_SPI<pinDC, pinCS, pinRST, pinCLK, pinMOSI, spiChannel, spiSpeed>, pushSleepDuration, stackHeight, priority>
{};
#endif

#endif