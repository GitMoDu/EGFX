#ifndef _ELEMENT_DRAWER_h
#define _ELEMENT_DRAWER_h

#include "../Model/IFrameDraw.h"
#include "../Model/IFrameBuffer.h"

#include "ProgressScaler.h"

namespace Egfx
{
	class ElementDrawer : public IFrameDraw
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
		virtual void DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter, const uint8_t elementIndex) = 0;

	public:
		ElementDrawer(const uint8_t elementsCount)
			: IFrameDraw()
			, ElementsCount(elementsCount)
			, OriginalCount(elementsCount)
		{
		}

		/// <summary>
		/// Virtual destructor to maintain a proper virtual chain.
		/// </summary>
		virtual ~ElementDrawer() = default;

		uint8_t GetElementsCount() const
		{
			return ElementsCount;
		}

		bool IsEnabled() const
		{
			return OriginalCount > 0 && ElementsCount == OriginalCount;
		}

		void SetEnabled(const bool enabled)
		{
			if (enabled)
			{
				if (ElementsCount != OriginalCount)
				{
					ElementIndex = 0;
					ElementsCount = OriginalCount;
				}
			}
			else
			{
				ElementsCount = 0;
				ElementIndex = 0;
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

		bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
		{
			if (ElementIndex >= ElementsCount)
			{
				ElementIndex = 0;

				return true;
			}

			DrawCall(frame, frameTime, frameCounter, ElementIndex);
			ElementIndex++;

			return false;
		}
	};
}
#endif