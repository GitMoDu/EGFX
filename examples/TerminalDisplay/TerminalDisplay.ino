/*
* Terminal Display, serial print to screen using EGFX graphics engine.
* Auto layout based on screen dimensions.
* Includes configurations for multiple screens (DisplayConfiguration.h).
* Available options for serial logging, dynamic/double frame buffers, and performance logging.
*/

#define SERIAL_LOG // Enable serial logging.
//#define DEBUG // Extra serial logging for debug builds.
#define SERIAL_BAUD_RATE 115200

//#define USE_DYNAMIC_FRAME_BUFFER // Enable dynamic allocation of framebuffer.
//#define USE_DOUBLE_FRAME_BUFFER // Enable double framebuffer.
#define USE_PERFORMANCE_LOG_TASK // Enable performance logging task.

//#define EGFX_PERFORMANCE_LOG // Enable performance logging for EGFX engine.
//#define EGFX_PERFORMANCE_LOG_DETAIL // Enable detailed performance logging for EGFX engine.

#define _TASK_OO_CALLBACKS
#include <TScheduler.hpp>

// Configure display in this header.
#include "DisplayConfiguration.h"
#include <EgfxDisplayEngine.h>
#include <Modules/Kit/Include.h>

#include <EgfxModules.h>

// Process scheduler.
TS::Scheduler SchedulerBase{};

// Display communication instance.
auto& DisplayInterface(DisplayConfig::Interface());

// Screen driver using specified communications hardware.
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

// Screen layout for terminal display.
struct Layout
{
	static constexpr int16_t Margin() { return 1; }
	static constexpr Egfx::pixel_t X() { return Margin(); }
	static constexpr Egfx::pixel_t Y() { return Margin(); }
	static constexpr Egfx::pixel_t Width() { return FramebufferType::FrameWidth - 2 * Margin(); }
	static constexpr Egfx::pixel_t Height() { return FramebufferType::FrameHeight - 2 * Margin(); }
};

// Terminal font type. Must be a monospace font for terminal display. 
using TerminalFontType = Egfx::Modules::Kit::Font::Bitmask::Resin::FontType6x6;

// Scaled font size based on screen width.
static constexpr uint8_t TerminalFontScale = 1 + (Layout::Width() / TerminalFontType::GetFontWidth()) / 24;

// Terminal glyph style for text rendering. Uses standard transparent black color for bitmask font, and auto scaled font size.
using TerminalGlyphStyle = Egfx::Framework::Image::TemplateImageStyle<
	Egfx::Framework::Layout::AlignmentEnum::TopLeft,
	true, Egfx::RGB_COLOR_BLACK,
	IntegerSignal::MaxValue(1, TerminalFontScale / 2), TerminalFontScale
>;

// Terminal text view type, using the specified layout, font, and glyph style.
using TerminalTextViewType = Egfx::Framework::Text::Bitmask::TextView<
	typename Egfx::Framework::AutoDimension::ByLayout<Layout>::dimension_t,
	Layout, TerminalFontType, TerminalGlyphStyle>;

// Terminal configuration for text rendering, including font size, kerning, line spacing, and other parameters.
using TerminalConfig = Egfx::Modules::TerminalWindow::Definitions::TerminalConfig<
	uint16_t(TerminalFontType::GetFontWidth())* TerminalGlyphStyle::ScaleX, uint16_t(TerminalFontType::GetFontHeight())* TerminalGlyphStyle::ScaleY>;

// Terminal view type, combining the layout, text view, and configuration for the terminal display.
using TerminalViewType = Egfx::Modules::TerminalWindow::View::Terminal<Layout,
	TerminalTextViewType, TerminalConfig>;

// Terminal buffer type, derived from the terminal view type, used for managing the text content of the terminal display.
using TerminalBufferType = typename TerminalViewType::BufferType;

// Single terminal display view.
Egfx::Framework::View::ViewAdapter<TerminalViewType> ViewsView{};

// Reference to terminal buffer instance. Can be used for direct buffer access.
auto& TerminalBuffer = ViewsView.ViewInstance.Buffer();

// Adapter to allow Print interface to write to terminal buffer.  Can be used as serial output replacement.
Egfx::Modules::TerminalWindow::Input::PrintAdapter<TerminalBufferType> SerialDisplay(TerminalBuffer);

// Optional performance logging task.
#if defined(USE_PERFORMANCE_LOG_TASK) // Optional performance logging task. Will output to serial display.
Egfx::PerformanceLogTask<5000> EngineLog(SchedulerBase, DisplayEngine, SerialDisplay);
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
	Buffer = new uint8_t[FramebufferType::BufferSize]{};
	Framebuffer.SetBuffer(Buffer);
#if defined(USE_DOUBLE_FRAME_BUFFER)
	Buffer2 = new uint8_t[FramebufferType::BufferSize]{};
	Framebuffer.SetAltBuffer(Buffer2);
#endif
#endif

	// Initialize backlight pin, if defined.
	if (DisplayConfig::BACKLIGHT != UINT8_MAX)
	{
		pinMode(DisplayConfig::BACKLIGHT, OUTPUT);
		digitalWrite(DisplayConfig::BACKLIGHT, HIGH);
	}

	// Initialize comms hardware.
	DisplayInterface.begin();

	// Set I2C max clock speed for AVR platforms.
#if defined(ARDUINO_ARCH_AVR)
	Wire.setClock(F_CPU >= 16000000L ? 800000L : 400000L);
#endif

	// Optional callback for RTOS driver variants.
	DisplayEngine.SetBufferTaskCallback(BufferTaskCallback);

	DisplayEngine.SetDrawer(&ViewsView);

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
	Serial.print(FramebufferType::ColorDepth);
	if (FramebufferType::Monochrome)
	{
		Serial.println(F(" bit monochrome screen."));
	}
	else
	{
		Serial.println(F(" bit color screen."));
	}
#endif

	SerialDisplay.println(F("Terminal Display Start."));
	SerialDisplay.print(F("\tLines: "));
	SerialDisplay.println(TerminalViewType::Lines);
	SerialDisplay.print(F("\tCharacters: "));
	SerialDisplay.println(TerminalViewType::CharactersPerLine);

	SerialDisplay.print(F("Push text to serial port to see it here."));
}

void loop()
{
	// Serial loopback to display.
	if (Serial.available() > 0)
	{
		while (SerialDisplay.availableForWrite() && Serial.available() > 0)
		{
			SerialDisplay.print(static_cast<char>(Serial.read()));
		}
	}

	SchedulerBase.execute();
}

void BufferTaskCallback(void* parameter)
{
	DisplayEngine.BufferTaskCallback(parameter);
}