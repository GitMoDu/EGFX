// LayoutElement.h

#ifndef _LAYOUT_ELEMENT_h
#define _LAYOUT_ELEMENT_h

template<const uint8_t x, const uint8_t y,
	const uint8_t width, const uint8_t height>
struct LayoutElement
{
	static constexpr uint8_t X() { return x; }
	static constexpr uint8_t Y() { return y; }

	static constexpr uint8_t Width() { return width; }
	static constexpr uint8_t Height() { return height; }
};

template<const uint8_t x, const uint8_t y,
	const uint8_t width, const uint8_t height,
	const uint8_t fontWidth, const  uint8_t fontHeight,
	const uint8_t fontKerning>
struct FontLayoutElement : public LayoutElement<x, y, width, height>
{
	static constexpr uint8_t FontWidth() { return fontWidth; }
	static constexpr uint8_t FontHeight() { return fontHeight; }
	static constexpr uint8_t FontKerning() { return fontKerning; }
};
#endif