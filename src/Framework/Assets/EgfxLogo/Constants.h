#ifndef _EGFX_FRAMEWORK_ASSETS_EGFX_LOGO_CONSTANTS_h
#define _EGFX_FRAMEWORK_ASSETS_EGFX_LOGO_CONSTANTS_h

#include <EgfxDrawer.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace EgfxLogo
			{
				namespace Colors
				{
					static constexpr rgb_color_t Background = RGB_COLOR_BLACK;
					static constexpr rgb_color_t Foreground = RGB_COLOR_WHITE;

					// Palette colors
					static constexpr rgb_color_t Color1 = Rgb::Color(uint32_t(0xA80F21));
					static constexpr rgb_color_t Color2 = Rgb::Color(uint32_t(0xA2D300));
					static constexpr rgb_color_t Color3 = Rgb::Color(uint32_t(0x004AB2));
				}

				namespace Dimensions
				{
					static constexpr uint8_t LogoMinHeight() { return 16; }

					static constexpr uint8_t LetterCount() { return 4; }
					static constexpr uint8_t KerningRatio() { return 12; }
					static constexpr uint8_t LogoAspectWidth() { return 22; }
					static constexpr uint8_t LogoAspectHeight() { return 9; }
				}
			}
		}
	}
}
#endif