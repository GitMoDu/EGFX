#ifndef _EGFX_FRAMEWORK_ICON_VECTOR_MODEL_h
#define _EGFX_FRAMEWORK_ICON_VECTOR_MODEL_h

#include "../Model.h"
#include "../../Vector/Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Icon
		{
			/// <summary>
			/// Vector implemention of the Icon model, using the Vector model as a base for defining the icon's vector graphics.
			/// </summary>
			namespace Vector
			{
				namespace Icon15x15
				{
					using model_t = Framework::Vector::Image15x15::model_t;

					using axis_t = typename model_t::axis_t;
					using opcode_t = typename model_t::opcode_t;
					using source_t = typename model_t::source_t;
					using packed_t = typename model_t::Node::packed_t;

					static constexpr uint8_t AxisSize = model_t::AxisSize;
					static constexpr axis_t AxisMask = model_t::AxisMask;
					static constexpr axis_t AxisMax = model_t::AxisMax;
					static constexpr axis_t OpCodeAxis = model_t::OpCodeAxis;
					static constexpr opcode_t OpCodeCount = model_t::OpCodeCount;
				}
			}
		}
	}
}
#endif