#ifndef _EGFX_FRAMEWORK_BITMAP_TEMPLATE_DRAWABLE_h
#define _EGFX_FRAMEWORK_BITMAP_TEMPLATE_DRAWABLE_h

#include "TemplateDrawer.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Bitmap
		{
			/// <summary>
			/// Layout-bound drawable for a 3-3-2 (8-bit) bitmap source.
			/// Wraps a bitmap pointer and draws it using Bitmap::TemplateDrawer at ParentLayout::X()/Y().
			/// </summary>
			/// <typeparam name="ParentLayout">Parent layout providing X/Y/Width/Height.</typeparam>
			/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
			/// <typeparam name="Width">Bitmap width in pixels (source-space).</typeparam>
			/// <typeparam name="Height">Bitmap height in pixels (source-space).</typeparam>
			/// <typeparam name="BitmapReader">Bitmap reader (e.g., flash/RAM) used by TemplateDrawer.</typeparam>
			/// <typeparam name="ScaleX">Horizontal integer scale factor.</typeparam>
			/// <typeparam name="ScaleY">Vertical integer scale factor.</typeparam>
			/// <typeparam name="TransparentColor">Transparent key color (source-space RGB), if enabled.</typeparam>
			/// <typeparam name="UseTransparentColor">Enables transparent keying when true.</typeparam>
			/// <typeparam name="ColorShaderType">Optional color shader applied per pixel.</typeparam>
			/// <typeparam name="TransformShaderType">Optional transform shader applied to coordinates.</typeparam>
			template<
				typename ParentLayout,
				typename dimension_t,
				dimension_t Width,
				dimension_t Height,
				typename BitmapReader = Reader::Flash,
				uint8_t ScaleX = 1,
				uint8_t ScaleY = 1,
				rgb_color_t TransparentColor = RGB_COLOR_BLACK,
				bool UseTransparentColor = false,
				typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
				typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>
			>
			class Bitmap332Drawable : public TemplateDrawer<
				dimension_t, Width, Height,
				BitmapReader,
				ScaleX, ScaleY,
				TransparentColor, UseTransparentColor,
				ColorShaderType,
				TransformShaderType>
			{
			private:
				using Base = TemplateDrawer<
					dimension_t, Width, Height,
					BitmapReader,
					ScaleX, ScaleY,
					TransparentColor, UseTransparentColor,
					ColorShaderType,
					TransformShaderType>;

			private:
				/// <summary>Pointer to the bitmap pixel data source.</summary>
				const uint8_t* BitmapSource;

			public:
				/// <summary>Constructs a drawable from a bitmap data pointer.</summary>
				explicit Bitmap332Drawable(const uint8_t* bitmap)
					: Base()
					, BitmapSource(bitmap)
				{
				}

				~Bitmap332Drawable() = default;

				/// <summary>Draws the bitmap at ParentLayout::X(), ParentLayout::Y().</summary>
				void Draw(IFrameBuffer* framebuffer)
				{
					Base::Draw(framebuffer, ParentLayout::X(), ParentLayout::Y(), BitmapSource);
				}
			};

			/// <summary>
			/// Layout-bound drawable for a 5-6-5 (16-bit) bitmap source.
			/// Wraps a bitmap pointer and draws it using Bitmap::TemplateDrawer at ParentLayout::X()/Y().
			/// </summary>
			/// <inheritdoc />
			template<
				typename ParentLayout,
				typename dimension_t,
				dimension_t Width,
				dimension_t Height,
				typename BitmapReader = Reader::Flash,
				uint8_t ScaleX = 1,
				uint8_t ScaleY = 1,
				rgb_color_t TransparentColor = RGB_COLOR_BLACK,
				bool UseTransparentColor = false,
				typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
				typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>
			>
			class Bitmap565Drawable : public TemplateDrawer<
				dimension_t, Width, Height,
				BitmapReader,
				ScaleX, ScaleY,
				TransparentColor, UseTransparentColor,
				ColorShaderType,
				TransformShaderType>
			{
			private:
				using Base = TemplateDrawer<
					dimension_t, Width, Height,
					BitmapReader,
					ScaleX, ScaleY,
					TransparentColor, UseTransparentColor,
					ColorShaderType,
					TransformShaderType>;

			private:
				/// <summary>Pointer to the bitmap pixel data source.</summary>
				const uint16_t* BitmapSource;

			public:
				/// <summary>Constructs a drawable from a bitmap data pointer.</summary>
				explicit Bitmap565Drawable(const uint16_t* bitmap)
					: Base()
					, BitmapSource(bitmap)
				{
				}

				~Bitmap565Drawable() = default;

				/// <summary>Draws the bitmap at ParentLayout::X(), ParentLayout::Y().</summary>
				void Draw(IFrameBuffer* framebuffer)
				{
					Base::Draw(framebuffer, ParentLayout::X(), ParentLayout::Y(), BitmapSource);
				}
			};

			/// <summary>
			/// Layout-bound drawable for a 8-8-8 (32-bit) bitmap source.
			/// Wraps a bitmap pointer and draws it using Bitmap::TemplateDrawer at ParentLayout::X()/Y().
			/// </summary>
			/// <inheritdoc />
			template<
				typename ParentLayout,
				typename dimension_t,
				dimension_t Width,
				dimension_t Height,
				typename BitmapReader = Reader::Flash,
				uint8_t ScaleX = 1,
				uint8_t ScaleY = 1,
				rgb_color_t TransparentColor = RGB_COLOR_BLACK,
				bool UseTransparentColor = false,
				typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
				typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>
			>
			class Bitmap888Drawable : public TemplateDrawer<
				dimension_t, Width, Height,
				BitmapReader,
				ScaleX, ScaleY,
				TransparentColor, UseTransparentColor,
				ColorShaderType,
				TransformShaderType>
			{
			private:
				using Base = TemplateDrawer<
					dimension_t, Width, Height,
					BitmapReader,
					ScaleX, ScaleY,
					TransparentColor, UseTransparentColor,
					ColorShaderType,
					TransformShaderType>;

			private:
				/// <summary>Pointer to the bitmap pixel data source.</summary>
				const uint32_t* BitmapSource;

			public:
				/// <summary>Constructs a drawable from a bitmap data pointer.</summary>
				explicit Bitmap888Drawable(const uint32_t* bitmap)
					: Base()
					, BitmapSource(bitmap)
				{
				}

				~Bitmap888Drawable() = default;

				/// <summary>Draws the bitmap at ParentLayout::X(), ParentLayout::Y().</summary>
				void Draw(IFrameBuffer* framebuffer)
				{
					Base::Draw(framebuffer, ParentLayout::X(), ParentLayout::Y(), BitmapSource);
				}
			};
		}
	}
}
#endif