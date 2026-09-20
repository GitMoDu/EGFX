#ifndef _INTEGERGLASS_FRAMEWORK_SHADER_IMAGE_BITMAP_SCALED_h
#define _INTEGERGLASS_FRAMEWORK_SHADER_IMAGE_BITMAP_SCALED_h

#include "Unscaled.h"

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Image
			{
				namespace Bitmap
				{
					template<typename dimension_t,
						typename color_t,
						uint8_t ScaleX = 1,
						uint8_t ScaleY = 1,
						Framework::DataSourceTypeEnum dataSourceType = Framework::DataSourceTypeEnum::Ram,
						bool UseTransparency = true,
						rgb_color_t TransparentColor = RGB_COLOR_BLACK,
						Framework::Image::ScaledTransformOrderEnum TransformOrder = Framework::Image::ScaledTransformOrderEnum::TransformThenScale,
						typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
						Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
					>
					class Scaled : public Framework::Shader::Pixel::TemplateShader<dimension_t,
						Framework::Bitmap::AbstractBitmap<dimension_t, color_t, dataSourceType>,
						ColorShaderType, TransformShaderType, BlendMode>
					{
					private:
						static_assert(ScaleX > 1 || ScaleY > 1, "ScaleX or ScaleY must be greater than 1.");

						using Base = Framework::Shader::Pixel::TemplateShader<dimension_t,
							Framework::Bitmap::AbstractBitmap<dimension_t, color_t, dataSourceType>,
							ColorShaderType, TransformShaderType, BlendMode>;

						using signed_t = typename AutoDimension::ByDimension<dimension_t>::signed_t;

						using SkipTransformTag = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
							IntegerSignal::TypeTraits::TypeDispatch::TrueType,
							IntegerSignal::TypeTraits::TypeDispatch::FalseType,
							IntegerSignal::TypeTraits::TypeDispatch::is_same<TransformShaderType, Shader::Transform::NoTransform<dimension_t>>::value>::type;

						using TransformOrderTag = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
							IntegerSignal::TypeTraits::TypeDispatch::TrueType,
							IntegerSignal::TypeTraits::TypeDispatch::FalseType,
							TransformOrder == Framework::Image::ScaledTransformOrderEnum::TransformThenScale>::type;

					public:
						Scaled(const dimension_t left, const dimension_t top,
							const dimension_t right, const dimension_t bottom)
							: Base(left, top, right, bottom) {}
						~Scaled() = default;

						void Draw(IFrameBuffer* frame, const color_t* bitmap,
							const dimension_t width, const dimension_t height,
							const dimension_t offsetX, const dimension_t offsetY)
						{
							if (width == 0 || height == 0 || !Base::ColorSource.PrepareBitmapData(bitmap, width, height))
								return;

							Draw(frame, width, height, offsetX, offsetY, SkipTransformTag{}, TransformOrderTag{});
						}

					private:
						void Draw(IFrameBuffer* frame, const dimension_t width, const dimension_t height,
							const dimension_t offsetX, const dimension_t offsetY,
							TypeTraits::TypeDispatch::TrueType,
							TypeTraits::TypeDispatch::TrueType)
						{
							DrawTransformThenScale(frame, width, height, offsetX, offsetY, TypeTraits::TypeDispatch::TrueType{});
						}

						void Draw(IFrameBuffer* frame, const dimension_t width, const dimension_t height,
							const dimension_t offsetX, const dimension_t offsetY,
							TypeTraits::TypeDispatch::FalseType,
							TypeTraits::TypeDispatch::TrueType)
						{
							DrawTransformThenScale(frame, width, height, offsetX, offsetY, TypeTraits::TypeDispatch::FalseType{});
						}

						void Draw(IFrameBuffer* frame, const dimension_t width, const dimension_t height,
							const dimension_t offsetX, const dimension_t offsetY,
							TypeTraits::TypeDispatch::TrueType,
							TypeTraits::TypeDispatch::FalseType)
						{
							DrawScaleThenTransform(frame, width, height, offsetX, offsetY, TypeTraits::TypeDispatch::TrueType{});
						}

						void Draw(IFrameBuffer* frame, const dimension_t width, const dimension_t height,
							const dimension_t offsetX, const dimension_t offsetY,
							TypeTraits::TypeDispatch::FalseType,
							TypeTraits::TypeDispatch::FalseType)
						{
							DrawScaleThenTransform(frame, width, height, offsetX, offsetY, TypeTraits::TypeDispatch::FalseType{});
						}

						template<typename TransformTag>
						void DrawTransformThenScale(IFrameBuffer* frame, const dimension_t width, const dimension_t height,
							const dimension_t offsetX, const dimension_t offsetY, TransformTag)
						{
							for (dimension_t sourceY = 0; sourceY < height; sourceY++)
							{
								for (dimension_t sourceX = 0; sourceX < width; sourceX++)
								{
									const rgb_color_t color = Base::ColorSource.Source(sourceX, sourceY);
									if (UseTransparency && color == TransparentColor)
										continue;

									dimension_t transformedX = sourceX;
									dimension_t transformedY = sourceY;
									if (!Base::TransformShader.Transform(transformedX, transformedY))
										continue;

									const rgb_color_t shadedColor = Base::ColorShader.Shade(color);
									const signed_t startX = static_cast<signed_t>(transformedX) * ScaleX + offsetX;
									const signed_t startY = static_cast<signed_t>(transformedY) * ScaleY + offsetY;
									for (dimension_t y = 0; y < ScaleY; y++)
										for (dimension_t x = 0; x < ScaleX; x++)
											Base::PixelBlend(frame, shadedColor,
												static_cast<dimension_t>(startX + x), static_cast<dimension_t>(startY + y));
								}
							}
						}

						template<typename TransformTag>
						void DrawScaleThenTransform(IFrameBuffer* frame, const dimension_t width, const dimension_t height,
							const dimension_t offsetX, const dimension_t offsetY, TransformTag)
						{
							const dimension_t scaledWidth = static_cast<dimension_t>(width * ScaleX);
							const dimension_t scaledHeight = static_cast<dimension_t>(height * ScaleY);
							const dimension_t clippedStartX = MaxValue<dimension_t>(offsetX, Base::GetBoundsLeft());
							const dimension_t clippedStartY = MaxValue<dimension_t>(offsetY, Base::GetBoundsTop());
							const dimension_t clippedEndX = MinValue<dimension_t>(offsetX + scaledWidth - 1, Base::GetBoundsRight());
							const dimension_t clippedEndY = MinValue<dimension_t>(offsetY + scaledHeight - 1, Base::GetBoundsBottom());
							if (clippedStartX > clippedEndX || clippedStartY > clippedEndY)
								return;

							for (dimension_t sourceY = 0; sourceY < height; sourceY++)
							{
								for (dimension_t sourceX = 0; sourceX < width; sourceX++)
								{
									const rgb_color_t color = Base::ColorSource.Source(sourceX, sourceY);
									if (UseTransparency && color == TransparentColor)
										continue;

									const signed_t startX = static_cast<signed_t>(sourceX) * ScaleX + offsetX;
									const signed_t startY = static_cast<signed_t>(sourceY) * ScaleY + offsetY;
									const signed_t endX = startX + ScaleX - 1;
									const signed_t endY = startY + ScaleY - 1;
									const signed_t drawStartX = MaxValue(startX, static_cast<signed_t>(clippedStartX));
									const signed_t drawStartY = MaxValue(startY, static_cast<signed_t>(clippedStartY));
									const signed_t drawEndX = MinValue(endX, static_cast<signed_t>(clippedEndX));
									const signed_t drawEndY = MinValue(endY, static_cast<signed_t>(clippedEndY));
									if (drawStartX > drawEndX || drawStartY > drawEndY)
										continue;

									const rgb_color_t shadedColor = Base::ColorShader.Shade(color);
									for (dimension_t y = drawStartY; y <= drawEndY; y++)
									{
										for (dimension_t x = drawStartX; x <= drawEndX; x++)
										{
											dimension_t transformedX = x - offsetX;
											dimension_t transformedY = y - offsetY;
											if (Base::TransformShader.Transform(transformedX, transformedY))
												Base::PixelBlend(frame, shadedColor,
													static_cast<dimension_t>(transformedX + offsetX),
													static_cast<dimension_t>(transformedY + offsetY));
										}
									}
								}
							}
						}
					};
				}
			}
		}
	}
}
#endif