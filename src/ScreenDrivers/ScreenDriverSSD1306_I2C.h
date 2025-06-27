#ifndef _SCREEN_DRIVER_SSD1306_I2C_h
#define _SCREEN_DRIVER_SSD1306_I2C_h

#include "AbstractScreenDriverI2C.h"
#include "TemplateScreenDriverRtos.h"
#include "TemplateScreenDriverI2CDma.h"
#include "TemplateScreenDriverI2CAsync.h"
#include "SSD1306\SSD1306.h"

namespace Egfx
{
	template<const uint8_t width,
		const uint8_t height,
		const int8_t verticalOffset,
		const uint8_t i2cAddress>
	class AbstractScreenDriverSSD1306_I2C : public AbstractScreenDriverI2C<width, height, i2cAddress>
	{
	public:
		static constexpr size_t BufferSize = Egfx::GetFrameBufferMonochromeSize(width, height);

	private:
		using BaseClass = AbstractScreenDriverI2C<width, height, i2cAddress>;

	public:
		using BaseClass::ScreenWidth;
		using BaseClass::ScreenHeight;

	protected:
		using BaseClass::WireInstance;
		using BaseClass::I2CAddress;

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
		AbstractScreenDriverSSD1306_I2C(TwoWire& wire) : BaseClass(wire)
		{
		}

		void SetConstrast(const int8_t constrast)
		{
			CommandStart();
			WireInstance.write((uint8_t)SSD1306::CommandEnum::CommandStart);
			WireInstance.write((uint8_t)SSD1306::CommandEnum::SetContrast);

			if (constrast >= 0)
			{
				WireInstance.write(constrast);
			}
			else
			{
				WireInstance.write(0);
			}
			CommandEnd();
		}

		virtual bool Start()
		{
			return BaseClass::Start();
		}

		virtual void Stop()
		{
			CommandReset();
			WireInstance.end();
		}

		virtual bool CanPushBuffer()
		{
			return true;
		}

		virtual void StartBuffer()
		{
			CommandStartBuffer();
		}

		virtual uint32_t PushBuffer(const uint8_t* frameBuffer)
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

		virtual bool PushingBuffer(const uint8_t* frameBuffer)
		{
			return false;
		}

		virtual void EndBuffer()
		{
		}

	protected:
		bool Initialize(const bool backlightInternal = false)
		{
			CommandReset();

			delayMicroseconds(SSD1306::RESET_DELAY_MICROS);

			CommandStart();
			WireInstance.write(SSD1306::ConfigBatch, sizeof(SSD1306::ConfigBatch));
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
			WireInstance.write((uint8_t)SSD1306::CommandEnum::SetLineStart);
			WireInstance.write(chunk, chunkSize);
			CommandEnd();
		}

#if defined(TEMPLATE_SCREEN_DRIVER_I2C_DMA)
		void PushChunkDma(const uint8_t* chunk, const size_t chunkSize)
		{
			DmaBuffer[0] = (uint8_t)SSD1306::CommandEnum::SetLineStart;
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
			//WireInstance.write((uint8_t)SSD1306::CommandEnum::Backlight);
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

	class ScreenDriverSSD1306_64x32x1_I2C : public AbstractScreenDriverSSD1306_I2C<SSD1306_64x32::Width, SSD1306_64x32::Height, 0, SSD1306::I2CAddress>
	{
	private:
		using BaseClass = AbstractScreenDriverSSD1306_I2C<SSD1306_64x32::Width, SSD1306_64x32::Height, 0, SSD1306::I2CAddress>;

	public:
		ScreenDriverSSD1306_64x32x1_I2C(TwoWire& wire) : BaseClass(wire) {}

		virtual bool Start()
		{
			return BaseClass::Start() && BaseClass::Initialize(false);
		}
	};

	class ScreenDriverSSD1306_64x48x1_I2C : public AbstractScreenDriverSSD1306_I2C<SSD1306_64x48::Width, SSD1306_64x48::Height, SSD1306_64x48::VerticalOffset, SSD1306::I2CAddress>
	{
	private:
		using BaseClass = AbstractScreenDriverSSD1306_I2C<SSD1306_64x48::Width, SSD1306_64x48::Height, SSD1306_64x48::VerticalOffset, SSD1306::I2CAddress>;

	public:
		ScreenDriverSSD1306_64x48x1_I2C(TwoWire& wire) : BaseClass(wire) {}

		virtual bool Start()
		{
			return BaseClass::Start() && BaseClass::Initialize(false);
		}
	};

	class ScreenDriverSSD1306_72x40x1_I2C : public AbstractScreenDriverSSD1306_I2C<SSD1306_72x40::Width, SSD1306_72x40::Height, 0, SSD1306::I2CAddress>
	{
	private:
		using BaseClass = AbstractScreenDriverSSD1306_I2C<SSD1306_72x40::Width, SSD1306_72x40::Height, 0, SSD1306::I2CAddress>;

	public:
		ScreenDriverSSD1306_72x40x1_I2C(TwoWire& wire) : BaseClass(wire) {}

		virtual bool Start()
		{
			return BaseClass::Start() && BaseClass::Initialize(true);
		}
	};

	class ScreenDriverSSD1306_128x32x1_I2C : public AbstractScreenDriverSSD1306_I2C<SSD1306_128x32::Width, SSD1306_128x32::Height, SSD1306_128x32::VerticalOffset, SSD1306::I2CAddress>
	{
	private:
		using BaseClass = AbstractScreenDriverSSD1306_I2C<SSD1306_128x32::Width, SSD1306_128x32::Height, SSD1306_128x32::VerticalOffset, SSD1306::I2CAddress>;

	public:
		ScreenDriverSSD1306_128x32x1_I2C(TwoWire& wire) : BaseClass(wire) {}

		virtual bool Start()
		{
			return BaseClass::Start() && BaseClass::Initialize(false);
		}
	};

	class ScreenDriverSSD1306_128x64x1_I2C : public AbstractScreenDriverSSD1306_I2C<SSD1306_128x64::Width, SSD1306_128x64::Height, 0, SSD1306::I2CAddress>
	{
	private:
		using BaseClass = AbstractScreenDriverSSD1306_I2C<SSD1306_128x64::Width, SSD1306_128x64::Height, 0, SSD1306::I2CAddress>;

	public:
		ScreenDriverSSD1306_128x64x1_I2C(TwoWire& wire) : BaseClass(wire) {}

		virtual bool Start()
		{
			return BaseClass::Start() && BaseClass::Initialize(false);
		}
	};

	template<const uint8_t i2cChunkSize = 8>
	using ScreenDriverSSD1306_64x32x1_I2C_Async = TemplateScreenDriverI2CAsync<ScreenDriverSSD1306_64x32x1_I2C, i2cChunkSize>;

	template<const uint8_t i2cChunkSize = 8>
	using ScreenDriverSSD1306_64x48x1_I2C_Async = TemplateScreenDriverI2CAsync<ScreenDriverSSD1306_64x48x1_I2C, i2cChunkSize>;

	template<const uint8_t i2cChunkSize = 8>
	using ScreenDriverSSD1306_72x40x1_I2C_Async = TemplateScreenDriverI2CAsync<ScreenDriverSSD1306_72x40x1_I2C, i2cChunkSize>;

	template<const uint8_t i2cChunkSize = 8>
	using ScreenDriverSSD1306_128x32x1_I2C_Async = TemplateScreenDriverI2CAsync<ScreenDriverSSD1306_128x32x1_I2C, i2cChunkSize>;

	template<const uint8_t i2cChunkSize = 8>
	using ScreenDriverSSD1306_128x64x1_I2C_Async = TemplateScreenDriverI2CAsync<ScreenDriverSSD1306_128x64x1_I2C, i2cChunkSize>;

#if defined(TEMPLATE_SCREEN_DRIVER_RTOS)
	template<const uint32_t pushSleepDuration = 0,
		uint32_t stackHeight = 1500,
		portBASE_TYPE priority = 1>
	using ScreenDriverSSD1306_64x32x1_I2C_Rtos = TemplateScreenDriverRtos<TwoWire, ScreenDriverSSD1306_64x32x1_I2C, pushSleepDuration, stackHeight, priority>;

	template<const uint32_t pushSleepDuration = 0,
		uint32_t stackHeight = 1500,
		portBASE_TYPE priority = 1>
	using ScreenDriverSSD1306_64x48x1_I2C_Rtos = TemplateScreenDriverRtos<TwoWire, ScreenDriverSSD1306_64x48x1_I2C, pushSleepDuration, stackHeight, priority>;

	template<const uint32_t pushSleepDuration = 0,
		uint32_t stackHeight = 1500,
		portBASE_TYPE priority = 1>
	using ScreenDriverSSD1306_72x40x1_I2C_Rtos = TemplateScreenDriverRtos<TwoWire, ScreenDriverSSD1306_72x40x1_I2C, pushSleepDuration, stackHeight, priority>;

	template<const uint32_t pushSleepDuration = 0,
		uint32_t stackHeight = 1500,
		portBASE_TYPE priority = 1>
	using ScreenDriverSSD1306_128x32x1_I2C_Rtos = TemplateScreenDriverRtos<TwoWire, ScreenDriverSSD1306_128x32x1_I2C, pushSleepDuration, stackHeight, priority>;

	template<const uint32_t pushSleepDuration = 0,
		uint32_t stackHeight = 1500,
		portBASE_TYPE priority = 1>
	using ScreenDriverSSD1306_128x64x1_I2C_Rtos = TemplateScreenDriverRtos<TwoWire, ScreenDriverSSD1306_128x64x1_I2C, pushSleepDuration, stackHeight, priority>;
#endif

#if defined(TEMPLATE_SCREEN_DRIVER_I2C_DMA)
	using ScreenDriverSSD1306_64x32x1_I2C_Dma = TemplateScreenDriverI2CDma<ScreenDriverSSD1306_64x32x1_I2C>;

	using ScreenDriverSSD1306_64x48x1_I2C_Dma = TemplateScreenDriverI2CDma<ScreenDriverSSD1306_64x48x1_I2C>;

	using ScreenDriverSSD1306_72x40x1_I2C_Dma = TemplateScreenDriverI2CDma<ScreenDriverSSD1306_72x40x1_I2C>;

	using ScreenDriverSSD1306_128x32x1_I2C_Dma = TemplateScreenDriverI2CDma<ScreenDriverSSD1306_128x32x1_I2C>;

	using ScreenDriverSSD1306_128x64x1_I2C_Dma = TemplateScreenDriverI2CDma<ScreenDriverSSD1306_128x64x1_I2C>;
#endif
}
#endif