#ifndef _EGFX_FRAMEWORK_BITMAP_MODEL_h
#define _EGFX_FRAMEWORK_BITMAP_MODEL_h

#include "../../EgfxCore.h"
#include "../Bitmask/Model.h"

namespace Egfx
{
	namespace Framework
	{
		/// <summary>
		/// Bitmap graphics model.
		///
		/// A bitmap is stored as a 2D array of pixel values in row-major order.
		/// This header provides reader utilities for accessing bitmap pixels from RAM or flash/PROGMEM.
		/// </summary>
		namespace Bitmap
		{
			namespace Reader
			{
				/// <summary>
				/// Bitmap reader for data stored in RAM.
				/// </summary>
				struct Ram
				{
					/// <summary>
					/// Reads the pixel value at (x, y) from a row-major bitmap stored in RAM.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Bitmap width (pixels).</typeparam>
					/// <typeparam name="bitmap_color_t">Bitmap pixel storage type.</typeparam>
					/// <param name="bitmap">Pointer to bitmap pixels in row-major order.</param>
					/// <param name="x">X coordinate (pixels).</param>
					/// <param name="y">Y coordinate (pixels).</param>
					/// <returns>Pixel value at (x, y).</returns>
					template<typename dimension_t,
						dimension_t Width,
						typename bitmap_color_t>
					static bitmap_color_t ReadColorAt(const bitmap_color_t* bitmap, const dimension_t x, const dimension_t y)
					{
						using index_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;

						const index_t offset =
							(static_cast<index_t>(y) * static_cast<index_t>(Width)) + static_cast<index_t>(x);

						return bitmap[offset];
					}

					/// <summary>
					/// Reads an 8-bit pixel value at (x, y).
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Bitmap width (pixels).</typeparam>
					template<typename dimension_t, dimension_t Width>
					static uint8_t ReadColorAt(const uint8_t* bitmap, const dimension_t x, const dimension_t y)
					{
						return ReadColorAt<dimension_t, Width, uint8_t>(bitmap, x, y);
					}

					/// <summary>
					/// Reads a 16-bit pixel value at (x, y).
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Bitmap width (pixels).</typeparam>
					template<typename dimension_t, dimension_t Width>
					static uint16_t ReadColorAt(const uint16_t* bitmap, const dimension_t x, const dimension_t y)
					{
						return ReadColorAt<dimension_t, Width, uint16_t>(bitmap, x, y);
					}

					/// <summary>
					/// Reads a 32-bit pixel value at (x, y).
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Bitmap width (pixels).</typeparam>
					template<typename dimension_t, dimension_t Width>
					static uint32_t ReadColorAt(const uint32_t* bitmap, const dimension_t x, const dimension_t y)
					{
						return ReadColorAt<dimension_t, Width, uint32_t>(bitmap, x, y);
					}
				};

#if defined(ARDUINO_ARCH_AVR)
				/// <summary>
				/// Bitmap reader for data stored in flash/PROGMEM on AVR.
				/// </summary>
				struct Flash
				{
					/// <summary>
					/// Reads an 8-bit pixel value at (x, y) from PROGMEM.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Bitmap width (pixels).</typeparam>
					template<typename dimension_t, dimension_t Width>
					static uint8_t ReadColorAt(const uint8_t* bitmap, const dimension_t x, const dimension_t y)
					{
						using index_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;

						const index_t offset =
							(static_cast<index_t>(y) * static_cast<index_t>(Width)) + static_cast<index_t>(x);

						return pgm_read_byte(&bitmap[offset]);
					}

					/// <summary>
					/// Reads a 16-bit pixel value at (x, y) from PROGMEM.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Bitmap width (pixels).</typeparam>
					template<typename dimension_t, dimension_t Width>
					static uint16_t ReadColorAt(const uint16_t* bitmap, const dimension_t x, const dimension_t y)
					{
						using index_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;

						const index_t offset =
							(static_cast<index_t>(y) * static_cast<index_t>(Width)) + static_cast<index_t>(x);

						return static_cast<uint16_t>(pgm_read_word(&bitmap[offset]));
					}

					/// <summary>
					/// Reads a 32-bit pixel value at (x, y) from PROGMEM.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Bitmap width (pixels).</typeparam>
					template<typename dimension_t, dimension_t Width>
					static uint32_t ReadColorAt(const uint32_t* bitmap, const dimension_t x, const dimension_t y)
					{
						using index_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;

						const index_t offset =
							(static_cast<index_t>(y) * static_cast<index_t>(Width)) + static_cast<index_t>(x);

						return static_cast<uint32_t>(pgm_read_dword(&bitmap[offset]));
					}
				};
#else
				/// <summary>
				/// Bitmap reader for flash/PROGMEM.
				/// On non-AVR targets this is an alias of the RAM reader.
				/// </summary>
				using Flash = Ram;
#endif
			}
		}
	}
}
#endif