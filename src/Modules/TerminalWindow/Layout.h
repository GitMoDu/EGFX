#ifndef _EGFX_MODULES_TERMINAL_WINDOW_LAYOUT_h
#define _EGFX_MODULES_TERMINAL_WINDOW_LAYOUT_h

#include "Constant.h"

namespace Egfx
{
	namespace Modules
	{
		namespace TerminalWindow
		{
			namespace Layout
			{
				template<typename ParentLayout,
					typename GlyphSourceType = Definitions::DefaultFont,
					typename GlyphStyle = Definitions::DefaultGlyphStyle,
					uint8_t Kerning = 1,
					uint8_t LineSpacing = 1>
				struct TerminalLayout : ParentLayout
				{
					using dimension_t = typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t;

					static constexpr dimension_t FontWidth = static_cast<dimension_t>(GlyphSourceType::GetFontWidth() * GlyphStyle::ScaleX);
					static constexpr dimension_t FontHeight = static_cast<dimension_t>(GlyphSourceType::GetFontHeight() * GlyphStyle::ScaleY);
					static_assert(FontWidth > 0 && FontHeight > 0, "Terminal font dimensions must be positive.");
					static_assert(ParentLayout::Width() >= FontWidth && ParentLayout::Height() >= FontHeight,
						"Terminal layout must fit at least one glyph.");

					static constexpr size_t CharactersPerLine = (size_t(ParentLayout::Width()) + Kerning) / (FontWidth + Kerning);
					static constexpr size_t Lines = (size_t(ParentLayout::Height()) + LineSpacing) / (FontHeight + LineSpacing);

					static constexpr dimension_t LineY(const uint8_t line)
					{
						return static_cast<dimension_t>(ParentLayout::Height() - FontHeight
							- (Lines - 1 - line) * (FontHeight + LineSpacing));
					}
				};
			}
		}
	}
}
#endif