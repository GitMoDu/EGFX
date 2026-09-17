#ifndef _EGFX_MODULES_KIT_DRAWABLE_SHADOW_CARDINAL_GRADIENT_h
#define _EGFX_MODULES_KIT_DRAWABLE_SHADOW_CARDINAL_GRADIENT_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Modules
	{
		namespace Kit
		{
			namespace Drawable
			{
				namespace Shadow
				{
					enum class CardinalOrientationEnum
					{
						LeftToRight,
						RightToLeft,
						TopToBottom,
						BottomToTop
					};


					template<typename dimension_t,
						typename ParentLayout,
						CardinalOrientationEnum Orientation = CardinalOrientationEnum::TopToBottom,
						typename ColorSourceType = Framework::Shader::Source::StaticColor<dimension_t>,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>,
						Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::BlendAlpha
					>
					class CardinalGradient : public Framework::Shader::Geometry::RectangleShader<dimension_t,
						Framework::Shader::Pixel::TemplateShader<dimension_t, ColorSourceType, ColorShaderType,
						TransformShaderType, BlendMode>>
					{
					public:
						using DrawableLayout = ParentLayout;

					private:
						using Base = Framework::Shader::Geometry::RectangleShader<dimension_t,
							Framework::Shader::Pixel::TemplateShader<dimension_t, ColorSourceType, ColorShaderType,
							TransformShaderType, BlendMode>>;

					private:
						UFraction16::scalar_t ShadowOpacity = UFraction16::SCALAR_UNIT / 2;

					public:
						CardinalGradient() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height())
						{}
						~CardinalGradient() = default;

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

							switch (Orientation)
							{
							case CardinalOrientationEnum::TopToBottom:
								DrawTopToBottom(frame, color);
								break;
							case  CardinalOrientationEnum::BottomToTop:
								DrawBottomToTop(frame, color);
								break;
							case CardinalOrientationEnum::LeftToRight:
								DrawLeftToRight(frame, color);
								break;
							case CardinalOrientationEnum::RightToLeft:
								DrawRightToLeft(frame, color);
								break;
							default:
								break;
							}
						}

					private:
						rgb_color_t ShadowColor(const rgb_color_t color,
							const UFraction16::scalar_t opacity) const
						{
							return Rgb::Color(
								UFraction16::Fraction<uint8_t>(opacity, UINT8_MAX - Rgb::R8(color)),
								UFraction16::Fraction<uint8_t>(opacity, UINT8_MAX - Rgb::G8(color)),
								UFraction16::Fraction<uint8_t>(opacity, UINT8_MAX - Rgb::B8(color)));
						}

						void DrawTopToBottom(IFrameBuffer* frame, const rgb_color_t color)
						{
							const dimension_t boundsLeft = Base::GetBoundsLeft();
							const dimension_t boundsRight = Base::GetBoundsRight();
							const dimension_t boundsTop = Base::GetBoundsTop();
							const dimension_t boundsBottom = Base::GetBoundsBottom();

							for (dimension_t y = boundsTop; y < boundsBottom; ++y)
							{
								const auto rowOpacity = UFraction16::GetScalar<dimension_t>(
									ParentLayout::Height() - y, ParentLayout::Height());

								const auto blendedColor = ShadowColor(color,
									UFraction16::Fraction(ShadowOpacity, rowOpacity));

								if (blendedColor != 0)
									for (dimension_t x = boundsLeft; x < boundsRight; ++x)
										Base::PixelBlendUnclipped(frame, blendedColor, x, y);
							}
						}

						void DrawBottomToTop(IFrameBuffer* frame, const rgb_color_t color)
						{
							const dimension_t boundsLeft = Base::GetBoundsLeft();
							const dimension_t boundsRight = Base::GetBoundsRight();
							const dimension_t boundsTop = Base::GetBoundsTop();
							const dimension_t boundsBottom = Base::GetBoundsBottom();


							for (dimension_t y = boundsTop; y < boundsBottom; y++)
							{
								const UFraction16::scalar_t rowOpacity = UFraction16::GetScalar<dimension_t>(
									y, ParentLayout::Height() - 1);

								const uint8_t alpha = UFraction16::Fraction<uint8_t>(UFraction16::Fraction(rowOpacity, ShadowOpacity), UINT8_MAX);
								if (rowOpacity != 0)
								{
									Base::SetAlpha(alpha);
									for (dimension_t x = boundsLeft; x < boundsRight; x++)
									{
										Base::PixelBlendUnclipped(frame, color, x, y);
									}
								}
							}
						}

						void DrawLeftToRight(IFrameBuffer* frame, const rgb_color_t color)
						{
							const dimension_t boundsLeft = Base::GetBoundsLeft();
							const dimension_t boundsRight = Base::GetBoundsRight();
							const dimension_t boundsTop = Base::GetBoundsTop();
							const dimension_t boundsBottom = Base::GetBoundsBottom();

							for (dimension_t x = boundsLeft; x < boundsRight; ++x)
							{
								const auto columnOpacity = UFraction16::GetScalar<dimension_t>(
									x, ParentLayout::Width());
								const auto blendedColor = ShadowColor(color,
									UFraction16::Fraction(ShadowOpacity, columnOpacity));

								if (blendedColor != 0)
									for (dimension_t y = boundsTop; y < boundsBottom; ++y)
										Base::PixelBlendUnclipped(frame, blendedColor, x, y);
							}
						}

						void DrawRightToLeft(IFrameBuffer* frame, const rgb_color_t color)
						{
							const dimension_t boundsLeft = Base::GetBoundsLeft();
							const dimension_t boundsRight = Base::GetBoundsRight();
							const dimension_t boundsTop = Base::GetBoundsTop();
							const dimension_t boundsBottom = Base::GetBoundsBottom();

							for (dimension_t x = boundsLeft; x < boundsRight; ++x)
							{
								const auto columnOpacity = UFraction16::GetScalar<dimension_t>(
									ParentLayout::Width() - x, ParentLayout::Width());
								const auto blendedColor = ShadowColor(color,
									UFraction16::Fraction(ShadowOpacity, columnOpacity));

								if (blendedColor != 0)
									for (dimension_t y = boundsTop; y < boundsBottom; ++y)
										Base::PixelBlendUnclipped(frame, blendedColor, x, y);
							}
						}
					};
				}
			}
		}
	}
}
#endif