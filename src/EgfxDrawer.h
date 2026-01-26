#ifndef _EGFX_DRAWER_INCLUDE_h
#define _EGFX_DRAWER_INCLUDE_h

// Core framework.
#include "EgfxCore.h"

// Drawer interface.
#include "Model/IFrameDraw.h"

// Sprite models and renderers.
#include "Model/ISprite.h"
#include "GraphicsRender/SpriteRenderer.h"
#include "Sprite/BitmapSprite.h"
#include "Sprite/BitMaskSprite.h"

// Sprite shaders.
#include "Sprite/Shader/SpriteShader.h"
#include "Sprite/Shader/SpriteShaderEffect.h"

// Sprite transforms.
#include "Sprite/Transform/InvertTransform.h"
#include "Sprite/Transform/RotateTransform.h"
#include "Sprite/Transform/SkewTransform.h"
#include "Sprite/Transform/ScaleTransform.h"

// Equation-based sprites.
#include "Sprite/Equation/CircleSprite.h"
#include "Sprite/Equation/RingSprite.h"
#include "Sprite/Equation/RectangleSprite.h"

// Vector graphics.
#include "VectorGraphics/Model.h"
#include "VectorGraphics/Shaders.h"
#include "VectorGraphics/TemplateVectorDrawer.h"

// Vector font, derives from vector graphics.
#include "VectorFont/Model.h"
#include "VectorFont/TemplateFontDrawer.h"

// Code font.
#include "CodeFont/Model.h"
#include "CodeFont/TemplateFontDrawer.h"

// Bitmask font.
#include "BitmaskFont/Model.h"
#include "BitmaskFont/TemplateFontDrawer.h"

// Text rendering.
#include "Text/FontText.h"
#include "Text/TemplateTextWriter.h"

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

// 2D drawable framework.
#include "Framework/Drawable.h"
#include "Framework/View/Model.h"
#include "Framework/View/AbstractView.h"
#include "Framework/View/DrawablesView.h"
#include "Framework/View/CompositeView.h"

// Included assets.
// Bitmask fonts.
#include "BitmaskFont/Fonts/Plastic/Plastic.h" // Simple small font with 3x5 and 5x5 variants.
#include "BitmaskFont/Fonts/Micron/Micron.h" // Subpixel-font with 1x5 and 2x5 variants.

// Code font implementations.
#include "CodeFont/Fonts/RawBot.h" // Monospaced font with programming ligatures.

// Vector font Epoxy variants.
#include "VectorFont/Fonts/Epoxy/EpoxyFullType.h" // Full-featured vector font with uppercase, lowercase, numbers, punctuation, and symbols.
#include "VectorFont/Fonts/Epoxy/EpoxyNumbersType.h" // Compact vector font with numbers and basic punctuation.

#endif