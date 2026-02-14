#ifndef _EGFX_FRAMEWORK_LAYOUT_TRANSLATE_h
#define _EGFX_FRAMEWORK_LAYOUT_TRANSLATE_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Layout
		{
			/// <summary>
			/// Translates a layout by a fixed pixel offset, preserving its size.
			/// </summary>
			/// <typeparam name="ParentLayout">Parent layout providing X/Y/Width/Height.</typeparam>
			/// <typeparam name="ShiftX">X offset in pixels (added to ParentLayout X).</typeparam>
			/// <typeparam name="ShiftY">Y offset in pixels (added to ParentLayout Y).</typeparam>
			template<typename ParentLayout,
				pixel_t ShiftX = 0,
				pixel_t ShiftY = 0
			>
			struct Translate
			{
				/// <summary>Translated X position.</summary>
				static constexpr pixel_t X() { return ParentLayout::X() + ShiftX; }

				/// <summary>Translated Y position.</summary>
				static constexpr pixel_t Y() { return ParentLayout::Y() + ShiftY; }

				/// <summary>Unchanged width.</summary>
				static constexpr pixel_t Width() { return ParentLayout::Width(); }

				/// <summary>Unchanged height.</summary>
				static constexpr pixel_t Height() { return ParentLayout::Height(); }
			};
		}
	}
}
#endif