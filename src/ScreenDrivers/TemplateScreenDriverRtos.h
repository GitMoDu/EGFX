#ifndef _TEMPLATE_SCREEN_DRIVER_RTOS_h
#define _TEMPLATE_SCREEN_DRIVER_RTOS_h

#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_RP2040)
#if !defined(ARDUINO_ARCH_RP2040) || defined(__FREERTOS)
#define TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE
#endif
#endif
#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_NRF52)
#if !defined(ARDUINO_ARCH_RP2040) || defined(__FREERTOS)
#define TEMPLATE_SCREEN_DRIVER_RTOS
#endif

#if defined(TEMPLATE_SCREEN_DRIVER_RTOS)
#include "../Model/IScreenDriver.h"

#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_NRF52)
#include <Arduino.h>
#endif

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

namespace Egfx
{
	/// <summary>
	/// Wraps an Inline ScreenDriver with threaded buffer push.
	/// Non - blocking push and wait until push is done.
	/// </summary>
	/// <typeparam name="InlineScreenDriver"></typeparam>
	/// <typeparam name="pushSleepDuration">Allows the main task scheduller to skip after a push start (in microseconds).</typeparam>
	/// <typeparam name="stackHeight"></typeparam>
	/// <typeparam name="priority"></typeparam>
	/// <typeparam name="coreAffinity"></typeparam>
	template<typename ConstructorParameterType,
		typename InlineScreenDriver,
		const uint32_t pushSleepDuration = 0,
		const uint32_t stackHeight = 1500,
		const UBaseType_t priority = 1
#if defined(TEMPLATE_SCREEN_DRIVER_RTOS_MULTI_CORE)
		, const uint32_t coreAffinity = tskNO_AFFINITY
#endif
	>
	class TemplateScreenDriverRtos : public InlineScreenDriver
	{
	private:
		using BaseClass = InlineScreenDriver;
		const SemaphoreHandle_t Mutex;

	private:
		uint8_t* FrameBuffer = nullptr;
		uint8_t* TaskFrameBuffer = nullptr;

		TaskFunction_t TaskCallback = nullptr;

		TaskHandle_t BufferTaskHandle = NULL;

		volatile bool TaskReady = false;

	public:
		TemplateScreenDriverRtos(ConstructorParameterType& parameter)
			: BaseClass(parameter)
			, Mutex(xSemaphoreCreateMutex())
		{
		}

		bool Start() final
		{
			if (TaskCallback != nullptr
				&& Mutex != NULL
				&& BaseClass::Start())
			{
#if defined(ARDUINO_ARCH_ESP32)
				xTaskCreatePinnedToCore(TaskCallback, "BufferTask", stackHeight, this, priority, &BufferTaskHandle, coreAffinity);
#elif defined(ARDUINO_ARCH_RP2040)
				xTaskCreateAffinitySet(TaskCallback, "BufferTask", stackHeight, this, priority, coreAffinity, &BufferTaskHandle);
#else // ESP8266, NRF52, others with FreeRTOS
				xTaskCreate(TaskCallback, "BufferTask", stackHeight, this, priority, &BufferTaskHandle);
#endif
				if (BufferTaskHandle != NULL)
				{
					return true;
				}
			}

			return false;
		}

		void Stop() final
		{
			if (BufferTaskHandle != NULL)
			{
				if (xSemaphoreTake(Mutex, portMAX_DELAY) == pdTRUE)
				{
					TaskReady = false;
					FrameBuffer = nullptr;
					TaskFrameBuffer = nullptr;
					xSemaphoreGive(Mutex);
				}
				vTaskDelete(BufferTaskHandle);
				BufferTaskHandle = NULL;
			}


			BaseClass::Stop();
		}

		bool CanPushBuffer() final
		{
			if (!BaseClass::CanPushBuffer())
				return false;

			bool can = false;
			if (xSemaphoreTake(Mutex, portMAX_DELAY) == pdTRUE)
			{
				const bool pushing = FrameBuffer != nullptr;
				can = TaskReady && !pushing;
				xSemaphoreGive(Mutex);
			}

			return can;
		}

		void StartBuffer() final
		{
		}

		uint32_t PushBuffer(const uint8_t* frameBuffer) final
		{
			if (xSemaphoreTake(Mutex, portMAX_DELAY) == pdTRUE)
			{
				FrameBuffer = (uint8_t*)frameBuffer;
				xSemaphoreGive(Mutex);
			}

			// Wake worker task to process buffer.
			if (BufferTaskHandle != NULL)
			{
				vTaskResume(BufferTaskHandle);
			}

			return pushSleepDuration;
		}

		bool PushingBuffer(const uint8_t* frameBuffer) final
		{
			bool pushing = true;
			if (xSemaphoreTake(Mutex, portMAX_DELAY) == pdTRUE)
			{
				pushing = FrameBuffer != nullptr;
				xSemaphoreGive(Mutex);
			}

			return pushing;
		}

		void EndBuffer() final
		{
		}

		void SetBufferTaskCallback(void (*taskCallback)(void* parameter)) final
		{
			TaskCallback = taskCallback;
		}

		// Worker task entry point (infinite loop).
		void BufferTaskCallback(void* parameter) final
		{
			TemplateScreenDriverRtos* self = static_cast<TemplateScreenDriverRtos*>(parameter);

			for (;;)
			{
				// Prime TaskReady once.
				if (!self->TaskReady)
				{
					if (xSemaphoreTake(self->Mutex, portMAX_DELAY) == pdTRUE)
					{
						self->TaskReady = true;
						xSemaphoreGive(self->Mutex);
					}
				}

				// Snapshot buffer pointer.
				if (xSemaphoreTake(self->Mutex, portMAX_DELAY) == pdTRUE)
				{
					self->TaskFrameBuffer = self->FrameBuffer;
					xSemaphoreGive(self->Mutex);
				}

				if (self->TaskFrameBuffer != nullptr)
				{
					self->BaseClass::StartBuffer();

					const uint32_t pushSleepMicro = self->BaseClass::PushBuffer(self->TaskFrameBuffer);

					// Convert microseconds to ticks safely.
					if (pushSleepMicro > 0)
					{
						const TickType_t delayTicks = pdMS_TO_TICKS(pushSleepMicro / 1000);
						if (delayTicks > 0)
							vTaskDelay(delayTicks);
					}

					while (self->BaseClass::PushingBuffer(self->TaskFrameBuffer))
					{
						vTaskDelay(1);
					}

					self->BaseClass::EndBuffer();
					self->TaskFrameBuffer = nullptr;

					// Clear FrameBuffer and suspend until next resume.
					if (xSemaphoreTake(self->Mutex, portMAX_DELAY) == pdTRUE)
					{
						self->FrameBuffer = nullptr;
						xSemaphoreGive(self->Mutex);
					}

					vTaskSuspend(self->BufferTaskHandle);
				}
				else
				{
					// No work: suspend until resumed on next PushBuffer.
					vTaskSuspend(self->BufferTaskHandle);
				}
			}
		}
	};
}
#endif
#endif
#endif
