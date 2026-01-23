#ifndef _EGFX_SCREEN_DRIVERS_GC9A01_SPI_h
#define _EGFX_SCREEN_DRIVERS_GC9A01_SPI_h

#include "AbstractScreenDriverSPI.h"
#include "TemplateScreenDriverRtos.h"
#include "TemplateScreenDriverSpiAsync.h"
#include "TemplateScreenDriverSpiDma.h"
#include "GC9A01/GC9A01.h"

namespace Egfx
{
	template<typename pixel_color_t,
		const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed>
	class AbstractScreenDriverGC9A01_SPI : public AbstractScreenDriverSPI<Egfx::GetFrameBufferSize<pixel_color_t>(GC9A01::Width, GC9A01::Height), GC9A01::Width, GC9A01::Height, pinCS, pinDC, pinRST>
	{
	private:
		using BaseClass = AbstractScreenDriverSPI<Egfx::GetFrameBufferSize<pixel_color_t>(GC9A01::Width, GC9A01::Height), GC9A01::Width, GC9A01::Height, pinCS, pinDC, pinRST>;

		SPISettings Settings;

	public:
		AbstractScreenDriverGC9A01_SPI(Egfx::SpiType& spi)
			: BaseClass(spi)
			, Settings(GC9A01::LimitedSpiSpeed(spiSpeed), MSBFIRST, SPI_MODE0)
		{
		}

		~AbstractScreenDriverGC9A01_SPI() override = default;

		bool Start() override
		{
			if (!BaseClass::Start())
				return false;

			BaseClass::PinReset(GC9A01::RESET_DELAY_MICROS);

			return Initialize();
		}

		void StartBuffer() override
		{
			SetAddressWindow(0, 0, GC9A01::Width - 1, GC9A01::Height - 1);

			BaseClass::CommandStart(Settings);
			BaseClass::SpiInstance.transfer((uint8_t)GC9A01::CommandEnum::MemoryWrite);
			digitalWrite(pinDC, HIGH);
		}

		void EndBuffer() override
		{
			BaseClass::SpiInstance.transfer((uint8_t)GC9A01::CommandEnum::Nop);
			BaseClass::EndBuffer();
		}

	private:
		bool Initialize()
		{
			SendRaw(GC9A01::CommandEnum::SoftwareReset);
			DelayMicros(20000);
			SendRaw(GC9A01::CommandEnum::SleepOut);
			DelayMicros(GC9A01::SLEEP_EXIT_DELAY_MICROS);

			SendInit(GC9A01::Init::Sequence, sizeof(GC9A01::Init::Sequence) / sizeof(GC9A01::Init::Sequence[0]));

			{
				const uint8_t d = 0x08;
				SendRaw(GC9A01::CommandEnum::MemoryAccessCtrl, &d, 1);
			}
			{
				const uint8_t d = GC9A01::PixelFormats::Rgb565;
				SendRaw(GC9A01::CommandEnum::PixelFormatSet, &d, 1);
			}

			SetAddressWindow(0, 0, GC9A01::Width - 1, GC9A01::Height - 1);

			SendRaw((uint8_t)0x21);
			SendRaw(GC9A01::CommandEnum::NormalDisplayOn);
			DelayMicros(GC9A01::DISPLAY_ON_DELAY_MICROS);
			SendRaw(GC9A01::CommandEnum::DisplayOn);
			DelayMicros(GC9A01::DISPLAY_ON_DELAY_MICROS);

			return true;
		}

		void SendInit(const GC9A01::Init::RawCommand* seq, const size_t count)
		{
			for (size_t i = 0; i < count; i++)
			{
				SendRaw(seq[i].Command, seq[i].Data, seq[i].Length);
			}
		}

		void SendRaw(const GC9A01::CommandEnum cmd, const uint8_t* data = nullptr, const size_t len = 0)
		{
			SendRaw((uint8_t)cmd, data, len);
		}

		void SendRaw(const uint8_t cmd, const uint8_t* data = nullptr, const size_t len = 0)
		{
			BaseClass::CommandStart(Settings);
			BaseClass::SpiInstance.transfer(cmd);

			if (len && data)
			{
				digitalWrite(pinDC, HIGH);
				BaseClass::SpiTransfer(data, len);
				digitalWrite(pinDC, LOW);
			}

			BaseClass::CommandEnd();
		}

		void SetAddressWindow(const uint16_t x0, const uint16_t y0, const uint16_t x1, const uint16_t y1)
		{
			const uint8_t caset[]{
				uint8_t(x0 >> 8), uint8_t(x0 & 0xFF),
				uint8_t(x1 >> 8), uint8_t(x1 & 0xFF)
			};
			SendRaw((uint8_t)GC9A01::CommandEnum::ColumnAddressSet, caset, sizeof(caset));

			const uint8_t raset[]{
				uint8_t(y0 >> 8), uint8_t(y0 & 0xFF),
				uint8_t(y1 >> 8), uint8_t(y1 & 0xFF)
			};
			SendRaw((uint8_t)GC9A01::CommandEnum::RowAddressSet, raset, sizeof(raset));
		}

		static void DelayMicros(const uint32_t us)
		{
			if (!us) return;
			delay(us / 1000);
			delayMicroseconds(us % 1000);
		}
	};

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = GC9A01::SpiDefaultSpeed>
	using ScreenDriverGC9A01_240x240x16_SPI = AbstractScreenDriverGC9A01_SPI<uint16_t, pinCS, pinDC, pinRST, spiSpeed>;

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = GC9A01::SpiDefaultSpeed,
		const uint8_t spiChunkDivisor = 2>
	using ScreenDriverGC9A01_240x240x16_SPI_Async = TemplateScreenDriverSpiAsync<ScreenDriverGC9A01_240x240x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, spiChunkDivisor>;

#if defined(TEMPLATE_SCREEN_DRIVER_SPI_DMA)
	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = GC9A01::SpiDefaultSpeed,
		const uint32_t pushSleepDuration = 0>
	using ScreenDriverGC9A01_240x240x16_SPI_Dma = TemplateScreenDriverSpiDma<ScreenDriverGC9A01_240x240x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration, GC9A01::SpiMaxChunkSize>;
#endif

#if defined(TEMPLATE_SCREEN_DRIVER_RTOS)
	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = GC9A01::SpiDefaultSpeed,
		const uint32_t pushSleepDuration = 0,
		uint32_t stackHeight = 1500,
		const UBaseType_t priority = 1
	>
	using ScreenDriverGC9A01_240x240x16_SPI_Rtos = TemplateScreenDriverRtos<Egfx::SpiType, ScreenDriverGC9A01_240x240x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration, stackHeight, priority>;
#endif
}
#endif