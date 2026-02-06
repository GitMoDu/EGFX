#ifndef _EGFX_MODULES_TERMINAL_WINDOW_VIEW_h
#define _EGFX_MODULES_TERMINAL_WINDOW_VIEW_h

#include "Constant.h"
#include "Drawable.h"
#include "Layout.h"

namespace Egfx
{
	namespace Modules
	{
		namespace TerminalWindow
		{
			namespace View
			{
				/// <summary>
				/// A view component that displays serial text output using a specified layout and font drawer.
				/// </summary>
				/// <typeparam name="ParentLayout">The layout type that defines how the text view is positioned and sized.</typeparam>
				/// <typeparam name="FontDrawerType">The type responsible for rendering font glyphs. Defaults to Definitions::DefaultFontDrawer.</typeparam>
				/// <typeparam name="FontHeight">The height of the font in pixels. Fixed-height fonts must specify their native height here. Defaults to Definitions::DefaultFont::Height.</typeparam>
				/// <typeparam name="FontWidth">The width of the font in pixels. Fixed-width fonts must specify their native width here. Defaults to Definitions::DefaultFont::Width.</typeparam>
				/// <typeparam name="Kerning">The horizontal spacing between characters in pixels. Defaults to 1.</typeparam>
				/// <typeparam name="LineSpacing">The vertical spacing between lines in pixels. Defaults to 1.</typeparam>
				template<typename ParentLayout
					, typename FontDrawerType = Definitions::DefaultFontDrawer
					, pixel_t FontWidth = Definitions::DefaultFont::Width
					, pixel_t FontHeight = Definitions::DefaultFont::Height
					, uint8_t Kerning = 1
					, uint8_t LineSpacing = 1>
				class SerialText : public Framework::View::AbstractView
				{
				public:
					using DrawableType = Drawable::SerialText<ParentLayout, FontDrawerType, FontWidth, FontHeight, Kerning, LineSpacing>;

					static constexpr auto CharactersPerLine = DrawableType::CharactersPerLine;
					static constexpr auto Lines = DrawableType::Lines;

				public:
					uint32_t AnimationDuration = 125000; // Animation duration in microseconds.

				private:
					DrawableType Drawable{};

				private:
					uint8_t CallIndex = 0;

				public:
					SerialText() : Framework::View::AbstractView() {}

					~SerialText() = default;

					// Expose Print interface for external writing.
					Print& Serial()
					{
						return Drawable;
					}

					// Expose underlying font drawer for configuration.
					FontDrawerType& FontDrawer()
					{
						return Drawable.TextWriter;
					}

				protected:
					void ViewStep(const uint32_t /*frameTime*/, const uint16_t /*frameCounter*/) override
					{
						const uint32_t elapsed = Drawable.GetNewLineElapsedTime();

						if (AnimationDuration > 0 && elapsed < AnimationDuration)
						{
							// Animate offset Y for smooth line scroll.
							Drawable.OffsetY = FontHeight - static_cast<int16_t>((elapsed * FontHeight) / AnimationDuration);
						}
						else
						{
							// Animation complete - reset offset Y.
							Drawable.OffsetY = 0;
						}
					}

					bool Draw(IFrameBuffer* frame) override
					{
						// Draw a text line from buffer each draw call.
						Drawable.Draw(frame, CallIndex);
						CallIndex++; // Advance call index and determine if cycle is complete.
						if (CallIndex >= Drawable.Lines + 1)
						{
							CallIndex = 0;
							return true;
						}
						return false;
					}
				};
			}
		}
	}
}
#endif