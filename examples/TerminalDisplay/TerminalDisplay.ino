/*
* Terminal Display, serial print to screen using EGFX graphics engine.
* Auto layout based on screen dimensions.
* Includes configurations for multiple screens (DisplayConfiguration.h).
* Available options for serial logging, dynamic/double frame buffers, and performance logging.
*/

#define SERIAL_LOG // Enable serial logging.
#define SERIAL_BAUD_RATE 115200

//#define USE_DYNAMIC_FRAME_BUFFER // Enable dynamic allocation of framebuffer.
//#define USE_DOUBLE_FRAME_BUFFER // Enable double framebuffer.
//#define USE_PERFORMANCE_LOG_TASK // Enable performance logging task.

#define EGFX_PERFORMANCE_LOG // Enable performance logging for EGFX engine.
//#define EGFX_PERFORMANCE_LOG_DETAIL // Enable detailed performance logging for EGFX engine.


#define _TASK_OO_CALLBACKS
#include <TScheduler.hpp>

// Configure display in this header.
#include "DisplayConfiguration.h"

#include <EgfxCore.h>
#include <EgfxDisplayEngine.h>

#include "DisplayPrint.h"
#include "Assets.h"

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

// Use full screen layout for terminal display.
using FullLayout = Egfx::LayoutElement<0, 0, DisplayConfiguration::FramebufferType::FrameWidth,
	DisplayConfiguration::FramebufferType::FrameHeight>;

// Display terminal print. Can be used as serial output replacement.
Assets::TextDisplayAutoType<FullLayout> TerminalDisplayPrinter{};

#if defined(USE_PERFORMANCE_LOG_TASK) // Optional performance logging task. Will output to display serial.
Egfx::PerformanceLogTask<4000> EngineLog(SchedulerBase, DisplayEngine, TerminalDisplayPrinter);
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
	// Always start serial to enable loopback to display.
	Serial.begin(SERIAL_BAUD_RATE);

#if defined(SERIAL_LOG)
	while (!Serial)
		;
	delay(1000);
	Serial.println(F("Terminal Display setup..."));
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

	// Setup display printer.
	if (!TerminalDisplayPrinter.Setup(DisplayEngine))
	{
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
	DisplayEngine.SetSyncType(Egfx::DisplaySyncType::Vrr);

#if defined(USE_PERFORMANCE_LOG_TASK) // Start performance logging task.
	EngineLog.Start();
#endif

#if defined(SERIAL_LOG)
	Serial.println(F("Terminal Display Start."));
	Serial.print(DisplayConfiguration::FramebufferType::ColorDepth);
	if (DisplayConfiguration::FramebufferType::Monochrome)
	{
		Serial.println(F(" bit monochrome screen."));
	}
	else
	{
		Serial.println(F(" bit color screen."));
	}
#endif
	TerminalDisplayPrinter.println(F("Terminal Display Start."));
	TerminalDisplayPrinter.print(F("\tLines: "));
	TerminalDisplayPrinter.println(TerminalDisplayPrinter.LineCount);
	TerminalDisplayPrinter.print(F("\tCharacters: "));
	TerminalDisplayPrinter.println(TerminalDisplayPrinter.CharacterCount);
	TerminalDisplayPrinter.print(DisplayConfiguration::FramebufferType::ColorDepth);
	if (DisplayConfiguration::FramebufferType::Monochrome)
	{
		TerminalDisplayPrinter.println(F(" bit monochrome screen."));
	}
	else
	{
		TerminalDisplayPrinter.println(F(" bit color screen."));
	}
}

void loop()
{
	// Serial loopback to display.
	while (Serial.available() > 0)
	{
		TerminalDisplayPrinter.print(static_cast<char>(Serial.read()));
	}

	SchedulerBase.execute();
}

void BufferTaskCallback(void* parameter)
{
	DisplayEngine.BufferTaskCallback(parameter);
}