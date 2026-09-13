#ifndef _EGFX_FRAMEWORK_SHADER_IMAGE_BITMAP_UNSCALED_h
#define _EGFX_FRAMEWORK_SHADER_IMAGE_BITMAP_UNSCALED_h

#include "../../../Bitmap/Abstract.h"
#include "../../Color/Shaders.h"
#include "../../Transform/Shaders.h"
#include "../../Primitive/Shaders.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Image
			{
				namespace Bitmap
				{
					/// <summary>
					/// Bitmap image shader that draws a static bitmap without scaling.
					/// //TODO: Add chunk rendering for partial renders on each drawable call.
					/// </summary>
					template<typename dimension_t,
						typename color_t,
						Framework::DataSourceTypeEnum dataSourceType = Framework::DataSourceTypeEnum::Ram,
						bool UseTransparency = true,
						rgb_color_t TransparentColor = RGB_COLOR_BLACK,
						typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
						Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
					>
					class Unscaled : public Framework::Shader::Pixel::TemplateShader<dimension_t,
						Framework::Bitmap::AbstractBitmap<dimension_t, color_t, dataSourceType>,
						ColorShaderType, TransformShaderType
					>
					{
					private:
						using Base = Framework::Shader::Pixel::TemplateShader < dimension_t,
							Framework::Bitmap::AbstractBitmap<dimension_t, color_t, dataSourceType>,
							ColorShaderType, TransformShaderType>;

					private:
						// Compile-time dispatch: skip per-pixel transparency check only when UseTransparency is false
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

					public:
						Unscaled(const dimension_t left, const dimension_t top,
							const dimension_t right, const dimension_t bottom)
							: Base(left, top, right, bottom) {}
						~Unscaled() = default;

						void Draw(IFrameBuffer* frame,
							const color_t* bitmap,
							const dimension_t width,
							const dimension_t height,
							const dimension_t offsetX,
							const dimension_t offsetY)
						{
							if (Base::ColorSource.PrepareBitmapData(bitmap, width, height))
							{
								Draw(frame,
									bitmap, width, height, offsetX, offsetY,
									SkipTransparency{}, SkipShadeTag{}, SkipTransformTag{});
							}
						}

					private:
						// Draw with transparency check, color shading, and transform.
						void Draw(IFrameBuffer* frame, const color_t* bitmap,
							const dimension_t width, const dimension_t height,
							const dimension_t offsetX, const dimension_t offsetY,
							TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::FalseType)
						{
							dimension_t y = 0;
							while (true)
							{
								for (dimension_t x = 0; x < width; x++)
								{
									const rgb_color_t color = Base::ColorSource.Source(x, y);
									if (color == TransparentColor)
										continue;

									dimension_t tx = x;
									dimension_t ty = y;
									if (Base::TransformShader.Transform(tx, ty))
									{
										Base::PixelBlend(frame, Base::ColorShader.Shade(color),
											tx + offsetX, ty + offsetY);
									}
								}

								if (y == height - 1)
								{
									break;
								}
								++y;
							}
						}

						// Draw with color shading and transform, skipping transparency check.
						void Draw(IFrameBuffer* frame, const color_t* bitmap,
							const dimension_t width, const dimension_t height,
							const dimension_t offsetX, const dimension_t offsetY,
							TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::FalseType)
						{
							dimension_t y = 0;
							while (true)
							{
								for (dimension_t x = 0; x < width; x++)
								{
									dimension_t tx = x;
									dimension_t ty = y;
									if (Base::TransformShader.Transform(tx, ty))
									{
										Base::PixelBlend(frame,
											Base::ColorShader.Shade(Base::ColorSource.Source(x, y)),
											tx + offsetX, ty + offsetY);
									}
								}

								if (y == height - 1)
								{
									break;
								}
								++y;
							}
						}

						// Draw with color shading and transparency check, skipping transform.
						void Draw(IFrameBuffer* frame, const color_t* bitmap,
							const dimension_t width, const dimension_t height,
							const dimension_t offsetX, const dimension_t offsetY,
							TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::TrueType)
						{
							const dimension_t clippedStartY = MaxValue<dimension_t>(offsetY, Base::GetBoundsTop());
							const dimension_t clippedEndY = MinValue<dimension_t>(offsetY + height - 1, Base::GetBoundsBottom());
							if (clippedStartY > clippedEndY)
								return;

							const dimension_t clippedStartX = MaxValue<dimension_t>(offsetX, Base::GetBoundsLeft());
							const dimension_t clippedEndX = MinValue<dimension_t>(offsetX + width - 1, Base::GetBoundsRight());
							if (clippedStartX > clippedEndX)
								return;

							dimension_t y = clippedStartY;
							while (true)
							{
								for (dimension_t x = clippedStartX; x <= clippedEndX; x++)
								{
									const dimension_t sourceX = static_cast<dimension_t>(x - offsetX);
									const dimension_t sourceY = static_cast<dimension_t>(y - offsetY);
									const rgb_color_t color = Base::ColorSource.Source(sourceX, sourceY);
									if (color != TransparentColor)
									{
										// Clipping has been performed, so we can use the unclipped version of PixelBlend.
										Base::PixelBlendUnclipped(frame,
											Base::ColorShader.Shade(color),
											sourceX + offsetX, sourceY + offsetY);
									}
								}

								if (y == clippedEndY)
								{
									break;
								}
								++y;
							}
						}

						// Draw with color shading, skipping transparency check and transform.
						void Draw(IFrameBuffer* frame, const color_t* bitmap,
							const dimension_t width, const dimension_t height,
							const dimension_t offsetX, const dimension_t offsetY,
							TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::TrueType)
						{
							const dimension_t clippedStartY = MaxValue<dimension_t>(offsetY, Base::GetBoundsTop());
							const dimension_t clippedEndY = MinValue<dimension_t>(offsetY + height - 1, Base::GetBoundsBottom());
							if (clippedStartY > clippedEndY)
								return;

							const dimension_t clippedStartX = MaxValue<dimension_t>(offsetX, Base::GetBoundsLeft());
							const dimension_t clippedEndX = MinValue<dimension_t>(offsetX + width - 1, Base::GetBoundsRight());
							if (clippedStartX > clippedEndX)
								return;

							dimension_t y = clippedStartY;
							while (true)
							{
								for (dimension_t x = clippedStartX; x <= clippedEndX; x++)
								{
									const dimension_t sourceX = static_cast<dimension_t>(x - offsetX);
									const dimension_t sourceY = static_cast<dimension_t>(y - offsetY);

									// Clipping has been performed, so we can use the unclipped version of PixelBlend.
									Base::PixelBlendUnclipped(frame,
										Base::ColorShader.Shade(Base::ColorSource.Source(sourceX, sourceY)),
										sourceX + offsetX, sourceY + offsetY);
								}

								if (y == clippedEndY)
								{
									break;
								}
								++y;
							}
						}

						// Draw with transparency check and transform, skipping color shading.
						void Draw(IFrameBuffer* frame, const color_t* bitmap,
							const dimension_t width, const dimension_t height,
							const dimension_t offsetX, const dimension_t offsetY,
							TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::TrueType)
						{
							const dimension_t clippedStartY = MaxValue<dimension_t>(offsetY, Base::GetBoundsTop());
							const dimension_t clippedEndY = MinValue<dimension_t>(offsetY + height - 1, Base::GetBoundsBottom());
							if (clippedStartY > clippedEndY)
								return;

							const dimension_t clippedStartX = MaxValue<dimension_t>(offsetX, Base::GetBoundsLeft());
							const dimension_t clippedEndX = MinValue<dimension_t>(offsetX + width - 1, Base::GetBoundsRight());
							if (clippedStartX > clippedEndX)
								return;

							for (dimension_t y = clippedStartY; y <= clippedEndY; y++)
							{
								for (dimension_t x = clippedStartX; x <= clippedEndX; x++)
								{
									Base::PixelBlendUnclipped(frame,
										Base::ColorSource.Source(x - offsetX, y - offsetY),
										x, y);
								}
							}
						}

						// Draw without color shading, skipping transparency check and transform.
						void Draw(IFrameBuffer* frame, const color_t* bitmap,
							const dimension_t width, const dimension_t height,
							const dimension_t offsetX, const dimension_t offsetY,
							TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::TrueType)
						{
							const dimension_t clippedStartY = MaxValue<dimension_t>(offsetY, Base::GetBoundsTop());
							const dimension_t clippedEndY = MinValue<dimension_t>(offsetY + height - 1, Base::GetBoundsBottom());
							if (clippedStartY > clippedEndY)
								return;

							const dimension_t clippedStartX = MaxValue<dimension_t>(offsetX, Base::GetBoundsLeft());
							const dimension_t clippedEndX = MinValue<dimension_t>(offsetX + width - 1, Base::GetBoundsRight());
							if (clippedStartX > clippedEndX)
								return;

							for (dimension_t y = clippedStartY; y <= clippedEndY; y++)
							{
								for (dimension_t x = clippedStartX; x <= clippedEndX; x++)
								{
									const rgb_color_t color = Base::ColorSource.Source(x - offsetX, y - offsetY);

									if (color != TransparentColor)
										Base::PixelBlendUnclipped(frame, color, x, y);
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