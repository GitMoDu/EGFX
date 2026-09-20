#ifndef _INTEGERGLASS_MODULES_FPS_DISPLAY_LAYOUT_h
#define _INTEGERGLASS_MODULES_FPS_DISPLAY_LAYOUT_h

#include "Constant.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace FpsDisplay
		{
			/// <summary>
			/// Specifies the corner position for the FPS overlay on the display.
			/// </summary>
			enum class FpsDrawerPosition
			{
				TopLeft,
				TopRight,
				BottomLeft,
				BottomRight
			};
		}
	}
}
#endif