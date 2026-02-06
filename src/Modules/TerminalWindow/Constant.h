
#ifndef _EGFX_MODULES_TERMINAL_WINDOW_CONSTANT_h
#define _EGFX_MODULES_TERMINAL_WINDOW_CONSTANT_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Modules
	{
		namespace TerminalWindow
		{
			namespace Definitions
			{
				using DefaultFont = Framework::Assets::Font::Bitmask::Plastic::FontType3x5;
				using DefaultFontDrawer = Framework::Bitmask::Font::TemplateDrawer<DefaultFont>;
			}

			namespace Durations
			{
				static constexpr uint32_t CursorBlinkPeriod = 400000; // Cursor blink period in microseconds.
			}

		}
	}
}
#endif