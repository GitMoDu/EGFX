#ifndef _GRAPHICS_DUAL_BUFFER_TASK_h
#define _GRAPHICS_DUAL_BUFFER_TASK_h

#define _TASK_OO_CALLBACKS
#include <TSchedulerDeclarations.hpp>

#include "DisplaySyncType.h"
#include "AsyncBufferPushTask.h"

namespace Egfx
{
	template<typename FramebufferType, typename ScreenDriverType>
	class DisplayEngineTask : public IFrameEngine, private TS::Task
	{
	private:
		enum class StateEnum : uint8_t
		{
			WaitForScreenStart,
			Clear,
			ClearEnd,
			Render,
			Sync,
			PushBuffer,
			WaitingForPush,
			FinalizeFrame
		};

	private:
		// Display timing information. Performance objects have a local state and a read-only copy.
#if defined(EGFX_PERFORMANCE_LOG)
		uint32_t MeasureTimestamp = 0;
#if defined(EGFX_PERFORMANCE_LOG_DETAIL)
		uint32_t StepTimestamp = 0;
		DisplayPerformanceDetailStruct FrameTiming{};
		DisplayPerformanceDetailStruct FrameTimingCopy{};
#else
		DisplayPerformanceStruct FrameTiming{};
		DisplayPerformanceStruct FrameTimingCopy{};
#endif
#else 
		DisplayTimingsStruct FrameTiming{};
		DisplayTimingsStruct FrameTimingCopy{};
#endif

	private:
		AsyncBufferPushTask<ScreenDriverType> BufferPusher;

	private:
		FramebufferType& FrameBuffer;
		ScreenDriverType& ScreenDriver;

		IFrameDraw* Drawer = nullptr;

	private:
		uint32_t SyncReference = 0;
		StateEnum State = StateEnum::WaitForScreenStart;
		DisplaySyncType SyncType = DisplaySyncType::Vrr;

	public:
		DisplayEngineTask(TS::Scheduler& scheduler,
			FramebufferType& framebuffer,
			ScreenDriverType& screenDriver,
			const uint32_t targetPeriodMicros = 16666)
			: IFrameEngine()
			, TS::Task(TASK_IMMEDIATE, TASK_FOREVER, &scheduler, false)
			, BufferPusher(scheduler, screenDriver)
			, FrameBuffer(framebuffer)
			, ScreenDriver(screenDriver)
		{
			SetTargetPeriod(targetPeriodMicros);
		}

		void SetTargetPeriod(const uint32_t targetPeriodMicros)
		{
			FrameTiming.TargetDuration = targetPeriodMicros;
		}

		void SetSyncType(const DisplaySyncType syncType)
		{
			SyncType = syncType;
		}

		void SetBufferTaskCallback(void (*taskCallback)(void* parameter))
		{
			BufferPusher.SetBufferTaskCallback(taskCallback);
		}

		void BufferTaskCallback(void* parameter)
		{
			BufferPusher.BufferTaskCallback(parameter);
		}

	public: // IFrameEngine implementation.
		void SetDrawer(IFrameDraw* drawer) final
		{
			if (drawer != nullptr)
			{
				Drawer = drawer;

				if (State == StateEnum::Render)
				{
					State = StateEnum::WaitForScreenStart;
				}
			}
		}

		void SetBrightness(const uint8_t /*brightness*/) final
		{
			//TODO: integrate with ScreenDriver brightness control.
		}

		bool Start() final
		{
			if (FramebufferType::PhysicalWidth == ScreenDriver.GetScreenWidth()
				&& FramebufferType::PhysicalHeight == ScreenDriver.GetScreenHeight()
				&& ScreenDriver.GetScreenWidth() > 0
				&& ScreenDriver.GetScreenHeight() > 0)
			{
				if (ScreenDriver.Start())
				{
					TS::Task::enableDelayed(0);
					TS::Task::forceNextIteration();
					State = StateEnum::WaitForScreenStart;

					return true;
				}
			}
			Stop();

			return false;
		}

		void Stop() final
		{
			ScreenDriver.Stop();
			TS::Task::disable();
		}

		virtual void GetDisplayTimings(DisplayTimingsStruct& timings) const
		{
			timings = FrameTimingCopy;
		}

#if defined(EGFX_PERFORMANCE_LOG)
		virtual void GetDisplayPerformance(DisplayPerformanceStruct& timings) const
		{
			timings = FrameTimingCopy;
		}
#if defined(EGFX_PERFORMANCE_LOG_DETAIL)
		virtual void GetDisplayPerformanceDetail(DisplayPerformanceDetailStruct& timings) const
		{
			timings = FrameTimingCopy;
		}
#endif
#endif

	public:
		bool Callback() final
		{
			switch (State)
			{
			case StateEnum::WaitForScreenStart:
				if (ScreenDriver.CanPushBuffer())
				{
					State = StateEnum::Clear;
					FrameTiming.FrameCounter = UINT16_MAX; // Clear step will increment to 0.
#if defined(EGFX_PERFORMANCE_LOG)
					MeasureTimestamp = micros();
					SyncReference = MeasureTimestamp - FrameTiming.TargetDuration + 1;
#else
					SyncReference = micros() - FrameTiming.TargetDuration + 1;
#endif
				}
				break;
			case StateEnum::Clear:
#if defined(EGFX_PERFORMANCE_LOG) && defined(EGFX_PERFORMANCE_LOG_DETAIL)
				StepTimestamp = micros();
#endif				
				if (FrameBuffer.ClearFrameBuffer())
				{
					State = StateEnum::Render;
					FrameTiming.FrameCounter++;
					FrameTiming.FrameTimestamp = micros();
#if defined(EGFX_PERFORMANCE_LOG)
					FrameTiming.ClearDuration = FrameTiming.FrameTimestamp - MeasureTimestamp;
#endif
				}
				break;
			case StateEnum::Render:
				if (Drawer == nullptr
					|| Drawer->DrawCall(&FrameBuffer, FrameTiming.FrameTimestamp, FrameTiming.FrameCounter))
				{
					State = StateEnum::Sync;
#if defined(EGFX_PERFORMANCE_LOG)
					MeasureTimestamp = micros();
					FrameTiming.RenderDuration = MeasureTimestamp - FrameTiming.FrameTimestamp;
#endif
				}
				break;
			case StateEnum::Sync:
				if (Sync(micros()))
				{
					State = StateEnum::PushBuffer;
#if defined(EGFX_PERFORMANCE_LOG)
					FrameTiming.SyncDuration = micros() - MeasureTimestamp;
#endif
				}
				break;
			case StateEnum::PushBuffer:
				if (BufferPusher.CanPushBuffer())
				{
#if defined(EGFX_PERFORMANCE_LOG)
					MeasureTimestamp = micros();
#endif
					BufferPusher.StartPushBuffer(FrameBuffer.GetFrameBuffer());
					if (FrameBuffer.Flip())
					{
						// Multi-buffering, ready for next frame drawing.
						State = StateEnum::FinalizeFrame;
#if defined(EGFX_PERFORMANCE_LOG)
						// Use last measured parallel push duration.
						FrameTiming.PushDuration = BufferPusher.GetPushDuration();
#endif
					}
					else
					{
						// Wait for single-buffer push to complete.
						State = StateEnum::WaitingForPush;
					}
#if defined(EGFX_PERFORMANCE_LOG)
					MeasureTimestamp = micros();
#endif
				}
				break;
			case StateEnum::WaitingForPush:
				if (BufferPusher.CanPushBuffer())
				{
					// Ready for next frame drawing.
					State = StateEnum::FinalizeFrame;
#if defined(EGFX_PERFORMANCE_LOG)
					FrameTiming.PushDuration = micros() - MeasureTimestamp;
#endif
				}
				break;
			case StateEnum::FinalizeFrame:
				State = StateEnum::Clear;
#if defined(EGFX_PERFORMANCE_LOG)
				// Copy the timings for external read at any time.
				memcpy(&FrameTimingCopy, &FrameTiming, sizeof(FrameTiming));
				// Prepare for next frame measurement.
				MeasureTimestamp = micros();
#else
				// Copy the timings for external read at any time.
				memcpy(&FrameTimingCopy, &FrameTiming, sizeof(DisplayTimingsStruct));
#endif
				break;
			default:
				TS::Task::disable();
				return false;
				break;
			}

			return true;
		}

	private:
		bool Sync(const uint32_t timestamp)
		{
			const uint32_t syncElapsed = timestamp - SyncReference;

			bool synced = false;
			switch (SyncType)
			{
			case DisplaySyncType::NoSync:
				synced = true;
				SyncReference = timestamp;
				break;
			case DisplaySyncType::Vrr:
				if (syncElapsed >= FrameTiming.TargetDuration)
				{
					synced = true;
					// VRR aligns to the next frame time.
					SyncReference = timestamp;
				}
				break;
			case DisplaySyncType::VSync:
				if (syncElapsed >= FrameTiming.TargetDuration)
				{
					synced = true;
					// VSync aligns to the next frame period multiple.
					const uint32_t framesElapsed = syncElapsed / FrameTiming.TargetDuration;
					SyncReference += FrameTiming.TargetDuration * framesElapsed;
				}
				break;
			default:
				break;
			}

			if (synced)
			{
				FrameTiming.FrameDuration = syncElapsed;
				TS::Task::delay(0);
			}
			else
			{
				if (syncElapsed <= FrameTiming.TargetDuration)
				{
					const uint32_t sleepDuration = IntegerSignal::MaxValue<uint32_t>(1, (FrameTiming.TargetDuration - syncElapsed) / 1000) - 1;
					TS::Task::delay(sleepDuration);
				}
				else
				{
					TS::Task::delay(0);
				}
			}

			return synced;
		}
	};
}
#endif