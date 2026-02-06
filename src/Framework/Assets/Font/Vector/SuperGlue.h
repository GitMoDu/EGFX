#ifndef _EGFX_VECTOR_FONTS_SUPER_GLUE_h
#define _EGFX_VECTOR_FONTS_SUPER_GLUE_h

#include "../../VectorGraphics/Model.h"

namespace Egfx
{
	namespace VectorFonts
	{
		namespace SuperGlue
		{
			using namespace VectorGraphics;
			using namespace VectorFont;

			namespace VectorData
			{
				namespace InnerDimensions
				{
				}

				using namespace InnerDimensions;
			}

			struct FontType
			{
				using CharacterSetType = CharacterSet8x8;
				using VectorModel = CharacterSetType::VectorModel;
				static constexpr VectorModel::axis_t SpaceWidth = 4;
				static constexpr VectorModel::axis_t TabWidth = 7;
				static constexpr bool Monospace = false;

				static CharacterSetType GetCharacterSet(const char character)
				{
					switch (character)
					{
					default:
						return CharacterSet8x8{ nullptr, 0, 0 };
					}
				}
			};
		}
	}
}

#endif

