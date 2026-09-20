#ifndef _INTEGERGLASS_FRAMEWORK_TEXT_WALKER_h
#define _INTEGERGLASS_FRAMEWORK_TEXT_WALKER_h

#include "Model.h"

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Text
		{
			namespace Reader
			{
				struct Ram
				{
					static char Read(const char* text, const size_t index)
					{
						return text[index];
					}
				};

				struct Flash
				{
					static char Read(const char* text, const size_t index)
					{
#if defined(ARDUINO_ARCH_AVR)
						return static_cast<char>(pgm_read_byte(text + index));
#else
						return text[index];
#endif
					}
				};
			}

			struct TextWalkerMetrics
			{
				uint16_t FontWidth;
				uint16_t FontHeight;
				uint16_t TabWidth;
				uint16_t SpaceWidth;
				uint16_t KerningWidth;
				uint16_t LineSpacing;

				uint16_t GetLineHeight() const { return FontHeight + LineSpacing; }
			};

			struct TemplateTextNumberWalker
			{
				using signed_t = int16_t;

				template<typename number_t, typename NumberCharacterCallback>
				static void Walk(const number_t number, NumberCharacterCallback onNumberCharacter)
				{
					using number_unsigned_t = typename IntegerSignal::TypeTraits::TypeSign::make_unsigned<number_t>::type;
					number_unsigned_t magnitude = static_cast<number_unsigned_t>(number);
					number_unsigned_t divisor = 1;

					while (magnitude / divisor >= 10)
					{
						divisor *= 10;
					}

					while (true)
					{
						const uint8_t digit = static_cast<uint8_t>((magnitude / divisor) % 10);
						onNumberCharacter(static_cast<char>(AsciiDefinition::NumbersStart + digit));
						if (divisor == 1)
						{
							break;
						}
						divisor /= 10;
					}
				}
			};

			class TemplateTextWalker
			{
			public:
				using unsigned_t = uint16_t;
				using signed_t = int16_t;
				using cursor_t = size_t;

				struct Measurement
				{
					unsigned_t Width;
					unsigned_t LineCount;
				};

			public:
				template<typename CharacterCallback, typename ReaderType = Reader::Ram>
				static Measurement MeasureText(const char* text,
					const TextWalkerMetrics& metrics,
					const signed_t originX,
					const signed_t originY,
					CharacterCallback onCharacter)
				{
					if (text == nullptr)
					{
						return Measurement{ 0, 0 };
					}

					signed_t cursorX = originX;
					signed_t cursorY = originY;
					signed_t lineWidth = 0;
					signed_t maxLineWidth = 0;
					unsigned_t lines = 1;
					bool first = true;

					for (cursor_t index = 0; ; ++index)
					{
						const char character = ReaderType::Read(text, index);
						if (character == static_cast<char>(AsciiDefinition::Control::NUL))
						{
							break;
						}

						if (character == static_cast<char>(AsciiDefinition::Control::LF)
							|| character == static_cast<char>(AsciiDefinition::Control::CR))
						{
							if (lineWidth > maxLineWidth)
							{
								maxLineWidth = lineWidth;
							}

							if (character == static_cast<char>(AsciiDefinition::Control::CR)
								&& ReaderType::Read(text, index + 1) == static_cast<char>(AsciiDefinition::Control::LF))
							{
								++index;
							}

							cursorX = originX;
							cursorY += static_cast<signed_t>(metrics.GetLineHeight());
							lineWidth = 0;
							++lines;
							first = true;
							continue;
						}

						cursorX += static_cast<signed_t>(first ? 0 : metrics.KerningWidth);
						if (character == static_cast<char>(AsciiDefinition::Control::HT))
						{
							if (metrics.TabWidth > 0)
							{
								const signed_t remainder = cursorX % static_cast<signed_t>(metrics.TabWidth);
								cursorX += (remainder == 0) ? static_cast<signed_t>(metrics.TabWidth) : static_cast<signed_t>(metrics.TabWidth) - remainder;
							}
						}
						else if (character == static_cast<char>(AsciiDefinition::Printable::Space))
						{
							cursorX += static_cast<signed_t>(metrics.SpaceWidth);
						}
						else if (static_cast<uint8_t>(character) >= AsciiDefinition::PrintableStart
							&& static_cast<uint8_t>(character) < AsciiDefinition::PrintableEnd)
						{
							cursorX += static_cast<signed_t>(onCharacter(character,
								cursorX, cursorY, first));
						}

						lineWidth = cursorX - originX;
						first = false;
					}

					if (lineWidth > maxLineWidth)
					{
						maxLineWidth = lineWidth;
					}

					return Measurement{ static_cast<unsigned_t>(maxLineWidth), lines };
				}

				template<typename CharacterCallback>
				static Measurement MeasureText(const char* text, const TextWalkerMetrics& metrics,
					const signed_t originX, const signed_t originY, CharacterCallback onCharacter,
					const Framework::DataSourceTypeEnum dataSourceType)
				{
					return dataSourceType == Framework::DataSourceTypeEnum::Flash
						? MeasureText<CharacterCallback, Reader::Flash>(text, metrics, originX, originY, onCharacter)
						: MeasureText<CharacterCallback, Reader::Ram>(text, metrics, originX, originY, onCharacter);
				}

				template<typename CharacterCallback, typename ReaderType = Reader::Ram>
				static unsigned_t WalkText(const char* text, const TextWalkerMetrics& metrics,
					const signed_t originX, const signed_t originY, CharacterCallback onCharacter)
				{
					return MeasureText<CharacterCallback, ReaderType>(text, metrics, originX, originY, onCharacter).Width;
				}

				template<typename CharacterCallback>
				static unsigned_t WalkText(const char* text, const TextWalkerMetrics& metrics,
					const signed_t originX, const signed_t originY, CharacterCallback onCharacter,
					const Framework::DataSourceTypeEnum dataSourceType)
				{
					return dataSourceType == Framework::DataSourceTypeEnum::Flash
						? WalkText<CharacterCallback, Reader::Flash>(text, metrics, originX, originY, onCharacter)
						: WalkText<CharacterCallback, Reader::Ram>(text, metrics, originX, originY, onCharacter);
				}
			};

			class TemplateTextMonospaceWalker
			{
			public:
				using signed_t = int16_t;
				using unsigned_t = uint16_t;
				using cursor_t = size_t;

				struct Measurement
				{
					unsigned_t Width;
					unsigned_t LineCount;
				};

			public:
				template<typename CharacterCallback, typename ReaderType = Reader::Ram>
				static Measurement MeasureText(const char* text, const TextWalkerMetrics& metrics,
					const signed_t originX, const signed_t originY, CharacterCallback onCharacter)
				{
					if (text == nullptr)
					{
						return Measurement{ 0, 0 };
					}

					signed_t cursorX = originX;
					signed_t cursorY = originY;
					signed_t lineWidth = 0;
					signed_t maxLineWidth = 0;
					unsigned_t lines = 1;
					bool first = true;

					for (cursor_t index = 0; ; ++index)
					{
						const char character = ReaderType::Read(text, index);
						if (character == static_cast<char>(AsciiDefinition::Control::NUL))
						{
							break;
						}

						if (character == static_cast<char>(AsciiDefinition::Control::LF)
							|| character == static_cast<char>(AsciiDefinition::Control::CR))
						{
							if (lineWidth > maxLineWidth)
							{
								maxLineWidth = lineWidth;
							}

							if (character == static_cast<char>(AsciiDefinition::Control::CR)
								&& ReaderType::Read(text, index + 1) == static_cast<char>(AsciiDefinition::Control::LF))
							{
								++index;
							}

							cursorX = originX;
							cursorY += static_cast<signed_t>(metrics.GetLineHeight());
							lineWidth = 0;
							++lines;
							first = true;
							continue;
						}

						if (character == static_cast<char>(AsciiDefinition::Control::HT))
						{
							if (!first)
								cursorX += static_cast<signed_t>(metrics.KerningWidth);

							if (metrics.TabWidth > 0)
							{
								const signed_t remainder = cursorX % static_cast<signed_t>(metrics.TabWidth);
								cursorX += (remainder == 0) ? static_cast<signed_t>(metrics.TabWidth) : static_cast<signed_t>(metrics.TabWidth) - remainder;
							}
						}
						else if (character == static_cast<char>(AsciiDefinition::Printable::Space))
						{
							cursorX += static_cast<signed_t>(metrics.SpaceWidth);
						}
						else if (static_cast<uint8_t>(character) >= AsciiDefinition::PrintableStart
							&& static_cast<uint8_t>(character) < AsciiDefinition::PrintableEnd)
						{
							if (!first)
								cursorX += static_cast<signed_t>(metrics.KerningWidth);

							onCharacter(character, cursorX, cursorY, first);
							cursorX += static_cast<signed_t>(metrics.FontWidth);
						}

						lineWidth = cursorX - originX;
						first = false;
					}

					if (lineWidth > maxLineWidth)
					{
						maxLineWidth = lineWidth;
					}

					return Measurement{ static_cast<unsigned_t>(maxLineWidth), lines };
				}

				template<typename CharacterCallback>
				static Measurement MeasureText(const char* text, const TextWalkerMetrics& metrics,
					const signed_t originX, const signed_t originY, CharacterCallback onCharacter,
					const Framework::DataSourceTypeEnum dataSourceType)
				{
					return dataSourceType == Framework::DataSourceTypeEnum::Flash
						? MeasureText<CharacterCallback, Reader::Flash>(text, metrics, originX, originY, onCharacter)
						: MeasureText<CharacterCallback, Reader::Ram>(text, metrics, originX, originY, onCharacter);
				}

				template<typename CharacterCallback, typename ReaderType = Reader::Ram>
				static unsigned_t WalkText(const char* text, const TextWalkerMetrics& metrics,
					const signed_t originX, const signed_t originY, CharacterCallback onCharacter)
				{
					return MeasureText<CharacterCallback, ReaderType>(text, metrics, originX, originY, onCharacter).Width;
				}

				template<typename CharacterCallback>
				static unsigned_t WalkText(const char* text, const TextWalkerMetrics& metrics,
					const signed_t originX, const signed_t originY, CharacterCallback onCharacter,
					const Framework::DataSourceTypeEnum dataSourceType)
				{
					return dataSourceType == Framework::DataSourceTypeEnum::Flash
						? WalkText<CharacterCallback, Reader::Flash>(text, metrics, originX, originY, onCharacter)
						: WalkText<CharacterCallback, Reader::Ram>(text, metrics, originX, originY, onCharacter);
				}
			};
		}
	}
}
#endif