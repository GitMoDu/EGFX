// PrimitiveDemo.h

#ifndef _PRIMITIVE_DEMO_h
#define _PRIMITIVE_DEMO_h

#include <ArduinoGraphicsDrawer.h>

template<typename Layout>
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

	static constexpr uint8_t GetWandererDimension()
	{
		return GetShortest() / 4;
	}

	static constexpr uint8_t GetAlignmentCircleDiameter()
	{
		return	GetShortest() - (2 * (GetWandererDimension() + 1));
	}

	static constexpr uint8_t GetShortest()
	{
		return ((Layout::Height() >= Layout::Width()) * (Layout::Width())) | ((Layout::Height() < Layout::Width()) * (Layout::Height()));
	}

	RgbColor Color;

public:
	PrimitiveDemo(IFrameBuffer* frame)
		: ElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{}

	/// <summary>
	/// ElementIndex can be used to separate draw calls, avoiding hogging the co-operative scheduler.
	/// </summary>
	void DrawCall(DrawState* drawState) final
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

private:
	void DrawDiagonalLines(DrawState* drawState)
	{
		Color.r = UINT8_MAX / 4;
		Color.g = 0;
		Color.b = 0;

		Frame->Line(Color, Layout::X(), Layout::Y(),
			Layout::X() + Layout::Width() - 1, Layout::Y() + Layout::Height() - 1);
		Frame->Line(Color, Layout::X(), Layout::Y() + Layout::Height() - 1, Layout::Width() - 1, 0);
	}

	void DrawAlignmentRectangle()
	{
		static constexpr uint8_t wandererSize = GetWandererDimension();
		Color.r = UINT8_MAX;
		Color.g = UINT8_MAX;
		Color.b = UINT8_MAX;
		Frame->Rectangle(Color, Layout::X() + wandererSize + 1, Layout::Y() + wandererSize + 1,
			Layout::X() + Layout::Width() - wandererSize - 1, Layout::Y() + Layout::Height() - wandererSize - 1);
	}

	void DrawAlignmentSquare(DrawState* drawState)
	{
		static constexpr uint8_t squareSize = GetShortest() - ((GetWandererDimension() + 2) * 2);

		static constexpr uint8_t x = Layout::X() + ((Layout::Width() - squareSize) / 2);
		static constexpr uint8_t y = Layout::Y() + ((Layout::Height() - squareSize) / 2);

		const uint16_t colorProgress = drawState->GetProgress<SquareAlignmentColorPeriodMicros>();

		Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		Frame->Rectangle(Color, x, y, x + squareSize, y + squareSize);
	}

	void DrawBreathSquare(DrawState* drawState)
	{
		static constexpr uint8_t minSquareSize = 2;
		static constexpr uint8_t maxSquareSize = GetShortest() - ((GetWandererDimension() + 3) * 2);

		const uint16_t breathProgress = ProgressScaler::TriangleResponse(drawState->GetProgress<BreathPeriodMicros>());
		const uint8_t squareSize = (((uint32_t)breathProgress * maxSquareSize) / UINT16_MAX) + (((uint32_t)(UINT16_MAX - breathProgress) * minSquareSize) / UINT16_MAX);

		const uint8_t x = ((Layout::Width() - squareSize) / 2);
		const uint8_t y = ((Layout::Height() - squareSize) / 2);

		const uint16_t colorProgress = drawState->GetProgress<SquareColorPeriodMicros>();

		Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		Frame->RectangleFill(Color, x, y, x + squareSize, y + squareSize);
	}

	void DrawWandererSquare(DrawState* drawState)
	{
		const uint16_t section = ProgressScaler::ScaleProgress(drawState->GetProgress<MovePeriodMicros * MoveCount>(), MoveCount);
		const uint16_t progress = drawState->GetProgress<MovePeriodMicros>();
		const uint8_t wandererSize = GetWandererDimension();

		uint8_t x = 0;
		uint8_t y = 0;

		switch (section)
		{
		case 0:
			x = ((uint32_t)progress * (Layout::Width() - 1 - wandererSize)) / UINT16_MAX;
			y = 0;
			Color.r = UINT8_MAX;
			Color.g = 0;
			Color.b = 0;
			break;
		case 1:
			x = Layout::Width() - wandererSize - 1;
			y = ((uint32_t)progress * (Layout::Height() - 1 - wandererSize)) / UINT16_MAX;
			Color.r = 0;
			Color.g = UINT8_MAX;
			Color.b = 0;
			break;
		case 2:
			x = ((uint32_t)(UINT16_MAX - progress) * (Layout::Width() - 1 - wandererSize)) / UINT16_MAX;
			y = Layout::Height() - wandererSize - 1;
			Color.r = 0;
			Color.g = 0;
			Color.b = UINT8_MAX;
			break;
		case 3:
			x = 0;
			y = ((uint32_t)(UINT16_MAX - progress) * (Layout::Height() - 1 - wandererSize)) / UINT16_MAX;

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

		Frame->RectangleFill(Color, Layout::X() + x, Layout::Y() + y, Layout::X() + x + wandererSize, Layout::Y() + y + wandererSize);
	}

};
#endif