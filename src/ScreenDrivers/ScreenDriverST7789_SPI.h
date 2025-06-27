// ScreenDriverST7789_SPI.h

#ifndef _SCREEN_DRIVER_ST7789_SPI_h
#define _SCREEN_DRIVER_ST7789_SPI_h

#include "AbstractScreenDriverSPI.h"
#include "TemplateScreenDriverRtos.h"
#include "TemplateScreenDriverSpiAsync.h"
#include "TemplateScreenDriverSpiDma.h"
#include "ST7789/ST7789.h"

namespace Egfx
{
	template<typename pixel_color_t,
		const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed>
	class AbstractScreenDriverST7789_SPI : public AbstractScreenDriverSPI<Egfx::GetFrameBufferSize<pixel_color_t>(ST7789::Width, ST7789::Height), ST7789::Width, ST7789::Height, pinCS, pinDC, pinRST>
	{
	private:
		using BaseClass = AbstractScreenDriverSPI<Egfx::GetFrameBufferSize<pixel_color_t>(ST7789::Width, ST7789::Height), ST7789::Width, ST7789::Height, pinCS, pinDC, pinRST>;

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
		AbstractScreenDriverST7789_SPI(Egfx::SpiType& spi)
			: BaseClass(spi)
			, Settings(ST7789::LimitedSpiSpeed(spiSpeed), MSBFIRST, SPI_MODE0)
		{
		}

	public:
		virtual bool Start()
		{
			if (BaseClass::Start())
			{
				PinReset(ST7789::RESET_DELAY_MICROS);

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7789::CommandEnum::SoftwareReset);
				CommandEnd();
				delayMicroseconds(ST7789::RESET_WAIT_MICROS);

				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)ST7789::CommandEnum::SetSleepOff);
				SpiInstance.transfer((uint8_t)ST7789::CommandEnum::SetPixelFormat);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0x55);
				digitalWrite(pinDC, LOW);

				SpiInstance.transfer((uint8_t)ST7789::CommandEnum::SetMemoryDataAccessControl);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0x00);
				digitalWrite(pinDC, LOW);

				SpiInstance.transfer((uint8_t)ST7789::CommandEnum::SetColumnAddress);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0x00);
				SpiInstance.transfer(0x00);
				SpiInstance.transfer((uint8_t)((ST7789::Width) >> 8));
				SpiInstance.transfer((uint8_t)ST7789::Width);
				digitalWrite(pinDC, LOW);

				SpiInstance.transfer((uint8_t)ST7789::CommandEnum::SetRowAddress);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0x00);
				SpiInstance.transfer(0x00);
				SpiInstance.transfer((uint8_t)((ST7789::Height) >> 8));
				SpiInstance.transfer((uint8_t)ST7789::Height);
				digitalWrite(pinDC, LOW);

				SpiInstance.transfer((uint8_t)ST7789::CommandEnum::SetInversionOn);
				SpiInstance.transfer((uint8_t)ST7789::CommandEnum::PartialDisplayOff);
				SpiInstance.transfer((uint8_t)ST7789::CommandEnum::SetDisplayOn);
				CommandEnd();

				return true;
			}

			return false;
		}

		virtual void StartBuffer()
		{
			CommandStart(Settings);
			SpiInstance.transfer((uint8_t)ST7789::CommandEnum::MemoryWrite);
			digitalWrite(pinDC, HIGH);
		}

		virtual void EndBuffer()
		{
			SpiInstance.transfer((uint8_t)ST7789::CommandEnum::Nop);
			BaseClass::EndBuffer();
		}
	};

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = 4000000>
	using ScreenDriverST7789_240x240x16_SPI = AbstractScreenDriverST7789_SPI<uint16_t, pinCS, pinDC, pinRST, spiSpeed>;

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = 4000000,
		const uint8_t spiChunkDivisor = 2>
	using ScreenDriverST7789_240x240x16_SPI_Async = TemplateScreenDriverSpiAsync<ScreenDriverST7789_240x240x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, spiChunkDivisor>;

#if defined(TEMPLATE_SCREEN_DRIVER_SPI_DMA)
	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = 4000000,
		const uint32_t pushSleepDuration = 0>
	using ScreenDriverST7789_240x240x16_SPI_Dma = TemplateScreenDriverSpiDma<ScreenDriverST7789_240x240x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration>;
#endif

#if defined(TEMPLATE_SCREEN_DRIVER_RTOS)
	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = 4000000,
		uint32_t stackHeight = 1500,
		portBASE_TYPE priority = 1>
	using ScreenDriverST7789_240x240x16_SPI_Rtos = TemplateScreenDriverRtos<ScreenDriverST7789_240x240x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, void, stackHeight, priority>;
#endif
}
#endif