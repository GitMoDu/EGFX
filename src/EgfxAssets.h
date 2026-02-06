#ifndef _EGFX_FRAMEWORK_ASSETS_INCLUDE_h
#define _EGFX_FRAMEWORK_ASSETS_INCLUDE_h

// Color sources.
#include "Framework/Assets/Shader/Source/Checkerboard.h"
#include "Framework/Assets/Shader/Source/Gradient.h"
#include "Framework/Assets/Shader/Source/AxisSplit.h"

// Color shaders.
#include "Framework/Assets/Shader/Color/Invert.h"
#include "Framework/Assets/Shader/Color/Brightness.h"
#include "Framework/Assets/Shader/Color/Contrast.h"
#include "Framework/Assets/Shader/Color/Saturation.h"

// Transform shaders.
#include "Framework/Assets/Shader/Transform/Translate.h"
#include "Framework/Assets/Shader/Transform/Crop.h"
#include "Framework/Assets/Shader/Transform/Orient.h"
#include "Framework/Assets/Shader/Transform/ScaleDown.h"
#include "Framework/Assets/Shader/Transform/Skew.h"
#include "Framework/Assets/Shader/Transform/Rotate.h"
#include "Framework/Assets/Shader/Transform/DitherMask.h"

// Bitmask fonts.
#include "Framework/Assets/Font/Bitmask/Plastic/Plastic.h" // Simple small font with 3x5 and 5x5 variants.
#include "Framework/Assets/Font/Bitmask/Micron/Micron.h" // Subpixel-font with 1x5 and 2x5 variants.

// Code font implementations.
#include "Framework/Assets/Font/Code/RawBot.h" // Monospaced font with programming ligatures.

// Vector font Epoxy variants.
#include "Framework/Assets/Font/Vector/Epoxy/EpoxyFullType.h" // Full-featured vector font with uppercase, lowercase, numbers, punctuation, and symbols.
#include "Framework/Assets/Font/Vector/Epoxy/EpoxyNumbersType.h" // Compact vector font with numbers and basic punctuation.

// Drawables.
#include "Framework/Assets/Drawable/Code/Rectangle.h"

#endif