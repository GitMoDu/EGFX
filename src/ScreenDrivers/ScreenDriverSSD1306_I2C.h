// ScreenDriverSSD1306.h

#ifndef _SCREEN_DRIVER_SSD1306_I2C_h
#define _SCREEN_DRIVER_SSD1306_I2C_h

#include "AbstractScreenDriverI2C.h"

#include "TemplateScreenDriverRtos.h"
#include "SSD1306\SSD1306.h"

template<const uint8_t width,
	const uint8_t height,
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
	static constexpr uint8_t OffsetY = ((SSD1306::Height / 8) - (ScreenHeight / 8));

protected:
	static constexpr uint8_t I2C_BUFFER_SIZE = 31;

private:
	static constexpr uint8_t BUFFER_WHOLE = BufferSize / I2C_BUFFER_SIZE;
	static constexpr uint8_t BUFFER_REMAINDER = BufferSize % I2C_BUFFER_SIZE;
	static constexpr size_t REMAINDER_START = (size_t)BUFFER_WHOLE * I2C_BUFFER_SIZE;

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
			CommandStart();
			WireInstance.write((uint8_t)SSD1306::CommandEnum::BufferStart);
			WireInstance.write(&frameBuffer[(size_t)i * I2C_BUFFER_SIZE], I2C_BUFFER_SIZE);
			CommandEnd();
		}

		return 0;
	}

	virtual const bool PushingBuffer(const uint8_t* frameBuffer)
	{
		if (BUFFER_REMAINDER > 0)
		{
			CommandStart();
			WireInstance.write((uint8_t)SSD1306::CommandEnum::BufferStart);
			WireInstance.write(&frameBuffer[REMAINDER_START], BUFFER_REMAINDER);
			CommandEnd();
		}

		return false;
	}

	virtual void EndBuffer()
	{}

protected:
	const bool Initialize(const uint8_t* configBatch, const size_t configBatchSize)
	{
		CommandReset();

		delayMicroseconds(SSD1306::RESET_DELAY_MICROS);

		CommandStart();
		WireInstance.write(configBatch, configBatchSize);
		CommandEnd();

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
};


template<const uint8_t width,
	const uint8_t height,
	const uint8_t pinSCL,
	const uint8_t pinSDA,
	const uint8_t pinRST,
	const uint8_t i2cAddress,
	const uint8_t i2cChannel,
	const uint32_t i2cSpeed,
	const uint8_t i2cChunkSize = 8>
class AbstractScreenDriverSSD1306_I2C_Async : public AbstractScreenDriverSSD1306_I2C<width, height, pinSCL, pinSDA, pinRST, i2cAddress, i2cChannel, i2cSpeed>
{
private:
	using BaseClass = AbstractScreenDriverSSD1306_I2C<width, height, pinSCL, pinSDA, pinRST, i2cAddress, i2cChannel, i2cSpeed>;

public:
	using BaseClass::BufferSize;

protected:
	using BaseClass::I2C_BUFFER_SIZE;
	using BaseClass::WireInstance;
	using BaseClass::CommandStart;
	using BaseClass::CommandEnd;

private:
	static constexpr uint8_t CHUNK_SIZE = i2cChunkSize;
	static constexpr size_t WHOLE_SIZE = (BufferSize / CHUNK_SIZE) * CHUNK_SIZE;
	static constexpr size_t REMAINDER_SIZE = BufferSize - WHOLE_SIZE;
	static constexpr size_t REMAINDER_START = WHOLE_SIZE;

private:
	size_t PushIndex = 0;

public:
	AbstractScreenDriverSSD1306_I2C_Async() : BaseClass() {}

	const bool Start()
	{
		return CHUNK_SIZE <= BufferSize
			&& CHUNK_SIZE <= I2C_BUFFER_SIZE
			&& BaseClass::Start();
	}

	virtual const uint32_t PushBuffer(const uint8_t* frameBuffer) final
	{
		PushIndex = 0;

		if (WHOLE_SIZE > 0)
		{
			CommandStart();
			WireInstance.write((uint8_t)SSD1306::CommandEnum::BufferStart);
			WireInstance.write(frameBuffer, (size_t)CHUNK_SIZE);
			CommandEnd();

			PushIndex += CHUNK_SIZE;
		}

		return 0;
	}

	virtual const bool PushingBuffer(const uint8_t* frameBuffer) final
	{
		if (PushIndex < WHOLE_SIZE)
		{
			CommandStart();
			WireInstance.write((uint8_t)SSD1306::CommandEnum::BufferStart);
			WireInstance.write(&frameBuffer[PushIndex], (size_t)CHUNK_SIZE);
			CommandEnd();

			PushIndex += CHUNK_SIZE;

			return true;
		}
		else
		{
			if (REMAINDER_SIZE > 0)
			{
				CommandStart();
				WireInstance.write((uint8_t)SSD1306::CommandEnum::BufferStart);
				WireInstance.write(&frameBuffer[REMAINDER_START], REMAINDER_SIZE);
				CommandEnd();
			}

			return false;
		}
	}
};

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000>
class ScreenDriverSSD1306_72x40x1_I2C : public AbstractScreenDriverSSD1306_I2C<SSD1306_72x40::Width, SSD1306_72x40::Height, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed>
{
private:
	using BaseClass = AbstractScreenDriverSSD1306_I2C<SSD1306_72x40::Width, SSD1306_72x40::Height, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed>;

public:
	ScreenDriverSSD1306_72x40x1_I2C() : BaseClass() {}

	virtual const bool Start()
	{
		return BaseClass::Start() && BaseClass::Initialize(SSD1306_72x40::ConfigBatch, SSD1306_72x40::ConfigBatchSize);
	}
};

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000,
	const uint8_t i2cChunkSize = 8>
class ScreenDriverSSD1306_72x40x1_I2C_Async : public AbstractScreenDriverSSD1306_I2C_Async<SSD1306_72x40::Width, SSD1306_72x40::Height, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed, i2cChunkSize>
{
private:
	using BaseClass = AbstractScreenDriverSSD1306_I2C_Async<SSD1306_72x40::Width, SSD1306_72x40::Height, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed, i2cChunkSize>;

public:
	ScreenDriverSSD1306_72x40x1_I2C_Async() : BaseClass() {}

	virtual const bool Start() final
	{
		return BaseClass::Start() && BaseClass::Initialize(SSD1306_72x40::ConfigBatch, SSD1306_72x40::ConfigBatchSize);
	}
};


template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000>
class ScreenDriverSSD1306_128x64x1_I2C : public AbstractScreenDriverSSD1306_I2C<SSD1306_128x64::Width, SSD1306_128x64::Height, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed>
{
private:
	using BaseClass = AbstractScreenDriverSSD1306_I2C<SSD1306_128x64::Width, SSD1306_128x64::Height, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed>;

public:
	ScreenDriverSSD1306_128x64x1_I2C() : BaseClass() {}

	virtual const bool Start()
	{
		return BaseClass::Start() && BaseClass::Initialize(SSD1306_128x64::ConfigBatch, SSD1306_128x64::ConfigBatchSize);
	}
};

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000,
	const uint8_t i2cChunkSize = 8>
class ScreenDriverSSD1306_128x64x1_I2C_Async : public AbstractScreenDriverSSD1306_I2C_Async<SSD1306_128x64::Width, SSD1306_128x64::Height, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed, i2cChunkSize>
{
private:
	using BaseClass = AbstractScreenDriverSSD1306_I2C_Async<SSD1306_128x64::Width, SSD1306_128x64::Height, pinSCL, pinSDA, pinRST, SSD1306::I2CAddress, i2cChannel, i2cSpeed, i2cChunkSize>;

public:
	ScreenDriverSSD1306_128x64x1_I2C_Async() : BaseClass() {}

	virtual const bool Start() final
	{
		return BaseClass::Start() && BaseClass::Initialize(SSD1306_128x64::ConfigBatch, SSD1306_128x64::ConfigBatchSize);
	}
};

#if defined(TEMPLATE_SCREEN_DRIVER_RTOS)
template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000,
	const uint32_t pushSleepDuration = 0,
	uint32_t stackHeight = 1500,
	portBASE_TYPE priority = 1>
class ScreenDriverSSD1306_72x40x1_I2C_Rtos : public TemplateScreenDriverRtos<ScreenDriverSSD1306_72x40x1_I2C<pinSCL, pinSDA, pinRST, i2cChannel, i2cSpeed>, pushSleepDuration, stackHeight, priority>
{};

template<const uint8_t pinSCL = UINT8_MAX,
	const uint8_t pinSDA = UINT8_MAX,
	const uint8_t pinRST = UINT8_MAX,
	const uint8_t i2cChannel = 0,
	const uint32_t i2cSpeed = 400000,
	const uint32_t pushSleepDuration = 0,
	uint32_t stackHeight = 1500,
	portBASE_TYPE priority = 1>
class ScreenDriverSSD1306_128x64x1_I2C_Rtos : public TemplateScreenDriverRtos<ScreenDriverSSD1306_128x64x1_I2C<pinSCL, pinSDA, pinRST, i2cChannel, i2cSpeed>, pushSleepDuration, stackHeight, priority>
{};
#endif

#endif