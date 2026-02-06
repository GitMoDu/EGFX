#ifndef _EGFX_FRAMEWORK_SHADER_TRANSFORM_SHADERS_h
#define _EGFX_FRAMEWORK_SHADER_TRANSFORM_SHADERS_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Transform
			{
				/// <summary>
				/// Compile-time utility to select appropriate fraction types based on dimensions.
				/// Selects appropriate fraction types based on dimensions and fraction's precision needs.
				/// UFraction8 is used for smaller dimensions (<=128), UFraction16 for larger ones.
				/// </summary>
				/// <typeparam name="Width">The width dimension used to determine the appropriate fraction type size.</typeparam>
				/// <typeparam name="Height">The height dimension used to determine the appropriate fraction type size. Defaults to Width if not specified.</typeparam>
				template<int16_t Width, int16_t Height = Width>
				struct FractionSelector
				{
					static constexpr bool Use16 = (Width > 128) || (Height > 128);

					// Unsigned fraction storage type (for Fraction / GetScalar).
					using ufraction_t = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						ufraction16_t,
						ufraction8_t,
						Use16
					>::type;

					using fraction_t = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						fraction16_t,
						fraction8_t,
						Use16
					>::type;
				};

				/// <summary>
				/// No-op transformation shader that leaves coordinates unchanged.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				template<typename dimension_t>
				using NoTransform = BaseTransform<dimension_t>;
			}
		}
	}
}
#endif