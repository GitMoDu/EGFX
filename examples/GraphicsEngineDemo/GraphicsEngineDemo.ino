/*
* Graphics Engine Demo.
* Includes configurations for multiple screens (uncomment screen driver).
* Can run in FrameBuffer or DirectDraw mode with the same engine (USE_FRAME_BUFFER).
* Frame buffer can be allocated dinamically (USE_DYNAMIC_FRAME_BUFFER).
* IFrameDraw demos can handle DirectDraw with selective screen clearing.
*/

#define USE_FRAME_BUFFER // Select FrameBuffer or DirectDraw versions.
#if defined(USE_FRAME_BUFFER)
//#define USE_DYNAMIC_FRAME_BUFFER // Enable dynamic allocation frame buffer.
#endif
#define DEBUG
#define SERIAL_BAUD_RATE 115200

#if defined(ARDUINO_ARCH_STM32F1)
#define TFT_DC		8
#define TFT_RST     7

#define TFT_CS		9
#define TFT_SPI		1

#define TFT_I2C		1
#define TFT_SCL		-1
#define TFT_SDA		-1

#elif defined(ARDUINO_ARCH_AVR)
#define TFT_DC		6
#define TFT_RST		7

#define TFT_CS		10
#define TFT_SPI		0

#define TFT_I2C		0
#define TFT_SCL		-1
#define TFT_SDA		-1

#elif defined(ARDUINO_ARCH_ESP32)
#define TFT_DC		47
#define TFT_RST		21

#define TFT_CS		48
#define TFT_SPI		2

#define TFT_I2C		1
#define TFT_SCL		18
#define TFT_SDA		17
#endif


#define _TASK_OO_CALLBACKS
#define _TASK_SLEEP_ON_IDLE_RUN // Enable 1 ms SLEEP_IDLE powerdowns between tasks if no callback methods were invoked during the pass.
#include <TaskScheduler.h>

#include <ArduinoGraphicsCore.h>
#include <GraphicsEngineTask.h>

#include "PrimitiveDemo.h"
#include "TextCharactersDemo.h"
#include "TextAlignmentDemo.h"
#include "TextFlowDemo.h"

#include "DemoCyclerTask.h"
#include "EngineLogTask.h"

#if defined(USE_FRAME_BUFFER)
// SPI SSD1331 96x64x8
using pixel_t = SSD1331PixelType;
DriverSSD1331_96_64<TFT_SPI, TFT_CS, TFT_DC, TFT_RST> ScreenDriver{};

// SPI SSD1351 128x128x16
// using pixel_t = SSD1351PixelType;
//DriverSSD1351_128_128<TFT_SPI, TFT_CS, TFT_DC, TFT_RST> ScreenDriver{};
#else
// I2C SSD1306 128x64x1
ScreenSSD1306_128_64<TFT_I2C, TFT_SCL, TFT_SDA, TFT_RST> ScreenDriver{};

// SPI SSD1331 96x64x8
//ScreenSSD1331_96_64<TFT_SPI, TFT_CS, TFT_DC, TFT_RST> ScreenDriver{};

// SPI SSD1331 128x128x16
//ScreenSSD1351_128_128<TFT_SPI, TFT_CS, TFT_DC, TFT_RST> ScreenDriver{};

// SPI ST7789 240x240x16
//ScreenST7789_240_240<TFT_SPI,TFT_CS, TFT_DC, TFT_RST> ScreenDriver{};
#endif

#if defined(USE_FRAME_BUFFER)
#if defined(USE_DYNAMIC_FRAME_BUFFER)
#error Dynamic frame buffer allocation not implemented.
#else
// Frame buffer with static allocation, based on Screen dimensions and pixel type.
pixel_t Buffer[ScreenDriver.ScreenWidth][ScreenDriver.ScreenHeight]{};
#endif
// Driver provides color conversion to display format.
FrameBufferPrimitives<pixel_t> FrameBuffer((pixel_t*)Buffer, ScreenDriver.ScreenWidth, ScreenDriver.ScreenHeight, &ScreenDriver);
IFramePrimitives* Frame = &FrameBuffer;
//
#else
// Screen Buffer drivers implement both interfaces.
IFramePrimitives* Frame = &ScreenDriver;
#endif

// 
// Calibration output renderer.
PrimitiveDemo PrimitiveDemoDrawer(Frame);
TextCharactersDemo TextCharactersDemoDrawer(Frame);
TextAlignmentDemo TextAlignmentDemoDrawer(Frame);
TextFlowDemo TextFlowDemoDrawer(Frame);
//

// Process scheduler.
Scheduler SchedulerBase;
//

// Graphic engine task.
#if defined(USE_FRAME_BUFFER)
const uint32_t FramePeriod = 15000;
#else
const uint32_t FramePeriod = 500000;
#endif
GraphicsEngineTask GraphicsEngine(&SchedulerBase, Frame, &ScreenDriver, FramePeriod);
//

// Demo cycler task.
DemoCyclerTask<15000> DemoCycler(&SchedulerBase, &GraphicsEngine,
	&PrimitiveDemoDrawer,
	&TextCharactersDemoDrawer,
	&TextAlignmentDemoDrawer
);
//

EngineLogTask EngineLog(&SchedulerBase, &GraphicsEngine);

void halt()
{
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
