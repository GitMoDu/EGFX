#ifndef _EGFX_FRAMEWORK_ASSETS_FONT_VECTOR_EPOXY_SYMBOLS_h
#define _EGFX_FRAMEWORK_ASSETS_FONT_VECTOR_EPOXY_SYMBOLS_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace Font
			{
				namespace Vector
				{
					namespace Epoxy
					{
						namespace VectorData
						{
							namespace Symbols
							{
								using namespace Framework::Vector;
								using namespace Framework::Vector::Font;

								using namespace Dimensions;

								static constexpr uint8_t CharBackslash[] PROGMEM
								{
									Node(NodeEnum::Start, 0, 0),
									Node(NodeEnum::End, Triple::EndX, FalseBottom),
								};

								static constexpr uint8_t CharForwardSlash[] PROGMEM
								{
									Node(NodeEnum::Start, Triple::EndX, 0),
									Node(NodeEnum::End, 0, FalseBottom),
								};

								static constexpr uint8_t CharParenthesisOpen[] PROGMEM
								{
									Node(NodeEnum::Start, Triple::EndX, 0),
									Node(NodeEnum::Start, Triple::CenterX, 1),
									Node(NodeEnum::Start, 0, 2),
									Node(NodeEnum::Start, 0, FalseBottom - 2),
									Node(NodeEnum::Start, Triple::CenterX, FalseBottom - 1),
									Node(NodeEnum::Start, Triple::EndX, FalseBottom)
								};

								static constexpr uint8_t CharParenthesisClose[] PROGMEM
								{
									Node(NodeEnum::Start, 0, 0),
									Node(NodeEnum::Start, Triple::CenterX, 1),
									Node(NodeEnum::Start, Triple::EndX, 2),
									Node(NodeEnum::Start, Triple::EndX, FalseBottom - 2),
									Node(NodeEnum::Start, Triple::CenterX, FalseBottom - 1),
									Node(NodeEnum::Start, 0, FalseBottom)
								};

								static constexpr uint8_t CharBracketOpen[] PROGMEM
								{
									Node(NodeEnum::Start, Triple::EndX, 0),
									Node(NodeEnum::Start, 0, 0),
									Node(NodeEnum::Start, 0, FalseBottom),
									Node(NodeEnum::Start, Triple::EndX, FalseBottom),
								};

								static constexpr uint8_t CharBracketClose[] PROGMEM
								{
									Node(NodeEnum::Start, 0, 0),
									Node(NodeEnum::Start, Triple::EndX, 0),
									Node(NodeEnum::Start, Triple::EndX, FalseBottom),
									Node(NodeEnum::Start, 0, FalseBottom),
								};

								static constexpr uint8_t CharPeriod[] PROGMEM
								{
									Node(NodeEnum::Start, 0, FalseBottom - 1),
									Node(NodeEnum::End, 0, FalseBottom),
								};

								static constexpr uint8_t CharComma[] PROGMEM
								{
									Node(NodeEnum::Start, 0, AxisMax),
									Node(NodeEnum::End, Double::EndX, FalseBottom),
								};

								static constexpr uint8_t CharColon[] PROGMEM
								{
									Node(NodeEnum::Start, 0, 1),
									Node(NodeEnum::End, 0, 2),
									Node(NodeEnum::Start, 0, FalseBottom - 1),
									Node(NodeEnum::End, 0, FalseBottom),
								};

								static constexpr uint8_t CharSemicolon[] PROGMEM
								{
									Node(NodeEnum::Start, 1, 1),
									Node(NodeEnum::End, 1, 2),
									Node(NodeEnum::Start, 1, FalseBottom),
									Node(NodeEnum::End, 0, AxisMax),
								};

								static constexpr uint8_t CharLessThan[] PROGMEM
								{
									Node(NodeEnum::Start, Triple::EndX, 1),
									Node(NodeEnum::Start, 0, AxisCenter),
									Node(NodeEnum::End, Triple::EndX, AxisMax - 2),
								};

								static constexpr uint8_t CharGreaterThan[] PROGMEM
								{
									Node(NodeEnum::Start, 0, 1),
									Node(NodeEnum::Start, Triple::EndX, AxisCenter),
									Node(NodeEnum::End, 0, AxisMax - 2),
								};

								static constexpr uint8_t CharEquals[] PROGMEM
								{
									Node(NodeEnum::Start, 0, AxisCenter - 1),
									Node(NodeEnum::End, Triple::EndX, AxisCenter - 1),
									Node(NodeEnum::Start, 0, AxisCenter + 1),
									Node(NodeEnum::End, Triple::EndX, AxisCenter + 1),
								};

								static constexpr uint8_t CharMinus[] PROGMEM
								{
									Node(NodeEnum::Start, 0, AxisCenter),
									Node(NodeEnum::End, Small::EndX, AxisCenter)
								};

								static constexpr uint8_t CharPlus[] PROGMEM
								{
									Node(NodeEnum::Start, 0, AxisCenter),
									Node(NodeEnum::End, Small::EndX, AxisCenter),
									Node(NodeEnum::Start, Small::CenterX, FalseBottom - Small::EndX),
									Node(NodeEnum::End, Small::CenterX, FalseBottom),
								};

								static constexpr uint8_t CharAsterisk[] PROGMEM
								{
									Node(NodeEnum::Start, Triple::CenterX, AxisCenter),
									Node(NodeEnum::End, Triple::CenterX, AxisCenter - 2),

									Node(NodeEnum::Start, Triple::CenterX, AxisCenter),
									Node(NodeEnum::End, 0, AxisCenter - 1),

									Node(NodeEnum::Start, Triple::CenterX, AxisCenter),
									Node(NodeEnum::End, Triple::EndX, AxisCenter - 1),

									Node(NodeEnum::Start, Triple::CenterX, AxisCenter),
									Node(NodeEnum::End, Triple::EndX, AxisCenter + 2),

									Node(NodeEnum::Start, Triple::CenterX, AxisCenter),
									Node(NodeEnum::End, 0, AxisCenter + 2)
								};

								static constexpr uint8_t CharExclamation[] PROGMEM
								{
									Node(NodeEnum::Start, 0, 0),
									Node(NodeEnum::Start, Double::EndX,0),
									Node(NodeEnum::End, Double::EndX, AxisCenter + 1),

									Node(NodeEnum::Start, 0, FalseBottom),
									Node(NodeEnum::End, Double::EndX, FalseBottom)
								};

								static constexpr uint8_t CharQuestion[] PROGMEM
								{
									Node(NodeEnum::Start, 0, 1),
									Node(NodeEnum::Start, 1, 0),
									Node(NodeEnum::Start, Small::EndX - 1, 0),
									Node(NodeEnum::Start, Small::EndX, 1),
									Node(NodeEnum::Start, Small::EndX - 0, 2),
									Node(NodeEnum::End, Small::CenterX, FalseBottom - 2),
									Node(NodeEnum::Start, Small::CenterX, FalseBottom - 1),
									Node(NodeEnum::End, Small::CenterX, FalseBottom)
								};

								static constexpr uint8_t CharDollar[] PROGMEM
								{
									Node(NodeEnum::Start, Small::EndX, 1 + 1),
									Node(NodeEnum::Start, Small::EndX - 1, 0 + 1),
									Node(NodeEnum::Start, Small::EndX - 1, 0 + 1),
									Node(NodeEnum::Start, 1, 0 + 1),
									Node(NodeEnum::Start, 0, 1 + 1),
									Node(NodeEnum::Start, 1, AxisCenter),
									Node(NodeEnum::Start, Small::EndX - 1, AxisCenter),
									Node(NodeEnum::Start, Small::EndX, AxisCenter + 1),
									Node(NodeEnum::Start, Small::EndX, FalseBottom - 1),
									Node(NodeEnum::Start, Small::EndX - 1, FalseBottom),
									Node(NodeEnum::Start, 1, FalseBottom),
									Node(NodeEnum::End, 0, FalseBottom - 1),

									Node(NodeEnum::Start, Small::CenterX, 0),
									Node(NodeEnum::End, Small::CenterX, FalseBottom + 1),
								};

								static constexpr uint8_t CharPercent[] PROGMEM
								{
									Node(NodeEnum::Start, 0, 0),
									Node(NodeEnum::Start, 2, 0),
									Node(NodeEnum::Start, 2, 2),
									Node(NodeEnum::Start, 0, 2),
									Node(NodeEnum::End, 0, 0),

									Node(NodeEnum::Start, Small::EndX - 2, FalseBottom - 2),
									Node(NodeEnum::Start, Small::EndX, FalseBottom - 2),
									Node(NodeEnum::Start, Small::EndX, FalseBottom),
									Node(NodeEnum::Start, Small::EndX - 2, FalseBottom),
									Node(NodeEnum::End, Small::EndX - 2, FalseBottom - 2),

									Node(NodeEnum::Start, 0, FalseBottom),
									Node(NodeEnum::End, Small::EndX, 0),
								};

								static constexpr uint8_t CharNumberSign[] PROGMEM
								{
									Node(NodeEnum::Start, 1, FalseBottom),
									Node(NodeEnum::End, 1, FalseBottom),
									Node(NodeEnum::Start, Largest::EndX - 2, FalseBottom),
									Node(NodeEnum::End, Largest::EndX - 2, FalseBottom),

									Node(NodeEnum::Start, 2, 0),
									Node(NodeEnum::End, 1, FalseBottom),
									Node(NodeEnum::Start, Largest::EndX - 1, 0),
									Node(NodeEnum::End, Largest::EndX - 2, FalseBottom),

									Node(NodeEnum::Start, 0, 1),
									Node(NodeEnum::End, Largest::EndX, 1),
									Node(NodeEnum::Start, 0, FalseBottom - 1),
									Node(NodeEnum::End, Largest::EndX, FalseBottom - 1),
								};

								static constexpr uint8_t CharUnderscore[] PROGMEM
								{
									Node(NodeEnum::Start, 0, FalseBottom),
									Node(NodeEnum::End, Triple::EndX, FalseBottom),
								};

								static constexpr uint8_t CharVerticalBar[] PROGMEM
								{
									Node(NodeEnum::Start, 0, 0),
									Node(NodeEnum::End, Single::EndX, FalseBottom),
								};

								static constexpr uint8_t CharAmpersand[] PROGMEM
								{
									Node(NodeEnum::Start, Small::EndX, AxisCenter),
									Node(NodeEnum::Start, Small::EndX - 1, FalseBottom - 1),
									Node(NodeEnum::Start, Small::EndX - 2, FalseBottom),
									Node(NodeEnum::Start, 1, FalseBottom),
									Node(NodeEnum::Start, 0, FalseBottom - 1),
									Node(NodeEnum::Start, 0, AxisCenter + 1),
									Node(NodeEnum::Start, 1, AxisCenter),
									Node(NodeEnum::Start, Small::EndX - 1, AxisCenter - 1),
									Node(NodeEnum::Start, Small::EndX - 1, 1),
									Node(NodeEnum::Start, Small::EndX - 2, 0),
									Node(NodeEnum::Start, 1, 0),
									Node(NodeEnum::Start, 0, 1),
									Node(NodeEnum::Start, 1, AxisCenter - 1),
									Node(NodeEnum::Start, 2, AxisCenter),
									Node(NodeEnum::End, Small::EndX, FalseBottom),
								};

								static constexpr uint8_t CharApostrophe[] PROGMEM
								{
									Node(NodeEnum::Start, 0, 0),
									Node(NodeEnum::End, 0, 1),
								};

								static constexpr uint8_t CharDoubleQuote[] PROGMEM
								{
									Node(NodeEnum::Start, 0, 0),
									Node(NodeEnum::End, 0, 1),
									Node(NodeEnum::Start, Double::EndX, 0),
									Node(NodeEnum::End, Double::EndX, 1),
								};
							}
						}
					}
				}
			}
		}
	}
}
#endif