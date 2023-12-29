// GraphicsEngineTask.h

#ifndef _GRAPHICS_ENGINE_TASK_h
#define _GRAPHICS_ENGINE_TASK_h

#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>

#include <ArduinoGraphicsCore.h>

/// <summary>
/// Task based Graphics Engine.
/// Can work with both ScreenDriver and FrameBuffer sources/sinks.
/// Single Image Buffer updating and rendering.
/// TODO: Measure some statistics only in debug mode.
/// </summary>
class GraphicsEngineTask : private Task, public virtual IFrameEngine
{
private:
	enum class EngineStateEnum : uint8_t
	{
		ClearFrame,
		WaitForClear,
		Render,
		WaitForPush,
		PushBuffer,
		VSync,
		WaitForDisplay
	};

	static constexpr uint32_t TimeoutPeriod = 1000000;

private:
	IFramePrimitives* Source;
	IFrameSink* Sink;
	IFrameDraw* Drawer;

private:
	EngineStatusStruct EngineStatus{};
	EngineStatusStruct EngineStatusRunTime{};

private:
	const uint32_t TargetPeriod;

private:
	uint32_t FrameTime = 0;
	uint32_t RenderAccumulated = 0;
	EngineStateEnum EngineState = EngineStateEnum::WaitForDisplay;

public:
	GraphicsEngineTask(Scheduler* scheduler,
		IFramePrimitives* source,
		IFrameSink* sink,
		const uint32_t targetPeriodMicros = 16666,
		IFrameDraw* drawer = nullptr)
		: IFrameEngine()
		, Task(0, TASK_FOREVER, scheduler, false)
		, Source(source)
		, Sink(sink)
		, Drawer(drawer)
		, TargetPeriod(targetPeriodMicros)
	{}

public: // IFrameEngine implementation.
	virtual void SetDrawer(IFrameDraw* drawer) final
	{
		if (drawer != nullptr)
		{
			Drawer = drawer;

			if (Task::isEnabled())
			{
				EngineState = EngineStateEnum::ClearFrame;
				FrameTime = micros();
			}
		}
	}

	virtual const bool Start() final
	{
		if (Source != nullptr
			&& Sink != nullptr
			//&& RenderSource != nullptr
			&& Source->GetWidth() == Sink->GetWidth()
			&& Source->GetHeight() == Sink->GetHeight()
			&& Sink->GetWidth() > 0
			&& Sink->GetWidth() < UINT8_MAX
			&& Sink->GetHeight() > 0
			&& Sink->GetHeight() < UINT8_MAX)
		{
			if (Sink->Start())
			{
				Task::enable();
				Task::forceNextIteration();

				EngineState = EngineStateEnum::ClearFrame;

				EngineStatusRunTime.ClearDuration = 0;
				EngineStatusRunTime.RenderDuration = 0;
				EngineStatusRunTime.PushDuration = 0;
				EngineStatusRunTime.LongestDrawCall = 0;
				EngineStatusRunTime.FrameDuration = 0;
				EngineStatusRunTime.FrameCounter = 0;
				FrameTime = micros();

				return true;
			}
#ifdef DEBUG
			else
			{
				Serial.println(F("Sink Start failed."));
			}
#endif
		}
#ifdef DEBUG
		else
		{
			Serial.println(F("Sink Validation failed."));

			if (Source != nullptr)
			{
				Serial.print(F("Source: "));
				Serial.print(Source->GetWidth());
				Serial.print('x');
				Serial.println(Source->GetHeight());
			}

			if (Sink != nullptr)
			{
				Serial.print(F("Sink: "));
				Serial.print(Sink->GetWidth());
				Serial.print('x');
				Serial.println(Sink->GetHeight());
			}

			if (Source == nullptr)
				Serial.println(F("IFramePrimitives == nullptr"));
			if (Sink == nullptr)
				Serial.println(F("IFrameSink == nullptr"));
			if ((Source->GetWidth() != Sink->GetWidth()) || (Source->GetHeight() != Sink->GetHeight()))
				Serial.println(F("IFrame dimensions don't match."));
			if ((Sink->GetWidth() == 0)
				|| (Sink->GetWidth() == UINT8_MAX)
				|| (Sink->GetHeight() == 0)
				|| (Sink->GetHeight() == UINT8_MAX))
				Serial.println(F("Invalid dimensions."));
		}
#endif
		Stop();

		Serial.println(F("Sink Start stopped."));

		return false;
	}

	virtual void Stop() final
	{
		if (Sink != nullptr)
		{
			Sink->Stop();
		}
		Task::disable();
	}

public:
	virtual void GetEngineStatus(EngineStatusStruct& status) final
	{
		EngineStatus.CopyTo(status);
	}

public:
	bool Callback() final
	{
		const uint32_t timestamp = micros();

		switch (EngineState)
		{
		case EngineStateEnum::ClearFrame:
			Source->ClearFrame();
			EngineStatusRunTime.ClearDuration = micros() - timestamp;
			EngineState = EngineStateEnum::WaitForClear;
			RenderAccumulated = 0;
			EngineStatusRunTime.LongestDrawCall = 0;
			Task::forceNextIteration();
			break;
		case EngineStateEnum::WaitForClear:
			if (Sink->CanPushFrame())
			{
				EngineState = EngineStateEnum::Render;
			}
			Task::forceNextIteration();
			break;
		case EngineStateEnum::Render:
			if (Render(timestamp))
			{
				EngineState = EngineStateEnum::WaitForPush;
				Task::forceNextIteration();
			}
#if defined(DEBUG)
			else if (timestamp - FrameTime >= TimeoutPeriod)
			{
				Serial.println(F("Frame render is taking too long."));
			}
#endif
			break;
		case EngineStateEnum::WaitForPush:
			if (Sink->CanPushFrame())
			{
				EngineState = EngineStateEnum::PushBuffer;
			}
			Task::forceNextIteration();
			break;
		case EngineStateEnum::PushBuffer:
			if (Sink->CanPushFrame())
			{
				if (Sink->PushFrame(Source->GetFrameBuffer()))
				{
					EngineStatusRunTime.PushDuration = micros() - timestamp;
					EngineState = EngineStateEnum::VSync;
					EngineStatusRunTime.FrameCounter++;
					Task::forceNextIteration();
				}
				else
				{
					//TODO: Log Error: Error while pushing frame.
					Stop();
				}
			}
#if defined(DEBUG)
			else if (timestamp - FrameTime >= TimeoutPeriod)
			{
				Serial.println(F("Buffer push is taking too long"));
				Stop();
			}
#endif
			break;
		case EngineStateEnum::VSync:
			if (timestamp - FrameTime >= TargetPeriod)
			{
				EngineStatusRunTime.FrameDuration = timestamp - FrameTime;
				EngineStatusRunTime.RenderDuration = RenderAccumulated;
				FrameTime = timestamp;
				EngineState = EngineStateEnum::WaitForDisplay;
				Task::forceNextIteration();

#if defined(DEBUG)
				if (EngineStatusRunTime.FrameDuration >= TargetPeriod + 100)
				{
					Serial.print(F("Full Frame is taking too long: "));
					Serial.print(EngineStatusRunTime.FrameDuration);
					Serial.println(F(" us"));
				}
#endif
			}
			else if (TargetPeriod - (timestamp - FrameTime) > 1000)
			{
				// More than 1ms until push, sleep until we're at the last millisecond.
				Task::delay(((TargetPeriod - (timestamp - FrameTime)) / 1000) - 1);
			}
			else
			{
				Task::forceNextIteration();
			}
			break;
		case EngineStateEnum::WaitForDisplay:
			if (Source->IsDirectDraw())
			{
				EngineState = EngineStateEnum::WaitForClear;
				EngineStatusRunTime.CopyTo(EngineStatus);
				RenderAccumulated = 0;
				EngineStatusRunTime.LongestDrawCall = 0;
			}
			else if (Sink->CanPushFrame())
			{
				EngineState = EngineStateEnum::ClearFrame;
				EngineStatusRunTime.CopyTo(EngineStatus);
			}
			Task::forceNextIteration();
			break;
		default:
			Task::disable();
			return false;
			break;
		}

		return true;
	}

private:
	const bool Render(const uint32_t timestamp)
	{
		if (Drawer != nullptr)
		{
			const bool finished = Drawer->DrawToFrame(FrameTime, EngineStatusRunTime.FrameCounter);
			const uint32_t duration = micros() - timestamp;

			if (duration > EngineStatusRunTime.LongestDrawCall)
			{
				EngineStatusRunTime.LongestDrawCall = duration;
			}
			RenderAccumulated += duration;
			Task::forceNextIteration();

			return finished;
		}
		else
		{
			return true;
		}
	}
};
#endif