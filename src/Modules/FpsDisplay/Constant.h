#ifndef _EGFX_MODULES_FPS_DISPLAY_CONSTANT_h
#define _EGFX_MODULES_FPS_DISPLAY_CONSTANT_h

#include <EgfxFramework.h>
#include <EgfxAssets.h>

namespace Egfx
{
	namespace Modules
	{
		namespace FpsDisplay
		{
			namespace Definitions
			{
				template<typename ParentLayout>
				struct AutoFontSelector
				{
				private:
					static constexpr pixel_t Dimension = MinValue(ParentLayout::Width(), ParentLayout::Height());
					static constexpr ufraction16_t FontWidthProportion = UFraction16::GetScalar<uint8_t>(5, 7);

				public:
					static constexpr pixel_t FontHeight = MaxValue<pixel_t>(9, Dimension / 10) | 0b1;
					static constexpr pixel_t FontWidth = UFraction16::Fraction(FontWidthProportion, FontHeight);

					using Drawer = Framework::Assets::Font::Code::RawBot<FontWidth, FontHeight>;
				};
			}

			namespace Strings
			{
				static constexpr char LabelText[] PROGMEM = "FPS";
			}
		}
	}
}
#endif