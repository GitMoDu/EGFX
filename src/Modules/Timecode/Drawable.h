#ifndef _EGFX_MODULES_TIMECODE_DRAWABLE_h
#define _EGFX_MODULES_TIMECODE_DRAWABLE_h

#include "Constant.h"
#include "Layout.h"

#include "../../Modules/Kit/Include.h"
#include "../../Framework/Text/Vector/Drawable.h"

namespace Egfx
{
	namespace Modules
	{
		namespace Timecode
		{
			namespace Drawable
			{
				/// Timecode drawable (HH:MM:SS or MM:SS:cs).
				template<typename dimension_t,
					typename ParentLayout,
					typename GlyphSourceType = Timecode::DefaultFont,
					typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
					typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>
				>
				struct Timecode
				{
				private:
					using GlyphStyle = Framework::Image::TemplateImageStyle<
						Framework::Layout::AlignmentEnum::MiddleCenter
					>;
					using DigitLayout = Layout::Digit<ParentLayout>;

					using Digit1Layout = Layout::Digit1<ParentLayout>;
					using Digit2Layout = Layout::Digit2<ParentLayout>;
					using MinutesDigit1Layout = Layout::Digit3<ParentLayout>;
					using MinutesDigit2Layout = Layout::Digit4<ParentLayout>;
					using SecondsDigit1Layout = Layout::Digit5<ParentLayout>;
					using SecondsDigit2Layout = Layout::Digit6<ParentLayout>;
					using Separator1Layout = Layout::Separator1<ParentLayout>;
					using Separator2Layout = Layout::Separator2<ParentLayout>;

				private:
					using TextDrawerType = Framework::Text::Vector::Drawable::Text<
						dimension_t, DigitLayout, GlyphSourceType, GlyphStyle, ColorShaderType, TransformShaderType>;

					TextDrawerType TextDrawer{};

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
						TextDrawer.SetFontSize(
							DigitLayout::Width(),
							DigitLayout::Height(), false);
						TextDrawer.SetKerningWidth(0, false);
					}
					~Timecode() = default;

					TextDrawerType& GetTextDrawer()
					{
						return TextDrawer;
					}

					// MM:SS:cs (centiseconds)
					void SetMinutes(const uint32_t totalMilliseconds)
					{
						if (totalMilliseconds >= Durations::MaxMinutesTimecodeMilliseconds)
						{
							// 99:59:99
							Digit1 = 9;
							Digit2 = 9;
							Digit3 = 5;
							Digit4 = 9;
							Digit5 = 9;
							Digit6 = 9;
						}
						else
						{
							const uint32_t minutes = totalMilliseconds / 60000;
							const uint32_t seconds = (totalMilliseconds % 60000) / 1000;
							const uint32_t milliseconds = totalMilliseconds % 1000;
							const uint8_t centiseconds = static_cast<uint8_t>(milliseconds / 10);

							Digit6 = static_cast<uint8_t>(centiseconds % 10);
							Digit5 = static_cast<uint8_t>(centiseconds / 10);
							Digit4 = static_cast<uint8_t>(seconds % 10);
							Digit3 = static_cast<uint8_t>(seconds / 10);
							Digit2 = static_cast<uint8_t>(minutes % 10);
							Digit1 = static_cast<uint8_t>(minutes / 10);
						}
					}

					void SetHours(const uint32_t totalSeconds)
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

					bool IsVisible() const
					{
						return TextDrawer.IsVisible();
					}


					bool Draw(IFrameBuffer* frame)
					{
						if (CurrentMode == PresentModeEnum::Invisible)
							return true;

						if (CurrentMode == PresentModeEnum::NoDuration)
						{
							DrawCharacter(frame, Digit1Layout{}, Texts::UnknownChar);
							DrawCharacter(frame, Digit2Layout{}, Texts::UnknownChar);
							DrawCharacter(frame, MinutesDigit1Layout{}, Texts::UnknownChar);
							DrawCharacter(frame, MinutesDigit2Layout{}, Texts::UnknownChar);
							DrawCharacter(frame, SecondsDigit1Layout{}, Texts::UnknownChar);
							DrawCharacter(frame, SecondsDigit2Layout{}, Texts::UnknownChar);
						}
						else
						{
							DrawCharacter(frame, Digit1Layout{}, static_cast<char>('0' + Digit1));
							DrawCharacter(frame, Digit2Layout{}, static_cast<char>('0' + Digit2));
							DrawCharacter(frame, MinutesDigit1Layout{}, static_cast<char>('0' + Digit3));
							DrawCharacter(frame, MinutesDigit2Layout{}, static_cast<char>('0' + Digit4));
							DrawCharacter(frame, SecondsDigit1Layout{}, static_cast<char>('0' + Digit5));
							DrawCharacter(frame, SecondsDigit2Layout{}, static_cast<char>('0' + Digit6));
						}

						DrawCharacter(frame, Separator1Layout{}, ':');
						DrawCharacter(frame, Separator2Layout{}, ':');

						return true;
					}

					void SetBounds(const pixel_t left, const pixel_t top,
						const pixel_t right, const pixel_t bottom)
					{
						TextDrawer.SetBounds(left, top, right, bottom);
					}

					void SetTranslation(const int16_t x, const int16_t y)
					{
						TextDrawer.SetTranslation(x, y);
					}

				protected:
					dimension_t GetBoundsLeft() const { return TextDrawer.GetBoundsLeft(); }
					dimension_t GetBoundsTop() const { return TextDrawer.GetBoundsTop(); }
					dimension_t GetBoundsRight() const { return TextDrawer.GetBoundsRight(); }
					dimension_t GetBoundsBottom() const { return TextDrawer.GetBoundsBottom(); }

				private:
					template<typename CharacterLayout>
					void DrawCharacter(IFrameBuffer* frame, CharacterLayout, const char character)
					{
						TextDrawer.SetOffset(
							static_cast<int16_t>(CharacterLayout::X() - ParentLayout::X()),
							static_cast<int16_t>(CharacterLayout::Y() - ParentLayout::Y()), false);
						TextDrawer.SetText(character, true);
						TextDrawer.Draw(frame);
					}

				};
			}
		}
	}
}
#endif