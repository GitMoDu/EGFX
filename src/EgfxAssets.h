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

// Animation Interpolators.
#include "Framework/Assets/Animation/Interpolator/EaseInOut.h"
#include "Framework/Assets/Animation/Interpolator/AntecipateOvershoot.h"
#include "Framework/Assets/Animation/Interpolator/Bounce.h"

// Bitmask fonts.
#include "Framework/Assets/Font/Bitmask/Contact/FontType.h" // Simple small font 5X6 with full ASCII set.
#include "Framework/Assets/Font/Bitmask/Sealant/FontType.h" // Reduced character set of uppercase letters (F, P, S) and numbers for FPS counters.
#include "Framework/Assets/Font/Bitmask/Threadlocker/FontType.h" // Tiny font for very small sizes with clear(ish) legibility. Uppercase-only letters, numbers and a few symbols.
#include "Framework/Assets/Font/Bitmask/Resin/FontType.h" // Monospace 6x6 bitmask font for retro terminal and serial output.

// Bitmap fonts.
#include "Framework/Assets/Font/Bitmap/ContactChromatic/FontType.h" // Colored bitmap version of the Contact font with full ASCII set, and groovy colors.
#include "Framework/Assets/Font/Bitmap/Cyanoacrylate/FontType.h" // Subpixel-font with 1x5 and 2x5 variants.


// Vector fonts.
#include "Framework/Assets/Font/Vector/Epoxy/FontType.h" // Full-featured vector font with uppercase, lowercase, numbers, punctuation, and symbols.

// Vector icons.
#include "Framework/Assets/Icon/Vector/Silkscreen/IconSetType.h" // Rich icon set in outline style.

// Shape drawables.
#include "Framework/Assets/Shape/Drawable/Rectangle.h"
#include "Framework/Assets/Shape/Drawable/RoundedRectangle.h"

#endif