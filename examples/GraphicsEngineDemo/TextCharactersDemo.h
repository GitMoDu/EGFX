// TextCharactersDemo.h

#ifndef _TEXT_CHARACTERS_DEMO_h
#define _TEXT_CHARACTERS_DEMO_h

#include <ArduinoGraphicsDrawer.h>

class TextCharactersDemo : public ElementDrawer
{
private:
	enum class DrawElementsEnum : uint8_t
	{
		UpperCase1,
		UpperCase2,
		LowerCase1,
		LowerCase2,
		DrawElementsCount
	};

	static constexpr uint32_t TextColorPeriodMicros = 3000000;
	static constexpr uint32_t NumberPeriodMicros = UINT32_MAX;
	static constexpr uint32_t NumberRange = 100000;

	RgbColor Color{};
	FontStyle SmallFont{};

public:
	TextCharactersDemo(IFrameBuffer* frame)
		: ElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{
		SmallFont.SetHeight(FontStyle::FONT_SIZE_REGULAR + 2, FontStyle::WIDTH_RATIO_THIN);
	}

	/// <summary>
	/// ElementIndex can be used to separate draw calls, avoiding hogging the co-operative scheduler.
	/// </summary>
	virtual void DrawCall(DrawState* drawState) final
	{
		switch (drawState->ElementIndex)
		{
		case (uint8_t)DrawElementsEnum::UpperCase1:
			DrawUpperCase1(drawState);
			break;
		case (uint8_t)DrawElementsEnum::UpperCase2:
			DrawUpperCase2(drawState);
			break;
		case (uint8_t)DrawElementsEnum::LowerCase1:
			DrawLowerCase1(drawState);
			break;
		case (uint8_t)DrawElementsEnum::LowerCase2:
			DrawLowerCase2(drawState);
			break;
		default:
			break;
		}
	}

private:
	void DrawUpperCase1(DrawState* drawState)
	{
		const uint16_t colorProgress = drawState->GetProgress<TextColorPeriodMicros>();

		SmallFont.Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		SmallFont.Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		SmallFont.Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		if (ShowUpperCase1())
		{
			if (ShowFullSet())
			{
				TextRenderer::TextTopLeft(Frame, SmallFont, 0, 0, F("ABCDEFGHIJKLM"));
			}
			else
			{
				TextRenderer::TextTopLeft(Frame, SmallFont, 0, 0, F("ABCDEFG"));
			}
		}
	}

	void DrawUpperCase2(DrawState* drawState)
	{
		const uint16_t colorProgress = drawState->GetProgress<TextColorPeriodMicros>();

		SmallFont.Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		SmallFont.Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		SmallFont.Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		if (ShowUpperCase2())
		{
			if (ShowFullSet())
			{
				TextRenderer::TextTopLeft(Frame, SmallFont, 0, SmallFont.Height + 1, F("NOPQRSTUVWXYZ"));
			}
			else
			{
				TextRenderer::TextTopLeft(Frame, SmallFont, 0, SmallFont.Height + 1, F("NOPQRST"));
			}
		}
	}

	void DrawLowerCase1(DrawState* drawState)
	{
		const uint16_t colorProgress = drawState->GetProgress<TextColorPeriodMicros>();

		SmallFont.Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		SmallFont.Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		SmallFont.Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		if (ShowLowerCase1())
		{
			if (ShowFullSet())
			{
				TextRenderer::TextTopLeft(Frame, SmallFont, 0, (SmallFont.Height + 1) * 2, F("abcdefghijklm"));
			}
			else
			{
				TextRenderer::TextTopLeft(Frame, SmallFont, 0, (SmallFont.Height + 1) * 2, F("abcdefg"));
			}
		}
	}

	void DrawLowerCase2(DrawState* drawState)
	{
		const uint16_t colorProgress = drawState->GetProgress<TextColorPeriodMicros>();

		SmallFont.Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		SmallFont.Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		SmallFont.Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		if (ShowLowerCase2())
		{
			if (ShowFullSet())
			{
				TextRenderer::TextTopLeft(Frame, SmallFont, 0, (SmallFont.Height + 1) * 3, F("nopqrstuvwxyz"));
			}
			else
			{
				TextRenderer::TextTopLeft(Frame, SmallFont, 0, (SmallFont.Height + 1) * 3, F("nopqrst"));
			}
		}
	}

	const bool ShowFullSet()
	{
		return Frame->GetWidth() > SmallFont.GetTextWidth(13) + 1;
	}

	const bool ShowUpperCase1()
	{
		return Frame->GetHeight() >= SmallFont.Height;
	}

	const bool ShowUpperCase2()
	{
		return ShowUpperCase1() && (Frame->GetHeight() > ((SmallFont.Height * 2) + 1));
	}

	const bool ShowLowerCase1()
	{
		return ShowUpperCase2() && (Frame->GetHeight() > ((SmallFont.Height * 3) + 2));
	}

	const bool ShowLowerCase2()
	{
		return ShowLowerCase1() && (Frame->GetHeight() > ((SmallFont.Height * 4) + 3));
	}
};
#endif