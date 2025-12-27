#ifndef _LAYOUT_ELEMENT_h
#define _LAYOUT_ELEMENT_h

namespace Egfx
{
	template<const pixel_t x, const pixel_t y,
		const pixel_t width, const pixel_t height>
	struct LayoutElement
	{
		static constexpr pixel_t X() { return x; }
		static constexpr pixel_t Y() { return y; }

		static constexpr pixel_t Width() { return width; }
		static constexpr pixel_t Height() { return height; }

		static constexpr pixel_t Smallest()
		{
			return MinValue(width, height);
		}

		static constexpr pixel_t Largest()
		{
			return MaxValue(width, height);
		}
	};

	template<const pixel_t x, const pixel_t y,
		const pixel_t width, const pixel_t height,
		const pixel_t fontWidth, const pixel_t fontHeight,
		const uint8_t fontKerning>
	struct FontLayoutElement : public LayoutElement<x, y, width, height>
	{
		static constexpr pixel_t FontWidth() { return fontWidth; }
		static constexpr pixel_t FontHeight() { return fontHeight; }
		static constexpr uint8_t FontKerning() { return fontKerning; }
	};
}
#endif