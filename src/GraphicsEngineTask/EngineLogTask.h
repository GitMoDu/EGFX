// EngineLogTask.h

#ifndef _ENGINE_LOG_TASK_h
#define _ENGINE_LOG_TASK_h

#define _TASK_OO_CALLBACKS
#include <TSchedulerDeclarations.hpp>

#include <ArduinoGraphicsCore.h>

template<const uint32_t LogPeriodMillis = 1000>
class EngineLogTask : private TS::Task
{
private:
	IFrameEngine* Engine;

	IFrameEngine::EngineStatusStruct EngineStatus{};

public:
	EngineLogTask(TS::Scheduler* scheduler,
		IFrameEngine* engine)
		: TS::Task(LogPeriodMillis, TASK_FOREVER, scheduler, true)
		, Engine(engine)
	{}

public:
	bool Callback() final
	{
		if (Engine != nullptr)
		{
			Engine->GetEngineStatus(EngineStatus);

			if (EngineStatus.RenderDuration > 0)
			{
				const uint8_t clearPercent = ((uint16_t)EngineStatus.GetClearWeight() * 100) / UINT8_MAX;
				const uint8_t renderPercent = ((uint16_t)EngineStatus.GetRenderWeight() * 100) / UINT8_MAX;
				const uint8_t idlePercent = ((uint16_t)EngineStatus.GetIdleWeight() * 100) / UINT8_MAX;
				const uint8_t renderLoadPercent = ((uint16_t)EngineStatus.GetRenderLoad() * 100) / UINT8_MAX;
				const uint8_t pushPercent = ((uint16_t)EngineStatus.GetPushWeight() * 100) / UINT8_MAX;


				const uint32_t fpMs = 1000000000 / EngineStatus.FrameDuration;
				const uint16_t fps = fpMs / 1000;
				const uint8_t fpsRemainder = (fpMs % 1000) / 10;

				Serial.print(F("Display "));
				Serial.print(fps);
				Serial.print('.');
				if (fpsRemainder < 10)
				{
					Serial.print(0);
				}
				Serial.print(fpsRemainder);
				Serial.print(F(" FPS ("));
				Serial.print(renderLoadPercent);
				Serial.println(F("% load)"));

				Serial.print('\t');
				Serial.print(F("DrawCalls "));
				Serial.println(EngineStatus.DrawCallCount);

				Serial.print('\t');
				Serial.print(F("Clear "));
				Serial.print(EngineStatus.ClearDuration);
				Serial.print(F(" us ("));
				Serial.print(clearPercent);
				Serial.println(F("%)"));

				Serial.print('\t');
				Serial.print(F("Render "));
				Serial.print(EngineStatus.RenderDuration);
				Serial.print(F(" us ("));
				Serial.print(renderPercent);
				Serial.println(F("%)"));

				Serial.print('\t');
				Serial.print(F("Push "));
				Serial.print(EngineStatus.PushDuration);
				Serial.print(F(" us ("));
				Serial.print(pushPercent);
				Serial.println(F("%)"));

				Serial.print('\t');
				Serial.print(F("Idle "));
				Serial.print(EngineStatus.GetIdleDuration());
				Serial.print(F(" us ("));
				Serial.print(idlePercent);
				Serial.println(F("%)"));


				Serial.print('\t');
				Serial.print(F("LongestRenderCall "));
				Serial.print(EngineStatus.LongestRenderCall);
				Serial.println(F(" us"));
				Serial.print('\t');
				Serial.print(F("LongestPushCall "));
				Serial.print(EngineStatus.LongestPushCall);
				Serial.println(F(" us"));
				Serial.println();
			}
		}

		return true;
	}
};
#endif