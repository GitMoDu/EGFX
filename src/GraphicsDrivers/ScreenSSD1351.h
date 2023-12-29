// ScreenSSD1351.h

#ifndef _SCREEN_SSD1351_h
#define _SCREEN_SSD1351_h

#include "../IFrame/IFramePrimitives.h"
#include "../IFrame/IFrameSink.h"
#include "GraphicsEngine/Geometry.h"
#include "lcdgfx\SSD1351.h"


/// <summary>
/// TODO: Black Frame insertion, since this is an OLED.
/// </summary>
template<const uint8_t spiChannel,
	const uint8_t pinCS,
	const uint8_t pinDC,
	const uint8_t pinRST,
	const uint8_t screenWidth,
	const uint8_t screenHeight>
class ScreenSSD1351_X_X
	: public virtual IFrameSink
	, public virtual IFramePrimitives
{
protected:
	DisplaySSD1351_128x128x16_SPI DriverSpi;

public:
	ScreenSSD1351_X_X()
		: IFrameSink()
		, IFramePrimitives()
		, DriverSpi(pinRST, { spiChannel, {pinCS}, pinDC, SSD1351SpiSpeed, -1, -1 })
	{
	}

public: // IFrameSink overloads.
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

	/// <summary>
	/// Since graphics are drawn directly to screen buffer, there's nothing to push.
	/// </summary>
	/// <param name="frameBuffer"></param>
	/// <returns>True on success.</returns>
	virtual const bool PushFrame(const uint8_t* frameBuffer) final
	{
		return true;
	}

	virtual const bool CanPushFrame() final
	{
		return true;
	}

public: // IFrameBufferPrimitives overloads.
	virtual const bool IsDirectDraw() final
	{
		return true;
	}

	virtual void ClearFrame() final
	{
		DriverSpi.clear();
	}

	virtual void Pixel(const uint8_t x, const uint8_t y, const RgbColor& color) final
	{
#if defined(DEBUG)
		if (x >= screenWidth
			|| y >= screenHeight)
		{
			Serial.println(F("Invalid parameters."));
			return;
		}
#endif
		DriverSpi.setColor(RGB_COLOR16(color.r, color.g, color.b));
		DriverSpi.putPixel(x, y);
	}

	virtual void Line(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const RgbColor& color) final
	{
#if defined(DEBUG)
		if (x1 >= screenWidth
			|| y1 >= screenHeight
			|| x2 >= screenWidth
			|| y2 >= screenHeight)
		{
			Serial.println(F("Invalid parameters."));
			return;
		}
#endif

		if (x1 == x2)
		{
			DriverSpi.setColor(RGB_COLOR16(color.r, color.g, color.b));
			DriverSpi.drawVLine(x1, y1, y2);
		}
		else if (y1 == y2)
		{
			DriverSpi.setColor(RGB_COLOR16(color.r, color.g, color.b));
			DriverSpi.drawHLine(x1, y1, x2);
		}
		else
		{
			DriverSpi.setColor(RGB_COLOR16(color.r, color.g, color.b));
			DriverSpi.drawLine(x1, y1, x2, y2);
		}
	}

	virtual void RectangleFill(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const RgbColor& color) final
	{
#if defined(DEBUG)
		if (x1 >= screenWidth
			|| y1 >= screenHeight)
		{
			Serial.println(F("Invalid parameters."));
			Serial.print(F("x1: "));
			Serial.println(x1);
			Serial.print(F("y1: "));
			Serial.println(y1);
			Serial.print(F("x2: "));
			Serial.println(x2);
			Serial.print(F("y2: "));
			Serial.println(y2);
			Serial.println();
			Serial.print(F("Screen: "));
			Serial.print(screenWidth);
			Serial.print('x');
			Serial.println(screenHeight);

			return;
		}
#endif
		DriverSpi.setColor(RGB_COLOR16(color.r, color.g, color.b));
		if (x2 >= screenWidth)
		{
			if (y2 >= screenWidth)
			{
				DriverSpi.fillRect(x1, y1, screenWidth - 1, screenHeight - 1);
			}
			else
			{
				DriverSpi.fillRect(x1, y1, screenWidth - 1, y2);
			}
		}
		else if (y2 >= screenHeight)
		{
			DriverSpi.fillRect(x1, y1, x2, screenHeight - 1);
		}
		else
		{
			DriverSpi.fillRect(x1, y1, x2, y2);
		}
	}

	virtual void Rectangle(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2, const RgbColor& color, const uint8_t width) final
	{
#if defined(DEBUG)
		if (x1 >= screenWidth
			|| y1 >= screenHeight
			|| x2 < x1
			|| y2 < y1)
		{
			Serial.println(F("Invalid parameters."));
			return;
		}
#endif
#if defined(DEBUG)
		if ((width > ((x2 - x1) / 2))
			|| (width > ((y2 - y1) / 2)))
		{
			Serial.println(F("Invalid parameters."));
			return;
		}
#endif
		for (uint_fast8_t i = 0; i < width; i++)
		{
#if defined(DEBUG)
			if (
				((x1 + i) > ((x2 + x1) / 2))
				|| ((y1 + i) > ((y2 + y1) / 2)))
			{
				Serial.println(F("Invalid parameters 3."));
				return;
			}
#endif

			DriverSpi.setColor(RGB_COLOR16(color.r, color.g, color.b));
			DriverSpi.drawRect(x1 + i, y1 + i, x2 - i - 1, y2 - i - 1);
		}
	}

	/// <summary>
	/// No hardware support, workaround implementation.
	/// Based on https://github.com/Sarah-Hesham-2022/ComputerGraphics-Algorithms-With-CPP
	/// </summary>
	/// <param name="xCenter"></param>
	/// <param name="yCenter"></param>
	/// <param name="radius"></param>
	/// <param name="color"></param>
	virtual void CircleFill(const uint8_t xCenter, const uint8_t yCenter, const uint8_t radius, const RgbColor& color) final
	{
#if defined(DEBUG)
		if (xCenter >= screenWidth
			|| yCenter >= screenHeight
			|| (xCenter + radius) >= screenWidth
			|| (yCenter + radius) >= screenWidth)
		{
			Serial.println(F("Invalid parameters."));

			return;
		}
#endif
		DriverSpi.setColor(RGB_COLOR16(color.r, color.g, color.b));

		// Fill center rectangle.
		const uint_fast8_t squareDimension = Geometry::GetSquareInsideCircleDimension(radius);

		const uint_fast8_t squareStartX = (xCenter - (squareDimension / 2));
		const uint_fast8_t squareStartY = (yCenter - (squareDimension / 2));
		const uint_fast8_t squareEndX = (xCenter + (squareDimension / 2));
		const uint_fast8_t squareEndY = (yCenter + (squareDimension / 2));
		DriverSpi.fillRect(squareStartX, squareStartY, squareEndX, squareEndY);


		// Fill circle lines.
		const uint8_t isEven = (radius & 0x01) == 0;
		const uint8_t effectiveRadius = radius - isEven;
		uint_fast8_t x = 0;
		uint_fast8_t y = effectiveRadius;
		int16_t d1 = 1 - effectiveRadius;
		int_fast8_t c1 = 3;
		uint_fast8_t c2 = 5 - (2 * effectiveRadius);
		while (x < y)
		{
			if (d1 < 0)
			{
				d1 += c1;
				c2 += 2;
			}
			else
			{
				d1 += c2;
				c2 += 4;
				y--;
			}
			c1 += 2;
			x++;

			DriverSpi.drawHLine(xCenter - x, yCenter + y, xCenter + x);
			DriverSpi.drawHLine(xCenter - x, yCenter - y, xCenter + x);

			DriverSpi.drawVLine(xCenter - y, yCenter - x, yCenter + x);
			DriverSpi.drawVLine(xCenter + y, yCenter - x, yCenter + x);
		}
	}

	virtual void Circle(const uint8_t xCenter, const uint8_t yCenter, const uint8_t radius, const RgbColor& color) final
	{
#if defined(DEBUG)
		if (xCenter >= screenWidth
			|| yCenter >= screenHeight
			|| (xCenter + radius) >= screenWidth
			|| (yCenter + radius) >= screenWidth)
		{
			Serial.println(F("Invalid parameters."));

			return;
		}
#endif
		DriverSpi.setColor(RGB_COLOR16(color.r, color.g, color.b));
		DriverSpi.drawCircle(xCenter, yCenter, radius - 1);
	}
};

template<const uint8_t spiChannel,
	const uint8_t pinCS,
	const uint8_t pinDC,
	const uint8_t pinRST>
class ScreenSSD1351_128_128 : public ScreenSSD1351_X_X<spiChannel, pinCS, pinDC, pinRST, 128, 128>
{
public:
	ScreenSSD1351_128_128()
		: ScreenSSD1351_X_X<spiChannel, pinCS, pinDC, pinRST, 128, 128>()
	{}
};
#endif