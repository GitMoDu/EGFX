#ifndef _INTEGERGLASS_FRAMEWORK_TEXT_VECTOR_MODEL_h
#define _INTEGERGLASS_FRAMEWORK_TEXT_VECTOR_MODEL_h

#include "../Model.h"
#include "../../Vector/Model.h"

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Text
		{
			namespace Vector
			{
				static constexpr Framework::Vector::thickness_scale_t ThicknessScale = (Framework::Vector::THICKNESS_SCALE_1X / 2);

				/// <summary>
				/// A vector glyph is a subset of vector images.
				/// Colors are allowed, but the glyphs are typically monochrome.
				/// Thickness scaling is fixed at 1/2 fraction (64). Inset weight is fixed at 1.
				/// Glyphs that require more than 1 unit of inset must take that into account when designing the glyphs, or they will be clipped.
				/// </summary>
				namespace Glyph15x15
				{
					using model_t = Framework::Vector::Detail::template_model_t<uint8_t, uint8_t, 4, 14>;
					using axis_t = typename model_t::axis_t;
					using opcode_t = typename model_t::opcode_t;
					using source_t = typename model_t::source_t;
					using packed_t = typename model_t::Node::packed_t;

					static constexpr uint8_t AxisSize = model_t::AxisSize;
					static constexpr axis_t AxisMask = model_t::AxisMask;
					static constexpr axis_t AxisMax = model_t::AxisMax;
					static constexpr axis_t OpCodeAxis = model_t::OpCodeAxis;
					static constexpr opcode_t OpCodeCount = model_t::OpCodeCount;

					struct Glyph
					{
						const packed_t* Nodes;
						const size_t Count;
						const axis_t Width;
						const axis_t Height;
					};

					namespace Contract
					{
						struct GlyphSource
						{
							using glyph_t = Glyph15x15::Glyph;
							static constexpr bool IsMonospace = false;
							static constexpr Framework::DataSourceTypeEnum DataSourceType = Framework::DataSourceTypeEnum::Ram;

							glyph_t GetGlyph(const uint8_t /*character*/) const
							{
								return glyph_t{ nullptr, 0, 0, 0 };
							}

							uint8_t GetFontWidth() const { return 0; }
							uint8_t GetFontHeight() const { return 0; }

							uint8_t GetAdvance(const char /*character*/) const { return 0; }
						};
					}
				};
			}
		}
	}
}
#endif