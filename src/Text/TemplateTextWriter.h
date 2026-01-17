#ifndef _EGFX_FONT_TEXT_TEMPLATE_TEXT_WRITER_h
#define _EGFX_FONT_TEXT_TEMPLATE_TEXT_WRITER_h

#include "FontText.h"

namespace Egfx
{
	namespace FontText
	{
		namespace Contract
		{
			struct FontDrawer
			{
				using dimension_t = uint8_t;

				pixel_t GetFontWidth() const { return 0; }
				pixel_t GetFontHeight() const { return 0; }
				pixel_t GetTabWidth() const { return 0; }
				pixel_t GetSpaceWidth() const { return 0; }
				pixel_t GetKerningWidth() const { return 0; }
				pixel_t Advance(const char /*printableChar*/) const { return 0; }

				pixel_t Draw(IFrameBuffer* /*framebuffer*/, const pixel_t /*x*/, const pixel_t /*y*/, const char /*printableChar*/)
				{
					return 0;
				}
			};
		}

		/// <summary>
		/// Template-based text writer that renders text strings and numbers to a framebuffer with support for kerning, special characters, and alignment.
		/// Turns a font drawer into a text writer capable of rendering strings and numbers with kerning.
		/// </summary>
		/// <typeparam name="Layout">The layout configuration that defines positioning and spacing parameters.</typeparam>
		/// <typeparam name="FontDrawerType">The underlying font drawer implementation that handles individual character rendering. Must follow the FontDrawerContract interface.</typeparam>
		template<typename Layout,
			typename FontDrawerType = Contract::FontDrawer
		>
		class TemplateTextWriter : public FontDrawerType
		{
		private:
			using Base = FontDrawerType;

			using dimension_t = typename FontDrawerType::dimension_t;

		public:
			TemplateTextWriter() : Base() {}

		public: // Measurement API
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
						const pixel_t rel = x + cursor - Layout::X();
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

			pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const int8_t number, const TextAlignmentEnum alignment)
			{
				return WriteSigned<int8_t>(framebuffer, x, y, number, alignment);
			}

			pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const int16_t number, const TextAlignmentEnum alignment)
			{
				return WriteSigned<int16_t>(framebuffer, x, y, number, alignment);
			}

			pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const int32_t number, const TextAlignmentEnum alignment)
			{
				return WriteSigned<int32_t>(framebuffer, x, y, number, alignment);
			}

			pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const uint8_t number, const TextAlignmentEnum alignment)
			{
				return WriteUnsigned<uint8_t>(framebuffer, x, y, number, alignment);
			}

			pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const uint16_t number, const TextAlignmentEnum alignment)
			{
				return WriteUnsigned<uint16_t>(framebuffer, x, y, number, alignment);
			}

			pixel_t Write(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const uint32_t number, const TextAlignmentEnum alignment)
			{
				return WriteUnsigned<uint32_t>(framebuffer, x, y, number, alignment);
			}

		private:
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
#endif