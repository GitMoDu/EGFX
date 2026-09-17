#ifndef _EGFX_MODULES_FPS_DISPLAY_CONSTANT_h
#define _EGFX_MODULES_FPS_DISPLAY_CONSTANT_h

#include "../../EgfxFramework.h"
#include "../../Modules/Kit/Include.h"

namespace Egfx
{
	namespace Modules
	{
		namespace FpsDisplay
		{
			namespace Definitions
			{
				static constexpr pixel_t PixelsPerScale = 96;

				template<typename Layout>
				struct TextScale
				{
				private:
					static constexpr pixel_t LayoutDimension = MinValue(Layout::Width(), Layout::Height());

				public:
					static constexpr uint8_t Value = MaxValue<uint8_t>(1, static_cast<uint8_t>(LayoutDimension / PixelsPerScale));
				};
			}
		}
	}
}
#endif