// BitmapSprite.h

#ifndef _BITMAP_SPRITE_h
#define _BITMAP_SPRITE_h

#include "../Model/ISprite.h"

template<const uint8_t width,
	const uint8_t height>
class AbstractBitmapSprite : public AbstractSprite<width, height>
{
protected:
	const uint32_t* RgbBitmap;

public:
	AbstractBitmapSprite(const uint32_t* rgbBitmap = nullptr)
		: AbstractSprite<width, height>()
		, RgbBitmap(rgbBitmap)
	{}

	void SetBitmap(const uint32_t* rgbBitmap)
	{
		if (rgbBitmap != nullptr)
		{
			RgbBitmap = rgbBitmap;
		}
	}
};

template<const uint8_t width,
	const uint8_t height>
class BitmapSprite : public AbstractBitmapSprite<width, height>
{
private:
	using BaseClass = AbstractBitmapSprite<width, height>;

protected:
	using BaseClass::RgbBitmap;

public:
	BitmapSprite(const uint32_t* rgbBitmap = nullptr) : BaseClass(rgbBitmap)
	{
		RgbBitmap = rgbBitmap;
	}

	const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
	{
		return GetColor(color, x, y);
	}

protected:
	virtual const bool GetColor(RgbColor& color, const uint8_t x, const uint8_t y)
	{
		const size_t offset = ((size_t)y * width) + x;

		color.r = (RgbBitmap[offset] >> 16) & UINT8_MAX;
		color.g = (RgbBitmap[offset] >> 8) & UINT8_MAX;
		color.b = RgbBitmap[offset];

		return true;
	}
};


#if defined(ARDUINO_ARCH_AVR)
template<const uint8_t width,
	const uint8_t height>
class FlashBitmapSprite : public AbstractBitmapSprite<width, height>
{
private:
	using BaseClass = AbstractBitmapSprite<width, height>;

protected:
	using BaseClass::RgbBitmap;

public:
	FlashBitmapSprite(const uint32_t* rgbBitmap = nullptr)
		: BaseClass(rgbBitmap)
	{}

	const bool Get(RgbColor& color, const uint8_t x, const uint8_t y) final
	{
		return GetColor(color, x, y);
	}

protected:
	virtual const bool GetColor(RgbColor& color, const uint8_t x, const uint8_t y)
	{
		const size_t offset = (((size_t)y * width) + x) * sizeof(uint32_t);
		const uint8_t* bitmap = (const uint8_t*)RgbBitmap;

		color.r = pgm_read_byte(&bitmap[offset + 2]);
		color.g = pgm_read_byte(&bitmap[offset + 1]);
		color.b = pgm_read_byte(&bitmap[offset + 0]);

		return true;
	}
};
#else
template<const uint8_t width,
	const uint8_t height>
class FlashBitmapSprite : public BitmapSprite<width, height>
{
public:
	FlashBitmapSprite(const uint32_t* rgbBitmap = nullptr)
		: BitmapSprite<width, height>(rgbBitmap)
	{}
};
#endif
#endif