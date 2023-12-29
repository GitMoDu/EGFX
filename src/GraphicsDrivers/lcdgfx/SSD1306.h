// SSD1306.h

#ifndef _SSD1306_h
#define _SSD1306_h

#include <stdint.h>
/* https://github.com/lexus2k/lcdgfx */
#include <lcdgfx.h>

//#if defined(ARDUINO_ARCH_ESP32)
//#include <HT_SSD1306Wire.h>
//#else
//#include <lcdgfx.h>
//#endif

#if defined(ARDUINO_ARCH_AVR)
#define SCREEN_I2C_SPEED 400000
#elif defined(ARDUINO_ARCH_STM32F1)
#define SCREEN_I2C_SPEED 700000
#else
#define SCREEN_I2C_SPEED 700000
#endif

typedef uint8_t SSD1306PixelType;
static const uint32_t SSD1306I2CSpeed = 0;
static const uint8_t SSD1306I2CAddress = 0x3C;

#undef SCREEN_I2C_SPEED
#endif