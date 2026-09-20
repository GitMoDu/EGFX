#ifndef _INTEGERGLASS_FRAMEWORK_SHADER_IMAGE_BITMASK_SCALED_h
#define _INTEGERGLASS_FRAMEWORK_SHADER_IMAGE_BITMASK_SCALED_h

#include "Unscaled.h"

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Image
			{
				namespace Bitmask
				{
					template<typename dimension_t,
						uint8_t ScaleX = 1,
						uint8_t ScaleY = 1,
						Framework::DataSourceTypeEnum dataSourceType = Framework::DataSourceTypeEnum::Ram,
						bool UseTransparency = true,
						rgb_color_t TransparentColor = RGB_COLOR_BLACK,
						Framework::Image::ScaledTransformOrderEnum TransformOrder = Framework::Image::ScaledTransformOrderEnum::TransformThenScale,
						typename SourceShaderType = Shader::Source::StaticColor<dimension_t>,
						typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
						Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
					>
					class Scaled : public Framework::Shader::Pixel::TemplateShader<dimension_t,
						SourceShaderType, ColorShaderType, TransformShaderType, BlendMode>
					{
					private:
						using Base = Framework::Shader::Pixel::TemplateShader<dimension_t,
							SourceShaderType, ColorShaderType, TransformShaderType, BlendMode>;

					private:
						using InvertTransparentColor = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
							IntegerSignal::TypeTraits::TypeDispatch::TrueType,
							IntegerSignal::TypeTraits::TypeDispatch::FalseType,
							(TransparentColor != RGB_COLOR_BLACK)>::type;

						using SkipShadeTag = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
							IntegerSignal::TypeTraits::TypeDispatch::TrueType,
							IntegerSignal::TypeTraits::TypeDispatch::FalseType,
							IntegerSignal::TypeTraits::TypeDispatch::is_same<ColorShaderType, Shader::Color::NoShader<dimension_t>>::value>::type;

						using SkipTransformTag = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
							IntegerSignal::TypeTraits::TypeDispatch::TrueType,
							IntegerSignal::TypeTraits::TypeDispatch::FalseType,
							IntegerSignal::TypeTraits::TypeDispatch::is_same<TransformShaderType, Shader::Transform::NoTransform<dimension_t>>::value>::type;

					private:
						/// <summary>Single-bit mask for MSB-first packed data.</summary>
						static constexpr uint8_t SingleMask = 1 << (Framework::Bitmask::BitsPerByte - 1);

					private:
						Framework::Bitmask::Source::ShaderBitmask<dimension_t, dataSourceType> MaskSource{};
						using bit_index_t = typename Framework::Bitmask::Source::ShaderBitmask<dimension_t, dataSourceType>::bit_index_t;

						dimension_t BytesPerLine = 0;
						uint8_t BytesLineRemainder = 0;
						uint8_t LastByteBitCount = 0;

					public:
						Scaled(const dimension_t left, const dimension_t top,
							const dimension_t right, const dimension_t bottom)
							: Base(left, top, right, bottom) {}
						~Scaled() = default;

						void Draw(IFrameBuffer* frame,
							const uint8_t* bitmask,
							const dimension_t width,
							const dimension_t height,
							const dimension_t offsetX,
							const dimension_t offsetY)
						{
							if (MaskSource.PrepareBitmaskData(bitmask, width, height))
							{
								// Cache intermediate values for drawing.
								BytesPerLine = static_cast<dimension_t>(width / Framework::Bitmask::BitsPerByte + ((width % Framework::Bitmask::BitsPerByte) != 0 ? 1 : 0));
								BytesLineRemainder = static_cast<uint8_t>(width % Framework::Bitmask::BitsPerByte);
								LastByteBitCount = (BytesLineRemainder == 0) ? Framework::Bitmask::BitsPerByte : BytesLineRemainder;

								DrawGeneric(frame, width, height, offsetX, offsetY);
							}
						}

					private:
						void DrawGeneric(IFrameBuffer* frame,
							const dimension_t width, const dimension_t height,
							const dimension_t offsetX, const dimension_t offsetY)
						{
							for (dimension_t row = 0; row < height; row++)
							{
								for (dimension_t xByte = 0; xByte < BytesPerLine; xByte++)
								{
									const uint8_t maskByte = ReadMaskByte(xByte, row, width, InvertTransparentColor{});
									const dimension_t baseCol = static_cast<dimension_t>(xByte * Framework::Bitmask::BitsPerByte);
									const uint8_t bitCount = (xByte + 1 < BytesPerLine) ? Framework::Bitmask::BitsPerByte : LastByteBitCount;

									for (uint_fast8_t bit = 0; bit < bitCount; bit++)
									{
										const dimension_t sourceX = static_cast<dimension_t>(baseCol + bit);
										const rgb_color_t color = (maskByte & static_cast<uint8_t>(SingleMask >> bit)) != 0
											? Base::ColorSource.Source(sourceX, row)
											: RGB_COLOR_BLACK;

										if (UseTransparency && color == TransparentColor)
											continue;

										const rgb_color_t shadedColor = Base::ColorShader.Shade(color);
										if (TransformOrder == Framework::Image::ScaledTransformOrderEnum::TransformThenScale)
										{
											dimension_t tx = sourceX;
											dimension_t ty = row;
											if (!Base::TransformShader.Transform(tx, ty))
												continue;

											DrawBigPixel(frame, shadedColor, tx, ty, offsetX, offsetY);
										}
										else
										{
											DrawTransformedBigPixel(frame, shadedColor, sourceX, row, offsetX, offsetY);
										}
									}
								}
							}
						}

						void DrawBigPixel(IFrameBuffer* frame, const rgb_color_t color,
							const dimension_t sourceX, const dimension_t sourceY, const dimension_t offsetX,
							const dimension_t offsetY)
						{
							const dimension_t startX = static_cast<dimension_t>(sourceX * ScaleX);
							const dimension_t startY = static_cast<dimension_t>(sourceY * ScaleY);
							for (dimension_t y = 0; y < ScaleY; y++)
							{
								for (dimension_t x = 0; x < ScaleX; x++)
								{
									Base::PixelBlend(frame, color,
										static_cast<dimension_t>(startX + x + offsetX),
										static_cast<dimension_t>(startY + y + offsetY));
								}
							}
						}

						void DrawTransformedBigPixel(IFrameBuffer* frame, const rgb_color_t color,
							const dimension_t sourceX, const dimension_t sourceY, const dimension_t offsetX,
							const dimension_t offsetY)
						{
							const dimension_t startX = static_cast<dimension_t>(sourceX * ScaleX);
							const dimension_t startY = static_cast<dimension_t>(sourceY * ScaleY);
							for (dimension_t y = 0; y < ScaleY; y++)
							{
								for (dimension_t x = 0; x < ScaleX; x++)
								{
									dimension_t tx = static_cast<dimension_t>(startX + x);
									dimension_t ty = static_cast<dimension_t>(startY + y);
									if (Base::TransformShader.Transform(tx, ty))
									{
										Base::PixelBlend(frame, color,
											static_cast<dimension_t>(tx + offsetX),
											static_cast<dimension_t>(ty + offsetY));
									}
								}
							}
						}

						uint8_t ReadMaskByte(const dimension_t xByte, const dimension_t row, const dimension_t width, TypeTraits::TypeDispatch::TrueType)
						{
							return static_cast<uint8_t>(~ReadMaskByte(xByte, row, width, TypeTraits::TypeDispatch::FalseType{}));
						}

						uint8_t ReadMaskByte(const dimension_t xByte, const dimension_t row, const dimension_t width, TypeTraits::TypeDispatch::FalseType)
						{
							const bit_index_t byteIndex = static_cast<bit_index_t>(row) * static_cast<bit_index_t>(BytesPerLine) + static_cast<bit_index_t>(xByte);
							const bit_index_t firstBit = byteIndex * Framework::Bitmask::BitsPerByte;
							const uint8_t bitCount = (xByte + 1 < BytesPerLine) ? Framework::Bitmask::BitsPerByte : static_cast<uint8_t>(width - (xByte * Framework::Bitmask::BitsPerByte));
							return MaskSource.ReadByte(firstBit, bitCount);
						}

						void Draw(IFrameBuffer* frame, TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::FalseType)
						{}

						void Draw(IFrameBuffer* frame, TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::TrueType)
						{}

						void Draw(IFrameBuffer* frame, TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::FalseType)
						{}

						void Draw(IFrameBuffer* frame, TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::TrueType)
						{}

					};
				}
			}
		}
	}
}
#endif