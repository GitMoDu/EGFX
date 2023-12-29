// DemoCyclerTask.h

#ifndef _DEMO_CYCLER_TASK_h
#define _DEMO_CYCLER_TASK_h

#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>

#include <ArduinoGraphicsCore.h>

template<const uint32_t DemoStepDurationMillis = 10000>
class DemoCyclerTask : private Task
{
private:
	IFrameEngine* Engine;

private:
	IFrameDraw* Demo0;
	IFrameDraw* Demo1;
	IFrameDraw* Demo2;
	IFrameDraw* Demo3;
	IFrameDraw* Demo4;
	uint8_t Index = 0;

public:
	DemoCyclerTask(Scheduler* scheduler,
		IFrameEngine* engine,
		IFrameDraw* demo0,
		IFrameDraw* demo1 = nullptr,
		IFrameDraw* demo2 = nullptr,
		IFrameDraw* demo3 = nullptr,
		IFrameDraw* demo4 = nullptr)
		: Task(DemoStepDurationMillis, TASK_FOREVER, scheduler, true)
		, Engine(engine)
		, Demo0(demo0)
		, Demo1(demo1)
		, Demo2(demo2)
		, Demo3(demo3)
		, Demo4(demo4)
	{}

	bool Callback()
	{
		switch (Index)
		{
		case 0:
			Engine->SetDrawer(Demo0);
			Index++;
			break;
		case 1:
			if (Demo1 != nullptr)
			{
				Engine->SetDrawer(Demo1);
				Index++;
			}
			else
			{
				Index = 0;
			}
			break;
		case 2:
			if (Demo2 != nullptr)
			{
				Engine->SetDrawer(Demo2);
				Index++;
			}
			else
			{
				Index = 0;
			}
			break;
		case 3:
			if (Demo3 != nullptr)
			{
				Engine->SetDrawer(Demo3);
				Index++;
			}
			else
			{
				Index = 0;
			}
			break;
		case 4:
			if (Demo4 != nullptr)
			{
				Engine->SetDrawer(Demo4);
				Index++;
			}
			else
			{
				Index = 0;
			}
			break;
		default:
			Index = 0;
			break;
		}

		return true;
	}
};
#endif