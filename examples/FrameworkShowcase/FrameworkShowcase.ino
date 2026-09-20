/*
* Display Engine Demo, with multiple visual demos cycling automatically.
* Includes configurations for multiple screens (DisplayConfiguration.h).
* Available options for serial logging, dynamic/double frame buffers, and performance logging.
*/


#define SERIAL_LOG // Enable serial logging.
#define DEBUG // Extra serial logging for debug builds.
#define SERIAL_BAUD_RATE 115200

//#define USE_DYNAMIC_FRAME_BUFFER // Enable dynamic allocation of framebuffer.

#if !defined(ARDUINO_ARCH_AVR)
#define USE_DOUBLE_FRAME_BUFFER // Enable double framebuffer.
#endif
//#define USE_PERFORMANCE_LOG_TASK // Enable performance logging task.

//#define INTEGERGLASS_PERFORMANCE_LOG // Enable performance logging for INTEGERGLASS engine.
//#define INTEGERGLASS_PERFORMANCE_LOG_DETAIL // Enable detailed performance logging for INTEGERGLASS engine.

#define _TASK_OO_CALLBACKS
#include <TScheduler.hpp>

// INTEGERGLASS display engine and modules.
#include <IntegerGlassDisplayEngine.h>
#include <IntegerGlassFramework.h>
#include <Modules/Kit/Include.h>

// platform and display configuration.
#include "DisplayConfiguration.h"

#include "MasterView.h"


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
IntegerGlass::TemplateDoubleBufferedFramebuffer<FramebufferType> Framebuffer;
#else
FramebufferType Framebuffer;
#endif
#else
uint8_t Buffer[FramebufferType::BufferSize]{};
#if defined(USE_DOUBLE_FRAME_BUFFER)
uint8_t AltBuffer[FramebufferType::BufferSize]{};
#endif
#if defined(USE_DOUBLE_FRAME_BUFFER)
IntegerGlass::TemplateDoubleBufferedFramebuffer<FramebufferType> Framebuffer(Buffer, AltBuffer);
#else
FramebufferType Framebuffer(Buffer);
#endif
#endif

// INTEGERGLASS display engine task.
IntegerGlass::DisplayEngineTask<FramebufferType, ScreenDriverType> DisplayEngine(
	SchedulerBase, Framebuffer, ScreenDriver, 20000);

// The layout of the demos within the screen area.
using Layout = IntegerGlass::Framework::TemplateLayout<0, 0, FramebufferType::FrameWidth, FramebufferType::FrameHeight>;

// Is the framebuffer monochrome? Will affect demo rendering.
static constexpr bool Monochrome = FramebufferType::ColorDepth == 1;

// The master that manages everything on screen, adapted with the framework template view adapter.
IntegerGlass::Framework::View::ViewAdapter<Showcase::MasterView::MasterView<Layout, Monochrome>> ShowcaseView{};

#if defined(USE_PERFORMANCE_LOG_TASK) // Optional performance logging task.
IntegerGlass::PerformanceLogTask<2000> EngineLog(SchedulerBase, DisplayEngine, Serial);
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

#if defined(SERIAL_LOG)
#if defined(USE_DYNAMIC_FRAME_BUFFER)
	Serial.println(F("Framebuffer allocation: heap"));
#elif defined(USE_RP2350_RAM_FRAME_BUFFER)
	Serial.println(F("Framebuffer allocation: RP2350 SDK RAM sections"));
#else
	Serial.println(F("Framebuffer allocation: default static SRAM"));
#endif
	Serial.print(F("Framebuffer: 0x"));
	Serial.println(reinterpret_cast<uintptr_t>(Buffer), HEX);
#if defined(USE_DOUBLE_FRAME_BUFFER)
	Serial.print(F("Alternate framebuffer: 0x"));
#if defined(USE_DYNAMIC_FRAME_BUFFER)
	Serial.println(reinterpret_cast<uintptr_t>(Buffer2), HEX);
#else
	Serial.println(reinterpret_cast<uintptr_t>(AltBuffer), HEX);
#endif
#endif
#endif

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

	// Start INTEGERGLASS display engine.
	if (!DisplayEngine.Start())
	{
		halt();
	}

	// Set the Display Sync Type.
	DisplayEngine.SetSyncType(IntegerGlass::DisplaySyncType::Vrr);

	// Set the drawer for the display engine to the showcase view.
	DisplayEngine.SetDrawer(&ShowcaseView);

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

#if defined(INTEGERGLASS_PLATFORM_32BIT)
	Serial.println(F("INTEGERGLASS_PLATFORM_32BIT "));
#else
	Serial.println(F("INTEGERGLASS_PLATFORM_8BIT "));
#endif

#if defined(INTEGERGLASS_PLATFORM_HDR)
	Serial.println(F("INTEGERGLASS_PLATFORM_HDR "));
#else
	Serial.println(F("INTEGERGLASS_PLATFORM_NO_HDR "));
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