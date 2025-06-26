// ScreenDriverSSD1351_SPI.h

#ifndef _SCREEN_DRIVER_SSD1351_SPI_h
#define _SCREEN_DRIVER_SSD1351_SPI_h

#include "AbstractScreenDriverSPI.h"
#include "TemplateScreenDriverRtos.h"
#include "TemplateScreenDriverSpiAsync.h"
#include "TemplateScreenDriverSpiDma.h"
#include "SSD1351/SSD1351.h"

namespace Egfx
{
	template<typename pixel_color_t,
		const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed>
	class AbstractScreenDriverSSD1351_SPI : public AbstractScreenDriverSPI<Egfx::GetFrameBufferSize<pixel_color_t>(SSD1351::Width, SSD1351::Height), SSD1351::Width, SSD1351::Height, pinCS, pinDC, pinRST>
	{
	private:
		using BaseClass = AbstractScreenDriverSPI<Egfx::GetFrameBufferSize<pixel_color_t>(SSD1351::Width, SSD1351::Height), SSD1351::Width, SSD1351::Height, pinCS, pinDC, pinRST>;

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
		AbstractScreenDriverSSD1351_SPI(Egfx::SpiType& spi)
			: BaseClass(spi)
			, Settings(SSD1351::LimitedSpiSpeed(spiSpeed), MSBFIRST, SPI_MODE0)
		{
		}

	public:
		virtual void EndBuffer()
		{
			digitalWrite(pinDC, LOW);
			SpiInstance.transfer((uint8_t)SSD1351::CommandEnum::Nop1);
			BaseClass::EndBuffer();
		}

		virtual const bool Start()
		{
			if (BaseClass::Start())
			{
				PinReset(SSD1351::RESET_WAIT_MICROS);
				delayMicroseconds(SSD1351::RESET_DELAY_MICROS);

				CommandStart(Settings);

				SpiInstance.transfer((uint8_t)SSD1351::CommandEnum::Unlock);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0x12);
				digitalWrite(pinDC, LOW);
				SpiInstance.transfer((uint8_t)SSD1351::CommandEnum::Unlock);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0xB1);
				digitalWrite(pinDC, LOW);

				SpiInstance.transfer((uint8_t)SSD1351::CommandEnum::SetSleepOn);

				ExecuteCommonBatch();

				SpiInstance.transfer((uint8_t)SSD1351::CommandEnum::SetDisplayModeNormal);

				SpiInstance.transfer((uint8_t)SSD1351::CommandEnum::SetContrastA);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0xC8);
				SpiInstance.transfer(0x80);
				SpiInstance.transfer(0xC8);
				digitalWrite(pinDC, LOW);

				SpiInstance.transfer((uint8_t)SSD1351::CommandEnum::SetMasterCurrentAF);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0x0F);
				digitalWrite(pinDC, LOW);

				SpiInstance.transfer((uint8_t)SSD1351::CommandEnum::SetSegmentLowVoltage);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0xA0);
				SpiInstance.transfer(0xB5);
				SpiInstance.transfer(0x55);
				digitalWrite(pinDC, LOW);

				SpiInstance.transfer((uint8_t)SSD1351::CommandEnum::SetPrechargeSpeed2);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0x01);
				digitalWrite(pinDC, LOW);

				SpiInstance.transfer((uint8_t)SSD1351::CommandEnum::SetSleepOff);
				SpiInstance.transfer((uint8_t)SSD1351::CommandEnum::SetDisplayModeNormal);

				SpiInstance.transfer((uint8_t)SSD1351::CommandEnum::SetColumnAddress);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0);
				SpiInstance.transfer(SSD1351::Width - 1);
				digitalWrite(pinDC, LOW);

				SpiInstance.transfer((uint8_t)SSD1351::CommandEnum::SetRowAddress);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(0);
				SpiInstance.transfer(SSD1351::Height - 1);
				CommandEnd();

				return true;
			}

			return false;
		}

		virtual void StartBuffer()
		{
			CommandStart(Settings);
			SpiInstance.transfer((uint8_t)SSD1351::CommandEnum::WriteData);
			digitalWrite(pinDC, HIGH);
		}

	private:
		void ExecuteCommonBatch()
		{
			size_t index = 0;
			while (index < sizeof(SSD1351::CommonCommandArgBatch))
			{
				SpiInstance.transfer(SSD1351::CommonCommandArgBatch[index++]);
				digitalWrite(pinDC, HIGH);
				SpiInstance.transfer(SSD1351::CommonCommandArgBatch[index++]);
				digitalWrite(pinDC, LOW);
			}
		}
	};

	template<const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed>
	using ScreenDriverSSD1351_128x128x16_SPI = AbstractScreenDriverSSD1351_SPI<uint16_t, pinCS, pinDC, pinRST, spiSpeed>;

	template<const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed,
		const uint8_t spiChunkDivisor = 2>
	using ScreenDriverSSD1351_128x128x16_SPI_Async = TemplateScreenDriverSpiAsync<ScreenDriverSSD1351_128x128x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, spiChunkDivisor>;

#if defined(TEMPLATE_SCREEN_DRIVER_SPI_DMA)
	template<const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed,
		const uint32_t pushSleepDuration = 0>
	using ScreenDriverSSD1351_128x128x16_SPI_Dma = TemplateScreenDriverSpiDma<ScreenDriverSSD1351_128x128x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration>;
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
	using ScreenDriverSSD1351_128x128x16_SPI_Rtos = TemplateScreenDriverRtos<Egfx::SpiType, ScreenDriverSSD1351_128x128x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration, stackHeight, priority
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		, coreAffinity
#endif
	>;
#endif
}
#endif