#ifndef _EGFX_MODULES_TERMINAL_WINDOW_LAYOUT_h
#define _EGFX_MODULES_TERMINAL_WINDOW_LAYOUT_h

#include "Constant.h"

namespace Egfx
{
	namespace Modules
	{
		namespace TerminalWindow
		{
			namespace Layout
			{
				template<typename ParentLayout>
				struct TerminalLayout
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