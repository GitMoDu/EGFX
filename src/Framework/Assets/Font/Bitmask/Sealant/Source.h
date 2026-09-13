#ifndef _EGFX_FRAMEWORK_ASSETS_FONT_BITMASK_SEALANT_SOURCE_h
#define _EGFX_FRAMEWORK_ASSETS_FONT_BITMASK_SEALANT_SOURCE_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace Font
			{
				namespace Bitmask
				{
					namespace Sealant
					{
						// Atlas: MaxWidth: 3 Height: 5 SpaceWidth: 1
						namespace Source3x5
						{
							static constexpr uint8_t Width = 3;
							static constexpr uint8_t Height = 5;
							static constexpr uint8_t SpaceWidth = 1;

							static constexpr uint8_t Atlas[] PROGMEM
							{
								// Glyph: LetterF Width: 3 row-padded
								0xE0, 0x80, 0xC0, 0x80, 0x80,
								// Glyph: LetterP Width: 3 row-padded
								0xE0, 0xA0, 0xE0, 0x80, 0x80,
								// Glyph: LetterS Width: 3 row-padded
								0x60, 0x80, 0x40, 0x20, 0xC0,
								// Glyph: Number0 Width: 3 row-padded
								0x40, 0xA0, 0xA0, 0xA0, 0x40,
								// Glyph: Number1 Width: 3 row-padded
								0xC0, 0x40, 0x40, 0x40, 0xE0,
								// Glyph: Number2 Width: 3 row-padded
								0xE0, 0x20, 0xE0, 0x80, 0xE0,
								// Glyph: Number3 Width: 3 row-padded
								0xE0, 0x20, 0x60, 0x20, 0xE0,
								// Glyph: Number4 Width: 3 row-padded
								0xA0, 0xA0, 0xE0, 0x20, 0x20,
								// Glyph: Number5 Width: 3 row-padded
								0xE0, 0x80, 0xE0, 0x20, 0xE0,
								// Glyph: Number6 Width: 3 row-padded
								0xE0, 0x80, 0xE0, 0xA0, 0xE0,
								// Glyph: Number7 Width: 3 row-padded
								0xE0, 0x20, 0x40, 0x40, 0x80,
								// Glyph: Number8 Width: 3 row-padded
								0xE0, 0xA0, 0xE0, 0xA0, 0xE0,
								// Glyph: Number9 Width: 3 row-padded
								0xE0, 0xA0, 0xE0, 0x20, 0x20,
							};
						}
					}
				}
			}
		}
	}
}
#endif