// DemoCyclerTask.h

#ifndef _DEMO_CYCLER_TASK_h
#define _DEMO_CYCLER_TASK_h

#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>

#include <ArduinoGraphicsCore.h>
#include <ArduinoGraphicsDrawers.h>

/// <summary>
/// Add up to 10 different drawers to automatically rotate every DemoStepDurationMillis.
/// </summary>
/// <typeparam name="DemoStepDurationMillis">Demo rotation period, in microseconds.</typeparam>
template<const uint32_t DemoStepDurationMillis = 10000>
class DemoCyclerTask : private Task
{
public:
	static const uint8_t MaxDemoCount = 10;

private:
	IFrameEngine* Engine;

	MultiDrawerWrapper<2> DrawerWrapper{};

private:
	ElementDrawer* FpsOverlay;
	ElementDrawer* Demos[MaxDemoCount];
	uint8_t Index = 0;

public:
	DemoCyclerTask(TS::Scheduler* scheduler,
		IFrameEngine* engine,
		ElementDrawer* fpsOverlay,
		ElementDrawer* demo0 = nullptr,
		ElementDrawer* demo1 = nullptr,
		ElementDrawer* demo2 = nullptr,
		ElementDrawer* demo3 = nullptr,
		ElementDrawer* demo4 = nullptr,
		ElementDrawer* demo5 = nullptr,
		ElementDrawer* demo6 = nullptr,
		ElementDrawer* demo7 = nullptr,
		ElementDrawer* demo8 = nullptr,
		ElementDrawer* demo9 = nullptr
	)
		: Task(0, TASK_FOREVER, scheduler, true)
		, Engine(engine)
		, FpsOverlay(fpsOverlay)
		, Demos{ demo0, demo1, demo2, demo3, demo4, demo5, demo6, demo7, demo8, demo9 }
	{
		Engine->SetDrawer(&DrawerWrapper);
		DrawerWrapper.ClearDrawers();
		DrawerWrapper.AddDrawer(demo0);
		DrawerWrapper.AddDrawer(FpsOverlay);
	}

	bool Callback() final
	{
#if defined(DEBUG)
		Serial.print(F("Demo: "));
		Serial.println(Index);
#endif

		DrawerWrapper.ClearDrawers();
		DrawerWrapper.AddDrawer(Demos[Index]);
		DrawerWrapper.AddDrawer(FpsOverlay);

		IFrameDraw* demo = nullptr;
		while (demo == nullptr)
		{
			Index++;
			Index = Index % MaxDemoCount;
			demo = Demos[Index];
		}

		Task::delay(DemoStepDurationMillis);

		return true;
	}
};
#endif