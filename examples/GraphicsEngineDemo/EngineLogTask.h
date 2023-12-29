// EngineLogTask.h

#ifndef _ENGINE_LOG_TASK_h
#define _ENGINE_LOG_TASK_h

#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>

#include <ArduinoGraphicsCore.h>

class EngineLogTask : private Task
{
private:
	static constexpr uint32_t LogPeriodMillis = 500;
private:
	IFrameEngine* Engine;

	IFrameEngine::EngineStatusStruct EngineStatus{};

public:
	EngineLogTask(Scheduler* scheduler,
		IFrameEngine* engine)
		: Task(LogPeriodMillis, TASK_FOREVER, scheduler, true)
		, Engine(engine)
	{}

public:
	bool Callback() final
	{
		if (Engine != nullptr)
		{
			Engine->GetEngineStatus(EngineStatus);

			Serial.print(F("Frame "));
			Serial.println(EngineStatus.FrameCounter);

			Serial.print('\t');
			Serial.print(F("FrameDuration "));
			Serial.print(EngineStatus.FrameDuration);
			Serial.println(F(" us"));
			Serial.println();
			Serial.print('\t');
			Serial.print(F("ClearDuration "));
			Serial.print(EngineStatus.ClearDuration);
			Serial.println(F(" us"));		
			Serial.print('\t');
			Serial.print(F("LongestDrawCall "));
			Serial.print(EngineStatus.LongestDrawCall);
			Serial.println(F(" us"));

			Serial.print('\t');
			Serial.print(F("RenderDuration "));
			Serial.print(EngineStatus.RenderDuration);
			Serial.println(F(" us"));

			Serial.print('\t');
			Serial.print(F("PushDuration "));
			Serial.print(EngineStatus.PushDuration);
			Serial.println(F(" us"));
		}

		return true;
	}
};
#endif