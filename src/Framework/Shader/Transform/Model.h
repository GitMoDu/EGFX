#ifndef _EGFX_FRAMEWORK_SHADER_TRANSFORM_MODEL_h
#define _EGFX_FRAMEWORK_SHADER_TRANSFORM_MODEL_h

#include "../../../EgfxCore.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Transform
			{
				/// <summary>
				/// No-op transform shader that returns input coordinates unchanged.
				/// Stores origin point for reference in derived classes.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				template<typename dimension_t>
				class BaseTransform
				{
				protected:
					pixel_point_t Origin{ 0, 0 };

				public:
					BaseTransform() = default;
					~BaseTransform() = default;

					void Prepare(const pixel_t originX, const pixel_t originY)
					{
						Origin.x = originX;
						Origin.y = originY;
					}

					static constexpr bool Transform(dimension_t& /*x*/, dimension_t& /*y*/)
					{
						return true;
					}
				};

				namespace Contract
				{
					/// <summary>
					/// Defines the contract for coordinate transformation shading.
					/// Return false to skip pixel and further transforms.
					/// This class serves as documentation only, and is not intended to be used directly.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="BaseTransform">The base transform to inherit from.</typeparam>
					template<typename dimension_t,
						typename Base = BaseTransform<dimension_t>>
						struct TransformShader : Base
					{
						// Prepare is called once per batch with the origin point.
						void Prepare(const pixel_t originX, const pixel_t originY) {}

						bool Transform(dimension_t& x, dimension_t& y)
						{
							// Inline call to base transform
							if (!Base::Transform(x, y))
							{
								// Skip when base transform has returned false.
								return false;
							}

							// Apply transform logic.
							/*	x = x + X;
							y = Y - y;*/

							// Return true to continue processing.
							return true;
						}
					};
				}
			}
		}
	}
}
#endif