#ifndef _PRIMITIVE_DEMO_h
#define _PRIMITIVE_DEMO_h

#include <ArduinoGraphicsDrawer.h>

template<typename Layout, bool BinaryDisplay>
class PrimitiveDemo : public IFrameDraw
{
private:
	enum class DrawCallEnum : uint8_t
	{
		Lines,
		Rectangle,
		Triangle,
		EnumCount
	};

	struct LayerLayout
	{
		static constexpr pixel_t X() { return Layout::X(); }
		static constexpr pixel_t Y() { return Layout::Y(); }
		static constexpr pixel_t Width() { return Layout::Width(); }
		static constexpr pixel_t Height() { return Layout::Height(); }
		static constexpr pixel_t CenterX() { return X() + Width() / 2; }
		static constexpr pixel_t CenterY() { return Y() + Height() / 2; }

		struct Lines
		{
			static constexpr pixel_t HorizontalSeparation() { return Layout::Width() / 4; }
			static constexpr pixel_t VerticalSeparation() { return Layout::Height() / 4; }
			static constexpr pixel_t HorizontalCount() { return Layout::Width() / HorizontalSeparation(); }
			static constexpr pixel_t VerticalCount() { return Layout::Height() / VerticalSeparation(); }
		};

		struct Rectangle
		{
			static constexpr pixel_t CenterX() { return Layout::X() + (Layout::Width() / 2); }
			static constexpr pixel_t CenterY() { return Layout::Y() + (Layout::Height() / 2); }
		};

		struct Triangle
		{
			static constexpr pixel_t CenterX() { return Layout::X() + (Layout::Width() / 2); }
			static constexpr pixel_t CenterY() { return Layout::Y() + (Layout::Height() / 2); }
			static constexpr pixel_t MaxHeight() { return ((MinValue(Layout::Width(), Layout::Height())) * 3) / 4; }
		};
	};

private:
	pixel_point_t TriangleA{};
	pixel_point_t TriangleB{};
	pixel_point_t TriangleC{};

	uint8_t CallIndex = 0;

public:
	PrimitiveDemo() : IFrameDraw()
	{
	}

	~PrimitiveDemo() = default;

	// Always enabled.
	virtual bool IsEnabled() const final { return true; }
	virtual void SetEnabled(const bool /*enabled*/) final {}

#if defined(SERIAL_LOG)
	void PrintDescription() const
	{
		Serial.println(F("Primitives\n\tfast draw lines, rectangles, and triangles."));
	}
#endif

	virtual bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
	{
		switch (DrawCallEnum(CallIndex))
		{
		case DrawCallEnum::Lines:
			// Draw moving lines.
			DrawLines(frame, frameTime, frameCounter);
			break;
		case DrawCallEnum::Rectangle:
		{
			// Animate triangle points.
			//const uint16_t progress = ProgressScaler::GetProgress<MovePeriodMicros>(frameTime);
			TriangleA = GetRoundSquare<LayerLayout::Triangle::MaxHeight()>(frameTime / 90);
			TriangleB = GetRoundSquare<LayerLayout::Triangle::MaxHeight()>(frameTime / 166);
			TriangleC = GetRoundSquare<LayerLayout::Triangle::MaxHeight()>(frameTime / 75);

			const auto xShift = -GetTriangleXMax() / 2;
			const auto yShift = -GetTriangleYMax() / 2;

			// Draw rectangle around triangle.
			if (!BinaryDisplay || frameCounter & 0b1)
				frame->Rectangle(RGB_COLOR_WHITE,
					LayerLayout::Rectangle::CenterX() + GetTriangleXMin() - 1 + xShift,
					LayerLayout::Rectangle::CenterY() + GetTriangleYMin() - 1 + yShift,
					LayerLayout::Rectangle::CenterX() + GetTriangleXMax() + 1 + xShift,
					LayerLayout::Rectangle::CenterY() + GetTriangleYMax() + 1 + yShift);
		}
		break;
		case DrawCallEnum::Triangle:
		{
			rgb_color_t triangleColor;

			if (BinaryDisplay)
			{
				triangleColor = RGB_COLOR_WHITE;
			}
			else
			{
				const uint8_t segment = (frameTime / 400000) % 3;
				switch (segment)
				{
				case 0:
					triangleColor = RGB_COLOR_RED;
					break;
				case 1:
					triangleColor = RGB_COLOR_GREEN;
					break;
				case 2:
				default:
					triangleColor = RGB_COLOR_BLUE;
					break;
				}
			}

			const auto xShift = -GetTriangleXMax() / 2;
			const auto yShift = -GetTriangleYMax() / 2;

			// Draw triangle.
			frame->TriangleFill(triangleColor,
				LayerLayout::Triangle::CenterX() + TriangleA.x + xShift, LayerLayout::Triangle::CenterY() + TriangleA.y + yShift,
				LayerLayout::Triangle::CenterX() + TriangleB.x + xShift, LayerLayout::Triangle::CenterY() + TriangleB.y + yShift,
				LayerLayout::Triangle::CenterX() + TriangleC.x + xShift, LayerLayout::Triangle::CenterY() + TriangleC.y + yShift);
		}
		break;
		default:
			break;
		}

		CallIndex++; // Advance call index and determine if cycle is complete.
		if (CallIndex >= uint8_t(DrawCallEnum::EnumCount))
		{
			CallIndex = 0;
			return true;
		}

		return false;
	}

private:
	void DrawLines(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter)
	{
		const rgb_color_t lineColor = BinaryDisplay ? RGB_COLOR_WHITE : Rgb::ColorFromHSV(static_cast<angle_t>(frameTime / 100), UINT8_MAX, (uint16_t(UINT8_MAX) * 9) / 10);

		pixel_t deltaX = (frameTime / 70000) % LayerLayout::Width();
		if (deltaX >= LayerLayout::Width() / 2)
		{
			deltaX = (LayerLayout::Width() - deltaX) * 2;
		}
		else
		{
			deltaX = deltaX * 2;
		}

		pixel_t deltaY = (frameTime / 60000) % LayerLayout::Height();
		if (deltaY >= LayerLayout::Height() / 2)
		{
			deltaY = (LayerLayout::Height() - deltaY) * 2;
		}
		else
		{
			deltaY = deltaY * 2;
		}

		for (uint8_t i = 0; i < LayerLayout::Lines::HorizontalCount(); i++)
		{
			const pixel_t x = LayerLayout::CenterX() + ((((LayerLayout::Lines::HorizontalSeparation() / 2) * i) + deltaX / 2) % (LayerLayout::Width() / 2));
			frame->Line(lineColor, x, LayerLayout::Y(), x + deltaX, LayerLayout::Y() + LayerLayout::Height() - 1);

			const pixel_t y = LayerLayout::Y() + ((((LayerLayout::Lines::VerticalSeparation() / 2) * i) + deltaY / 2) % LayerLayout::Height());
			frame->Line(lineColor, LayerLayout::X(), y, LayerLayout::CenterX(), y + deltaY);
		}
	}

	template<pixel_t SquareDimension>
	pixel_point_t GetRoundSquare(const uint16_t progress)
	{
		const uint16_t section = ProgressScaler::ScaleProgress(progress, uint8_t(4));
		const uint16_t innerProgress = static_cast<uint16_t>(progress * 4);
		switch (section)
		{
		case 0:
			return pixel_point_t{
				static_cast<pixel_t>(ProgressScaler::ScaleProgress(innerProgress, uint16_t(SquareDimension))),
				static_cast<pixel_t>(0),
			};
			break;
		case 1:
			return pixel_point_t{
				static_cast<pixel_t>(SquareDimension - 1),
				static_cast<pixel_t>(ProgressScaler::ScaleProgress(innerProgress, uint16_t(SquareDimension))),
			};
			break;
		case 2:
			return pixel_point_t{
				static_cast<pixel_t>(SquareDimension - 1 - int16_t(ProgressScaler::ScaleProgress(innerProgress, uint16_t(SquareDimension)))),
				static_cast<pixel_t>(SquareDimension - 1),
			};
			break;
		case 3:
		default:
			return pixel_point_t{
			static_cast<pixel_t>(0),
			static_cast<pixel_t>(SquareDimension - 1 - int16_t(ProgressScaler::ScaleProgress(innerProgress, uint16_t(SquareDimension)))),
			};
			break;
		}
	}

	pixel_t GetTriangleXMin() const
	{
		pixel_t minX = TriangleA.x;
		if (TriangleB.x < minX)
		{
			minX = TriangleB.x;
		}
		if (TriangleC.x < minX)
		{
			minX = TriangleC.x;
		}
		return minX;
	}

	pixel_t GetTriangleXMax() const
	{
		pixel_t maxX = TriangleA.x;
		if (TriangleB.x > maxX)
		{
			maxX = TriangleB.x;
		}
		if (TriangleC.x > maxX)
		{
			maxX = TriangleC.x;
		}
		return maxX;
	}

	pixel_t GetTriangleYMin() const
	{
		pixel_t minY = TriangleA.y;
		if (TriangleB.y < minY)
		{
			minY = TriangleB.y;
		}
		if (TriangleC.y < minY)
		{
			minY = TriangleC.y;
		}
		return minY;
	}

	pixel_t GetTriangleYMax() const
	{
		pixel_t maxY = TriangleA.y;
		if (TriangleB.y > maxY)
		{
			maxY = TriangleB.y;
		}
		if (TriangleC.y > maxY)
		{
			maxY = TriangleC.y;
		}
		return maxY;
	}

	pixel_t GetTriangleHeight() const
	{
		return GetTriangleYMax() - GetTriangleYMin() + 1;
	}
	pixel_t GetTriangleWidth() const
	{
		return GetTriangleXMax() - GetTriangleXMin() + 1;
	}
};
#endif