#ifndef _EGFX_FRAMEWORK_BITMASK_TEMPLATE_DRAWER_h
#define _EGFX_FRAMEWORK_BITMASK_TEMPLATE_DRAWER_h

#include "Model.h"
#include "../Shader/Primitive/Shaders.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Bitmask
		{
			/// <summary>
			/// Bitmask drawer that renders a 1bpp mask using a primitive shader.
			///
			/// The mask is stored as packed bits (MSB-first per byte). Each set bit is rendered using the
			/// supplied primitive shader implementation.
			/// </summary>
			/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
			/// <typeparam name="MaskWidth">Bitmask width (pixels).</typeparam>
			/// <typeparam name="MaskHeight">Bitmask height (pixels).</typeparam>
			/// <typeparam name="MaskReader">Mask reader type providing ReadByteAt(...).</typeparam>
			/// <typeparam name="PrimitiveShaderType">Primitive shader type providing Prepare(...) and primitive draw calls.</typeparam>
			template<typename dimension_t,
				dimension_t MaskWidth,
				dimension_t MaskHeight,
				typename MaskReader = Reader::Flash,
				typename PrimitiveShaderType = Shader::Primitive::TemplateShader<dimension_t>
			>
			struct TemplateDrawerNoScale : PrimitiveShaderType
			{
			private:
				using intermediate_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;

			private:
				/// <summary>Packed byte width of a scanline.</summary>
				static constexpr intermediate_t BitScale =
					(intermediate_t)MaskWidth / BitsPerByte + (((intermediate_t)MaskWidth % BitsPerByte) != 0 ? 1 : 0);

				/// <summary>Byte width of a scanline (packed pixels).</summary>
				static constexpr dimension_t BytesPerLine = static_cast<dimension_t>(BitScale);

				/// <summary>Remainder bits in the final byte of a scanline.</summary>
				static constexpr uint8_t BytesLineRemainder = static_cast<uint8_t>(MaskWidth % BitsPerByte);

				/// <summary>Number of valid bits in the last byte of each scanline.</summary>
				static constexpr uint8_t LastByteBitCount = (BytesLineRemainder == 0) ? BitsPerByte : BytesLineRemainder;

				/// <summary>Single-bit mask for MSB-first packed data.</summary>
				static constexpr uint8_t SingleMask = 1 << (BitsPerByte - 1);

			public:
				TemplateDrawerNoScale() : PrimitiveShaderType() {}
				~TemplateDrawerNoScale() = default;

				/// <summary>
				/// Draws the mask at the given pixel-space origin.
				/// </summary>
				/// <param name="framebuffer">Target framebuffer to draw into.</param>
				/// <param name="x">Pixel-space origin X (pixels).</param>
				/// <param name="y">Pixel-space origin Y (pixels).</param>
				/// <param name="mask">Pointer to packed 1bpp mask bytes.</param>
				void Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const uint8_t* mask)
				{
					PrimitiveShaderType::Prepare(x, y);

					for (dimension_t row = 0; row < MaskHeight; row++)
					{
						for (dimension_t xByte = 0; xByte < BytesPerLine; xByte++)
						{
							const uint8_t maskByte = MaskReader::template ReadByteAt<dimension_t, BytesPerLine>(mask, xByte, row);

							if (maskByte == 0)
							{
								continue;
							}

							const dimension_t baseCol = static_cast<dimension_t>(xByte) * BitsPerByte;
							const uint8_t bitCount = (xByte + 1 < BytesPerLine) ? BitsPerByte : LastByteBitCount;

							// Fast path: full byte set (only valid when all 8 bits are part of the mask).
							if (MaskWidth >= BitsPerByte && maskByte == UINT8_MAX)
							{
								for (uint_fast8_t bit = 0; bit < bitCount; bit++)
								{
									PrimitiveShaderType::Pixel(framebuffer, static_cast<dimension_t>(baseCol + bit), row);
								}
							}
							else
							{
								for (uint_fast8_t bit = 0; bit < bitCount; bit++)
								{
									if ((maskByte & static_cast<uint8_t>(SingleMask >> bit)) != 0)
									{
										PrimitiveShaderType::Pixel(framebuffer, static_cast<dimension_t>(baseCol + bit), row);
									}
								}
							}
						}
					}
				}
			};

			/// <summary>
			/// Bitmask drawer that renders a 1bpp mask using integer scaling.
			///
			/// Each set bit is expanded into a filled rectangle of size (ScaleX x ScaleY) in local space.
			/// </summary>
			/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
			/// <typeparam name="MaskWidth">Bitmask width (pixels).</typeparam>
			/// <typeparam name="MaskHeight">Bitmask height (pixels).</typeparam>
			/// <typeparam name="MaskReader">Mask reader type providing ReadByteAt(...).</typeparam>
			/// <typeparam name="ScaleX">Horizontal scale factor (integer).</typeparam>
			/// <typeparam name="ScaleY">Vertical scale factor (integer).</typeparam>
			/// <typeparam name="PrimitiveShaderType">Primitive shader type providing Prepare(...) and primitive draw calls.</typeparam>
			template<typename dimension_t,
				dimension_t MaskWidth,
				dimension_t MaskHeight,
				typename MaskReader = Reader::Flash,
				uint8_t ScaleX = 1,
				uint8_t ScaleY = ScaleX,
				typename PrimitiveShaderType = Shader::Primitive::TemplateShader<dimension_t>
			>
			struct TemplateDrawerIntegerScale : PrimitiveShaderType
			{
			private:
				using intermediate_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;

			private:
				static constexpr intermediate_t BitScale =
					(intermediate_t)MaskWidth / BitsPerByte + (((intermediate_t)MaskWidth % BitsPerByte) != 0 ? 1 : 0);

				static constexpr dimension_t BytesPerLine = static_cast<dimension_t>(BitScale);
				static constexpr uint8_t BytesLineRemainder = static_cast<uint8_t>(MaskWidth % BitsPerByte);

				static constexpr uint8_t LastByteBitCount = (BytesLineRemainder == 0) ? BitsPerByte : BytesLineRemainder;
				static constexpr uint8_t SingleMask = 1 << (BitsPerByte - 1);

			public:
				TemplateDrawerIntegerScale() : PrimitiveShaderType() {}
				~TemplateDrawerIntegerScale() = default;

				/// <summary>
				/// Draws the mask at the given pixel-space origin.
				/// </summary>
				/// <param name="framebuffer">Target framebuffer to draw into.</param>
				/// <param name="x">Pixel-space origin X (pixels).</param>
				/// <param name="y">Pixel-space origin Y (pixels).</param>
				/// <param name="mask">Pointer to packed 1bpp mask bytes.</param>
				void Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const uint8_t* mask)
				{
					PrimitiveShaderType::Prepare(x, y);

					for (dimension_t row = 0; row < MaskHeight; row++)
					{
						for (dimension_t xByte = 0; xByte < BytesPerLine; xByte++)
						{
							const uint8_t maskByte = MaskReader::template ReadByteAt<dimension_t, BytesPerLine>(mask, xByte, row);

							if (maskByte == 0)
							{
								continue;
							}

							const dimension_t baseCol = static_cast<dimension_t>(xByte) * BitsPerByte;
							const uint8_t bitCount = (xByte + 1 < BytesPerLine) ? BitsPerByte : LastByteBitCount;

							// Fast path: full byte set (only valid when all 8 bits are part of the mask).
							if (MaskWidth >= BitsPerByte && maskByte == UINT8_MAX)
							{
								for (uint_fast8_t bit = 0; bit < bitCount; bit++)
								{
									const pixel_t x1 = static_cast<pixel_t>(baseCol + bit) * ScaleX;
									const pixel_t y1 = static_cast<pixel_t>(row) * ScaleY;

									PrimitiveShaderType::RectangleFill(framebuffer,
										x1, y1,
										x1 + (ScaleX - 1), y1 + (ScaleY - 1));
								}
							}
							else
							{
								for (uint_fast8_t bit = 0; bit < bitCount; bit++)
								{
									if ((maskByte & static_cast<uint8_t>(SingleMask >> bit)) != 0)
									{
										const pixel_t x1 = static_cast<pixel_t>(baseCol + bit) * ScaleX;
										const pixel_t y1 = static_cast<pixel_t>(row) * ScaleY;

										PrimitiveShaderType::RectangleFill(framebuffer,
											x1, y1,
											x1 + (ScaleX - 1), y1 + (ScaleY - 1));
									}
								}
							}
						}
					}
				}
			};

			/// <summary>
			/// Bitmask drawer selector.
			/// Uses optimized TemplateDrawerNoScale when ScaleX == 1 and ScaleY == 1, otherwise uses TemplateDrawerIntegerScale.
			/// </summary>
			/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
			/// <typeparam name="MaskWidth">Bitmask width (pixels).</typeparam>
			/// <typeparam name="MaskHeight">Bitmask height (pixels).</typeparam>
			/// <typeparam name="MaskReader">Mask reader type providing ReadByteAt(...).</typeparam>
			/// <typeparam name="ScaleX">Horizontal scale factor (integer).</typeparam>
			/// <typeparam name="ScaleY">Vertical scale factor (integer).</typeparam>
			/// <typeparam name="PrimitiveShaderType">Primitive shader type providing Prepare(...) and primitive draw calls.</typeparam>
			template<typename dimension_t,
				dimension_t MaskWidth,
				dimension_t MaskHeight,
				typename MaskReader = Reader::Flash,
				uint8_t ScaleX = 1,
				uint8_t ScaleY = 1,
				typename PrimitiveShaderType = Shader::Primitive::TemplateShader<dimension_t>
			>
			using TemplateDrawer = typename TypeTraits::TypeConditional::conditional_type<
				TemplateDrawerNoScale<dimension_t, MaskWidth, MaskHeight, MaskReader, PrimitiveShaderType>,
				TemplateDrawerIntegerScale<dimension_t, MaskWidth, MaskHeight, MaskReader, ScaleX, ScaleY, PrimitiveShaderType>,
				(ScaleX == 1 && ScaleY == 1)>::type;
		}
	}
}
#endif