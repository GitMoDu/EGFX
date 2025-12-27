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

	public:
		PerformanceLogTask(TS::Scheduler& scheduler, IFrameEngine& engine)
			: TS::Task(LogPeriodMillis, TASK_FOREVER, &scheduler, false)
			, Engine(engine)
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

					Serial.print(F("EGFX Display "));
					Serial.print(fps);
					Serial.print('.');
					if (fpsRemainder < 10)
					{
						Serial.print(0);
					}
					Serial.print(fpsRemainder);
					Serial.println(F(" FPS"));
				}
#if defined(EGFX_PERFORMANCE_LOG)
				{
					const uint8_t frameLoadPercent = ((uint16_t)DisplayTimings.GetFrameLoad() * 100) / UINT8_MAX;
					Serial.print('\t');
					Serial.print(F("Frame "));
					Serial.print(DisplayTimings.GetBusyDuration());
					Serial.print(F(" us ("));
					Serial.print(frameLoadPercent);
					Serial.println(F("% load)"));
				}

				{
					const uint8_t clearPercent = ((uint16_t)DisplayTimings.GetClearWeight() * 100) / UINT8_MAX;
					Serial.print('\t');
					Serial.print(F("Clear "));
					Serial.print(DisplayTimings.ClearDuration);
					Serial.print(F(" us ("));
					Serial.print(clearPercent);
					Serial.println(F("%)"));
				}

#if defined(EGFX_PERFORMANCE_LOG_DETAIL)
				Serial.print('\t');
				Serial.print(F(" - Max Step "));
				Serial.print(DisplayTimings.ClearStepMaxDuration);
				Serial.println(F(" us"));
#endif
				{
					const uint8_t renderPercent = ((uint16_t)DisplayTimings.GetRenderWeight() * 100) / UINT8_MAX;
					Serial.print('\t');
					Serial.print(F("Render "));
					Serial.print(DisplayTimings.RenderDuration);
					Serial.print(F(" us ("));
					Serial.print(renderPercent);
					Serial.println(F("%)"));
				}

#if defined(EGFX_PERFORMANCE_LOG_DETAIL)
				Serial.print('\t');
				Serial.print(F(" - Max Step "));
				Serial.print(DisplayTimings.RenderCallMaxDuration);
				Serial.println(F(" us"));
#endif
				{
					const uint8_t idlePercent = ((uint16_t)DisplayTimings.GetIdleWeight() * 100) / UINT8_MAX;
					Serial.print('\t');
					Serial.print(F("Idle "));
					Serial.print(DisplayTimings.GetIdleDuration());
					Serial.print(F(" us ("));
					Serial.print(idlePercent);
					Serial.println(F("%)"));
				}

				{
					const uint8_t pushPercent = ((uint16_t)DisplayTimings.GetPushWeight() * 100) / UINT8_MAX;
					Serial.print('\t');
					Serial.print(F("Push "));
					Serial.print(DisplayTimings.PushDuration);
					Serial.print(F(" us ("));
					Serial.print(pushPercent);
					Serial.println(F("%)"));
				}

#if defined(EGFX_PERFORMANCE_LOG_DETAIL)
				Serial.print('\t');
				Serial.print(F(" - Max Step "));
				Serial.print(DisplayTimings.PushStepMaxDuration);
				Serial.println(F(" us"));
#endif
#endif
				Serial.println();
			}

			return true;
		}
	};
}
#endif