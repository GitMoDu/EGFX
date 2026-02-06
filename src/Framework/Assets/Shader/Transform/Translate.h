#ifndef _EGFX_FRAMEWORK_ASSETS_SHADER_TRANSFORM_TRANSLATE_h
#define _EGFX_FRAMEWORK_ASSETS_SHADER_TRANSFORM_TRANSLATE_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace Shader
			{
				namespace Transform
				{
					/// <summary>
					/// Translation transform shader.
					/// Adds a constant (X,Y) offset after the base transform chain has been applied.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Base">Base transform type in the chain (defaults to BaseTransform).</typeparam>
					template<typename dimension_t,
						typename Base = Framework::Shader::Transform::BaseTransform<dimension_t>
					>
					class Translate : public Base
					{
					private:
						/// <summary>Translation offset applied to the X coordinate.</summary>
						dimension_t OffsetX = 0;

						/// <summary>Translation offset applied to the Y coordinate.</summary>
						dimension_t OffsetY = 0;

					public:
						Translate() : Base() {}
						~Translate() {}

						/// <summary>
						/// Sets the translation offsets.
						/// </summary>
						/// <param name="offsetX">Translation along X (in transform-space units).</param>
						/// <param name="offsetY">Translation along Y (in transform-space units).</param>
						void SetTranslation(const dimension_t offsetX, const dimension_t offsetY)
						{
							OffsetX = offsetX;
							OffsetY = offsetY;
						}

						/// <summary>
						/// Applies the transform chain and then adds the configured translation offset.
						/// </summary>
						/// <param name="x">X coordinate to transform in-place.</param>
						/// <param name="y">Y coordinate to transform in-place.</param>
						/// <returns>True if the base transform chain succeeds; otherwise false.</returns>
						bool Transform(dimension_t& x, dimension_t& y)
						{
							if (!Base::Transform(x, y))
							{
								return false;
							}

							x = static_cast<dimension_t>(x + OffsetX);
							y = static_cast<dimension_t>(y + OffsetY);

							return true;
						}
					};
				}
			}
		}
	}
}
#endif