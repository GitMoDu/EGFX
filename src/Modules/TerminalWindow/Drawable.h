#ifndef _EGFX_MODULES_TERMINAL_WINDOW_DRAWABLE_h
#define _EGFX_MODULES_TERMINAL_WINDOW_DRAWABLE_h

#include "Constant.h"
#include "Layout.h"

namespace Egfx
{
	namespace Modules
	{
		namespace TerminalWindow
		{
			namespace Drawable
			{
				template<typename dimension_t, typename ParentLayout,
					dimension_t FontHeight,
					typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>>
				class Cursor : public Framework::Shader::Geometry::LineShader<dimension_t, PixelShaderType>
				{
				private:
					using Base = Framework::Shader::Geometry::LineShader<dimension_t, PixelShaderType>;
					using signed_t = typename Framework::AutoDimension::ByDimension<dimension_t>::signed_t;
					dimension_t OffsetX = 0;
					signed_t OffsetY = 0;

				public:
					using DrawableLayout = ParentLayout;
					bool Visible = true;

					Cursor() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height()) {}

					void SetOffsetX(const dimension_t offsetX) { OffsetX = offsetX; }
					dimension_t GetOffsetX() const { return OffsetX; }
					void SetOffsetY(const signed_t offsetY) { OffsetY = offsetY; }
					signed_t GetOffsetY() const { return OffsetY; }

					void Draw(IFrameBuffer* frame)
					{
						if (!Visible || OffsetX >= ParentLayout::Width())
							return;

						Base::Prepare(ParentLayout::X(), ParentLayout::Y());
						const signed_t lineTop = static_cast<signed_t>(ParentLayout::Height() - FontHeight) + OffsetY;
						Base::Line(frame, OffsetX, static_cast<dimension_t>(lineTop),
							OffsetX, static_cast<dimension_t>(lineTop + static_cast<signed_t>(FontHeight) - 1));
					}
				};
			}
		}
	}
}
#endif