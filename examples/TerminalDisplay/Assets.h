#ifndef _ASSETS_h
#define _ASSETS_h

#include "DisplayPrint.h"

namespace Assets
{
	using namespace Egfx;

	static constexpr uint8_t GetScaleX(const pixel_t width)
	{
		return 1 + (width / (128 + 32));
	}

	static constexpr uint8_t GetScaleY(const pixel_t width)
	{
		return 1 + (width / 128);
	}

	template<typename Layout>
	using TextDisplayScaledType = Egfx::TextDisplaySerial<Layout
		, Egfx::BitmaskFont::TemplateColorScaledFontDrawer<Egfx::BitmaskFonts::Plastic::FontType3x5, Egfx::FontText::FullColorSource, GetScaleX(Layout::Width()), GetScaleY(Layout::Width())>
		, Egfx::BitmaskFonts::Plastic::FontType3x5::Width* GetScaleX(Layout::Width())
		, Egfx::BitmaskFonts::Plastic::FontType3x5::Height* GetScaleY(Layout::Width())>;

	template<typename Layout>
	using TextDisplayType = Egfx::TextDisplaySerial<Layout
		, typename Egfx::BitmaskFont::TemplateColorFontDrawer<Egfx::BitmaskFonts::Plastic::FontType3x5, Egfx::FontText::FullColorSource>
		, Egfx::BitmaskFonts::Plastic::FontType3x5::Width
		, Egfx::BitmaskFonts::Plastic::FontType3x5::Height>;

	template<typename Layout, bool UseScaled>
	struct TextDisplaySelector;

	template<typename Layout>
	struct TextDisplaySelector<Layout, true>
	{
		using type = TextDisplayScaledType<Layout>;
	};

	template<typename Layout>
	struct TextDisplaySelector<Layout, false>
	{
		using type = TextDisplayType<Layout>;
	};

	template<typename Layout>
	using TextDisplayAutoType = typename TextDisplaySelector<Layout, (GetScaleX(Layout::Width()) > 1)>::type;
}

#endif