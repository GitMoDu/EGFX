#ifndef _EGFX_FRAMEWORK_ASSETS_TERMINAL_WINDOW_LAYOUT_h
#define _EGFX_FRAMEWORK_ASSETS_TERMINAL_WINDOW_LAYOUT_h

#include "Constants.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace TerminalWindow
			{
				template<typename ParentLayout>
				struct Layout
				{
					static constexpr pixel_t X() { return ParentLayout::X(); }
					static constexpr pixel_t Y() { return ParentLayout::Y(); }
					static constexpr pixel_t Width() { return ParentLayout::Width(); }
					static constexpr pixel_t Height() { return ParentLayout::Height(); }
				};
			}
		}
	}
}
#endif