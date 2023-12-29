// SSD1331.h

#ifndef _SSD1331_h
#define _SSD1331_h

#include <stdint.h>
/* https://github.com/lexus2k/lcdgfx */
#include <lcdgfx.h>

#if defined(ARDUINO_ARCH_AVR)
#define SCREEN_SPI_SPEED 0
#elif defined(ARDUINO_ARCH_STM32F1)
#define SCREEN_SPI_SPEED (F_CPU / 2)
#else
#define SCREEN_SPI_SPEED 1000000
#endif

typedef uint8_t SSD1331PixelType;
static const uint32_t SSD1331SpiSpeed = SCREEN_SPI_SPEED;

#undef SCREEN_SPI_SPEED
#endif