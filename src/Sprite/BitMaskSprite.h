// BitMaskSprite.h

#ifndef _EGFX_BIT_MASK_SPRITE_h
#define _EGFX_BIT_MASK_SPRITE_h

#include "../Model/ISprite.h"

#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

namespace Egfx
{
	template<const pixel_t width,
		const pixel_t height>
	class AbstractBitMaskSprite : public AbstractSprite<width, height>
	{
	private:
		using Base = AbstractSprite<width, height>;

	protected:
		static constexpr uint8_t BitScale = ((width - 1) / 8) + 1;

	protected:
		const uint8_t* Mask = nullptr;

	public:
		AbstractBitMaskSprite(const uint8_t* mask = nullptr)
			: Base()
			, Mask(mask)
		{
		}

		void SetMask(const uint8_t* mask)
		{
			Mask = mask;
		}

		pixel_t GetWidth() const final
		{
			return (Mask != nullptr) * Base::GetWidth();
		}

		pixel_t GetHeight() const final
		{
			return (Mask != nullptr) * Base::GetHeight();
		}
	};

	template<const pixel_t Width,
		const pixel_t Height>
	class BitMaskSprite : public AbstractBitMaskSprite<Width, Height>
	{
	private:
		using Base = AbstractBitMaskSprite<Width, Height>;

	protected:
		using Base::Mask;
		using Base::BitScale;

	public:
		BitMaskSprite(const uint8_t* mask = nullptr)
			: Base(mask)
		{
		}

		virtual bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t yByte = (pixel_index_t)y * BitScale;
			const pixel_t xByte = (x / 8);
			const pixel_index_t offset = yByte + xByte;

			const uint8_t xBit = 7 - (x % 8);

			if (((Mask[offset] >> xBit) & 0x01) != 0)
			{
				color = Rgb::Color(INT8_MAX, INT8_MAX, INT8_MAX);

				return true;
			}
			else
			{
				return false;
			}
		}
	};

	template<const pixel_t Width,
		const pixel_t Height>
	class BitMaskBufferSprite : public BitMaskSprite<Width, Height>
	{
	private:
		using Base = BitMaskSprite<Width, Height>;

	protected:
		using Base::BitScale;

	private:
		static constexpr pixel_index_t MaskSize = ((pixel_index_t)Width * Height) / 8;

		uint8_t Mask[MaskSize]{};

	public:
		BitMaskBufferSprite()
			: Base(Mask)
		{
		}

		void SetAlpha(const pixel_t x, const pixel_t y, const bool value)
		{
			const pixel_index_t yByte = (pixel_index_t)y * BitScale;
			const pixel_t xByte = (x / 8);
			const pixel_index_t offset = yByte + xByte;

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
	template<const pixel_t Width,
		const pixel_t Height>
	class FlashBitMaskSprite : public AbstractBitMaskSprite<Width, Height>
	{
	private:
		using Base = AbstractBitMaskSprite<Width, Height>;

	private:
		using Base::Mask;
		using Base::BitScale;

	public:
		FlashBitMaskSprite(const uint8_t* mask = nullptr)
			: Base(mask)
		{
		}

		virtual bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t yByte = (pixel_index_t)y * BitScale;
			const pixel_t xByte = (x / 8);
			const pixel_index_t offset = yByte + xByte;

			const uint8_t xBit = 7 - (x % 8);

			if (((pgm_read_byte(&Mask[offset]) >> xBit) & 0x01) != 0)
			{
				color = Rgb::Color(INT8_MAX, INT8_MAX, INT8_MAX);
				return true;
			}
			else
			{
				return false;
			}
		}
	};
#else
	template<const pixel_t Width,
		const pixel_t Height>
	using FlashBitMaskSprite = BitMaskSprite<Width, Height>;
#endif
}
#endif