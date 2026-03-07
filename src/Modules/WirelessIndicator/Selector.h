#ifndef _EGFX_MODULES_WIRELESS_INDICATOR_SELECTOR_h
#define _EGFX_MODULES_WIRELESS_INDICATOR_SELECTOR_h

#include "Constant.h"

namespace Egfx
{
	namespace Modules
	{
		namespace WirelessIndicator
		{
			// Compile-time helper for layouts and drawables.
			namespace Selector
			{
				template<typename ParentLayout, bool Monochrome>
				struct BaseSelector
				{
					static constexpr bool UseBitmask() { return ParentLayout::Width() > 18 && ParentLayout::Height() > 15; }
					static constexpr bool UseTallBitmask() { return ParentLayout::Height() > 24 && ParentLayout::Width() > 16; }
					static constexpr bool UseMediumBitmask() { return ParentLayout::Width() > 22; }
					static constexpr bool UseWidePrimitive() { return ParentLayout::Width() > 7; }
					static constexpr bool UseTallPrimitive() { return ParentLayout::Height() > 8; }
					static constexpr bool UseHighPrimitive() { return ParentLayout::Height() > 6 && ParentLayout::Width() > 9; }
				};
			}
		}
	}
}
#endif