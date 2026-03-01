#ifndef _LOLA_GRAPHICS_MODULES_TIMECODE_TEXT_DRAWABLE_h
#define _LOLA_GRAPHICS_MODULES_TIMECODE_TEXT_DRAWABLE_h

#include <EgfxAssets.h>

#include "Constant.h"
#include "Layout.h"

namespace Egfx
{
	namespace Modules
	{
		namespace TimecodeText
		{
			namespace Drawable
			{
				/// Timecode (fixed 6-digit) drawable (HH:MM:SS or MM:SS:cs).
				template<typename ParentLayout,
					typename CodeFontDrawerType = Framework::Assets::Font::Code::RawBot<>
				>
				struct Timecode
				{
				private:
					using Digit1Layout = Layout::Digit1<ParentLayout>;
					using Digit2Layout = Layout::Digit2<ParentLayout>;
					using MinutesDigit1Layout = Layout::Digit3<ParentLayout>;
					using MinutesDigit2Layout = Layout::Digit4<ParentLayout>;
					using SecondsDigit1Layout = Layout::Digit5<ParentLayout>;
					using SecondsDigit2Layout = Layout::Digit6<ParentLayout>;
					using Separator1Layout = Layout::Separator1<ParentLayout>;
					using Separator2Layout = Layout::Separator2<ParentLayout>;

				private:
					Framework::Text::TemplateWriter<ParentLayout, CodeFontDrawerType> TextDrawer{};

					uint8_t Digit1 = 0;
					uint8_t Digit2 = 0;
					uint8_t Digit3 = 0;
					uint8_t Digit4 = 0;
					uint8_t Digit5 = 0;
					uint8_t Digit6 = 0;

				public:
					PresentModeEnum CurrentMode = PresentModeEnum::Duration;

				public:
					Timecode()
					{
						TextDrawer.SetFontDimensions(Digit1Layout::Width(), Digit1Layout::Height());
					}

					Framework::Text::TemplateWriter<ParentLayout, CodeFontDrawerType>& GetTextDrawer()
					{
						return TextDrawer;
					}

					// Back-compat with older view APIs.
					void SetDuration(const uint32_t totalSeconds)
					{
						SetSeconds(totalSeconds);
					}

					void SetMilliseconds(const uint32_t totalMilliseconds)
					{
						if (totalMilliseconds >= Durations::MaxDurationMilliseconds)
						{
							Digit1 = 9;
							Digit2 = 9;
							Digit3 = 5;
							Digit4 = 9;
							Digit5 = 9;
							Digit6 = 9;
						}
						else
						{
							const uint32_t minutes = (totalMilliseconds % 3600000) / 60000;
							const uint32_t seconds = (totalMilliseconds % 60000) / 1000;
							const uint32_t milliseconds = totalMilliseconds % 1000;
							const uint8_t centiseconds = milliseconds / 10;
							Digit6 = static_cast<uint8_t>(centiseconds % 10);
							Digit5 = static_cast<uint8_t>(centiseconds / 10);
							Digit4 = static_cast<uint8_t>(seconds % 10);
							Digit3 = static_cast<uint8_t>(seconds / 10);
							Digit2 = static_cast<uint8_t>(minutes % 10);
							Digit1 = static_cast<uint8_t>(minutes / 10);
						}
					}

					void SetSeconds(const uint32_t totalSeconds)
					{
						if (totalSeconds >= Durations::MaxDurationSeconds)
						{
							Digit1 = 9;
							Digit2 = 9;
							Digit3 = 5;
							Digit4 = 9;
							Digit5 = 5;
							Digit6 = 9;
						}
						else
						{
							const uint32_t hours = totalSeconds / 3600;
							const uint32_t minutes = (totalSeconds % 3600) / 60;
							const uint32_t seconds = totalSeconds % 60;
							Digit1 = static_cast<uint8_t>(hours / 10);
							Digit2 = static_cast<uint8_t>(hours % 10);
							Digit3 = static_cast<uint8_t>(minutes / 10);
							Digit4 = static_cast<uint8_t>(minutes % 10);
							Digit5 = static_cast<uint8_t>(seconds / 10);
							Digit6 = static_cast<uint8_t>(seconds % 10);
						}
					}

					void Draw(IFrameBuffer* frame)
					{
						switch (CurrentMode)
						{
						case PresentModeEnum::Invisible:
							return;
						case PresentModeEnum::NoDuration:
							TextDrawer.Write(frame, Digit1Layout::X(), Digit1Layout::Y(), Texts::UnknownChar);
							TextDrawer.Write(frame, Digit2Layout::X(), Digit2Layout::Y(), Texts::UnknownChar);
							TextDrawer.Write(frame, MinutesDigit1Layout::X(), MinutesDigit1Layout::Y(), Texts::UnknownChar);
							TextDrawer.Write(frame, MinutesDigit2Layout::X(), MinutesDigit2Layout::Y(), Texts::UnknownChar);
							TextDrawer.Write(frame, SecondsDigit1Layout::X(), SecondsDigit1Layout::Y(), Texts::UnknownChar);
							TextDrawer.Write(frame, SecondsDigit2Layout::X(), SecondsDigit2Layout::Y(), Texts::UnknownChar);
							break;
						case PresentModeEnum::Duration:
							TextDrawer.Write(frame, Digit1Layout::X(), Digit1Layout::Y(), Digit1);
							TextDrawer.Write(frame, Digit2Layout::X(), Digit2Layout::Y(), Digit2);
							TextDrawer.Write(frame, MinutesDigit1Layout::X(), MinutesDigit1Layout::Y(), Digit3);
							TextDrawer.Write(frame, MinutesDigit2Layout::X(), MinutesDigit2Layout::Y(), Digit4);
							TextDrawer.Write(frame, SecondsDigit1Layout::X(), SecondsDigit1Layout::Y(), Digit5);
							TextDrawer.Write(frame, SecondsDigit2Layout::X(), SecondsDigit2Layout::Y(), Digit6);
							break;
						default:
							break;
						}

						DrawSeparator(frame, Separator1Layout::X());
						DrawSeparator(frame, Separator2Layout::X());
					}

				private:
					void DrawSeparator(IFrameBuffer* frame, const pixel_t x)
					{
						static constexpr pixel_t padX = Separator1Layout::Width() / 3;
						static constexpr pixel_t dotWidth = (Separator1Layout::Width() * 3) / 4 - padX;
						static constexpr pixel_t thickness = 2;

						static constexpr pixel_t y1 = Separator1Layout::Y() + (Separator1Layout::Height() / 4);
						static constexpr pixel_t y2 = Separator1Layout::Y() + ((Separator1Layout::Height() * 5) / 8);

						const pixel_t x1 = x + padX;
						const pixel_t x2 = x + padX + dotWidth;

						const rgb_color_t color = TextDrawer.ColorSource.Source(0, 0);
						frame->RectangleFill(color, x1, y1, x2, y1 + thickness);
						frame->RectangleFill(color, x1, y2, x2, y2 + thickness);
					}
				};
			}
		}
	}
}
#endif