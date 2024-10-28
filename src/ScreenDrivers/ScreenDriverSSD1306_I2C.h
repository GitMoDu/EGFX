// ScreenDriverSSD1306.h

#ifndef _SCREEN_DRIVER_SSD1306_I2C_h
#define _SCREEN_DRIVER_SSD1306_I2C_h

#include "AbstractScreenDriverI2C.h"
#include "TemplateScreenDriverRtos.h"
#include "TemplateScreenDriverI2CDma.h"
#include "TemplateScreenDriverI2CAsync.h"
#include "SSD1306\SSD1306.h"

template<const uint8_t width,
	const uint8_t height,
	const int8_t verticalOffset,
	const uint8_t pinSCL,
	const uint8_t pinSDA,
	const uint8_t pinRST,
	const uint8_t i2cAddress,
	const uint8_t i2cChannel,
	const uint32_t i2cSpeed>
class AbstractScreenDriverSSD1306_I2C : public AbstractScreenDriverI2C<width, height, pinSCL, pinSDA, pinRST, i2cAddress, i2cChannel, i2cSpeed>
{
public:
	static constexpr size_t BufferSize = GraphicsBuffer::GetMonochromeBufferSize(width, height);

private:
	using BaseClass = AbstractScreenDriverI2C<width, height, pinSCL, pinSDA, pinRST, i2cAddress, i2cChannel, i2cSpeed>;

public:
	using BaseClass::ScreenWidth;
	using BaseClass::ScreenHeight;

protected:
	using BaseClass::WireInstance;
	using BaseClass::I2CAddress;
	using BaseClass::I2CChannel;
	using BaseClass::I2CSpeed;

private:
	static constexpr uint8_t OffsetX = ((SSD1306::Width - ScreenWidth) / 2);
	static constexpr uint8_t OffsetY = ((SSD1306::Height / 8) - (ScreenHeight / 8)) + verticalOffset;

protected:
	static constexpr uint8_t I2C_BUFFER_SIZE = 31;

	static constexpr uint8_t BUFFER_WHOLE = BufferSize / I2C_BUFFER_SIZE;
	static constexpr uint8_t BUFFER_REMAINDER = BufferSize % I2C_BUFFER_SIZE;
	static constexpr size_t REMAINDER_START = (size_t)BUFFER_WHOLE * I2C_BUFFER_SIZE;

#if defined(ARDUINO_ARCH_RP2040)
private:
	uint8_t DmaBuffer[I2C_BUFFER_SIZE + 1]{};
#endif

public:
	AbstractScreenDriverSSD1306_I2C() : BaseClass()
	{}

	virtual const bool Start()
	{
		return BaseClass::Start();
	}

	virtual void Stop()
	{
		CommandReset();
		WireInstance.end();
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
		for (uint8_t i = 0; i < BUFFER_WHOLE; i++)
		{
			PushChunk(&frameBuffer[(size_t)i * I2C_BUFFER_SIZE], I2C_BUFFER_SIZE);
		}

		if (BUFFER_REMAINDER > 0)
		{
			PushChunk(&frameBuffer[REMAINDER_START], BUFFER_REMAINDER);
		}

		return 0;
	}

	virtual const bool PushingBuffer(const uint8_t* frameBuffer)
	{
		return false;
	}

	virtual void EndBuffer()
	{}

protected:
	const bool Initialize(const bool backlightInternal = false)
	{
		CommandReset();

		delayMicroseconds(SSD1306::RESET_DELAY_MICROS);

		CommandStart();
		WireInstance.write(SSD1306::ConfigBatch, SSD1306::ConfigBatchSize);
		CommandEnd();

		SetBacklightMode(backlightInternal);

		return true;
	}

	void CommandStart()
	{
		WireInstance.beginTransmission(I2CAddress);
	}

	void CommandEnd()
	{
		WireInstance.endTransmission();
	}

	void PushChunk(const uint8_t* chunk, const size_t chunkSize)
	{
		CommandStart();
		WireInstance.write((uint8_t)SSD1306::CommandEnum::BufferStart);
		WireInstance.write(chunk, chunkSize);
		CommandEnd();
	}

#if defined(TEMPLATE_SCREEN_DRIVER_I2C_DMA)
	void PushChunkDma(const uint8_t* chunk, const size_t chunkSize)
	{
		DmaBuffer[0] = (uint8_t)SSD1306::CommandEnum::BufferStart;
		memcpy(&DmaBuffer[1], chunk, chunkSize);
		WireInstance.writeAsync(I2CAddress, (const void*)DmaBuffer, chunkSize + 1, true);
	}
#endif

private:
	void CommandReset()
	{
		CommandStart();
		CommandEnd();
		CommandStart();
		CommandEnd();

		CommandStart();
		WireInstance.write((uint8_t)SSD1306::CommandEnum::CommandStart);
		WireInstance.write((uint8_t)SSD1306::CommandEnum::Reset);
		CommandEnd();
	}

	void CommandStartBuffer()
	{
		CommandStart();
		WireInstance.write((uint8_t)SSD1306::CommandEnum::CommandStart);

		WireInstance.write((uint8_t)SSD1306::CommandEnum::Column);
		WireInstance.write(OffsetX);
		WireInstance.write(OffsetX + ScreenWidth - 1);

		WireInstance.write((uint8_t)SSD1306::CommandEnum::Page);
		WireInstance.write(OffsetY);
		WireInstance.write((SSD1306::Height / 8) - 1);
		CommandEnd();
	}

	void SetBacklightMode(const bool backlightInternal)
	{
		CommandStart();
		WireInstance.write((uint8_t)SSD1306::CommandEnum::CommandStart);
		WireInstance.write((uint8_t)SSD1306::CommandEnum::Backlight);
		if (backlightInternal)
		{
			WireInstance.write((uint8_t)SSD1306::BacklightEnum::Internal);
		}
		else
		{
			WireInstance.write((uint8_t)SSD1306::BacklightEnum::External);
		}
		CommandEnd();
	}
};

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000>
class ScreenDriverSSD1306_64x32x1_I2C : public AbstractScreenDriverSSD1306_I2C<SSD1306_64x32::Width, SSD1306_64x32::Height, 0, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed>
{
private:
	using BaseClass = AbstractScreenDriverSSD1306_I2C<SSD1306_64x32::Width, SSD1306_64x32::Height, 0, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed>;

public:
	ScreenDriverSSD1306_64x32x1_I2C() : BaseClass() {}

	virtual const bool Start()
	{
		return BaseClass::Start() && BaseClass::Initialize(false);
	}
};

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000>
class ScreenDriverSSD1306_64x48x1_I2C : public AbstractScreenDriverSSD1306_I2C<SSD1306_64x48::Width, SSD1306_64x48::Height, SSD1306_64x48::VerticalOffset, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed>
{
private:
	using BaseClass = AbstractScreenDriverSSD1306_I2C<SSD1306_64x48::Width, SSD1306_64x48::Height, SSD1306_64x48::VerticalOffset, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed>;

public:
	ScreenDriverSSD1306_64x48x1_I2C() : BaseClass() {}

	virtual const bool Start()
	{
		return BaseClass::Start() && BaseClass::Initialize(false);
	}
};

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000>
class ScreenDriverSSD1306_72x40x1_I2C : public AbstractScreenDriverSSD1306_I2C<SSD1306_72x40::Width, SSD1306_72x40::Height, 0, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed>
{
private:
	using BaseClass = AbstractScreenDriverSSD1306_I2C<SSD1306_72x40::Width, SSD1306_72x40::Height, 0, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed>;

public:
	ScreenDriverSSD1306_72x40x1_I2C() : BaseClass() {}

	virtual const bool Start()
	{
		return BaseClass::Start() && BaseClass::Initialize(true);
	}
};

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000>
class ScreenDriverSSD1306_128x32x1_I2C : public AbstractScreenDriverSSD1306_I2C<SSD1306_128x32::Width, SSD1306_128x32::Height, SSD1306_128x32::VerticalOffset, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed>
{
private:
	using BaseClass = AbstractScreenDriverSSD1306_I2C<SSD1306_128x32::Width, SSD1306_128x32::Height, SSD1306_128x32::VerticalOffset, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed>;

public:
	ScreenDriverSSD1306_128x32x1_I2C() : BaseClass() {}

	virtual const bool Start()
	{
		return BaseClass::Start() && BaseClass::Initialize(false);
	}
};

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000>
class ScreenDriverSSD1306_128x64x1_I2C : public AbstractScreenDriverSSD1306_I2C<SSD1306_128x64::Width, SSD1306_128x64::Height, 0, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed>
{
private:
	using BaseClass = AbstractScreenDriverSSD1306_I2C<SSD1306_128x64::Width, SSD1306_128x64::Height, 0, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed>;

public:
	ScreenDriverSSD1306_128x64x1_I2C() : BaseClass() {}

	virtual const bool Start()
	{
		return BaseClass::Start() && BaseClass::Initialize(false);
	}
};

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000,
	const uint8_t i2cChunkSize = 8>
using  ScreenDriverSSD1306_64x32x1_I2C_Async = TemplateScreenDriverI2CAsync<ScreenDriverSSD1306_64x32x1_I2C<pinSCL, pinSDA, pinRST, i2cChannel, i2cSpeed>, i2cChunkSize>;

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000,
	const uint8_t i2cChunkSize = 8>
using ScreenDriverSSD1306_64x48x1_I2C_Async = TemplateScreenDriverI2CAsync<ScreenDriverSSD1306_64x48x1_I2C<pinSCL, pinSDA, pinRST, i2cChannel, i2cSpeed>, i2cChunkSize>;

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000,
	const uint8_t i2cChunkSize = 8>
using ScreenDriverSSD1306_72x40x1_I2C_Async = TemplateScreenDriverI2CAsync<ScreenDriverSSD1306_72x40x1_I2C<pinSCL, pinSDA, pinRST, i2cChannel, i2cSpeed>, i2cChunkSize>;

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000,
	const uint8_t i2cChunkSize = 8>
using ScreenDriverSSD1306_128x32x1_I2C_Async = TemplateScreenDriverI2CAsync<ScreenDriverSSD1306_128x32x1_I2C<pinSCL, pinSDA, pinRST, i2cChannel, i2cSpeed>, i2cChunkSize>;

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000,
	const uint8_t i2cChunkSize = 8>
using ScreenDriverSSD1306_128x64x1_I2C_Async = TemplateScreenDriverI2CAsync<ScreenDriverSSD1306_128x64x1_I2C<pinSCL, pinSDA, pinRST, i2cChannel, i2cSpeed>, i2cChunkSize>;

#if defined(TEMPLATE_SCREEN_DRIVER_RTOS)
template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000,
	const uint32_t pushSleepDuration = 0,
	uint32_t stackHeight = 1500,
	portBASE_TYPE priority = 1>
using ScreenDriverSSD1306_64x32x1_I2C_Rtos = TemplateScreenDriverRtos<ScreenDriverSSD1306_64x32x1_I2C<pinSCL, pinSDA, pinRST, i2cChannel, i2cSpeed>, pushSleepDuration, stackHeight, priority>;

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000,
	const uint32_t pushSleepDuration = 0,
	uint32_t stackHeight = 1500,
	portBASE_TYPE priority = 1>
using ScreenDriverSSD1306_64x48x1_I2C_Rtos = TemplateScreenDriverRtos<ScreenDriverSSD1306_64x48x1_I2C<pinSCL, pinSDA, pinRST, i2cChannel, i2cSpeed>, pushSleepDuration, stackHeight, priority>;

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000,
	const uint32_t pushSleepDuration = 0,
	uint32_t stackHeight = 1500,
	portBASE_TYPE priority = 1>
using ScreenDriverSSD1306_72x40x1_I2C_Rtos = TemplateScreenDriverRtos<ScreenDriverSSD1306_72x40x1_I2C<pinSCL, pinSDA, pinRST, i2cChannel, i2cSpeed>, pushSleepDuration, stackHeight, priority>;

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000,
	const uint32_t pushSleepDuration = 0,
	uint32_t stackHeight = 1500,
	portBASE_TYPE priority = 1>
using ScreenDriverSSD1306_128x32x1_I2C_Rtos = TemplateScreenDriverRtos<ScreenDriverSSD1306_128x32x1_I2C<pinSCL, pinSDA, pinRST, i2cChannel, i2cSpeed>, pushSleepDuration, stackHeight, priority>;

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000,
	const uint32_t pushSleepDuration = 0,
	uint32_t stackHeight = 1500,
	portBASE_TYPE priority = 1>
using ScreenDriverSSD1306_128x64x1_I2C_Rtos = TemplateScreenDriverRtos<ScreenDriverSSD1306_128x64x1_I2C<pinSCL, pinSDA, pinRST, i2cChannel, i2cSpeed>, pushSleepDuration, stackHeight, priority>;
#endif

#if defined(TEMPLATE_SCREEN_DRIVER_I2C_DMA)
template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000>
using ScreenDriverSSD1306_64x32x1_I2C_Dma = TemplateScreenDriverI2CDma<ScreenDriverSSD1306_64x32x1_I2C<pinSCL, pinSDA, pinRST, i2cChannel, i2cSpeed>>;

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000>
using ScreenDriverSSD1306_64x48x1_I2C_Dma = TemplateScreenDriverI2CDma<ScreenDriverSSD1306_64x48x1_I2C<pinSCL, pinSDA, pinRST, i2cChannel, i2cSpeed>>;

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000>
using ScreenDriverSSD1306_72x40x1_I2C_Dma = TemplateScreenDriverI2CDma<ScreenDriverSSD1306_72x40x1_I2C<pinSCL, pinSDA, pinRST, i2cChannel, i2cSpeed>>;

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000>
using ScreenDriverSSD1306_128x32x1_I2C_Dma = TemplateScreenDriverI2CDma<ScreenDriverSSD1306_128x32x1_I2C<pinSCL, pinSDA, pinRST, i2cChannel, i2cSpeed>>;

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000>
using ScreenDriverSSD1306_128x64x1_I2C_Dma = TemplateScreenDriverI2CDma<ScreenDriverSSD1306_128x64x1_I2C<pinSCL, pinSDA, pinRST, i2cChannel, i2cSpeed>>;
#endif
#endif