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
		Numbers,
		DrawElementsCount
	};

	static constexpr uint32_t TextColorPeriodMicros = 3000000;
	static constexpr uint32_t NumberPeriodMicros = UINT32_MAX;
	static constexpr uint32_t NumberRange = 100000;

	RgbColor Color{};
	FontStyle SmallFont{};

public:
	TextCharactersDemo()
		: ElementDrawer((uint8_t)DrawElementsEnum::DrawElementsCount)
	{
		SmallFont.SetHeight(FontStyle::FONT_SIZE_REGULAR);
	}

	virtual void DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter, const uint8_t elementIndex) final
	{
		switch (elementIndex)
		{
		case (uint8_t)DrawElementsEnum::UpperCase1:
			DrawUpperCase1(frame, frameTime);
			break;
		case (uint8_t)DrawElementsEnum::UpperCase2:
			DrawUpperCase2(frame, frameTime);
			break;
		case (uint8_t)DrawElementsEnum::LowerCase1:
			DrawLowerCase1(frame, frameTime);
			break;
		case (uint8_t)DrawElementsEnum::LowerCase2:
			DrawLowerCase2(frame, frameTime);
			break;
		case (uint8_t)DrawElementsEnum::Numbers:
			DrawNumbers(frame, frameTime);
			break;
		default:
			break;
		}
	}

private:
	void DrawUpperCase1(IFrameBuffer* frame, const uint32_t frameTime)
	{
		const uint16_t colorProgress = ProgressScaler::GetProgress<TextColorPeriodMicros>(frameTime);

		SmallFont.Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		SmallFont.Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		SmallFont.Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		if (ShowUpperCase1(frame))
		{
			if (ShowFullSet(frame))
			{
				TextRenderer::TextTopLeft(frame, SmallFont, 0, 0, F("ABCDEFGHIJKLM"));
			}
			else
			{
				TextRenderer::TextTopLeft(frame, SmallFont, 0, 0, F("ABCDEFG"));
			}
		}
	}

	void DrawUpperCase2(IFrameBuffer* frame, const uint32_t frameTime)
	{
		const uint16_t colorProgress = ProgressScaler::GetProgress<TextColorPeriodMicros>(frameTime);

		SmallFont.Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		SmallFont.Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		SmallFont.Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		if (ShowUpperCase2(frame))
		{
			if (ShowFullSet(frame))
			{
				TextRenderer::TextTopLeft(frame, SmallFont, 0, SmallFont.Height + 1, F("NOPQRSTUVWXYZ"));
			}
			else
			{
				TextRenderer::TextTopLeft(frame, SmallFont, 0, SmallFont.Height + 1, F("HIJKLMN"));
			}
		}
	}

	void DrawLowerCase1(IFrameBuffer* frame, const uint32_t frameTime)
	{
		const uint16_t colorProgress = ProgressScaler::GetProgress<TextColorPeriodMicros>(frameTime);

		SmallFont.Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		SmallFont.Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		SmallFont.Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		if (ShowLowerCase1(frame))
		{
			if (ShowFullSet(frame))
			{
				TextRenderer::TextTopLeft(frame, SmallFont, 0, (SmallFont.Height + 1) * 2, F("abcdefghijklm"));
			}
			else
			{
				TextRenderer::TextTopLeft(frame, SmallFont, 0, (SmallFont.Height + 1) * 2, F("OPQRSTU"));
			}
		}
	}

	void DrawLowerCase2(IFrameBuffer* frame, const uint32_t frameTime)
	{
		const uint16_t colorProgress = ProgressScaler::GetProgress<TextColorPeriodMicros>(frameTime);

		SmallFont.Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		SmallFont.Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		SmallFont.Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		if (ShowLowerCase2(frame))
		{
			if (ShowFullSet(frame))
			{
				TextRenderer::TextTopLeft(frame, SmallFont, 0, (SmallFont.Height + 1) * 3, F("nopqrstuvwxyz"));
			}
			else
			{
				TextRenderer::TextTopLeft(frame, SmallFont, 0, (SmallFont.Height + 1) * 3, F("VWXYZ-,."));
			}
		}
	}

	void DrawNumbers(IFrameBuffer* frame, const uint32_t frameTime)
	{
		const uint16_t colorProgress = ProgressScaler::GetProgress<TextColorPeriodMicros>(frameTime);

		SmallFont.Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		SmallFont.Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		SmallFont.Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		if (ShowNumbers(frame))
		{
			TextRenderer::TextTopLeft(frame, SmallFont, 0, (SmallFont.Height + 1) * 4, F("1234567890"));
		}
	}

	const bool ShowFullSet(IFrameBuffer* frame)
	{
		return frame->GetFrameWidth() > SmallFont.GetTextWidth(13) + 1;
	}

	const bool ShowUpperCase1(IFrameBuffer* frame)
	{
		return frame->GetFrameHeight() >= SmallFont.Height;
	}

	const bool ShowUpperCase2(IFrameBuffer* frame)
	{
		return ShowUpperCase1(frame) && (frame->GetFrameHeight() > ((SmallFont.Height * 2) + 1));
	}

	const bool ShowLowerCase1(IFrameBuffer* frame)
	{
		return ShowUpperCase2(frame) && (frame->GetFrameHeight() > ((SmallFont.Height * 3) + 2));
	}

	const bool ShowLowerCase2(IFrameBuffer* frame)
	{
		return ShowLowerCase1(frame) && (frame->GetFrameHeight() > ((SmallFont.Height * 4) + 3));
	}

	const bool ShowNumbers(IFrameBuffer* frame)
	{
		return ShowLowerCase2(frame) && (frame->GetFrameHeight() > ((SmallFont.Height * 5) + 4));
	}
};
#endif