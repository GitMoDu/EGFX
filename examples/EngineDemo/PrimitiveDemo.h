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

	rgb_color_t Color;

public:
	PrimitiveDemo()
		: ElementDrawer((uint8_t)DrawElementsEnum::DrawElementsCount)
	{
	}

	/// <summary>
	/// ElementIndex can be used to separate draw calls, avoiding hogging the co-operative scheduler.
	/// </summary>
	virtual void DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter, const uint8_t elementIndex) final
	{
		switch (elementIndex)
		{
		case (uint8_t)DrawElementsEnum::DiagonalLines:
			DrawDiagonalLines(frame);
			break;
		case (uint8_t)DrawElementsEnum::AlignmentRectangle:
			DrawAlignmentRectangle(frame);
			break;
		case (uint8_t)DrawElementsEnum::AlignmentSquare:
			DrawAlignmentSquare(frame, frameTime);
			break;
		case (uint8_t)DrawElementsEnum::BreathSquare:
			DrawBreathSquare(frame, frameTime);
			break;
		case (uint8_t)DrawElementsEnum::WandererSquare:
			DrawWandererSquare(frame, frameTime, frameCounter);
			break;
		default:
			break;
		}
	}

private:
	void DrawDiagonalLines(IFrameBuffer* frame)
	{
		Color = Rgb::Color(UINT8_MAX / 4, 0, 0);

		frame->Line(Color, Layout::X(), Layout::Y(),
			Layout::X() + Layout::Width() - 1, Layout::Y() + Layout::Height() - 1);
		frame->Line(Color, Layout::X(), Layout::Y() + Layout::Height() - 1, Layout::Width() - 1, 0);
	}

	void DrawAlignmentRectangle(IFrameBuffer* frame)
	{
		static constexpr pixel_t wandererSize = GetWandererDimension();
		Color = Rgb::Color(UINT8_MAX, UINT8_MAX, UINT8_MAX);

		frame->Rectangle(Color, Layout::X() + wandererSize + 1, Layout::Y() + wandererSize + 1,
			Layout::X() + Layout::Width() - wandererSize - 1, Layout::Y() + Layout::Height() - wandererSize - 1);
	}

	void DrawAlignmentSquare(IFrameBuffer* frame, const uint32_t frameTime)
	{
		static constexpr pixel_t squareSize = GetShortest() - ((GetWandererDimension() + 2) * 2);

		static constexpr pixel_t x = Layout::X() + ((Layout::Width() - squareSize) / 2);
		static constexpr pixel_t y = Layout::Y() + ((Layout::Height() - squareSize) / 2);

		const uint16_t colorProgress = ProgressScaler::GetProgress<SquareAlignmentColorPeriodMicros>(frameTime);

		const uint8_t r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		const uint8_t g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		const uint8_t b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		Color = Rgb::Color(r, g, b);

		frame->Rectangle(Color, x, y, x + squareSize, y + squareSize);
	}

	void DrawBreathSquare(IFrameBuffer* frame, const uint32_t frameTime)
	{
		static constexpr pixel_t minSquareSize = 2;
		static constexpr pixel_t maxSquareSize = GetShortest() - ((GetWandererDimension() + 3) * 2);

		const uint16_t breathProgress = ProgressScaler::TriangleResponse(ProgressScaler::GetProgress<BreathPeriodMicros>(frameTime));
		const pixel_t squareSize = (((uint32_t)breathProgress * maxSquareSize) / UINT16_MAX) + (((uint32_t)(UINT16_MAX - breathProgress) * minSquareSize) / UINT16_MAX);

		const pixel_t x = ((Layout::Width() - squareSize) / 2);
		const pixel_t y = ((Layout::Height() - squareSize) / 2);

		const uint16_t colorProgress = ProgressScaler::GetProgress<SquareColorPeriodMicros>(frameTime);

		const uint8_t r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		const uint8_t g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		const uint8_t b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		Color = Rgb::Color(r, g, b);

		frame->RectangleFill(Color, x, y, x + squareSize, y + squareSize);
	}

	void DrawWandererSquare(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter)
	{
		const uint16_t section = ProgressScaler::ScaleProgress(ProgressScaler::GetProgress<MovePeriodMicros * MoveCount>(frameTime), MoveCount);
		const uint16_t progress = ProgressScaler::GetProgress<MovePeriodMicros>(frameTime);
		const pixel_t wandererSize = GetWandererDimension();

		pixel_t x = 0;
		pixel_t y = 0;

		switch (section)
		{
		case 0:
			x = ((uint32_t)progress * (Layout::Width() - 1 - wandererSize)) / UINT16_MAX;
			y = 0;
			Color = Rgb::Color(UINT8_MAX, 0, 0);
			break;
		case 1:
			x = Layout::Width() - wandererSize - 1;
			y = ((uint32_t)progress * (Layout::Height() - 1 - wandererSize)) / UINT16_MAX;
			Color = Rgb::Color(0, UINT8_MAX, 0);
			break;
		case 2:
			x = ((uint32_t)(UINT16_MAX - progress) * (Layout::Width() - 1 - wandererSize)) / UINT16_MAX;
			y = Layout::Height() - wandererSize - 1;
			Color = Rgb::Color(0, 0, UINT8_MAX);
			break;
		case 3:
			x = 0;
			y = ((uint32_t)(UINT16_MAX - progress) * (Layout::Height() - 1 - wandererSize)) / UINT16_MAX;

			switch (frameCounter % 3)
			{
			case 0:
				Color = Rgb::Color(UINT8_MAX, 0, 0);
				break;
			case 1:
				Color = Rgb::Color(0, UINT8_MAX, 0);
				break;
			case 2:
			default:
				Color = Rgb::Color(0, 0, UINT8_MAX);
				break;
			}
		default:
			break;
		}

		frame->RectangleFill(Color, Layout::X() + x, Layout::Y() + y, Layout::X() + x + wandererSize, Layout::Y() + y + wandererSize);
	}

};
#endif