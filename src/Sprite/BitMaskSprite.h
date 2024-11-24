// BitMaskSprite.h

#ifndef _BIT_MASK_SPRITE_h
#define _BIT_MASK_SPRITE_h

#include "../Model/ISprite.h"

#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

template<const uint8_t width,
	const uint8_t height>
class AbstractBitMaskSprite : public AbstractSprite<width, height>
{
protected:
	static constexpr uint8_t BitScale = ((width - 1) / 8) + 1;

protected:
	const uint8_t* Mask = nullptr;

public:
	AbstractBitMaskSprite(const uint8_t* mask = nullptr)
		: AbstractSprite<width, height>()
		, Mask(mask)
	{
	}

	void SetMask(const uint8_t* mask)
	{
		Mask = mask;
	}

	const uint8_t GetWidth() const
	{
		return (Mask != nullptr) * width;
	}

	const uint8_t GetHeight() const
	{
		return (Mask != nullptr) * height;
	}
};

template<const uint8_t Width,
	const uint8_t Height>
class BitMaskSprite : public AbstractBitMaskSprite<Width, Height>
{
private:
	using BaseClass = AbstractBitMaskSprite<Width, Height>;

protected:
	using BaseClass::Mask;
	using BaseClass::BitScale;

public:
	BitMaskSprite(const uint8_t* mask = nullptr)
		: BaseClass(mask)
	{
	}

	virtual const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
	{
		const uint_fast16_t yByte = (uint_fast16_t)y * BitScale;
		const uint8_t xByte = (x / 8);
		const uint_fast16_t offset = yByte + xByte;

		const uint8_t xBit = 7 - (x % 8);

		if (((Mask[offset] >> xBit) & 0x01) != 0)
		{
			color.r = INT8_MAX;
			color.g = INT8_MAX;
			color.b = INT8_MAX;
			return true;
		}
		else
		{
			return false;
		}
	}
};

template<const uint8_t Width,
	const uint8_t Height>
class BitMaskBufferSprite : public BitMaskSprite<Width, Height>
{
private:
	using BaseClass = BitMaskSprite<Width, Height>;

protected:
	using BaseClass::BitScale;

private:
	static constexpr size_t MaskSize = ((size_t)Width * Height) / 8;

	uint8_t Mask[MaskSize]{};

public:
	BitMaskBufferSprite()
		: BaseClass(Mask)
	{
	}

	void SetAlpha(const uint8_t x, const uint8_t y, const bool value)
	{
		const uint_fast16_t yByte = (uint_fast16_t)y * BitScale;
		const uint8_t xByte = (x / 8);
		const uint_fast16_t offset = yByte + xByte;

		const uint8_t xBit = 7 - (x % 8);

		if (value)
		{
			Mask[offset] |= (1 << xBit);
		}
		else
		{
			Mask[offset] &= ~(1 << xBit);
		}
	}
};



#if defined(ARDUINO_ARCH_AVR)
template<const uint8_t Width,
	const uint8_t Height>
class FlashBitMaskSprite : public AbstractBitMaskSprite<Width, Height>
{
private:
	using BaseClass = AbstractBitMaskSprite<Width, Height>;

private:
	using BaseClass::Mask;
	using BaseClass::BitScale;

public:
	FlashBitMaskSprite(const uint8_t* mask = nullptr)
		: BaseClass(mask)
	{
	}

	virtual const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
	{
		const uint_fast16_t yByte = (uint_fast16_t)y * BitScale;
		const uint8_t xByte = (x / 8);
		const uint_fast16_t offset = yByte + xByte;

		const uint8_t xBit = 7 - (x % 8);

		if (((pgm_read_byte(&Mask[offset]) >> xBit) & 0x01) != 0)
		{
			color.r = INT8_MAX;
			color.g = INT8_MAX;
			color.b = INT8_MAX;
			return true;
		}
		else
		{
			return false;
		}
	}
};
#else
template<const uint8_t Width,
	const uint8_t Height>
class FlashBitMaskSprite : public BitMaskSprite<Width, Height>
{
public:
	FlashBitMaskSprite(const uint8_t* mask = nullptr)
		: BitMaskSprite<Width, Height>(mask)
	{
	}
};
#endif
#endif