#ifndef _EGFX_SCREEN_DRIVERS_ST7789T3_h
#define _EGFX_SCREEN_DRIVERS_ST7789T3_h

#include <stdint.h>

namespace Egfx
{
	namespace ST7789T3
	{
		static constexpr uint32_t RESET_DELAY_MICROS = 100000;
		static constexpr uint16_t RESET_WAIT_MICROS = 5000;
		static constexpr uint32_t SLEEP_EXIT_DELAY_MICROS = 120000 / 100;
		static constexpr uint16_t DISPLAY_ON_DELAY_MICROS = 1000;

		static constexpr uint32_t SpiMaxSpeed = 80000000;
		static constexpr uint32_t SpiDefaultSpeed = 40000000;

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
			NormalDisplayOn = 0x13,
			InversionOn = 0x21,
			DisplayOn = 0x29,
			SetColumnAddress = 0x2A,
			SetRowAddress = 0x2B,
			MemoryWrite = 0x2C,
			SetMemoryAccessControl = 0x36,
			SetPixelFormat = 0x3A,
			PorchSetting = 0xB2,
			GateControl = 0xB7,
			VCOMSSetting = 0xBB,
			LCMControl = 0xC0,
			VDVVRHEN = 0xC2,
			VRHSet = 0xC3,
			VDVSet = 0xC4,
			FrameRateControl = 0xC6,
			PowerControl1 = 0xD0,
			PositiveGamma = 0xE0,
			NegativeGamma = 0xE1,
		};

		namespace PixelFormats
		{
			static constexpr uint8_t Rgb565 = 0x55;
		}
	}
}

#endif