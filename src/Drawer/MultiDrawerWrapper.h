// MultiDrawerWrapper.h

#ifndef _MULTI_DRAWER_WRAPPER_h
#define _MULTI_DRAWER_WRAPPER_h

#include "ElementDrawer.h"

/// <summary>
/// Abstracts a list of ElementDrawers as an IFrameDraw.
/// Fires DrawCalls on ElementDrawers with corrected ElementIndex.
/// Drawers can be added and removed at runtime.
/// </summary>
/// <typeparam name="MaxDrawersCount"></typeparam>
template<const uint8_t MaxDrawersCount = 5>
class MultiDrawerWrapper : public virtual IFrameDraw
{
private:
	DrawState State{};
	uint16_t ElementIndex = 0;

private:
	ElementDrawer* Drawers[MaxDrawersCount]{};
	uint8_t DrawersCount = 0;

public:
	MultiDrawerWrapper() : IFrameDraw()
	{}

public:
	void ClearDrawers()
	{
		DrawersCount = 0;
		ElementIndex = 0;
	}

	const uint16_t GetElementsCount()
	{
		uint16_t total = 0;
		for (uint8_t i = 0; i < DrawersCount; i++)
		{
			total += Drawers[i]->GetElementsCount();
		}

		return total;
	}

	const bool AddDrawer(ElementDrawer* drawer)
	{
		if (drawer != nullptr
			&& DrawersCount < MaxDrawersCount)
		{
			Drawers[DrawersCount] = drawer;
			DrawersCount++;

			ElementIndex %= GetElementsCount();

			return true;
		}

		return false;
	}

	virtual const bool DrawToFrame(const uint32_t frameTime, const uint16_t frameCounter) final
	{
		State.FrameTime = frameTime;
		State.FrameCounter = frameCounter;

		uint16_t base = 0;
		uint8_t singleCount = 0;
		for (uint8_t i = 0; i < DrawersCount; i++)
		{
			singleCount = Drawers[i]->GetElementsCount();

			// Find index of the current drawer from global ElementIndex.
			if (ElementIndex < (base + singleCount))
			{
				// Translate ElementIndex from global to Drawer local.
				State.ElementIndex = ElementIndex - base;
				Drawers[i]->DrawCall(&State);
				break;
			}
			else
			{
				base += singleCount;
			}
		}

		ElementIndex++;
		if (ElementIndex >= GetElementsCount())
		{
			ElementIndex = 0;

			return true;
		}

		return false;
	}
};
#endif