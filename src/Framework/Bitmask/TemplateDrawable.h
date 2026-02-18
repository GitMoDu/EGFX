#ifndef _EGFX_FRAMEWORK_BITMASK_TEMPLATE_DRAWABLE_h
#define _EGFX_FRAMEWORK_BITMASK_TEMPLATE_DRAWABLE_h

#include "TemplateDrawer.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Bitmask
		{
			/// <summary>
			/// Drawable wrapper for a packed 1bpp bitmask.
			///
			/// Binds a bitmask source pointer to a layout and renders it using the selected bitmask drawer
			/// (unscaled or integer scaled) backed by a primitive shader.
			/// </summary>
			/// <typeparam name="ParentLayout">Parent layout providing X/Y/Width/Height.</typeparam>
			/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
			/// <typeparam name="BitmaskWidth">Bitmask width (pixels).</typeparam>
			/// <typeparam name="BitmaskHeight">Bitmask height (pixels).</typeparam>
			/// <typeparam name="BitmaskReader">Mask reader type providing ReadByteAt(...).</typeparam>
			/// <typeparam name="ScaleX">Horizontal scale factor (integer).</typeparam>
			/// <typeparam name="ScaleY">Vertical scale factor (integer).</typeparam>
			/// <typeparam name="PrimitiveShaderType">Primitive shader type providing Prepare(...) and primitive draw calls.</typeparam>
			template<
				typename ParentLayout,
				typename dimension_t,
				dimension_t BitmaskWidth,
				dimension_t BitmaskHeight,
				typename BitmaskReader = Framework::Bitmask::Reader::Flash,
				uint8_t ScaleX = 1,
				uint8_t ScaleY = 1,
				typename PrimitiveShaderType = Shader::Primitive::TemplateShader<dimension_t>
			>
			class BitmaskDrawable : public TemplateDrawer<
				dimension_t, BitmaskWidth, BitmaskHeight,
				BitmaskReader,
				ScaleX, ScaleY,
				PrimitiveShaderType
			>
			{
			private:
				using Base = TemplateDrawer<
					dimension_t, BitmaskWidth, BitmaskHeight,
					BitmaskReader,
					ScaleX, ScaleY,
					PrimitiveShaderType>;

			public:
				/// <summary>Rendered width in pixels after scaling.</summary>
				static constexpr dimension_t Width = BitmaskWidth * ScaleX;

				/// <summary>Rendered height in pixels after scaling.</summary>
				static constexpr dimension_t Height = BitmaskHeight * ScaleY;

			private:
				/// <summary>Pointer to packed 1bpp mask bytes.</summary>
				const uint8_t* Source;

			public:
				/// <summary>
				/// Constructs the drawable from a bitmask pointer.
				/// </summary>
				/// <param name="bitmask">Pointer to packed 1bpp mask bytes.</param>
				explicit BitmaskDrawable(const uint8_t* bitmask)
					: Base()
					, Source(bitmask)
				{
				}

				~BitmaskDrawable() = default;

				/// <summary>
				/// Draws the bound bitmask at the layout origin.
				/// </summary>
				/// <param name="framebuffer">Target framebuffer to draw into.</param>
				void Draw(IFrameBuffer* framebuffer)
				{
					Base::Draw(framebuffer, ParentLayout::X(), ParentLayout::Y(), Source);
				}
			};
		}
	}
}
#endif