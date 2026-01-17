#ifndef _EGFX_DISPLAY_ENGINE_LOG_TASK_h
#define _EGFX_DISPLAY_ENGINE_LOG_TASK_h

#define _TASK_OO_CALLBACKS
#include <TSchedulerDeclarations.hpp>

#include <EgfxCore.h>

namespace Egfx
{
	template<const uint32_t LogPeriodMillis = 1000>
	class PerformanceLogTask : public TS::Task
	{
	private:
		IFrameEngine& Engine;

#if defined(EGFX_PERFORMANCE_LOG)
#if defined(EGFX_PERFORMANCE_LOG_DETAIL)
		DisplayPerformanceDetailStruct DisplayTimings{};
#else
		DisplayPerformanceStruct DisplayTimings{};
#endif
#else
		DisplayTimingsStruct DisplayTimings{};
#endif
		Print& SerialInstance;

	public:
		PerformanceLogTask(TS::Scheduler& scheduler, IFrameEngine& engine, Print& serial = Serial)
			: TS::Task(LogPeriodMillis, TASK_FOREVER, &scheduler, false)
			, Engine(engine)
			, SerialInstance(serial)
		{
		}

		void Start()
		{
			TS::Task::enableDelayed(LogPeriodMillis);
		}

		void Stop()
		{
			TS::Task::disable();
		}

	public:
		bool Callback() final
		{
#if defined(EGFX_PERFORMANCE_LOG)
#if defined(EGFX_PERFORMANCE_LOG_DETAIL)
			Engine.GetDisplayPerformanceDetail(DisplayTimings);
#else
			Engine.GetDisplayPerformance(DisplayTimings);
#endif
#else
			Engine.GetDisplayTimings(DisplayTimings);
#endif
			if (DisplayTimings.FrameDuration > 0)
			{
				{
					const uint32_t fpMs = 1000000000 / DisplayTimings.FrameDuration;
					const uint16_t fps = fpMs / 1000;
					const uint8_t fpsRemainder = (fpMs % 1000) / 10;

					SerialInstance.println();
					SerialInstance.print(F("\nEGFX "));
					SerialInstance.print(fps);
					SerialInstance.print('.');
					if (fpsRemainder < 10)
					{
						SerialInstance.print(0);
					}
					SerialInstance.print(fpsRemainder);
					SerialInstance.print(F(" FPS"));
				}
#if defined(EGFX_PERFORMANCE_LOG)
				{
					const uint8_t frameLoadPercent = ((uint16_t)DisplayTimings.GetFrameLoad() * 100) / UINT8_MAX;
					SerialInstance.println();
					SerialInstance.print('\t');
					SerialInstance.print(F("Frame "));
					SerialInstance.print(DisplayTimings.GetBusyDuration());
					SerialInstance.print(F(" us ("));
					SerialInstance.print(frameLoadPercent);
					SerialInstance.print(F("% load)"));
				}

				{
					const uint8_t clearPercent = ((uint16_t)DisplayTimings.GetClearWeight() * 100) / UINT8_MAX;
					SerialInstance.println();
					SerialInstance.print('\t');
					SerialInstance.print(F("Clear "));
					SerialInstance.print(DisplayTimings.ClearDuration);
					SerialInstance.print(F(" us ("));
					SerialInstance.print(clearPercent);
					SerialInstance.print(F("%)"));
				}

#if defined(EGFX_PERFORMANCE_LOG_DETAIL)
				SerialInstance.println();
				SerialInstance.print('\t');
				SerialInstance.print(F(" - Max Step "));
				SerialInstance.print(DisplayTimings.ClearStepMaxDuration);
				SerialInstance.print(F(" us"));
#endif
				{
					const uint8_t renderPercent = ((uint16_t)DisplayTimings.GetRenderWeight() * 100) / UINT8_MAX;
					SerialInstance.println();
					SerialInstance.print('\t');
					SerialInstance.print(F("Render "));
					SerialInstance.print(DisplayTimings.RenderDuration);
					SerialInstance.print(F(" us ("));
					SerialInstance.print(renderPercent);
					SerialInstance.print(F("%)"));
				}

#if defined(EGFX_PERFORMANCE_LOG_DETAIL)
				SerialInstance.println();
				SerialInstance.print('\t');
				SerialInstance.print(F(" - Max Step "));
				SerialInstance.print(DisplayTimings.RenderCallMaxDuration);
				SerialInstance.print(F(" us"));
#endif
				{
					const uint8_t idlePercent = ((uint16_t)DisplayTimings.GetIdleWeight() * 100) / UINT8_MAX;
					SerialInstance.println();
					SerialInstance.print('\t');
					SerialInstance.print(F("Idle "));
					SerialInstance.print(DisplayTimings.GetIdleDuration());
					SerialInstance.print(F(" us ("));
					SerialInstance.print(idlePercent);
					SerialInstance.print(F("%)"));
				}

				{
					const uint8_t pushPercent = ((uint16_t)DisplayTimings.GetPushWeight() * 100) / UINT8_MAX;
					SerialInstance.println();
					SerialInstance.print('\t');
					SerialInstance.print(F("Push "));
					SerialInstance.print(DisplayTimings.PushDuration);
					SerialInstance.print(F(" us ("));
					SerialInstance.print(pushPercent);
					SerialInstance.print(F("%)"));
				}

#if defined(EGFX_PERFORMANCE_LOG_DETAIL)
				SerialInstance.println();
				SerialInstance.print('\t');
				SerialInstance.print(F(" - Max Step "));
				SerialInstance.print(DisplayTimings.PushStepMaxDuration);
				SerialInstance.print(F(" us"));
#endif
#endif
			}

			return true;
		}
	};
}
#endif