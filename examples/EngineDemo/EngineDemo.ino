/*
* Graphics Engine Demo.
* Includes configurations for multiple screens (uncomment screen driver).
* Frame buffer can be allocated dinamically (USE_DYNAMIC_FRAME_BUFFER).
*/

#define DEBUG
#define SERIAL_BAUD_RATE 115200

//#define EGFX_PLATFORM_HDR
//#define EGFX_PLATFORM_BIG

//#define USE_DYNAMIC_FRAME_BUFFER // Enable dynamic allocation frame buffer.
//#define GRAPHICS_ENGINE_DEBUG // Enable debug logging and coordinates validation.
#define GRAPHICS_ENGINE_MEASURE // Enabled engine measuring and logging.

// Preset of SPI pin definitions for various platforms.
#if defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32)
#define TFT_DC		PB0
#define TFT_RST     PB1
#define TFT_CS		PA4
#elif defined(ARDUINO_ARCH_STM32F1)
#define TFT_CS		7
#define TFT_DC		10
#define TFT_RST     11
#elif defined(ARDUINO_ARCH_AVR)
#define TFT_CS		10
#define TFT_DC		9
#define TFT_RST		8
#elif defined(ARDUINO_ARCH_ESP32)
#define TFT_CS		19
#define TFT_DC		20
#define TFT_RST		21
#elif defined(ARDUINO_SEEED_XIAO_RP2350)
#define TFT_CS		D3
#define TFT_DC		D7
#define TFT_RST		D6
#elif defined(ARDUINO_ARCH_RP2040)
#define TFT_CS		19
#define TFT_DC		20
#define TFT_RST		21
#elif defined(ARDUINO_ARCH_NRF52)
#define TFT_CS		7
#define TFT_DC		4
#define TFT_RST		5
#endif

#define _TASK_OO_CALLBACKS
#include <TScheduler.hpp>

#include <ArduinoGraphicsDrivers.h>
#include <ArduinoGraphicsEngineTask.h>
#include <ArduinoGraphicsCore.h>
#include <ArduinoGraphicsDrawers.h>


#include "DemoCyclerTask.h"
#include "PrimitiveDemo.h"
#include "TransformDemo.h"
#include "TextCharactersDemo.h"
#include "NumberAlignmentDemo.h"
#include "TextSpriteDemo.h"
#include "SpriteDemo.h"
#include "BitmaskDemo.h"
#include "BitmapDemo.h"
#include "CloneDemo.h"


// Process scheduler.
TS::Scheduler SchedulerBase{};
//

// Forward declare the used communications hardware.
TwoWire& WireInstance(Wire);
#if ARDUINO_MAPLE_MINI
Egfx::SpiType SpiInstance(1);
#endif

// Uncomment Driver and matching framebuffer type. Drivers will have Async, DMA, and RTOS variants, depending on the platform.
//ScreenDriverSSD1306_64x32x1_I2C ScreenDriver(WireInstance);
//ScreenDriverSSD1306_64x48x1_I2C ScreenDriver(WireInstance);
//ScreenDriverSSD1306_72x40x1_I2C ScreenDriver(WireInstance);
//ScreenDriverSSD1306_128x32x1_I2C ScreenDriver(WireInstance);
//ScreenDriverSSD1306_128x64x1_I2C_Rtos<> ScreenDriver(WireInstance);
//ScreenDriverSSD1306_128x64x1_SPI<TFT_CS, TFT_DC, TFT_RST> ScreenDriver(SpiInstance);
//ScreenDriverSH1106_128x64x1_SPI_Dma<TFT_CS, TFT_DC, TFT_RST> ScreenDriver(SpiInstance);
//using FrameBufferType = BinaryFrameBuffer<ScreenDriver.ScreenWidth, ScreenDriver.ScreenHeight>;

//ScreenDriverSSD1331_96x64x8_SPI<TFT_CS, TFT_DC, TFT_RST> ScreenDriver(SpiInstance);
//using FrameBufferType = Color8FrameBuffer<ScreenDriver.ScreenWidth, ScreenDriver.ScreenHeight>;

//ScreenDriverSSD1331_96x64x16_SPI<TFT_CS, TFT_DC, TFT_RST> ScreenDriver(SpiInstance);
//ScreenDriverSSD1351_128x128x16_SPI<TFT_CS, TFT_DC, TFT_RST> ScreenDriver(SpiInstance);
//ScreenDriverST7789_240x240x16_SPI<TFT_CS, TFT_DC, TFT_RST> ScreenDriver(SpiInstance);
//ScreenDriverST7735S_80x160x16_SPI<TFT_CS, TFT_DC, TFT_RST> ScreenDriver(SpiInstance);
//using FrameBufferType = Color16FrameBuffer<ScreenDriver.ScreenWidth, ScreenDriver.ScreenHeight>;


// The layout of drawers can be set independently of screen dimensions.
using FullLayout = LayoutElement<0, 0, FrameBufferType::FrameWidth, FrameBufferType::FrameHeight>;

// In-memory frame-buffer.
#if defined(USE_DYNAMIC_FRAME_BUFFER)
uint8_t* Buffer = nullptr;
#else
uint8_t Buffer[FrameBufferType::BufferSize]{};
#endif
FrameBufferType FrameBuffer(Buffer);

#if defined(ARDUINO_ARCH_AVR)
static constexpr uint32_t FramePeriod = 33333;
#else
static constexpr uint32_t FramePeriod = 16665;
#endif
GraphicsEngineTask GraphicsEngine(&SchedulerBase, &FrameBuffer, &ScreenDriver, FramePeriod);
//

// Drawer demos.
PrimitiveDemo<FullLayout> PrimitiveDemoDrawer{};
TransformDemo<FullLayout> TransformDemoDrawer{};
BitmaskDemo<FullLayout> BitmaskDemoDrawer{};


#if defined(ARDUINO_ARCH_AVR) 
SpriteDemo<FullLayout> SpriteDemoDrawer{};
NumberAlignmentDemo<FullLayout> NumberAlignmentDemoDrawer{};
#else
TextSpriteDemo<FullLayout> TextSpriteDemoDrawer{};
BitmapDemo<FullLayout> BitmapDemoDrawer{};
SpriteDemo<FullLayout> SpriteDemoDrawer{};
TextCharactersDemo TextCharactersDemoDrawer{};
CloneDemo<FullLayout> CloneDemoDrawer{};
NumberAlignmentDemo<FullLayout> NumberAlignmentDemoDrawer{};
#endif
//

// Demo Cycler task.
DemoCyclerTask<7000> DemoCycler(&SchedulerBase, &GraphicsEngine,
	&PrimitiveDemoDrawer
#if defined(ARDUINO_ARCH_AVR)
	, & TransformDemoDrawer
	, & BitmaskDemoDrawer
	, & SpriteDemoDrawer
	, & NumberAlignmentDemoDrawer
#else
	, & TextCharactersDemoDrawer
	, & TextSpriteDemoDrawer
	, & SpriteDemoDrawer
	, & PrimitiveDemoDrawer
	, & CloneDemoDrawer
	, & BitmapDemoDrawer
	, & NumberAlignmentDemoDrawer
#endif
);


#if defined(GRAPHICS_ENGINE_MEASURE)
EngineLogTask<500> EngineLog(SchedulerBase, GraphicsEngine);
#endif

void halt()
{
#if defined(USE_DYNAMIC_FRAME_BUFFER)
	delete[] Buffer;
#endif
#if defined(DEBUG)
	Serial.println(F("Screen Setup Failed."));
#endif
	while (true)
		;
}

void setup()
{
#if defined(DEBUG)
	Serial.begin(SERIAL_BAUD_RATE);
	while (!Serial)
		;
	delay(1000);
#endif

#if defined(USE_DYNAMIC_FRAME_BUFFER)
	// Allocate memory and set frame buffer.
	Buffer = new uint8_t[FrameBufferType::BufferSize]{};
	FrameBuffer.SetBuffer(Buffer);
#endif

	// Initialize comms hardware.
	WireInstance.begin();
	SpiInstance.begin();

	// Optional callback for RTOS driver variants.
	GraphicsEngine.SetBufferTaskCallback(BufferTaskCallback);

	// Set the Display Sync Type.
	GraphicsEngine.SetSyncType(DisplaySyncType::VSync);

	if (!GraphicsEngine.Start())
	{
		halt();
	}

#if defined(DEBUG)
	Serial.println(F("Graphics Engine Demo Start."));
	Serial.print(FrameBuffer.GetColorDepth());
	if (FrameBuffer.IsMonochrome())
	{
		Serial.println(F(" bit monochrome screen."));
	}
	else
	{
		Serial.println(F(" bit color screen."));
	}


	Serial.print(F("EGFX_PLATFORM_BIG "));
#if defined(EGFX_PLATFORM_BIG)
	Serial.println(1);
#else
	Serial.println(0);
#endif

	Serial.print(F("EGFX_PLATFORM_HDR "));
#if defined(EGFX_PLATFORM_HDR)
	Serial.println(1);
#else
	Serial.println(0);
#endif
#endif
}

void loop()
{
	SchedulerBase.execute();
}

void BufferTaskCallback(void* parameter)
{
	GraphicsEngine.BufferTaskCallback(parameter);
}