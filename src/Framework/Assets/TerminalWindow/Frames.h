#ifndef _EGFX_FRAMEWORK_ASSETS_TERMINAL_WINDOW_FRAMES_h
#define _EGFX_FRAMEWORK_ASSETS_TERMINAL_WINDOW_FRAMES_h

#include "Views.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace TerminalWindow
			{
				namespace Frames
				{
					template<typename Layout
						, typename FontDrawerType = Definitions::DefaultFontDrawer
						, pixel_t FontWidth = Definitions::DefaultFont::Width
						, pixel_t FontHeight = Definitions::DefaultFont::Height
						, uint8_t Kerning = 1
						, uint8_t LineSpacing = 1>
					using Frame = View::FrameAdapter<Views::SerialText<Layout, FontDrawerType, FontWidth, FontHeight, Kerning, LineSpacing>>;
				}
			}
		}
	}
}
#endif