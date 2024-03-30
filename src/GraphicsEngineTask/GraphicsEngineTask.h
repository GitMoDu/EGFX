// GraphicsEngineTask.h

#ifndef _GRAPHICS_ENGINE_TASK_h
#define _GRAPHICS_ENGINE_TASK_h

#define _TASK_OO_CALLBACKS
#include <TaskSchedulerDeclarations.h>

#include <ArduinoGraphicsCore.h>

// Enable to log (on Serial/Serial1).
// Logs out-of-sync frames and render timeouts.
// Also does a single log of the duration of a frame-buffer DMA transfer.
//#define GRAPHICS_ENGINE_DEBUG

// Enable Engine measure and GetStatus(). Required for GRAPHICS_ENGINE_DEBUG.
#if defined(GRAPHICS_ENGINE_DEBUG)
#define GRAPHICS_ENGINE_MEASURE
#endif

/// <summary>
/// Task based Graphics Engine.
/// Frame Buffer rendering from Drawer.
/// Buffer pushing using ScreenDriver.
/// </summary>
class GraphicsEngineTask : private Task, public virtual IFrameEngine
{
private:
	enum class EngineStateEnum : uint8_t
	{
		WaitForScreenStart,
		Clear,
		Render,
		FrameLimit,
		PushBufferStart,
		PushBuffer,
		PushingBuffer,
		VSync
	};

private:
	IFrameBuffer* FrameBuffer;
	IScreenDriver* ScreenDriver;
	IFrameDraw* Drawer = nullptr;

#if defined(GRAPHICS_ENGINE_MEASURE)
	EngineStatusStruct EngineStatus{};
	uint32_t ClearDuration = 0;
	uint32_t FrameDuration = 0;
	uint32_t RenderDuration = 0;
	uint32_t LongestRenderCall = 0;
	uint32_t LongestPushCall = 0;

	uint32_t PushStart = 0;
#endif

private:
	const uint32_t TargetPeriod;
	uint32_t FrameTime = 0;
	uint32_t FrameStart = 0;
	uint16_t FrameCounter = 0;

	EngineStateEnum EngineState = EngineStateEnum::WaitForScreenStart;

public:
	GraphicsEngineTask(Scheduler* scheduler,
		IFrameBuffer* source,
		IScreenDriver* screenDriver,
		const uint32_t targetPeriodMicros = 16666)
		: IFrameEngine()
		, Task(TASK_IMMEDIATE, TASK_FOREVER, scheduler, false)
		, FrameBuffer(source)
		, ScreenDriver(screenDriver)
		, TargetPeriod(targetPeriodMicros)
	{}

	void SetBufferTaskCallback(void (*taskCallback)(void* parameter))
	{
		if (ScreenDriver != nullptr)
		{
			ScreenDriver->SetBufferTaskCallback(taskCallback);
		}
	}

	void BufferTaskCallback(void* parameter)
	{
		if (ScreenDriver != nullptr)
		{
			ScreenDriver->BufferTaskCallback(parameter);
		}
	}

public: // IFrameEngine implementation.
	virtual void SetDrawer(IFrameDraw* drawer) final
	{
		if (drawer != nullptr)
		{
			Drawer = drawer;

			if (EngineState == EngineStateEnum::Render)
			{
				EngineState = EngineStateEnum::WaitForScreenStart;
			}
		}
	}

	virtual void SetInverted(const bool inverted) final
	{
		if (FrameBuffer != nullptr)
		{
			FrameBuffer->SetInverted(inverted);
		}
	}

	virtual const bool Start() final
	{
		if (FrameBuffer != nullptr
			&& FrameBuffer->GetWidth() == ScreenDriver->GetWidth()
			&& FrameBuffer->GetHeight() == ScreenDriver->GetHeight()
			&& ScreenDriver->GetWidth() > 0
			&& ScreenDriver->GetWidth() < UINT8_MAX)
		{
			if (ScreenDriver->Start())
			{
				Task::enable();
				Task::forceNextIteration();

				FrameCounter = 0;
				EngineState = EngineStateEnum::WaitForScreenStart;

#if defined(GRAPHICS_ENGINE_MEASURE)
				EngineStatus.Clear();
#endif
				return true;
			}
#if defined(GRAPHICS_ENGINE_DEBUG)
			else
			{
				Serial.println(F("Sink Start failed."));
			}
#endif
		}
#if defined(GRAPHICS_ENGINE_DEBUG)
		else
		{
			Serial.println(F("ScreenDriver Validation failed."));

			if (FrameBuffer != nullptr)
			{
				Serial.print(F("FrameBuffer: "));
				Serial.print(FrameBuffer->GetWidth());
				Serial.print('x');
				Serial.println(FrameBuffer->GetHeight());
			}

			Serial.print(F("ScreenDriver: "));
			Serial.print(ScreenDriver->GetWidth());
			Serial.print('x');
			Serial.println(ScreenDriver->GetHeight());

			if (FrameBuffer == nullptr)
				Serial.println(F("IFrameBuffer == nullptr"));
			if ((FrameBuffer->GetWidth() != ScreenDriver->GetWidth()) || (FrameBuffer->GetHeight() != ScreenDriver->GetHeight()))
				Serial.println(F("FrameBuffer dimensions don't match ScreenDriver."));
			if ((ScreenDriver->GetWidth() == 0)
				|| (ScreenDriver->GetWidth() == UINT8_MAX))
				Serial.println(F("Invalid dimensions."));
		}
#endif
		Stop();

		return false;
	}

	virtual void Stop() final
	{
		if (ScreenDriver != nullptr)
		{
			ScreenDriver->Stop();
		}
		Task::disable();
	}

	virtual void GetEngineStatus(EngineStatusStruct& status) final
	{
#if defined(GRAPHICS_ENGINE_MEASURE)
		EngineStatus.CopyTo(status);
#endif
	}

public:
	bool Callback() final
	{
		const uint32_t timestamp = micros();

		switch (EngineState)
		{
		case EngineStateEnum::WaitForScreenStart:
			if (ScreenDriver->CanPushBuffer())
			{
				EngineState = EngineStateEnum::Clear;
				FrameStart = timestamp;
			}
			Task::delay(0);
			break;
		case EngineStateEnum::Clear:
			FrameBuffer->ClearFrameBuffer();
#if defined(GRAPHICS_ENGINE_MEASURE)
			ClearDuration = micros() - timestamp;
			LongestRenderCall = 0;
#endif
			FrameTime = timestamp;
			EngineState = EngineStateEnum::Render;
			Task::delay(0);
			break;
		case EngineStateEnum::Render:
			if (Drawer != nullptr)
			{
				if (Drawer->DrawToFrame(FrameTime, FrameCounter))
				{
#if defined(GRAPHICS_ENGINE_MEASURE)
					RenderDuration = micros() - FrameTime;
#endif
					Task::delay(0);
					EngineState = EngineStateEnum::FrameLimit;
				}
#if defined(GRAPHICS_ENGINE_MEASURE)
				UpdateLongestRender(micros() - timestamp);
#endif
			}
			else
			{
				Task::delay(0);
				EngineState = EngineStateEnum::FrameLimit;
			}
			break;
		case EngineStateEnum::FrameLimit:
			if (FrameLimit(timestamp))
			{

				EngineState = EngineStateEnum::PushBufferStart;
				Task::delay(0);
			}
			break;
		case EngineStateEnum::PushBufferStart:
			if (ScreenDriver->CanPushBuffer())
			{
				ScreenDriver->StartBuffer();

#if defined(GRAPHICS_ENGINE_MEASURE)
				PushStart = timestamp;
				LongestPushCall = 0;
				UpdateLongestPush(micros() - timestamp);
#endif

				EngineState = EngineStateEnum::PushBuffer;
				Task::delay(0);
			}
			break;
		case EngineStateEnum::PushBuffer:
			Task::delay(ScreenDriver->PushBuffer(FrameBuffer->GetFrameBuffer()) / 1000);
#if defined(GRAPHICS_ENGINE_MEASURE)
			UpdateLongestPush(micros() - timestamp);
#endif
			EngineState = EngineStateEnum::PushingBuffer;
			break;
		case EngineStateEnum::PushingBuffer:
			Task::delay(0);
			if (!ScreenDriver->PushingBuffer(FrameBuffer->GetFrameBuffer()))
			{
				EngineState = EngineStateEnum::VSync;
			}
#if defined(GRAPHICS_ENGINE_MEASURE)
			UpdateLongestPush(micros() - timestamp);
#endif
			break;
		case EngineStateEnum::VSync:
			ScreenDriver->EndBuffer();
#if defined(GRAPHICS_ENGINE_MEASURE)
			UpdateLongestPush(micros() - timestamp);
			UpdateEngineStatus(timestamp - PushStart);
#endif
			FrameCounter++;
			EngineState = EngineStateEnum::Clear;
			Task::delay(0);
			break;
		default:
			Task::disable();
			return false;
			break;
		}

		return true;
	}

private:
	const bool FrameLimit(const uint32_t timestamp)
	{
		const uint32_t frameDuration = timestamp - FrameStart;

		if (frameDuration >= TargetPeriod)
		{
#if defined(GRAPHICS_ENGINE_MEASURE)
			FrameDuration = frameDuration;
#endif
			if (frameDuration >= (TargetPeriod * 2))
			{
				const uint16_t skipped = (frameDuration - TargetPeriod) / TargetPeriod;

				FrameStart = timestamp;
				FrameCounter += skipped;

#if defined(GRAPHICS_ENGINE_DEBUG)
				Serial.print(F("Frame(s) skipped: "));
				Serial.println(skipped);
#endif
			}
			else
			{
				FrameStart += TargetPeriod;
			}

			return true;
		}
		else
		{
			// Sleep until we're at the last millisecond.
			const uint32_t sleepDuration = (TargetPeriod - frameDuration) / 1000;
			Task::delay(sleepDuration);

			return false;
		}
	}

#if defined(GRAPHICS_ENGINE_MEASURE)
	void UpdateEngineStatus(const uint32_t pushDuration)
	{
		EngineStatus.ClearDuration = ClearDuration;
		EngineStatus.RenderDuration = RenderDuration;
		EngineStatus.LongestRenderCall = LongestRenderCall;
		EngineStatus.PushDuration = pushDuration;
		EngineStatus.LongestPushCall = LongestPushCall;
		EngineStatus.FrameDuration = FrameDuration;
		EngineStatus.TargetDuration = TargetPeriod;
	}

	void UpdateLongestRender(const uint32_t renderCallDuration)
	{
		if (renderCallDuration > LongestRenderCall)
		{
			LongestRenderCall = renderCallDuration;
		}
	}

	void UpdateLongestPush(const uint32_t pushCallDuration)
	{
		if (pushCallDuration > LongestPushCall)
		{
			LongestPushCall = pushCallDuration;
		}
	}

#endif
};
#endif