#ifndef _EGFX_FONT_TEXT_h
#define _EGFX_FONT_TEXT_h

#include "../Platform/Platform.h"
#include "../Model/AsciiDefinition.h"

namespace Egfx
{
	namespace FontText
	{
		enum class TextAlignmentEnum : uint8_t
		{
			Left,
			Center,
			Right
		};

		static constexpr uint32_t PowerOfTen[]
		{
			1,
			10,
			100,
			1000,
			10000,
			100000,
			1000000,
			10000000,
			100000000,
			1000000000
		};

		struct DefaultLayout
		{
			static constexpr pixel_t X() { return 0; }
			static constexpr pixel_t Y() { return 0; }
			static constexpr pixel_t Width() { return 1; }
			static constexpr pixel_t Height() { return 1; }
		};
	}
}
#endif