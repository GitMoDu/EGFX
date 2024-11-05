// TemplateScreenDriverRtos.h

#ifndef _TEMPLATE_SCREEN_DRIVER_RTOS_h
#define _TEMPLATE_SCREEN_DRIVER_RTOS_h

#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_RP2040)
#define TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE
#endif
#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_NRF52)
#define TEMPLATE_SCREEN_DRIVER_RTOS

#include "../Model/IScreenDriver.h"

#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
#include <Arduino.h>
#elif defined(ARDUINO_ARCH_RP2040)
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#endif

/// <summary>
/// Wraps an Inline ScreenDriver with threaded buffer push.
/// Non - blocking push and wait until push is done.
/// </summary>
/// <typeparam name="InlineScreenDriver"></typeparam>
/// <typeparam name="pushSleepDuration">Allows the main task scheduller to skip after a push start (in microseconds).</typeparam>
/// <typeparam name="stackHeight"></typeparam>
/// <typeparam name="priority"></typeparam>
/// <typeparam name="coreAffinity"></typeparam>
template<typename InlineScreenDriver,
	const uint32_t pushSleepDuration = 0,
	const uint32_t stackHeight = 1500,
	const portBASE_TYPE priority = 1
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
	, const uint32_t coreAffinity = tskNO_AFFINITY
#endif
>
class TemplateScreenDriverRtos : public InlineScreenDriver
{
private:
	using BaseClass = InlineScreenDriver;

private:
	const SemaphoreHandle_t Mutex;

private:
	uint8_t* FrameBuffer = nullptr;
	uint8_t* TaskFrameBuffer = nullptr;

	TaskFunction_t TaskCallback = nullptr;
	TaskHandle_t BufferTaskHandle = NULL;

	volatile bool TaskReady = false;

public:
	TemplateScreenDriverRtos()
		: BaseClass()
		, Mutex(xSemaphoreCreateMutex())
	{}

	virtual const bool Start() final
	{
		Stop();

		if (TaskCallback != nullptr
			&& Mutex != NULL
			&& BaseClass::Start())
		{
#if defined(ARDUINO_ARCH_ESP32)
			xTaskCreatePinnedToCore(TaskCallback, "BufferTask", stackHeight, NULL, priority, &BufferTaskHandle, coreAffinity);
#elif defined(ARDUINO_ARCH_RP2040)
			xTaskCreateAffinitySet((TaskFunction_t)TaskCallback, "BufferTask", stackHeight, NULL, priority, coreAffinity, &BufferTaskHandle);
#elif defined(ARDUINO_ARCH_NRF52)
			xTaskCreate(TaskCallback, "BufferTask", stackHeight, NULL, (UBaseType_t)priority, &BufferTaskHandle);
#endif
			if (BufferTaskHandle != NULL)
			{
				return true;
			}
		}

		return false;
	}

	virtual void Stop() final
	{
		if (BufferTaskHandle != NULL)
		{
			if (xSemaphoreTake(Mutex, portMAX_DELAY) == pdTRUE)
			{
				TaskReady = false;
				xSemaphoreGive(Mutex);
			}

			vTaskDelete(BufferTaskHandle);
		}

		TaskFrameBuffer = nullptr;
		TaskReady = false;

		BaseClass::Stop();
	}

	virtual const bool CanPushBuffer() final
	{
		if (BaseClass::CanPushBuffer())
		{
			bool taskReady = false;
			bool pushing = true;
			if (xSemaphoreTake(Mutex, portMAX_DELAY) == pdTRUE)
			{
				taskReady = TaskReady;
				pushing = FrameBuffer != nullptr;
				xSemaphoreGive(Mutex);
			}

			return taskReady && !pushing;
		}

		return false;
	}

	virtual void StartBuffer() final
	{}

	virtual const uint32_t PushBuffer(const uint8_t* frameBuffer) final
	{
		if (xSemaphoreTake(Mutex, portMAX_DELAY) == pdTRUE)
		{
			FrameBuffer = (uint8_t*)frameBuffer;
			xSemaphoreGive(Mutex);
		}

		vTaskResume(BufferTaskHandle);

		return pushSleepDuration;
	}

	virtual const bool PushingBuffer(const uint8_t* frameBuffer) final
	{
		bool pushing = true;
		if (xSemaphoreTake(Mutex, portMAX_DELAY) == pdTRUE)
		{
			pushing = FrameBuffer != nullptr;
			xSemaphoreGive(Mutex);
		}

		return pushing;
	}

	virtual void EndBuffer() final
	{}

	virtual void SetBufferTaskCallback(void (*taskCallback)(void* parameter)) final
	{
		TaskCallback = taskCallback;
	}

	virtual void BufferTaskCallback(void* parameter) final
	{
		for (;;)
		{
			if (xSemaphoreTake(Mutex, portMAX_DELAY) == pdTRUE)
			{
				TaskFrameBuffer = FrameBuffer;
				xSemaphoreGive(Mutex);

				if (TaskReady && TaskFrameBuffer != nullptr)
				{
					BaseClass::StartBuffer();

					const uint32_t pushSleep = BaseClass::PushBuffer(TaskFrameBuffer);
					vTaskDelay(pushSleep / portTICK_PERIOD_MS);

					while (BaseClass::PushingBuffer(TaskFrameBuffer))
					{
						yield();
					}

					BaseClass::EndBuffer();
					TaskFrameBuffer = nullptr;
					yield();

					if (xSemaphoreTake(Mutex, portMAX_DELAY) == pdTRUE)
					{
						FrameBuffer = nullptr;
						xSemaphoreGive(Mutex);

						vTaskSuspend(BufferTaskHandle);
					}
				}
				else
				{
					if (xSemaphoreTake(Mutex, portMAX_DELAY) == pdTRUE)
					{
						TaskReady = true;
						xSemaphoreGive(Mutex);

						vTaskSuspend(BufferTaskHandle);
					}
				}
			}
		}

		TaskFrameBuffer = nullptr;
		if (xSemaphoreTake(Mutex, portMAX_DELAY) == pdTRUE)
		{
			TaskReady = false;
			xSemaphoreGive(Mutex);
		}

		vTaskDelete(BufferTaskHandle);
	}
};
#endif
#endif