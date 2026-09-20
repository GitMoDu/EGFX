#ifndef _INTEGERGLASS_MODULES_INCLUDE_h
#define _INTEGERGLASS_MODULES_INCLUDE_h

// Modules depend on the framework.
#include "IntegerGlassFramework.h"

// Framework Kit module. Reusable higher-level utilities built on the framework.
#include "Modules/Kit/Include.h"

// Logo module with splash screen.
#include "Modules/Logo/View.h"

// Example images module, bitmasks, bitmaps, and vector graphics.
#include "Modules/ExampleImages/Assets.h"

// FPS Display module.
#include "Modules/FpsDisplay/View.h"

// Terminal Window module.
#include "Modules/TerminalWindow/View.h"
#include "Modules/TerminalWindow/PrintAdapter.h"

// Battery indicator module.
#include "Modules/BatteryIndicator/View.h"

// Wireless indicator module.
#include "Modules/WirelessIndicator/View.h"

// Timecode text module.
#include "Modules/Timecode/View.h"

// Plot module.
#include "Modules/Plot/View.h"

// Progress indicator modules.
#include "Modules/ProgressIndicator/Linear/View.h"
#include "Modules/ProgressIndicator/Circular/View.h"
#include "Modules/ProgressIndeterminate/Circular/View.h"
#include "Modules/ProgressIndeterminate/Linear/View.h"


#endif