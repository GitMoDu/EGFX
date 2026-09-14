#ifndef _EGFX_FRAMEWORK_SHADER_IMAGE_BITMASK_UNSCALED_h
#define _EGFX_FRAMEWORK_SHADER_IMAGE_BITMASK_UNSCALED_h

#include "../../../Bitmask/Model.h"
#include "../../../Bitmask/Source.h"
#include "../../Color/Shaders.h"
#include "../../Transform/Shaders.h"

namespace Egfx
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
						Framework::DataSourceTypeEnum dataSourceType = Framework::DataSourceTypeEnum::Ram,
						bool UseTransparency = true,
						rgb_color_t TransparentColor = RGB_COLOR_BLACK,
						typename SourceShaderType = Shader::Source::StaticColor<dimension_t>,
						typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
						Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
					>
					class Unscaled : public Framework::Shader::Pixel::TemplateShader<dimension_t,
						SourceShaderType, ColorShaderType, TransformShaderType, BlendMode
					>
					{
					private:
						using Base = Framework::Shader::Pixel::TemplateShader<dimension_t,
							SourceShaderType, ColorShaderType, TransformShaderType, BlendMode>;

					private:
						using InvertTransparentColor = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
							IntegerSignal::TypeTraits::TypeDispatch::TrueType,
							IntegerSignal::TypeTraits::TypeDispatch::FalseType,
							(TransparentColor != RGB_COLOR_BLACK)>::type;

						using SkipTransparency = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
							IntegerSignal::TypeTraits::TypeDispatch::FalseType,
							IntegerSignal::TypeTraits::TypeDispatch::TrueType,
							UseTransparency>::type;

						// Compile-time dispatch: skip per-pixel source sampling only when the source is constant
						using SkipShadeTag = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
							IntegerSignal::TypeTraits::TypeDispatch::TrueType,
							IntegerSignal::TypeTraits::TypeDispatch::FalseType,
							IntegerSignal::TypeTraits::TypeDispatch::is_same<ColorShaderType, Shader::Color::NoShader<dimension_t>>::value&&
							IntegerSignal::TypeTraits::TypeDispatch::is_same<TransformShaderType, Shader::Transform::NoTransform<dimension_t>>::value
						>::type;

						// Compile-time dispatch: skip Transform(...) when the transform shader is NoTransform.
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
						Unscaled(const dimension_t left, const dimension_t top,
							const dimension_t right, const dimension_t bottom)
							: Base(left, top, right, bottom) {}
						~Unscaled() = default;

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

								Draw(frame,
									width, height, offsetX, offsetY,
									SkipTransparency{}, SkipShadeTag{}, SkipTransformTag{});
							}
						}

					private:
						// Draw with color shading, transparency check and transform.
						void Draw(IFrameBuffer* frame,
							const dimension_t width,
							const dimension_t height,
							const dimension_t offsetX,
							const dimension_t offsetY,
							TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::FalseType)
						{
							for (dimension_t row = 0; row < height; row++)
							{
								for (dimension_t xByte = 0; xByte < BytesPerLine; xByte++)
								{
									const uint8_t maskByte = ReadMaskByte(xByte, row, width, InvertTransparentColor{});

									// Full byte is zero, skip the entire byte (8 pixels).
									if (maskByte == 0)
									{
										continue;
									}

									const dimension_t baseCol = xByte * Framework::Bitmask::BitsPerByte;
									const uint8_t bitCount = (xByte + 1 < BytesPerLine) ? Framework::Bitmask::BitsPerByte : LastByteBitCount;

									for (uint_fast8_t bit = 0; bit < bitCount; bit++)
									{
										// Check if the bit is set in the mask byte.
										if ((maskByte & static_cast<uint8_t>(SingleMask >> bit)) != 0)
										{
											dimension_t tx = baseCol + bit;
											dimension_t ty = row;
											if (Base::TransformShader.Transform(tx, ty))
											{
												const rgb_color_t color = Base::ColorSource.Source(baseCol + bit, row);
												Base::PixelBlend(frame, Base::ColorShader.Shade(color),
													tx + offsetX, ty + offsetY);
											}
										}
									}
								}
							}
						}

						// Draw with color shading and transform, skipping transparency check.
						void Draw(IFrameBuffer* frame,
							const dimension_t width,
							const dimension_t height,
							const dimension_t offsetX,
							const dimension_t offsetY,
							TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::FalseType)
						{
							for (dimension_t row = 0; row < height; row++)
							{
								for (dimension_t xByte = 0; xByte < BytesPerLine; xByte++)
								{
									const uint8_t maskByte = ReadMaskByte(xByte, row, width, TypeTraits::TypeDispatch::FalseType{});

									const dimension_t baseCol = xByte * Framework::Bitmask::BitsPerByte;
									const uint8_t bitCount = (xByte + 1 < BytesPerLine) ? Framework::Bitmask::BitsPerByte : LastByteBitCount;

									for (uint_fast8_t bit = 0; bit < bitCount; bit++)
									{
										dimension_t tx = baseCol + bit;
										dimension_t ty = row;
										if (Base::TransformShader.Transform(tx, ty))
										{
											// Use the mask byte to determine the pixel color. If the bit is set, use the source color; otherwise, use black.
											const rgb_color_t color = ((maskByte & static_cast<uint8_t>(SingleMask >> bit)) != 0) ?
												Base::ColorSource.Source(baseCol + bit, row) : RGB_COLOR_BLACK;
											Base::PixelBlend(frame, Base::ColorShader.Shade(color),
												tx + offsetX, ty + offsetY);
										}
									}
								}
							}
						}

						// Draw with transparency check and transform, skipping color shading.
						void Draw(IFrameBuffer* frame,
							const dimension_t width,
							const dimension_t height,
							const dimension_t offsetX,
							const dimension_t offsetY,
							TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::FalseType)
						{
							for (dimension_t row = 0; row < height; row++)
							{
								for (dimension_t xByte = 0; xByte < BytesPerLine; xByte++)
								{
									const uint8_t maskByte = ReadMaskByte(xByte, row, width, InvertTransparentColor{});

									// Full byte is zero, skip the entire byte (8 pixels).
									if (maskByte == 0)
									{
										continue;
									}

									const dimension_t baseCol = xByte * Framework::Bitmask::BitsPerByte;
									const uint8_t bitCount = (xByte + 1 < BytesPerLine) ? Framework::Bitmask::BitsPerByte : LastByteBitCount;

									for (uint_fast8_t bit = 0; bit < bitCount; bit++)
									{
										// Check if the bit is set in the mask byte.
										if ((maskByte & static_cast<uint8_t>(SingleMask >> bit)) != 0)
										{
											dimension_t tx = baseCol + bit;
											dimension_t ty = row;
											if (Base::TransformShader.Transform(tx, ty))
											{
												const rgb_color_t color = Base::ColorSource.Source(baseCol + bit, row);
												Base::PixelBlend(frame, color, tx + offsetX, ty + offsetY);
											}
										}
									}
								}
							}
						}

						// Draw with transparency check and transform, skipping color shading.
						void Draw(IFrameBuffer* frame,
							const dimension_t width,
							const dimension_t height,
							const dimension_t offsetX,
							const dimension_t offsetY,
							TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::FalseType)
						{
							for (dimension_t row = 0; row < height; row++)
							{
								for (dimension_t xByte = 0; xByte < BytesPerLine; xByte++)
								{
									const uint8_t maskByte = ReadMaskByte(xByte, row, width, TypeTraits::TypeDispatch::FalseType{});

									const dimension_t baseCol = xByte * Framework::Bitmask::BitsPerByte;
									const uint8_t bitCount = (xByte + 1 < BytesPerLine) ? Framework::Bitmask::BitsPerByte : LastByteBitCount;

									for (uint_fast8_t bit = 0; bit < bitCount; bit++)
									{
										dimension_t tx = baseCol + bit;
										dimension_t ty = row;
										if (Base::TransformShader.Transform(tx, ty))
										{
											// Use the mask byte to determine the pixel color. If the bit is set, use the source color; otherwise, use black.
											const rgb_color_t color = ((maskByte & static_cast<uint8_t>(SingleMask >> bit)) != 0) ?
												Base::ColorSource.Source(baseCol + bit, row) : RGB_COLOR_BLACK;
											Base::PixelBlend(frame, color, tx + offsetX, ty + offsetY);
										}
									}
								}
							}
						}


						// Draw with color shading and transparency check, skipping transform.
						void Draw(IFrameBuffer* frame,
							const dimension_t width,
							const dimension_t height,
							const dimension_t offsetX,
							const dimension_t offsetY,
							TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::TrueType)
						{
							const dimension_t clippedStartY = MaxValue<dimension_t>(offsetY, Base::GetBoundsTop());
							const dimension_t clippedEndY = MinValue<dimension_t>(offsetY + height - 1, Base::GetBoundsBottom());
							const dimension_t clippedStartX = MaxValue<dimension_t>(offsetX, Base::GetBoundsLeft());
							const dimension_t clippedEndX = MinValue<dimension_t>(offsetX + width - 1, Base::GetBoundsRight());
							if (clippedStartX > clippedEndX || clippedStartY > clippedEndY)
								return;

							const dimension_t firstByte = static_cast<dimension_t>((clippedStartX - offsetX) / Framework::Bitmask::BitsPerByte);
							const dimension_t lastByte = static_cast<dimension_t>((clippedEndX - offsetX) / Framework::Bitmask::BitsPerByte);
							for (dimension_t y = clippedStartY; y <= clippedEndY; y++)
							{
								const dimension_t sourceY = static_cast<dimension_t>(y - offsetY);
								for (dimension_t xByte = firstByte; xByte <= lastByte; xByte++)
								{
									const uint8_t maskByte = ReadMaskByte(xByte, sourceY, width, InvertTransparentColor{});
									const dimension_t baseCol = static_cast<dimension_t>(xByte * Framework::Bitmask::BitsPerByte);
									const uint8_t bitCount = (xByte + 1 < BytesPerLine) ? Framework::Bitmask::BitsPerByte : LastByteBitCount;
									for (uint_fast8_t bit = 0; bit < bitCount; bit++)
									{
										const dimension_t sourceX = static_cast<dimension_t>(baseCol + bit);
										const dimension_t targetX = static_cast<dimension_t>(sourceX + offsetX);
										if (targetX >= clippedStartX && targetX <= clippedEndX &&
											(maskByte & static_cast<uint8_t>(SingleMask >> bit)) != 0)
										{
											Base::PixelBlendUnclipped(frame,
												Base::ColorShader.Shade(Base::ColorSource.Source(sourceX, sourceY)),
												targetX, y);
										}
									}
								}
							}
						}

						// Draw with color shading, skipping transparency check and transform.
						void Draw(IFrameBuffer* frame,
							const dimension_t width,
							const dimension_t height,
							const dimension_t offsetX,
							const dimension_t offsetY,
							TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::TrueType)
						{
							const dimension_t clippedStartY = MaxValue<dimension_t>(offsetY, Base::GetBoundsTop());
							const dimension_t clippedEndY = MinValue<dimension_t>(offsetY + height - 1, Base::GetBoundsBottom());
							const dimension_t clippedStartX = MaxValue<dimension_t>(offsetX, Base::GetBoundsLeft());
							const dimension_t clippedEndX = MinValue<dimension_t>(offsetX + width - 1, Base::GetBoundsRight());
							if (clippedStartX > clippedEndX || clippedStartY > clippedEndY)
								return;

							const dimension_t firstByte = static_cast<dimension_t>((clippedStartX - offsetX) / Framework::Bitmask::BitsPerByte);
							const dimension_t lastByte = static_cast<dimension_t>((clippedEndX - offsetX) / Framework::Bitmask::BitsPerByte);
							for (dimension_t y = clippedStartY; y <= clippedEndY; y++)
							{
								const dimension_t sourceY = static_cast<dimension_t>(y - offsetY);
								for (dimension_t xByte = firstByte; xByte <= lastByte; xByte++)
								{
									const uint8_t maskByte = ReadMaskByte(xByte, sourceY, width, TypeTraits::TypeDispatch::FalseType{});
									const dimension_t baseCol = static_cast<dimension_t>(xByte * Framework::Bitmask::BitsPerByte);
									const uint8_t bitCount = (xByte + 1 < BytesPerLine) ? Framework::Bitmask::BitsPerByte : LastByteBitCount;
									for (uint_fast8_t bit = 0; bit < bitCount; bit++)
									{
										const dimension_t sourceX = static_cast<dimension_t>(baseCol + bit);
										const dimension_t targetX = static_cast<dimension_t>(sourceX + offsetX);
										if (targetX >= clippedStartX && targetX <= clippedEndX)
										{
											const rgb_color_t color = (maskByte & static_cast<uint8_t>(SingleMask >> bit)) != 0 ?
												Base::ColorSource.Source(sourceX, sourceY) : RGB_COLOR_BLACK;
											Base::PixelBlendUnclipped(frame, Base::ColorShader.Shade(color), targetX, y);
										}
									}
								}
							}
						}

						// Draw with transparency check, skipping color shading and transform.
						void Draw(IFrameBuffer* frame,
							const dimension_t width,
							const dimension_t height,
							const dimension_t offsetX,
							const dimension_t offsetY,
							TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::TrueType)
						{
							const dimension_t clippedStartY = MaxValue<dimension_t>(offsetY, Base::GetBoundsTop());
							const dimension_t clippedEndY = MinValue<dimension_t>(offsetY + height - 1, Base::GetBoundsBottom());
							const dimension_t clippedStartX = MaxValue<dimension_t>(offsetX, Base::GetBoundsLeft());
							const dimension_t clippedEndX = MinValue<dimension_t>(offsetX + width - 1, Base::GetBoundsRight());
							if (clippedStartX > clippedEndX || clippedStartY > clippedEndY)
								return;

							const dimension_t firstByte = static_cast<dimension_t>((clippedStartX - offsetX) / Framework::Bitmask::BitsPerByte);
							const dimension_t lastByte = static_cast<dimension_t>((clippedEndX - offsetX) / Framework::Bitmask::BitsPerByte);
							for (dimension_t y = clippedStartY; y <= clippedEndY; y++)
							{
								const dimension_t sourceY = static_cast<dimension_t>(y - offsetY);
								for (dimension_t xByte = firstByte; xByte <= lastByte; xByte++)
								{
									const uint8_t maskByte = ReadMaskByte(xByte, sourceY, width, InvertTransparentColor{});
									const dimension_t baseCol = static_cast<dimension_t>(xByte * Framework::Bitmask::BitsPerByte);
									const uint8_t bitCount = (xByte + 1 < BytesPerLine) ? Framework::Bitmask::BitsPerByte : LastByteBitCount;
									for (uint_fast8_t bit = 0; bit < bitCount; bit++)
									{
										const dimension_t sourceX = static_cast<dimension_t>(baseCol + bit);
										const dimension_t targetX = static_cast<dimension_t>(sourceX + offsetX);
										if (targetX >= clippedStartX && targetX <= clippedEndX &&
											(maskByte & static_cast<uint8_t>(SingleMask >> bit)) != 0)
										{
											Base::PixelBlendUnclipped(frame,
												Base::ColorSource.Source(sourceX, sourceY), targetX, y);
										}
									}
								}
							}
						}

						// Draw skipping color shading, transparency check, and transform.
						void Draw(IFrameBuffer* frame,
							const dimension_t width,
							const dimension_t height,
							const dimension_t offsetX,
							const dimension_t offsetY,
							TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::TrueType)
						{
							const dimension_t clippedStartY = MaxValue<dimension_t>(offsetY, Base::GetBoundsTop());
							const dimension_t clippedEndY = MinValue<dimension_t>(offsetY + height - 1, Base::GetBoundsBottom());
							const dimension_t clippedStartX = MaxValue<dimension_t>(offsetX, Base::GetBoundsLeft());
							const dimension_t clippedEndX = MinValue<dimension_t>(offsetX + width - 1, Base::GetBoundsRight());
							if (clippedStartX > clippedEndX || clippedStartY > clippedEndY)
								return;

							const dimension_t firstByte = static_cast<dimension_t>((clippedStartX - offsetX) / Framework::Bitmask::BitsPerByte);
							const dimension_t lastByte = static_cast<dimension_t>((clippedEndX - offsetX) / Framework::Bitmask::BitsPerByte);
							for (dimension_t y = clippedStartY; y <= clippedEndY; y++)
							{
								const dimension_t sourceY = static_cast<dimension_t>(y - offsetY);
								for (dimension_t xByte = firstByte; xByte <= lastByte; xByte++)
								{
									const uint8_t maskByte = ReadMaskByte(xByte, sourceY, width, TypeTraits::TypeDispatch::FalseType{});
									const dimension_t baseCol = static_cast<dimension_t>(xByte * Framework::Bitmask::BitsPerByte);
									const uint8_t bitCount = (xByte + 1 < BytesPerLine) ? Framework::Bitmask::BitsPerByte : LastByteBitCount;
									for (uint_fast8_t bit = 0; bit < bitCount; bit++)
									{
										const dimension_t sourceX = static_cast<dimension_t>(baseCol + bit);
										const dimension_t targetX = static_cast<dimension_t>(sourceX + offsetX);
										if (targetX >= clippedStartX && targetX <= clippedEndX &&
											(maskByte & static_cast<uint8_t>(SingleMask >> bit)) != 0)
										{
											Base::PixelBlendUnclipped(frame, Base::ColorSource.Source(sourceX, sourceY), targetX, y);
										}
									}
								}
							}
						}


						// Read mask byte with inverted color.
						uint8_t ReadMaskByte(const dimension_t xByte, const dimension_t row, const dimension_t width, TypeTraits::TypeDispatch::TrueType)
						{
							return static_cast<uint8_t>(~ReadMaskByte(xByte, row, width, TypeTraits::TypeDispatch::FalseType{}));
						}

						// Read mask byte without inverted color.
						uint8_t ReadMaskByte(const dimension_t xByte, const dimension_t row, const dimension_t /*width*/, TypeTraits::TypeDispatch::FalseType)
						{
							const bit_index_t byteIndex = static_cast<bit_index_t>(row) * static_cast<bit_index_t>(BytesPerLine) + static_cast<bit_index_t>(xByte);
							const bit_index_t firstBit = byteIndex * Framework::Bitmask::BitsPerByte;
							const uint8_t bitCount = (xByte + 1 < BytesPerLine) ? Framework::Bitmask::BitsPerByte : LastByteBitCount;
							return MaskSource.ReadByte(firstBit, bitCount);
						}
					};
				}
			}
		}
	}
}
#endif