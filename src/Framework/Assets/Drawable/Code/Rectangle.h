#ifndef _EGFX_FRAMEWORK_ASSETS_DRAWBLE_CODE_RECTANGLE_h
#define _EGFX_FRAMEWORK_ASSETS_DRAWBLE_CODE_RECTANGLE_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace Drawable
			{
				namespace Code
				{
					namespace Rectangle
					{
						/// <summary>
						/// Draws a rectangle outlined/filled.
						/// Configurable layout and primitive shader.
						/// Primitive shader defaults to no-shader (direct drawing, single color).
						/// </summary>
						template<typename ParentLayout,
							typename PrimitiveShaderType = Framework::Shader::Primitive::NoShader<pixel_t>
						>
						struct TemplateDrawable : PrimitiveShaderType
						{
							bool Fill;

							TemplateDrawable(const bool fill = false)
								: Fill(fill)
							{
							}

							void Draw(IFrameBuffer* frame)
							{
								if (Fill)
								{
									PrimitiveShaderType::RectangleFill(frame,
										ParentLayout::X(),
										ParentLayout::Y(),
										ParentLayout::X() + ParentLayout::Width() - 1,
										ParentLayout::Y() + ParentLayout::Height() - 1);
								}
								else
								{
									PrimitiveShaderType::Line(frame,
										ParentLayout::X(),
										ParentLayout::Y(),
										ParentLayout::X() + ParentLayout::Width() - 1,
										ParentLayout::Y());
									PrimitiveShaderType::Line(frame,
										ParentLayout::X() + ParentLayout::Width() - 1,
										ParentLayout::Y(),
										ParentLayout::X() + ParentLayout::Width() - 1,
										ParentLayout::Y() + ParentLayout::Height() - 1);
									PrimitiveShaderType::Line(frame,
										ParentLayout::X() + ParentLayout::Width() - 1,
										ParentLayout::Y() + ParentLayout::Height() - 1,
										ParentLayout::X(),
										ParentLayout::Y() + ParentLayout::Height() - 1);
									PrimitiveShaderType::Line(frame,
										ParentLayout::X(),
										ParentLayout::Y() + ParentLayout::Height() - 1,
										ParentLayout::X(),
										ParentLayout::Y());
								}
							}
						};
					}
				}
			}
		}
	}
}
#endif