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
#include "VectorGraphics/TemplateShadeDecoder.h"
#include "VectorGraphics/SingleColorVectorDrawer.h"

// Vector font, derives from vector graphics.
#include "VectorFont/Model.h"
#include "VectorFont/AbstractFontDrawer.h"
#include "VectorFont/ColorFontDrawer.h"

// Code font.
#include "CodeFont/Model.h"
#include "CodeFont/AbstractFontDrawer.h"
#include "CodeFont/ColorFontDrawer.h"

// Bitmask font.
#include "BitmaskFont/AbstractFontDrawer.h"
#include "BitmaskFont/ColorFontDrawer.h"

// Text rendering.
#include "Text/FontText.h"
#include "Text/TemplateTextWriter.h"

// 2D drawable framework.
#include "Framework/Layout.h"
#include "Framework/Drawable.h"
#include "Framework/ProgressScaler.h"
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