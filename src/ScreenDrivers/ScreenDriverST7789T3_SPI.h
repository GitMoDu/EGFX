#ifndef _EGFX_SCREEN_DRIVERS_ST7789T3_SPI_h
#define _EGFX_SCREEN_DRIVERS_ST7789T3_SPI_h

#include "AbstractScreenDriverSPI.h"
#include "TemplateScreenDriverRtos.h"
#include "TemplateScreenDriverSpiAsync.h"
#include "TemplateScreenDriverSpiDma.h"
#include "ST7789/ST7789.h"
#include "ST7789/ST7789T3.h"

namespace Egfx
{
	template<typename pixel_color_t,
		const pixel_t width,
		const pixel_t height,
		const uint16_t offsetX,
		const uint16_t offsetY,
		const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed>
	class AbstractScreenDriverST7789T3_SPI :
		public AbstractScreenDriverSPI<Egfx::GetFrameBufferSize<pixel_color_t>(width, height),
		width, height, pinCS, pinDC, pinRST>
	{
	private:
		using BaseClass = AbstractScreenDriverSPI<Egfx::GetFrameBufferSize<pixel_color_t>(width, height),
			width, height, pinCS, pinDC, pinRST>;

		static_assert(width <= ST7789::Width, "ST7789T3 panel width exceeded.");
		static_assert(height <= ST7789::Height, "ST7789T3 panel height exceeded.");

		SPISettings Settings;

	public:
		AbstractScreenDriverST7789T3_SPI(Egfx::SpiType& spi)
			: BaseClass(spi)
			, Settings(ST7789T3::LimitedSpiSpeed(spiSpeed), MSBFIRST, SPI_MODE0)
		{
		}

		bool Start() override
		{
			if (!BaseClass::Start())
				return false;

			BaseClass::PinReset(ST7789T3::RESET_DELAY_MICROS);

			return Initialize();
		}

		void StartBuffer() override
		{
			ConfigureAddressWindow();  // full-screen window every frame

			BaseClass::CommandStart(Settings);
			BaseClass::SpiInstance.transfer((uint8_t)ST7789T3::CommandEnum::MemoryWrite);
			digitalWrite(pinDC, HIGH);
		}

		void EndBuffer() override
		{
			BaseClass::SpiInstance.transfer((uint8_t)ST7789T3::CommandEnum::Nop);
			BaseClass::EndBuffer();
		}

	private:
		bool Initialize()
		{
			// Reset + Sleep Out
			SendCommand(ST7789T3::CommandEnum::SoftwareReset);
			DelayMicros(ST7789T3::RESET_WAIT_MICROS);

			SendCommand(ST7789T3::CommandEnum::SleepOut);
			DelayMicros(ST7789T3::SLEEP_EXIT_DELAY_MICROS);

			// Pixel format: RGB565
			SendCommand(ST7789T3::CommandEnum::SetPixelFormat,
				ST7789T3::PixelFormats::Rgb565);

			// MADCTL: no rotation, no mirroring, BGR order
			SendCommand(ST7789T3::CommandEnum::SetMemoryAccessControl, 0x08);

			// Porch
			{
				const uint8_t d[]{ 0x0C, 0x0C, 0x00, 0x33, 0x33 };
				SendCommand(ST7789T3::CommandEnum::PorchSetting, d, sizeof(d));
			}

			// Gate
			{
				const uint8_t d[]{ 0x35 };
				SendCommand(ST7789T3::CommandEnum::GateControl, d, sizeof(d));
			}

			// VCOM
			{
				const uint8_t d[]{ 0x28 };
				SendCommand(ST7789T3::CommandEnum::VCOMSSetting, d, sizeof(d));
			}

			// LCM
			{
				const uint8_t d[]{ 0x0C };
				SendCommand(ST7789T3::CommandEnum::LCMControl, d, sizeof(d));
			}

			// VDV/VRH enable
			{
				const uint8_t d[]{ 0x01 };
				SendCommand(ST7789T3::CommandEnum::VDVVRHEN, d, sizeof(d));
			}

			// VRH
			{
				const uint8_t d[]{ 0x10 };
				SendCommand(ST7789T3::CommandEnum::VRHSet, d, sizeof(d));
			}

			// VDV
			{
				const uint8_t d[]{ 0x20 };
				SendCommand(ST7789T3::CommandEnum::VDVSet, d, sizeof(d));
			}

			// Frame rate
			{
				const uint8_t d[]{ 0x0F };
				SendCommand(ST7789T3::CommandEnum::FrameRateControl, d, sizeof(d));
			}

			// Power
			{
				const uint8_t d[]{ 0xA4, 0xA1 };
				SendCommand(ST7789T3::CommandEnum::PowerControl1, d, sizeof(d));
			}

			// Gamma (JLX240)
			{
				const uint8_t d[]{
					0xD0,0x00,0x02,0x07,0x0A,0x28,0x32,0x44,
					0x42,0x06,0x0E,0x12,0x14,0x17
				};
				SendCommand(ST7789T3::CommandEnum::PositiveGamma, d, sizeof(d));
			}
			{
				const uint8_t d[]{
					0xD0,0x00,0x02,0x07,0x0A,0x28,0x31,0x54,
					0x47,0x0E,0x1C,0x17,0x1B,0x1E
				};
				SendCommand(ST7789T3::CommandEnum::NegativeGamma, d, sizeof(d));
			}

			// Inversion ON
			SendCommand(ST7789T3::CommandEnum::InversionOn);

			SendCommand(ST7789T3::CommandEnum::NormalDisplayOn);
			DelayMicros(ST7789T3::DISPLAY_ON_DELAY_MICROS);

			SendCommand(ST7789T3::CommandEnum::DisplayOn);
			DelayMicros(ST7789T3::DISPLAY_ON_DELAY_MICROS);

			return true;
		}

		void ConfigureAddressWindow()
		{
			// Full 240×320 window, no offsets.
			const uint8_t caset[]{
				0x00, 0x00,
				0x00, uint8_t(width - 1)      // 0x00EF for 240
			};
			SendCommand(ST7789T3::CommandEnum::SetColumnAddress, caset, sizeof(caset));

			const uint16_t hEnd = height - 1; // 0x013F for 320
			const uint8_t raset[]{
				0x00, 0x00,
				uint8_t(hEnd >> 8),
				uint8_t(hEnd & 0xFF)
			};
			SendCommand(ST7789T3::CommandEnum::SetRowAddress, raset, sizeof(raset));
		}

		void SendCommand(const ST7789T3::CommandEnum cmd)
		{
			SendCommand(cmd, nullptr, 0);
		}

		void SendCommand(const ST7789T3::CommandEnum cmd, uint8_t value)
		{
			const uint8_t d[]{ value };
			SendCommand(cmd, d, 1);
		}

		void SendCommand(const ST7789T3::CommandEnum cmd,
			const uint8_t* data, size_t len)
		{
			BaseClass::CommandStart(Settings);
			BaseClass::SpiInstance.transfer((uint8_t)cmd);

			if (len && data)
			{
				digitalWrite(pinDC, HIGH);
				BaseClass::SpiTransfer(data, len);
				digitalWrite(pinDC, LOW);
			}

			BaseClass::CommandEnd();
		}

		static void DelayMicros(uint32_t us)
		{
			if (!us) return;
			delay(us / 1000);
			delayMicroseconds(us % 1000);
		}
	};

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = ST7789T3::SpiDefaultSpeed>
	using ScreenDriverST7789T3_240x320x16_SPI =
		AbstractScreenDriverST7789T3_SPI<uint16_t,
		ST7789::Width,
		ST7789::Height,
		0, 0,
		pinCS, pinDC, pinRST,
		spiSpeed>;

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = ST7789T3::SpiDefaultSpeed,
		const uint8_t spiChunkDivisor = 2>
	using ScreenDriverST7789T3_240x320x16_SPI_Async =
		TemplateScreenDriverSpiAsync<ScreenDriverST7789T3_240x320x16_SPI<pinCS, pinDC, pinRST, spiSpeed>,
		spiChunkDivisor>;

#if defined(TEMPLATE_SCREEN_DRIVER_SPI_DMA)
	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = ST7789T3::SpiDefaultSpeed,
		const uint32_t pushSleepDuration = 0>
	using ScreenDriverST7789T3_240x320x16_SPI_Dma =
		TemplateScreenDriverSpiDma<ScreenDriverST7789T3_240x320x16_SPI<pinCS, pinDC, pinRST, spiSpeed>,
		pushSleepDuration,
		ST7789T3::SpiMaxChunkSize>;
#endif

#if defined(TEMPLATE_SCREEN_DRIVER_RTOS)
	template<const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed = ST7789T3::SpiDefaultSpeed,
		const uint32_t pushSleepDuration = 0,
		uint32_t stackHeight = 1500,
		portBASE_TYPE priority = 1
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		, const uint32_t coreAffinity = tskNO_AFFINITY
#endif
	>
	using ScreenDriverST7789T3_240x320x16_SPI_Rtos = TemplateScreenDriverRtos<Egfx::SpiType, ScreenDriverST7789T3_240x320x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration, stackHeight, priority
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		, coreAffinity
#endif
	>;
#endif
}

#endif
