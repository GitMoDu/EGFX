#ifndef _SCREEN_DRIVER_SSD1306_SPI_h
#define _SCREEN_DRIVER_SSD1306_SPI_h

#include "AbstractScreenDriverSPI.h"
#include "TemplateScreenDriverRtos.h"
#include "TemplateScreenDriverSpiAsync.h"
#include "TemplateScreenDriverSpiDma.h"
#include "SSD1306\SSD1306.h"

namespace Egfx
{
	template<const pixel_t width,
		const pixel_t height,
		const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed>
	class AbstractScreenDriverSSD1306_SPI : public AbstractScreenDriverSPI<Egfx::GetFrameBufferMonochromeSize(width, height), width, height, pinCS, pinDC, pinRST>
	{
	private:
		using BaseClass = AbstractScreenDriverSPI<Egfx::GetFrameBufferMonochromeSize(width, height), width, height, pinCS, pinDC, pinRST>;

	public:
		using BaseClass::ScreenWidth;
		using BaseClass::ScreenHeight;
		using BaseClass::BufferSize;

	protected:
		using BaseClass::SpiInstance;
		using BaseClass::CommandStart;
		using BaseClass::CommandEnd;
		using BaseClass::SpiTransfer;
		using BaseClass::PinReset;

	private:
		SPISettings Settings;

	public:
		AbstractScreenDriverSSD1306_SPI(Egfx::SpiType& spi)
			: BaseClass(spi)
			, Settings(spiSpeed, MSBFIRST, SPI_MODE0)
		{
		}

	public:
		virtual void StartBuffer()
		{
			CommandStart(Settings);
			SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::CommandStart);

			SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::Column);
			SpiInstance.transfer((uint8_t)0);
			SpiInstance.transfer((uint8_t)(ScreenWidth - 1));

			SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::Page);
			SpiInstance.transfer((uint8_t)0);
			SpiInstance.transfer((uint8_t)((SSD1306::Height / 8) - 1));

			digitalWrite(pinDC, HIGH);
		}

	protected:
		bool Initialize(const bool backlightInternal = false)
		{
			PinReset(SSD1306::RESET_WAIT_MICROS);
			delayMicroseconds(SSD1306::RESET_DELAY_MICROS);

			CommandStart(Settings);
			SpiTransfer(SSD1306::ConfigBatch, sizeof(SSD1306::ConfigBatch));
			CommandEnd();

			SetBacklightMode(backlightInternal);

			return true;
		}

	private:
		void CommandReset()
		{
			CommandStart(Settings);
			SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::CommandStart);
			SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::Reset);
			CommandEnd();
		}

		void SetBacklightMode(const bool backlightInternal)
		{
			CommandStart(Settings);
			SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::CommandStart);
			//SpiInstance.transfer((uint8_t)SSD1306::CommandEnum::Backlight);
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

	template<const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed = 4000000>
	class ScreenDriverSSD1306_128x64x1_SPI : public AbstractScreenDriverSSD1306_SPI<SSD1306_128x64::Width, SSD1306_128x64::Height, pinCS, pinDC, pinRST, spiSpeed>
	{
	private:
		using BaseClass = AbstractScreenDriverSSD1306_SPI<SSD1306_128x64::Width, SSD1306_128x64::Height, pinCS, pinDC, pinRST, spiSpeed>;

	public:
		ScreenDriverSSD1306_128x64x1_SPI(Egfx::SpiType& spi) : BaseClass(spi) {}

		virtual bool Start()
		{
			return BaseClass::Start() && BaseClass::Initialize(false);
		}
	};

	template<const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed = 4000000,
		const uint8_t spiChunkDivisor = 2>
	using ScreenDriverSSD1306_128x64x1_SPI_Async = TemplateScreenDriverSpiAsync<ScreenDriverSSD1306_128x64x1_SPI<pinCS, pinDC, pinRST, spiSpeed>, spiChunkDivisor>;

#if defined(TEMPLATE_SCREEN_DRIVER_SPI_DMA)
	template<const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed = 4000000,
		const uint32_t pushSleepDuration = 0>
	using ScreenDriverSSD1306_128x64x1_SPI_Dma = TemplateScreenDriverSpiDma<ScreenDriverSSD1306_128x64x1_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration>;
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
	using ScreenDriverSSD1306_128x64x1_SPI_Rtos = TemplateScreenDriverRtos<Egfx::SpiType, ScreenDriverSSD1306_128x64x1_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration, stackHeight, priority
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		, coreAffinity
#endif
	>;
#endif
#endif
}