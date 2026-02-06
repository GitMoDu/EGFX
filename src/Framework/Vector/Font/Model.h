#ifndef _EGFX_FRAMEWORK_VECTOR_FONT_MODEL_h
#define _EGFX_FRAMEWORK_VECTOR_FONT_MODEL_h

#include "../Model.h"
#include "../TemplateDrawer.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Vector
		{
			/// <summary>
			/// Vector font model types.
			///
			/// A vector font glyph is defined as a stream of packed vector nodes plus metadata describing the
			/// glyph's local width. Rendering scales local-space grid coordinates to the configured draw size.
			/// </summary>
			namespace Font
			{
				/// <summary>
				/// Glyph record for an 8x8 vector model.
				/// </summary>
				struct CharacterSet8x8
				{
					/// <summary>Vector node model used to decode and scale nodes.</summary>
					struct VectorModel : Vector::VectorNode8x8 {};

					/// <summary>Pointer to packed vector nodes.</summary>
					const uint8_t* Nodes;

					/// <summary>Number of nodes in the stream.</summary>
					const uint8_t Count;

					/// <summary>Glyph local width on the model grid (pixels).</summary>
					const uint8_t Width;
				};

				/// <summary>
				/// Glyph record for a 65x65 (shift-optimized) vector model.
				/// </summary>
				struct CharacterSet65x65
				{
					/// <summary>Vector node model used to decode and scale nodes.</summary>
					struct VectorModel : Vector::VectorNode65x65 {};

					/// <summary>Pointer to packed vector nodes.</summary>
					const uint16_t* Nodes;

					/// <summary>Number of nodes in the stream.</summary>
					const uint8_t Count;

					/// <summary>Glyph local width on the model grid (pixels).</summary>
					const uint8_t Width;
				};

				/// <summary>
				/// Glyph record for a 128x128 (full range) vector model.
				/// </summary>
				struct CharacterSet128x128
				{
					/// <summary>Vector node model used to decode and scale nodes.</summary>
					struct VectorModel : Vector::VectorNode128x128 {};

					/// <summary>Pointer to packed vector nodes.</summary>
					const uint16_t* Nodes;

					/// <summary>Number of nodes in the stream.</summary>
					const uint8_t Count;

					/// <summary>Glyph local width on the model grid (pixels).</summary>
					const uint8_t Width;
				};
			}
		}
	}
}
#endif