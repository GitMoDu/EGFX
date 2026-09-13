#ifndef _EGFX_MODULES_TERMINAL_WINDOW_CONSTANT_h
#define _EGFX_MODULES_TERMINAL_WINDOW_CONSTANT_h

#include "../../EgfxFramework.h"
#include "../../Framework/Assets/Font/Bitmask/Contact/FontType.h"

namespace Egfx
{
	namespace Modules
	{
		namespace TerminalWindow
		{
			namespace Definitions
			{
				template<size_t MaximumValue>
				struct CountType
				{
					using type = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						uint8_t, uint16_t, (MaximumValue <= UINT8_MAX)>::type;
				};

				using DefaultFont = Framework::Assets::Font::Bitmask::Contact::FontType5x6;

				using DefaultGlyphStyle = Framework::Image::TemplateImageStyle<
					Framework::Layout::AlignmentEnum::TopLeft,
					true, RGB_COLOR_BLACK,
					1, 1
				>;

				/// <summary>
				/// Configuration settings for the terminal window.
				/// </summary>
				/// <typeparam name="fontWidth">Font width in pixels. Must match the width of the font used for rendering text in the terminal window.</typeparam>
				/// <typeparam name="fontHeight">Font height in pixels. Must match the height of the font used for rendering text in the terminal window.</typeparam>
				/// <typeparam name="kerning">Kerning value in pixels.</typeparam>
				/// <typeparam name="lineSpacing">Line spacing value in pixels.</typeparam>
				/// <typeparam name="newLineAnimationDuration">New line animation duration in microseconds.</typeparam>
				/// <typeparam name="characterRevealInterval">Character reveal interval in microseconds.</typeparam>
				/// <typeparam name="doubleBuffered">Indicates whether double buffering is enabled. 
				/// Double buffering ensures that the terminal display is updated smoothly without flickering, especially during rapid text updates.
				/// Can be turned off to save RAM without crashes, but may result in flickering during updates.
				/// </typeparam>
				/// <typeparam name="cursorBlinkPeriod">Cursor blink period in microseconds.</typeparam>
				template<uint16_t fontWidth,
					uint16_t fontHeight,
					uint16_t kerning = 1 + (fontWidth / 5),
					uint16_t lineSpacing = 2 + (fontHeight / 6),
					uint32_t newLineAnimationDuration = 180000,
					uint32_t characterRevealInterval = 5000,
					bool doubleBuffered = true,
					uint32_t cursorBlinkPeriod = 400000
				>
				struct TerminalConfig
				{
					static constexpr uint16_t FontWidth = fontWidth;
					static constexpr uint16_t FontHeight = fontHeight;
					static constexpr uint16_t Kerning = kerning;
					static constexpr uint16_t LineSpacing = lineSpacing;
					static constexpr uint32_t NewLineAnimationDuration = newLineAnimationDuration;
					static constexpr uint32_t CharacterRevealInterval = characterRevealInterval;
					static constexpr bool DoubleBuffered = doubleBuffered;
					static constexpr uint32_t CursorBlinkPeriod = cursorBlinkPeriod;
				};
			}

		}
	}
}
#endif