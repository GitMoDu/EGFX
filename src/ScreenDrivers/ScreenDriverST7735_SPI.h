#ifndef _SCREEN_DRIVER_ST7735_SPI_h
#define _SCREEN_DRIVER_ST7735_SPI_h

#include "AbstractScreenDriverSPI.h"
#include "TemplateScreenDriverRtos.h"
#include "TemplateScreenDriverSpiAsync.h"
#include "TemplateScreenDriverSpiDma.h"
#include "ST7735/ST7735.h"

namespace Egfx
{
	template<typename pixel_color_t,
		typename panel_t,
		const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed>
	class AbstractScreenDriverST7735S_SPI : public AbstractScreenDriverSPI<
		Egfx::GetFrameBufferSize<pixel_color_t>(panel_t::Width, panel_t::Height),
		panel_t::Width,
		panel_t::Height,
		pinCS, pinDC, pinRST>
	{
	private:
		using BaseClass = AbstractScreenDriverSPI<
			Egfx::GetFrameBufferSize<pixel_color_t>(panel_t::Width, panel_t::Height),
			panel_t::Width,
			panel_t::Height,
			pinCS, pinDC, pinRST>;

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
		AbstractScreenDriverST7735S_SPI(Egfx::SpiType& spi)
			: BaseClass(spi)
			, Settings(ST7735::LimitedSpiSpeed(spiSpeed), MSBFIRST, SPI_MODE0)
		{
		}

		~AbstractScreenDriverST7735S_SPI() override = default;

	protected:
		void SetupDisplayArea(const uint8_t width, const uint8_t height, const uint8_t offsetX, const uint8_t offsetY)
		{
				const uint16_t x0 = (uint16_t)panel_t::XOffset;
				const uint16_t y0 = (uint16_t)panel_t::YOffset;
				const uint16_t x1 = (uint16_t)panel_t::XOffset + (uint16_t)panel_t::Width - 1;
				const uint16_t y1 = (uint16_t)panel_t::YOffset + (uint16_t)panel_t::Height - 1;

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::SetColumnAddress);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer((uint8_t)(x0 >> 8));
				SpiInstance.transfer((uint8_t)(x0 & 0xFF));
				SpiInstance.transfer((uint8_t)(x1 >> 8));
				SpiInstance.transfer((uint8_t)(x1 & 0xFF));
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::SetRowAddress);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer((uint8_t)(y0 >> 8));
				SpiInstance.transfer((uint8_t)(y0 & 0xFF));
				SpiInstance.transfer((uint8_t)(y1 >> 8));
				SpiInstance.transfer((uint8_t)(y1 & 0xFF));
				CommandEnd();
		}

	public:
		virtual bool Start()
		{
			if (sizeof(pixel_t) != sizeof(uint16_t))
			{
				return false;
			}

			if (BaseClass::Start())
			{
				PinReset(ST7735::RESET_DELAY_MICROS);

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::SoftwareReset);
				CommandEnd();
				delayMicroseconds(ST7735::RESET_WAIT_MICROS);

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::SetSleepOff);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::SetDisplayOff);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::FrameRateControl1);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0x00);
				SpiInstance.transfer(0b111111);
				SpiInstance.transfer(0b111111);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::FrameRateControl2);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0b1111);
				SpiInstance.transfer(0x01);
				SpiInstance.transfer(0x01);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::FrameRateControl3);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0x05);
				SpiInstance.transfer(0x3c);
				SpiInstance.transfer(0x3c);
				SpiInstance.transfer(0x05);
				SpiInstance.transfer(0x3c);
				SpiInstance.transfer(0x3c);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::InversionControl);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0x03);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::PowerControl1);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0b11111100);
				SpiInstance.transfer(0x08);
				SpiInstance.transfer(0b10);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::PowerControl2);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0xC0);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::PowerControl3);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0x0D);
				SpiInstance.transfer(0x00);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::PowerControl4);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0x8D);
				SpiInstance.transfer(0x2A);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::PowerControl5);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0x8D);
				SpiInstance.transfer(0xEE);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::GatePumpClockFrequency);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0b11011000);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::NvmControlStatus);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0b01000000);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::PowerVmControl);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0b001111);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::PowerLControl);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0b10000);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::SetGamma);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0x08);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::SetMemoryDataAccessControl);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(panel_t::Madctl);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::SetPixelFormat);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0x05);
				CommandEnd();
				delay(10);

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::SetGammaPositiveCorrection);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer((uint8_t*)ST7735::GammaPositiveCorrection, sizeof(ST7735::GammaPositiveCorrection));
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::SetGammaNegativeCorrection);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer((uint8_t*)ST7735::GammaNegativeCorrection, sizeof(ST7735::GammaNegativeCorrection));
				CommandEnd();

				const uint16_t x0 = (uint16_t)panel_t::XOffset;
				const uint16_t y0 = (uint16_t)panel_t::YOffset;
				const uint16_t x1 = (uint16_t)panel_t::XOffset + (uint16_t)panel_t::Width - 1;
				const uint16_t y1 = (uint16_t)panel_t::YOffset + (uint16_t)panel_t::Height - 1;

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::SetColumnAddress);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer((uint8_t)(x0 >> 8));
				SpiInstance.transfer((uint8_t)(x0 & 0xFF));
				SpiInstance.transfer((uint8_t)(x1 >> 8));
				SpiInstance.transfer((uint8_t)(x1 & 0xFF));
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::SetRowAddress);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer((uint8_t)(y0 >> 8));
				SpiInstance.transfer((uint8_t)(y0 & 0xFF));
				SpiInstance.transfer((uint8_t)(y1 >> 8));
				SpiInstance.transfer((uint8_t)(y1 & 0xFF));
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)(panel_t::InvertColors
					? ST7735::CommandEnum::SetInversionOn
					: ST7735::CommandEnum::SetInversionOff));
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::TearingOff);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::SetIdleOff);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::PartialDisplayOff);
				CommandEnd();
				delay(10);

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::SetDisplayOn);
				CommandEnd();

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7735::CommandEnum::PartialDisplayOn);
				CommandEnd();
				delay(255);

				return true;
			}

			return false;
		}

		virtual void StartBuffer()
		{
			CommandStart(Settings);
			SpiInstance.transfer((uint8_t)ST7735::CommandEnum::MemoryWrite);
			digitalWrite(pinDC, HIGH);
		}
	};

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = ST7735::SpiMaxSpeed>
	using ScreenDriverST7735S_80x160x16_SPI =
		AbstractScreenDriverST7735S_SPI<uint16_t, ST7735::ST7735S, pinCS, pinDC, pinRST, spiSpeed>;

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = ST7735::SpiMaxSpeed>
	using ScreenDriverST7735S_160x128x16_SPI =
		AbstractScreenDriverST7735S_SPI<uint16_t, ST7735::ST7735S_160x128, pinCS, pinDC, pinRST, spiSpeed>;

	// Async variants
	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = ST7735::SpiMaxSpeed,
		const uint8_t spiChunkDivisor = 2>
	using ScreenDriverST7735S_80x160x16_SPI_Async =
		TemplateScreenDriverSpiAsync<ScreenDriverST7735S_80x160x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, spiChunkDivisor>;

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = ST7735::SpiMaxSpeed,
		const uint8_t spiChunkDivisor = 2>
	using ScreenDriverST7735S_160x128x16_SPI_Async =
		TemplateScreenDriverSpiAsync<ScreenDriverST7735S_160x128x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, spiChunkDivisor>;

#if defined(TEMPLATE_SCREEN_DRIVER_SPI_DMA)
	// DMA variants
	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = ST7735::SpiMaxSpeed,
		const uint32_t pushSleepDuration = 0>
	using ScreenDriverST7735S_80x160x16_SPI_Dma =
		TemplateScreenDriverSpiDma<ScreenDriverST7735S_80x160x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration>;

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = ST7735::SpiMaxSpeed,
		const uint32_t pushSleepDuration = 0>
	using ScreenDriverST7735S_160x128x16_SPI_Dma =
		TemplateScreenDriverSpiDma<ScreenDriverST7735S_160x128x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration>;
#endif

#if defined(TEMPLATE_SCREEN_DRIVER_RTOS)
	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = ST7735::SpiMaxSpeed,
		uint32_t stackHeight = 1500,
		portBASE_TYPE priority = 1
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		, const uint32_t coreAffinity = tskNO_AFFINITY
#endif
	>
	using ScreenDriverST7735S_80x160x16_SPI_Rtos =
		TemplateScreenDriverRtos<Egfx::SpiType, ScreenDriverST7735S_80x160x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, stackHeight, priority
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		, coreAffinity
#endif
		>;

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = ST7735::SpiMaxSpeed,
		uint32_t stackHeight = 1500,
		portBASE_TYPE priority = 1
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		, const uint32_t coreAffinity = tskNO_AFFINITY
#endif
	>
	using ScreenDriverST7735S_160x128x16_SPI_Rtos =
		TemplateScreenDriverRtos<Egfx::SpiType, ScreenDriverST7735S_160x128x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, stackHeight, priority
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		, coreAffinity
#endif
		>;
#endif
}
#endif