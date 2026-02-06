#ifndef _EGFX_CORE_INCLUDE_h
#define _EGFX_CORE_INCLUDE_h

// Core platform definitions.
#include "Platform/Platform.h"

// RGB color model.
#include "Model/RgbColor.h"

// Display options for framebuffers and display engine.
#include "Model/DisplayOptions.h"

// Buffer size calculator.
#include "Model/GraphicsBuffer.h"

// Core interfaces.
#include "Model/IFrameBuffer.h" // Framebuffer interface.
#include "Model/IScreenDriver.h" // Screen driver interface.

#include "Model/IFrameDraw.h" // Display engine draw interface.
#include "Model/IFrameEngine.h" // Display engine interface.

#endif