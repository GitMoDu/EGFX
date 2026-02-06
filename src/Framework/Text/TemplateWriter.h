#ifndef _EGFX_FRAMEWORK_TEXT_TEMPLATE_WRITER_h
#define _EGFX_FRAMEWORK_TEXT_TEMPLATE_WRITER_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Text
		{
			namespace Contract
			{
				/// <summary>
				/// Font drawer contract used by TemplateWriter.
				/// Provides font metrics and per-glyph draw/advance methods.
				/// </summary>
				struct FontDrawer
				{
					/// <summary>The shader's intrinsic dimension type.</summary>
					using dimension_t = uint8_t;

					/// <summary>Gets the nominal glyph width (pixels).</summary>
					pixel_t GetFontWidth() const { return 0; }

					/// <summary>Gets the nominal glyph height (pixels).</summary>
					pixel_t GetFontHeight() const { return 0; }

					/// <summary>Gets the tab stop width (pixels).</summary>
					pixel_t GetTabWidth() const { return 0; }

					/// <summary>Gets the space width (pixels).</summary>
					pixel_t GetSpaceWidth() const { return 0; }

					/// <summary>Gets the kerning width (pixels) inserted between glyphs.</summary>
					pixel_t GetKerningWidth() const { return 0; }

					/// <summary>Gets the horizontal advance for a glyph (pixels).</summary>
					pixel_t Advance(const char /*printableChar*/) const { return 0; }

					/// <summary>
					/// Draws a glyph at the given pixel-space position.
					/// </summary>
					/// <returns>Glyph advance (pixels).</returns>
					pixel_t Draw(IFrameBuffer* /*framebuffer*/, const pixel_t /*x*/, const pixel_t /*y*/, const char /*printableChar*/)
					{
						return 0;
					}
				};
			}

			/// <summary>
			/// Template-based text writer.
			///
			/// Provides measurement and drawing routines for:
			/// - NUL-terminated RAM strings
			/// - AVR flash strings (__FlashStringHelper)
			/// - unsigned and signed integers
			///
			/// Formatting rules:
			/// - No leading kerning is applied before the first unit.
			/// - Kerning is applied before each subsequent unit and removed at end-of-line.
			/// - Special characters:
			///   - LF, CR, NUL: terminate the operation (end-of-line)
			///   - HT: tab-align cursor to the next tab stop using GetTabWidth()
			///   - Space: advances cursor using GetSpaceWidth()
			/// - Printable glyphs use the underlying font drawer (Advance/Draw).
			/// - When out of bounds, glyphs are not drawn but the cursor still advances.
			/// </summary>
			/// <typeparam name="Layout">Layout type that provides X(), Y(), Width(), and Height().</typeparam>
			/// <typeparam name="FontDrawerType">Font drawer implementation providing font metrics and Draw/Advance.</typeparam>
			template<typename Layout,
				typename FontDrawerType = Contract::FontDrawer
			>
			class TemplateWriter : public FontDrawerType
			{
			private:
				using Base = FontDrawerType;

				using dimension_t = typename FontDrawerType::dimension_t;

			public:
				TemplateWriter() : Base() {}

				~TemplateWriter() = default;

			public: // Measurement API

				/// <summary>
				/// Returns the width contribution for a single character.
				/// Special characters are mapped to their configured widths and line terminators return 0.
				/// </summary>
				dimension_t GetCharacterWidth(const char character) const
				{
					switch (static_cast<uint8_t>(character))
					{
					case uint8_t(AsciiDefinition::Control::LF):
					case uint8_t(AsciiDefinition::Control::CR):
					case uint8_t(AsciiDefinition::Control::NUL):
						return 0;
					case uint8_t(AsciiDefinition::Control::HT):
						return Base::GetTabWidth();
					case uint8_t(AsciiDefinition::Printable::Space):
						return Base::GetSpaceWidth();
					default:
						return Base::Advance(character);
					}

					return 0;
				}

				/// <summary>
				/// Measures the width of a NUL-terminated RAM string.
				/// Stops on LF, CR, or NUL.
				/// Uses the same special-character handling as Write().
				/// </summary>
				pixel_t GetTextWidth(const char* text) const
				{
					if (text != NULL)
					{
						uint8_t* ch = (uint8_t*)text;
						pixel_t width = 0;

						// First unit: no leading kerning.
						char character = *ch++;
						switch (static_cast<uint8_t>(character))
						{
						case uint8_t(AsciiDefinition::Control::LF):
						case uint8_t(AsciiDefinition::Control::CR):
						case uint8_t(AsciiDefinition::Control::NUL):
							return width;
						case uint8_t(AsciiDefinition::Control::HT):
							width += Base::GetTabWidth();
							break;
						case uint8_t(AsciiDefinition::Printable::Space):
							width += Base::GetSpaceWidth();
							break;
						default:
							width += Base::Advance(character);
							break;
						}
						// Remaining units: add kerning before each.
						while (true)
						{
							width += Base::GetKerningWidth();
							character = *ch++;
							switch (static_cast<uint8_t>(character))
							{
							case uint8_t(AsciiDefinition::Control::LF):
							case uint8_t(AsciiDefinition::Control::CR):
							case uint8_t(AsciiDefinition::Control::NUL):
								width -= Base::GetKerningWidth(); // Remove kerning before line end return.
								return width;
							case uint8_t(AsciiDefinition::Control::HT):
								width += Base::GetTabWidth();
								break;
							case uint8_t(AsciiDefinition::Printable::Space):
								width += Base::GetSpaceWidth();
								break;
							default:
								width += Base::Advance(character);
								break;
							}
						}
						return width;
					}
					else
					{
						return 0;
					}
				}

				/// <summary>
				/// Measures the width of an AVR flash string (__FlashStringHelper).
				/// On non-AVR targets, this forwards to the RAM string overload.
				/// Stops on LF, CR, or NUL.
				/// </summary>
				pixel_t GetTextWidth(const __FlashStringHelper* ifsh) const
				{
#if defined(ARDUINO_ARCH_AVR)
					if (ifsh != NULL)
					{
						char* ptr = (char*)reinterpret_cast<const char*>(ifsh);
						pixel_t width = 0;

						// First unit: no leading kerning.
						char character = pgm_read_byte(ptr++);
						switch (static_cast<uint8_t>(character))
						{
						case uint8_t(AsciiDefinition::Control::LF):
						case uint8_t(AsciiDefinition::Control::CR):
						case uint8_t(AsciiDefinition::Control::NUL):
							return width;
						case uint8_t(AsciiDefinition::Control::HT):
							width += Base::GetTabWidth();
							break;
						case uint8_t(AsciiDefinition::Printable::Space):
							width += Base::GetSpaceWidth();
							break;
						default:
							width += Base::Advance(character);
							break;
						}
						// Remaining units: add kerning before each.
						while (true)
						{
							width += Base::GetKerningWidth();
							character = pgm_read_byte(ptr++);
							switch (static_cast<uint8_t>(character))
							{
							case uint8_t(AsciiDefinition::Control::LF):
							case uint8_t(AsciiDefinition::Control::CR):
							case uint8_t(AsciiDefinition::Control::NUL):
								width -= Base::GetKerningWidth(); // Remove kerning before line end return.
								return width;
							case uint8_t(AsciiDefinition::Control::HT):
								width += Base::GetTabWidth();
								break;
							case uint8_t(AsciiDefinition::Printable::Space):
								width += Base::GetSpaceWidth();
								break;
							default:
								width += Base::Advance(character);
								break;
							}
						}
					}
					else
					{
						return 0;
					}
#else
					return GetTextWidth(reinterpret_cast<const char*>(ifsh));
#endif
				}

				/// <summary>
				/// Measures the width of an unsigned integer rendered as ASCII decimal.
				/// Kerning is inserted between digits (no trailing kerning).
				/// </summary>
				pixel_t GetNumberWidth(const uint32_t number) const
				{
					pixel_t width = 0;
					bool started = false;

					for (int_fast8_t i = 9; i >= 0; --i)
					{
						const uint32_t power = FontText::PowerOfTen[i];
						const uint8_t digit = static_cast<uint8_t>((number / power) % 10);

						if (digit != 0 || started || i == 0)
						{
							width += Base::Advance(static_cast<const char>(AsciiDefinition::NumbersStart + digit));
							started = true;

							if (i != 0)
							{
								width += Base::GetKerningWidth();
							}
						}
					}

					return width;
				}

				/// <summary>
				/// Measures the width of an unsigned 16-bit integer rendered as ASCII decimal.
				/// Kerning is inserted between digits (no trailing kerning).
				/// </summary>
				pixel_t GetNumberWidth(const uint16_t number) const
				{
					pixel_t width = 0;
					bool started = false;

					for (int_fast8_t i = 4; i >= 0; --i)
					{
						const uint16_t power = static_cast<uint16_t>(FontText::PowerOfTen[i]);
						const uint8_t digit = static_cast<uint8_t>((number / power) % 10);
						if (digit != 0 || started || i == 0)
						{
							width += Base::Advance(static_cast<const char>(AsciiDefinition::NumbersStart + digit));
							started = true;
							if (i != 0)
							{
								width += Base::GetKerningWidth();
							}
						}
					}

					return width;
				}

				/// <summary>
				/// Measures the width of an unsigned 8-bit integer rendered as ASCII decimal.
				/// Kerning is inserted between digits (no trailing kerning).
				/// </summary>
				pixel_t GetNumberWidth(const uint8_t number) const
				{
					pixel_t width = 0;
					bool started = false;

					for (int_fast8_t i = 2; i >= 0; --i)
					{
						const uint8_t power = static_cast<uint8_t>(FontText::PowerOfTen[i]);
						const uint8_t digit = static_cast<uint8_t>((number / power) % 10);
						if (digit != 0 || started || i == 0)
						{
							width += Base::Advance(static_cast<const char>(AsciiDefinition::NumbersStart + digit));
							started = true;
							if (i != 0)
							{
								width += Base::GetKerningWidth();
							}
						}
					}

					return width;
				}

			public: // Text drawing API
				/// <summary>
				/// Draws a single printable ASCII character at (x, y) and returns the next cursor X (pixels).
				/// If the glyph is outside the Layout bounds, nothing is drawn but the cursor still advances.
				/// Non-printable characters are ignored and return 0.
				/// </summary>
				/// <param name="framebuffer">Target framebuffer to draw into.</param>
				/// <param name="x">Pixel-space X position.</param>
				/// <param name="y">Pixel-space Y position.</param>
				/// <param name="character">Printable ASCII character to draw.</param>
				/// <returns>Next cursor X (pixels).</returns>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char character)
				{
					if (character >= AsciiDefinition::PrintableStart
						&& character <= AsciiDefinition::PrintableEnd)
					{
						if (x >= Layout::X() + Layout::Width()
							|| x + Base::GetFontWidth() < Layout::X()
							|| y >= Layout::Y() + Layout::Height()
							|| y + Base::GetFontHeight() < Layout::Y())
						{
							return x + Base::Advance(character); // Out of bounds.
						}
						else
						{
							return x + Base::Draw(framebuffer, x, y, character);
						}
					}
					else
					{
						// Non-printable character, no drawing.
						return 0;
					}
				}

				/// <summary>
				/// Draws a single character using the requested alignment about the provided X coordinate.
				/// Alignment uses GetCharacterWidth() (tabs/spaces/control characters are measured, then aligned).
				/// </summary>
				/// <param name="framebuffer">Target framebuffer to draw into.</param>
				/// <param name="x">Alignment anchor X (pixels).</param>
				/// <param name="y">Pixel-space Y position.</param>
				/// <param name="character">Character to draw.</param>
				/// <param name="alignment">Alignment mode.</param>
				/// <returns>Next cursor X (pixels).</returns>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char character, const TextAlignmentEnum alignment)
				{
					switch (alignment)
					{
					case TextAlignmentEnum::Left:
						return Write(framebuffer, x, y, character);
					case TextAlignmentEnum::Center:
					{
						const pixel_t charWidth = GetCharacterWidth(character);
						const pixel_t startX = x - SignedRightShift(charWidth, 1);
						return Write(framebuffer, startX, y, character);
					}
					break;
					case TextAlignmentEnum::Right:
					{
						const pixel_t charWidth = GetCharacterWidth(character);
						const pixel_t startX = x - charWidth;
						return Write(framebuffer, startX, y, character);
					}
					break;
					default:
						break;
					}
					return 0;
				}

				/// <summary>
				/// Writes an AVR flash string (__FlashStringHelper) and returns the final cursor X (pixels).
				/// Stops on LF, CR, or NUL.
				/// Special characters:
				/// - HT: advances to the next tab stop relative to Layout::X()
				/// - Space: advances by GetSpaceWidth()
				/// Kerning:
				/// - No leading kerning before the first unit
				/// - Kerning is added before each subsequent unit and removed before returning at end-of-line
				/// </summary>
				/// <param name="framebuffer">Target framebuffer to draw into.</param>
				/// <param name="x">Pixel-space start X.</param>
				/// <param name="y">Pixel-space start Y.</param>
				/// <param name="ifsh">Flash string pointer.</param>
				/// <returns>Final cursor X (pixels).</returns>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const __FlashStringHelper* ifsh)
				{
#if defined(ARDUINO_ARCH_AVR)
					if (ifsh != NULL)
					{
						char* ptr = (char*)reinterpret_cast<const char*>(ifsh);
						pixel_t cursor = x;

						// First unit: no leading kerning.
						char character = pgm_read_byte(ptr++);
						switch (static_cast<uint8_t>(character))
						{
						case uint8_t(AsciiDefinition::Control::LF):
						case uint8_t(AsciiDefinition::Control::CR):
						case uint8_t(AsciiDefinition::Control::NUL):
							return cursor;
						case uint8_t(AsciiDefinition::Control::HT):
						{
							const pixel_t rel = cursor - Layout::X();
							const pixel_t rem = rel % Base::GetTabWidth();
							cursor += (rem == 0) ? Base::GetTabWidth() : (Base::GetTabWidth() - rem);
						}
						break;
						case uint8_t(AsciiDefinition::Printable::Space):
							cursor += Base::GetSpaceWidth();
							break;
						default:
							if (cursor >= Layout::X() + Layout::Width()
								|| cursor + Base::GetFontWidth() < Layout::X()
								|| y >= Layout::Y() + Layout::Height()
								|| y + Base::GetFontHeight() < Layout::Y())
							{
								cursor += Base::Advance(character); // Out of bounds.
							}
							else
							{
								cursor += Base::Draw(framebuffer, cursor, y, character);
							}
							break;
						}

						// Remaining units: add kerning before each.
						while (true)
						{
							cursor += Base::GetKerningWidth();
							character = pgm_read_byte(ptr++);
							switch (static_cast<uint8_t>(character))
							{
							case uint8_t(AsciiDefinition::Control::LF):
							case uint8_t(AsciiDefinition::Control::CR):
							case uint8_t(AsciiDefinition::Control::NUL):
								cursor -= Base::GetKerningWidth(); // Remove kerning before line end return.
								return cursor;
							case uint8_t(AsciiDefinition::Control::HT):
							{
								const pixel_t rel = cursor - Layout::X();
								const pixel_t rem = rel % Base::GetTabWidth();
								cursor += (rem == 0) ? Base::GetTabWidth() : (Base::GetTabWidth() - rem);
							}
							break;
							case uint8_t(AsciiDefinition::Printable::Space):
								cursor += Base::GetSpaceWidth();
								break;
							default:
								if (cursor >= Layout::X() + Layout::Width()
									|| cursor + Base::GetFontWidth() < Layout::X()
									|| y >= Layout::Y() + Layout::Height()
									|| y + Base::GetFontHeight() < Layout::Y())
								{
									cursor += Base::Advance(character); // Out of bounds.
								}
								else
								{
									cursor += Base::Draw(framebuffer, cursor, y, character);
								}
								break;
							}
						}
						return cursor;
					}
					else
					{
						return x;
					}
#else
					return Write(framebuffer, x, y, reinterpret_cast<const char*>(ifsh));
#endif
				}

				/// <summary>
				/// Writes an AVR flash string with alignment about the provided X coordinate.
				/// </summary>
				/// <param name="framebuffer">Target framebuffer to draw into.</param>
				/// <param name="x">Alignment anchor X (pixels).</param>
				/// <param name="y">Pixel-space start Y.</param>
				/// <param name="ifsh">Flash string pointer.</param>
				/// <param name="alignment">Alignment mode.</param>
				/// <returns>Final cursor X (pixels).</returns>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const __FlashStringHelper* ifsh, const TextAlignmentEnum alignment)
				{
					switch (alignment)
					{
					case TextAlignmentEnum::Left:
						return Write(framebuffer, x, y, ifsh);
					case TextAlignmentEnum::Center:
					{
						const pixel_t textWidth = GetTextWidth(ifsh);
						const pixel_t startX = x - SignedRightShift(textWidth, 1);
						return Write(framebuffer, startX, y, ifsh);
					}
					break;
					case TextAlignmentEnum::Right:
					{
						const pixel_t textWidth = GetTextWidth(ifsh);
						const pixel_t startX = x - textWidth;
						return Write(framebuffer, startX, y, ifsh);
					}
					break;
					default:
						break;
					}
					return 0;
				}

				/// <summary>
				/// Writes a NUL-terminated RAM string and returns the final cursor X (pixels).
				/// Stops on LF, CR, or NUL.
				/// Special characters:
				/// - HT: advances to the next tab stop relative to Layout::X()
				/// - Space: advances by GetSpaceWidth()
				/// Kerning:
				/// - No leading kerning before the first unit
				/// - Kerning is added before each subsequent unit and removed before returning at end-of-line
				/// </summary>
				/// <param name="framebuffer">Target framebuffer to draw into.</param>
				/// <param name="x">Pixel-space start X.</param>
				/// <param name="y">Pixel-space start Y.</param>
				/// <param name="text">NUL-terminated string pointer.</param>
				/// <returns>Final cursor X (pixels).</returns>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char* text)
				{
					if (text != NULL)
					{
						uint8_t* ch = (uint8_t*)text;
						pixel_t cursor = x;

						// First unit: no leading kerning.
						char character = *ch++;
						switch (static_cast<uint8_t>(character))
						{
						case uint8_t(AsciiDefinition::Control::LF):
						case uint8_t(AsciiDefinition::Control::CR):
						case uint8_t(AsciiDefinition::Control::NUL):
							return cursor;
						case uint8_t(AsciiDefinition::Control::HT):
						{
							const pixel_t rel = cursor - Layout::X();
							const pixel_t rem = rel % Base::GetTabWidth();
							cursor += (rem == 0) ? Base::GetTabWidth() : (Base::GetTabWidth() - rem);
						}
						break;
						case uint8_t(AsciiDefinition::Printable::Space):
							cursor += Base::GetSpaceWidth();
							break;
						default:
							if (cursor >= Layout::X() + Layout::Width()
								|| cursor + Base::GetFontWidth() < Layout::X()
								|| y >= Layout::Y() + Layout::Height()
								|| y + Base::GetFontHeight() < Layout::Y())
							{
								cursor += Base::Advance(character); // Out of bounds.
							}
							else
							{
								cursor += Base::Draw(framebuffer, cursor, y, character);
							}
							break;
						}

						// Remaining units: add kerning before each.
						while (true)
						{
							cursor += Base::GetKerningWidth();
							character = *ch++;
							switch (static_cast<uint8_t>(character))
							{
							case uint8_t(AsciiDefinition::Control::LF):
							case uint8_t(AsciiDefinition::Control::CR):
							case uint8_t(AsciiDefinition::Control::NUL):
								cursor -= Base::GetKerningWidth(); // Remove kerning before line end return.
								return cursor;
							case uint8_t(AsciiDefinition::Control::HT):
							{
								const pixel_t rel = cursor - Layout::X();
								const pixel_t rem = rel % Base::GetTabWidth();
								cursor += (rem == 0) ? Base::GetTabWidth() : (Base::GetTabWidth() - rem);
							}
							break;
							case uint8_t(AsciiDefinition::Printable::Space):
								cursor += Base::GetSpaceWidth();
								break;
							default:
								if (character >= AsciiDefinition::PrintableStart
									&& character <= AsciiDefinition::PrintableEnd)
								{
									if (cursor >= Layout::X() + Layout::Width()
										|| cursor + Base::GetFontWidth() < Layout::X()
										|| y >= Layout::Y() + Layout::Height()
										|| y + Base::GetFontHeight() < Layout::Y())
									{
										cursor += Base::Advance(character); // Out of bounds.
									}
									else
									{
										cursor += Base::Draw(framebuffer, cursor, y, character);
									}
								}
								break;
							}
						}

						return cursor;
					}
					else
					{
						return x;
					}
				}

				/// <summary>
				/// Writes a RAM string with alignment about the provided X coordinate.
				/// Text width is computed using GetTextWidth().
				/// </summary>
				/// <param name="framebuffer">Target framebuffer to draw into.</param>
				/// <param name="x">Alignment anchor X (pixels).</param>
				/// <param name="y">Pixel-space start Y.</param>
				/// <param name="text">NUL-terminated string pointer.</param>
				/// <param name="alignment">Alignment mode.</param>
				/// <returns>Final cursor X (pixels).</returns>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char* text, const TextAlignmentEnum alignment)
				{
					switch (alignment)
					{
					case TextAlignmentEnum::Left:
						return Write(framebuffer, x, y, text);
					case TextAlignmentEnum::Center:
					{
						const pixel_t textWidth = GetTextWidth(text);
						const pixel_t startX = x - SignedRightShift(textWidth, 1);
						return Write(framebuffer, startX, y, text);
					}
					break;
					case TextAlignmentEnum::Right:
					{
						const pixel_t textWidth = GetTextWidth(text);
						const pixel_t startX = x - textWidth;

						return Write(framebuffer, startX, y, text);
					}
					break;
					default:
						break;
					}

					return 0;
				}

			public:// Number drawing API
				/// <summary>
				/// Writes a signed 8-bit integer as ASCII decimal and returns the final cursor X (pixels).
				/// Negative values emit a leading '-' followed by kerning and digits.
				/// </summary>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const int8_t number)
				{
					if (number >= 0)
					{
						return Write(framebuffer, x, y, static_cast<const uint8_t>(number));
					}
					else
					{
						pixel_t cursor = x + Base::Draw(framebuffer, x, y,
							static_cast<const char>(AsciiDefinition::Printable::Minus))
							+ Base::GetKerningWidth();

						return Write(framebuffer, cursor, y, static_cast<const uint8_t>(-number));
					}
				}

				/// <summary>
				/// Writes a signed 16-bit integer as ASCII decimal and returns the final cursor X (pixels).
				/// Negative values emit a leading '-' followed by kerning and digits.
				/// </summary>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const int16_t number)
				{
					if (number >= 0)
					{
						return Write(framebuffer, x, y, static_cast<const uint16_t>(number));
					}
					else
					{
						const pixel_t cursor = x + Base::Draw(framebuffer, x, y,
							static_cast<const char>(AsciiDefinition::Printable::Minus))
							+ Base::GetKerningWidth();
						return Write(framebuffer, cursor, y, static_cast<const uint16_t>(-number));
					}
				}

				/// <summary>
				/// Writes a signed 32-bit integer as ASCII decimal and returns the final cursor X (pixels).
				/// Negative values emit a leading '-' followed by kerning and digits.
				/// </summary>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const int32_t number)
				{
					if (number >= 0)
					{
						return Write(framebuffer, x, y, static_cast<const uint32_t>(number));
					}
					else
					{
						const pixel_t cursor = x + Base::Draw(framebuffer, x, y,
							static_cast<const char>(AsciiDefinition::Printable::Minus))
							+ Base::GetKerningWidth();
						return Write(framebuffer, cursor, y, static_cast<const uint32_t>(-number));
					}
				}

				/// <summary>
				/// Writes an unsigned 8-bit integer as ASCII decimal and returns the final cursor X (pixels).
				/// Leading zeros are suppressed. Kerning is inserted between digits (no trailing kerning).
				/// </summary>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const uint8_t number)
				{
					pixel_t cursor = x;
					bool started = false;

					for (int_fast8_t i = 2; i >= 0; --i)
					{
						const uint8_t power = static_cast<uint8_t>(FontText::PowerOfTen[i]);
						const uint8_t digit = static_cast<uint8_t>((number / power) % 10);

						if (digit != 0 || started || i == 0)
						{
							cursor += Base::Draw(framebuffer, cursor, y, static_cast<const char>(AsciiDefinition::NumbersStart + digit));
							started = true;

							if (i != 0)
							{
								cursor += Base::GetKerningWidth();
							}
						}
					}

					return cursor;
				}

				/// <summary>
				/// Writes an unsigned 16-bit integer as ASCII decimal and returns the final cursor X (pixels).
				/// Leading zeros are suppressed. Kerning is inserted between digits (no trailing kerning).
				/// </summary>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const uint16_t number)
				{
					pixel_t cursor = x;
					bool started = false;

					for (int_fast8_t i = 4; i >= 0; --i)
					{
						const uint16_t power = static_cast<uint16_t>(FontText::PowerOfTen[i]);
						const uint8_t digit = static_cast<uint8_t>((number / power) % 10);

						if (digit != 0 || started || i == 0)
						{
							cursor += Base::Draw(framebuffer, cursor, y, static_cast<const char>(AsciiDefinition::NumbersStart + digit));
							started = true;

							if (i != 0)
							{
								cursor += Base::GetKerningWidth();
							}
						}
					}

					return cursor;
				}

				/// <summary>
				/// Writes an unsigned 32-bit integer as ASCII decimal and returns the final cursor X (pixels).
				/// Leading zeros are suppressed. Kerning is inserted between digits (no trailing kerning).
				/// </summary>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const uint32_t number)
				{
					pixel_t cursor = x;
					bool started = false;

					for (int_fast8_t i = 9; i >= 0; --i)
					{
						const uint32_t power = FontText::PowerOfTen[i];
						const uint8_t digit = static_cast<uint8_t>((number / power) % 10);

						if (digit != 0 || started || i == 0)
						{
							cursor += Base::Draw(framebuffer, cursor, y, static_cast<const char>(AsciiDefinition::NumbersStart + digit));
							started = true;

							if (i != 0)
							{
								cursor += Base::GetKerningWidth();
							}
						}
					}

					return cursor;
				}

				/// <summary>
				/// Writes a signed 8-bit integer using alignment about the provided X coordinate.
				/// </summary>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const int8_t number, const TextAlignmentEnum alignment)
				{
					return WriteSigned<int8_t>(framebuffer, x, y, number, alignment);
				}

				/// <summary>
				/// Writes a signed 16-bit integer using alignment about the provided X coordinate.
				/// </summary>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const int16_t number, const TextAlignmentEnum alignment)
				{
					return WriteSigned<int16_t>(framebuffer, x, y, number, alignment);
				}

				/// <summary>
				/// Writes a signed 32-bit integer using alignment about the provided X coordinate.
				/// </summary>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const int32_t number, const TextAlignmentEnum alignment)
				{
					return WriteSigned<int32_t>(framebuffer, x, y, number, alignment);
				}

				/// <summary>
				/// Writes an unsigned 8-bit integer using alignment about the provided X coordinate.
				/// </summary>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const uint8_t number, const TextAlignmentEnum alignment)
				{
					return WriteUnsigned<uint8_t>(framebuffer, x, y, number, alignment);
				}

				/// <summary>
				/// Writes an unsigned 16-bit integer using alignment about the provided X coordinate.
				/// </summary>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const uint16_t number, const TextAlignmentEnum alignment)
				{
					return WriteUnsigned<uint16_t>(framebuffer, x, y, number, alignment);
				}

				/// <summary>
				/// Writes an unsigned 32-bit integer using alignment about the provided X coordinate.
				/// </summary>
				pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const uint32_t number, const TextAlignmentEnum alignment)
				{
					return WriteUnsigned<uint32_t>(framebuffer, x, y, number, alignment);
				}

			private:
				/// <summary>
				/// Alignment helper for signed numbers. Negative values are handled by emitting '-' and digits.
				/// </summary>
				template<typename signed_t>
				pixel_t WriteSigned(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const signed_t number, const TextAlignmentEnum alignment)
				{
					using unsigned_t = typename TypeTraits::TypeSign::make_unsigned< signed_t>::type;

					if (number >= 0)
					{
						return Write(framebuffer, x, y, static_cast<const unsigned_t>(number), alignment);
					}
					else
					{
						switch (alignment)
						{
						case TextAlignmentEnum::Left:
							return Write(framebuffer, x, y, number);
						case TextAlignmentEnum::Center:
						{
							const pixel_t textWidth = GetSignedNumberWidth(number);
							return Write(framebuffer, x - (textWidth / 2), y, static_cast<const unsigned_t>(-number));
						}
						break;
						case TextAlignmentEnum::Right:
						{
							const pixel_t textWidth = GetSignedNumberWidth(number);
							return Write(framebuffer, x - textWidth, y, static_cast<const unsigned_t>(-number));
						}
						break;
						default:
							break;
						}
					}
					return 0;
				}

				/// <summary>
				/// Alignment helper for unsigned numbers (center/right alignment uses GetNumberWidth()).
				/// </summary>
				template<typename unsigned_t>
				pixel_t WriteUnsigned(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const unsigned_t number, const TextAlignmentEnum alignment)
				{
					switch (alignment)
					{
					case TextAlignmentEnum::Left:
						return Write(framebuffer, x, y, number);
					case TextAlignmentEnum::Center:
					{
						const pixel_t startX = x - SignedRightShift(GetNumberWidth(number), 1);
						return Write(framebuffer, startX, y, number);
					}
					break;
					case TextAlignmentEnum::Right:
					{
						const pixel_t startX = x - GetNumberWidth(number);
						return Write(framebuffer, startX, y, number);
					}
					default:
						break;
					}
					return 0;
				}

				/// <summary>
				/// Measures the width of a signed number including an optional leading '-' and kerning.
				/// </summary>
				template<typename signed_t>
				pixel_t GetSignedNumberWidth(const signed_t number)
				{
					using unsigned_t = typename TypeTraits::TypeSign::make_unsigned< signed_t>::type;

					if (number < 0)
					{
						return Base::Advance(static_cast<const char>(AsciiDefinition::Printable::Minus))
							+ Base::GetKerningWidth()
							+ GetNumberWidth(static_cast<const unsigned_t>(-number));
					}
					else
					{
						return GetNumberWidth(static_cast<const unsigned_t>(number));
					}
				}
			};
		}
	}
}
#endif