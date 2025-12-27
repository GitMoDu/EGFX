#ifndef _EGFX_ASYNC_BUFFER_PUSH_TASK_h
#define _EGFX_ASYNC_BUFFER_PUSH_TASK_h

#define _TASK_OO_CALLBACKS
#include <TSchedulerDeclarations.hpp>

//#include <ArduinoGraphicsCore.h>
//#include "DisplaySyncType.h"

namespace Egfx
{
	/// <summary>
	/// Task that manages asynchronous buffer pushing operations to a screen driver.
	/// </summary>
	/// <typeparam name="ScreenDriverType">The type of the screen driver that will receive the buffer data.</typeparam>
	template<typename ScreenDriverType>
	class AsyncBufferPushTask : public TS::Task
	{
	private:
		enum class PushStateEnum : uint8_t
		{
			Idle,
			Start,
			Pushing,
			End
		};

	private:
		ScreenDriverType& ScreenDriver;

	private:
		uint8_t* Buffer = nullptr;

	private:
#if defined(EGFX_PERFORMANCE_LOG)
		uint32_t PushStartTimestamp = 0;
		uint32_t LastPushDuration = 0;
#endif
		PushStateEnum PushState = PushStateEnum::Idle;

	public:
		AsyncBufferPushTask(TS::Scheduler& scheduler, ScreenDriverType& screenDriver)
			: TS::Task(TASK_IMMEDIATE, TASK_FOREVER, &scheduler, false)
			, ScreenDriver(screenDriver)
		{
		}

		void SetBufferTaskCallback(void (*taskCallback)(void* parameter))
		{
			ScreenDriver.SetBufferTaskCallback(taskCallback);
		}

		void BufferTaskCallback(void* parameter)
		{
			ScreenDriver.BufferTaskCallback(parameter);
		}

		bool CanPushBuffer() const
		{
			return PushState == PushStateEnum::Idle && ScreenDriver.CanPushBuffer();
		}

		uint32_t GetPushDuration() const
		{
#if defined(EGFX_PERFORMANCE_LOG)
			return LastPushDuration;
#else
			return 0;
#endif
		}

		void StartPushBuffer(uint8_t* frameBuffer)
		{
			if (frameBuffer != nullptr)
			{
				Buffer = frameBuffer;
				PushState = PushStateEnum::Start;
				TS::Task::enableDelayed(0);
				TS::Task::forceNextIteration();
			}
		}

		bool Callback() final
		{
			switch (PushState)
			{
			case PushStateEnum::Idle:
				TS::Task::disable();
				break;
			case PushStateEnum::Start:
				if (ScreenDriver.CanPushBuffer())
				{
#if defined(EGFX_PERFORMANCE_LOG)
					PushStartTimestamp = micros();
#endif
					ScreenDriver.StartBuffer();
					TS::Task::delay(ScreenDriver.PushBuffer(Buffer));
					PushState = PushStateEnum::Pushing;
				}
				else
				{
					TS::Task::delay(0);
				}
				break;
			case PushStateEnum::Pushing:
				TS::Task::delay(0);
				if (!ScreenDriver.PushingBuffer(Buffer))
				{
					PushState = PushStateEnum::End;
				}
				break;
			case PushStateEnum::End:
			default:
				ScreenDriver.EndBuffer();
				Buffer = nullptr;
				PushState = PushStateEnum::Idle;
				TS::Task::disable();
#if defined(EGFX_PERFORMANCE_LOG)
				LastPushDuration = micros() - PushStartTimestamp;
#endif
				break;
			}
			return true;
		}
	};
}
#endif