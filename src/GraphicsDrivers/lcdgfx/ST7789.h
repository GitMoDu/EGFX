// ST7789.h

#ifndef _ST7789_h
#define _ST7789_h

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

typedef uint16_t ST7789PixelType;
static const uint32_t ST7789SpiSpeed = SCREEN_SPI_SPEED;

#undef SCREEN_SPI_SPEED
#endif