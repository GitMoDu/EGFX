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
		const pixel_t width,
		const pixel_t height,
		const uint16_t offsetX,
		const uint16_t offsetY,
		const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed>
	class AbstractScreenDriverST7789_SPI : public AbstractScreenDriverSPI<Egfx::GetFrameBufferSize<pixel_color_t>(width, height), width, height, pinCS, pinDC, pinRST>
	{
	private:
		using BaseClass = AbstractScreenDriverSPI<Egfx::GetFrameBufferSize<pixel_color_t>(width, height), width, height, pinCS, pinDC, pinRST>;

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

		~AbstractScreenDriverST7789_SPI() override = default;

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
				SpiInstance.transfer((uint8_t)((offsetX) >> 8));
				SpiInstance.transfer((uint8_t)(offsetX & UINT8_MAX));
				SpiInstance.transfer((uint8_t)((((uint16_t)offsetX + (uint16_t)width - 1)) >> 8));
				SpiInstance.transfer((uint8_t)(((uint16_t)offsetX + (uint16_t)width - 1) & UINT8_MAX));
				digitalWrite(pinDC, LOW);

				SpiInstance.transfer((uint8_t)ST7789::CommandEnum::SetRowAddress);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer((uint8_t)((offsetY) >> 8));
				SpiInstance.transfer((uint8_t)(offsetY & UINT8_MAX));
				SpiInstance.transfer((uint8_t)((((uint16_t)offsetY + (uint16_t)height - 1)) >> 8));
				SpiInstance.transfer((uint8_t)(((uint16_t)offsetY + (uint16_t)height - 1) & UINT8_MAX));
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
	using ScreenDriverST7789_240x240x16_SPI = AbstractScreenDriverST7789_SPI<uint16_t, 240, 240, 0, 0, pinCS, pinDC, pinRST, spiSpeed>;

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = 4000000>
	using ScreenDriverST7789_172x320x16_SPI = AbstractScreenDriverST7789_SPI<uint16_t, 172, 320, 34, 0, pinCS, pinDC, pinRST, spiSpeed>;

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = 4000000,
		const uint8_t spiChunkDivisor = 2>
	using ScreenDriverST7789_240x240x16_SPI_Async = TemplateScreenDriverSpiAsync<ScreenDriverST7789_240x240x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, spiChunkDivisor>;

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = 4000000,
		const uint8_t spiChunkDivisor = 2>
	using ScreenDriverST7789_172x320x16_SPI_Async = TemplateScreenDriverSpiAsync<ScreenDriverST7789_172x320x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, spiChunkDivisor>;

#if defined(TEMPLATE_SCREEN_DRIVER_SPI_DMA)
	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = 4000000,
		const uint32_t pushSleepDuration = 0>
	using ScreenDriverST7789_240x240x16_SPI_Dma = TemplateScreenDriverSpiDma<ScreenDriverST7789_240x240x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration, ST7789::SpiMaxChunkSize>;

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = 4000000,
		const uint32_t pushSleepDuration = 0>
	using ScreenDriverST7789_172x320x16_SPI_Dma = TemplateScreenDriverSpiDma<ScreenDriverST7789_172x320x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration, ST7789::SpiMaxChunkSize>;
#endif

#if defined(TEMPLATE_SCREEN_DRIVER_RTOS)
	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = 4000000,
		const uint32_t pushSleepDuration = 0,
		const uint32_t stackHeight = 1500,
		const UBaseType_t priority = 1
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		, const uint32_t coreAffinity = tskNO_AFFINITY
#endif
	>
	using ScreenDriverST7789_240x240x16_SPI_Rtos =
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		TemplateScreenDriverRtos<Egfx::SpiType,
		ScreenDriverST7789_240x240x16_SPI<pinCS, pinDC, pinRST, spiSpeed>,
		pushSleepDuration,
		stackHeight,
		priority,
		coreAffinity>;
#else
		TemplateScreenDriverRtos<Egfx::SpiType,
		ScreenDriverST7789_240x240x16_SPI<pinCS, pinDC, pinRST, spiSpeed>,
		pushSleepDuration,
		stackHeight,
		priority>;
#endif

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = 4000000,
		const uint32_t pushSleepDuration = 0,
		const uint32_t stackHeight = 1500,
		const UBaseType_t priority = 1
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		, const uint32_t coreAffinity = tskNO_AFFINITY
#endif
	>
	using ScreenDriverST7789_172x320x16_SPI_Rtos =
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		TemplateScreenDriverRtos<Egfx::SpiType,
		ScreenDriverST7789_172x320x16_SPI<pinCS, pinDC, pinRST, spiSpeed>,
		pushSleepDuration,
		stackHeight,
		priority,
		coreAffinity>;
#else
		TemplateScreenDriverRtos<Egfx::SpiType,
		ScreenDriverST7789_172x320x16_SPI<pinCS, pinDC, pinRST, spiSpeed>,
		pushSleepDuration,
		stackHeight,
		priority>;
#endif
#endif
}
#endif