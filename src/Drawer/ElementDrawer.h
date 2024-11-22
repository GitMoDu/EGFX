// ElementDrawer.h

#ifndef _ELEMENT_DRAWER_h
#define _ELEMENT_DRAWER_h

#include "../Model/IFrameDraw.h"
#include "../Model/IFrameBuffer.h"

#include "ProgressScaler.h"

class ElementDrawer : public virtual IFrameDraw
{
private:
	uint8_t ElementsCount;
	uint8_t OriginalCount;

private:
	uint8_t ElementIndex = 0;

protected:
	/// <summary>
	/// Recurrent call for indexed drawing elements.
	/// </summary>
	/// <param name="frameTime"></param>
	/// <param name="frameCounter"></param>
	/// <param name="elementIndex"></param>
	virtual void DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter, const uint8_t elementIndex) {}

public:
	ElementDrawer(const uint8_t elementsCount)
		: IFrameDraw()
		, ElementsCount(elementsCount)
		, OriginalCount(elementsCount)
	{
	}

	const uint8_t GetElementsCount() const
	{
		return ElementsCount;
	}

	void Disable()
	{
		ElementsCount = 0;
		ElementIndex = 0;
	}

	void Enable()
	{
		if (ElementsCount != OriginalCount)
		{
			ElementIndex = 0;
			ElementsCount = OriginalCount;
		}
	}

	void SetElementsCount(const uint8_t count)
	{
		ElementsCount = count;
		OriginalCount = ElementsCount;
		if (ElementIndex >= ElementsCount)
		{
			ElementIndex = ElementsCount;
		}
	}

	virtual const bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
	{
		DrawCall(frame, frameTime, frameCounter, ElementIndex);

		ElementIndex++;
		if (ElementIndex >= ElementsCount)
		{
			ElementIndex = 0;

			return true;
		}

		return false;
	}
};
#endif