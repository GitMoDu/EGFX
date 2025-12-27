#ifndef _EGFX_VECTOR_FONTS_EPOXY_LETTERS_h
#define _EGFX_VECTOR_FONTS_EPOXY_LETTERS_h

#include "Model.h"

namespace Egfx
{
	namespace VectorFonts
	{
		namespace Epoxy
		{
			namespace VectorData
			{
				namespace Letters
				{
					using namespace ::Egfx::VectorGraphics;
					using namespace ::Egfx::VectorFont;
					using namespace Dimensions;

					static constexpr uint8_t CharA[] PROGMEM
					{
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::Start, Big::CenterX, 0),
						Node(NodeEnum::End, Big::EndX, FalseBottom),
						Node(NodeEnum::Start, 1, FalseBottom - 2),
						Node(NodeEnum::Start, Big::EndX - 1, FalseBottom - 2),
					};

					static constexpr uint8_t Chara[] PROGMEM
					{
						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::Start, 1, AxisCenter - 1),
						Node(NodeEnum::Start, Lowercase::EndX - 1, AxisCenter - 1),
						Node(NodeEnum::Start, Lowercase::EndX, AxisCenter),
						Node(NodeEnum::Start, Lowercase::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Lowercase::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 0, AxisCenter + 1),
						Node(NodeEnum::Start, Lowercase::CenterX, AxisCenter),
						Node(NodeEnum::Start, Lowercase::EndX - 1, AxisCenter),
						Node(NodeEnum::Start, Lowercase::EndX, AxisCenter + 1),
						Node(NodeEnum::End, Lowercase::EndX, FalseBottom)
					};

					static constexpr uint8_t CharB[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::Start, Small::EndX - 1, 0),
						Node(NodeEnum::Start, Small::EndX, 1),
						Node(NodeEnum::Start, Small::EndX, AxisCenter - 1),
						Node(NodeEnum::Start, Small::EndX - 1, AxisCenter),

						Node(NodeEnum::Start, 0, AxisCenter),

						Node(NodeEnum::Start, Small::EndX - 1, AxisCenter),
						Node(NodeEnum::Start, Small::EndX, AxisCenter + 1),
						Node(NodeEnum::Start, Small::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Small::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::End, 0, 0)
					};

					static constexpr uint8_t Charb[] PROGMEM
					{
						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::Start, Lowercase::EndX - 1, AxisCenter - 1),
						Node(NodeEnum::Start, Lowercase::EndX, AxisCenter),
						Node(NodeEnum::Start, Lowercase::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Lowercase::EndX - 1, FalseBottom),
						Node(NodeEnum::End, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::End, 0, 0)
					};

					static constexpr uint8_t CharC[] PROGMEM
					{
						Node(NodeEnum::Start, Small::EndX, 1),
						Node(NodeEnum::Start, Small::EndX - 1, 0),
						Node(NodeEnum::Start, 1, 0),
						Node(NodeEnum::Start, 0, 1),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, Small::EndX - 1, FalseBottom),
						Node(NodeEnum::End, Small::EndX, FalseBottom - 1),
					};

					static constexpr uint8_t Charc[] PROGMEM
					{
						Node(NodeEnum::Start, Lowercase::EndX, AxisCenter),
						Node(NodeEnum::Start, Lowercase::EndX - 1, AxisCenter - 1),
						Node(NodeEnum::Start, 1, AxisCenter - 1),
						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, Lowercase::EndX - 1, FalseBottom),
						Node(NodeEnum::End, Lowercase::EndX, FalseBottom - 1),
					};

					static constexpr uint8_t CharD[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::Start, Small::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, Small::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Small::EndX, 1),
						Node(NodeEnum::Start, Small::EndX - 1, 0),
						Node(NodeEnum::End, 0, 0)
					};

					static constexpr uint8_t Chard[] PROGMEM
					{
						Node(NodeEnum::Start, Lowercase::EndX, AxisCenter),
						Node(NodeEnum::Start, Lowercase::EndX - 1, AxisCenter - 1),
						Node(NodeEnum::Start, 1, AxisCenter - 1),
						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, Lowercase::EndX - 1, FalseBottom),
						Node(NodeEnum::End, Lowercase::EndX, FalseBottom),
						Node(NodeEnum::Start, Lowercase::EndX, 0),
						Node(NodeEnum::End, Lowercase::EndX, FalseBottom),
					};

					static constexpr uint8_t CharE[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::End, Small::EndX, FalseBottom),
						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::End, Small::EndX - 2, AxisCenter),
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::End, Small::EndX, 0),
					};

					static constexpr uint8_t Chare[] PROGMEM
					{
						Node(NodeEnum::Start, 0, AxisCenter + 1),
						Node(NodeEnum::Start, Lowercase::EndX, AxisCenter + 1),
						Node(NodeEnum::Start, Lowercase::EndX, AxisCenter),
						Node(NodeEnum::Start, Lowercase::EndX - 1, AxisCenter - 1),
						Node(NodeEnum::Start, 1, AxisCenter - 1),
						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, Lowercase::EndX - 1, FalseBottom),
						Node(NodeEnum::End, Lowercase::EndX, FalseBottom - 1),
					};

					static constexpr uint8_t CharF[] PROGMEM
					{
						Node(NodeEnum::Start, Small::EndX, 0),
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::End, 0, FalseBottom),
						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::End, Small::EndX - 1, AxisCenter),
					};

					static constexpr uint8_t Charf[] PROGMEM
					{
						Node(NodeEnum::Start, Lowercase::EndX, 0),
						Node(NodeEnum::Start, 1, 1),
						Node(NodeEnum::End, 1, FalseBottom),
						Node(NodeEnum::Start, 0, AxisCenter - 1),
						Node(NodeEnum::End, Lowercase::EndX, AxisCenter - 1),
					};

					static constexpr uint8_t CharG[] PROGMEM
					{
						Node(NodeEnum::Start, Big::EndX, 1),
						Node(NodeEnum::Start, Big::EndX - 1, 0),
						Node(NodeEnum::Start, 1, 0),
						Node(NodeEnum::Start, 0, 1),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, Big::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, Big::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Big::EndX, AxisCenter),
						Node(NodeEnum::End, Big::EndX - 2, AxisCenter)
					};

					static constexpr uint8_t Charg[] PROGMEM
					{
						Node(NodeEnum::Start, 0, AxisMax),
						Node(NodeEnum::Start, Lowercase::EndX - 1, AxisMax),
						Node(NodeEnum::Start, Lowercase::EndX, FalseBottom),
						Node(NodeEnum::Start, Lowercase::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Lowercase::EndX, AxisCenter),
						Node(NodeEnum::Start, Lowercase::EndX - 1, AxisCenter - 1),
						Node(NodeEnum::Start, 1, AxisCenter - 1),
						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, Lowercase::CenterX - 1, FalseBottom),
						Node(NodeEnum::Start, Lowercase::EndX - 1, FalseBottom),
						Node(NodeEnum::End, Lowercase::EndX, FalseBottom - 1),
					};

					static constexpr uint8_t CharH[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::End, 0, FalseBottom),
						Node(NodeEnum::Start, Small::EndX, 0),
						Node(NodeEnum::End, Small::EndX, FalseBottom),
						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::End, Small::EndX, AxisCenter),
					};

					static constexpr uint8_t Charh[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::End, 0, FalseBottom),
						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::Start, Lowercase::EndX - 1, AxisCenter - 1),
						Node(NodeEnum::Start, Lowercase::EndX, AxisCenter),
						Node(NodeEnum::End, Lowercase::EndX, FalseBottom),
					};

					static constexpr uint8_t CharI[] PROGMEM
					{
						Node(NodeEnum::Start, Triple::CenterX, 0),
						Node(NodeEnum::End, Triple::CenterX, FalseBottom),
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::End, Triple::EndX, 0),
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::End, Triple::EndX, FalseBottom),
					};

					static constexpr uint8_t Chari[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::End, Double::EndX, 0),
						Node(NodeEnum::Start, 0, AxisCenter - 1),
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::End, Double::EndX, FalseBottom),
					};

					static constexpr uint8_t CharJ[] PROGMEM
					{
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, Lowercase::EndX - 1,FalseBottom),
						Node(NodeEnum::Start, Lowercase::EndX, FalseBottom - 1),
						Node(NodeEnum::End, Lowercase::EndX, 0)
					};

					static constexpr uint8_t Charj[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::End, Double::EndX, 0),
						Node(NodeEnum::Start, 0, AxisCenter - 1),
						Node(NodeEnum::Start, Double::EndX, AxisCenter - 1),
						Node(NodeEnum::Start, Double::EndX, FalseBottom),
						Node(NodeEnum::End, 0, AxisMax),
					};

					static constexpr uint8_t CharK[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::End, 0, FalseBottom),
						Node(NodeEnum::Start, Small::EndX, 0),
						Node(NodeEnum::Start, 1, AxisCenter),
						Node(NodeEnum::End,  Small::EndX, FalseBottom),
					};

					static constexpr uint8_t Chark[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::End, 0, FalseBottom),
						Node(NodeEnum::Start, 0, AxisCenter + 1),
						Node(NodeEnum::End, Lowercase::EndX, AxisCenter - 1),
						Node(NodeEnum::Start, Lowercase::CenterX - 1, AxisCenter + 1),
						Node(NodeEnum::End, Lowercase::EndX, FalseBottom),
					};

					static constexpr uint8_t CharL[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::End, Lowercase::EndX, FalseBottom),
					};

					static constexpr uint8_t Charl[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::Start, Triple::CenterX, 0),
						Node(NodeEnum::End, Triple::CenterX, FalseBottom),
						Node(NodeEnum::Start, Triple::EndX, FalseBottom),
						Node(NodeEnum::End, 0, FalseBottom),
					};

					static constexpr uint8_t CharM[] PROGMEM
					{
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::Start, Largest::CenterX, FalseBottom),
						Node(NodeEnum::Start, Largest::EndX, 0),
						Node(NodeEnum::End, Largest::EndX, FalseBottom),
					};

					static constexpr uint8_t Charm[] PROGMEM
					{
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::Start, 0, AxisCenter),

						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::Start, 1, AxisCenter - 1),
						Node(NodeEnum::Start, Largest::CenterX - 1, AxisCenter - 1),
						Node(NodeEnum::Start, Largest::CenterX, AxisCenter),
						Node(NodeEnum::End, Largest::CenterX, FalseBottom),
						Node(NodeEnum::Start, Largest::CenterX, AxisCenter),
						Node(NodeEnum::Start, Largest::CenterX + 1, AxisCenter - 1),
						Node(NodeEnum::Start, Largest::EndX - 1, AxisCenter - 1),
						Node(NodeEnum::Start, Largest::EndX, AxisCenter),
						Node(NodeEnum::End, Largest::EndX, FalseBottom),
						Node(NodeEnum::Start, 0, AxisCenter - 1),
						Node(NodeEnum::End, 0, AxisCenter),
					};

					static constexpr uint8_t CharN[] PROGMEM
					{
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::Start, Small::EndX, FalseBottom),
						Node(NodeEnum::End, Small::EndX, 0),
					};

					static constexpr uint8_t Charn[] PROGMEM
					{
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::Start, 1, AxisCenter - 1),
						Node(NodeEnum::Start, Lowercase::EndX - 1, AxisCenter - 1),
						Node(NodeEnum::Start, Lowercase::EndX, AxisCenter),
						Node(NodeEnum::End, Lowercase::EndX, FalseBottom),
						Node(NodeEnum::Start, 0, AxisCenter - 1),
						Node(NodeEnum::End, 0, AxisCenter),
					};

					static constexpr uint8_t CharO[] PROGMEM
					{
						Node(NodeEnum::Start, 1, 0),
						Node(NodeEnum::Start, Small::EndX - 1, 0),
						Node(NodeEnum::Start, Small::EndX, 1),
						Node(NodeEnum::Start, Small::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Small::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 0, 1),
						Node(NodeEnum::End, 1, 0)
					};

					static constexpr uint8_t Charo[] PROGMEM
					{
						Node(NodeEnum::Start, 1, AxisCenter - 1),
						Node(NodeEnum::Start, Lowercase::EndX - 1, AxisCenter - 1),
						Node(NodeEnum::Start, Lowercase::EndX, AxisCenter),
						Node(NodeEnum::Start, Lowercase::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Lowercase::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::End, 1, AxisCenter - 1)
					};

					static constexpr uint8_t CharP[] PROGMEM
					{
						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::Start, Small::EndX - 1, AxisCenter),
						Node(NodeEnum::Start, Small::EndX, AxisCenter - 1),
						Node(NodeEnum::Start, Small::EndX, 1),
						Node(NodeEnum::Start, Small::EndX - 1, 0),
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::End, 0, FalseBottom),
					};

					static constexpr uint8_t Charp[] PROGMEM
					{
						Node(NodeEnum::Start, 0, AxisMax),
						Node(NodeEnum::End, 0, AxisCenter - 1),
						Node(NodeEnum::Start, 0, AxisCenter - 1),
						Node(NodeEnum::Start, Lowercase::EndX - 1, AxisCenter - 1),
						Node(NodeEnum::Start, Lowercase::EndX, AxisCenter),
						Node(NodeEnum::Start, Lowercase::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Lowercase::EndX - 1, FalseBottom),
						Node(NodeEnum::End, 0, FalseBottom),
					};

					static constexpr uint8_t CharQ[] PROGMEM
					{
						Node(NodeEnum::Start, 1, 0),
						Node(NodeEnum::Start, Small::EndX - 1, 0),
						Node(NodeEnum::Start, Small::EndX, 1),
						Node(NodeEnum::Start, Small::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Small::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 0, 1),
						Node(NodeEnum::End, 1, 0),
						Node(NodeEnum::Start, Small::CenterX, FalseBottom - 1),
						Node(NodeEnum::End, Small::EndX, AxisMax)
					};

					static constexpr uint8_t Charq[] PROGMEM
					{
						Node(NodeEnum::Start, Lowercase::EndX, AxisCenter - 1),
						Node(NodeEnum::End, Lowercase::EndX, AxisMax),
						Node(NodeEnum::Start, Lowercase::EndX, AxisCenter),
						Node(NodeEnum::Start, Lowercase::EndX - 1, AxisCenter - 1),
						Node(NodeEnum::Start, 1, AxisCenter - 1),
						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, Lowercase::EndX - 1, FalseBottom),
						Node(NodeEnum::End, Lowercase::EndX, FalseBottom - 1),
					};

					static constexpr uint8_t CharR[] PROGMEM
					{
						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::Start, Small::EndX - 1, AxisCenter),
						Node(NodeEnum::Start, Small::EndX, AxisCenter - 1),
						Node(NodeEnum::Start, Small::EndX, 1),
						Node(NodeEnum::Start, Small::EndX - 1, 0),
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::End, 0, FalseBottom),
						Node(NodeEnum::Start, Small::CenterX + 1, AxisCenter),
						Node(NodeEnum::Start, Small::EndX, FalseBottom),
					};

					static constexpr uint8_t Charr[] PROGMEM
					{
						Node(NodeEnum::Start, 0, AxisCenter - 1),
						Node(NodeEnum::End, 0, FalseBottom),
						Node(NodeEnum::Start, 0, AxisCenter + 1),
						Node(NodeEnum::Start, Lowercase::EndX - 1, AxisCenter - 1),
						Node(NodeEnum::End, Lowercase::EndX, AxisCenter - 1),
					};

					static constexpr uint8_t CharS[] PROGMEM
					{
						Node(NodeEnum::Start, Small::EndX, 1),
						Node(NodeEnum::Start, Small::EndX - 1, 0),
						Node(NodeEnum::Start, Small::EndX - 1, 0),
						Node(NodeEnum::Start, 1, 0),
						Node(NodeEnum::Start, 0, 1),
						Node(NodeEnum::Start, 0, 2),
						Node(NodeEnum::Start, Small::EndX - 1, AxisCenter),
						Node(NodeEnum::Start, Small::EndX, AxisCenter + 2),
						Node(NodeEnum::Start, Small::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Small::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::End, 0, FalseBottom - 1)
					};

					static constexpr uint8_t Chars[] PROGMEM
					{
						Node(NodeEnum::Start, Lowercase::EndX, AxisCenter),
						Node(NodeEnum::Start, Lowercase::EndX - 1, AxisCenter - 1),
						Node(NodeEnum::Start, 1, AxisCenter - 1),
						Node(NodeEnum::Start, 0, AxisCenter),
						Node(NodeEnum::Start, Lowercase::EndX - 1, AxisCenter + 1),
						Node(NodeEnum::Start, Lowercase::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Lowercase::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, Lowercase::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::End, 0, FalseBottom - 1),
					};

					static constexpr uint8_t CharT[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::End, Small::EndX, 0),
						Node(NodeEnum::Start, Small::CenterX, 0),
						Node(NodeEnum::End, Small::CenterX, FalseBottom)
					};

					static constexpr uint8_t Chart[] PROGMEM
					{
						Node(NodeEnum::Start, 0, AxisCenter - 1),
						Node(NodeEnum::End, Lowercase::EndX, AxisCenter - 1),
						Node(NodeEnum::Start, Lowercase::CenterX - 1, 0),
						Node(NodeEnum::Start, Lowercase::CenterX - 1, FalseBottom - 1),
						Node(NodeEnum::Start, Lowercase::CenterX, FalseBottom),
						Node(NodeEnum::End, Lowercase::EndX - 1, FalseBottom)
					};

					static constexpr uint8_t CharU[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, Small::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, Small::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, Small::EndX, FalseBottom - 1),
						Node(NodeEnum::Start, Small::EndX, 0),
						Node(NodeEnum::End, Small::EndX, FalseBottom - 1)
					};

					static constexpr uint8_t Charu[] PROGMEM
					{
						Node(NodeEnum::Start, 0, AxisCenter - 1),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 0, FalseBottom - 1),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, Lowercase::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, Lowercase::EndX - 1, FalseBottom),
						Node(NodeEnum::Start, Lowercase::EndX , FalseBottom - 1),
						Node(NodeEnum::Start, Lowercase::EndX , AxisCenter - 1),
						Node(NodeEnum::Start, Lowercase::EndX, FalseBottom - 1),
						Node(NodeEnum::End, Lowercase::EndX, FalseBottom),
					};

					static constexpr uint8_t CharV[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::End, Largest::CenterX, FalseBottom),
						Node(NodeEnum::Start, Big::EndX, 0),
						Node(NodeEnum::End, Big::CenterX, FalseBottom)
					};

					static constexpr uint8_t Charv[] PROGMEM
					{
						Node(NodeEnum::Start, 0, AxisCenter - 1),
						Node(NodeEnum::End, Small::CenterX, FalseBottom),
						Node(NodeEnum::Start, Small::EndX, AxisCenter - 1),
						Node(NodeEnum::End, Small::CenterX, FalseBottom)
					};

					static constexpr uint8_t CharW[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, Largest::CenterX, AxisCenter - 1),
						Node(NodeEnum::Start,Largest::EndX - 1, FalseBottom),
						Node(NodeEnum::End, Largest::EndX, 0),
					};

					static constexpr uint8_t Charw[] PROGMEM
					{
						Node(NodeEnum::Start, 0, AxisCenter - 1),
						Node(NodeEnum::Start, 1, FalseBottom),
						Node(NodeEnum::Start, Largest::CenterX, AxisCenter - 1),
						Node(NodeEnum::Start, Largest::CenterX + 1, FalseBottom),
						Node(NodeEnum::End, Largest::EndX, AxisCenter - 1),
					};

					static constexpr uint8_t CharX[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::End, Small::EndX, FalseBottom),
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::End, Small::EndX, 0)
					};

					static constexpr uint8_t Charx[] PROGMEM
					{
						Node(NodeEnum::Start, 0, AxisCenter - 1),
						Node(NodeEnum::End, Lowercase::EndX, FalseBottom),
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::End, Lowercase::EndX, AxisCenter - 1)
					};

					static constexpr uint8_t CharY[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::End, Small::CenterX, AxisCenter + 1),
						Node(NodeEnum::Start, Small::EndX, 0),
						Node(NodeEnum::End, Small::CenterX, AxisCenter + 1),
						Node(NodeEnum::Start, Small::CenterX, AxisCenter) + 1,
						Node(NodeEnum::End, Small::CenterX, FalseBottom)
					};

					static constexpr uint8_t Chary[] PROGMEM
					{
						Node(NodeEnum::Start, 0, AxisMax),
						Node(NodeEnum::Start, Lowercase::CenterX - 1, FalseBottom),
						Node(NodeEnum::End, Lowercase::EndX, AxisCenter - 1),
						Node(NodeEnum::Start, Lowercase::CenterX - 1, FalseBottom),
						Node(NodeEnum::End, 0, AxisCenter - 1),
					};

					static constexpr uint8_t CharZ[] PROGMEM
					{
						Node(NodeEnum::Start, 0, 0),
						Node(NodeEnum::Start, Small::EndX, 0),
						Node(NodeEnum::Start, Small::EndX, 0),
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::End, Small::EndX, FalseBottom)
					};

					static constexpr uint8_t Charz[] PROGMEM
					{
						Node(NodeEnum::Start, 0, AxisCenter - 1),
						Node(NodeEnum::Start, Lowercase::EndX, AxisCenter - 1),
						Node(NodeEnum::Start, 0, FalseBottom),
						Node(NodeEnum::End, Lowercase::EndX, FalseBottom)
					};
				}
			}
		}
	}
}
#endif