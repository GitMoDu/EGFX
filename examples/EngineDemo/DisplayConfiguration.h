#ifndef _DISPLAY_CONFIGURATION_h
#define _DISPLAY_CONFIGURATION_h

#include <IntegerGlassScreenDrivers.h>
#include <IntegerGlassFramebuffers.h>
#include <IntegerGlassPlatformPresets.h>


// Configuration, based on display wiring and platform. Uncomment the desired pin configuration.
#if defined(STM32H7xx)
using DisplayConfig = IntegerGlass::PlatformPresets::Configs::STM32H7_SPI_GENERIC;
#elif defined(STM32F4)
using DisplayConfig = IntegerGlass::PlatformPresets::Configs::STM32F4_SPI_GENERIC;
#elif defined(ARDUINO_ARCH_STM32F1) || defined(STM32F1)
using DisplayConfig = IntegerGlass::PlatformPresets::Configs::STM32F1_SPI_GENERIC;
#elif defined(ARDUINO_ARCH_AVR)
//using DisplayConfig = IntegerGlass::PlatformPresets::Configs::AVR_I2C_GENERIC;
using DisplayConfig = IntegerGlass::PlatformPresets::Configs::ARDUINO_SPI_GENERIC;
#elif defined(ARDUINO_ARCH_ESP32)
using DisplayConfig = IntegerGlass::PlatformPresets::Configs::ESP32_SPI_GENERIC;
#elif defined(ARDUINO_SEEED_XIAO_RP2350)
using DisplayConfig = IntegerGlass::PlatformPresets::Configs::RP2350_XIAO_SPI_GENERIC;
#elif defined(ARDUINO_ARCH_RP2040)
using DisplayConfig = IntegerGlass::PlatformPresets::Configs::RP2350_SPI_GENERIC;
#elif defined(ARDUINO_ARCH_NRF52)
using DisplayConfig = IntegerGlass::PlatformPresets::Configs::NRF52_SPI_GENERIC;
#endif

// Display options definition. Adjust as needed and add to Framebuffer type definition below.
using IntegerGlassOptions = IntegerGlass::DisplayOptions::TemplateOptions<
	IntegerGlass::DisplayOptions::MirrorEnum::None,
	IntegerGlass::DisplayOptions::RotationEnum::None, 
	false
>;

// Uncomment Driver and matching Framebuffer type. Drivers will have Async, DMA, and RTOS variants, depending on the platform.
//using ScreenDriverType = IntegerGlass::ScreenDriverSSD1306_64x32x1_I2C;
//using ScreenDriverType = IntegerGlass::ScreenDriverSSD1306_64x48x1_I2C;
////using ScreenDriverType = IntegerGlass::ScreenDriverSSD1306_72x40x1_I2C;
//using ScreenDriverType = IntegerGlass::ScreenDriverSSD1306_128x32x1_I2C;
//using ScreenDriverType = IntegerGlass::ScreenDriverSSD1306_128x64x1_I2C;
//using ScreenDriverType = IntegerGlass::ScreenDriverSH1107_128x128x1_I2C;
using ScreenDriverType = IntegerGlass::ScreenDriverSSD1306_128x64x1_SPI<DisplayConfig::CS, DisplayConfig::DC, DisplayConfig::RESET>;
//using ScreenDriverType = IntegerGlass::ScreenDriverSH1106_128x64x1_SPI<DisplayConfig::CS, DisplayConfig::DC, DisplayConfig::RESET>;
//using ScreenDriverType = IntegerGlass::ScreenDriverSH1106_132x64x1_SPI<DisplayConfig::CS, DisplayConfig::DC, DisplayConfig::RESET>;
using FramebufferType = IntegerGlass::BinaryFramebuffer<ScreenDriverType::ScreenWidth, ScreenDriverType::ScreenHeight, 0, 0, IntegerGlassOptions>;

//using ScreenDriverType = IntegerGlass::ScreenDriverSSD1331_96x64x8_SPI<DisplayConfig::CS, DisplayConfig::DC, DisplayConfig::RESET>;
//using FramebufferType = IntegerGlass::Color8Framebuffer<ScreenDriverType::ScreenWidth, ScreenDriverType::ScreenHeight, 0, IntegerGlassOptions>;

//using ScreenDriverType = IntegerGlass::ScreenDriverSSD1331_96x64x16_SPI<DisplayConfig::CS, DisplayConfig::DC, DisplayConfig::RESET>;
//using ScreenDriverType = IntegerGlass::ScreenDriverSSD1351_128x128x16_SPI<DisplayConfig::CS, DisplayConfig::DC, DisplayConfig::RESET>;
//using ScreenDriverType = IntegerGlass::ScreenDriverST7735S_80x160x16_SPI<DisplayConfig::CS, DisplayConfig::DC, DisplayConfig::RESET>;
//using ScreenDriverType = IntegerGlass::ScreenDriverST7735S_160x128x16_SPI<DisplayConfig::CS, DisplayConfig::DC, DisplayConfig::RESET>;
//using ScreenDriverType = IntegerGlass::ScreenDriverST7789_172x320x16_SPI_Dma<DisplayConfig::CS, DisplayConfig::DC, DisplayConfig::RESET>;
//using ScreenDriverType = IntegerGlass::ScreenDriverST7789_240x240x16_SPI_Dma<DisplayConfig::CS, DisplayConfig::DC, DisplayConfig::RESET>;
//using ScreenDriverType = IntegerGlass::ScreenDriverST7789T3_240x320x16_SPI_Dma<DisplayConfig::CS, DisplayConfig::DC, DisplayConfig::RESET>;
//using ScreenDriverType = IntegerGlass::ScreenDriverGC9107_128x128x16_SPI<DisplayConfig::CS, DisplayConfig::DC, DisplayConfig::RESET>;
//using ScreenDriverType = IntegerGlass::ScreenDriverGC9A01_240x240x16_SPI<DisplayConfig::CS, DisplayConfig::DC, DisplayConfig::RESET>;
//using FramebufferType = IntegerGlass::Color16Framebuffer<ScreenDriverType::ScreenWidth, ScreenDriverType::ScreenHeight, 0, IntegerGlassOptions>;


#endif
