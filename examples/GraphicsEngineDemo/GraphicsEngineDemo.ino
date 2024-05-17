/*
* Graphics Engine Demo.
* Includes configurations for multiple screens (uncomment screen driver).
* Frame buffer can be allocated dinamically (USE_DYNAMIC_FRAME_BUFFER).
*/

#define DEBUG
//#define USE_DYNAMIC_FRAME_BUFFER // Enable dynamic allocation frame buffer.
#define GRAPHICS_ENGINE_MEASURE // Enabled engine measuring and logging.
//#define GRAPHICS_ENGINE_DEBUG // Enable debug logging and coordinates validation.
#define SERIAL_BAUD_RATE 115200

#if defined(ARDUINO_ARCH_STM32F1)
#define TFT_SPI		1
#define TFT_CS		7
#define TFT_DC		8
#define TFT_RST     9
#define TFT_CLK		UINT8_MAX
#define TFT_MOSI	UINT8_MAX
#define TFT_I2C		1
#define TFT_SPI_HZ	F_CPU/8
#define TFT_SCL		UINT8_MAX
#define TFT_SDA		UINT8_MAX
#define TFT_I2C_HZ	1000000
#elif defined(ARDUINO_ARCH_AVR)
#define TFT_SPI		0
#define TFT_SPI_HZ	0
#define TFT_CS		10
#define TFT_DC		9
#define TFT_RST		8
#define TFT_CLK		UINT8_MAX
#define TFT_MOSI	UINT8_MAX
#define TFT_I2C		UINT8_MAX
#define TFT_SCL		UINT8_MAX
#define TFT_SDA		UINT8_MAX
#define TFT_I2C_HZ	1000000
#elif defined(ARDUINO_ARCH_ESP32)
#define TFT_SPI		0
#define TFT_SPI_HZ	48000000
#define TFT_CS		19
#define TFT_DC		20
#define TFT_RST		21
#define TFT_CLK		26
#define TFT_MOSI	48
#define TFT_I2C		1
#define TFT_I2C_HZ	1000000
#define TFT_SCL		18
#define TFT_SDA		17
#endif

#define _TASK_OO_CALLBACKS
#define _TASK_SLEEP_ON_IDLE_RUN // Enable 1 ms SLEEP_IDLE powerdowns between tasks if no callback methods were invoked during the pass.
#include <TaskScheduler.h>

#include <ArduinoGraphicsDrivers.h>
#include <ArduinoGraphicsEngineTask.h>


#include "DemoCyclerTask.h"
#include "PrimitiveDemo.h"
#include "TextCharactersDemo.h"
#include "TextAlignmentDemo.h"
#include "TextFlowDemo.h"
#include "SpriteDemo.h"
#include "TransformDemo.h"
#include "BitmapDemo.h"
#include "TextSpriteDemo.h"


#include "CloneDemo.h"

// Process scheduler.
Scheduler SchedulerBase;
//

// Uncomment Driver and matching framebuffer type.
//ScreenDriverSSD1306_64x32x1_I2C_Async<TFT_SCL, TFT_SDA, TFT_RST, TFT_I2C, TFT_I2C_HZ> ScreenDriver{};
//ScreenDriverSSD1306_64x48x1_I2C<TFT_SCL, TFT_SDA, TFT_RST, TFT_I2C, TFT_I2C_HZ> ScreenDriver{};
//ScreenDriverSSD1306_72x40x1_I2C<TFT_SCL, TFT_SDA, TFT_RST, TFT_I2C, TFT_I2C_HZ> ScreenDriver{};
//ScreenDriverSSD1306_128x64x1_I2C<TFT_SCL, TFT_SDA, TFT_RST, TFT_I2C, TFT_I2C_HZ> ScreenDriver{};
//ScreenDriverSSD1306_128x64x1_SPI<TFT_DC, TFT_CS, TFT_CLK, TFT_MOSI, TFT_RST, TFT_SPI, TFT_SPI_HZ> ScreenDriver{};
using FrameBufferType = MonochromeFrameBuffer<ScreenDriver.ScreenWidth, ScreenDriver.ScreenHeight>;

//ScreenDriverSSD1331_96x64x8_SPI<TFT_DC, TFT_CS, TFT_CLK, TFT_MOSI, TFT_RST, TFT_SPI, TFT_SPI_HZ> ScreenDriver{};
//using FrameBufferType = Color8FrameBuffer<ScreenDriver.ScreenWidth, ScreenDriver.ScreenHeight>;

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
static constexpr uint32_t FramePeriod = 16666;
#endif
GraphicsEngineTask GraphicsEngine(&SchedulerBase, &FrameBuffer, &ScreenDriver, FramePeriod);
//

// Drawer demos.
SpriteDemo SpriteDemoDrawer(&FrameBuffer);
TransformDemo TransformDemoDrawer(&FrameBuffer);
TextAlignmentDemo TextAlignmentDemoDrawer(&FrameBuffer);
BitmapDemo BitmapDemoDrawer(&FrameBuffer);
#if defined(ARDUINO_ARCH_AVR)
#if !defined(GRAPHICS_ENGINE_MEASURE)
PrimitiveDemo PrimitiveDemoDrawer(&FrameBuffer);
#endif
#if !defined(DEBUG)
CloneDemo CloneDemoDrawer(&FrameBuffer);
#endif
#else
PrimitiveDemo PrimitiveDemoDrawer(&FrameBuffer);
TextCharactersDemo TextCharactersDemoDrawer(&FrameBuffer);
TextSpriteDemo TextSpriteDemoDrawer(&FrameBuffer);
CloneDemo CloneDemoDrawer(&FrameBuffer);
#endif


DemoCyclerTask<7000> DemoCycler(&SchedulerBase, &GraphicsEngine,
	&SpriteDemoDrawer,
	&TransformDemoDrawer,
	&TextAlignmentDemoDrawer,
	&BitmapDemoDrawer
#if defined(ARDUINO_ARCH_AVR) 
#if !defined(GRAPHICS_ENGINE_MEASURE)
	, & PrimitiveDemoDrawer
#endif
#if !defined(DEBUG)
	, & CloneDemoDrawer
#endif
#else
	, & PrimitiveDemoDrawer
	, & CloneDemoDrawer
	, & TextSpriteDemoDrawer
#endif
);
//

#if defined(GRAPHICS_ENGINE_MEASURE)
EngineLogTask<500> EngineLog(&SchedulerBase, &GraphicsEngine);
#endif

void halt()
{
#if defined(USE_DYNAMIC_FRAME_BUFFER)
	delete[] Buffer;
#endif
#ifdef DEBUG
	Serial.println(F("Screen Test Setup Failed."));
#endif
	while (true)
		;
}

void setup()
{
#ifdef DEBUG
	Serial.begin(SERIAL_BAUD_RATE);
	while (!Serial)
		;
	delay(1000);
#endif

#if defined(USE_DYNAMIC_FRAME_BUFFER)
	Buffer = new uint8_t[FrameBufferType::BufferSize]{};
	FrameBuffer.SetBuffer(Buffer);
#endif

	GraphicsEngine.SetBufferTaskCallback(BufferTaskCallback);
	GraphicsEngine.SetInverted(false);

	if (!GraphicsEngine.Start())
	{
		halt();
	}

#ifdef DEBUG
	Serial.println(F("Screen Test Setup OK"));
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