// CharacterBuffer.h

#ifndef _CHARACTER_BUFFER_h
#define _CHARACTER_BUFFER_h

#include "../../../Model/Character.h"

template<const uint8_t CharacterCount = 30,
	const uint8_t HistoryLineCount = 10>
class CharacterBuffer
{
public:
	int8_t LineBuffer[CharacterCount]{};
	int8_t TextBuffer[(size_t)HistoryLineCount * CharacterCount]{};

private:
	uint8_t LinePosition = 0;
	uint8_t CharPosition = 0;

public:
	CharacterBuffer()
	{
		Clear();
	}

	const size_t GetLineOffset(const uint8_t line) const
	{
		return ((size_t)GetLineIndex(line) * CharacterCount);
	}

	void Clear()
	{
		for (uint8_t line = 0; line < HistoryLineCount; line++)
		{
			for (uint8_t i = 0; i < CharacterCount; i++)
			{
				const size_t offset = ((size_t)line * CharacterCount) + i;
				TextBuffer[offset] = Character::Space;
			}
		}

		for (uint8_t i = 0; i < CharacterCount; i++)
		{
			LineBuffer[i] = Character::Space;
		}
		CharPosition = 0;
	}

public:
	void write(const char character)
	{
		switch (character)
		{
		case Character::Tab:
			if (CharPosition & 1)
			{
				CharPosition += 1;
			}
			else
			{
				CharPosition += 2;
			}
			break;
		case Character::NewLine:
			println();
			break;
		default:
			if (CharPosition < CharacterCount)
			{
				LineBuffer[CharPosition++] = character;
			}
			break;
		}
	}

private:
	void println()
	{
		LinePosition++;

		if (LinePosition >= HistoryLineCount)
		{
			LinePosition = 0;
		}

		for (uint8_t i = 0; i < CharacterCount; i++)
		{
			const size_t offset = ((size_t)LinePosition * CharacterCount) + i;
			TextBuffer[offset] = LineBuffer[i];
		}

		for (uint8_t c = 0; c < CharacterCount; c++)
		{
			LineBuffer[c] = Character::Space;
		}
		CharPosition = 0;
	}

	const uint8_t GetLineIndex(const uint8_t line) const
	{
		if (LinePosition < line)
		{
			return HistoryLineCount - (line - LinePosition);
		}
		else
		{
			return LinePosition - line;
		}
	}
};
#endif