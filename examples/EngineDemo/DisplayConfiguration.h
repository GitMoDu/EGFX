#ifndef _DISPLAY_CONFIGURATION_h
#define _DISPLAY_CONFIGURATION_h

#include <Arduino.h>
#include <EgfxCore.h>
#include <EgfxScreenDrivers.h>

#include <EgfxPlatformPresets.h>

namespace DisplayConfiguration
{
	// Uncomment the desired pin configuration.
	// Pin definitions, based on LCD wiring and platform.
#if defined(STM32H7xx)
	using DisplayPins = Egfx::PlatformPresets::Pins::STM32H7_SPI_GENERIC;
#elif defined(STM32F4)
	using DisplayPins = Egfx::PlatformPresets::Pins::STM32F4_SPI_GENERIC;
#elif defined(ARDUINO_ARCH_STM32F1) || defined(STM32F1)
	using DisplayPins = Egfx::PlatformPresets::Pins::STM32F1_SPI_GENERIC;
#elif defined(ARDUINO_ARCH_AVR)
	using DisplayPins = Egfx::PlatformPresets::Pins::AVR_SPI_GENERIC;
#elif defined(ARDUINO_ARCH_ESP32)
	using DisplayPins = Egfx::PlatformPresets::Pins::ESP32_SPI_GENERIC;
#elif defined(ARDUINO_SEEED_XIAO_RP2350)
	using DisplayPins = Egfx::PlatformPresets::Pins::RP2350_XIAO_SPI_GENERIC;
#elif defined(ARDUINO_ARCH_RP2040)
	//using DisplayPins = Egfx::PremadePlatformPresets::Pins::RP2350_SPI_GENERIC;
	using DisplayPins = Egfx::PlatformPresets::Pins::RP2350_SPI_LCD_1_47_INCH;
#elif defined(ARDUINO_ARCH_NRF52)
	using DisplayPins = Egfx::PremadePlatformPresets::Pins::NRF52_SPI_GENERIC;
#endif

	// Uncomment the matching line for the display communications type. Screen communications instance definition.
	Egfx::WireType& DisplayCommsInstance(Wire);
//#if defined(ARDUINO_ARCH_STM32F1) || defined(STM32F1) || defined(ARDUINO_ARCH_STM32F4) || defined(STM32F4) || defined(STM32H7)
//	Egfx::SpiType DisplayCommsInstance(3);
//#elif defined(ARDUINO_ARCH_ESP32)
//	Egfx::SpiType& DisplayCommsInstance(VSPI);
//#else
//	Egfx::SpiType& DisplayCommsInstance(SPI);
//#endif

	// Uncomment Driver and matching Framebuffer type. 
	// Drivers will have Async, DMA, and RTOS variants, depending on platform and driver.
	// Framebuffers can be monochrome (BinaryFramebuffer) or color (Color8Framebuffer,
	// and have configurable async clear steps, with dma async clear on supported platforms.

	//using ScreenDriverType = Egfx::ScreenDriverSSD1306_64x32x1_I2C;
	//using ScreenDriverType = Egfx::ScreenDriverSSD1306_64x48x1_I2C;
	using ScreenDriverType = Egfx::ScreenDriverSSD1306_72x40x1_I2C;
	//using ScreenDriverType = Egfx::ScreenDriverSSD1306_128x32x1_I2C;
	//using ScreenDriverType = Egfx::ScreenDriverSSD1306_128x64x1_I2C;
	using FramebufferType = Egfx::BinaryFramebuffer<ScreenDriverType::ScreenWidth, ScreenDriverType::ScreenHeight>;

	//using ScreenDriverType = Egfx::ScreenDriverSSD1306_128x64x1_SPI<DisplayPins::CS, DisplayPins::DC, DisplayPins::RESET, F_CPU / 4>;
	//using ScreenDriverType = Egfx::ScreenDriverSH1106_128x64x1_SPI_Rtos<DisplayPins::CS, DisplayPins::DC, DisplayPins::RESET, F_CPU / 4>;
	//using FramebufferType = Egfx::BinaryFramebuffer<ScreenDriverType::ScreenWidth, ScreenDriverType::ScreenHeight>;

	//using ScreenDriverType = Egfx::ScreenDriverSSD1331_96x64x8_SPI_Async<DisplayPins::CS, DisplayPins::DC, DisplayPins::RESET, F_CPU, 2>;
	//using FramebufferType = Egfx::Color8Framebuffer<ScreenDriverType::ScreenWidth, ScreenDriverType::ScreenHeight, 2>;

	//using ScreenDriverType = Egfx::ScreenDriverSSD1331_96x64x16_SPI_Async<DisplayPins::CS, DisplayPins::DC, DisplayPins::RESET, F_CPU / 2, 4>;
	//using ScreenDriverType = Egfx::ScreenDriverSSD1351_128x128x16_SPI_Rtos<DisplayPins::CS, DisplayPins::DC, DisplayPins::RESET, F_CPU / 2>;
	//using ScreenDriverType = Egfx::ScreenDriverST7789_240x240x16_SPI_Dma<DisplayPins::CS, DisplayPins::DC, DisplayPins::RESET, F_CPU / 2>;
	//using ScreenDriverType = Egfx::ScreenDriverST7735S_80x160x16_SPI<DisplayPins::CS, DisplayPins::DC, DisplayPins::RESET>;
	//using ScreenDriverType = Egfx::ScreenDriverSSD1331_96x64x16_SPI_Dma<DisplayPins::CS, DisplayPins::DC, DisplayPins::RESET, F_CPU / 2, 0>;
	//using ScreenDriverType = Egfx::ScreenDriverST7789_172x320x16_SPI_Dma<DisplayPins::CS, DisplayPins::DC, DisplayPins::RESET, F_CPU / 2>;
	//using FramebufferType = Egfx::Color16Framebuffer<ScreenDriverType::ScreenWidth, ScreenDriverType::ScreenHeight>;
}
#endif

