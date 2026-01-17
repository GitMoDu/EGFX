#ifndef _EGFX_FRAMEWORK_ASSETS_FPS_DISPLAY_CONSTANTS_h
#define _EGFX_FRAMEWORK_ASSETS_FPS_DISPLAY_CONSTANTS_h

#include <EgfxDrawer.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace FpsDisplay
			{
				namespace Definitions
				{
					using DefaultFont = BitmaskFonts::Plastic::FontType3x5;
					using LargeFont = BitmaskFonts::Plastic::FontType5x5;
					using DefaultFontDrawer = BitmaskFont::TemplateFontDrawer<DefaultFont>;

					namespace Detail
					{
						template<bool UseScaled, typename TrueType, typename FalseType>
						struct SelectFontType
						{
							using type = TrueType;
						};

						template<typename TrueType, typename FalseType>
						struct SelectFontType<false, TrueType, FalseType>
						{
							using type = FalseType;
						};
					}

					template<typename Layout>
					struct AutoFontSelector
					{
					private:
						static constexpr pixel_t Dimension = (Layout::Width() < Layout::Height()) ? Layout::Width() : Layout::Height();
						static constexpr uint8_t Scale = 1 + (Dimension / 128);
						static constexpr bool UseScaled = (Scale > 1);

					public:
						static constexpr uint8_t FontScaleX = Scale / 2;
						static constexpr uint8_t FontScaleY = Scale;
						using BaseDrawer = BitmaskFont::TemplateFontDrawer<DefaultFont>;
						using ScaledDrawer = BitmaskFont::TemplateScaledFontDrawer<LargeFont, FontScaleX, FontScaleY>;
						using Drawer = typename Detail::SelectFontType<UseScaled, ScaledDrawer, BaseDrawer>::type;
						static constexpr pixel_t FontWidth = DefaultFont::Width * (UseScaled ? FontScaleX : 1);
						static constexpr pixel_t FontHeight = DefaultFont::Height * (UseScaled ? FontScaleY : 1);
					};
				}

				namespace Strings
				{
					static constexpr char LabelText[] PROGMEM = "FPS";
				}
			}
		}
	}
}
#endif