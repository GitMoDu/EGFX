// ScreenDriverHelper.h

#ifndef _SCREEN_DRIVER_HELPER_h
#define _SCREEN_DRIVER_HELPER_h

#include <Wire.h>
#include <SPI.h>

namespace IntegerGlass
{
	/// <summary>
	/// Platform aware pin and clock setup of Wire (I2C).
	/// </summary>
	/// <param name="wire">Wire instance.</param>
	/// <param name="i2cSpeed">Clock speed.</param>
	/// <param name="pinSCL">Clock pin.</param>
	/// <param name="pinSDA">Data pin.</param>
	/// <returns>True on success.</returns>
	static const bool SetupAndBeginI2C(TwoWire& wire, const uint32_t i2cSpeed = 0, const uint8_t pinSCL = UINT8_MAX, const uint8_t pinSDA = UINT8_MAX)
	{
#if defined(ARDUINO_ARCH_ESP32)
		return wire.begin(pinSDA, pinSCL, i2cSpeed);
#else
#if defined(ARDUINO_ARCH_RP2040)
		if (pinSCL != UINT8_MAX)
		{
			wire.setSCL(pinSCL);
		}
		if (pinSDA != UINT8_MAX)
		{
			wire.setSDA(pinSDA);
		}
		wire.begin();
#elif defined(ARDUINO_ARCH_NRF52)
		wire.begin();

		if (pinSDA != UINT8_MAX
			&& pinSCL != UINT8_MAX)
		{
			wire.setPins(pinSDA, pinSCL);
		}
#else
		wire.begin();
#endif

		if (i2cSpeed > 0)
		{
			wire.setClock(i2cSpeed);
		}
#endif

		return true;
	}

	/// <summary>
	/// Platform aware pin setup of SPI.
	/// </summary>
	/// <param name="spi">SPI instance.</param>
	/// <param name="cs">Chip Select pin.</param>
	/// <param name="clk">Clock pin.</param>
	/// <param name="mosi">MOSI (Data) pin</param>
	/// <returns>True on success.</returns>
	static const bool SetupAndBeginSPI(IntegerGlass::SpiType& spi, const uint8_t cs = UINT8_MAX, const uint8_t clk = UINT8_MAX, const uint8_t mosi = UINT8_MAX)
	{
#if defined(ARDUINO_ARCH_ESP32)
		if (cs != UINT8_MAX
			&& clk != UINT8_MAX
			&& mosi != UINT8_MAX)
		{
			spi.begin((int8_t)clk, (int8_t)-1, (int8_t)mosi, (int8_t)cs);
		}
		else if (cs != UINT8_MAX && clk != UINT8_MAX)
		{
			spi.begin((int8_t)clk, (int8_t)-1, (int8_t)-1, (int8_t)cs);
		}
		else if (cs != UINT8_MAX)
		{
			spi.begin((int8_t)-1, (int8_t)-1, (int8_t)-1, (int8_t)cs);
		}
#elif defined(ARDUINO_ARCH_NRF52)
		if (clk != UINT8_MAX
			&& mosi != UINT8_MAX)
		{
			spi.setPins(UINT8_MAX, clk, mosi);
			spi.begin();
		}
		else
		{
			spi.begin();
		}
#elif defined(ARDUINO_ARCH_RP2040)
		if (cs != UINT8_MAX)
		{
			spi.setCS(cs);
		}

		if (clk != UINT8_MAX)
		{
			spi.setSCK(clk);
		}

		if (mosi != UINT8_MAX)
		{
			spi.setMOSI(mosi);
		}

		spi.begin(cs == UINT8_MAX);
#else
		spi.begin();
#endif

		return true;
	}
};
#endif