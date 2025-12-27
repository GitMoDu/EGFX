#ifndef _EGFX_VECTOR_FONTS_EPOXY_h
#define _EGFX_VECTOR_FONTS_EPOXY_h

#include "Model.h"

namespace Egfx
{
	namespace VectorFonts
	{
		namespace Epoxy
		{
			namespace VectorData
			{
				namespace Numbers
				{
					using namespace ::Egfx::VectorGraphics;
					using namespace ::Egfx::VectorFont;
					using namespace Dimensions;

					static constexpr uint8_t Char0[] PROGMEM
					{
						Node(NodeEnum::Start, 1, 0),
						Node(NodeEnum::Start, Lowercase::EndX - 1, 0),
						Node(NodeEnum::Start, Lowercase::EndX, 1),
						Node(NodeEnum::Start, Lowercase::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Lowercase::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 0, 1),
						Node(NodeEnum::End, 1, 0)
					};

					static constexpr uint8_t Char1[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 1),
						Node(NodeEnum::Start, Triple::CenterX, 0),
						Node(NodeEnum::End, Triple::CenterX, FalseBottom),
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::End, Triple::EndX, FalseBottom)
					};

					static constexpr uint8_t Char2[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 1),
						Node(NodeEnum::Start, 1, 0),
						Node(NodeEnum::Start, Small::EndX - 1, 0),
						Node(NodeEnum::Start, Small::EndX, 1),
						Node(NodeEnum::Start, Small::EndX, Small::CenterX - 0),
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::Start, Small::EndX, FalseBottom),
						Node(NodeEnum::End, 0, FalseBottom)
					};

					static constexpr uint8_t Char3[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 1),
						Node(NodeEnum::Start, 1, 0),
						Node(NodeEnum::Start, Small::EndX - 1, 0),
						Node(NodeEnum::Start, Small::EndX, 1),
						Node(NodeEnum::Start, Small::EndX, AxisCenter - 1),
						Node(NodeEnum::Start, Small::EndX - 1, AxisCenter),

						Node(NodeEnum::Start, Small::CenterX, AxisCenter),

						Node(NodeEnum::Start, Small::EndX - 1, AxisCenter),
						Node(NodeEnum::Start, Small::EndX, AxisCenter + 1),
						Node(NodeEnum::Start, Small::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Small::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::End, 0, FalseBottom - 1)
					};

					static constexpr uint8_t Char4[] PROGMEM
					{
						Node(NodeEnum::Start, Small::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, Small::EndX - 1, 0),
						Node(NodeEnum::Start, 0, AxisCenter + 1),
						Node(NodeEnum::End, Small::EndX, AxisCenter + 1),
					};

					static constexpr uint8_t Char5[] PROGMEM
					{
						Node(NodeEnum::Start, Small::EndX, 0),
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::Start, Small::EndX - 1, AxisCenter),
						Node(NodeEnum::Start, Small::EndX, AxisCenter + 1),
						Node(NodeEnum::Start, Small::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Small::EndX - 1, FalseBottom),
						Node(NodeEnum::End, 0, FalseBottom),
					};

					static constexpr uint8_t Char6[] PROGMEM
					{
						Node(NodeEnum::Start, Small::EndX, 1),
						Node(NodeEnum::Start, Small::EndX - 1, 0),
						Node(NodeEnum::Start, 1, 0),
						Node(NodeEnum::Start, 0, 1),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, Small::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, Small::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Small::EndX, AxisCenter + 1),
						Node(NodeEnum::Start, Small::EndX - 1, AxisCenter),
						Node(NodeEnum::Start, 1, AxisCenter),
						Node(NodeEnum::End, 0, AxisCenter + 1)
					};

					static constexpr uint8_t Char7[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::Start, Small::EndX, 0),
						Node(NodeEnum::Start, Small::EndX - 1, AxisCenter - 1),
						Node(NodeEnum::End, 1, FalseBottom),
					};

					static constexpr uint8_t Char8[] PROGMEM
					{
						Node(NodeEnum::Start, 1, AxisCenter),
						Node(NodeEnum::Start, 0, AxisCenter - 1),
						Node(NodeEnum::Start, 0, 1),
						Node(NodeEnum::Start, 1, 0),
						Node(NodeEnum::Start, Small::EndX - 1, 0),
						Node(NodeEnum::Start, Small::EndX, 1),
						Node(NodeEnum::Start, Small::EndX, AxisCenter - 1),
						Node(NodeEnum::Start, Small::EndX - 1, AxisCenter),
						Node(NodeEnum::Start, 1, AxisCenter),
						Node(NodeEnum::Start, 1, AxisCenter),
						Node(NodeEnum::Start, 0, AxisCenter + 1),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, Small::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, Small::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Small::EndX, AxisCenter + 1),
						Node(NodeEnum::End, Small::EndX - 1, AxisCenter),
					};

					static constexpr uint8_t Char9[] PROGMEM
					{
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, Small::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, Small::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Small::EndX, 1),
						Node(NodeEnum::Start, Small::EndX - 1, 0),
						Node(NodeEnum::Start, 1, 0),
						Node(NodeEnum::Start, 0, 1),
						Node(NodeEnum::Start, 0, AxisCenter - 1),
						Node(NodeEnum::Start, 1, AxisCenter),
						Node(NodeEnum::Start, Small::EndX - 1, AxisCenter),
						Node(NodeEnum::End, Small::EndX, AxisCenter - 1)
					};
				}
			}
		}
	}
}
#endif