#define DEBUG
#define SERIAL_BAUD_RATE 115200

//#define USE_DYNAMIC_FRAME_BUFFER // Enable dynamic allocation frame buffer.
#define GRAPHICS_ENGINE_DEBUG
#define GRAPHICS_ENGINE_MEASURE // Required for this demo.


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
#define TFT_SPI_HZ	48000000
#define TFT_I2C		1
#define TFT_SCL		15
#define TFT_SDA		14
#define TFT_I2C_HZ	1000000
#endif

#define _TASK_OO_CALLBACKS
#include <TScheduler.hpp>

#include <ArduinoGraphicsDrivers.h>
#include <ArduinoGraphicsDrawers.h>
#include <ArduinoGraphicsEngineTask.h>

#include "LineUnitTester.h"

// Process scheduler.
TS::Scheduler SchedulerBase;
//

// Screen Driver and frame-buffer definition.
ScreenDriverSSD1306_64x48x1_I2C<TFT_SCL, TFT_SDA, TFT_RST, TFT_I2C, TFT_I2C_HZ> ScreenDriver{};
//ScreenDriverSSD1306_128x64x1_I2C<TFT_SCL, TFT_SDA, TFT_RST, TFT_I2C, TFT_I2C_HZ> ScreenDriver{};

using FrameBufferType = BinaryFrameBuffer<ScreenDriver.ScreenWidth, ScreenDriver.ScreenHeight>;

// In-memory frame-buffer.
#if defined(USE_DYNAMIC_FRAME_BUFFER)
uint8_t* Buffer = nullptr;
#else
uint8_t Buffer[FrameBufferType::BufferSize]{};
#endif
FrameBufferType FrameBuffer(Buffer);

// Engine Task.
GraphicsEngineTask GraphicsEngine(&SchedulerBase, &FrameBuffer, &ScreenDriver, 10000);

// Log to Serial task.
#if defined(GRAPHICS_ENGINE_MEASURE)
EngineLogTask<1000> EngineLog(&SchedulerBase, &GraphicsEngine);
#endif

// Visual Unit Tests.
LineUnitTester LineUnitTest(&FrameBuffer);


void halt()
{
#if defined(USE_DYNAMIC_FRAME_BUFFER)
	delete[] Buffer;
#endif
#ifdef DEBUG
	Serial.println(F("Setup Failed."));
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

	GraphicsEngine.SetDrawer(&LineUnitTest);

	if (!GraphicsEngine.Start())
	{
		halt();
	}
}

void loop()
{
	SchedulerBase.execute();
}

void BufferTaskCallback(void* parameter)
{
	GraphicsEngine.BufferTaskCallback(parameter);
}