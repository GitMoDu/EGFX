#ifndef _EGFX_BITMAP_SPRITE_h
#define _EGFX_BITMAP_SPRITE_h

#include "../Model/ISprite.h"
#include "../Model/ColorConverter.h"

namespace Egfx
{
	template<typename ColorType,
		const pixel_t width,
		const pixel_t height>
	class AbstractBitmapSprite : public AbstractSprite<width, height>
	{
	private:
		using Base = AbstractSprite<width, height>;

	protected:
		const ColorType* RgbBitmap;

	public:
		AbstractBitmapSprite(const ColorType* rgbBitmap)
			: Base()
			, RgbBitmap(rgbBitmap)
		{
		}

		void SetBitmap(const ColorType* rgbBitmap)
		{
			RgbBitmap = rgbBitmap;
		}

		pixel_t GetWidth() const final
		{
			return (RgbBitmap != nullptr) * Base::GetWidth();
		}

		pixel_t GetHeight() const final
		{
			return (RgbBitmap != nullptr) * Base::GetHeight();
		}
	};

	template<const pixel_t width,
		const pixel_t height>
	class BitmapRgb332Sprite : public AbstractBitmapSprite<uint8_t, width, height>
	{
	private:
		using Base = AbstractBitmapSprite<uint8_t, width, height>;

	protected:
		using Base::RgbBitmap;

	public:
		BitmapRgb332Sprite(const uint8_t* rgbBitmap = nullptr) : Base(rgbBitmap)
		{
		}

		virtual bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = ((pixel_index_t)y * width) + x;

			color = Rgb::Color((uint8_t)RgbBitmap[offset]);

			return true;
		}
	};

	template<const pixel_t width,
		const pixel_t height>
	class BitmapRgb565Sprite : public AbstractBitmapSprite<uint16_t, width, height>
	{
	private:
		using Base = AbstractBitmapSprite<uint16_t, width, height>;

	protected:
		using Base::RgbBitmap;

	public:
		BitmapRgb565Sprite(const uint16_t* rgbBitmap = nullptr) : Base(rgbBitmap)
		{
		}

		virtual bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = ((pixel_index_t)y * width) + x;

			color = Rgb::Color((uint16_t)RgbBitmap[offset]);

			return true;
		}
	};

	template<const pixel_t width,
		const pixel_t height>
	class BitmapRgb888Sprite : public AbstractBitmapSprite<uint32_t, width, height>
	{
	private:
		using Base = AbstractBitmapSprite<uint32_t, width, height>;

	protected:
		using Base::RgbBitmap;

	public:
		BitmapRgb888Sprite(const uint32_t* rgbBitmap = nullptr) : Base(rgbBitmap)
		{
		}

		virtual bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = ((pixel_index_t)y * width) + x;

			color = Rgb::Color((uint32_t)RgbBitmap[offset]);

			return true;
		}
	};

#if defined(ARDUINO_ARCH_AVR)
	template<const pixel_t width,
		const pixel_t height>
	class FlashBitmapRgb332Sprite : public AbstractBitmapSprite<uint8_t, width, height>
	{
	private:
		using Base = AbstractBitmapSprite<uint8_t, width, height>;

	protected:
		using Base::RgbBitmap;

	public:
		FlashBitmapRgb332Sprite(const uint8_t* rgbBitmap = nullptr)
			: Base(rgbBitmap)
		{
		}

		virtual bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = ((pixel_index_t)y * width) + x;
			const uint8_t value = pgm_read_byte(&RgbBitmap[offset]);

			color = Rgb::Color(value);

			return true;
		}
	};

	template<const pixel_t width,
		const pixel_t height>
	class FlashBitmapRgb565Sprite : public AbstractBitmapSprite<uint16_t, width, height>
	{
	private:
		using Base = AbstractBitmapSprite<uint16_t, width, height>;

	protected:
		using Base::RgbBitmap;

	public:
		FlashBitmapRgb565Sprite(const uint16_t* rgbBitmap = nullptr)
			: Base(rgbBitmap)
		{
		}

		virtual bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = (((pixel_index_t)y * width) + x);
			const uint16_t value = (uint16_t)pgm_read_word(&RgbBitmap[offset]);

			color = Rgb::Color(value);

			return true;
		}
	};

	template<const pixel_t width,
		const pixel_t height>
	class FlashBitmapRgb888Sprite : public AbstractBitmapSprite<uint32_t, width, height>
	{
	private:
		using Base = AbstractBitmapSprite<uint32_t, width, height>;

	protected:
		using Base::RgbBitmap;

	public:
		FlashBitmapRgb888Sprite(const uint32_t* rgbBitmap = nullptr)
			: Base(rgbBitmap)
		{
		}

		virtual bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = (((pixel_index_t)y * width) + x) * sizeof(uint32_t);

			const uint32_t value = (uint32_t)pgm_read_dword(&RgbBitmap[offset]);

			color = Rgb::Color(value);

			return true;
		}
	};
#else
	template<const pixel_t width,
		const pixel_t height>
	using FlashBitmapRgb332Sprite = BitmapRgb332Sprite<width, height>;

	template<const pixel_t width,
		const pixel_t height>
	using FlashBitmapRgb565Sprite = BitmapRgb565Sprite<width, height>;

	template<const pixel_t width,
		const pixel_t height>
	using FlashBitmapRgb888Sprite = BitmapRgb888Sprite<width, height>;
#endif
}
#endif