// DemoCyclerTask.h

#ifndef _DEMO_CYCLER_TASK_h
#define _DEMO_CYCLER_TASK_h

#define _TASK_OO_CALLBACKS
#include <TSchedulerDeclarations.hpp>

#include <ArduinoGraphicsCore.h>

using namespace Egfx;

/// <summary>
/// Add up to 10 different drawers to automatically rotate every DemoStepDurationMillis.
/// </summary>
/// <typeparam name="DemoStepDurationMillis">Demo rotation period, in microseconds.</typeparam>
template<const uint32_t DemoStepDurationMillis = 10000>
class DemoCyclerTask : private TS::Task
{
public:
	static const uint8_t MaxDemoCount = 10;

private:
	IFrameEngine* Engine;


private:
	IFrameDraw* Demos[MaxDemoCount];
	uint8_t Index = 0;

public:
	DemoCyclerTask(TS::Scheduler* scheduler,
		IFrameEngine* engine,
		IFrameDraw* demo0,
		IFrameDraw* demo1 = nullptr,
		IFrameDraw* demo2 = nullptr,
		IFrameDraw* demo3 = nullptr,
		IFrameDraw* demo4 = nullptr,
		IFrameDraw* demo5 = nullptr,
		IFrameDraw* demo6 = nullptr,
		IFrameDraw* demo7 = nullptr,
		IFrameDraw* demo8 = nullptr,
		IFrameDraw* demo9 = nullptr
	)
		: TS::Task(0, TASK_FOREVER, scheduler, true)
		, Engine(engine)
		, Demos{ demo0, demo1, demo2, demo3, demo4, demo5, demo6, demo7, demo8, demo9 }
	{
	}

	bool Callback() final
	{
#if defined(DEBUG)
		Serial.print(F("Demo: "));
		Serial.println(Index);
#endif

		Engine->SetDrawer(Demos[Index]);

		IFrameDraw* demo = nullptr;
		while (demo == nullptr)
		{
			Index++;
			Index = Index % MaxDemoCount;
			demo = Demos[Index];
		}

		TS::Task::delay(DemoStepDurationMillis);

		return true;
	}
};
#endif