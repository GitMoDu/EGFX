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

// Configure display in this header.
#include "DisplayConfiguration.h"

#include <ArduinoGraphicsEngineTask.h>

// Automatic demo cycler task.
#include "DemoCyclerTask.h"

// Include demos.
#include "SpriteTransformDemo.h"
#include "VectorTextDemo.h"
#include "BitmaskTextDemo.h"
#include "PrimitiveDemo.h"

// Bitmap demo is excluded on Arduino AVR due to memory constraints.
#if !defined(ARDUINO_ARCH_AVR) 
#include "BitmapDemo.h"
#endif



// Process scheduler.
TS::Scheduler SchedulerBase{};

// Screen driver using specified communications hardware.
DisplayConfiguration::ScreenDriverType ScreenDriver(DisplayConfiguration::DisplayCommsInstance);

// In-memory frame-buffer. Optionally allocated dynamically. Double-buffering is also optional.
#if defined(USE_DYNAMIC_FRAME_BUFFER)
uint8_t* Buffer = nullptr;
// Frame buffer instance.
#if defined(USE_DOUBLE_FRAME_BUFFER)
uint8_t* Buffer2 = nullptr;
Egfx::TemplateDoubleBufferedFramebuffer<DisplayConfiguration::FramebufferType> Framebuffer;
#else
DisplayConfiguration::FramebufferType Framebuffer;
#endif
#else
uint8_t Buffer[DisplayConfiguration::FramebufferType::BufferSize]{};
#if defined(USE_DOUBLE_FRAME_BUFFER)
uint8_t AltBuffer[DisplayConfiguration::FramebufferType::BufferSize]{};
Egfx::TemplateDoubleBufferedFramebuffer<DisplayConfiguration::FramebufferType> Framebuffer(Buffer, AltBuffer);
#else
DisplayConfiguration::FramebufferType Framebuffer(Buffer);
#endif
#endif

// EGFX display engine task.
Egfx::DisplayEngineTask<DisplayConfiguration::FramebufferType,
	DisplayConfiguration::ScreenDriverType> DisplayEngine(
		SchedulerBase, Framebuffer, ScreenDriver);

// The layout of drawers can be set independently of screen dimensions.
static constexpr uint8_t Margin = 0;
using FullLayout = LayoutElement<Margin, Margin, DisplayConfiguration::FramebufferType::FrameWidth - (Margin * 2), DisplayConfiguration::FramebufferType::FrameHeight - (Margin * 2)>;
static constexpr bool BinaryDisplay = DisplayConfiguration::FramebufferType::ColorDepth == 1;

// Demo Cycler task. Auto-magic vararg template listing all demo tasks to cycle through.
DynamicDemoCyclerTask<10000
	, PrimitiveDemo<FullLayout, BinaryDisplay>
	, VectorTextDemo<FullLayout, BinaryDisplay>
	, BitmaskTextDemo<FullLayout, BinaryDisplay>
	, SpriteTransformDemo<FullLayout, BinaryDisplay>
#if !defined(ARDUINO_ARCH_AVR) // Arduino AVR can only handle so much.
	, BitmapDemo<FullLayout, BinaryDisplay>
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
	Buffer = new uint8_t[DisplayConfiguration::FramebufferType::BufferSize]{};
	Framebuffer.SetBuffer(Buffer);
#if defined(USE_DOUBLE_FRAME_BUFFER)
	Buffer2 = new uint8_t[DisplayConfiguration::FramebufferType::BufferSize]{};
	Framebuffer.SetAltBuffer(Buffer2);
#endif
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
	if (DisplayConfiguration::DisplayPins::BACKLIGHT != UINT8_MAX)
	{
		pinMode(DisplayConfiguration::DisplayPins::BACKLIGHT, OUTPUT);
		digitalWrite(DisplayConfiguration::DisplayPins::BACKLIGHT, HIGH);
	}

	// Initialize comms hardware.
	DisplayConfiguration::DisplayCommsInstance.begin();
#if defined(ARDUINO_ARCH_AVR)
	DisplayConfiguration::DisplayCommsInstance.setClock(800000);
#endif
	// Optional callback for RTOS driver variants.
	DisplayEngine.SetBufferTaskCallback(BufferTaskCallback);

	// Start EGFX display engine.
	if (!DisplayEngine.Start())
	{
		halt();
	}

	// Set the Display Sync Type.
	DisplayEngine.SetSyncType(DisplaySyncType::Vrr);

#if defined(SERIAL_LOG)
#if defined(USE_PERFORMANCE_LOG_TASK) // Start performance logging task.
	EngineLog.Start();
#endif

	Serial.println(F("Graphics Engine Demo Start."));
	Serial.print(DisplayConfiguration::FramebufferType::ColorDepth);
	if (DisplayConfiguration::FramebufferType::Monochrome)
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