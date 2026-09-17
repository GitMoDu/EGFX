/*
* Display Engine Demo, with multiple visual demos cycling automatically.
* Includes configurations for multiple screens (DisplayConfiguration.h).
* Available options for serial logging, dynamic/double frame buffers, and performance logging.
*/

//#define SERIAL_LOG // Enable serial logging.
//#define DEBUG // Extra serial logging for debug builds.
#define SERIAL_BAUD_RATE 115200

//#define USE_DYNAMIC_FRAME_BUFFER // Enable dynamic allocation of framebuffer.

#if !defined(ARDUINO_ARCH_AVR)
#define USE_DOUBLE_FRAME_BUFFER // Enable double framebuffer.
#endif

//#define USE_FPS_DISPLAY // Enable FPS display module.
//#define USE_PERFORMANCE_LOG_TASK // Enable performance logging task.

//#define EGFX_PERFORMANCE_LOG // Enable performance logging for EGFX engine.
//#define EGFX_PERFORMANCE_LOG_DETAIL // Enable detailed performance logging for EGFX engine.

#define _TASK_OO_CALLBACKS
#include <TScheduler.hpp>

// EGFX display engine and modules.
#include <EgfxDisplayEngine.h>
#include <EgfxModules.h>

// platform and display configuration.
#include "DisplayConfiguration.h"

// Logo splash demo with custom renderer and auto-layout.
#include "LogoSplashDemo.h"

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
	static constexpr Egfx::pixel_t X() { return 0; }
	static constexpr Egfx::pixel_t Y() { return 0; }
	static constexpr Egfx::pixel_t Width() { return FramebufferType::FrameWidth; }
	static constexpr Egfx::pixel_t Height() { return FramebufferType::FrameHeight; }
};


// Is the framebuffer monochrome? Will affect demo rendering.
static constexpr bool Monochrome = FramebufferType::ColorDepth == 1;

// Animated splash logo as demo view, with default parameters for animation duration, auto-start, and loop.
using DemoViewType = LogoSplashDemo::AnimatedView<Layout, Monochrome>;

#if defined(USE_FPS_DISPLAY)
// Wrap the demo view with FPS display, using the specified layout and demo view type.
using FpsCompositeViewType = Egfx::Modules::FpsDisplay::View::CompositeWithFps<
	Layout,
	Egfx::Modules::FpsDisplay::FpsDrawerPosition::TopRight,
	DemoViewType>;
#else
using FpsCompositeViewType = DemoViewType;
#endif

// View adapter for the display engine, using the FPS composite view type.
using EngineViewType = Egfx::Framework::View::ViewAdapter<FpsCompositeViewType>;

// View instance for the display engine, using the FPS composite view type.
EngineViewType DemoView{};

#if defined(USE_PERFORMANCE_LOG_TASK) // Optional performance logging task.
Egfx::PerformanceLogTask<2000> EngineLog(SchedulerBase, DisplayEngine, Serial);
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

	// Set the demo view as the drawer for the display engine.
	DisplayEngine.SetDrawer(&DemoView);

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
	
#if defined(EGFX_PLATFORM_32BIT)
	Serial.println(F("EGFX_PLATFORM_32BIT "));
#else
	Serial.println(F("EGFX_PLATFORM_8BIT "));
#endif

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

	Serial.println(F("Graphics Engine Start."));
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