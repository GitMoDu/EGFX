#ifndef _DISPLAYPRINT_h
#define _DISPLAYPRINT_h

#include <Print.h>
#include <EgfxDrawer.h>

namespace Egfx
{
	namespace PrintSurface
	{
		template<typename Layout
			, typename FontDrawerType
			, uint16_t CharactersPerLine
			, uint8_t Lines
			, uint8_t FontHeight
			, uint8_t LineSpacing = 0>
		class FontPrintSurface : public IFrameDraw
		{
		private:
			struct PrintLayout
			{
				static constexpr pixel_t X() { return Layout::X(); }
				static constexpr pixel_t Y() { return Layout::Y(); }
				static constexpr pixel_t Width() { return Layout::Width(); }
				static constexpr pixel_t Height() { return Layout::Height(); }

				static constexpr pixel_t GetLineYOffset(const uint16_t lineIndex)
				{
					return FontHeight + ((lineIndex) * (FontHeight + LineSpacing));
				}
			};

		private:
			static constexpr size_t BufferSize = ((size_t)Lines) * CharactersPerLine;
			uint8_t TextBuffer[Lines][CharactersPerLine + 1]{};

		private:
			FontText::TemplateTextWriter<FontDrawerType, PrintLayout> TextWriter{};

			uint8_t CallIndex = 0;

			int8_t OffsetY = 0; // Animatable offset Y.

			pixel_t CursorX = 0;

		public:
			FontPrintSurface() : IFrameDraw()
			{
			}

			~FontPrintSurface()
			{
			}

			// Always enabled.
			bool IsEnabled() const final { return true; }
			void SetEnabled(const bool /*enabled*/) final {}

			int availableForWrite()
			{
				return CallIndex == 0;
			}

			bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
			{
				// Draw a text line from buffer each draw call.
				const pixel_t lineOffsetY = PrintLayout::GetLineYOffset(CallIndex);
				TextWriter.Write(frame,
					Layout::X(),
					Layout::Y() + Layout::Height() - lineOffsetY + OffsetY,
					reinterpret_cast<const char*>(&TextBuffer[Lines - 1 - CallIndex]));

				CallIndex++; // Advance call index and determine if cycle is complete.
				if (CallIndex >= Lines)
				{
					CallIndex = 0;
					return true;
				}

				return false;
			}

			void Clear()
			{
				memset(TextBuffer, static_cast<uint8_t>(AsciiDefinition::Control::NUL), sizeof(TextBuffer));
			}

			// Text writing interface.
		public:
			void Push(const uint8_t character)
			{
				if (CursorX < static_cast<pixel_t>(CharactersPerLine))
				{
					// Write character to current line buffer.
					TextBuffer[Lines - 1][CursorX++] = character;
					TextBuffer[Lines - 1][CursorX] = static_cast<uint8_t>(AsciiDefinition::Control::NUL);
				}
			}

			void NewLine()
			{
				// Shift all lines up by one.
				for (uint8_t line = 0; line < Lines - 1; line++)
				{
					for (uint16_t i = 0; i < CharactersPerLine; i++)
					{
						TextBuffer[line][i] = TextBuffer[line + 1][i];
					}
					TextBuffer[line][CharactersPerLine] = static_cast<uint8_t>(AsciiDefinition::Control::NUL);
				}
				// Clear last line.
				for (uint16_t i = 0; i < CharactersPerLine; i++)
				{
					TextBuffer[Lines - 1][i] = static_cast<uint8_t>(AsciiDefinition::Control::NUL);
				}
				TextBuffer[Lines - 1][CharactersPerLine] = static_cast<uint8_t>(AsciiDefinition::Control::NUL);
				CursorX = 0;
			}
		};
	}

	template<typename Layout
		, typename BitmaskFontDrawerType
		, uint8_t FontWidth
		, uint8_t FontHeight>
	class TextDisplaySerial : public Print
	{
	public:
		static constexpr uint8_t Kerning = 1;
		static constexpr uint8_t LineSpacing = 1;

		static constexpr auto CharacterCount = Layout::Width() / (FontWidth + Kerning);
		static constexpr auto LineCount = Layout::Height() / (FontHeight + LineSpacing);

	private:
		PrintSurface::FontPrintSurface<Layout, BitmaskFontDrawerType,
			CharacterCount,
			LineCount,
			FontHeight,
			LineSpacing> PrintSurface{};

	public:
		TextDisplaySerial() : Print()
		{
		}

		bool Setup(Egfx::IFrameEngine& engine)
		{
			engine.SetDrawer(&PrintSurface);
			PrintSurface.Clear();

			return true;
		}

	public:
		virtual int availableForWrite()
		{
			return PrintSurface.availableForWrite();
		}

	public:
		size_t write(uint8_t character) final
		{
			switch (static_cast<uint8_t>(character))
			{
			case uint8_t(AsciiDefinition::Control::NUL):
				return 0;
			case uint8_t(AsciiDefinition::Control::CR):
			case uint8_t(AsciiDefinition::Control::LF):
				PrintSurface.NewLine();
				return 0;
			default:
				PrintSurface.Push(character);
				break;
			}

			return 1;
		}
	};

	template<typename Layout>
	using BitmaskTextDisplaySerial = TextDisplaySerial<Layout
		, typename Egfx::BitmaskFont::TemplateFontDrawer<Egfx::BitmaskFonts::Plastic::FontType3x5>
		, Egfx::BitmaskFonts::Plastic::FontType3x5::Width
		, Egfx::BitmaskFonts::Plastic::FontType3x5::Height>;
}

#endif

