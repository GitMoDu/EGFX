#ifndef _EGFX_FRAMEWORK_BITMASK_MODEL_h
#define _EGFX_FRAMEWORK_BITMASK_MODEL_h

#include "../../EgfxCore.h"

namespace Egfx
{
	namespace Framework
	{
		/// <summary>
		/// Bitmask graphics model.
		///
		/// Bitmasks are 1-bit-per-pixel images stored as packed bytes. Provides basic
		/// constants and readers for accessing bitmask data from RAM or flash/PROGMEM.
		/// </summary>
		namespace Bitmask
		{
			/// <summary>Number of bits stored in a single byte of bitmask data.</summary>
			static constexpr uint8_t BitsPerByte = 8;

			namespace Reader
			{
				/// <summary>
				/// Bitmask byte reader for data stored in RAM.
				/// </summary>
				struct Ram
				{
					/// <summary>
					/// Reads the packed byte at (xByte, y).
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="BytesPerLine">Byte width of a scanline (packed pixels).</typeparam>
					/// <param name="mask">Pointer to packed bitmask bytes.</param>
					/// <param name="xByte">Byte index on the X axis (packed pixels).</param>
					/// <param name="y">Row index on the Y axis.</param>
					/// <returns>The packed bitmask byte at the requested position.</returns>
					template<typename dimension_t, dimension_t BytesPerLine>
					static uint8_t ReadByteAt(const uint8_t* mask, const dimension_t xByte, const dimension_t y)
					{
						using index_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;

						const index_t offset = (static_cast<index_t>(y) * BytesPerLine) + xByte;

						return mask[offset];
					}
				};

				/// <summary>
				/// Bitmask byte reader for data stored in flash/PROGMEM on AVR.
				/// On non-AVR targets, this behaves like the RAM reader.
				/// </summary>
				struct Flash
				{
					/// <summary>
					/// Reads the packed byte at (xByte, y).
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="BytesPerLine">Byte width of a scanline (packed pixels).</typeparam>
					/// <param name="mask">Pointer to packed bitmask bytes.</param>
					/// <param name="xByte">Byte index on the X axis (packed pixels).</param>
					/// <param name="y">Row index on the Y axis.</param>
					/// <returns>The packed bitmask byte at the requested position.</returns>
					template<typename dimension_t, dimension_t BytesPerLine>
					static uint8_t ReadByteAt(const uint8_t* mask, const dimension_t xByte, const dimension_t y)
					{
						using index_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;

						const index_t offset = (static_cast<index_t>(y) * BytesPerLine) + xByte;

#if defined(ARDUINO_ARCH_AVR)
						return pgm_read_byte(&mask[offset]);
#else
						// On non-AVR, treat as RAM.
						return mask[offset];
#endif
					}
				};
			}
		}
	}
}
#endif