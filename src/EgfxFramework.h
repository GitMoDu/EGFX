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
#include "Framework/Layout/Combine.h"

// Animation framework.
#include "Framework/Animation/Model.h"
#include "Framework/Animation/Interpolator.h"
#include "Framework/Animation/Animator.h"

// Dynamic motion layout framework.
#include "Framework/Projection/Model.h"
#include "Framework/Projection/Layout.h"
#include "Framework/Projection/Control/Scroll.h"
#include "Framework/Projection/View/CompositeListView.h"
#include "Framework/Projection/Effect/Model.h"
#include "Framework/Projection/Effect/Adapter.h"

// Drawable contract.
#include "Framework/Drawable/Model.h"

// View framework, based on drawables.
#include "Framework/View/Model.h"
#include "Framework/View/AbstractView.h"
#include "Framework/View/DrawablesView.h"
#include "Framework/View/CompositeView.h"
#include "Framework/View/SelectorView.h"

// Shader chain framework.
#include "Framework/Shader/Model.h"

// Radial adapter for radial shaders.
#include "Framework/Shader/Radial.h"

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

// Geometry shaders.
#include "Framework/Shader/Geometry/Model.h"
#include "Framework/Shader/Geometry/Line.h"
#include "Framework/Shader/Geometry/Rectangle.h"
#include "Framework/Shader/Geometry/Triangle.h"
#include "Framework/Shader/Geometry/CircleLine.h"
#include "Framework/Shader/Geometry/CircleFill.h"
#include "Framework/Shader/Geometry/CircleRing.h" // No dependency on Sine().
#include "Framework/Shader/Geometry/CircleArc.h" // Depends on Sine().

// Bitmask graphics.
#include "Framework/Bitmask/Model.h"
#include "Framework/Bitmask/Source.h"

// Bitmap graphics.
#include "Framework/Bitmap/Model.h"
#include "Framework/Bitmap/Source.h"
#include "Framework/Bitmap/Abstract.h"

// Graphics classes models
#include "Framework/Image/Model.h"
#include "Framework/Icon/Model.h"
#include "Framework/Vector/Model.h"
#include "Framework/Text/Model.h"

// Text rendering support.
#include "Framework/Text/TextWalker.h"

// Bitmask image, derives from bitmask shader.
#include "Framework/Shader/Image/Bitmask/ImageShader.h"
#include "Framework/Image/Bitmask/Drawable.h"
#include "Framework/Image/Bitmask/View.h"

// Bitmask text, derives from bitmask image.
#include "Framework/Text/Bitmask/Model.h"
#include "Framework/Text/Bitmask/Drawable.h"
#include "Framework/Text/Bitmask/View.h"

// Bitmap image, derives from bitmap shader.
#include "Framework/Shader/Image/Bitmap/ImageShader.h"
#include "Framework/Image/Bitmap/Drawable.h"
#include "Framework/Image/Bitmap/View.h"

// Bitmap text, derives from bitmap image.
#include "Framework/Text/Bitmap/Model.h"
#include "Framework/Text/Bitmap/Drawable.h"
#include "Framework/Text/Bitmap/View.h"

// Vector shader.
#include "Framework/Vector/Decoder.h"
#include "Framework/Shader/Image/Vector/Model.h"
#include "Framework/Shader/Image/Vector/VectorShader.h"
#include "Framework/Shader/Image/Vector/ImageShader.h"

// Vector image, derives from vector shader.
#include "Framework/Image/Vector/Drawable.h"
#include "Framework/Image/Vector/View.h"

// Vector text, derives from vector image.
#include "Framework/Text/Vector/Model.h"
#include "Framework/Text/Vector/Drawable.h"
#include "Framework/Text/Vector/View.h"

// Vector icon, derives from vector image.
#include "Framework/Icon/Vector/Model.h"
#include "Framework/Icon/Vector/Drawable.h"
#include "Framework/Icon/Vector/View.h"

// Projectors and views.
#include "Framework/Projection/Projectors/PageSlide/View.h"

#endif