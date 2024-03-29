// ElementDrawer.h

#ifndef _ELEMENT_DRAWER_h
#define _ELEMENT_DRAWER_h

#include "../Model/IFrameDraw.h"
#include "../Model/IFrameBuffer.h"

#include "DrawState.h"
#include "ProgressScaler.h"

class ElementDrawer : public virtual IFrameDraw
{
private:
	DrawState State{};

protected:
	IFrameBuffer* Frame;

private:
	uint8_t ElementsCount;

public:
	/// <summary>
	/// Recurrent call for drawing elements, indexed to frameDrawState.ElementIndex.
	/// </summary>
	/// <param name="drawState">Element-indexed, element-draw state.</param>
	virtual void DrawCall(DrawState* drawState) { }

public:
	ElementDrawer(IFrameBuffer* frame, const uint8_t elementsCount)
		: IFrameDraw()
		, Frame(frame)
		, ElementsCount(elementsCount)
	{}

	const uint8_t GetElementsCount()
	{
		return ElementsCount;
	}

	void SetElementsCount(const uint8_t count)
	{
		ElementsCount = count;
		if (State.ElementIndex >= ElementsCount)
		{
			State.ElementIndex = ElementsCount;
		}
	}

	virtual const bool DrawToFrame(const uint32_t frameTime, const uint16_t frameCounter) final
	{
		State.FrameTime = frameTime;
		State.FrameCounter = frameCounter;

		DrawCall(&State);

		State.ElementIndex++;
		if (State.ElementIndex >= ElementsCount)
		{
			State.ElementIndex = 0;

			return true;
		}

		return false;
	}

protected:
	const uint8_t GetShortestDimension() const
	{
		if (Frame->GetHeight() >= Frame->GetWidth())
		{
			return Frame->GetWidth();
		}
		else
		{
			return Frame->GetHeight();
		}
	}
};
#endif