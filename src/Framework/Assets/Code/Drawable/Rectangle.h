#ifndef _EGFX_FRAMEWORK_ASSETS_CODE_DRAWABLE_RECTANGLE_h
#define _EGFX_FRAMEWORK_ASSETS_CODE_DRAWABLE_RECTANGLE_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace Code
			{
				namespace Drawable
				{
					/// <summary>
					/// Fills a rectangle.
					/// </summary>
					/// <typeparam name="ParentLayout">Parent layout providing X/Y/Width/Height.</typeparam>
					/// <typeparam name="dimension_t">The drawable's intrinsic dimension type.</typeparam>
					/// <typeparam name="PrimitiveShaderType">The primitive shader type used for rendering.</typeparam>
					template<typename ParentLayout,
						typename dimension_t = pixel_t,
						typename PrimitiveShaderType = Framework::Shader::Primitive::TemplateShader<dimension_t>
					>
					struct RectangleFill : PrimitiveShaderType
					{
						/// <summary>
						/// Prepares the primitive shader origin to the rectangle's top-left corner.
						/// </summary>
						RectangleFill()
						{
							PrimitiveShaderType::Prepare(ParentLayout::X(), ParentLayout::Y());
						}
						~RectangleFill() = default;

						/// <summary>
						/// Draws a filled rectangle using local-space coordinates (0..Width-1, 0..Height-1).
						/// The prepared origin offsets this into pixel space.
						/// </summary>
						void Draw(IFrameBuffer* frame)
						{
							PrimitiveShaderType::RectangleFill(frame,
								0, 0,
								ParentLayout::Width() - 1, ParentLayout::Height() - 1);
						}
					};

					/// <summary>
					/// Draws a rectangle outline with a specified stroke width using a primitive shader.
					/// </summary>
					/// <typeparam name="ParentLayout">Parent layout providing X/Y/Width/Height.</typeparam>
					/// <typeparam name="dimension_t">The drawable's intrinsic dimension type.</typeparam>
					/// <typeparam name="PrimitiveShaderType">The Primitive shader type used for rendering the rectangle.</typeparam>
					/// <typeparam name="StrokeWidth">The width of the rectangle's border in units of dimension_t. Must be greater than 0. Defaults to 1.</typeparam>
					template<typename ParentLayout,
						typename dimension_t = pixel_t,
						dimension_t StrokeWidth = 1,
						typename PrimitiveShaderType = Framework::Shader::Primitive::TemplateShader<dimension_t>
					>
					struct Rectangle : PrimitiveShaderType
					{
						static_assert(StrokeWidth > 0, "Stroke width must be greater than 0.");

						Rectangle()
						{
							PrimitiveShaderType::Prepare(ParentLayout::X(), ParentLayout::Y());
						}

						~Rectangle() = default;

						void Draw(IFrameBuffer* frame)
						{
							// Top
							PrimitiveShaderType::RectangleFill(frame,
								0, 0,
								ParentLayout::Width() - 1, StrokeWidth - 1);

							// Right
							PrimitiveShaderType::RectangleFill(frame,
								ParentLayout::Width() - 1, StrokeWidth,
								ParentLayout::Width() - 1 - StrokeWidth + 1, ParentLayout::Height() - 1 - StrokeWidth + 1);

							// Bottom
							PrimitiveShaderType::RectangleFill(frame,
								0, ParentLayout::Height() - 1 - StrokeWidth + 1,
								ParentLayout::Width() - 1, ParentLayout::Height() - 1);

							// Left
							PrimitiveShaderType::RectangleFill(frame,
								0, StrokeWidth,
								StrokeWidth - 1, ParentLayout::Height() - 1 - StrokeWidth + 1);
						}
					};
				}
			}
		}
	}
}
#endif