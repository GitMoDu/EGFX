// DriverSSD1331.h

#ifndef _DRIVER_SSD1331_h
#define _DRIVER_SSD1331_h

#include "../IFrame/IFrameSink.h"
#include "lcdgfx\SSD1331.h"

template<const uint8_t spiChannel,
	const uint8_t pinCS,
	const uint8_t pinDC,
	const uint8_t pinRST,
	const uint8_t screenWidth,
	const uint8_t screenHeight>
class DriverSSD1331_X_X
	: public virtual IFrameSink
	, public virtual IColorConverter<SSD1331PixelType>
{
public:
	static constexpr uint8_t ScreenWidth = screenWidth;
	static constexpr uint8_t ScreenHeight = screenHeight;

	typedef SSD1331PixelType pixel_t;

private:
	static constexpr uint32_t BufferSize = IFrame::GetBufferSize<SSD1331PixelType>(screenWidth, screenHeight);

protected:
	DisplaySSD1331_96x64x8_SPI DriverSpi;

public:
	DriverSSD1331_X_X()
		: IFrameSink()
		, IColorConverter<SSD1331PixelType>()
		, DriverSpi(pinRST, { spiChannel, {pinCS}, pinDC, SSD1331SpiSpeed, -1, -1 })
	{}

public:
	virtual const SSD1331PixelType GetRawColor(const RgbColor& color) final
	{
		return RGB_COLOR8(color.r, color.g, color.b);
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
		DriverSpi.drawBuffer8(0, 0, screenWidth, screenHeight, frameBuffer);

		return true;
	}

	virtual const bool CanPushFrame() final
	{
		return true;
		//return !digitalRead(pinCS);
	}
};

template<const uint8_t spiChannel,
	const uint8_t pinCS,
	const uint8_t pinDC,
	const uint8_t pinRST>
class DriverSSD1331_96_64 : public DriverSSD1331_X_X<spiChannel, pinCS, pinDC, pinRST, 96, 64>
{
public:
	DriverSSD1331_96_64()
		: DriverSSD1331_X_X<spiChannel, pinCS, pinDC, pinRST, 96, 64>()
	{}
};
#endif