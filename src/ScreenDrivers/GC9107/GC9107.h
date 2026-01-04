#ifndef _EGFX_SCREEN_DRIVERS_GC9107_h
#define _EGFX_SCREEN_DRIVERS_GC9107_h

#include <stdint.h>

namespace Egfx
{
	namespace GC9107
	{
		static constexpr uint8_t Width = 128;
		static constexpr uint8_t Height = 128;

		static constexpr uint32_t RESET_DELAY_MICROS = 100000;
		static constexpr uint32_t SOFTWARE_RESET_DELAY_MICROS = 5000;
		static constexpr uint32_t SLEEP_EXIT_DELAY_MICROS = 120000;
		static constexpr uint32_t DISPLAY_ON_DELAY_MICROS = 5000;

		static constexpr uint32_t SpiDefaultSpeed = 24000000;
		static constexpr uint32_t SpiMaxSpeed = 80000000;

		static constexpr uint32_t LimitedSpiSpeed(const uint32_t speed)
		{
			return speed < SpiMaxSpeed ? speed : SpiMaxSpeed;
		}

#if defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F4)
		static constexpr size_t SpiMaxChunkSize = size_t(UINT8_MAX);
#else
		static constexpr size_t SpiMaxChunkSize = SIZE_MAX;
#endif

		enum class CommandEnum : uint8_t
		{
			Nop = 0x00,
			SoftwareReset = 0x01,
			SleepIn = 0x10,
			SleepOut = 0x11,
			InversionOff = 0x20,
			InversionOn = 0x21,
			DisplayOff = 0x28,
			DisplayOn = 0x29,
			IdleOff = 0x38,
			IdleOn = 0x39,
			SetColumnAddress = 0x2A,
			SetRowAddress = 0x2B,
			MemoryWrite = 0x2C,
			MemoryRead = 0x2E,
			SetMemoryAccessControl = 0x36,
			SetPixelFormat = 0x3A
		};

		namespace PixelFormats
		{
			static constexpr uint8_t Rgb565 = 0x55;
		}

		namespace MemoryAccessControl
		{
			static constexpr uint8_t RowAddressOrder = 0x80;
			static constexpr uint8_t ColumnAddressOrder = 0x40;
			static constexpr uint8_t RowColumnExchange = 0x20;
			static constexpr uint8_t VerticalRefreshOrder = 0x10;
			static constexpr uint8_t ColorOrderBgr = 0x08;
			static constexpr uint8_t HorizontalRefreshOrder = 0x04;

			static constexpr uint8_t Default = 0x00;
		}
	}
}
#endif