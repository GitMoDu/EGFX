#ifndef _EGFX_FRAMEWORK_WIDGETS_SERIAL_TEXT_h
#define _EGFX_FRAMEWORK_WIDGETS_SERIAL_TEXT_h

#include <Print.h>
#include "../../EgfxDrawers.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Widgets
		{
			namespace SerialText
			{
				namespace Definitions
				{
					using DefaultFont = BitmaskFonts::Plastic::FontType3x5;
					using DefaultFontDrawer = BitmaskFont::TemplateColorFontDrawer<DefaultFont, FontText::FullColorSource>;
				}

				// Layout helper for serial text.
				template<typename ParentLayout>
				struct Layout
				{
					static constexpr pixel_t X() { return ParentLayout::X(); }
					static constexpr pixel_t Y() { return ParentLayout::Y(); }
					static constexpr pixel_t Width() { return ParentLayout::Width(); }
					static constexpr pixel_t Height() { return ParentLayout::Height(); }
				};

				namespace Drawables
				{
					template<typename Layout
						, typename FontDrawerType = Definitions::DefaultFontDrawer
						, pixel_t FontWidth = Definitions::DefaultFont::Width
						, pixel_t FontHeight = Definitions::DefaultFont::Height
						, uint8_t Kerning = 1
						, uint8_t LineSpacing = 1>
					class SerialText : public Print
					{
					public:
						static constexpr auto CharactersPerLine = Layout::Width() / (FontWidth + Kerning);
						static constexpr auto Lines = Layout::Height() / (FontHeight + LineSpacing);

					private:
						/// <summary>
						/// The total size of the character buffer.
						/// </summary>
						static constexpr size_t BufferSize = ((size_t)Lines) * CharactersPerLine;

						/// <summary>
						/// Text buffer to hold the serial text output.
						/// +1 line for animated new line.
						/// +1 character per line for null-terminator.
						/// </summary>
						uint8_t TextBuffer[Lines + 1][CharactersPerLine + 1]{};

					public:
						FontText::TemplateTextWriter<FontDrawerType, Layout> TextWriter{};

					private:
						uint32_t LastNewLineTime = 0;
						pixel_t CursorX = 0;

					public:
						pixel_t OffsetY = 0; // Animatable offset Y.

					public:
						SerialText() = default;
						~SerialText() = default;

					public: // Print interface
						int availableForWrite()
						{
							return OffsetY == 0;
						}

						size_t write(uint8_t character) final
						{
							switch (static_cast<uint8_t>(character))
							{
							case uint8_t(AsciiDefinition::Control::NUL):
								return 0;
							case uint8_t(AsciiDefinition::Control::CR):
							case uint8_t(AsciiDefinition::Control::LF):
								NewLine();
								return 0;
							default:
								Push(character);
								break;
							}

							return 1;
						}

					public:
						uint32_t GetNewLineElapsedTime() const
						{
							return micros() - LastNewLineTime;
						}

						void Clear()
						{
							memset(TextBuffer, static_cast<uint8_t>(AsciiDefinition::Control::NUL), sizeof(TextBuffer));
						}

					public: // Drawable interface
						void Draw(IFrameBuffer* frame, const uint8_t line)
						{
							const pixel_t lineOffsetY = GetLineYOffset(line, LineSpacing);
							TextWriter.Write(frame,
								Layout::X(),
								Layout::Y() + Layout::Height() - lineOffsetY + OffsetY,
								reinterpret_cast<const char*>(&TextBuffer[Lines - line]));
						}

						// Text writing interface.
					private:
						static constexpr pixel_t GetLineYOffset(const uint16_t lineIndex, uint8_t lineSpacing)
						{
							return FontHeight + (lineIndex * (FontHeight + lineSpacing));
						}

						void Push(const uint8_t character)
						{
							if (CursorX < CharactersPerLine)
							{
								// Write character to current line buffer.
								TextBuffer[Lines][CursorX++] = character;
								TextBuffer[Lines][CursorX] = static_cast<uint8_t>(AsciiDefinition::Control::NUL);
							}
						}

						void NewLine()
						{
							// Shift all lines up by one. <= to clear null-terminator as well.
							for (uint8_t line = 0; line <= Lines; line++)
							{
								for (uint16_t i = 0; i <= CharactersPerLine; i++)
								{
									TextBuffer[line][i] = TextBuffer[line + 1][i];
								}
							}
							// Clear last line. <= to clear null-terminator as well.
							for (uint16_t i = 0; i <= CharactersPerLine; i++)
							{
								TextBuffer[Lines][i] = static_cast<uint8_t>(AsciiDefinition::Control::NUL);
							}

							CursorX = 0; // Reset cursor X.
							OffsetY = FontHeight; // Start offset for animation.
							LastNewLineTime = micros(); // Update new line time.
						}
					};
				}

				namespace Views
				{
					/// <summary>
					/// A view component that displays serial text output using a specified layout and font drawer.
					/// </summary>
					/// <typeparam name="Layout">The layout type that defines how the text view is positioned and sized.</typeparam>
					/// <typeparam name="FontDrawerType">The type responsible for rendering font glyphs. Defaults to Definitions::DefaultFontDrawer.</typeparam>
					/// <typeparam name="FontHeight">The height of the font in pixels. Fixed-height fonts must specify their native height here. Defaults to Definitions::DefaultFont::Height.</typeparam>
					/// <typeparam name="FontWidth">The width of the font in pixels. Fixed-width fonts must specify their native width here. Defaults to Definitions::DefaultFont::Width.</typeparam>
					/// <typeparam name="Kerning">The horizontal spacing between characters in pixels. Defaults to 1.</typeparam>
					/// <typeparam name="LineSpacing">The vertical spacing between lines in pixels. Defaults to 1.</typeparam>
					template<typename Layout
						, typename FontDrawerType = Definitions::DefaultFontDrawer
						, pixel_t FontWidth = Definitions::DefaultFont::Width
						, pixel_t FontHeight = Definitions::DefaultFont::Height
						, uint8_t Kerning = 1
						, uint8_t LineSpacing = 1>
					class SerialText : public View::AbstractView
					{
					public:
						using DrawableType = Drawables::SerialText<Layout, FontDrawerType, FontWidth, FontHeight, Kerning, LineSpacing>;

						static constexpr auto CharactersPerLine = DrawableType::CharactersPerLine;
						static constexpr auto Lines = DrawableType::Lines;

					public:
						uint32_t AnimationDuration = 125000; // Animation duration in microseconds.

					private:
						DrawableType Drawable{};

					private:
						uint8_t CallIndex = 0;

					public:
						SerialText() = default;

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
						void ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
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

				template<typename Layout
					, typename FontDrawerType = Definitions::DefaultFontDrawer
					, pixel_t FontWidth = Definitions::DefaultFont::Width
					, pixel_t FontHeight = Definitions::DefaultFont::Height
					, uint8_t Kerning = 1
					, uint8_t LineSpacing = 1>
				using Frame = View::FrameAdapter<Views::SerialText<Layout, FontDrawerType, FontWidth, FontHeight, Kerning, LineSpacing>>;
			}
		}
	}
}
#endif