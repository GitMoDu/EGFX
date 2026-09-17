#ifndef _EGFX_MODULES_KIT_DRAWABLE_SHADOW_CORNER_GRADIENT_h
#define _EGFX_MODULES_KIT_DRAWABLE_SHADOW_CORNER_GRADIENT_h

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
					using QuadrantEnum = Framework::Shader::Geometry::QuadrantEnum;

					template<typename dimension_t,
						typename ParentLayout,
						QuadrantEnum Corner,
						typename ColorSourceType = Framework::Shader::Source::StaticColor<dimension_t>,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>,
						Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::BlendAlpha
					>
					class CornerGradient : public Framework::Shader::Geometry::RectangleShader<dimension_t,
						Framework::Shader::Pixel::TemplateShader<dimension_t, ColorSourceType, ColorShaderType,
						TransformShaderType, BlendMode>>
					{
					public:
						using DrawableLayout = ParentLayout;

					private:
						using Base = Framework::Shader::Geometry::RectangleShader<dimension_t,
							Framework::Shader::Pixel::TemplateShader<dimension_t, ColorSourceType, ColorShaderType,
							TransformShaderType, BlendMode>>;
						using wide_t = typename Framework::AutoDimension::ByDimension<dimension_t>::wide_t;

						UFraction16::scalar_t ShadowOpacity = UFraction16::SCALAR_UNIT / 2;

					public:
						CornerGradient() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height()) {}
						~CornerGradient() = default;

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
							const dimension_t left = Base::GetBoundsLeft();
							const dimension_t right = Base::GetBoundsRight();
							const dimension_t top = Base::GetBoundsTop();
							const dimension_t bottom = Base::GetBoundsBottom();
							const wide_t width = static_cast<wide_t>(right - left);
							const wide_t height = static_cast<wide_t>(bottom - top);
							const wide_t maxDistance = (width - 1) + (height - 1);

							for (dimension_t y = top; y < bottom; y++)
							{
								for (dimension_t x = left; x < right; x++)
								{
									const wide_t distanceX = HorizontalDistance(x, left, right);
									const wide_t distanceY = VerticalDistance(y, top, bottom);
									const wide_t distance = distanceX + distanceY;
									const auto opacity = maxDistance == 0
										? UFraction16::SCALAR_UNIT
										: UFraction16::GetScalar<wide_t>(maxDistance - distance, maxDistance);
									const uint8_t alpha = UFraction16::Fraction<uint8_t>(
										UFraction16::Fraction(opacity, ShadowOpacity), UINT8_MAX);

									Base::SetAlpha(alpha);
									Base::PixelBlendUnclipped(frame, color, x, y);
								}
							}
						}

					private:
						static wide_t HorizontalDistance(const dimension_t x, const dimension_t left, const dimension_t right)
						{
							return Corner == QuadrantEnum::TopLeft
								|| Corner == QuadrantEnum::BottomLeft
								? static_cast<wide_t>(x - left)
								: static_cast<wide_t>(right - 1 - x);
						}

						static wide_t VerticalDistance(const dimension_t y, const dimension_t top, const dimension_t bottom)
						{
							return Corner == QuadrantEnum::TopLeft
								|| Corner == QuadrantEnum::TopRight
								? static_cast<wide_t>(y - top)
								: static_cast<wide_t>(bottom - 1 - y);
						}
					};
				}
			}
		}
	}
}
#endif
