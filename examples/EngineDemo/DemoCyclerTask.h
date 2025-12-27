#ifndef _DEMO_CYCLER_TASK_h
#define _DEMO_CYCLER_TASK_h

#define _TASK_OO_CALLBACKS
#include <TSchedulerDeclarations.hpp>

#include <EgfxCore.h>
#include <EgfxDrawer.h>


#if defined(ARDUINO_ARCH_STM32F4)
extern "C" void* _sbrk(int);
#endif

#if !defined(ARDUINO_ARCH_STM32F4) && !defined(ESP8266) && !defined(ESP32) && !defined(ARDUINO_ARCH_AVR)
extern "C" void* sbrk(int);
#endif

template<uint32_t StepDurationMicros,
	typename... AnimationTypes>
class DynamicDemoCyclerTask final : private TS::Task
{
private:
	static constexpr unsigned int TypeCount = sizeof...(AnimationTypes);
	static_assert(TypeCount > 0, "At least one animation type must be provided.");
	static_assert(TypeCount <= 255, "Too many animation types (max 255).");

	// Repeat alloc/free per type to ensure heap stays stable.
	static constexpr uint8_t AllocCheckRepeats = 8;

	Egfx::IFrameEngine* Engine;

	// Track current allocated demo (concrete pointer kept as void*)
	void* CurrentRaw = nullptr;
	Egfx::IFrameDraw* CurrentDrawer = nullptr;
	uint8_t CurrentType = 0; // runtime index into AnimationTypes...

	// Next index to allocate and show
	uint8_t Index = 0;

public:
	DynamicDemoCyclerTask(TS::Scheduler* scheduler, Egfx::IFrameEngine* engine)
		: TS::Task(0, TASK_FOREVER, scheduler, false)
		, Engine(engine)
	{
	}

	~DynamicDemoCyclerTask()
	{
		// Clear engine pointer before deleting current demo
		if (Engine != nullptr)
		{
			Engine->SetDrawer(nullptr);
		}

		if (CurrentRaw != nullptr)
		{
			DeleteByIndex(CurrentRaw, CurrentType);
			CurrentRaw = nullptr;
			CurrentDrawer = nullptr;
		}
	}

	bool Setup()
	{
#if defined(SERIAL_LOG) && defined(DEBUG)
		Serial.println(F("DynamicDemoCyclerTask::Setup - alloc/free with per-type RAM check"));
#endif
		// Only fail on allocation failure. RAM mismatch is logged but not fatal.
		for (uint8_t t = 0; t < (uint8_t)TypeCount; ++t)
		{
#if defined(SERIAL_LOG) && defined(DEBUG)
			const size_t before = GetFreeRam();
#endif
			for (uint8_t r = 0; r < AllocCheckRepeats; ++r)
			{
				void* tmpRaw = nullptr;
				Egfx::IFrameDraw* tmpDrawer = NewByIndex(t, tmpRaw);
				if (tmpDrawer == nullptr)
				{
#if defined(SERIAL_LOG) && defined(DEBUG)
					Serial.print(F("Alloc type index "));
					Serial.print(t);
					Serial.println(F(" failed"));
#endif
					return false;
				}
				DeleteByIndex(tmpRaw, t);
			}

#if defined(SERIAL_LOG) && defined(DEBUG)
			const size_t after = GetFreeRam();
			Serial.print(F("Type "));
			Serial.print(t);
			Serial.print(F(" free RAM before: "));
			Serial.print((unsigned long)before);
			Serial.print(F(", after: "));
			Serial.println((unsigned long)after);
			if (after != before)
			{
				Serial.print(F("RAM delta (non-fatal): "));
				long delta = (long)after - (long)before;
				Serial.println(delta);
			}
#endif
		}

		// Clean initial state; first animation will be set on first Callback
		CurrentRaw = nullptr;
		CurrentDrawer = nullptr;
		CurrentType = 0;
		Index = 0;

		TS::Task::enableDelayed(100);

		return true;
	}

	bool Callback()
	{
		if (TypeCount > 0)
		{
			(void)SetNextAnimation();
		}

		if (TypeCount > 1)
		{
			TS::Task::delay(StepDurationMicros);
		}
		else
		{
			TS::Task::disable();
		}

		return true;
	}

private:
	bool SetNextAnimation()
	{
#if defined(SERIAL_LOG) && defined(DEBUG)
		Serial.print(F("Dynamic Demo idx: "));
		Serial.println(Index);
#endif
		// Detach and delete current to free RAM early
		if (Engine != nullptr)
		{
			Engine->SetDrawer(nullptr);
		}
		if (CurrentRaw != nullptr)
		{
			DeleteByIndex(CurrentRaw, CurrentType);
			CurrentRaw = nullptr;
			CurrentDrawer = nullptr;
		}

		// Allocate new by runtime index
		void* nextRaw = nullptr;
		Egfx::IFrameDraw* nextDrawer = NewByIndex(Index, nextRaw);

		if (nextDrawer != nullptr)
		{
#if defined(SERIAL_LOG)
			PrintDescriptionByIndex(Index, nextRaw);
#endif
			if (Engine != nullptr)
			{
				Engine->SetDrawer(nextDrawer);
			}

			CurrentRaw = nextRaw;
			CurrentDrawer = nextDrawer;
			CurrentType = Index;

			// Advance index
			++Index;
			if (Index >= TypeCount) Index = 0;

			return true;
		}
		else
		{
#if defined(SERIAL_LOG)
			Serial.println(F("Allocation failed; engine drawer remains null"));
#endif
			return false;
		}
	}

	// Runtime factory without std: recursively walk the type list
	template<uint8_t I, typename... Ts>
	struct RuntimeFactory;

	template<uint8_t I, typename T, typename... Ts>
	struct RuntimeFactory<I, T, Ts...>
	{
		static Egfx::IFrameDraw* New(uint8_t idx, void*& raw)
		{
			if (idx == I)
			{
				T* obj = new T();
				raw = obj;
				return static_cast<Egfx::IFrameDraw*>(obj);
			}
			return RuntimeFactory<I + 1, Ts...>::New(idx, raw);
		}

		static bool Delete(uint8_t idx, void*& raw)
		{
			if (raw == nullptr) return false;
			if (idx == I)
			{
				delete static_cast<T*>(raw);
				raw = nullptr;
				return true;
			}
			return RuntimeFactory<I + 1, Ts...>::Delete(idx, raw);
		}
	};

	template<uint8_t I>
	struct RuntimeFactory<I>
	{
		static Egfx::IFrameDraw* New(uint8_t, void*& raw)
		{
			raw = nullptr;
			return nullptr;
		}

		static bool Delete(uint8_t, void*& raw)
		{
			(void)raw;
			return false;
		}
	};

	static Egfx::IFrameDraw* NewByIndex(uint8_t idx, void*& raw)
	{
		return RuntimeFactory<0, AnimationTypes...>::New(idx, raw);
	}

	static void DeleteByIndex(void*& p, uint8_t idx)
	{
		(void)RuntimeFactory<0, AnimationTypes...>::Delete(idx, p);
	}

#if defined(SERIAL_LOG)
	// Try to call T::PrintDescription(Serial) when it exists; otherwise no-op.
	template<typename T>
	static auto TryPrintDescription(T* obj, int) -> decltype(obj->PrintDescription(), void())
	{
		obj->PrintDescription();
	}

	template<typename T>
	static void TryPrintDescription(T*, long)
	{
	}

	template<uint8_t I, typename... Ts>
	struct RuntimePrinter;

	template<uint8_t I, typename T, typename... Ts>
	struct RuntimePrinter<I, T, Ts...>
	{
		static void Print(uint8_t idx, void* raw)
		{
			if (idx == I)
			{
				TryPrintDescription(static_cast<T*>(raw), 0);
			}
			else
			{
				RuntimePrinter<I + 1, Ts...>::Print(idx, raw);
			}
		}
	};

	template<uint8_t I>
	struct RuntimePrinter<I>
	{
		static void Print(uint8_t, void*)
		{
		}
	};

	static void PrintDescriptionByIndex(const uint8_t idx, void* raw)
	{
		RuntimePrinter<0, AnimationTypes...>::Print(idx, raw);
	}
#endif

	// Free RAM helpers
	static size_t GetFreeRam()
	{
#if defined(ESP8266) || defined(ESP32)
		return ESP.getFreeHeap();
#elif defined(ARDUINO_ARCH_AVR)
		extern char __heap_start, * __brkval;
		char top;
		char* brkval = __brkval;
		if (brkval == 0) brkval = &__heap_start;
		return (size_t)(&top - brkval);
#elif defined(ARDUINO_ARCH_STM32F4)
		// STM32 (e.g., Blackpill): use _sbrk provided by newlib-nano
		char top;
		void* heap_end = _sbrk(0);
		return (size_t)((const char*)&top - (const char*)heap_end);
#else
		// Generic fallback: try sbrk(0) if available
		char top;
		void* heap_end = sbrk(0);
		return (size_t)((const char*)&top - (const char*)heap_end);
#endif
	}
};

#endif