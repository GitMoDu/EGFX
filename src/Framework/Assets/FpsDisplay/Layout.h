#ifndef _EGFX_FRAMEWORK_ASSETS_FPS_DISPLAY_LAYOUT_h
#define _EGFX_FRAMEWORK_ASSETS_FPS_DISPLAY_LAYOUT_h

#include <EgfxDrawer.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
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
}
#endif