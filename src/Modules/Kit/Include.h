#ifndef _EGFX_MODULES_KIT_INCLUDE_h
#define _EGFX_MODULES_KIT_INCLUDE_h

// Color sources.
#include "Shader/Source/Checkerboard.h"
#include "Shader/Source/Gradient.h"
#include "Shader/Source/AxisSplit.h"

// Color shaders.
#include "Shader/Color/Invert.h"
#include "Shader/Color/Brightness.h"
#include "Shader/Color/Contrast.h"
#include "Shader/Color/Saturation.h"

// Transform shaders.
#include "Shader/Transform/Translate.h"
#include "Shader/Transform/Crop.h"
#include "Shader/Transform/Orient.h"
#include "Shader/Transform/ScaleDown.h"
#include "Shader/Transform/Skew.h"
#include "Shader/Transform/Rotate.h"
#include "Shader/Transform/DitherMask.h"

// Animation Interpolators. Commonly used interpolators for animation curves.
#include "Animation/Interpolator/EaseInOut.h"
#include "Animation/Interpolator/AnticipateOvershoot.h"
#include "Animation/Interpolator/Bounce.h"
#include "Animation/Interpolator/Triangle.h"
#include "Animation/Interpolator/Sine.h"

// Bitmask fonts.
#include "Font/Bitmask/Contact/FontType.h" // Simple small font 5X6 with full ASCII set.
#include "Font/Bitmask/Sealant/FontType.h" // Reduced character set of uppercase letters (F, P, S) and numbers for FPS counters.
#include "Font/Bitmask/Threadlocker/FontType.h" // Tiny font for very small sizes with clear(ish) legibility. Uppercase-only letters, numbers and a few symbols.
#include "Font/Bitmask/Resin/FontType.h" // Monospace 6x6 bitmask font for retro terminal and serial output.

// Bitmap fonts.
#include "Font/Bitmap/ContactChromatic/FontType.h" // Colored bitmap version of the Contact font with full ASCII set, and groovy colors.
#include "Font/Bitmap/Cyanoacrylate/FontType.h" // Subpixel-font with 1x5 and 2x5 variants.

// Vector fonts.
#include "Font/Vector/Epoxy/FontType.h" // Full-featured vector font with uppercase, lowercase, numbers, punctuation, and symbols.

// Vector icons.
#include "Icon/Vector/Silkscreen/IconSetType.h" // Rich icon set in outline style.

// Shape drawables. Ready-to-use shape drawables for common geometric shapes.
#include "Drawable/Shape/Rectangle.h"
#include "Drawable/Shape/RoundedRectangle.h"

// Shadow drawables. Gradients with configurable color and opacity for use as drop shadows, glows, or other effects.
#include "Drawable/Shadow/CardinalGradient.h"
#include "Drawable/Shadow/AxisGradient.h"
#include "Drawable/Shadow/CornerGradient.h"

#endif