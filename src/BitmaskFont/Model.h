#ifndef _EGFX_BITMASK_FONT_MODEL_h
#define _EGFX_BITMASK_FONT_MODEL_h

#include "../Platform/Platform.h"

namespace Egfx
{
	namespace Bitmask
	{
		namespace Contract
		{
			struct Font
			{
				using dimension_t = uint8_t;
				static constexpr dimension_t Width = 1;
				static constexpr dimension_t Height = 1;

				static const uint8_t* GetBitmask(const char /*printableChar*/)
				{
					return nullptr;
				}
			};
		}
	}

}
#endif