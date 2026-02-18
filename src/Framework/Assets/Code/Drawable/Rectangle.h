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
					/// <typeparam name="dimension_t">The drawable's intrinsic dimension type.</typeparam>
					/// <typeparam name="ParentLayout">Parent layout providing X/Y/Width/Height.</typeparam>
					/// <typeparam name="PixelShaderType">The pixel shader type used for rendering.</typeparam>
					template<typename dimension_t,
						typename ParentLayout,
						typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>
					>
					class RectangleFill : public Framework::Shader::Primitive::TemplateShader<dimension_t, PixelShaderType>
					{
					private:
						using Base = Framework::Shader::Primitive::TemplateShader<dimension_t, PixelShaderType>;

					public:
						/// <summary>
						/// Prepares the primitive shader origin to the rectangle's top-left corner.
						/// </summary>
						RectangleFill() : Base()
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
						}
						~RectangleFill() = default;

						/// <summary>
						/// Draws a filled rectangle using local-space coordinates (0..Width-1, 0..Height-1).
						/// The prepared origin offsets this into pixel space.
						/// </summary>
						void Draw(IFrameBuffer* frame)
						{
							Base::RectangleFill(frame,
								0, 0,
								ParentLayout::Width() - 1, ParentLayout::Height() - 1);
						}
					};

					/// <summary>
					/// Draws a rectangle outline with a specified stroke width using a primitive shader.
					/// </summary>
					/// <typeparam name="ParentLayout">Parent layout providing X/Y/Width/Height.</typeparam>
					/// <typeparam name="dimension_t">The drawable's intrinsic dimension type.</typeparam>
					/// <typeparam name="PixelShaderType">The Pixel shader type used for rendering the rectangle.</typeparam>
					/// <typeparam name="StrokeWidth">The width of the rectangle's border in units of dimension_t. Must be greater than 0. Defaults to 1.</typeparam>
					template<typename dimension_t,
						typename ParentLayout,
						dimension_t StrokeWidth = 1,
						typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>
					>
					class Rectangle : public Framework::Shader::Primitive::TemplateShader<dimension_t, PixelShaderType>
					{
					private:
						static_assert(StrokeWidth > 0, "Stroke width must be greater than 0.");

						using Base = Framework::Shader::Primitive::TemplateShader<dimension_t, PixelShaderType>;

					public:
						Rectangle() : Base()
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
						}

						~Rectangle() = default;

						void Draw(IFrameBuffer* frame)
						{
							// Top
							Base::RectangleFill(frame,
								0, 0,
								ParentLayout::Width() - 1, StrokeWidth - 1);

							// Right
							Base::RectangleFill(frame,
								ParentLayout::Width() - 1, StrokeWidth,
								ParentLayout::Width() - 1 - StrokeWidth + 1, ParentLayout::Height() - 1 - StrokeWidth + 1);

							// Bottom
							Base::RectangleFill(frame,
								0, ParentLayout::Height() - 1 - StrokeWidth + 1,
								ParentLayout::Width() - 1, ParentLayout::Height() - 1);

							// Left
							Base::RectangleFill(frame,
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