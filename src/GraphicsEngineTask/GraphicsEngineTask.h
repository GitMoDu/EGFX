// GraphicsEngineTask.h

#ifndef _GRAPHICS_ENGINE_TASK_h
#define _GRAPHICS_ENGINE_TASK_h

#define _TASK_OO_CALLBACKS
#include <TSchedulerDeclarations.hpp>

#include <ArduinoGraphicsCore.h>
#include "DisplaySyncType.h"

// Enable to log (on Serial/Serial1).
// Logs out-of-sync frames and render timeouts.
// Also does a single log of the duration of a frame-buffer DMA transfer.
//#define GRAPHICS_ENGINE_DEBUG

// Enable Engine measure and GetStatus(). Required for GRAPHICS_ENGINE_DEBUG.
#if defined(GRAPHICS_ENGINE_DEBUG)
#define GRAPHICS_ENGINE_MEASURE
#endif

/// <summary>
/// TS::Task based Graphics Engine.
/// Frame Buffer rendering from Drawer.
/// Buffer pushing using ScreenDriver.
/// </summary>
class GraphicsEngineTask : private TS::Task, public virtual IFrameEngine
{
private:
	enum class EngineStateEnum : uint8_t
	{
		WaitForScreenStart,
		Clear,
		WaitForFrameStart,
		Render,
		Vsync,
		PushBufferStart,
		PushBuffer,
		PushingBuffer,
		PushBufferEnd
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
	uint32_t LongestClearCall = 0;

	uint32_t ClearLoop = 0;
	uint32_t PushStart = 0;
#else
	uint32_t FrameDuration = 0;
#endif

private:
	const uint32_t TargetPeriod;
	uint32_t FrameTime = 0;
	uint32_t FrameStart = 0;
	uint16_t FrameCounter = 0;
	uint16_t DrawCallCounter = 0;

	EngineStateEnum EngineState = EngineStateEnum::WaitForScreenStart;

	DisplaySyncType SyncType = DisplaySyncType::VSync;

public:
	void SetSyncType(const DisplaySyncType syncType)
	{
		SyncType = syncType;
	}

	GraphicsEngineTask(TS::Scheduler* scheduler,
		IFrameBuffer* source,
		IScreenDriver* screenDriver,
		const uint32_t targetPeriodMicros = 16666)
		: IFrameEngine()
		, TS::Task(TASK_IMMEDIATE, TASK_FOREVER, scheduler, false)
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
				TS::Task::enable();
				TS::Task::forceNextIteration();

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
		TS::Task::disable();
	}

	virtual void GetEngineStatus(EngineStatusStruct& status) final
	{
#if defined(GRAPHICS_ENGINE_MEASURE)
		EngineStatus.CopyTo(status);
#else
		status.Clear();
		status.FrameDuration = GetFrameDuration();
#endif
	}

	virtual const uint32_t GetFrameDuration() const final
	{
		return FrameDuration;
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
#if defined(GRAPHICS_ENGINE_MEASURE)
				ClearDuration = 0;
#endif
			}
			TS::Task::delay(0);
			break;
		case EngineStateEnum::Clear:
			if (FrameBuffer->ClearFrameBuffer())
			{
#if defined(GRAPHICS_ENGINE_MEASURE)
				LongestRenderCall = 0;
				DrawCallCounter = 0;
#endif
				FrameTime = timestamp;
				EngineState = EngineStateEnum::WaitForFrameStart;
			}
#if defined(GRAPHICS_ENGINE_MEASURE)
			ClearLoop = micros() - timestamp;
			ClearDuration += ClearLoop;
			if (ClearLoop > LongestClearCall)
			{
				LongestClearCall = ClearLoop;
			}
#endif
			TS::Task::delay(0);
			break;
		case EngineStateEnum::WaitForFrameStart:
			if (timestamp - FrameStart < INT32_MAX)
			{
				EngineState = EngineStateEnum::Render;
			}
			TS::Task::delay(0);
			break;
		case EngineStateEnum::Render:
#if defined(GRAPHICS_ENGINE_MEASURE)
			DrawCallCounter++;
#endif
			if (Drawer != nullptr)
			{
				if (Drawer->DrawToFrame(FrameTime, FrameCounter))
				{
#if defined(GRAPHICS_ENGINE_MEASURE)
					RenderDuration = micros() - FrameTime;
#endif
					TS::Task::delay(0);
					EngineState = EngineStateEnum::Vsync;
				}
#if defined(GRAPHICS_ENGINE_MEASURE)
				UpdateLongestRender(micros() - timestamp);
#endif
			}
			else
			{
				EngineState = EngineStateEnum::Vsync;
				TS::Task::delay(0);
			}
			break;
		case EngineStateEnum::Vsync:
			switch (SyncType)
			{
			case DisplaySyncType::VSync:
				if (Vsync(timestamp))
				{
					EngineState = EngineStateEnum::PushBufferStart;
					TS::Task::delay(0);
				}
				break;
			case DisplaySyncType::Vrr:
				if (Vrr(timestamp))
				{
					EngineState = EngineStateEnum::PushBufferStart;
					TS::Task::delay(0);
				}
				break;
			case DisplaySyncType::NoSync:
				if (NoSync(timestamp))
				{
					EngineState = EngineStateEnum::PushBufferStart;
					TS::Task::delay(0);
				}
				break;
			default:
				break;
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
				TS::Task::delay(0);
			}
			break;
		case EngineStateEnum::PushBuffer:
			TS::Task::delay(ScreenDriver->PushBuffer(FrameBuffer->GetFrameBuffer()));
#if defined(GRAPHICS_ENGINE_MEASURE)
			UpdateLongestPush(micros() - timestamp);
#endif
			EngineState = EngineStateEnum::PushingBuffer;
			break;
		case EngineStateEnum::PushingBuffer:
			TS::Task::delay(0);
			if (!ScreenDriver->PushingBuffer(FrameBuffer->GetFrameBuffer()))
			{
				EngineState = EngineStateEnum::PushBufferEnd;
			}
#if defined(GRAPHICS_ENGINE_MEASURE)
			UpdateLongestPush(micros() - timestamp);
#endif
			break;
		case EngineStateEnum::PushBufferEnd:
			ScreenDriver->EndBuffer();
#if defined(GRAPHICS_ENGINE_MEASURE)
			UpdateLongestPush(micros() - timestamp);
			UpdateEngineStatus(timestamp - PushStart);
			ClearDuration = 0;
			LongestClearCall = 0;
#endif
			EngineState = EngineStateEnum::Clear;
			FrameCounter++;
			TS::Task::delay(0);
			break;
		default:
			TS::Task::disable();
			return false;
			break;
		}

		return true;
	}

private:
	const bool NoSync(const uint32_t timestamp)
	{
		const uint32_t frameElapsed = timestamp - FrameStart;

		if (frameElapsed >= INT32_MAX)
		{
			TS::Task::delay(0);
			return false;
		}

		FrameDuration = timestamp - FrameStart;
		FrameStart = timestamp;

		return true;
	}

	const bool Vrr(const uint32_t timestamp)
	{
		const uint32_t frameElapsed = timestamp - FrameStart;

		if (frameElapsed >= INT32_MAX)
		{
			TS::Task::delay(0);
			return false;
		}

		if (frameElapsed >= TargetPeriod)
		{
			FrameDuration = timestamp - FrameStart;
			FrameStart = timestamp;

			return true;
		}
		else
		{
			// Sleep until we're at the last millisecond.
			const uint32_t sleepDuration = (TargetPeriod - frameElapsed) / 1000;
			TS::Task::delay(sleepDuration);

			return false;
		}
	}

	const bool Vsync(const uint32_t timestamp)
	{
		const uint32_t frameElapsed = timestamp - FrameStart;
		const uint32_t framesElapsed = frameElapsed / TargetPeriod;

		if (frameElapsed >= INT32_MAX)
		{
			TS::Task::delay(0);
			return false;
		}

		if (framesElapsed > 0)
		{
			FrameDuration = timestamp - FrameStart;
			FrameStart += TargetPeriod * framesElapsed;

			if (timestamp - FrameStart >= INT32_MAX)
			{
				FrameStart += TargetPeriod;
			}

			return true;
		}
		else
		{
			// Sleep until we're at the last millisecond.
			const uint32_t sleepDuration = (TargetPeriod - frameElapsed) / 1000;
			TS::Task::delay(sleepDuration);

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
		EngineStatus.LongestClearCall = LongestClearCall;
		EngineStatus.FrameDuration = FrameDuration;
		EngineStatus.TargetDuration = TargetPeriod;
		EngineStatus.DrawCallCount = DrawCallCounter;
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