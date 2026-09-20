#ifndef _INTEGERGLASS_FRAMEWORK_IMAGE_VECTOR_MODEL_h
#define _INTEGERGLASS_FRAMEWORK_IMAGE_VECTOR_MODEL_h

#include "../Model.h"

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Image
		{
			namespace Vector
			{
				namespace Contract
				{
					/// <summary>
					/// Represents a compile-time vector image source with packed node data,
					/// source dimensions, and a rendering thickness scale.
					/// </summary>
					template<Framework::Vector::PackedVectorEnum vectorType = Framework::Vector::PackedVectorEnum::Image15x15,
						typename axis_type = uint8_t>
					struct StaticImageSource
					{
						static constexpr Framework::DataSourceTypeEnum DataSourceType = Framework::DataSourceTypeEnum::Flash;
						static constexpr Framework::Vector::PackedVectorEnum VectorType = vectorType;
						using axis_t = axis_type;

						static constexpr const void* Data = nullptr;
						static constexpr size_t Count = 0;
						static constexpr axis_t CanvasWidth = 0;
						static constexpr axis_t CanvasHeight = 0;
						static constexpr ufraction8_t ThicknessScale = 0;
					};
				}
			}
		}
	}
}
#endif