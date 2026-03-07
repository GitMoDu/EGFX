#ifndef _EGFX_MODULES_TIMECODE_TEXT_CONSTANT_h
#define _EGFX_MODULES_TIMECODE_TEXT_CONSTANT_h

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
				// HH:MM:SS maximum: 99:59:59
				static constexpr uint32_t MaxDurationSeconds = (static_cast<uint32_t>(99) * 3600)
					+ ((static_cast<uint32_t>(59) * 60) + 59);
				static constexpr uint32_t MaxDurationMilliseconds = MaxDurationSeconds * 1000;

				// MM:SS:cs maximum: 99:59:99 (i.e., 99 minutes, 59 seconds, 990..999 ms)
				static constexpr uint32_t MaxMinutesTimecodeMilliseconds =
					(static_cast<uint32_t>(99) * 60000)
					+ (static_cast<uint32_t>(59) * 1000)
					+ 999;
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