#ifndef _INTEGERGLASS_FRAMEWORK_SHADER_MODEL_h
#define _INTEGERGLASS_FRAMEWORK_SHADER_MODEL_h

#include "../../IntegerGlassCore.h"

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Shader
		{
			/// <summary>
			/// A viewport defines the shading area, including its bounds and translation within the parent layout.
			/// </summary>
			template<typename dimension_t>
			struct viewport_t
			{
				using signed_t = int16_t;

				dimension_t BoundsLeft;
				dimension_t BoundsTop;
				dimension_t BoundsRight;
				dimension_t BoundsBottom;

				signed_t TranslationX;
				signed_t TranslationY;

				signed_t GetOriginX() const
				{
					return static_cast<signed_t>(BoundsLeft) + TranslationX;
				}

				signed_t GetOriginY() const
				{
					return static_cast<signed_t>(BoundsTop) + TranslationY;
				}

				dimension_t GetTopLeftX() const
				{
					return BoundsLeft;
				}

				dimension_t GetTopLeftY() const
				{
					return BoundsTop;
				}

				dimension_t GetBottomRightX() const
				{
					return BoundsRight;
				}

				dimension_t GetBottomRightY() const
				{
					return BoundsBottom;
				}

				dimension_t GetWidth() const
				{
					return BoundsRight - BoundsLeft;
				}

				dimension_t GetHeight() const
				{
					return BoundsBottom - BoundsTop;
				}

				bool  IsValid() const
				{
					return BoundsLeft < BoundsRight && BoundsTop < BoundsBottom;
				}

				bool IsInside(const dimension_t x, const dimension_t y) const
				{
					return x >= BoundsLeft && x < BoundsRight && y >= BoundsTop && y < BoundsBottom;
				}
			};

		}
	}
}
#endif