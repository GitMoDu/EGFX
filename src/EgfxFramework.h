#ifndef _EGFX_FRAMEWORK_INCLUDE_h
#define _EGFX_FRAMEWORK_INCLUDE_h

// Egfx core model and interfaces.
#include "EgfxCore.h"

// Animation support.
#include "Framework/ProgressScaler.h"

// Compile-time layout framework.
#include "Framework/Layout/Model.h"
#include "Framework/Layout/ConstrainedDimensions.h"
#include "Framework/Layout/ConstrainedRatio.h"
#include "Framework/Layout/Grid.h"
#include "Framework/Layout/Weighted.h"
#include "Framework/Layout/Margin.h"
#include "Framework/Layout/Align.h"
#include "Framework/Layout/Translate.h"

// Drawable contract.
#include "Framework/Drawable.h"

// View framework, based on drawables.
#include "Framework/View/Model.h"
#include "Framework/View/AbstractView.h"
#include "Framework/View/DrawablesView.h"
#include "Framework/View/CompositeView.h"

// Color sources.
#include "Framework/Shader/Source/Model.h"
#include "Framework/Shader/Source/Sources.h"

// Color shaders.
#include "Framework/Shader/Color/Model.h"
#include "Framework/Shader/Color/Shaders.h"

// Transform shaders.
#include "Framework/Shader/Transform/Model.h"
#include "Framework/Shader/Transform/Shaders.h"

// Pixel shaders.
#include "Framework/Shader/Pixel/Model.h"
#include "Framework/Shader/Pixel/Shaders.h"

// Primitive shaders.
#include "Framework/Shader/Primitive/Model.h"
#include "Framework/Shader/Primitive/Shaders.h"

// Code graphics.
#include "Framework/Code/Model.h"

// Code font, derives from code graphics.
#include "Framework/Code/Font/Model.h"
#include "Framework/Code/Font/AbstractDrawer.h"

// Bitmask graphics.
#include "Framework/Bitmask/Model.h"
#include "Framework/Bitmask/TemplateDrawer.h"
#include "Framework/Bitmask/TemplateDrawable.h"

// Bitmask font, derives from bitmask graphics.
#include "Framework/Bitmask/Font/Model.h"
#include "Framework/Bitmask/Font/TemplateDrawer.h"

// Bitmap graphics.
#include "Framework/Bitmap/Model.h"
#include "Framework/Bitmap/TemplateDrawer.h"
#include "Framework/Bitmap/TemplateDrawable.h"

// Vector graphics.
#include "Framework/Vector/Model.h"
#include "Framework/Vector/TemplateDrawer.h"

// Vector font, derives from vector graphics.
#include "Framework/Vector/Font/Model.h"
#include "Framework/Vector/Font/TemplateDrawer.h"

// Text writing, templated from any font drawer type.
#include "Framework/Text/Model.h"
#include "Framework/Text/TemplateWriter.h"

#endif