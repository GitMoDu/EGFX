// BitmapSprite.h

#ifndef _BITMAP_SPRITE_h
#define _BITMAP_SPRITE_h

#include "../Model/ISprite.h"
#include "../Model/ColorConverter.h"

template<typename ColorType,
	const uint8_t width,
	const uint8_t height>
class AbstractBitmapSprite : public AbstractSprite<width, height>
{
protected:
	const ColorType* RgbBitmap;

protected:
	virtual const bool GetColor(RgbColor& color, const uint8_t x, const uint8_t y)
	{
		return false;
	}

public:
	AbstractBitmapSprite(const ColorType* rgbBitmap)
		: AbstractSprite<width, height>()
		, RgbBitmap(rgbBitmap)
	{}

	void SetBitmap(const ColorType* rgbBitmap)
	{
		if (rgbBitmap != nullptr)
		{
			RgbBitmap = rgbBitmap;
		}
	}

	const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
	{
		return GetColor(color, x, y);
	}

};

template<const uint8_t width,
	const uint8_t height>
class BitmapRgb888Sprite : public AbstractBitmapSprite<uint32_t, width, height>
{
private:
	using BaseClass = AbstractBitmapSprite<uint32_t, width, height>;

protected:
	using BaseClass::RgbBitmap;

public:
	BitmapRgb888Sprite(const uint32_t* rgbBitmap = nullptr) : BaseClass(rgbBitmap)
	{}

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

template<const uint8_t width,
	const uint8_t height>
class BitmapRgb565Sprite : public AbstractBitmapSprite<uint16_t, width, height>
{
private:
	using BaseClass = AbstractBitmapSprite<uint16_t, width, height>;

protected:
	using BaseClass::RgbBitmap;

public:
	BitmapRgb565Sprite(const uint16_t* rgbBitmap = nullptr) : BaseClass(rgbBitmap)
	{}

protected:
	virtual const bool GetColor(RgbColor& color, const uint8_t x, const uint8_t y)
	{
		const size_t offset = ((size_t)y * width) + x;

		const uint16_t value = RgbBitmap[offset];
		color.r = (value & 0xF800) >> 8;
		color.g = (value & 0x07E0) >> 3;
		color.b = (value & 0x1F) << 3;

		return true;
	}
};


#if defined(ARDUINO_ARCH_AVR)
template<const uint8_t width,
	const uint8_t height>
class FlashBitmapRgb888Sprite : public AbstractBitmapSprite<uint32_t, width, height>
{
private:
	using BaseClass = AbstractBitmapSprite<uint32_t, width, height>;

protected:
	using BaseClass::RgbBitmap;

public:
	FlashBitmapRgb888Sprite(const uint32_t* rgbBitmap = nullptr)
		: BaseClass(rgbBitmap)
	{}

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

template<const uint8_t width,
	const uint8_t height>
class FlashBitmapRgb565Sprite : public AbstractBitmapSprite<uint16_t, width, height>
{
private:
	using BaseClass = AbstractBitmapSprite<uint16_t, width, height>;

protected:
	using BaseClass::RgbBitmap;

public:
	FlashBitmapRgb565Sprite(const uint16_t* rgbBitmap = nullptr)
		: BaseClass(rgbBitmap)
	{}

protected:
	virtual const bool GetColor(RgbColor& color, const uint8_t x, const uint8_t y)
	{
		const size_t offset = (((size_t)y * width) + x) * sizeof(uint16_t);
		const uint8_t* bitmap = (const uint8_t*)RgbBitmap;
		const uint16_t value = (uint16_t)pgm_read_byte(&bitmap[offset + 0]) | ((uint16_t)pgm_read_byte(&bitmap[offset + 1]) << 8);

		color.r = (value & 0xF800) >> 8;
		color.g = (value & 0x07E0) >> 3;
		color.b = (value & 0x1F) << 3;

		return true;
	}
};
#else
template<const uint8_t width,
	const uint8_t height>
class FlashBitmapRgb888Sprite : public BitmapRgb888Sprite<width, height>
{
public:
	FlashBitmapRgb888Sprite(const uint32_t* rgbBitmap = nullptr)
		: BitmapRgb888Sprite<width, height>(rgbBitmap)
	{}
};

template<const uint8_t width,
	const uint8_t height>
class FlashBitmapRgb565Sprite : public BitmapRgb565Sprite<width, height>
{
public:
	FlashBitmapRgb565Sprite(const uint16_t* rgbBitmap = nullptr)
		: BitmapRgb565Sprite<width, height>(rgbBitmap)
	{}
};
#endif
#endif