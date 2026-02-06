#ifndef _EGFX_FRAMEWORK_BITMASK_FONT_MODEL_h
#define _EGFX_FRAMEWORK_BITMASK_FONT_MODEL_h

#include "../Model.h"
#include "../TemplateDrawer.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Bitmask
		{
			namespace Font
			{
				/// <summary>
				/// Glyph record for a bitmask font.
				/// Holds a pointer to packed 1bpp bitmask data and the glyph advance.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				template<typename dimension_t>
				struct Character
				{
					/// <summary>Pointer to packed 1bpp mask bytes for the glyph.</summary>
					const uint8_t* Bitmask;

					/// <summary>Horizontal advance for the glyph (pixels).</summary>
					dimension_t Advance;
				};

				namespace Contract
				{
					/// <summary>
					/// Bitmask font contract.
					///
					/// A bitmask font provides:
					/// - a reader type for accessing glyph bitmasks (RAM vs flash/PROGMEM)
					/// - fixed font dimensions (for layout and bounds)
					/// - a glyph lookup function returning a Character record
					/// </summary>
					struct Font
					{
						/// <summary>
						/// Bitmask reader type used to access glyph bytes.
						/// Flash is typical for AVR fonts stored in PROGMEM and falls back to RAM behavior on non-AVR.
						/// </summary>
						using ReaderType = Reader::Flash;

						/// <summary>Local space dimension type.</summary>
						using dimension_t = uint8_t;

						/// <summary>Nominal glyph width (pixels).</summary>
						static constexpr dimension_t Width = 1;

						/// <summary>Nominal glyph height (pixels).</summary>
						static constexpr dimension_t Height = 1;

						/// <summary>
						/// Returns the glyph record for a printable ASCII character.
						/// </summary>
						/// <param name="printableChar">ASCII printable character to look up.</param>
						/// <returns>
						/// Character record containing:
						/// - Bitmask: pointer to packed 1bpp glyph data (nullptr when unsupported)
						/// - Advance: horizontal advance (pixels); return 0 for unsupported characters
						/// </returns>
						static Character<dimension_t> GetCharacter(const char /*printableChar*/)
						{
							return Character<dimension_t>{ nullptr, 0 };
						}
					};
				}
			}
		}
	}
}
#endif