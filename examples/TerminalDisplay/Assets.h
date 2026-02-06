#ifndef _ASSETS_h
#define _ASSETS_h

#include <EgfxFramework.h>
#include <EgfxModules.h>

namespace Assets
{
	namespace SerialTextAssets
	{
		using namespace Egfx;

		static constexpr uint8_t Kerning = 1;
		static constexpr uint8_t LineSpacing = 1;

		template<typename ParentLayout>
		struct DefaultFontConfig
		{
			using Font = Framework::Assets::Font::Bitmask::Plastic::FontType3x5;

			using Drawer = Framework::Bitmask::Font::TemplateDrawer<Font>;

			static constexpr pixel_t FontWidth = Font::Width;
			static constexpr pixel_t FontHeight = Font::Height;
		};

		template<typename ParentLayout>
		struct FontScale
		{
			static constexpr uint8_t GetScale()
			{
				return 1 + (GetDimension() / (128 + 64));
			}

			static constexpr pixel_t GetDimension()
			{
				return MinValue(ParentLayout::Width(), ParentLayout::Height());
			}
		};

		template<typename ParentLayout>
		struct ScaledFontConfig
		{
			using Font = Framework::Assets::Font::Bitmask::Plastic::FontType5x5;

			static constexpr uint8_t ScaleX = FontScale<ParentLayout>::GetScale() / 2;
			static constexpr uint8_t ScaleY = FontScale<ParentLayout>::GetScale();

			using Drawer = Framework::Bitmask::Font::TemplateDrawer<Font, ScaleX, ScaleY>;

			static constexpr pixel_t FontWidth = Font::Width * ScaleX;
			static constexpr pixel_t FontHeight = Font::Height * ScaleY;
		};

		template<typename ParentLayout, bool UseScaled>
		struct FontSelector;

		template<typename ParentLayout>
		struct FontSelector<ParentLayout, true> : ScaledFontConfig<ParentLayout> {};

		template<typename ParentLayout>
		struct FontSelector<ParentLayout, false> : DefaultFontConfig<ParentLayout> {};

		template<typename ParentLayout>
		using FontConfig = FontSelector<ParentLayout, (FontScale<ParentLayout>::GetScale() > 1)>;
	}

	template<typename ParentLayout>
	using SerialTextDrawableType = Egfx::Modules::TerminalWindow::Drawable::SerialText<
		ParentLayout,
		typename SerialTextAssets::FontConfig<ParentLayout>::Drawer,
		SerialTextAssets::FontConfig<ParentLayout>::FontWidth,
		SerialTextAssets::FontConfig<ParentLayout>::FontHeight,
		SerialTextAssets::Kerning,
		SerialTextAssets::LineSpacing>;

	template<typename ParentLayout>
	using SerialTextViewType = Egfx::Modules::TerminalWindow::View::SerialText<
		ParentLayout,
		typename SerialTextAssets::FontConfig<ParentLayout>::Drawer,
		SerialTextAssets::FontConfig<ParentLayout>::FontWidth,
		SerialTextAssets::FontConfig<ParentLayout>::FontHeight,
		SerialTextAssets::Kerning,
		SerialTextAssets::LineSpacing>;
}

#endif