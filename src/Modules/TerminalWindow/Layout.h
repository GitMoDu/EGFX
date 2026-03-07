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
					static constexpr int16_t X() { return ParentLayout::X(); }
					static constexpr int16_t Y() { return ParentLayout::Y(); }
					static constexpr int16_t Width() { return ParentLayout::Width(); }
					static constexpr int16_t Height() { return ParentLayout::Height(); }
				};
			}
		}
	}
}
#endif