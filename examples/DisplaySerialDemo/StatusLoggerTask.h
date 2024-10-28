// StatusLoggerTask.h

#ifndef _STATUSLOGGERTASK_h
#define _STATUSLOGGERTASK_h

#define _TASK_OO_CALLBACKS
#include <TSchedulerDeclarations.hpp>

#include <ArduinoGraphicsCore.h>
#include <ArduinoGraphicsDrawers.h>

template<const uint32_t LogPeriodMillis = 1000>
class StatusLoggerTask : private TS::Task
{
private:
	IFrameEngine* Engine;
	Print* SerialInstance;

	IFrameEngine::EngineStatusStruct EngineStatus{};

public:
	StatusLoggerTask(TS::Scheduler* scheduler,
		IFrameEngine* engine,
		Print* serial)
		: TS::Task(LogPeriodMillis, TASK_FOREVER, scheduler, true)
		, Engine(engine)
		, SerialInstance(serial)
	{}

public:
	bool Callback() final
	{
		if (Engine != nullptr)
		{
			Engine->GetEngineStatus(EngineStatus);

			if (EngineStatus.RenderDuration > 0)
			{
				const uint8_t renderLoadPercent = ((uint16_t)EngineStatus.GetRenderLoad() * 100) / UINT8_MAX;

				const uint32_t fpMs = 1000000000 / EngineStatus.FrameDuration;
				const uint16_t fps = fpMs / 1000;
				const uint8_t fpsRemainder = (fpMs % 1000) / 10;

				SerialInstance->print(F("Display "));
				SerialInstance->print(fps);
				SerialInstance->print('.');
				if (fpsRemainder < 10)
				{
					SerialInstance->print(0);
				}
				SerialInstance->print(fpsRemainder);
				SerialInstance->print(F(" FPS"));

				SerialInstance->println();

				SerialInstance->print(F("\tDrawCalls ["));
				SerialInstance->print(EngineStatus.DrawCallCount);
				SerialInstance->println(F("]"));

				SerialInstance->print(F("\tClear "));
				SerialInstance->print(EngineStatus.ClearDuration);
				SerialInstance->println(F(" us"));

				SerialInstance->print(F("\tRender "));
				SerialInstance->print(EngineStatus.RenderDuration);
				SerialInstance->println(F(" us"));

				SerialInstance->print(F("\tPush "));
				SerialInstance->print(EngineStatus.PushDuration);
				SerialInstance->println(F(" us "));

				SerialInstance->print(F("\tIdle "));
				SerialInstance->print(EngineStatus.GetIdleDuration());
				SerialInstance->println(F(" us"));

				SerialInstance->print(F("\tLRender "));
				SerialInstance->print(EngineStatus.LongestRenderCall);
				SerialInstance->println(F(" us"));
				SerialInstance->print(F("\tLPush "));
				SerialInstance->print(EngineStatus.LongestPushCall);
				SerialInstance->println(F(" us"));

				SerialInstance->print("(");
				SerialInstance->print(renderLoadPercent);
				SerialInstance->println("% load)");
			}
		}

		return true;
	}
};
#endif

