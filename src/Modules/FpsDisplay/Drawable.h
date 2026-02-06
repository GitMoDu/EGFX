#ifndef _EGFX_MODULES_FPS_DISPLAY_LAYOUT_DRAWABLE_h
#define _EGFX_MODULES_FPS_DISPLAY_LAYOUT_DRAWABLE_h

#include "Layout.h"
#include "Constant.h"

namespace Egfx
{
	namespace Modules
	{
		namespace FpsDisplay
		{
			namespace Drawable
			{
				template<typename Layout
					, typename FontDrawerType = typename Definitions::AutoFontSelector<Layout>::Drawer
					, FpsDrawerPosition fpsDrawerPosition = FpsDrawerPosition::TopRight
				>
				class FpsText
				{
				private:
					Framework::Text::TemplateWriter<Layout, FontDrawerType> TextDrawer{};

				public:
					uint16_t FrameRate = 0;

				public:
					FpsText() = default;
					~FpsText() = default;

					void Draw(IFrameBuffer* frame)
					{
#if defined(ARDUINO_ARCH_AVR)
						const auto* label = reinterpret_cast<const __FlashStringHelper*>(Strings::LabelText);
#else
						const auto* label = Strings::LabelText;
#endif
						const pixel_t valueWidth = TextDrawer.GetNumberWidth(FrameRate);
						const pixel_t spaceWidth = TextDrawer.GetSpaceWidth();

						switch (fpsDrawerPosition)
						{
						case FpsDrawerPosition::TopLeft:
							TextDrawer.Write(frame, Layout::X(), Layout::Y(), FrameRate);
							TextDrawer.Write(frame, Layout::X() + valueWidth + spaceWidth, Layout::Y(), label);
							break;
						case FpsDrawerPosition::TopRight:
						{
							const pixel_t labelWidth = TextDrawer.GetTextWidth(label);
							TextDrawer.Write(frame, Layout::X() + Layout::Width() - labelWidth - valueWidth - spaceWidth, Layout::Y(), FrameRate);
							TextDrawer.Write(frame, Layout::X() + Layout::Width() - labelWidth, Layout::Y(), label);
						}
						break;
						case FpsDrawerPosition::BottomLeft:
							TextDrawer.Write(frame, Layout::X(), Layout::Y() + Layout::Height() - TextDrawer.GetFontHeight(), FrameRate);
							TextDrawer.Write(frame, Layout::X() + valueWidth + spaceWidth, Layout::Y() + Layout::Height() - TextDrawer.GetFontHeight(), label);
							break;
						case FpsDrawerPosition::BottomRight:
						{
							const pixel_t labelWidth = TextDrawer.GetTextWidth(label);
							TextDrawer.Write(frame, Layout::X() + Layout::Width() - labelWidth, Layout::Y() + Layout::Height() - TextDrawer.GetFontHeight(), label);
							TextDrawer.Write(frame, Layout::X() + Layout::Width() - labelWidth - valueWidth - spaceWidth, Layout::Y() + Layout::Height() - TextDrawer.GetFontHeight(), FrameRate);
						}
						break;
						default:
							break;
						}
					}
				};
			}
		}
	}
}
#endif