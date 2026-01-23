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
		const uint8_t pageStartOffset,
		const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed>
	class AbstractScreenDriverSH1106_SPI : public AbstractScreenDriverSPI<Egfx::GetFrameBufferMonochromeSize(width, height), width, height, pinCS, pinDC, pinRST>
	{
	private:
		using BaseClass = AbstractScreenDriverSPI<Egfx::GetFrameBufferMonochromeSize(width, height), width, height, pinCS, pinDC, pinRST>;

	protected:
		static constexpr uint8_t Pages = (height + 7) / 8;
		static constexpr uint8_t PageChunkSize = width;

		static constexpr uint8_t PushPage1 = (uint8_t)SH1106::CommandEnum::PushAddress + (pageStartOffset >> 4);
		static constexpr uint8_t PushPage2 = pageStartOffset & 0x0F;

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

		~AbstractScreenDriverSH1106_SPI() override = default;

	public:
		virtual void StartBuffer()
		{
			// No-op, SH1106 pushes page by page.
		}

		virtual uint32_t PushBuffer(const uint8_t* frameBuffer)
		{
			for (uint_fast8_t p = 0; p < Pages; p++)
			{
				CommandStart(Settings);
				SpiInstance.transfer((uint8_t)SH1106::CommandEnum::PushPage + p);
				SpiInstance.transfer(PushPage1);
				SpiInstance.transfer(PushPage2);

				digitalWrite(pinDC, HIGH);
				SpiTransfer(&frameBuffer[(uint_least16_t)p * PageChunkSize], PageChunkSize);
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
	class ScreenDriverSH1106_128x64x1_SPI
		: public AbstractScreenDriverSH1106_SPI<SH1106_128x64::Width, SH1106_128x64::Height, SH1106_128x64::PageStartOffset, pinCS, pinDC, pinRST, spiSpeed>
	{
	private:
		using BaseClass = AbstractScreenDriverSH1106_SPI<SH1106_128x64::Width, SH1106_128x64::Height, SH1106_128x64::PageStartOffset, pinCS, pinDC, pinRST, spiSpeed>;

	public:
		ScreenDriverSH1106_128x64x1_SPI(Egfx::SpiType& spi) : BaseClass(spi) {}

		~ScreenDriverSH1106_128x64x1_SPI() override = default;

		virtual void StartBuffer()
		{
		}

		virtual bool Start()
		{
			return BaseClass::Start() && BaseClass::Initialize();
		}
	};

	template<const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed = SH1106::SpiMaxSpeed>
	class ScreenDriverSH1106_132x64x1_SPI
		: public AbstractScreenDriverSH1106_SPI<SH1106_132x64::Width, SH1106_132x64::Height, SH1106_132x64::PageStartOffset, pinCS, pinDC, pinRST, spiSpeed>
	{
	private:
		using BaseClass = AbstractScreenDriverSH1106_SPI<SH1106_132x64::Width, SH1106_132x64::Height, SH1106_132x64::PageStartOffset, pinCS, pinDC, pinRST, spiSpeed>;

	public:
		ScreenDriverSH1106_132x64x1_SPI(Egfx::SpiType& spi) : BaseClass(spi) {}

		~ScreenDriverSH1106_132x64x1_SPI() = default;

		virtual void StartBuffer()
		{
		}

		virtual bool Start()
		{
			return BaseClass::Start() && BaseClass::Initialize();
		}
	};

	/// <summary>
	/// SH1106 buffer pushing is already natively async, transfering blocks at a time.
	/// </summary>
	template<const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed = SH1106::SpiMaxSpeed>
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

		~ScreenDriverSH1106_128x64x1_SPI_Async() override = default;

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

	template<const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed = SH1106::SpiMaxSpeed>
	class ScreenDriverSH1106_132x64x1_SPI_Async : public ScreenDriverSH1106_132x64x1_SPI<pinCS, pinDC, pinRST, spiSpeed>
	{
	private:
		using BaseClass = ScreenDriverSH1106_132x64x1_SPI<pinCS, pinDC, pinRST, spiSpeed>;

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
		ScreenDriverSH1106_132x64x1_SPI_Async(Egfx::SpiType& spi) : BaseClass(spi) {}

		~ScreenDriverSH1106_132x64x1_SPI_Async() override = default;

		uint32_t PushBuffer(const uint8_t* frameBuffer) final
		{
			PushIndex = 0;

			CommandStart(Settings);
			SpiInstance.transfer((uint8_t)SH1106::CommandEnum::PushPage);
			SpiInstance.transfer((uint8_t)SH1106_132x64::CommandEnum::PushPage1);
			SpiInstance.transfer((uint8_t)SH1106_132x64::CommandEnum::PushPage2);

			digitalWrite(pinDC, HIGH);
			SpiTransfer(&frameBuffer[0], SH1106_132x64::PageChunkSize);
			CommandEnd();
			PushIndex++;

			return 0;
		}

		bool PushingBuffer(const uint8_t* frameBuffer) final
		{
			CommandStart(Settings);
			SpiInstance.transfer((uint8_t)SH1106::CommandEnum::PushPage + PushIndex);
			SpiInstance.transfer((uint8_t)SH1106_132x64::CommandEnum::PushPage1);
			SpiInstance.transfer((uint8_t)SH1106_132x64::CommandEnum::PushPage2);

			digitalWrite(pinDC, HIGH);
			SpiTransfer(&frameBuffer[(uint_least16_t)PushIndex * SH1106_132x64::PageChunkSize], SH1106_132x64::PageChunkSize);
			CommandEnd();
			PushIndex++;

			return PushIndex < SH1106_132x64::Pages;
		}
	};

#if defined(TEMPLATE_SCREEN_DRIVER_SPI_DMA)
	template<const pixel_t width,
		const pixel_t height,
		const uint8_t pageStartOffset,
		const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed = SH1106::SpiMaxSpeed,
		const uint32_t pushSleepDuration = 0>
	class AbstractScreenDriverSH1106_SPI_DMA
		: public AbstractScreenDriverSH1106_SPI<width, height, pageStartOffset, pinCS, pinDC, pinRST, spiSpeed>
	{
	private:
		using BaseClass = AbstractScreenDriverSH1106_SPI<width, height, pageStartOffset, pinCS, pinDC, pinRST, spiSpeed>;

		using BaseClass::Pages;
		using BaseClass::PageChunkSize;
		using BaseClass::PushPage1;
		using BaseClass::PushPage2;
		using BaseClass::Settings;
		using BaseClass::SpiInstance;
		using BaseClass::CommandStart;
		using BaseClass::CommandEnd;

		volatile bool _pushInProgress = false;
		uint8_t PushIndex = 0;
		const uint8_t* FrameBuffer = nullptr;

	public:
		AbstractScreenDriverSH1106_SPI_DMA(Egfx::SpiType& spi)
			: BaseClass(spi)
		{
		}

		~AbstractScreenDriverSH1106_SPI_DMA()
		{
			// Ensure any ongoing DMA is finished
			while (_pushInProgress)
			{
				PushingBuffer(FrameBuffer);
			}
		}

		bool Start() override
		{
			return BaseClass::Start() && BaseClass::Initialize();
		}

		uint32_t PushBuffer(const uint8_t* frameBuffer) final
		{
			FrameBuffer = frameBuffer;
			PushIndex = 0;
			_pushInProgress = true;

			StartPageDma(PushIndex);
			return pushSleepDuration;
		}

		bool PushingBuffer(const uint8_t* /*frameBuffer*/) final
		{
#if defined(ARDUINO_ARCH_STM32F1)
			const bool busy = (!spi_is_tx_empty(SpiInstance.dev()) || spi_is_busy(SpiInstance.dev()));
			if (busy) return true;
#elif defined(ARDUINO_ARCH_STM32F4)
			if (!SpiInstance.dmaSendReady()) return true;
#elif defined(ARDUINO_ARCH_RP2040)
			if (!SpiInstance.finishedAsync()) return true;
#elif defined(ARDUINO_ARCH_ESP32)
			if (SpiInstance.DmaBusy()) return true;
#else
			_pushInProgress = false;
			return false;
#endif

			// current page DMA finished
			if ((PushIndex + 1) >= Pages)
			{
				_pushInProgress = false;
				return false;
			}

			PushIndex++;
			StartPageDma(PushIndex);
			return true;
		}

	private:
		void StartPageDma(const uint8_t page)
		{
			// Per-page header (same as inline driver)
			CommandStart(Settings);
			SpiInstance.transfer((uint8_t)SH1106::CommandEnum::PushPage + page);
			SpiInstance.transfer(PushPage1);
			SpiInstance.transfer(PushPage2);

			digitalWrite(pinDC, HIGH);

			const uint8_t* pagePtr = &FrameBuffer[(uint_least16_t)page * PageChunkSize];

#if defined(ARDUINO_ARCH_STM32F1)
			SpiInstance.dmaSendAsync((void*)pagePtr, (size_t)PageChunkSize, true);
#elif defined(ARDUINO_ARCH_STM32F4)
			SpiInstance.dmaSend((void*)pagePtr, (uint16_t)PageChunkSize, true);
#elif defined(ARDUINO_ARCH_RP2040)
			SpiInstance.transferAsync((const void*)pagePtr, (void*)nullptr, PageChunkSize);
#elif defined(ARDUINO_ARCH_ESP32)
			SpiInstance.BeginConfigured();
			if (!SpiInstance.DmaBegin(80000000, SPI_MODE0, false))
			{
				_pushInProgress = false;
				CommandEnd();
				return;
			}
			if (!SpiInstance.DmaWriteAsync(pagePtr, PageChunkSize))
			{
				_pushInProgress = false;
				CommandEnd();
				return;
			}
#else
			_pushInProgress = false;
#endif
		}
	};


	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = SH1106::SpiMaxSpeed,
		const uint32_t pushSleepDuration = 0>
	using ScreenDriverSH1106_128x64x1_SPI_Dma =
		AbstractScreenDriverSH1106_SPI_DMA<SH1106_128x64::Width, SH1106_128x64::Height, SH1106_128x64::PageStartOffset,
		pinCS, pinDC, pinRST, spiSpeed, pushSleepDuration>;

	template<const uint8_t pinCS = UINT8_MAX,
		const uint8_t pinDC = UINT8_MAX,
		const uint8_t pinRST = UINT8_MAX,
		const uint32_t spiSpeed = SH1106::SpiMaxSpeed,
		const uint32_t pushSleepDuration = 0>
	using ScreenDriverSH1106_132x64x1_SPI_Dma =
		AbstractScreenDriverSH1106_SPI_DMA<SH1106_132x64::Width, SH1106_132x64::Height, SH1106_132x64::PageStartOffset,
		pinCS, pinDC, pinRST, spiSpeed, pushSleepDuration>;

#endif

#if defined(TEMPLATE_SCREEN_DRIVER_RTOS)
	template<const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed = SH1106::SpiMaxSpeed,
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

	template<const uint8_t pinCS,
		const uint8_t pinDC,
		const uint8_t pinRST,
		const uint32_t spiSpeed = SH1106::SpiMaxSpeed,
		const uint32_t pushSleepDuration = 0,
		uint32_t stackHeight = 1500,
		portBASE_TYPE priority = 1
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		, const uint32_t coreAffinity = tskNO_AFFINITY
#endif
	>
	using ScreenDriverSH1106_132x64x1_SPI_Rtos = TemplateScreenDriverRtos<Egfx::SpiType, ScreenDriverSH1106_132x64x1_SPI<pinCS, pinDC, pinRST, spiSpeed>, pushSleepDuration, stackHeight, priority
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		, coreAffinity
#endif
	>;
#endif
}
#endif