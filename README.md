# EGFX - Embedded Graphics

C++ embedded graphics engine targeted at Arduino.
Frame-Buffer only, MCU needs enough memory to hold a full frame.

Simple drivers for a few screens include only setup and frame-push.
Supports monochrome, low color and full color frame buffers and screen drivers.


Engine runs on a single task using Task Scheduler (https://github.com/arkhipenko/TaskScheduler).
Render is done in passes with DrawCalls, limiting the sequential time used for each visual piece render.
Frame-push can be optimized with optional Screen Driver DMA, Async and RTOS variants.

Supported Screens:
- SSD1306 OLED
  - 64x32x1 I2C
  - 64x48x1 I2C
  - 72x40x1 I2C
  - 128x64x1 I2C
  - 128x64x1 SPI
 
- SH1106 OLED
  - 128x64x1 SPI

- SSD1331 OLED
  - 96x64x8 SPI
  - 96x64x16 SPI 
 
- SSD1351 OLED
  - 128x128x16 SPI 

- ST7735 LCD
  - 160x80x16 SPI    

- ST7789 LCD
  - 240x240x16 SPI

 
Draw Primitives are provided directly by the chosen frame-buffer:
- Point,
- Line
- Rectangle
- Rectangle Fill


Text Rendering is available as a static call with a mono-spaced font and pre-calculated FontStyle, including kerning.

Features
- Numbers [0;10]
- Letters
- Symbols


Engine State Flow

Engine is set with target FPS (frame period) and will try to follow synchronized frame time.
Draw calls are provided with a single frame time for each frame.
Frame skip is automatically detected and compensated. 
Engine task sleeps when idle (Power saving via entering IDLE sleep mode https://github.com/arkhipenko/TaskScheduler/wiki).

![](https://raw.githubusercontent.com/GitMoDu/EGFX/master/media/engine_flow.svg)

Drawing is done on-demand by ElementDrawers, which can have up to 255 individual "elements" to render to screen.
Each draw call is done in a separate loop() pass, to avoid hogging the CPU for too long.
To use multiple ElementDrawers, MultiDrawerWrapper is available and can wrap up to 255 ElementDrawers, each with their individual DrawElements.


Realtime metrics of Graphics Engine are available with EngineLogTask.
- Display FPS.
- DrawCalls count.
- Frame-Buffer clear duration.
-	Render duration and load.
-	Push duration and occupancy.
- Idle duration and occupancy.
- Longest DrawCall duration.
- Longest push call duration.


