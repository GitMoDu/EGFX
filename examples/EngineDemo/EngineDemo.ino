/*
* Display Engine Demo, with multiple visual demos cycling automatically.
* Includes configurations for multiple screens (DisplayConfiguration.h).
* Available options for serial logging, dynamic/double frame buffers, and performance logging.
*/

//#define SERIAL_LOG // Enable serial logging.
//#define DEBUG // Extra serial logging for debug builds.
#define SERIAL_BAUD_RATE 115200

//#define USE_DYNAMIC_FRAME_BUFFER // Enable dynamic allocation of framebuffer.
//#define USE_DOUBLE_FRAME_BUFFER // Enable double framebuffer.
//#define USE_PERFORMANCE_LOG_TASK // Enable performance logging task.

//#define EGFX_PERFORMANCE_LOG // Enable performance logging for EGFX engine.
//#define EGFX_PERFORMANCE_LOG_DETAIL // Enable detailed performance logging for EGFX engine.


#define _TASK_OO_CALLBACKS
#include <TScheduler.hpp>

// EGFX display engine and modules.
#include <EgfxDisplayEngine.h>
#include <EgfxAssets.h>

// platform and display configuration.
#include "DisplayConfiguration.h"


// Automatic demo cycler task.
#include "DemoCyclerTask.h"


// Include demos.
#include "LogoSplashDemo.h"
#include "PrimitiveDemo.h"
#include "VectorTextDemo.h"
#include "BitmaskTextDemo.h"
#include "ImageDemo.h"

// Process scheduler.
TS::Scheduler SchedulerBase{};

// Display communication instance.
auto& DisplayInterface(DisplayConfig::Interface());

// Screen driver instance.
ScreenDriverType ScreenDriver(DisplayInterface);

// In-memory frame-buffer. Optionally allocated dynamically. Double-buffering is also optional.
#if defined(USE_DYNAMIC_FRAME_BUFFER)
uint8_t* Buffer = nullptr;
// Frame buffer instance.
#if defined(USE_DOUBLE_FRAME_BUFFER)
uint8_t* Buffer2 = nullptr;
Egfx::TemplateDoubleBufferedFramebuffer<FramebufferType> Framebuffer;
#else
FramebufferType Framebuffer;
#endif
#else
uint8_t Buffer[FramebufferType::BufferSize]{};
#if defined(USE_DOUBLE_FRAME_BUFFER)
uint8_t AltBuffer[FramebufferType::BufferSize]{};
Egfx::TemplateDoubleBufferedFramebuffer<FramebufferType> Framebuffer(Buffer, AltBuffer);
#else
FramebufferType Framebuffer(Buffer);
#endif
#endif

// EGFX display engine task.
Egfx::DisplayEngineTask<FramebufferType, ScreenDriverType> DisplayEngine(
	SchedulerBase, Framebuffer, ScreenDriver);

// The layout of the demos within the screen area.

struct Layout
{
	static constexpr uint8_t MarginX = 0;
	static constexpr uint8_t MarginY = 0;
	static constexpr Egfx::pixel_t X() { return MarginX; }
	static constexpr Egfx::pixel_t Y() { return MarginY; }
	static constexpr Egfx::pixel_t Width() { return FramebufferType::FrameWidth - (MarginX * 2); }
	static constexpr Egfx::pixel_t Height() { return FramebufferType::FrameHeight - (MarginY * 2); }
};

// Is the framebuffer monochrome? Will affect demo rendering.
static constexpr bool Monochrome = FramebufferType::ColorDepth == 1;

// Demo Cycler task. Auto-magic vararg template listing all demo tasks to cycle through.
static constexpr uint32_t CycleDurationMicros = 10000000; // 10 seconds per demo.
EngineDemo::DynamicDemoCyclerTask < CycleDurationMicros,
	LogoSplashDemo::AnimatedFrame<Layout, Monochrome, CycleDurationMicros>
	, PrimitiveDemo::Frame<Layout, Monochrome>
	, ImageDemo::Frame<Layout, Monochrome>
#if !defined(ARDUINO_ARCH_AVR) // Excluded on Arduino AVR due to memory constraints.
	, BitmaskTextDemo::Frame<Layout, Monochrome>
	, VectorTextDemo::Frame<Layout, Monochrome>
#endif
> DemoCycler(&SchedulerBase, &DisplayEngine);

#if defined(USE_PERFORMANCE_LOG_TASK) // Optional performance logging task.
Egfx::PerformanceLogTask<2000> EngineLog(SchedulerBase, DisplayEngine);
#endif

void halt()
{
	// Clean up dynamic memory, if used.
#if defined(USE_DYNAMIC_FRAME_BUFFER)
	delete[] Buffer;
#if defined(USE_DOUBLE_FRAME_BUFFER)
	delete[] Buffer2;
#endif
#endif
#if defined(SERIAL_LOG)
	Serial.println(F("Screen Setup Failed."));
#endif
	while (true)
		;
}

void setup()
{
#if defined(SERIAL_LOG)
	Serial.begin(SERIAL_BAUD_RATE);
	while (!Serial)
		;
	delay(1000);
	Serial.println(F("DemoCycler setup..."));
#endif


#if defined(USE_DYNAMIC_FRAME_BUFFER)
	// Allocate memory and set frame buffer.
	Buffer = new uint8_t[FramebufferType::BufferSize]{};
	Framebuffer.SetBuffer(Buffer);
#if defined(USE_DOUBLE_FRAME_BUFFER)
	Buffer2 = new uint8_t[FramebufferType::BufferSize]{};
	Framebuffer.SetAltBuffer(Buffer2);
#endif
#else
	// Static buffer - nothing to do.
#endif

	// Setup demos.
	if (!DemoCycler.Setup())
	{
#if defined(SERIAL_LOG)
		Serial.println(F("DemoCycler setup failed."));
#endif
		halt();
	}

	// Initialize backlight pin, if defined.
	if (DisplayConfig::BACKLIGHT != UINT8_MAX)
	{
		pinMode(DisplayConfig::BACKLIGHT, OUTPUT);
		digitalWrite(DisplayConfig::BACKLIGHT, HIGH);
	}

	// Initialize display communications.
	DisplayInterface.begin();

	// Set I2C max clock speed for AVR platforms.
#if defined(ARDUINO_ARCH_AVR)
	Wire.setClock(F_CPU >= 16000000L ? 800000L : 400000L);
#endif

	// Optional callback for RTOS driver variants.
	DisplayEngine.SetBufferTaskCallback(BufferTaskCallback);

	// Start EGFX display engine.
	if (!DisplayEngine.Start())
	{
		halt();
	}

	// Set the Display Sync Type.
	DisplayEngine.SetSyncType(Egfx::DisplaySyncType::Vrr);

#if defined(SERIAL_LOG)
#if defined(USE_PERFORMANCE_LOG_TASK) // Start performance logging task.
	EngineLog.Start();
#endif

	Serial.println(F("Graphics Engine Demo Start."));
	Serial.print(FramebufferType::FrameWidth);
	Serial.print(F("x"));
	Serial.println(FramebufferType::FrameHeight);

	Serial.print(FramebufferType::ColorDepth);
	if (FramebufferType::Monochrome)
	{
		Serial.println(F(" bit monochrome screen."));
	}
	else
	{
		Serial.println(F(" bit color screen."));
	}

#if defined(EGFX_PLATFORM_32BIT)
	Serial.println(F("EGFX_PLATFORM_32BIT "));
#else
	Serial.println(F("EGFX_PLATFORM_8BIT "));
#endif

#if defined(EGFX_PLATFORM_HDR)
	Serial.println(F("EGFX_PLATFORM_HDR "));
#else
	Serial.println(F("EGFX_PLATFORM_NO_HDR "));
#endif
#endif
}

void loop()
{
	SchedulerBase.execute();
}

void BufferTaskCallback(void* parameter)
{
	DisplayEngine.BufferTaskCallback(parameter);
}