#ifndef _ARDUINO_GRAPHICS_DRAWER_INCLUDE_h
#define _ARDUINO_GRAPHICS_DRAWER_INCLUDE_h

// Core framework.
#include "ArduinoGraphicsCore.h"

// Drawer interface.
#include "Model/IFrameDraw.h"

// Convenience drawers and wrappers.
#include "Drawer/ElementDrawer.h"
#include "Drawer/LayoutElement.h"
#include "Drawer/MultiDrawerWrapper.h"

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

#include "Sprite/Font/SpriteFont5x5.h"
#include "Sprite/Font/SpriteFont3x5.h"
#include "Sprite/Font/SpriteFont2x5.h"
#include "Sprite/Font/SpriteFont1x5.h"



#endif