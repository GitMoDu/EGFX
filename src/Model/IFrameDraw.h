// IFrameDraw.h

#ifndef _I_FRAME_DRAW_h
#define _I_FRAME_DRAW_h

#include <stdint.h>
#include "IFrameBuffer.h"

class IFrameDraw
{
public:
	/// <summary>
	/// Recurrent call for drawing. At least one draw call per frame.
	/// </summary>
	/// <param name="frameTime">Current frame time in microseconds.</param>
	/// <param name="frameCounter">Current frame being rendered.</param>
	/// <returns>True when all drawing is done for this frame.</returns>
	virtual const bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) { return true; }

	/// <summary>
	/// Flag layer as enabled/disabled to skip rendering.
	/// </summary>
	/// <param name="enabled"></param>
	virtual void SetEnabled(const bool enabled) {}

	/// <summary>
	/// </summary>
	/// <param name="enabled">Is layer enabled/disabled.</param>
	virtual const bool IsEnabled() const { return false; }
};

#endif