# EGFX - Embedded Graphics Framework

[![License](https://img.shields.io/github/license/GitMoDu/EGFX)](LICENSE)

EGFX is a frame-buffer-based graphics framework designed for embedded systems with limited resources, targeting Arduino-compatible microcontrollers.
It provides a complete graphics pipeline with hardware-accelerated display drivers, efficient rendering primitives, and a task-based architecture that avoids CPU hogging.

![Embedded Graphics Framework](https://github.com/GitMoDu/EGFX/blob/master/media/logo_splash_multi_screen.gif)

## Overview

![Library Organization](https://github.com/GitMoDu/EGFX/blob/master/media/high_level_organization.png)

The library is organized into five main components:

| Component | Description |
|-----------|-------------|
| **[Framebuffer](src/Framebuffer/)** | Manages screen memory and rendering operations |
| **[ScreenDrivers](src/ScreenDrivers/)** | Handles display communication over SPI/I2C with optional DMA, Async, and RTOS support |
| **[DisplayEngine](src/DisplayEngine/)** | Orchestrates the complete graphics pipeline—managing the asynchronous draw cycle, frame timing, buffer flipping, and pushing frames to the display |
| **[Framework](src/Framework/)** | Template-driven compile-time toolkit for building UIs: Drawables, Views, Layouts, Shaders, Fonts, and Text |
| **[Modules](src/Modules/)** | Self-contained, reusable views built on top of the Framework |

### Key Features

- **🎨 Frame-Buffer Architecture** — Full-frame rendering with monochrome (1-bit), low-color (8-bit), and full-color (16-bit RGB565) support
- **⚡ Cooperative Task Scheduling** — Non-blocking draw calls using [TaskScheduler](https://github.com/arkhipenko/TaskScheduler) to maintain system responsiveness
- **📊 Smart Frame Management** — Automatic frame synchronization, frame skip detection, and idle power saving
- **🖥️ Multiple Display Support** — Ready-to-use drivers for popular OLED and LCD screens (SSD1306, SH1106, SSD1331, SSD1351, ST7735, ST7789)
- **🧩 Compile-Time Framework** — Template-driven Drawables, Views, Layouts, and Shaders resolved entirely at compile time
- **🔤 Rich Text Rendering** — Vector fonts (scalable), code fonts (hard-coded vector), and bitmask fonts (pixel-perfect)
- **🔧 Memory Flexible** — Static or dynamic frame-buffer allocation with optional double-buffering
- **📈 Performance Monitoring** — Real-time metrics for FPS, render time, push duration, and draw call analysis

### Dependencies

- [TaskScheduler](https://github.com/arkhipenko/TaskScheduler) — Cooperative task scheduling

---

## 🧩 Framework

The Framework (`src/Framework/`) is EGFX's compile-time toolkit for building UI elements. It is built on two core contracts—**Drawables** and **Views**—and extends outward through Layouts, Shaders, graphics subsystems, and text rendering.

### Drawables

Drawables are **passive renderers**: they draw their current state to a framebuffer in a single `Draw()` call, without any animation logic.

```cpp
// Drawable contract
struct Drawable {
    void Draw(IFrameBuffer* frame) {}
};
```

All graphics subsystems (bitmask, bitmap, vector, code) produce Drawables.

### Views

Views **orchestrate animation and composition** over time. A View may require multiple `DrawCall()` invocations to complete a single frame cycle, enabling cooperative rendering without blocking the CPU.

```cpp
// View contract
struct View {
    bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) {
        return true; // true = cycle complete
    }
};
```

The Framework provides several View types:

| View Type | Purpose |
|-----------|---------|
| **`FrameAdapter`** | Wraps a View into the engine's `IFrameDraw` interface with enable/disable control |
| **`AbstractView`** | Base view with overridable `ViewStep()` for animation and sequential drawable dispatch |
| **`DrawablesView`** | Manages a compile-time pack of Drawables, rendering them sequentially across draw calls |
| **`CompositeView`** | Composes multiple child Views, advancing them sequentially within a single frame cycle |

### Layouts

The layout system computes positions and sizes **entirely at compile time**—zero runtime overhead.

| Layout | Description |
|--------|-------------|
| **`Grid`** | Divides a region into equal rows and columns |
| **`Weighted`** | Splits a region by proportional weights |
| **`Margin`** | Applies inner margins to a region |
| **`Align`** | Aligns an element within a region |
| **`ConstrainedDimensions`** | Constrains an element to min/max dimensions |
| **`ConstrainedRatio`** | Constrains an element to an aspect ratio |

### Shaders

The shader pipeline provides composable, template-driven effects:

| Shader Category | Examples |
|-----------------|----------|
| **Color Sources** | Checkerboard, Gradient, AxisSplit |
| **Color Shaders** | Invert, Brightness, Contrast, Saturation |
| **Transform Shaders** | Translate, Crop, Orient, ScaleDown, Skew, Rotate, DitherMask |
| **Pixel Shaders** | Per-pixel effects applied during rendering |
| **Primitive Shaders** | Effects applied to shape primitives |

### Graphics Subsystems

Each subsystem provides its own Model, Drawer, and Drawable templates:

| Subsystem | Description |
|-----------|-------------|
| **Bitmask** | 1-bit sprites with template drawer and drawable; supports both Flash (ROM) and RAM sources |
| **Bitmap** | Multi-color-mode bitmaps with template drawer and drawable; supports both Flash (ROM) and RAM sources |
| **Vector** | Encoded vector shapes with template drawer |
| **Code** | Hard-coded vector drawing with cached intermediates |

### Fonts & Text

Fonts are built on top of graphics subsystems. The text system is a **generic writer templated on any font drawer**.

| Font Type | Family | Description |
|-----------|--------|-------------|
| **Vector Font** | Epoxy (Full, Numbers) | Dynamically scalable, compact encoded vector format, configurable width/height/kerning |
| **Code Font** | RawBot | Hard-coded vector drawing with cached intermediates, configurable dimensions |
| **Bitmask Font** | Plastic (3×5, 5×5), Micron (1×5, 2×5) | Fixed-size pixel-perfect fonts with optional integer scaling, low memory footprint |

### Modules

Modules are **self-contained views** that bundle their own drawables, layouts, and assets. They are built entirely on the Framework and are ready to drop into any display engine setup.

| Module | Description |
|--------|-------------|
| **FpsDisplay** | Real-time FPS overlay |
| **EgfxLogo** | Animated EGFX logo splash |
| **TerminalWindow** | Scrollable text terminal view |

---

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

**Draw Calls**: Each visual element is rendered in a separate scheduler pass to avoid blocking the CPU. The engine automatically manages draw call sequencing.

**Frame Synchronization**: The engine maintains target FPS with automatic frame skip detection and compensation.

**Memory Efficiency**: Frame buffers can be statically allocated or dynamically managed based on your platform's capabilities.

> **Note**: Frame-buffer rendering requires sufficient RAM to hold a complete frame. Ensure your MCU has adequate memory for your chosen resolution and color depth.

### Display Drivers

EGFX includes optimized drivers with optional DMA, Async, and RTOS variants:

| Display | Color Depth | Interface |
|---------|-------------|-----------|
| **SSD1306** | Monochrome | I2C, SPI |
| **SH1106** | Monochrome | SPI |
| **SSD1331** | 8-bit, 16-bit | SPI |
| **SSD1351** | 16-bit | SPI |
| **ST7735** | 16-bit | SPI |
| **ST7789** | 16-bit | SPI |

---

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

---

## 🔧 Platform Support

EGFX supports multiple architectures with platform-specific optimizations:

| Architecture | Examples |
|-------------|----------|
| **AVR** | Arduino Uno, Mega |
| **ARM Cortex-M** | STM32F1, STM32F4, STM32H7 |
| **ESP32** | ESP32, ESP32-S3 |
| **RP2040/RP2350** | Raspberry Pi Pico |
| **nRF52** | nRF52840 |

---

## 🚀 Getting Started

### Basic Example

```cpp
#include <TScheduler.hpp>
#include <EgfxCore.h>
#include <EgfxDisplayEngine.h>
#include "DisplayConfiguration.h"

// Task scheduler
TS::Scheduler scheduler;

// Frame buffer
uint8_t buffer[/* calculated size */];
Egfx::BinaryFramebuffer<128, 64> framebuffer(buffer);

// Screen driver (example: SSD1306 OLED)
Egfx::ScreenDriverSSD1306_128x64x1_I2C screenDriver(Wire);

// Display engine
Egfx::DisplayEngineTask<decltype(framebuffer), decltype(screenDriver)>
    displayEngine(scheduler, framebuffer, screenDriver);

void setup() {
    Wire.begin();
    displayEngine.Start();
    displayEngine.SetSyncType(Egfx::DisplaySyncType::Vrr);
}

void loop() {
    scheduler.execute();
}
```

### Include Headers

| Header | Contents |
|--------|----------|
| `<EgfxCore.h>` | Core model, interfaces, and color types |
| `<EgfxFramebuffers.h>` | Framebuffer implementations |
| `<EgfxScreenDrivers.h>` | All screen driver variants |
| `<EgfxDisplayEngine.h>` | Display engine task |
| `<EgfxFramework.h>` | Complete Framework: Layouts, Drawables, Views, Shaders, Fonts, and Text |
| `<EgfxAssets.h>` | Built-in assets: shader presets, font families, and drawables |
| `<EgfxModules.h>` | Built-in Modules: FpsDisplay, EgfxLogo, TerminalWindow |

---

## 📂 Examples

### [EngineDemo](examples/EngineDemo/)

The recommended starting point. Each demo is automatically cycled and demonstrates best practices for implementing views:

1. **LogoSplashDemo** — Animated EGFX logo using the EgfxLogo module
2. **PrimitiveDemo** — Animated lines, rectangles, and triangles
3. **VectorTextDemo** — Dynamic text with vector fonts and animated scaling
4. **BitmaskTextDemo** — Fast text rendering with multiple scales
5. **ImageDemo** — Full-color bitmap rendering with rotation and brightness. For Monochrome displays, the demo uses bitmask with rotation instead.

#### Quick Start
1. Open `DisplayConfiguration.h` and select your screen type
2. Configure pin definitions for your platform
3. Upload to your board
4. Watch as demos automatically cycle

### [TerminalDisplay](examples/TerminalDisplay/)

A practical example using the **TerminalWindow** module to create a scrollable text display, demonstrating module-based UI composition.

---

## 📁 Source Structure

```
src/
├── Model/                  # Core interfaces (IFrameBuffer, IFrameDraw, IScreenDriver, IFrameEngine)
├── Framebuffer/            # Framebuffer implementations (Template, DoubleBuffered)
├── ScreenDrivers/          # Display drivers (SSD1306, SH1106, SSD1331, SSD1351, ST7735, ST7789)
├── DisplayEngine/          # Display engine task and sync management
├── Platform/               # Platform-specific optimizations
├── PlatformPresets/        # Ready-to-use platform configurations
├── Framework/
│   ├── Layout/             # Compile-time layouts (Grid, Weighted, Margin, Align, Constrained)
│   ├── View/               # View system (FrameAdapter, AbstractView, DrawablesView, CompositeView)
│   ├── Shader/             # Shader pipeline (Source, Color, Transform, Pixel, Primitive)
│   ├── Bitmask/            # Bitmask graphics and font subsystem
│   ├── Bitmap/             # Bitmap graphics subsystem
│   ├── Vector/             # Vector graphics and font subsystem
│   ├── Code/               # Code graphics and font subsystem
│   ├── Text/               # Generic text writer (templated on any font drawer)
│   └── Assets/             # Built-in assets (Drawables, Fonts, Shaders)
└── Modules/                # Self-contained view modules (FpsDisplay, EgfxLogo, TerminalWindow)
```
