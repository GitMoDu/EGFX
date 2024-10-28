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

#if defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32)
#define TFT_SPI		1
#define TFT_DC		PB0
#define TFT_RST     PB1
#define TFT_CS		PA4
#define TFT_CLK		PA5
#define TFT_MOSI	PA7
#define TFT_SPI_HZ	40000000
#define TFT_I2C		0
#define TFT_SCL		PB6
#define TFT_SDA		PB7
#define TFT_I2C_HZ	1000000
#elif defined(ARDUINO_ARCH_STM32F1)
#define TFT_SPI		1
#define TFT_CS		7
#define TFT_DC		8
#define TFT_RST     9
#define TFT_CLK		UINT8_MAX
#define TFT_MOSI	UINT8_MAX
#define TFT_SPI_HZ	F_CPU/2
#define TFT_I2C		1
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
#elif defined(ARDUINO_ARCH_RP2040)
#define TFT_CS		19
#define TFT_DC		20
#define TFT_RST		21
#define TFT_SPI		1
#define TFT_CLK		26
#define TFT_MOSI	27
#define TFT_SPI_HZ	F_CPU/2
#define TFT_I2C		1
#define TFT_SCL		15
#define TFT_SDA		14
#define TFT_I2C_HZ	1000000
#elif defined(ARDUINO_ARCH_NRF52)
#define TFT_SPI		0
#define TFT_CS		7
#define TFT_DC		4
#define TFT_RST		5
#define TFT_CLK		PIN_SPI_SCK
#define TFT_MOSI	PIN_SPI_MOSI
#define TFT_SPI_HZ	F_CPU/2
#define TFT_I2C		0
#define TFT_SCL		PIN_WIRE_SCL
#define TFT_SDA		PIN_WIRE_SDA
#define TFT_I2C_HZ	400000
#endif

#define _TASK_OO_CALLBACKS
#include <TScheduler.hpp>

#include <ArduinoGraphicsDrivers.h>
#include <ArduinoGraphicsEngineTask.h>
#include <ArduinoGraphicsCore.h>
#include <ArduinoGraphicsDrawers.h>


#include "DemoCyclerTask.h"
#include "PrimitiveDemo.h"
#include "TextCharactersDemo.h"
#include "NumberAlignmentDemo.h"
#include "TextFlowDemo.h"
#include "SpriteDemo.h"
#include "TransformDemo.h"
#include "BitmapDemo.h"
#include "BitmaskDemo.h"
#include "TextSpriteDemo.h"
#include "CloneDemo.h"


// Process scheduler.
TS::Scheduler SchedulerBase;
//

// Uncomment Driver and matching framebuffer type.
//ScreenDriverSSD1306_64x32x1_I2C_Async<TFT_SCL, TFT_SDA, TFT_RST, TFT_I2C, TFT_I2C_HZ> ScreenDriver{};
//ScreenDriverSSD1306_64x48x1_I2C<TFT_SCL, TFT_SDA, TFT_RST, TFT_I2C, TFT_I2C_HZ> ScreenDriver{};
//ScreenDriverSSD1306_72x40x1_I2C<TFT_SCL, TFT_SDA, TFT_RST, TFT_I2C, TFT_I2C_HZ> ScreenDriver{};
//ScreenDriverSSD1306_128x64x1_I2C<TFT_SCL, TFT_SDA, TFT_RST, TFT_I2C, TFT_I2C_HZ> ScreenDriver{};
//ScreenDriverSSD1306_128x64x1_SPI<TFT_DC, TFT_CS, TFT_RST, TFT_CLK, TFT_MOSI, TFT_SPI, TFT_SPI_HZ> ScreenDriver{};
//using FrameBufferType = BinaryFrameBuffer<ScreenDriver.ScreenWidth, ScreenDriver.ScreenHeight>;

//ScreenDriverSSD1331_96x64x8_SPI_Dma<TFT_DC, TFT_CS, TFT_RST, TFT_CLK, TFT_MOSI, TFT_SPI, TFT_SPI_HZ> ScreenDriver{};
//using FrameBufferType = Color8FrameBuffer<ScreenDriver.ScreenWidth, ScreenDriver.ScreenHeight>;


//ScreenDriverSSD1331_96x64x16_SPI<TFT_DC, TFT_CS, TFT_RST, TFT_CLK, TFT_MOSI, TFT_SPI, TFT_SPI_HZ> ScreenDriver{};
//ScreenDriverSSD1351_128x128x16_SPI<TFT_DC, TFT_CS, TFT_RST, TFT_CLK, TFT_MOSI, TFT_SPI, TFT_SPI_HZ> ScreenDriver{};
//ScreenDriverST7789_240x240x16_SPI<TFT_DC, TFT_CS, TFT_RST, TFT_CLK, TFT_MOSI, TFT_SPI, TFT_SPI_HZ> ScreenDriver{};
//ScreenDriverST7735S_160x80x16_SPI<TFT_DC, TFT_CS, TFT_RST, TFT_CLK, TFT_MOSI, TFT_SPI, TFT_SPI_HZ> ScreenDriver{};
using FrameBufferType = Color16FrameBuffer<ScreenDriver.ScreenWidth, ScreenDriver.ScreenHeight>;


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
BitmaskDemo<FullLayout> BitmaskDemoDrawer(&FrameBuffer);
#if defined(ARDUINO_ARCH_AVR) 
#if !defined(DEBUG) && !defined(GRAPHICS_ENGINE_MEASURE)
TransformDemo<FullLayout> TransformDemoDrawer(&FrameBuffer);
SpriteDemo<FullLayout> SpriteDemoDrawer(&FrameBuffer);
TextCharactersDemo TextCharactersDemoDrawer(&FrameBuffer);
PrimitiveDemo<FullLayout> PrimitiveDemoDrawer(&FrameBuffer);
#endif
#else
TransformDemo<FullLayout> TransformDemoDrawer(&FrameBuffer);
NumberAlignmentDemo<FullLayout> NumberAlignmentDemoDrawer(&FrameBuffer);
SpriteDemo<FullLayout> SpriteDemoDrawer(&FrameBuffer);
TextCharactersDemo TextCharactersDemoDrawer(&FrameBuffer);
PrimitiveDemo<FullLayout> PrimitiveDemoDrawer(&FrameBuffer);
BitmapDemo<FullLayout> BitmapDemoDrawer(&FrameBuffer);
TextSpriteDemo<FullLayout> TextSpriteDemoDrawer(&FrameBuffer);
CloneDemo<FullLayout> CloneDemoDrawer(&FrameBuffer);
#endif
//


using FpsLayout = LayoutElement<0, 0, FrameBufferType::FrameWidth, FrameBufferType::FrameHeight>;
DisplayFpsDrawer<FpsLayout,
	SpriteShader::ColorShader<SpriteFont3x5Renderer>,
	//SpriteShaderEffect::BrightnessEffect<SpriteFont2x5Renderer>,
	FpsDrawerPosition::TopRight>
	FpsDrawer(&FrameBuffer, &GraphicsEngine);

// Demo Cycler task.
DemoCyclerTask<8000> DemoCycler(&SchedulerBase, &GraphicsEngine, &FpsDrawer,
	&BitmaskDemoDrawer
#if defined(ARDUINO_ARCH_AVR) 
#if !defined(DEBUG) && !defined(GRAPHICS_ENGINE_MEASURE)
	, &TransformDemoDrawer
	, & TextCharactersDemoDrawer
	, & PrimitiveDemoDrawer
	, & SpriteDemoDrawer
#endif
#else
	, &TransformDemoDrawer
	, &NumberAlignmentDemoDrawer
	, & TextCharactersDemoDrawer
	, & TextSpriteDemoDrawer
	, & SpriteDemoDrawer
	, & PrimitiveDemoDrawer
	, & CloneDemoDrawer
	, & BitmapDemoDrawer
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

void BufferTaskCallback(void* parameter)
{
	GraphicsEngine.BufferTaskCallback(parameter);
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
	Serial.println(F("Graphics Engine Demo Start."));
	Serial.print(FrameBuffer.GetColorDepth());
	Serial.println(F(" bit color depth screen."));
#endif
}

void loop()
{
	SchedulerBase.execute();
}

