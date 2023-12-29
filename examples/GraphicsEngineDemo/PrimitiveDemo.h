// PrimitiveDemo.h

#ifndef _PRIMITIVE_DEMO_h
#define _PRIMITIVE_DEMO_h

#include <ArduinoGraphicsCore.h>

class PrimitiveDemo : public FrameElementDrawer
{
private:
	enum class DrawElementsEnum : uint8_t
	{
		DiagonalLines,
		AlignmentRectangle,
		AlignmentCircle,
		BreathCircle,
		SomethingPixels,
		WandererSquare,
		DrawElementsCount
	};

	static const uint8_t MoveCount = 4;
	static constexpr uint32_t MovePeriodMicros = 2 * MoveCount * 1000000;
	static constexpr uint32_t BreathPeriodMicros = 4 * 1000000;
	static constexpr uint32_t CircleColorPeriodMicros = 10 * 1000000;

	RgbColor Color;

	const uint8_t SquareDimension;
	const uint8_t CircleDimension;

public:
	PrimitiveDemo(IFramePrimitives* frame)
		: FrameElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
		, SquareDimension(GetWandererDimension())
		, CircleDimension((Height - (2 * SquareDimension)) / 2)
	{
	}

	/// <summary>
	/// ElementIndex can be used to separate draw calls, avoiding hogging the co-operative scheduler.
	/// </summary>
	virtual void DrawCall(const uint32_t frameTime, const uint16_t frameCounter, const uint8_t elementIndex) final
	{
		switch (elementIndex)
		{
		case (uint8_t)DrawElementsEnum::DiagonalLines:
			DrawDiagonalLines(frameTime);
			break;
		case (uint8_t)DrawElementsEnum::AlignmentRectangle:
			DrawAlignmentRectangle();
			break;
		case (uint8_t)DrawElementsEnum::AlignmentCircle:
			DrawAlignmentCircle();
			break;
		case (uint8_t)DrawElementsEnum::BreathCircle:
			DrawBreathCircle(frameTime);
			break;
		case (uint8_t)DrawElementsEnum::SomethingPixels:
			DrawSomethingPixels(frameCounter);
			break;
		case (uint8_t)DrawElementsEnum::WandererSquare:
			DrawWandererSquare(frameTime, frameCounter);
			break;
		default:
			break;
		}
	}

	void DrawDiagonalLines(const uint32_t frameTime)
	{
		if (IsDirectDraw)
		{
			// Clear path before diagonals, square and circles are drawn, to keep layer order.
			Color.r = 0;
			Color.g = 0;
			Color.b = 0;
			Frame->RectangleFill(0, 0, Width - 1, SquareDimension, Color);
			Frame->RectangleFill(0, Height - 1 - SquareDimension + 1, Width - 1, Height - 1, Color);
			Frame->RectangleFill(0, SquareDimension, SquareDimension, Height - 1 - SquareDimension, Color);
			Frame->RectangleFill(Width - 1 - SquareDimension + 1, SquareDimension, Width - 1, Height - 1 - SquareDimension, Color);
			Frame->CircleFill(Width / 2, Height / 2, CircleDimension - 1, Color);
		}

		Color.r = UINT8_MAX / 4;
		Color.g = 0;
		Color.b = 0;

		Frame->Line(0, 0, Width - 1, Height - 1, Color);
		Frame->Line(0, Height - 1, Width - 1, 0, Color);
	}

	void DrawAlignmentRectangle()
	{
		Color.r = UINT8_MAX;
		Color.g = UINT8_MAX;
		Color.b = UINT8_MAX;
		Frame->Rectangle(SquareDimension + 1, SquareDimension + 1, Width - SquareDimension, Height - SquareDimension, Color, 1);
	}

	void DrawAlignmentCircle()
	{
		Color.r = 0;
		Color.g = UINT8_MAX;
		Color.b = 0;
		Frame->Circle(Width / 2, Height / 2, CircleDimension, Color);
	}

	void DrawBreathCircle(const uint32_t frameTime)
	{
		const uint8_t minRadius = 3;
		const uint8_t maxRadius = CircleDimension - 1;

		const uint16_t colorProgress = GetProgress<CircleColorPeriodMicros>(frameTime);

		Color.r = ScaleProgress(TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		Color.g = ScaleProgress(TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		Color.b = ScaleProgress(TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		const uint16_t breathProgress = GetProgress<BreathPeriodMicros>(frameTime);
		const uint8_t section = (breathProgress / (UINT16_MAX / 2));
		const uint16_t sectionProgress = GetProgress<UINT16_MAX / 2>(breathProgress);

		if (section == 0)
		{
			const uint8_t breathRadius = minRadius + (((uint32_t)sectionProgress * (maxRadius - minRadius)) / UINT16_MAX);
			Frame->CircleFill(Width / 2, Height / 2, breathRadius, Color);
		}
		else
		{
			const uint8_t breathRadius = minRadius + (((uint32_t)(UINT16_MAX - sectionProgress) * (maxRadius - minRadius)) / UINT16_MAX);
			Frame->CircleFill(Width / 2, Height / 2, breathRadius, Color);
		}
	}

	void DrawSomethingPixels(const uint16_t frameCounter)
	{
		switch ((frameCounter / 10) % 3)
		{
		case 0:
			Color.r = UINT8_MAX;
			Color.g = 0;
			Color.b = 0;
			Frame->Pixel(Width / 2, 0, Color);
			break;
		case 1:
			Color.r = 0;
			Color.g = UINT8_MAX;
			Color.b = 0;
			Frame->Pixel(1 + (Width / 2), 0, Color);
			break;
		case 2:
			Color.r = 0;
			Color.g = 0;
			Color.b = UINT8_MAX;
			Frame->Pixel(1 + (Width / 2), 1, Color);
			break;
		default:
			break;
		}
	}

	void DrawWandererSquare(const uint32_t frameTime, const uint16_t frameCounter)
	{
		const uint16_t moveProgress = GetProgress<MovePeriodMicros>(frameTime);
		const uint8_t section = (moveProgress / (UINT16_MAX / MoveCount));
		const uint16_t sectionProgress = GetProgress<UINT16_MAX / MoveCount>(moveProgress);

		uint8_t x;
		uint8_t y;

		switch (section)
		{
		case 0:
			x = ((uint32_t)sectionProgress * (Width - SquareDimension)) / UINT16_MAX;
			y = 0;
			Color.r = UINT8_MAX;
			Color.g = 0;
			Color.b = 0;
			break;
		case 1:
			x = Width - SquareDimension;
			y = ((uint32_t)sectionProgress * (Height - SquareDimension)) / UINT16_MAX;
			Color.r = 0;
			Color.g = UINT8_MAX;
			Color.b = 0;
			break;
		case 2:
			x = ((uint32_t)(UINT16_MAX - sectionProgress) * (Width - SquareDimension)) / UINT16_MAX;
			y = Height - SquareDimension;
			Color.r = 0;
			Color.g = 0;
			Color.b = UINT8_MAX;
			break;
		case 3:
		default:
			x = 0;
			y = ((uint32_t)(UINT16_MAX - sectionProgress) * (Height - SquareDimension)) / UINT16_MAX;

			switch (frameCounter % 3)
			{
			case 0:
				Color.r = UINT8_MAX;
				Color.g = 0;
				Color.b = 0;
				break;
			case 1:
				Color.r = 0;
				Color.g = UINT8_MAX;
				Color.b = 0;
				break;
			case 2:
			default:
				Color.r = 0;
				Color.g = 0;
				Color.b = UINT8_MAX;
				break;
			}
			break;
		}

		Frame->RectangleFill(x, y, x + SquareDimension, y + SquareDimension, Color);
	}

private:
	const uint8_t GetWandererDimension()
	{
		return GetShortestDimension() / 5;
	}
};
#endif