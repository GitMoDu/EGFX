# EGFX - Embedded Graphics Framework

[![License](https://img.shields.io/github/license/GitMoDu/EGFX)](LICENSE)

A high-performance, cooperative-task graphics engine for memory-constrained embedded systems, targeting Arduino-compatible microcontrollers. 

## Overview

EGFX is a frame-buffer-based graphics framework designed for embedded systems with limited resources. It provides a complete graphics pipeline with hardware-accelerated display drivers, efficient rendering primitives, and a task-based architecture that prevents CPU hogging while maintaining smooth frame rates.

### Key Features

- **🎨 Frame-Buffer Architecture**: Full-frame rendering with monochrome (1-bit), low-color (8-bit), and full-color (16-bit RGB565) support
- **⚡ Cooperative Task Scheduling**: Non-blocking draw calls using [TaskScheduler](https://github.com/arkhipenko/TaskScheduler) to maintain system responsiveness
- **📊 Smart Frame Management**: Automatic frame synchronization, frame skip detection, and idle power saving
- **🖥️ Multiple Display Support**: Ready-to-use drivers for popular OLED and LCD screens (SSD1306, SSD1331, SSD1351, ST7735, ST7789, and more)
- **🎭 Rich Graphics Features**: Sprites, vector fonts, bitmask fonts, transforms, shaders, and primitive drawing
- **🔧 Memory Flexible**: Static or dynamic frame-buffer allocation with optional double-buffering
- **📈 Performance Monitoring**: Real-time metrics for FPS, render time, push duration, and draw call analysis

### Dependencies

- [TaskScheduler](https://github.com/arkhipenko/TaskScheduler) - Cooperative task scheduling

### Basic Example

```cpp
#include <TScheduler.hpp>
#include <EgfxCore.h>
#include <EgfxDisplayEngine.h>
#include "DisplayConfiguration.h"

// Task scheduler
TS:: Scheduler scheduler;

// Frame buffer
uint8_t buffer[/* calculated size */];
Egfx::BinaryFramebuffer<128, 64> framebuffer(buffer);

// Screen driver (example:  SSD1306 OLED)
Egfx::ScreenDriverSSD1306_128x64x1_I2C screenDriver(Wire);

// Display engine
Egfx::DisplayEngineTask<decltype(framebuffer), decltype(screenDriver)> 
    displayEngine(scheduler, framebuffer, screenDriver);

void setup() {
    Wire.begin();
    displayEngine.Start();
    displayEngine.SetSyncType(Egfx::DisplaySyncType:: Vrr);
}

void loop() {
    scheduler.execute();
}
```

## 🖥️ Architecture

### Engine State Flow

```
┌──────────┐      ┌───────────┐      ┌─────────┐      ┌──────────┐
│  Clear   │ ───> │  Render   │ ───> │  Sync   │ ───> │   Push   │
│  Buffer  │      │Draw Calls │      │         │      │to Screen │
└──────────┘      └───────────┘      └─────────┘      └──────────┘
     │                                                        │
     └────────────────────────────────────────────────────────┘
```

### Key Concepts

**Draw Calls**: Each visual element is rendered in a separate scheduler pass to avoid blocking the CPU.  The engine automatically manages draw call sequencing.

**Frame Synchronization**: The engine maintains target FPS with automatic frame skip detection and compensation.

**Memory Efficiency**: Frame buffers require enough RAM for a full frame but can be statically allocated or dynamically managed based on your platform's capabilities.

**Note**: Frame-buffer rendering requires sufficient RAM to hold a complete frame.  Ensure your MCU has adequate memory for your chosen resolution and color depth. 

### Display Drivers

EGFX includes optimized drivers with optional DMA, Async, and RTOS variants:

| Display | Resolution | Color Depth | Interface |
|---------|------------|-------------|-----------|
| **SSD1306** | 64×32, 64×48, 72×40, 128×64 | Monochrome | I2C, SPI |
| **SH1106** | 128×64 | Monochrome | SPI |
| **SSD1331** | 96×64 | 8-bit, 16-bit | SPI |
| **SSD1351** | 128×128 | 16-bit | SPI |
| **ST7735** | 160×80 | 16-bit | SPI |
| **ST7789** | 240×240 | 16-bit | SPI |

### Graphics Primitives

- **Shapes**: Point, Line, Rectangle, Filled Rectangle, Triangle
- **Text**: Vector fonts (scalable) and bitmask fonts (fast)
- **Sprites**: Flash-based bitmask and RGB565 bitmap sprites
- **Transforms**: Rotation, scaling, skewing, translation
- **Shaders**: Color effects, gradients, transparency, brightness

### Text Rendering

**Vector Fonts**: Dynamically scalable fonts with configurable width, height, and kerning
- Compact encoded vector format.
- Epoxy font family available with full ASCII set.

**Code Fonts**: Dynamically scalable fonts with configurable width, height, and kerning
- Hard-coded vector drawing with cached intermediates.
- CodeFont family (RawBot) available with partial ASCII coverage.

**Bitmask Fonts**:  Fixed size font, with configurable kerning.
- Fast, pixel-perfect rendering with optional integer scaling
- Plastic font family (3×5, 5×5)
- Micron font family, for RGB sub-pixel font (1×5, 2×5)
- Low memory footprint

## 📊 Performance Monitoring

Enable real-time performance metrics with `PerformanceLogTask`:

```cpp
// Enable performance monitoring
#define EGFX_PERFORMANCE_LOG
#define EGFX_PERFORMANCE_LOG_DETAIL

Egfx::PerformanceLogTask<2000> engineLog(scheduler, displayEngine);
engineLog.Start();
```

Tracks:
- Display FPS
- Draw call count and longest duration
- Frame-buffer clear duration
- Render duration and CPU load
- Push duration and occupancy
- Idle duration (power saving)

## 🔧 Configuration

### Platform Support

EGFX supports multiple architectures with platform-specific optimizations: 
- AVR (Arduino Uno, Mega, etc.)
- ARM Cortex-M (STM32F1, STM32F4, STM32H7)
- ESP32
- RP2040/RP2350
- nRF52


## Example Demos

The [EngineDemo](examples/EngineDemo/) is the recommended starting point. Each demo is automatically cycled and demonstrates best practices for implementing custom drawers:

1. **PrimitiveDemo**: Animated lines, rectangles, and triangles
2. **VectorTextDemo**: Dynamic text with vector fonts and animated scaling
3. **BitmaskTextDemo**: Fast text rendering with multiple scales
4. **SpriteTransformDemo**: Sprite rotation, skewing, and color effects
5. **BitmapDemo**: Full-color bitmap rendering with rotation and brightness (32-bit platforms)

## Getting Started with EngineDemo

1. Open `DisplayConfiguration.h` and select your screen type
2. Configure pin definitions for your platform
3. Upload to your board
4. Watch as demos automatically cycle every 10 seconds

The demo includes extensive comments and shows how to: 
- Set up the display engine
- Create custom drawers
- Implement frame-based animations
- Handle different color depths
- Optimize for your platform


