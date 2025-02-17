// DisplayPrint.h

#ifndef _DISPLAY_PRINT_h
#define _DISPLAY_PRINT_h

#include "DisplayPrintLayout.h"

/// <summary>
/// Drawer with Print.h interface and statically allocated character buffer.
/// Inherits directly from Print class, so ElementDrawer is not used.
/// </summary>
/// <typeparam name="Layout">Layout of LayoutElement type.</typeparam>
/// <typeparam name="FontRendererType"></typeparam>
/// <typeparam name="width"></typeparam>
/// <typeparam name="height"></typeparam>
template<typename Layout,
	typename FontRendererType>
class DisplayPrint : public Print, public virtual IFrameDraw
{
private:
	CharacterBuffer<Layout::CharacterCount(), Layout::LineCount()> Buffer{};

	FontRendererType Renderer{};

private:
	uint8_t ElementIndex = 0;

public:
	DisplayPrint() : IFrameDraw(), Print()
	{
	}

	FontRendererType& GetRenderer()
	{
		return Renderer;
	}

	void DrawCall(IFrameBuffer* frame, const uint8_t elementIndex)
	{
		if (elementIndex == 0)
		{
			DrawLineBuffer(frame);
		}
		else
		{
			DrawHistory(frame, elementIndex - 1);
		}
	}

	const bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
	{
		DrawCall(frame, ElementIndex);

		ElementIndex++;
		if (ElementIndex >= Layout::ElementsCount())
		{
			ElementIndex = 0;

			return true;
		}

		return false;
	}

private:
	void DrawHistory(IFrameBuffer* frame, const uint8_t line)
	{
		const size_t offset = Buffer.GetLineOffset(line);
		const uint8_t y = Layout::Y() + Layout::Height() - ((2 + line) * (FontRendererType::FontHeight() + 1));
		uint8_t x = Layout::X();

		for (uint8_t i = 0; i < Layout::CharacterCount(); i++)
		{
			Renderer.Write(frame, x, y, Buffer.TextBuffer[offset + i]);
			x += FontRendererType::FontWidth() + FontRendererType::FontKerning();
		}
	}

	void DrawLineBuffer(IFrameBuffer* frame)
	{
		const uint8_t y = Layout::Y() + Layout::Height() - FontRendererType::FontHeight() - 1;
		uint8_t x = Layout::X();

		for (uint8_t i = 0; i < Layout::CharacterCount(); i++)
		{
			Renderer.Write(frame, x, y, Buffer.LineBuffer[i]);
			x += FontRendererType::FontWidth() + FontRendererType::FontKerning();
		}
	}

public:
	size_t write(uint8_t character) final
	{
		Buffer.write((const char)character);

		return 1;
	}

#if defined(ARDUINO_ARCH_STM32F1)
	size_t write(const void* buffer, uint32 size) final
#else
	size_t write(const uint8_t* buffer, size_t size) final
#endif
	{
		for (size_t i = 0; i < size; i++)
		{
			Buffer.write((const char)((const uint8_t*)buffer)[i]);
		}

		return size;
	}
};

/// <summary>
/// ElementDrawer based wrapper for DisplayPrint.
/// Use GetSerial() to get the Print(able) object reference.
/// </summary>
/// <typeparam name="FontRendererType"></typeparam>
/// <typeparam name="Layout"></typeparam>
template<typename Layout, typename FontRendererType>
class DisplayPrintElement : public ElementDrawer
{
private:
	using DisplayPrintDrawer = DisplayPrint<Layout, FontRendererType>;

private:
	DisplayPrintDrawer PrintDrawer;

public:
	DisplayPrintElement()
		: ElementDrawer((uint8_t)Layout::ElementsCount())
		, PrintDrawer()
	{
	}

	virtual void DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter, const uint8_t elementIndex) final
	{
		PrintDrawer.DrawCall(frame, elementIndex);
	}

	Print& GetSerial()
	{
		return PrintDrawer;
	}

	FontRendererType& GetRenderer()
	{
		return PrintDrawer.GetRenderer();
	}
};

#endif