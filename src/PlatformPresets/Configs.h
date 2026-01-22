#ifndef _EGFX_PLATFORM_PRESETS_CONFIGS_h
#define _EGFX_PLATFORM_PRESETS_CONFIGS_h

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

namespace Egfx
{
	namespace PlatformPresets
	{
		namespace Configs
		{
			struct ARDUINO_I2C_DEFAULT
			{
				static constexpr uint8_t SDA = UINT8_MAX;
				static constexpr uint8_t SCL = UINT8_MAX;
				static constexpr uint8_t BACKLIGHT = UINT8_MAX;
				using InterfaceType = Egfx::WireType;
				static InterfaceType& Interface()
				{
					return Wire;
				}
			};

#if defined(ARDUINO_ARCH_AVR)
			struct AVR_I2C_GENERIC
			{
				static constexpr uint8_t SDA = A4;
				static constexpr uint8_t SCL = A5;
				static constexpr uint8_t BACKLIGHT = A3;
				using InterfaceType = Egfx::WireType;
				static InterfaceType& Interface()
				{
					return Wire;
				}
			};

			struct ARDUINO_SPI_GENERIC
			{
				static constexpr uint8_t CS = 10;
				static constexpr uint8_t DC = 9;
				static constexpr uint8_t RESET = 8;
				static constexpr uint8_t BACKLIGHT = 7;
				using InterfaceType = Egfx::SpiType;
				static InterfaceType& Interface()
				{
					return SPI;
				}
			};
#endif

#if defined(ARDUINO_ARCH_RP2040)
			struct RP2350_SPI_GENERIC
			{
				static constexpr uint8_t CS = 17;
				static constexpr uint8_t DC = 20;
				static constexpr uint8_t RESET = 21;
				static constexpr uint8_t BACKLIGHT = UINT8_MAX;

				using InterfaceType = Egfx::SpiType;
				static InterfaceType& Interface()
				{
					return SPI;
				}
			};

			struct RP2350_SPI_LCD_1_47_INCH
			{
				static constexpr uint8_t CS = 17;
				static constexpr uint8_t DC = 16;
				static constexpr uint8_t RESET = 20;
				static constexpr uint8_t BACKLIGHT = 21;

				using InterfaceType = Egfx::SpiType;
				static InterfaceType& Interface()
				{
					return SPI;
				}
			};

			struct RP2350_WAVESHARE_PICO_LCD_1_3_INCH
			{
				static constexpr uint8_t CS = 9;
				static constexpr uint8_t DC = 8;
				static constexpr uint8_t RESET = 12;
				static constexpr uint8_t BACKLIGHT = 13;

				static constexpr uint8_t SCK = 10;
				static constexpr uint8_t MOSI = 11;
				static constexpr uint8_t MISO = UINT8_MAX;

				using InterfaceType = Egfx::SpiType;
				static InterfaceType& Interface()
				{
					return SPI1;
				}
			};

			struct RP2350_SPOTPEAR_SPI_LCD_0_85_INCH_GC9107
			{
				static constexpr uint8_t CS = 13;
				static constexpr uint8_t DC = 12;
				static constexpr uint8_t RESET = UINT8_MAX;
				static constexpr uint8_t BACKLIGHT = UINT8_MAX;

				using InterfaceType = Egfx::SpiType;
				static InterfaceType& Interface()
				{
					return SPI1;
				}
			};

			struct RP2350_WAVESHARE_SPI_LCD_TOUCH_2_INCH
			{
				static constexpr uint8_t CS = 17;
				static constexpr uint8_t DC = 16;
				static constexpr uint8_t RESET = 20;
				static constexpr uint8_t BACKLIGHT = 15;

				static constexpr uint8_t SCK = 18;
				static constexpr uint8_t MOSI = 19;
				static constexpr uint8_t MISO = 12;

				using InterfaceType = Egfx::SpiType;
				static InterfaceType& Interface()
				{
					return SPI;
				}
			};
#endif

#if defined(ARDUINO_ARCH_ESP32)
			struct ESP32_SPI_GENERIC
			{
				static constexpr uint8_t CS = 5;
				static constexpr uint8_t DC = 16;
				static constexpr uint8_t RESET = 23;
				static constexpr uint8_t BACKLIGHT = UINT8_MAX;

				using InterfaceType = Egfx::SpiType;
				static InterfaceType& Interface()
				{
					static InterfaceType spi(VSPI);

					// Configure default pins once. 18 and 19 are VSPI default SCK and MOSI.
					spi.ConfigurePins((int8_t)18, (int8_t)(UINT8_MAX), (int8_t)19, (int8_t)CS);


					return spi;
				}
			};

			struct ESP32_LILYGO_SPI_TDISPLAY
			{
				static constexpr uint8_t CS = 5;
				static constexpr uint8_t DC = 16;
				static constexpr uint8_t SCK = 18;
				static constexpr uint8_t MOSI = 19;
				static constexpr uint8_t MISO = UINT8_MAX; // not used
				static constexpr uint8_t RESET = 23;
				static constexpr uint8_t BACKLIGHT = 4;

				using InterfaceType = Egfx::SpiType;

				static InterfaceType& Interface()
				{
					static InterfaceType spi(VSPI);

					spi.end();

					// Configure pins once (Esp32Spi stores them; BeginConfigured() can use them later)
					spi.ConfigurePins(SCK, -1, MOSI, CS);

					// Optional: tune max DMA transaction size (IDF limit is typically 4092 bytes)
					spi.ConfigureMaxTransferBytes(16384);
					spi.BeginConfigured();

					return spi;
				}
			};
#endif

#if defined(ARDUINO_SEEED_XIAO_RP2350)
			struct RP2350_XIAO_SPI_GENERIC
			{
				static constexpr uint8_t CS = D3;
				static constexpr uint8_t DC = D7;
				static constexpr uint8_t RESET = D6;
				static constexpr uint8_t BACKLIGHT = UINT8_MAX;

				using InterfaceType = Egfx::SpiType;
				static InterfaceType& Interface()
				{
					return SPI;
				}
			};
#endif

#if defined(ARDUINO_ARCH_NRF52)
			struct NRF52_SPI_GENERIC
			{
				static constexpr uint8_t CS = D3;
				static constexpr uint8_t DC = D7;
				static constexpr uint8_t RESET = D6;
				static constexpr uint8_t BACKLIGHT = UINT8_MAX;

				using InterfaceType = Egfx::SpiType;
				static InterfaceType& Interface()
				{
					return SPI;
				}
			};
#endif

#if defined(ARDUINO_ARCH_STM32F1) || defined(STM32F1)
			struct STM32F1_SPI_GENERIC
			{
				static constexpr uint8_t CS = 7;
				static constexpr uint8_t DC = 10;
				static constexpr uint8_t RESET = 11;
				static constexpr uint8_t BACKLIGHT = UINT8_MAX;

				using InterfaceType = Egfx::SpiType;
				static InterfaceType& Interface()
				{
					return SPI;
				}
			};
#endif

#if defined(STM32F4)
			struct STM32F4_SPI_GENERIC
			{
				static constexpr uint8_t CS = PA15;
				static constexpr uint8_t DC = PB9;
				static constexpr uint8_t RESET = PB8;
				static constexpr uint8_t BACKLIGHT = UINT8_MAX;

				using InterfaceType = Egfx::SpiType;
				static InterfaceType& Interface()
				{
					return SPI;
				}
			};
#endif

#if defined(STM32H7xx)
			struct STM32H7_SPI_GENERIC
			{
				static constexpr uint8_t CS = PE11;
				static constexpr uint8_t DC = PE13;
				static constexpr uint8_t RESET = UINT8_MAX;
				static constexpr uint8_t BACKLIGHT = UINT8_MAX;

				using InterfaceType = Egfx::SpiType;
				static InterfaceType& Interface()
				{
					return SPI;
				}
			};
#endif
		}
	}
}

#endif
