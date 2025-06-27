#ifndef _SCREEN_DRIVER_SH1106_SPI_h
#define _SCREEN_DRIVER_SH1106_SPI_h

#include "AbstractScreenDriverSPI.h"
#include "TemplateScreenDriverRtos.h"
#include "TemplateScreenDriverSpiAsync.h"
#include "TemplateScreenDriverSpiDma.h"
#include "SH1106/SH1106.h"

namespace Egfx
{
	template<const pixel_t width,
		const pixel_t height,
		const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed>
	class AbstractScreenDriverSH1106_SPI : public AbstractScreenDriverSPI<Egfx::GetFrameBufferMonochromeSize(width, height), width, height, pinCS, pinDC, pinRST>
	{
	private:
		using BaseClass = AbstractScreenDriverSPI<Egfx::GetFrameBufferMonochromeSize(width, height), width, height, pinCS, pinDC, pinRST>;

	public:
		using BaseClass::ScreenWidth;
		using BaseClass::ScreenHeight;
		using BaseClass::BufferSize;

	protected:
		using BaseClass::SpiInstance;
		using BaseClass::SpiTransfer;
		using BaseClass::CommandStart;
		using BaseClass::CommandEnd;
		using BaseClass::PinReset;

	protected:
		SPISettings Settings;

	public:
		AbstractScreenDriverSH1106_SPI(Egfx::SpiType& spi)
			: BaseClass(spi)
			, Settings(spiSpeed, MSBFIRST, SPI_MODE0)
		{
		}

	public:
		virtual uint32_t PushBuffer(const uint8_t* frameBuffer)
		{
			for (uint_fast8_t p = 0; p < SH1106_128x64::Pages; p++)
			{
				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)SH1106::CommandEnum::PushPage + p);
				SpiInstance.transfer((uint8_t)SH1106_128x64::CommandEnum::PushPage1);
				SpiInstance.transfer((uint8_t)SH1106_128x64::CommandEnum::PushPage2);

				digitalWrite(pinDC, HIGH);
				SpiTransfer(&frameBuffer[(uint_least16_t)p * SH1106_128x64::PageChunkSize], SH1106_128x64::PageChunkSize);
				CommandEnd();
			}

			return 0;
		}

	protected:
		bool Initialize()
		{
			PinReset(SH1106::RESET_WAIT_MICROS);
			delayMicroseconds(SH1106::RESET_DELAY_MICROS);

			CommandStart(Settings);
			SpiTransfer(SH1106::ConfigBatch, sizeof(SH1106::ConfigBatch));
			CommandEnd();

			delayMicroseconds(SH1106::BOOT_DELAY_MICROS);

			CommandStart(Settings);
			SpiInstance.transfer((uint8_t)SH1106::CommandEnum::CommandStart);
			SpiInstance.transfer((uint8_t)SH1106::CommandEnum::DisplayOn);
			CommandEnd();

			return true;
		}

	private:
		void CommandReset()
		{
			CommandStart(Settings);
			SpiInstance.transfer((uint8_t)SH1106::CommandEnum::CommandStart);
			SpiInstance.transfer((uint8_t)SH1106::CommandEnum::Reset);
			CommandEnd();
		}
	};

	template<const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed = 4000000>
	class ScreenDriverSH1106_128x64x1_SPI : public AbstractScreenDriverSH1106_SPI<SH1106_128x64::Width, SH1106_128x64::Height, pinCS, pinDC, pinRST, spiSpeed>
	{
	private:
		using BaseClass = AbstractScreenDriverSH1106_SPI<SH1106_128x64::Width, SH1106_128x64::Height, pinCS, pinDC, pinRST, spiSpeed>;

	public:
		ScreenDriverSH1106_128x64x1_SPI(Egfx::SpiType& spi) : BaseClass(spi) {}

		virtual bool Start()
		{
			return BaseClass::Start() && BaseClass::Initialize();
		}
	};

	/// <summary>
	/// SH1106 buffer pushing is already natively async, transfering blocks at a time.
	/// </summary>
	/// <typeparam name="pinCS"></typeparam>
	/// <typeparam name="pinDC"></typeparam>
	/// <typeparam name="pinRST"></typeparam>
	/// <typeparam name="spiSpeed"></typeparam>
	template<const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed = 4000000>
	class ScreenDriverSH1106_128x64x1_SPI_Async : public ScreenDriverSH1106_128x64x1_SPI<pinCS, pinDC, pinRST, spiSpeed>
	{
	private:
		using BaseClass = ScreenDriverSH1106_128x64x1_SPI<pinCS, pinDC, pinRST, spiSpeed>;

	protected:
		using BaseClass::Settings;
		using BaseClass::SpiInstance;
		using BaseClass::SpiTransfer;
		using BaseClass::CommandStart;
		using BaseClass::CommandEnd;
		using BaseClass::PinReset;

	private:
		uint8_t PushIndex = 0;

	public:
		ScreenDriverSH1106_128x64x1_SPI_Async(Egfx::SpiType& spi) : BaseClass(spi) {}

		uint32_t PushBuffer(const uint8_t* frameBuffer) final
		{
			PushIndex = 0;

			CommandStart(Settings);
			SpiInstance.transfer((uint8_t)SH1106::CommandEnum::PushPage);
			SpiInstance.transfer((uint8_t)SH1106_128x64::CommandEnum::PushPage1);
			SpiInstance.transfer((uint8_t)SH1106_128x64::CommandEnum::PushPage2);

			digitalWrite(pinDC, HIGH);
			SpiTransfer(&frameBuffer[0], SH1106_128x64::PageChunkSize);
			CommandEnd();
			PushIndex++;

			return 0;
		}

		bool PushingBuffer(const uint8_t* frameBuffer) final
		{
			CommandStart(Settings);
			SpiInstance.transfer((uint8_t)SH1106::CommandEnum::PushPage + PushIndex);
			SpiInstance.transfer((uint8_t)SH1106_128x64::CommandEnum::PushPage1);
			SpiInstance.transfer((uint8_t)SH1106_128x64::CommandEnum::PushPage2);

			digitalWrite(pinDC, HIGH);
			SpiTransfer(&frameBuffer[(uint_least16_t)PushIndex * SH1106_128x64::PageChunkSize], SH1106_128x64::PageChunkSize);
			CommandEnd();
			PushIndex++;

			return PushIndex < SH1106_128x64::Pages;
		}
	};

#if defined(TEMPLATE_SCREEN_DRIVER_SPI_DMA)
	/// <summary>
	/// SH1106 buffer pushing is natively async, 
	/// requiring a header push before data push.
	/// This DMA implementation pushes the header directly and 
	/// DMA pushes the data, waiting for DMA complete before moving to the next block.
	/// </summary>
	/// <typeparam name="pinCS"></typeparam>
	/// <typeparam name="pinDC"></typeparam>
	/// <typeparam name="pinRST"></typeparam>
	/// <typeparam name="spiSpeed"></typeparam>
	/// <typeparam name="pushSleepDuration"></typeparam>
	template<const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed = 4000000,
		const uint32_t pushSleepDuration = 0>
	class ScreenDriverSH1106_128x64x1_SPI_Dma : public ScreenDriverSH1106_128x64x1_SPI<pinCS, pinDC, pinRST, spiSpeed>
	{
	private:
		using BaseClass = ScreenDriverSH1106_128x64x1_SPI<pinCS, pinDC, pinRST, spiSpeed>;

	protected:
		using BaseClass::Settings;
		using BaseClass::SpiInstance;
		using BaseClass::SpiTransfer;
		using BaseClass::CommandStart;
		using BaseClass::CommandEnd;
		using BaseClass::PinReset;

	private:
		uint8_t PushIndex = 0;

	public:
		ScreenDriverSH1106_128x64x1_SPI_Dma(Egfx::SpiType& spi) : BaseClass(spi) {}

		uint32_t PushBuffer(const uint8_t* frameBuffer) final
		{
			PushIndex = 0;

			CommandStart(Settings);
			SpiInstance.transfer((uint8_t)SH1106::CommandEnum::PushPage);
			SpiInstance.transfer((uint8_t)SH1106_128x64::CommandEnum::PushPage1);
			SpiInstance.transfer((uint8_t)SH1106_128x64::CommandEnum::PushPage2);

			digitalWrite(pinDC, HIGH);
#if defined(ARDUINO_ARCH_STM32F1)
			SpiInstance.dmaSendAsync((void*)&frameBuffer[0], (uint16_t)SH1106_128x64::PageChunkSize, true);
#elif defined(ARDUINO_ARCH_RP2040)
			SpiInstance.transferAsync((const void*)&frameBuffer[0], (void*)nullptr, SH1106_128x64::PageChunkSize);
#endif
			PushIndex++;

			return pushSleepDuration;
		}

		bool PushingBuffer(const uint8_t* frameBuffer) final
		{
#if defined(ARDUINO_ARCH_STM32F1)
			const bool dmaBusy = !spi_is_tx_empty(SpiInstance.dev())
				|| spi_is_busy(SpiInstance.dev());
#elif defined(ARDUINO_ARCH_RP2040)
			const bool dmaBusy = !SpiInstance.finishedAsync();
#endif
			if (!dmaBusy)
			{
				CommandEnd();
				if (PushIndex < SH1106_128x64::Pages)
				{
					CommandStart(Settings);
					SpiInstance.transfer((uint8_t)SH1106::CommandEnum::PushPage + PushIndex);
					SpiInstance.transfer((uint8_t)SH1106_128x64::CommandEnum::PushPage1);
					SpiInstance.transfer((uint8_t)SH1106_128x64::CommandEnum::PushPage2);

					digitalWrite(pinDC, HIGH);
#if defined(ARDUINO_ARCH_STM32F1)
					SpiInstance.dmaSendAsync((void*)&frameBuffer[(uint_least16_t)PushIndex * SH1106_128x64::PageChunkSize], (uint16_t)SH1106_128x64::PageChunkSize, true);
#elif defined(ARDUINO_ARCH_RP2040)
					SpiInstance.transferAsync((const void*)&frameBuffer[(uint_least16_t)PushIndex * SH1106_128x64::PageChunkSize], (void*)nullptr, SH1106_128x64::PageChunkSize);
#endif
					PushIndex++;

					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
	};
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
	using ScreenDriverSH1106_128x64x1_SPI_Rtos = TemplateScreenDriverRtos<Egfx::SpiType, ScreenDriverSH1106_128x64x1_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration, stackHeight, priority
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		, coreAffinity
#endif
	>;
#endif
}
#endif