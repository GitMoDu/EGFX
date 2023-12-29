// IFrameBuffer.h

#ifndef _I_FRAME_BUFFER_h
#define _I_FRAME_BUFFER_h

#include "IFrame.h"

class IFrameBuffer : public virtual IFrame
{
public:
	IFrameBuffer() : IFrame() {}

public:
	/// <summary>
	/// Exposes the frame buffer for pushing to IFrameSink.
	/// Can be nullptr for DirectDraw.
	/// </summary>
	/// <returns></returns>
	virtual const uint8_t* GetFrameBuffer() { return nullptr; }

	/// <summary>
	/// Clear the frame buffer.
	/// Only called once, if it's not DirectDraw.
	/// </summary>
	virtual void ClearFrame() {}

	/// <summary>
	/// Informs engine and renderer of the underlying rendering.
	/// Engine will not clear the frame before render and Renderer items must clear their own background.
	/// </summary>
	/// <returns></returns>
	virtual const bool IsDirectDraw() { return false; }
};
#endif