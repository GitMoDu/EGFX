#ifndef _EGFX_FRAMEWORK_BITMASK_ABSTRACT_BITMASK_h
#define _EGFX_FRAMEWORK_BITMASK_ABSTRACT_BITMASK_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Bitmask
		{
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
#endif