// DrawState.h

#ifndef _DRAW_STATE_h
#define _DRAW_STATE_h

#include "ProgressScaler.h"

/// <summary>
/// Element-indexed, frame-render state.
/// </summary>
struct DrawState
{
	/// <summary>
	/// Current frame time in microseconds.
	/// </summary>
	uint32_t FrameTime = 0;

	/// <summary>
	/// Current frame counter.
	/// </summary>
	uint16_t FrameCounter = 0;

	/// <summary>
	/// Current element being rendered, out of ElementsCount.
	/// For every false loop of DrawToFrame, elementIndex will increase by 1.
	/// Resets to 0 when a new frame starts.
	/// </summary>
	uint8_t ElementIndex = 0;

	/// <summary>
	/// ProgressScaler::GetProgress using current FrameTime.
	/// </summary>
	/// <typeparam name="PeriodMicros"></typeparam>
	/// <returns></returns>
	template<const uint32_t PeriodMicros>
	const uint16_t GetProgress()
	{
		return ProgressScaler::GetProgress<PeriodMicros>(FrameTime);
	}

	/// <summary>
	/// ProgressScaler::GetProgress using current FrameTime.
	/// </summary>
	/// <param name="periodMicros"></param>
	/// <returns></returns>
	const uint16_t GetProgress(const uint32_t periodMicros)
	{
		return ProgressScaler::GetProgress(periodMicros, FrameTime);
	}
};
#endif