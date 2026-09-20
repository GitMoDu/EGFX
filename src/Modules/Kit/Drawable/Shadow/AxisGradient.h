#ifndef _INTEGERGLASS_MODULES_KIT_DRAWABLE_SHADOW_AXIS_GRADIENT_h
#define _INTEGERGLASS_MODULES_KIT_DRAWABLE_SHADOW_AXIS_GRADIENT_h

#include <IntegerGlassFramework.h>

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Kit
		{
			namespace Drawable
			{
				namespace Shadow
				{
					enum class AxisOrientationEnum
					{
						Vertical,
						Horizontal
					};

					template<typename dimension_t,
						typename ParentLayout,
						AxisOrientationEnum Orientation,
						typename ColorSourceType = Framework::Shader::Source::StaticColor<dimension_t>,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>,
						Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::BlendAlpha
					>
					class AxisGradient : public Framework::Shader::Geometry::RectangleShader<dimension_t,
						Framework::Shader::Pixel::TemplateShader<dimension_t, ColorSourceType, ColorShaderType,
						TransformShaderType, BlendMode>>
					{
					public:
						using DrawableLayout = ParentLayout;

					private:
						using Base = Framework::Shader::Geometry::RectangleShader<dimension_t,
							Framework::Shader::Pixel::TemplateShader<dimension_t, ColorSourceType, ColorShaderType,
							TransformShaderType, BlendMode>>;

						UFraction16::scalar_t ShadowOpacity = UFraction16::SCALAR_UNIT / 2;

					public:
						AxisGradient() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height()) {}
						~AxisGradient() = default;

						void SetOpacity(const UFraction16::scalar_t opacity)
						{
							ShadowOpacity = MinValue(opacity, UFraction16::SCALAR_UNIT);
						}

						void Draw(IFrameBuffer* frame)
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());

							auto& colorSource = Base::ColorSource;
							auto& colorShader = Base::ColorShader;
							const auto color = colorShader.Shade(colorSource.Source(0, 0));
							const dimension_t boundsLeft = Base::GetBoundsLeft();
							const dimension_t boundsRight = Base::GetBoundsRight();
							const dimension_t boundsTop = Base::GetBoundsTop();
							const dimension_t boundsBottom = Base::GetBoundsBottom();

							for (dimension_t y = boundsTop; y < boundsBottom; ++y)
							{
								for (dimension_t x = boundsLeft; x < boundsRight; ++x)
								{
									const dimension_t axisOpacity = AxisOpacity(x, y, boundsLeft, boundsTop,
										boundsRight - boundsLeft, boundsBottom - boundsTop);
									if (axisOpacity != 0)
									{
										const uint8_t alpha = UFraction16::Fraction<uint8_t>(
											UFraction16::Fraction(axisOpacity, ShadowOpacity), UINT8_MAX);
										Base::SetAlpha(alpha);
										Base::PixelBlendUnclipped(frame, color, x, y);
									}
								}
							}
						}

					private:
						static UFraction16::scalar_t AxisOpacity(const dimension_t x, const dimension_t y,
							const dimension_t left, const dimension_t top,
							const dimension_t width, const dimension_t height)
						{
							if (Orientation == AxisOrientationEnum::Vertical)
							{
								const dimension_t halfWidth = width / 2;
								if (halfWidth == 0)
									return UFraction16::SCALAR_UNIT;

								const dimension_t distance = x - left <= halfWidth
									? x - left : width - 1 - (x - left);
								return UFraction16::GetScalar<dimension_t>(distance, halfWidth);
							}

							const dimension_t halfHeight = height / 2;
							if (halfHeight == 0)
								return UFraction16::SCALAR_UNIT;

							const dimension_t distance = y - top <= halfHeight
								? y - top : height - 1 - (y - top);
							return UFraction16::GetScalar<dimension_t>(distance, halfHeight);
						}
					};

				template<typename dimension_t,
					typename ParentLayout,
					typename ColorSourceType = Framework::Shader::Source::StaticColor<dimension_t>,
					typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
					typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>,
					Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::BlendAlpha>
				using VerticalAxisGradient = AxisGradient<dimension_t, ParentLayout, AxisOrientationEnum::Vertical,
					ColorSourceType, ColorShaderType, TransformShaderType, BlendMode>;

				template<typename dimension_t,
					typename ParentLayout,
					typename ColorSourceType = Framework::Shader::Source::StaticColor<dimension_t>,
					typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
					typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>,
					Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::BlendAlpha>
				using HorizontalAxisGradient = AxisGradient<dimension_t, ParentLayout, AxisOrientationEnum::Horizontal,
					ColorSourceType, ColorShaderType, TransformShaderType, BlendMode>;
				}
			}
		}
	}
}
#endif
