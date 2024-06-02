// DisplayPrint.h

#ifndef _DISPLAY_PRINT_h
#define _DISPLAY_PRINT_h

#include <ArduinoGraphicsDrawer.h>
#include "CharacterBuffer/CharacterBuffer.h"

/// <summary>
/// Drawer with Print.h interface and statically allocated character buffer.
/// Inherits directly from Print class, so ElementDrawer is not used.
/// </summary>
/// <typeparam name="FontRendererType"></typeparam>
/// <typeparam name="width"></typeparam>
/// <typeparam name="height"></typeparam>
template<const uint8_t width,
	const uint8_t height,
	typename FontRendererType = SpriteFont5x5Renderer>
class DisplayPrint : public Print, public virtual IFrameDraw
{
private:
	static constexpr uint8_t SmallestUint8(const uint8_t a, const uint8_t b)
	{
		return (a * (a <= b)) | (b * (b < a));
	}

	static constexpr uint8_t GetLinesCount(const uint8_t fontHeight, const uint8_t maxLineCount = UINT8_MAX)
	{
		return SmallestUint8(((height - (fontHeight + 1)) / (fontHeight + 1)), maxLineCount);
	}

	static constexpr uint8_t GetCharactersCount(const uint8_t fontWidth, const uint8_t maxCharacterCount = UINT8_MAX)
	{
		return SmallestUint8(width / (fontWidth + 1), maxCharacterCount);
	}

private:
	static constexpr uint8_t CharacterCount = GetCharactersCount(FontRendererType::FontWidth);
	static constexpr uint8_t LineCount = GetLinesCount(FontRendererType::FontHeight);
	static constexpr uint8_t ElementsCount = 1 + LineCount;

private:
	CharacterBuffer<CharacterCount, LineCount> Buffer{};

	FontRendererType FontRenderer{};

public:
	RgbColor Color{ INT8_MAX, INT8_MAX, INT8_MAX };

private:
	IFrameBuffer* Frame;

	const uint8_t X;
	const uint8_t Y;

	uint8_t ElementIndex = 0;

public:
	DisplayPrint(IFrameBuffer* frame, const uint8_t x = 0, const uint8_t y = 0)
		: IFrameDraw()
		, Print()
		, Frame(frame)
		, X(x), Y(y)
	{}

	const bool DrawToFrame(const uint32_t frameTime, const uint16_t frameCounter) final
	{
		if (ElementIndex == 0)
		{
			DrawLineBuffer();
		}
		else
		{
			DrawHistory(ElementIndex - 1);
		}

		ElementIndex++;
		if (ElementIndex >= ElementsCount)
		{
			ElementIndex = 0;

			return true;
		}

		return false;
	}

private:
	void DrawHistory(const uint8_t line)
	{
		const size_t offset = Buffer.GetLineOffset(line);
		const uint8_t y = height - ((2 + line) * (FontRenderer.FontHeight + 1));
		uint8_t x = X;
		for (uint8_t i = 0; i < CharacterCount; i++)
		{
			FontRenderer.Write(Frame, Color, x, y, Buffer.TextBuffer[offset + i]);
			x += FontRenderer.FontWidth + 1;
		}
	}

	void DrawLineBuffer()
	{
		const uint8_t y = height - FontRenderer.FontHeight - 1;
		uint8_t x = X;
		for (uint8_t i = 0; i < CharacterCount; i++)
		{
			FontRenderer.Write(Frame, Color, x, y, Buffer.LineBuffer[i]);
			x += FontRenderer.FontWidth + 1;
		}
	}

public:
	size_t write(uint8_t character)
	{
		Buffer.write((const char)character);

		return 1;
	}

	size_t write(const uint8_t* buffer, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			Buffer.write((const char)buffer[i]);
		}

		return size;
	}
};
#endif