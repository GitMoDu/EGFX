#ifndef _EGFX_SCREEN_DRIVERS_GC9107_SPI_h
#define _EGFX_SCREEN_DRIVERS_GC9107_SPI_h

#include "AbstractScreenDriverSPI.h"
#include "TemplateScreenDriverRtos.h"
#include "TemplateScreenDriverSpiAsync.h"
#include "TemplateScreenDriverSpiDma.h"
#include "GC9107/GC9107.h"

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
	class AbstractScreenDriverGC9107_SPI : public AbstractScreenDriverSPI<Egfx::GetFrameBufferSize<pixel_color_t>(width, height), width, height, pinCS, pinDC, pinRST>
	{
	private:
		using BaseClass = AbstractScreenDriverSPI<Egfx::GetFrameBufferSize<pixel_color_t>(width, height), width, height, pinCS, pinDC, pinRST>;

		static_assert(width <= GC9107::Width, "GC9107 panel width exceeded.");
		static_assert(height <= GC9107::Height, "GC9107 panel height exceeded.");

	public:
		using BaseClass::BufferSize;
		using BaseClass::ScreenHeight;
		using BaseClass::ScreenWidth;

	protected:
		using BaseClass::CommandEnd;
		using BaseClass::CommandStart;
		using BaseClass::PinReset;
		using BaseClass::SpiInstance;
		using BaseClass::SpiTransfer;

	private:
		SPISettings Settings;

	public:
		AbstractScreenDriverGC9107_SPI(Egfx::SpiType& spi)
			: BaseClass(spi)
			, Settings(GC9107::LimitedSpiSpeed(spiSpeed), MSBFIRST, SPI_MODE0)
		{
		}

		bool Start() override
		{
			if (!BaseClass::Start())
			{
				return false;
			}

			PinReset(GC9107::RESET_DELAY_MICROS);

			return Initialize();
		}

		void StartBuffer() override
		{
			CommandStart(Settings);
			SpiInstance.transfer((uint8_t)GC9107::CommandEnum::MemoryWrite);
			digitalWrite(pinDC, HIGH);
		}

		void EndBuffer() override
		{
			SpiInstance.transfer((uint8_t)GC9107::CommandEnum::Nop);
			BaseClass::EndBuffer();
		}

	protected:
		virtual uint8_t GetMemoryAccessControlValue() const
		{
			// Align expected color order (BGR) with framebuffer color order (RGB).
			return GC9107::MemoryAccessControl::ColorOrderBgr;
		}

		virtual uint8_t GetPixelFormatValue() const
		{
			return GC9107::PixelFormats::Rgb565;
		}

	private:
		bool Initialize()
		{
			SoftwareResetAndWake();

			SendCommand(GC9107::CommandEnum::SetPixelFormat, GetPixelFormatValue());
			SendCommand(GC9107::CommandEnum::SetMemoryAccessControl, GetMemoryAccessControlValue());

			ConfigureAddressWindow();

			SendCommand(GC9107::CommandEnum::InversionOn);
			SendCommand(GC9107::CommandEnum::IdleOff);
			SendCommand(GC9107::CommandEnum::DisplayOn);
			DelayMicros(GC9107::DISPLAY_ON_DELAY_MICROS);

			return true;
		}

		void SoftwareResetAndWake()
		{
			SendCommand(GC9107::CommandEnum::SoftwareReset);
			DelayMicros(GC9107::SOFTWARE_RESET_DELAY_MICROS);

			SendCommand(GC9107::CommandEnum::SleepOut);
			DelayMicros(GC9107::SLEEP_EXIT_DELAY_MICROS);
		}

		void ConfigureAddressWindow()
		{
			const uint16_t columnStart = offsetX;
			const uint16_t columnEnd = static_cast<uint16_t>(offsetX + (uint16_t)width - 1);
			const uint8_t columnData[]
			{
				static_cast<uint8_t>(columnStart >> 8),
				static_cast<uint8_t>(columnStart & UINT8_MAX),
				static_cast<uint8_t>(columnEnd >> 8),
				static_cast<uint8_t>(columnEnd & UINT8_MAX)
			};
			SendCommand(GC9107::CommandEnum::SetColumnAddress, columnData, sizeof(columnData));

			const uint16_t rowStart = offsetY;
			const uint16_t rowEnd = static_cast<uint16_t>(offsetY + (uint16_t)height - 1);
			const uint8_t rowData[]
			{
				static_cast<uint8_t>(rowStart >> 8),
				static_cast<uint8_t>(rowStart & UINT8_MAX),
				static_cast<uint8_t>(rowEnd >> 8),
				static_cast<uint8_t>(rowEnd & UINT8_MAX)
			};
			SendCommand(GC9107::CommandEnum::SetRowAddress, rowData, sizeof(rowData));
		}

		void SendCommand(const GC9107::CommandEnum command)
		{
			SendCommand(command, nullptr, 0);
		}

		void SendCommand(const GC9107::CommandEnum command, const uint8_t value)
		{
			const uint8_t payload[]{ value };
			SendCommand(command, payload, sizeof(payload));
		}

		void SendCommand(const GC9107::CommandEnum command, const uint8_t* data, const size_t length)
		{
			CommandStart(Settings);
			SpiInstance.transfer(static_cast<uint8_t>(command));

			if (length > 0 && data != nullptr)
			{
				digitalWrite(pinDC, HIGH);
				SpiTransfer(data, length);
				digitalWrite(pinDC, LOW);
			}

			CommandEnd();
		}

		static void DelayMicros(const uint32_t micros)
		{
			if (micros == 0)
			{
				return;
			}

			const uint32_t milliseconds = micros / 1000UL;
			const uint32_t remainder = micros % 1000UL;

			if (milliseconds > 0)
			{
				delay(milliseconds);
			}

			if (remainder > 0)
			{
				delayMicroseconds(remainder);
			}
		}
	};

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = GC9107::SpiDefaultSpeed>
	using ScreenDriverGC9107_128x128x16_SPI = AbstractScreenDriverGC9107_SPI<uint16_t, GC9107::Width, GC9107::Height, 2, 1, pinCS, pinDC, pinRST, spiSpeed>;

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = GC9107::SpiDefaultSpeed,
		const uint8_t spiChunkDivisor = 2>
	using ScreenDriverGC9107_128x128x16_SPI_Async = TemplateScreenDriverSpiAsync<ScreenDriverGC9107_128x128x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, spiChunkDivisor>;

#if defined(TEMPLATE_SCREEN_DRIVER_SPI_DMA)
	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = GC9107::SpiDefaultSpeed,
		const uint32_t pushSleepDuration = 0>
	using ScreenDriverGC9107_128x128x16_SPI_Dma = TemplateScreenDriverSpiDma<ScreenDriverGC9107_128x128x16_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration, GC9107::SpiMaxChunkSize>;
#endif

#if defined(TEMPLATE_SCREEN_DRIVER_RTOS)
	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = GC9107::SpiDefaultSpeed,
		const uint32_t pushSleepDuration = 0,
		uint32_t stackHeight = 1500,
		portBASE_TYPE priority = 1
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		, const uint32_t coreAffinity = tskNO_AFFINITY
#endif
	>
	using ScreenDriverGC9107_128x128x16_SPI_Rtos =
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		TemplateScreenDriverRtos<Egfx::SpiType,
		ScreenDriverGC9107_128x128x16_SPI<pinCS, pinDC, pinRST, spiSpeed>,
		pushSleepDuration,
		stackHeight,
		priority,
		coreAffinity>;
#else
		TemplateScreenDriverRtos<Egfx::SpiType,
		ScreenDriverGC9107_128x128x16_SPI<pinCS, pinDC, pinRST, spiSpeed>,
		pushSleepDuration,
		stackHeight,
		priority>;
#endif
#endif
}
#endif