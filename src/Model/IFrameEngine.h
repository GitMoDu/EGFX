#ifndef _EGFX_I_FRAME_ENGINE_h
#define _EGFX_I_FRAME_ENGINE_h

#include "IFrameDraw.h"

namespace Egfx
{
	class IFrameEngine
	{
	public:
		struct EngineStatusStruct
		{
			uint32_t TargetDuration = 0;
			uint32_t FrameDuration = 0;
			uint32_t ClearDuration = 0;
			uint32_t RenderDuration = 0;
			uint32_t PushDuration = 0;
			uint32_t LongestRenderCall = 0;
			uint32_t LongestPushCall = 0;
			uint32_t LongestClearCall = 0;
			uint16_t DrawCallCount = 0;

			const uint32_t GetIdleDuration()  const
			{
				if (FrameDuration > 0)
				{
					const uint32_t busyDuration = GetBusyDuration();

					if (busyDuration < FrameDuration)
					{
						return FrameDuration - busyDuration;
					}
				}

				return 0;
			}

			const uint8_t GetIdleWeight() const
			{
				const uint32_t idleDuration = GetIdleDuration();

				return (idleDuration * UINT8_MAX) / FrameDuration;
			}

			const uint8_t GetFrameLoad() const
			{
				return UINT8_MAX - GetIdleWeight();
			}

			const uint32_t GetBusyDuration() const
			{
				return ClearDuration + RenderDuration + PushDuration;
			}

			const uint8_t GetRenderLoad() const
			{
				const uint32_t otherDuration = ClearDuration + PushDuration;

				if (TargetDuration > 0
					&& otherDuration < TargetDuration)
				{
					const uint32_t renderWindow = TargetDuration - otherDuration;

					if (RenderDuration < renderWindow)
					{
						return (RenderDuration * UINT8_MAX) / renderWindow;
					}
					else
					{
						return UINT8_MAX;
					}
				}
				else
				{
					return UINT8_MAX;
				}
			}

			const uint8_t GetRenderWeight() const
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

			const uint8_t GetPushWeight() const
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

			const uint8_t GetClearWeight() const
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

			void Clear()
			{
				TargetDuration = 0;
				FrameDuration = 0;
				ClearDuration = 0;
				RenderDuration = 0;
				LongestRenderCall = 0;
				PushDuration = 0;
				LongestPushCall = 0;
				LongestClearCall = 0;
				DrawCallCount = 0;
			}

			void CopyTo(EngineStatusStruct& target) const
			{
				target.TargetDuration = TargetDuration;
				target.FrameDuration = FrameDuration;
				target.ClearDuration = ClearDuration;
				target.RenderDuration = RenderDuration;
				target.LongestRenderCall = LongestRenderCall;
				target.PushDuration = PushDuration;
				target.LongestPushCall = LongestPushCall;
				target.LongestClearCall = LongestClearCall;
				target.DrawCallCount = DrawCallCount;
			}
		};

	public:
		virtual const bool Start() { return false; }
		virtual void Stop() {}
		virtual void SetDrawer(IFrameDraw* drawer) {}
		virtual void SetInverted(const bool inverted) {}
		virtual void SetBrightness(const uint8_t brightness) {}

	public:
		virtual const uint32_t GetFrameDuration() const { return 0; }
		virtual void GetEngineStatus(EngineStatusStruct& status) {}
	};
}
#endif