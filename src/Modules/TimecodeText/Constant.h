#ifndef _LOLA_GRAPHICS_MODULES_TIMECODE_TEXT_CONSTANT_h
#define _LOLA_GRAPHICS_MODULES_TIMECODE_TEXT_CONSTANT_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Modules
	{
		namespace TimecodeText
		{
			namespace Dimensions
			{
				static constexpr uint8_t TimeDigitCount = 6;
				static constexpr uint8_t TimeSeparatorCount = 2;
				static constexpr uint8_t TimeElementCount = TimeDigitCount + TimeSeparatorCount;
				static constexpr uint8_t DigitLayoutWeight = 5;
				static constexpr uint8_t KerningLayoutWeight = 2;
				static constexpr uint8_t SeparatorLayoutWeight = 1;
			}

			namespace Texts
			{
				static constexpr char UnknownChar = '-';
			}

			namespace Durations
			{
				static constexpr uint32_t MaxDurationSeconds = (static_cast<uint32_t>(99) * 3600) + ((static_cast<uint32_t>(59) * 60) + 59);
				static constexpr uint32_t MaxDurationMilliseconds = (static_cast<uint32_t>(99) * 3600000) + ((static_cast<uint32_t>(59) * 60000) + (59 * 1000) + 999);
			}

			enum class PresentModeEnum : uint8_t
			{
				Invisible,
				NoDuration,
				Duration,
				EnumCount
			};
		}
	}
}
#endif