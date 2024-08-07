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
	{}

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

private:
	using BaseClass::Mask;
	using BaseClass::BitScale;

public:
	BitMaskSprite(const uint8_t* mask = nullptr)
		: BaseClass(mask)
	{}

	const bool Get(RgbColor& color, const uint8_t x, const uint8_t y) final
	{
		if (GetColor(color, x, y))
		{
			const uint8_t yByte = y * BitScale;
			const uint8_t xByte = (x / 8);
			const uint8_t offset = yByte + xByte;

			const uint8_t xBit = 7 - (x % 8);

			return ((Mask[offset] >> xBit) & 0x01) != 0;
		}

		return false;
	}

protected:
	virtual const bool GetColor(RgbColor& color, const uint8_t x, const uint8_t y)
	{
		color.r = UINT8_MAX;
		color.g = UINT8_MAX;
		color.b = UINT8_MAX;

		return true;
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
	{}

	const bool Get(RgbColor& color, const uint8_t x, const uint8_t y) final
	{
		if (GetColor(color, x, y))
		{
			const uint8_t yByte = y * BitScale;
			const uint8_t xByte = (x / 8);
			const uint8_t offset = yByte + xByte;

			const uint8_t xBit = 7 - (x % 8);

			return ((pgm_read_byte(&Mask[offset]) >> xBit) & 0x01) != 0;
		}

		return false;
	}

protected:
	virtual const bool GetColor(RgbColor& color, const uint8_t x, const uint8_t y)
	{
		color.r = UINT8_MAX;
		color.g = UINT8_MAX;
		color.b = UINT8_MAX;

		return true;
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
	{}
};
#endif
#endif