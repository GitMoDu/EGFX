#ifndef _INTEGERGLASS_FRAMEWORK_BITMASK_SOURCE_h
#define _INTEGERGLASS_FRAMEWORK_BITMASK_SOURCE_h

#include "Model.h"

namespace IntegerGlass
{
	namespace Framework
	{

		namespace Bitmask
		{
			namespace Source
			{
				namespace Reader
				{
					/// <summary>
					/// Bitmask bitstream reader for data stored in RAM.
					/// </summary>
					struct Ram
					{
						/// <summary>
						/// Reads up to eight bits from an MSB-first packed bitstream.
						/// </summary>
						/// <typeparam name="bit_index_t">Unsigned bit index type.</typeparam>
						/// <param name="mask">Pointer to packed bitmask bytes.</param>
						/// <param name="bitIndex">Zero-based bit index.</param>
						/// <param name="bitCount">Number of valid bits to return.</param>
						/// <returns>The requested bits, MSB-aligned.</returns>
						template<typename bit_index_t>
						static uint8_t ReadByteAt(const uint8_t* mask, const bit_index_t bitIndex, const uint8_t bitCount)
						{
							const bit_index_t byteIndex = static_cast<bit_index_t>(bitIndex / 8);
							const uint8_t bitOffset = static_cast<uint8_t>(bitIndex % 8);
							const uint8_t first = mask[byteIndex];
							const uint8_t validMask = bitCount == 8 ? 0xFF : static_cast<uint8_t>(0xFF << (8 - bitCount));

							if (bitOffset == 0)
								return static_cast<uint8_t>(first & validMask);

							const uint16_t combined = static_cast<uint16_t>(first) << 8 | mask[byteIndex + 1];
							return static_cast<uint8_t>((combined >> (8 - bitOffset)) & validMask);
						}
					};

					/// <summary>
					/// Bitmask bitstream reader for data stored in flash/PROGMEM on AVR.
					/// On non-AVR targets, this behaves like the RAM reader.
					/// </summary>
					struct Flash
					{
						/// <summary>
						/// Reads up to eight bits from an MSB-first packed bitstream.
						/// </summary>
						/// <typeparam name="bit_index_t">Unsigned bit index type.</typeparam>
						/// <param name="mask">Pointer to packed bitmask bytes.</param>
						/// <param name="bitIndex">Zero-based bit index.</param>
						/// <param name="bitCount">Number of valid bits to return.</param>
						/// <returns>The requested bits, MSB-aligned.</returns>
						template<typename bit_index_t>
						static uint8_t ReadByteAt(const uint8_t* mask, const bit_index_t bitIndex, const uint8_t bitCount)
						{
							const bit_index_t byteIndex = static_cast<bit_index_t>(bitIndex / 8);
							const uint8_t bitOffset = static_cast<uint8_t>(bitIndex % 8);
							const uint8_t first =
#if defined(ARDUINO_ARCH_AVR)
								pgm_read_byte_near(mask + byteIndex);
#else
								mask[byteIndex];
#endif
							const uint8_t validMask = bitCount == 8 ? 0xFF : static_cast<uint8_t>(0xFF << (8 - bitCount));

							if (bitOffset == 0)
								return static_cast<uint8_t>(first & validMask);

#if defined(ARDUINO_ARCH_AVR)
							const uint8_t second = pgm_read_byte_near(mask + byteIndex + 1);
#else
							const uint8_t second = mask[byteIndex + 1];
#endif
							const uint16_t combined = static_cast<uint16_t>(first) << 8 | second;
							return static_cast<uint8_t>((combined >> (8 - bitOffset)) & validMask);
						}
					};
				}

				template<typename dimension_t,
					Framework::DataSourceTypeEnum dataSourceType>
				struct ShaderBitmask
				{
					using ReaderType = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						Reader::Flash,
						Reader::Ram,
						dataSourceType == Framework::DataSourceTypeEnum::Flash>::type;

					using color_t = bool;
					static constexpr bool IsConstantColor = false;

					using bit_index_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;

					const uint8_t* BitmaskData = nullptr;

					bool PrepareBitmaskData(const uint8_t* bitmaskData, const dimension_t width, const dimension_t height)
					{
						BitmaskData = bitmaskData;

						return BitmaskData != nullptr && width > 0 && height > 0;
					}

					uint8_t ReadByte(const bit_index_t bitIndex, const uint8_t bitCount)
					{
						return ReaderType::template ReadByteAt<bit_index_t>(BitmaskData, bitIndex, bitCount);
					}
				};
			}
		}
	}
}
#endif