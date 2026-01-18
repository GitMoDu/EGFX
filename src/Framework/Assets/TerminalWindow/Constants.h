#ifndef _EGFX_FRAMEWORK_ASSETS_TERMINAL_WINDOW_CONSTANTS_h
#define _EGFX_FRAMEWORK_ASSETS_TERMINAL_WINDOW_CONSTANTS_h

#include <EgfxDrawer.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace TerminalWindow
			{
				namespace Definitions
				{
					using DefaultFont = BitmaskFonts::Plastic::FontType3x5;
					using DefaultFontDrawer = BitmaskFont::TemplateFontDrawer<DefaultFont>;
				}
			}
		}
	}
}
#endif