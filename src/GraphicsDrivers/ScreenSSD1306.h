// ScreenSSD1306.h

#ifndef _SCREEN_SSD1306_h
#define _SCREEN_SSD1306_h

#include "../IFrame/IFramePrimitives.h"
#include "../IFrame/IFrameSink.h"
#include "lcdgfx\SSD1306.h"

template<const uint8_t i2cChannel,
	const int8_t pinSCL,
	const int8_t pinSDA,
	const int8_t pinRST,
	const uint8_t screenWidth,
	const uint8_t screenHeight>
class ScreenSSD1306_X_X
	: public virtual IFrameSink
	, public virtual IFramePrimitives
{
private:
	static constexpr uint8_t RgbToMonochrome(const uint8_t r, const uint8_t g, const uint8_t b)
	{
		return (r > 0) || (g > 0) || (b > 0);
	}

	static const uint8_t RgbToMonochrome(const RgbColor& color)
	{
		return (color.r > 0) || (color.g > 0) || (color.b > 0);
	}

protected:
	DisplaySSD1306_128x64_I2C Driver;

public:
	ScreenSSD1306_X_X()
		: IFrameSink()
		, IFramePrimitives()
		, Driver(pinRST,
			{ i2cChannel, SSD1306I2CAddress, pinSCL, pinSDA, SSD1306I2CSpeed })
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
		Driver.begin();
		Driver.setFixedFont(ssd1306xled_font6x8);
		//Driver.setBackground(0);
		//Driver.setOffset(0, 0);
		//Driver.getInterface().setContrast(1);
		Driver.clear();

		return true;
	}

	virtual void Stop() final
	{
		Driver.end();
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
		Driver.clear();
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
		Driver.setColor(RgbToMonochrome(color.r, color.g, color.b));
		Driver.putPixel(x, y);
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
		Driver.setColor(RgbToMonochrome(color.r, color.g, color.b));
		if (x1 == x2)
		{
			//Driver.drawVLine(x1, y1, y2);
		}
		else if (y1 == y2)
		{
			//Driver.drawHLine(x1, y1, x2);
		}
		else
		{
			Driver.drawLine(x1, y1, x2, y2);
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
		Driver.setColor(RgbToMonochrome(color.r, color.g, color.b));
		if (x2 >= screenWidth)
		{
			if (y2 >= screenWidth)
			{
				Driver.fillRect(x1, y1, screenWidth - 1, screenHeight - 1);
			}
			else
			{
				Driver.fillRect(x1, y1, screenWidth - 1, y2);
			}
		}
		else if (y2 >= screenHeight)
		{
			Driver.fillRect(x1, y1, x2, screenHeight - 1);
		}
		else
		{
			Driver.fillRect(x1, y1, x2, y2);
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

			Driver.setColor(RgbToMonochrome(color.r, color.g, color.b));
			Driver.drawRect(x1 + i, y1 + i, x2 - i - 1, y2 - i - 1);
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
		Driver.setColor(RgbToMonochrome(color.r, color.g, color.b));

		// Fill center rectangle.
		//const uint_fast8_t squareDimension = Geometry::GetSquareInsideCircleDimension(radius);

		//const uint_fast8_t squareStartX = (xCenter - (squareDimension / 2));
		//const uint_fast8_t squareStartY = (yCenter - (squareDimension / 2));
		//const uint_fast8_t squareEndX = (xCenter + (squareDimension / 2));
		//const uint_fast8_t squareEndY = (yCenter + (squareDimension / 2));
		//Driver.fillRect(squareStartX, squareStartY, squareEndX, squareEndY);


		// Fill circle lines.
		//const uint8_t isEven = (radius & 0x01) == 0;
		const uint8_t effectiveRadius = radius;// -isEven;
		uint_fast8_t x = 0;
		uint_fast8_t y = effectiveRadius;
		int16_t d1 = 1 - effectiveRadius;
		int_fast8_t c1 = 3;
		uint_fast8_t c2 = 5 - 2 * effectiveRadius;
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

			//Driver.drawHLine(xCenter - x, yCenter + y, xCenter + x);
			//Driver.drawHLine(xCenter - x, yCenter - y, xCenter + x);

			//Driver.drawVLine(xCenter - y, yCenter - x, yCenter + x);
			//Driver.drawVLine(xCenter + y, yCenter - x, yCenter + x);
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
		Driver.setColor(RgbToMonochrome(color.r, color.g, color.b));
		Driver.drawCircle(xCenter, yCenter, radius - 1);
	}
};

template<const uint8_t i2cChannel,
	const int8_t pinSCL = -1,
	const int8_t pinSDA = -1,
	const int8_t pinRST = -1>
class ScreenSSD1306_128_64 : public ScreenSSD1306_X_X<i2cChannel, pinSCL, pinSDA, pinRST, 128, 64>
{
public:
	ScreenSSD1306_128_64()
		: ScreenSSD1306_X_X<i2cChannel, pinSCL, pinSDA, pinRST, 128, 64>()
	{}
};
#endif