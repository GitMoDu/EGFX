// TemplateScreenDriverSpiDma.h

#ifndef _TEMPLATE_SCREEN_DRIVER_SPI_DMA_h
#define _TEMPLATE_SCREEN_DRIVER_SPI_DMA_h

#if defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_RP2040)
#define TEMPLATE_SCREEN_DRIVER_SPI_DMA

#include <stdint.h>

template<typename InlineSpiScreenDriver,
	const uint32_t pushSleepDuration = 0>
class TemplateScreenDriverSpiDma : public InlineSpiScreenDriver
{
public:
	using InlineSpiScreenDriver::BufferSize;

protected:
	using InlineSpiScreenDriver::SpiInstance;

public:
	TemplateScreenDriverSpiDma() : InlineSpiScreenDriver() {}

	virtual const uint32_t PushBuffer(const uint8_t* frameBuffer) final
	{
#if defined(ARDUINO_ARCH_STM32F1)
		SpiInstance.dmaSendAsync((void*)frameBuffer, BufferSize, true);
#elif defined(ARDUINO_ARCH_RP2040)
		SpiInstance.transferAsync((const void*)frameBuffer, (void*)nullptr, BufferSize);
#endif

		return pushSleepDuration;
	}

	virtual const bool PushingBuffer(const uint8_t* frameBuffer) final
	{
#if defined(ARDUINO_ARCH_STM32F1)
		return !spi_is_tx_empty(SpiInstance.dev())
			|| spi_is_busy(SpiInstance.dev());
#elif defined(ARDUINO_ARCH_RP2040)
		return !SpiInstance.finishedAsync();
#endif
	}
};
#endif
#endif