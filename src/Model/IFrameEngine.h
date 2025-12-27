#ifndef _EGFX_I_FRAME_ENGINE_h
#define _EGFX_I_FRAME_ENGINE_h

#include "IFrameDraw.h"

namespace Egfx
{
	struct DisplayTimingsStruct
	{
		uint32_t TargetDuration = 0;
		uint32_t FrameTimestamp = 0;
		uint32_t FrameDuration = 0;
		uint16_t FrameCounter = 0;
	};

	// Display timing structure.
	// Only available when EGFX_PERFORMANCE_LOG is defined.
	struct DisplayPerformanceStruct : DisplayTimingsStruct
	{
		uint32_t ClearDuration = 0;
		uint32_t RenderDuration = 0;
		uint32_t SyncDuration = 0;
		uint32_t PushDuration = 0;

		uint32_t GetIdleDuration() const
		{
			return SyncDuration;
		}

		uint32_t GetBusyDuration() const
		{
			return ClearDuration + RenderDuration + PushDuration;
		}

		uint8_t GetRenderWeight() const
		{
			if (FrameDuration > 0
				&& (RenderDuration < FrameDuration))
			{
				return (RenderDuration * UINT8_MAX) / FrameDuration;
			}
			else
			{
				return UINT8_MAX;
			}
		}

		uint8_t GetPushWeight() const
		{
			if (FrameDuration > 0
				&& (PushDuration < FrameDuration))
			{
				return (PushDuration * UINT8_MAX) / FrameDuration;
			}
			else
			{
				return UINT8_MAX;
			}
		}

		uint8_t GetClearWeight() const
		{
			if (FrameDuration > 0
				&& (ClearDuration < FrameDuration))
			{
				return (ClearDuration * UINT8_MAX) / FrameDuration;
			}
			else
			{
				return UINT8_MAX;
			}
		}

		uint8_t GetIdleWeight() const
		{
			if (SyncDuration < TargetDuration && TargetDuration > 0)
			{
				return ((SyncDuration * UINT8_MAX) / TargetDuration);
			}
			else
			{
				return UINT8_MAX;
			}
		}

		uint8_t GetFrameLoad() const
		{
			return UINT8_MAX - GetIdleWeight();
		}
	};

	// Detailed display timing structure.
	// Only available when EGFX_PERFORMANCE_LOG and EGFX_PERFORMANCE_LOG_DETAIL is defined.
	struct DisplayPerformanceDetailStruct : DisplayPerformanceStruct
	{
		uint32_t ClearStepMaxDuration = 0;
		uint32_t RenderCallMaxDuration = 0;
		uint32_t PushStepMaxDuration = 0;
	};

	class IFrameEngine
	{
	public:
		virtual bool Start() = 0;
		virtual void Stop() = 0;
		virtual void SetDrawer(IFrameDraw* drawer) = 0;
		virtual void SetBrightness(const uint8_t brightness) = 0;

	public:
		// Simple display timings retrieval.
		virtual void GetDisplayTimings(DisplayTimingsStruct& timings) const = 0;

#if defined(EGFX_PERFORMANCE_LOG)
		// Detailed display performance retrieval.
		virtual void GetDisplayPerformance(DisplayPerformanceStruct& timings) const = 0;
#if defined(EGFX_PERFORMANCE_LOG_DETAIL)
		// Full display performance detail retrieval.
		virtual void GetDisplayPerformanceDetail(DisplayPerformanceDetailStruct& timings) const = 0;
#endif
#endif
	};
}
#endif