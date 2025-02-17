// MultiDrawerWrapper.h

#ifndef _MULTI_DRAWER_WRAPPER_h
#define _MULTI_DRAWER_WRAPPER_h

#include "ElementDrawer.h"

namespace Egfx
{
	/// <summary>
	/// Abstracts a list of IFrameDraw layers as a single IFrameDraw.
	/// Forwards DrawCalls to layers until each is done or frame changed.
	/// Layers can be added at runtime.
	/// Forwards the SetEnabled to each layer by default.
	/// </summary>
	/// <typeparam name="MaxLayerCount"></typeparam>
	template<const uint8_t MaxLayerCount = 5>
	class MultiDrawerWrapper : public virtual IFrameDraw
	{
	private:
		IFrameDraw* Drawers[MaxLayerCount]{};
		uint16_t LastFrame = 0;
		uint8_t LayerIndex = 0;
		uint8_t LayersCount = 0;

	public:
		MultiDrawerWrapper() : IFrameDraw()
		{
		}

		void ClearDrawers()
		{
			LayersCount = 0;
			LayerIndex = 0;
		}

		virtual const bool IsEnabled() const
		{
			for (uint_fast8_t i = 0; i < LayersCount; i++)
			{
				if (Drawers[i] != nullptr
					&& Drawers[i]->IsEnabled())
				{
					return true;
				}
			}

			return false;
		}

		virtual void SetEnabled(const bool enabled)
		{
			for (uint_fast8_t i = 0; i < LayersCount; i++)
			{
				if (Drawers[i] != nullptr)
				{
					Drawers[i]->SetEnabled(enabled);
				}
			}
		}

		const bool AddDrawer(IFrameDraw& drawer)
		{
			return AddDrawer(&drawer);
		}

		const bool AddDrawer(IFrameDraw* drawer)
		{
			if (drawer != nullptr
				&& LayersCount < MaxLayerCount)
			{
				if (DrawerExists(drawer))
				{
					return false;
				}
				else
				{
					Drawers[LayersCount] = drawer;
					LayersCount++;

					LayerIndex = 0;

					return true;
				}
			}

			return false;
		}

		virtual const bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
		{
			if (frameCounter != LastFrame
				&& LayerIndex != 0)
			{
				LastFrame = frameCounter;
				LayerIndex = 0;

				return true;
			}

			if (Drawers[LayerIndex]->DrawCall(frame, frameTime, frameCounter))
			{
				LayerIndex++;
			}

			if (LayerIndex >= LayersCount)
			{
				LastFrame = frameCounter + 1;
				LayerIndex = 0;

				return true;
			}

			return false;
		}

	private:
		const bool DrawerExists(const IFrameDraw* drawer) const
		{
			for (uint_fast8_t i = 0; i < LayersCount; i++)
			{
				if (Drawers[i] != nullptr)
				{
					if (Drawers[i] == drawer)
					{
						return true;
					}
				}
			}

			return false;
		}
	};
}
#endif