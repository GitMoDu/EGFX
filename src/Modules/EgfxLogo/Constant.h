#ifndef _EGFX_MODULES_EGFX_LOGO_CONSTANT_h
#define _EGFX_MODULES_EGFX_LOGO_CONSTANT_h

#include <EgfxFramework.h>
#include <EgfxAssets.h>

namespace Egfx
{
	namespace Modules
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
				using dimension_t = pixel_t;

				// Code-font definitions.
				static constexpr uint8_t ColumnCount = 7;
				static constexpr uint8_t LineCount = 5;

				// Layout dimensions.
				static constexpr uint8_t LetterCount() { return 4; }
				static constexpr uint8_t KerningRatio() { return 12+2; }
				static constexpr uint8_t LogoAspectWidth() { return UINT8_MAX; }
				static constexpr uint8_t LogoAspectHeight() { return 100; }
				static constexpr uint8_t FontLayoutMargin() { return 1; }

				// Screen outline shimmering effect. Controls the max Screen effect alpha.
				static constexpr ufraction8_t ScreenAlphaScale = UFraction8::GetScalar<uint8_t>(9, 10);
			}
		}
	}
}
#endif