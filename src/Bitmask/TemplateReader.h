#ifndef _EGFX_BITMASK_TEMPLATE_READER_h
#define _EGFX_BITMASK_TEMPLATE_READER_h

#include "../Platform/Platform.h"

namespace Egfx
{
	namespace Bitmask
	{
		template<typename dimension_t, dimension_t width>
		struct TemplateReader
		{
			using intermediate_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;

			static constexpr intermediate_t BitScale = (intermediate_t)width / 8 + (((intermediate_t)width % 8) != 0 ? 1 : 0);

			static bool IsPixelSet(const uint8_t* mask, const dimension_t x, const dimension_t y)
			{
				const intermediate_t yByte = static_cast<intermediate_t>(y) * BitScale;
				const dimension_t xByte = SignedRightShift(x, 3);
				const intermediate_t offset = yByte + xByte;

				const uint8_t xBit = 7 - (x % 8);

#if defined(ARDUINO_ARCH_AVR)
				if (((pgm_read_byte(&mask[offset]) >> xBit) & 0x01) != 0)
#else
				if (((mask[offset] >> xBit) & 0x01) != 0)
#endif
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		};
	}
}
#endif