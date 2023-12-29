// FrameElementDrawer.h

#ifndef _FRAME_ELEMENT_DRAWER_h
#define _FRAME_ELEMENT_DRAWER_h

#include "IFrame/IFrameDraw.h"
#include "GraphicsText.h"

class FrameElementDrawer : public virtual IFrameDraw
{
protected:
	IFramePrimitives* Frame;

protected:
	const uint8_t Width;
	const uint8_t Height;

private:
	uint8_t ElementsCount;
	uint8_t ElementIndex;

protected:
	const bool IsDirectDraw;

protected:
	virtual void DrawCall(const uint32_t frameTime, const uint16_t frameCounter, const uint8_t elementIndex) { }

public:
	FrameElementDrawer(IFramePrimitives* frame, const uint8_t elementsCount)
		: IFrameDraw()
		, Frame(frame)
		, Width(Frame->GetWidth())
		, Height(Frame->GetHeight())
		, ElementsCount(elementsCount)
		, ElementIndex(0)
		, IsDirectDraw(frame->IsDirectDraw())
	{}

	virtual const bool DrawToFrame(const uint32_t frameTime, const uint16_t frameCounter) final
	{
		DrawCall(frameTime, frameCounter, ElementIndex);
		ElementIndex++;
		if (ElementIndex >= ElementsCount)
		{
			ElementIndex = 0;

			return true;
		}

		return false;
	}

protected:
	const uint8_t GetShortestDimension()
	{
		if (Height >= Width)
		{
			return Width;
		}
		else
		{
			return Height;
		}
	}

protected:
	template<const uint32_t PeriodMicros>
	static constexpr const uint16_t GetProgress(const uint32_t frameTime)
	{
		return ((uint64_t)(frameTime % PeriodMicros) * UINT16_MAX) / PeriodMicros;
	}

	static constexpr uint8_t ScaleProgress(const uint16_t progress, const uint8_t value)
	{
		return ScaleProgress(progress, (uint16_t)value);
	}

	static constexpr uint16_t ScaleProgress(const uint16_t progress, const uint16_t value)
	{
		return (((uint32_t)progress) * value) / UINT16_MAX;
	}

	static constexpr uint16_t ScaleProgress(const uint16_t progress, const uint32_t value)
	{
		return (((uint64_t)progress) * value) / UINT16_MAX;
	}

	static const uint16_t TriangleResponse(const uint16_t progress)
	{
		if (progress < ((uint16_t)INT16_MAX + 1))
		{
			return progress * 2;
		}
		else
		{
			return UINT16_MAX - (progress * 2);
		}
	}
};
#endif