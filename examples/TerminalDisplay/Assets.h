#ifndef _ASSETS_h
#define _ASSETS_h

#include <EgfxDrawer.h>

namespace Assets
{
	using namespace Egfx;

	namespace SerialTextAssets
	{
		static constexpr uint8_t Kerning = 1;
		static constexpr uint8_t LineSpacing = 1;

		template<typename Layout>
		struct DefaultFontConfig
		{
			using Font = BitmaskFonts::Plastic::FontType3x5;
			using Drawer = BitmaskFont::TemplateColorFontDrawer<Font, FontText::FullColorSource>;
			static constexpr pixel_t FontWidth = Font::Width;
			static constexpr pixel_t FontHeight = Font::Height;
		};

		template<typename Layout>
		struct FontScale
		{
			static constexpr uint8_t GetScale()
			{
				return 1 + (GetDimension() / (128));
			}

			static constexpr pixel_t GetDimension()
			{
				return MinValue(Layout::Width(), Layout::Height());
			}
		};

		template<typename Layout>
		struct ScaledFontConfig
		{
			using Font = BitmaskFonts::Plastic::FontType3x5;
			static constexpr uint8_t ScaleX = FontScale<Layout>::GetScale();
			static constexpr uint8_t ScaleY = FontScale<Layout>::GetScale();
			using Drawer = BitmaskFont::TemplateColorScaledFontDrawer<Font, FontText::FullColorSource, ScaleX, ScaleY>;
			static constexpr pixel_t FontWidth = Font::Width * ScaleX;
			static constexpr pixel_t FontHeight = Font::Height * ScaleY;
		};

		template<typename Layout, bool UseScaled>
		struct FontSelector;

		template<typename Layout>
		struct FontSelector<Layout, true> : ScaledFontConfig<Layout> {};

		template<typename Layout>
		struct FontSelector<Layout, false> : DefaultFontConfig<Layout> {};

		template<typename Layout>
		using FontConfig = FontSelector<Layout, (FontScale<Layout>::GetScale() > 1)>;
	}

	template<typename Layout>
	using SerialTextDrawableType = Framework::Widgets::SerialText::Drawables::SerialText<
		Layout,
		typename SerialTextAssets::FontConfig<Layout>::Drawer,
		SerialTextAssets::FontConfig<Layout>::FontWidth,
		SerialTextAssets::FontConfig<Layout>::FontHeight,
		SerialTextAssets::Kerning,
		SerialTextAssets::LineSpacing>;

	template<typename Layout>
	using SerialTextViewType = Framework::Widgets::SerialText::Views::SerialText<
		Layout,
		typename SerialTextAssets::FontConfig<Layout>::Drawer,
		SerialTextAssets::FontConfig<Layout>::FontWidth,
		SerialTextAssets::FontConfig<Layout>::FontHeight,
		SerialTextAssets::Kerning,
		SerialTextAssets::LineSpacing>;

	template<typename Layout>
	using SerialTextFrameType = Framework::Widgets::SerialText::Frame<
		Layout,
		typename SerialTextAssets::FontConfig<Layout>::Drawer,
		SerialTextAssets::FontConfig<Layout>::FontWidth,
		SerialTextAssets::FontConfig<Layout>::FontHeight,
		SerialTextAssets::Kerning,
		SerialTextAssets::LineSpacing>;
}

#endif