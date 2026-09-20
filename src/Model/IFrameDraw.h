#ifndef _INTEGERGLASS_I_FRAME_DRAW_h
#define _INTEGERGLASS_I_FRAME_DRAW_h

#include "IFrameBuffer.h"

namespace IntegerGlass
{
	struct IFrameDraw
	{
		/// <summary>
		/// Virtual defaulted destructor for the IFrameDraw interface. Ensures derived classes are destroyed correctly.
		/// </summary>
		virtual ~IFrameDraw() = default;

		/// <summary>
		/// Recurrent call for drawing. At least one draw call per frame.
		/// </summary>
		/// <param name="frameTime">Current frame time in microseconds.</param>
		/// <param name="frameCounter">Current frame being rendered.</param>
		/// <returns>True when all drawing is done for this frame.</returns>
		virtual bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) = 0;

	};
}
#endif