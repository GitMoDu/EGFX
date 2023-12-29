// SSD1351.h

#ifndef _SSD1351_h
#define _SSD1351_h

#include <stdint.h>
/* https://github.com/lexus2k/lcdgfx */
#include <lcdgfx.h>

#if defined(ARDUINO_ARCH_AVR)
#define SCREEN_SPI_SPEED 0
#elif defined(ARDUINO_ARCH_STM32F1)
#define SCREEN_SPI_SPEED 33000000
#else
#define SCREEN_SPI_SPEED 1000000
#endif

typedef uint16_t SSD1351PixelType;
static const uint32_t SSD1351SpiSpeed = SCREEN_SPI_SPEED;

#undef SCREEN_SPI_SPEED
#endif