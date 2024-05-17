// BitmapDemo.h

#ifndef _BITMAP_DEMO_h
#define _BITMAP_DEMO_h

#include <ArduinoGraphicsDrawer.h>
#include "Sprites.h"

using namespace DemoSprites;

class BitmapDemo : public ElementDrawer
{
private:
	enum class DrawElementsEnum : uint8_t
	{
		Bitmap,
		DrawElementsCount
	};

	TransparentColorEffect<DogeSprite> Doge{};

public:
	BitmapDemo(IFrameBuffer* frame)
		: ElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{
		Doge.SetTransparentColor(0, 0, 0);
	}

	/// <summary>
	/// ElementIndex can be used to separate draw calls, avoiding hogging the co-operative scheduler.
	/// </summary>
	void DrawCall(DrawState* drawState) final
	{
		switch (drawState->ElementIndex)
		{
		case (uint8_t)DrawElementsEnum::Bitmap:
			DrawBitmap(drawState);
			break;
		default:
			break;
		}
	}

private:
	void DrawBitmap(DrawState* drawState)
	{
		const uint8_t usableX = Frame->GetWidth() - Doge.Width - 1;
		const uint8_t usableY = Frame->GetHeight() - Doge.Height - 1;

		const uint16_t progressX = drawState->GetProgress<7000000>();
		const uint16_t progressY = drawState->GetProgress<3000000>();

		const uint8_t x = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progressX), usableX);
		const uint8_t y = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progressY), usableY);

		SpriteRenderer::Draw(Frame, &Doge, x, y);
	}
};

#endif

