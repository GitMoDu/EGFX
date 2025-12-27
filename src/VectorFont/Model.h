#ifndef _EGFX_VECTOR_FONT_h
#define _EGFX_VECTOR_FONT_h

#include "../VectorGraphics/Model.h"
#include "../VectorGraphics/TemplateShadeDecoder.h"
#include "../Model/AsciiDefinition.h"

namespace Egfx
{
	/// <summary>
	/// Vector font utilities.
	/// - Glyphs are defined on an 8×8 local grid (VectorGraphics Model8) using compact vector nodes.
	/// - Rendering scales local coordinates to the configured font Width/Height.
	/// - KerningWidth is applied between glyphs (or space/tab), not after the last glyph.
	/// </summary>
	namespace VectorFont
	{
		/// <summary>
		/// Glyph metadata for a VectorNode8x8 character:
		/// - Nodes: pointer to encoded nodes.
		/// - Count: node count.
		/// - Width: local width in the 8×8 grid (0..AxisMax).
		/// </summary>
		struct CharacterSet8x8
		{
			struct VectorModel : VectorGraphics::VectorNode8x8 {};

			const uint8_t* Nodes;
			const uint8_t Count;
			const uint8_t Width;
		};

		/// <summary>
		/// Glyph metadata for a VectorNode65x65 character:
		/// - Nodes: pointer to encoded nodes.
		/// - Count: node count.
		/// - Width: local width in the 65×65 grid (0..AxisMax).
		/// </summary>
		struct CharacterSet65x65
		{
			struct VectorModel : VectorGraphics::VectorNode65x65 {};

			const uint16_t* Nodes;
			const uint8_t Count;
			const uint8_t Width;
		};

		/// <summary>
		/// Glyph metadata for a VectorNode128x128 character:
		/// - Nodes: pointer to encoded nodes.
		/// - Count: node count.
		/// - Width: local width in the 128×128 grid (0..AxisMax).
		/// </summary>
		struct CharacterSet128x128
		{
			struct VectorModel : VectorGraphics::VectorNode128x128 {};

			const uint16_t* Nodes;
			const uint8_t Count;
			const uint8_t Width;
		};
	}
}
#endif