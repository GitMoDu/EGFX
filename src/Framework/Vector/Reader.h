#ifndef _INTEGERGLASS_FRAMEWORK_VECTOR_READER_h
#define _INTEGERGLASS_FRAMEWORK_VECTOR_READER_h

#include "Model.h"

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Vector
		{
			namespace Reader
			{
				struct Ram
				{
					template<typename node_t>
					static node_t ReadPacked(const node_t* data, const size_t index)
					{
						return data[index];
					}
				};

				struct Flash
				{
					template<typename node_t>
					static node_t ReadPacked(const node_t* data, const size_t index,
						IntegerSignal::TypeTraits::TypeDispatch::TrueType)
					{
#if defined(ARDUINO_ARCH_AVR)
						return static_cast<node_t>(pgm_read_byte_near(data + index));
#else
						return data[index];
#endif
					};

					template<typename node_t>
					static node_t ReadPacked(const node_t* data, const size_t index,
						IntegerSignal::TypeTraits::TypeDispatch::FalseType)
					{
#if defined(ARDUINO_ARCH_AVR)
						return static_cast<node_t>(pgm_read_word_near(data + index));
#else
						return data[index];
#endif
					};

					template<typename node_t>
					static node_t ReadPacked(const node_t* data, const size_t index)
					{
						using ReadTag = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
							IntegerSignal::TypeTraits::TypeDispatch::TrueType,
							IntegerSignal::TypeTraits::TypeDispatch::FalseType,
							sizeof(node_t) == 1>::type;

						return ReadPacked(data, index, ReadTag{});
					}
				};
			}

			template<Framework::DataSourceTypeEnum dataSourceType>
			using ReaderType = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
				Reader::Flash,
				Reader::Ram,
				dataSourceType == Framework::DataSourceTypeEnum::Flash>::type;
		}
	}
}
#endif