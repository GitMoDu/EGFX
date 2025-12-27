#ifndef _EGFX_PLATFORM_PRESETS_PINS_h
#define _EGFX_PLATFORM_PRESETS_PINS_h

#include <Arduino.h>

namespace Egfx
{
	namespace PlatformPresets
	{
		// Presets of pin definitions for various platforms.
		namespace Pins
		{
#if defined(ARDUINO_ARCH_AVR)
			struct AVR_SPI_GENERIC
			{
				static constexpr uint8_t CS = 10;
				static constexpr uint8_t DC = 9;
				static constexpr uint8_t RESET = 8;
				static constexpr uint8_t BACKLIGHT = UINT8_MAX;
			};
#endif

#if defined(ARDUINO_ARCH_RP2040)
			struct RP2350_SPI_GENERIC
			{
				static constexpr uint8_t CS = 19;
				static constexpr uint8_t DC = 20;
				static constexpr uint8_t RESET = 21;
				static constexpr uint8_t BACKLIGHT = UINT8_MAX;
			};

			struct RP2350_SPI_LCD_1_47_INCH
			{
				static constexpr uint8_t CS = 17;
				static constexpr uint8_t DC = 16;
				static constexpr uint8_t RESET = 20;
				static constexpr uint8_t BACKLIGHT = 21;
			};

			struct RP2350_SPI_PICO_LCD_P1_3_INCH
			{
				static constexpr uint8_t CS = 9;
				static constexpr uint8_t DC = 8;	
				static constexpr uint8_t RESET = 12;
				static constexpr uint8_t BACKLIGHT = 13;
			};
#endif

#if defined(ARDUINO_ARCH_ESP32)
			struct ESP32_SPI_GENERIC
			{
				static constexpr uint8_t CS = 5;
				static constexpr uint8_t DC = 16;
				static constexpr uint8_t RESET = 23;
				static constexpr uint8_t BACKLIGHT = UINT8_MAX;
			};

#endif
#if defined(ARDUINO_SEEED_XIAO_RP2350)
			struct RP2350_XIAO_SPI_GENERIC
			{
				static constexpr uint8_t CS = D3;
				static constexpr uint8_t DC = D7;
				static constexpr uint8_t RESET = D6;
				static constexpr uint8_t BACKLIGHT = UINT8_MAX;
			};
#endif

#if defined(ARDUINO_ARCH_NRF52)
			struct NRF52_SPI_GENERIC
			{
				static constexpr uint8_t CS = D3;
				static constexpr uint8_t DC = D7;
				static constexpr uint8_t RESET = D6;
				static constexpr uint8_t BACKLIGHT = UINT8_MAX;
			};
#endif

#if defined(ARDUINO_ARCH_STM32F1) || defined(STM32F1)
			struct STM32F1_SPI_GENERIC
			{
				static constexpr uint8_t CS = 7;
				static constexpr uint8_t DC = 10;
				static constexpr uint8_t RESET = 11;
				static constexpr uint8_t BACKLIGHT = UINT8_MAX;
			};
#endif

#if defined(STM32F4)
			struct STM32F4_SPI_GENERIC
			{
				static constexpr uint8_t CS = PA15;
				static constexpr uint8_t DC = PB9;
				static constexpr uint8_t RESET = PB8;
				static constexpr uint8_t BACKLIGHT = UINT8_MAX;
			};
#endif

#if defined(STM32H7xx)
			struct STM32H7_SPI_GENERIC
			{
				static constexpr uint8_t CS = PE11;
				static constexpr uint8_t DC = PE13;
				static constexpr uint8_t RESET = UINT8_MAX;
				static constexpr uint8_t BACKLIGHT = UINT8_MAX;
			};
#endif
		}
	}
}

#endif