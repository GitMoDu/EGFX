// DriverSSD1351.h

#ifndef _DRIVER_SSD1351_h
#define _DRIVER_SSD1351_h

#include "../IFrame/IFrameSink.h"
#include "lcdgfx\SSD1351.h"

template<const uint8_t spiChannel,
	const uint8_t pinCS,
	const uint8_t pinDC,
	const uint8_t pinRST,
	const uint8_t screenWidth,
	const uint8_t screenHeight>
class DriverSSD1351_X_X
	: public virtual IFrameSink
	, public virtual IColorConverter<SSD1351PixelType>
{
public:
	static constexpr uint8_t ScreenWidth = screenWidth;
	static constexpr uint8_t ScreenHeight = screenHeight;

private:
	static constexpr uint32_t BufferSize = IFrame::GetBufferSize<SSD1351PixelType>(screenWidth, screenHeight);

protected:
	DisplaySSD1351_128x128x16_SPI DriverSpi;

public:
	DriverSSD1351_X_X()
		: IFrameSink()
		, IColorConverter<SSD1351PixelType>()
		, DriverSpi(pinRST, { spiChannel, {pinCS}, pinDC, SSD1351SpiSpeed, -1, -1 })
	{}

public:
	virtual const SSD1351PixelType GetRawColor(const RgbColor& color) final
	{
		return RGB_COLOR16(color.r, color.g, color.b);
	}

public:
	virtual const uint8_t GetWidth() final
	{
		return screenWidth;
	}

	virtual const uint8_t GetHeight() final
	{
		return screenHeight;
	}

	virtual const bool Start() final
	{
		DriverSpi.begin();

		DriverSpi.getInterface().setRotation(0);
		DriverSpi.clear();

		return true;
	}

	virtual void Stop() final
	{
		DriverSpi.end();
	}

	virtual const bool PushFrame(const uint8_t* frameBuffer) final
	{
		DriverSpi.drawBuffer16(0, 0, screenWidth, screenHeight, frameBuffer);

		return true;
	}

	virtual const bool CanPushFrame() final
	{
		return true;
	}
};

template<const uint8_t spiChannel,
	const uint8_t pinCS,
	const uint8_t pinDC,
	const uint8_t pinRST>
class DriverSSD1351_128_128 : public DriverSSD1351_X_X<spiChannel, pinCS, pinDC, pinRST, 128, 128>
{
public:
	DriverSSD1351_128_128()
		: DriverSSD1351_X_X<spiChannel, pinCS, pinDC, pinRST, 128, 128>()
	{}
};
#endif