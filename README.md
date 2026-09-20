# IntegerGlass

[![License](https://img.shields.io/github/license/GitMoDu/IntegerGlass)](LICENSE)
[![Arduino library](https://img.shields.io/badge/Arduino-library-00979D?logo=arduino&logoColor=white)](library.properties)

IntegerGlass is a compile-time graphics framework and cooperative display engine for embedded systems.

![Embedded Graphics Framework](https://github.com/GitMoDu/IntegerGlass/blob/master/media/logo_splash_multi_screen.gif)

It provides the pieces needed to build real interfaces on microcontroller displays:

- Frame buffers
- Display drivers
- Cooperative frame scheduling
- Compile-time layouts
- Stateful views
- Drawable composition
- Animation
- Projection and scrolling
- Shader pipelines
- Bitmask, bitmap and vector graphics
- Text and icon rendering
- Reusable interface modules

## The model

IntegerGlass is organized around a display engine and a composable view framework.

```text
application
    ↓
view hierarchy
    ↓
drawables and shader pipelines
    ↓
frame buffer
    ↓
screen driver
    ↓
physical display
```

The display engine manages the frame cycle. The Framework determines what is rendered. Screen drivers handle communication with the display hardware.

Rendering is cooperative: work can be spread across multiple scheduler passes instead of blocking the rest of the application.

## Framework and Kit

The library is divided conceptually into two layers.

### Framework

The Framework is the foundational rendering system. It provides the contracts and mechanisms for:

- Layout
- Views
- Drawables
- Animation
- Projection
- Shader composition
- Bitmask graphics
- Bitmap graphics
- Vector graphics
- Text
- Icons
- Geometry

The Framework does not require the application to use the library's supplied fonts, shapes, effects, or modules.

## Views

Views are the active layer of IntegerGlass. They coordinate drawables, child views, animation state, and multi-step rendering over time.

A view can complete its work in one draw call or distribute it across several cooperative draw calls without blocking the scheduler.

### `ViewAdapter`

`ViewAdapter` is intended to be the single, final adaptation layer between a concrete view and the display engine's `IFrameDraw` interface.

It avoids building inheritance chains merely to make a view compatible with the engine. Define the view itself, then wrap it once:

```cpp
using ScreenView =
    IntegerGlass::Framework::View::ViewAdapter<MyView>;

ScreenView View{};

DisplayEngine.SetDrawer(&View);
```

The adapter owns the view instance and forwards the engine's draw-call contract to it. Application views can therefore focus on layout, composition, animation, and rendering rather than engine plumbing.

### View composition

The other view types are composition and lifecycle tools:

- **`AbstractView`** provides the stateful view lifecycle. Derive from it when a view needs custom animation or sequencing through `ViewStep()`.
- **`DrawablesView`** turns a compile-time collection of drawables into a view and dispatches them across draw calls.
- **`CompositeView`** combines child views into a larger view.
- **`SelectorView`** selects between view or content alternatives while preserving the surrounding view contract.

Composite views do more than store child objects. They automatically handle the bookkeeping required for nested composition:

- Child bounds are derived from the parent layout.
- Bounds are forwarded to child views.
- Parent-to-child coordinate translation is maintained.
- Child views remain in their own local coordinate spaces.
- Child progression and draw-cycle completion are coordinated.
- Compile-time view accessors expose composed children without runtime lookup.
- Template composition preserves the concrete types and layout information throughout the view tree.

This allows a complex interface to be assembled from local views without manually forwarding rectangles, translations, draw-cycle state, or child access.

A typical application view therefore has this shape:

```text
concrete view
    ├── local layout
    ├── local drawables
    ├── local animation state
    └── child views
            └── child drawables
```

Only the final outer view needs to be adapted with `ViewAdapter`.

## Layout

IntegerGlass layouts describe the position and dimensions of content within a parent view.

They are template-based and intended to be resolved at compile time rather than through a general-purpose runtime layout engine.

Available layout components include:

- `Grid`
- `Weighted`
- `Margin`
- `Align`
- `Translate`
- `Combine`
- `ConstrainedDimensions`
- `ConstrainedRatio`

Layouts provide the coordinate and bounds information used by views, drawables, and shader pipelines.

## Animation and projection

The Framework includes animation and projection support for dynamic interfaces.

Animation provides:

- Interpolators
- Animators
- Progress scaling
- Time-based view state changes

Projection provides:

- Dynamic layout movement
- Translation
- Scrolling
- Composite list views
- Projection effects
- Page-slide transitions

These systems are designed to work with the view composition model and can be combined with drawables and shader pipelines through templates.

## Graphics

IntegerGlass supports multiple graphics representations.

| Graphics type | Purpose |
| --- | --- |
| **Bitmask** | Compact 1-bit images and fonts with a small memory footprint. |
| **Bitmap** | Pixel assets and text containing explicit color data. |
| **Vector** | Encoded vector images, icons, and text. |
| **Geometry** | Lines, rectangles, triangles, circles, rings, and arcs. |

Each graphics subsystem provides the models and drawable/view types needed to integrate that representation into the Framework.

## Shader pipelines

IntegerGlass uses a compile-time shader pipeline:

```text
color source or palette
    ↓
color shader
    ↓
coordinate transform
    ↓
renderer
```

The first stage depends on the view and drawable type. It may use a procedural color source or a palette.

Each stage is optional and composable. A drawable or geometry renderer can use a source or palette directly, apply color shading, transform the result, or combine all of those stages.

Because the pipeline is template-based, unused stages can be represented by no-op types and optimized away. A drawable with no color shader or coordinate transform does not need to pay for an empty runtime effect layer; the compiler can reduce the composition to the corresponding direct path.

The same pipeline model is used by images, icons, text, and geometry. IntegerGlass does not require a separate runtime effect graph or a separate shading path for each content type.

## Vector

Vector images, icons, and text share the same encoded-vector geometry pipeline.

![Example Vector Image](https://github.com/GitMoDu/IntegerGlass/blob/master/media/image_vector_editor.png?raw=true)

Compact vector data is decoded into primitives, which are rendered through the same pixel-shading pipeline as other drawable content. Depending on the view type, the pipeline uses either a color source or a palette, followed by optional color shading and coordinate transforms.

```text
encoded vector data
    ↓
primitive geometry
    ↓
shared pixel-shading pipeline
```

This keeps vector content on the shared rendering path instead of requiring a separate vector renderer.

Vector geometry can therefore use the same:

- Color source or palette selection
- Color shading
- Coordinate transforms
- Viewport and bounds behavior
- Compile-time no-op optimizations
- Drawable and view composition rules

## Text and icons

Text rendering is generic. The text system is templated on a font drawer rather than tied to one particular font representation.

![Example Vector Image](https://github.com/GitMoDu/IntegerGlass/blob/master/media/vector_font_preview.png?raw=true)

This allows the same text and view concepts to cover:

- Small fixed-pixel labels
- Monospace text
- Colored bitmap text
- Scalable vector text
- Interface icons

## Reusable modules

`<IntegerGlassModules.h>` includes reusable modules built on top of the Framework.

### Kit

The Kit is optional library-provided convenience content built on top of the Framework. It includes:

#### Bitmask fonts for compact, pixel-oriented interfaces:
- Contact — a small general-purpose font.
- Sealant — a reduced uppercase and numeric font for compact status displays.
- Threadlocker — a tiny uppercase font for very small screens.
- Resin — a monospace 6×6 font suited to terminals and serial-style output.

#### Bitmap fonts for colored text and subpixel-style designs:
- ContactChromatic — a full-color bitmap version of the Contact style.
- Cyanoacrylate — compact 1×5 and 2×5 subpixel fonts.

#### Vector fonts for scalable text:
- Epoxy — a full-featured encoded vector font with uppercase, lowercase, numbers, punctuation, and symbols.
  
#### Vector icons:
- Silkscreen — an outline-style icon set for interface indicators and compact UI decoration.


The Framework defines the system. The Kit provides useful ready-made building blocks for applications and examples.


Available modules include:

- **Logo** — static and animated logo/splash views.
- **ExampleImages** — example bitmask, bitmap, and vector assets.
- **FpsDisplay** — an FPS overlay that can be composed with another view.
- **TerminalWindow** — a scrollable text terminal and `PrintAdapter`.
- **BatteryIndicator** — battery-status UI.
- **Timecode** — animated timecode text.
- **Plot** — line, fill, bar, and selector-based plotting views.
- **ProgressIndicator** — linear and circular determinate progress views.
- **ProgressIndeterminate** — linear and circular indeterminate progress views.

These modules are examples of higher-level components built on the Framework. They are optional; applications can use the Framework directly and create their own views and modules.

## Display engine

The display engine manages the frame lifecycle:

```text
clear frame
    ↓
execute draw calls
    ↓
synchronize frame
    ↓
push frame to display
```

The engine supports:

- Static frame-buffer allocation
- Dynamic frame-buffer allocation
- Optional double buffering
- Cooperative draw calls
- Variable-refresh synchronization
- Asynchronous buffer pushing where supported
- DMA-oriented driver variants where supported
- RTOS-oriented driver variants where supported
- Performance logging

The engine is designed to work with [TaskScheduler](https://github.com/arkhipenko/TaskScheduler).

A view is attached to the engine through the `IFrameDraw` interface, normally using one final `ViewAdapter` wrapper.

## Display drivers

IntegerGlass currently includes drivers for common OLED and LCD controllers, including:

- SSD1306
- SH1106
- SH1107
- SSD1331
- SSD1351
- ST7735
- ST7789
- ST7789T3
- GC9107
- GC9A01

The drivers cover I2C and SPI variants where supported.

Drivers handle communication and frame pushing. They do not define the view hierarchy or rendering model; those responsibilities belong to the display engine and Framework.

## Performance logging

The display engine includes an optional performance logging task.

Performance logging can be enabled with:

```cpp
#define INTEGERGLASS_PERFORMANCE_LOG
#define INTEGERGLASS_PERFORMANCE_LOG_DETAIL
```

The logging task can report information such as:

- Display frame rate
- Draw-call count
- Longest draw-call duration
- Frame-buffer clear duration
- Render duration
- Push duration
- Display occupancy
- Idle duration
- Estimated CPU load

Use performance logging when tuning frame-buffer size, color depth, buffering mode, view composition, or display transport.

## Include headers

Use the umbrella headers for the main library layers:

| Header | Provides |
| --- | --- |
| `<IntegerGlassCore.h>` | Platform definitions, colors, display options, buffer sizing, and core interfaces. |
| `<IntegerGlassFramebuffers.h>` | Frame-buffer implementations and double buffering. |
| `<IntegerGlassScreenDrivers.h>` | Supported screen drivers and wrappers. |
| `<IntegerGlassDisplayEngine.h>` | Display engine tasks, synchronization, and performance logging. |
| `<IntegerGlassFramework.h>` | Foundational layouts, animation, projections, views, shaders, graphics, text, and vector support. |
| `<IntegerGlassModules.h>` | The optional Kit and reusable modules. |
| `<IntegerGlassPlatformPresets.h>` | Platform configuration presets. |

## Getting started

The complete setup depends on the selected display, resolution, interface, and board. Start with [`examples/EngineDemo`](examples/EngineDemo) and its [`DisplayConfiguration.h`](examples/EngineDemo/DisplayConfiguration.h).

The general setup sequence is:

1. Install IntegerGlass and its dependencies.
2. Open `DisplayConfiguration.h`.
3. Select the display controller and resolution.
4. Configure the communication interface and board pins.
5. Select static or dynamic frame-buffer storage.
6. Select single or double buffering if appropriate.
7. Create a concrete view.
8. Wrap the outer view once with `ViewAdapter`.
9. Attach the adapter to `DisplayEngineTask`.
10. Run the scheduler from `loop()`.

For a complete hardware-specific example, see:

- [`examples/EngineDemo/EngineDemo.ino`](examples/EngineDemo/EngineDemo.ino)
- [`examples/EngineDemo/DisplayConfiguration.h`](examples/EngineDemo/DisplayConfiguration.h)
- [`examples/TerminalDisplay/TerminalDisplay.ino`](examples/TerminalDisplay/TerminalDisplay.ino)

## Examples

### EngineDemo

[`examples/EngineDemo`](examples/EngineDemo) demonstrates the display-engine integration:

- Multiple display configurations
- Static frame-buffer allocation
- Dynamic frame-buffer allocation
- Optional double buffering
- View adaptation through `ViewAdapter`
- Animated logo rendering
- Optional FPS display
- Optional performance logging
- Platform-aware setup and diagnostics

### TerminalDisplay

[`examples/TerminalDisplay`](examples/TerminalDisplay) turns serial input into a screen-backed terminal.

It demonstrates:

- Auto-sized terminal layouts
- Monospace bitmask fonts
- Scaled text rendering
- The `TerminalWindow` module
- `PrintAdapter`
- Performance logging routed to the display
- Serial-to-display loopback

## Asset editors

IntegerGlass includes browser-based asset editors under [`Tools`](Tools). They are local static web pages: open the relevant `index.html` file in a browser, edit the asset, and copy the generated source/declaration into your project or module.

Available editors:

| Editor | Purpose | Local page |
| --- | --- | --- |
| **Bitmap Font Editor** | Create and edit colored bitmap fonts. | [`Tools/FontEditor/Bitmap/index.html`](Tools/FontEditor/Bitmap/index.html) |
| **Bitmask Font Editor** | Create compact 1-bit fonts for small displays. | [`Tools/FontEditor/Bitmask/index.html`](Tools/FontEditor/Bitmask/index.html) |
| **Vector Font Editor** | Create encoded scalable vector fonts and export their declarations. | [`Tools/FontEditor/Vector/index.html`](Tools/FontEditor/Vector/index.html) |
| **Vector Icon Editor** | Design and export vector icon sets. | [`Tools/VectorIconEditor/index.html`](Tools/VectorIconEditor/index.html) |
| **Vector Image Editor** | Design, preview, transform, and export encoded vector images. | [`Tools/VectorImageEditor/index.html`](Tools/VectorImageEditor/index.html) |

These editors do not require a server or build system. They are intended to be opened locally as HTML pages. The generated assets are designed for the corresponding IntegerGlass graphics systems:

```text
bitmap editor      → bitmap graphics and bitmap text
bitmask editor     → bitmask graphics and bitmask text
vector font editor → vector text
vector icon editor → vector icons
vector image editor→ vector images
```

The editors are development tools rather than runtime dependencies. The resulting declarations and data can be compiled into an Arduino project and stored in program memory where appropriate.

## Repository structure

```text
src/
├── Model/                         # Core interfaces, colors, display options, buffer sizing
├── Framebuffer/                   # Frame-buffer implementations
├── DisplayEngine/                 # Frame orchestration, synchronization, and logging
├── ScreenDrivers/                 # Controller drivers and transport templates
├── ScreenWrappers/                # Display-specific frame-buffer wrappers
├── Platform/                      # Platform detection and behavior
├── PlatformPresets/               # Reusable platform configurations
├── Framework/
│   ├── Animation/                 # Interpolators and animators
│   ├── Bitmap/                    # Bitmap graphics
│   ├── Bitmask/                   # Bitmask graphics
│   ├── Drawable/                  # Drawable contracts
│   ├── Image/                     # Image drawables and views
│   ├── Icon/                      # Icon models and views
│   ├── Layout/                    # Compile-time layouts
│   ├── Projection/                # Scrolling, projections, and transitions
│   ├── Shader/                    # Shader pipeline components
│   ├── Text/                      # Generic text and font integration
│   ├── Vector/                    # Encoded-vector decoding
│   └── View/                      # View lifecycle and composition
└── Modules/
    ├── Kit/                       # Optional library-provided conveniences
    ├── BatteryIndicator/
    ├── ExampleImages/
    ├── FpsDisplay/
    ├── Logo/
    ├── Plot/
    ├── ProgressIndicator/
    ├── ProgressIndeterminate/
    ├── TerminalWindow/
    └── Timecode/
```


## Dependencies

- [TaskScheduler](https://github.com/arkhipenko/TaskScheduler)
- [IntegerSignal](https://github.com/GitMoDu/IntegerSignal)

