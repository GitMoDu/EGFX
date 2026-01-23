#ifndef _SCREEN_DRIVER_SH1107_I2C_h
#define _SCREEN_DRIVER_SH1107_I2C_h

#include "AbstractScreenDriverI2C.h"
#include "TemplateScreenDriverRtos.h"
#include "TemplateScreenDriverI2CDma.h"
#include "TemplateScreenDriverI2CAsync.h"
#include "SH1107/SH1107.h"

namespace Egfx
{
	template<const uint8_t width,
		const uint8_t height,
		const uint8_t i2cAddress>
	class AbstractScreenDriverSH1107_I2C : public AbstractScreenDriverI2C<width, height, i2cAddress>
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
		// Conservative: same as SSD1306 driver (keeps payload <= 32 bytes on Arduino Wire).
		static constexpr uint8_t I2C_BUFFER_SIZE = 31;

		static constexpr uint16_t BUFFER_WHOLE = (uint16_t)(BufferSize / I2C_BUFFER_SIZE);
		static constexpr uint8_t BUFFER_REMAINDER = (uint8_t)(BufferSize % I2C_BUFFER_SIZE);
		static constexpr size_t REMAINDER_START = (size_t)BUFFER_WHOLE * I2C_BUFFER_SIZE;

	public:
		AbstractScreenDriverSH1107_I2C(TwoWire& wire)
			: BaseClass(wire)
		{
		}

		~AbstractScreenDriverSH1107_I2C() override = default;

		bool Start() override
		{
			return BaseClass::Start();
		}

		void Stop() override
		{
			WireInstance.end();
		}

		bool CanPushBuffer() override
		{
			return true;
		}

		void StartBuffer() override
		{
		}

		uint32_t PushBuffer(const uint8_t* frameBuffer) override
		{
			for (uint8_t p = 0; p < (uint8_t)(ScreenHeight / 8); p++)
			{
				// Set page + column = 0 before sending the page data
				CommandStart();
				WireInstance.write((uint8_t)SH1107::CommandEnum::CommandStart); // 0x00 commands
				WireInstance.write((uint8_t)SH1107::CommandEnum::PushPage + p);
				WireInstance.write((uint8_t)SH1107::CommandEnum::PushAddressHigh);
				WireInstance.write((uint8_t)SH1107::CommandEnum::PushAddressLow);
				CommandEnd();

				// Push this page in chunks (Wire limit)
				const uint8_t* pagePtr = &frameBuffer[(uint_least16_t)p * (uint_least16_t)ScreenWidth];

				for (uint8_t x = 0; x < ScreenWidth; x = (uint8_t)(x + I2C_BUFFER_SIZE))
				{
					const uint8_t remaining = (uint8_t)(ScreenWidth - x);
					const uint8_t chunkSize = (remaining > I2C_BUFFER_SIZE) ? I2C_BUFFER_SIZE : remaining;

					PushChunk(&pagePtr[x], chunkSize);
				}
			}

			return 0;
		}

		bool PushingBuffer(const uint8_t* /*frameBuffer*/) override
		{
			return false;
		}

		void EndBuffer() override
		{
		}

	protected:
		// Note: SH1107 init varies by module. This just sends SH1107::ConfigBatch.
		bool Initialize()
		{
			delayMicroseconds(SH1107::RESET_DELAY_MICROS);

			CommandStart();
			WireInstance.write((uint8_t)SH1107::CommandEnum::CommandStart); // 0x00 control-byte for commands
			WireInstance.write(SH1107::ConfigBatch, sizeof(SH1107::ConfigBatch));
			CommandEnd();

			delayMicroseconds(SH1107::BOOT_DELAY_MICROS);

			return true;
		}

	private:
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
			WireInstance.write(0x40); // data control byte
			WireInstance.write(chunk, chunkSize);
			CommandEnd();
		}

		void CommandStartBuffer()
		{
			// For SH1107, many panels accept SH1106/SSD1306-like page mode setup.
			// Set column=0..(width-1) and page=0..(height/8-1).
			CommandStart();
			WireInstance.write((uint8_t)SH1107::CommandEnum::CommandStart); // command control byte

			// Column = 0
			WireInstance.write((uint8_t)SH1107::CommandEnum::PushAddressHigh); // 0x10
			WireInstance.write((uint8_t)SH1107::CommandEnum::PushAddressLow);  // 0x00

			// Page will be advanced during PushBuffer() page loop in memory layout,
			// but for I2C streaming we just start at page 0.
			WireInstance.write((uint8_t)SH1107::CommandEnum::PushPage); // page 0 (0xB0 + 0)

			CommandEnd();
		}
	};

	class ScreenDriverSH1107_128x128x1_I2C : public AbstractScreenDriverSH1107_I2C<SH1107::Width, SH1107::Height, SH1107::I2CAddress>
	{
	private:
		using BaseClass = AbstractScreenDriverSH1107_I2C<SH1107::Width, SH1107::Height, SH1107::I2CAddress>;

	public:
		ScreenDriverSH1107_128x128x1_I2C(TwoWire& wire)
			: BaseClass(wire)
		{
		}

		~ScreenDriverSH1107_128x128x1_I2C() override = default;

		bool Start() override
		{
			return BaseClass::Start() && BaseClass::Initialize();
		}
	};

	template<const uint8_t i2cChunkSize = 8>
	using ScreenDriverSH1107_128x128x1_I2C_Async =
		TemplateScreenDriverI2CAsync<ScreenDriverSH1107_128x128x1_I2C, i2cChunkSize>;

#if defined(TEMPLATE_SCREEN_DRIVER_RTOS)
	template<const uint32_t pushSleepDuration = 0,
		uint32_t stackHeight = 1500,
		portBASE_TYPE priority = 1>
	using ScreenDriverSH1107_128x128x1_I2C_Rtos =
		TemplateScreenDriverRtos<TwoWire, ScreenDriverSH1107_128x128x1_I2C, pushSleepDuration, stackHeight, priority>;
#endif

#if defined(TEMPLATE_SCREEN_DRIVER_I2C_DMA)
	using ScreenDriverSH1107_128x128x1_I2C_Dma = TemplateScreenDriverI2CDma<ScreenDriverSH1107_128x128x1_I2C>;
#endif
}

#endif