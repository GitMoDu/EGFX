// PrimitiveDemo.h

#ifndef _PRIMITIVE_DEMO_h
#define _PRIMITIVE_DEMO_h

#include <ArduinoGraphicsDrawer.h>

class PrimitiveDemo : public ElementDrawer
{
private:
	enum class DrawElementsEnum : uint8_t
	{
		DiagonalLines,
		AlignmentRectangle,
		AlignmentSquare,
		BreathSquare,
		WandererSquare,
		DrawElementsCount
	};

	static constexpr uint8_t MoveCount = 4;
	static constexpr uint32_t MovePeriodMicros = 1000000;
	static constexpr uint32_t BreathPeriodMicros = 2500000;
	static constexpr uint32_t SquareAlignmentColorPeriodMicros = 1000000 * 3;
	static constexpr uint32_t SquareColorPeriodMicros = 1000000 * 10;

	RgbColor Color;

public:
	PrimitiveDemo(IFrameBuffer* frame)
		: ElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{}

	/// <summary>
	/// ElementIndex can be used to separate draw calls, avoiding hogging the co-operative scheduler.
	/// </summary>
	virtual void DrawCall(DrawState* drawState) final
	{
		switch (drawState->ElementIndex)
		{
		case (uint8_t)DrawElementsEnum::DiagonalLines:
			DrawDiagonalLines(drawState);
			break;
		case (uint8_t)DrawElementsEnum::AlignmentRectangle:
			DrawAlignmentRectangle();
			break;
		case (uint8_t)DrawElementsEnum::AlignmentSquare:
			DrawAlignmentSquare(drawState);
			break;
		case (uint8_t)DrawElementsEnum::BreathSquare:
			DrawBreathSquare(drawState);
			break;
		case (uint8_t)DrawElementsEnum::WandererSquare:
			DrawWandererSquare(drawState);
			break;
		default:
			break;
		}
	}

	void DrawDiagonalLines(DrawState* drawState)
	{
		Color.r = UINT8_MAX / 4;
		Color.g = 0;
		Color.b = 0;

		Frame->Line(Color, 0, 0, Frame->GetWidth() - 1, Frame->GetHeight() - 1);
		Frame->Line(Color, 0, Frame->GetHeight() - 1, Frame->GetWidth() - 1, 0);
	}

	void DrawAlignmentRectangle()
	{
		const uint8_t wandererSize = GetWandererDimension();
		Color.r = UINT8_MAX;
		Color.g = UINT8_MAX;
		Color.b = UINT8_MAX;
		Frame->Rectangle(Color, wandererSize + 1, wandererSize + 1, Frame->GetWidth() - wandererSize - 1, Frame->GetHeight() - wandererSize - 1);
	}

	void DrawAlignmentSquare(DrawState* drawState)
	{
		const uint8_t squareSize = GetShortestDimension() - ((GetWandererDimension() + 2) * 2);

		const uint8_t x = (Frame->GetWidth() - squareSize) / 2;
		const uint8_t y = (Frame->GetHeight() - squareSize) / 2;

		const uint16_t colorProgress = drawState->GetProgress<SquareAlignmentColorPeriodMicros>();

		Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		Frame->Rectangle(Color, x, y, x + squareSize, y + squareSize);
	}

	void DrawBreathSquare(DrawState* drawState)
	{
		const uint8_t minSquareSize = 2;
		const uint8_t maxSquareSize = GetShortestDimension() - ((GetWandererDimension() + 3) * 2);
		const uint16_t breathProgress = ProgressScaler::TriangleResponse(drawState->GetProgress<BreathPeriodMicros>());
		const uint8_t squareSize = (((uint32_t)breathProgress * maxSquareSize) / UINT16_MAX) + (((uint32_t)(UINT16_MAX - breathProgress) * minSquareSize) / UINT16_MAX);

		const uint8_t x = ((Frame->GetWidth() - squareSize) / 2);
		const uint8_t y = ((Frame->GetHeight() - squareSize) / 2);

		const uint16_t colorProgress = drawState->GetProgress<SquareColorPeriodMicros>();

		Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		Frame->RectangleFill(Color, x, y, x + squareSize, y + squareSize);
	}

	void DrawWandererSquare(DrawState* drawState)
	{
		const uint16_t sectionProgress = drawState->GetProgress<MovePeriodMicros * MoveCount>();
		const uint8_t section = (((uint32_t)sectionProgress * MoveCount) / UINT16_MAX);
		const uint16_t progress = ProgressScaler::GetProgress<MovePeriodMicros>((drawState->FrameTime - (MovePeriodMicros * section)));
		const uint8_t wandererSize = GetWandererDimension();

		uint8_t x = 0;
		uint8_t y = 0;

		switch (section)
		{
		case 0:
			x = ((uint32_t)progress * (Frame->GetWidth() - 1 - wandererSize)) / UINT16_MAX;
			y = 0;
			Color.r = UINT8_MAX;
			Color.g = 0;
			Color.b = 0;
			break;
		case 1:
			x = Frame->GetWidth() - wandererSize - 1;
			y = ((uint32_t)progress * (Frame->GetHeight() - 1 - wandererSize)) / UINT16_MAX;
			Color.r = 0;
			Color.g = UINT8_MAX;
			Color.b = 0;
			break;
		case 2:
			x = ((uint32_t)(UINT16_MAX - progress) * (Frame->GetWidth() - 1 - wandererSize)) / UINT16_MAX;
			y = Frame->GetHeight() - wandererSize - 1;
			Color.r = 0;
			Color.g = 0;
			Color.b = UINT8_MAX;
			break;
		case 3:
			x = 0;
			y = ((uint32_t)(UINT16_MAX - progress) * (Frame->GetHeight() - 1 - wandererSize)) / UINT16_MAX;

			switch (drawState->FrameCounter % 3)
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
		default:
			break;
		}

		Frame->RectangleFill(Color, x, y, x + wandererSize, y + wandererSize);
	}

private:
	const uint8_t GetWandererDimension() const
	{
		return GetShortestDimension() / 4;
	}

	const uint8_t GetAlignmentCircleDiameter() const
	{
		return	GetShortestDimension() - (2 * (GetWandererDimension() + 1));
	}
};
#endif