#ifndef _EGFX_FRAMEWORK_ASSETS_TERMINAL_WINDOW_DRAWABLES_h
#define _EGFX_FRAMEWORK_ASSETS_TERMINAL_WINDOW_DRAWABLES_h

#include <Print.h>
#include "Constants.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace TerminalWindow
			{
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
						FontText::TemplateTextWriter<Layout, FontDrawerType> TextWriter{};

					private:
						uint32_t LastNewLineTime = 0;
						pixel_t CursorX = 0;

					public:
						pixel_t OffsetY = 0; // Animatable offset Y.

					public:
						SerialText() : Print() {}

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
			}
		}
	}
}
#endif