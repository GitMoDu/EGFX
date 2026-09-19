#ifndef _EGFX_MODULES_BATTERY_INDICATOR_DRAWABLE_h
#define _EGFX_MODULES_BATTERY_INDICATOR_DRAWABLE_h

#include "Constant.h"
#include "Layout.h"
#include "Shader.h"


namespace Egfx
{
	namespace Modules
	{
		namespace BatteryIndicator
		{
			namespace Drawable
			{
				template<typename dimension_t,
					typename ParentLayout,
					typename BatteryStyle = TemplateBatteryStyle<>,
					typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
					typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>
				>
				class Outside : public Framework::Shader::Geometry::RectangleShader<dimension_t,
					Framework::Shader::Pixel::TemplateShader<dimension_t,
					Framework::Shader::Source::StaticColor<dimension_t>, ColorShaderType, TransformShaderType>>
				{
				private:
					using Base = Framework::Shader::Geometry::RectangleShader<dimension_t,
						Framework::Shader::Pixel::TemplateShader<dimension_t,
						Framework::Shader::Source::StaticColor<dimension_t>, ColorShaderType, TransformShaderType>>;

					using BatteryLayout = typename Layout::Battery<ParentLayout, BatteryStyle>;

				public:
					Outside() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height())
					{}
					~Outside() = default;

					void SetBounds(const dimension_t left, const dimension_t top,
						const dimension_t right, const dimension_t bottom)
					{
						Base::SetBounds(left, top, right, bottom);
					}

					void SetTranslation(const int16_t x, const int16_t y)
					{
						Base::SetTranslation(x, y);
					}

					void Draw(IFrameBuffer* frame)
					{
						Base::Prepare(ParentLayout::X(), ParentLayout::Y());
						Base::RectangleFill(frame, BatteryLayout::HeadTop().topLeft.x, BatteryLayout::HeadTop().topLeft.y, BatteryLayout::HeadTop().bottomRight.x, BatteryLayout::HeadTop().bottomRight.y);
						Base::RectangleFill(frame, BatteryLayout::HeadBottom().topLeft.x, BatteryLayout::HeadBottom().topLeft.y, BatteryLayout::HeadBottom().bottomRight.x, BatteryLayout::HeadBottom().bottomRight.y);
						Base::RectangleFill(frame, BatteryLayout::HeadRight().topLeft.x, BatteryLayout::HeadRight().topLeft.y, BatteryLayout::HeadRight().bottomRight.x, BatteryLayout::HeadRight().bottomRight.y);

						Base::RectangleFill(frame, BatteryLayout::OutsideLeft().topLeft.x, BatteryLayout::OutsideLeft().topLeft.y, BatteryLayout::OutsideLeft().bottomRight.x, BatteryLayout::OutsideLeft().bottomRight.y);
						Base::RectangleFill(frame, BatteryLayout::OutsideTop().topLeft.x, BatteryLayout::OutsideTop().topLeft.y, BatteryLayout::OutsideTop().bottomRight.x, BatteryLayout::OutsideTop().bottomRight.y);
						Base::RectangleFill(frame, BatteryLayout::OutsideBottom().topLeft.x, BatteryLayout::OutsideBottom().topLeft.y, BatteryLayout::OutsideBottom().bottomRight.x, BatteryLayout::OutsideBottom().bottomRight.y);

						Base::RectangleFill(frame, BatteryLayout::HeadLeftTop().topLeft.x, BatteryLayout::HeadLeftTop().topLeft.y, BatteryLayout::HeadLeftTop().bottomRight.x, BatteryLayout::HeadLeftTop().bottomRight.y);
						Base::RectangleFill(frame, BatteryLayout::HeadLeftBottom().topLeft.x, BatteryLayout::HeadLeftBottom().topLeft.y, BatteryLayout::HeadLeftBottom().bottomRight.x, BatteryLayout::HeadLeftBottom().bottomRight.y);
					}
				};

				template<typename dimension_t,
					typename ParentLayout,
					typename BatteryStyle = TemplateBatteryStyle<>,
					typename WholeColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
					typename WholeTransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>,
					typename LastColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
					typename LastTransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>,
					Framework::Shader::Pixel::BlendModeEnum WholeBlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace,
					Framework::Shader::Pixel::BlendModeEnum LastBlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace
				>
				class ProgressBars
				{
				private:
					using WholePixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t,
						Framework::Shader::Source::StaticColor<dimension_t>, WholeColorShaderType, WholeTransformShaderType, WholeBlendMode>;
					using LastPixelShaderType = Shader::Pixel::BatteryBarsLast<dimension_t, BatteryStyle::Monochrome,
						LastColorShaderType, LastTransformShaderType, LastBlendMode>;
					using WholePrimitiveShaderType = Framework::Shader::Geometry::RectangleShader<dimension_t, WholePixelShaderType>;
					using LastPrimitiveShaderType = Framework::Shader::Geometry::RectangleShader<dimension_t, LastPixelShaderType>;

					using ProgressBarsLayout = typename Layout::ProgressBars<ParentLayout, BatteryStyle>;

				public:
					WholePrimitiveShaderType WholeShader{ 0, 0, ParentLayout::Width(), ParentLayout::Height() };
					LastPrimitiveShaderType LastShader{ 0, 0, ParentLayout::Width(), ParentLayout::Height() };

				private:
					uint8_t FullCount = BatteryStyle::BarCount;

				public:
					ProgressBars()
					{}
					~ProgressBars() = default;

					void SetBounds(const dimension_t left, const dimension_t top,
						const dimension_t right, const dimension_t bottom)
					{
						WholeShader.SetBounds(left, top, right, bottom);
						LastShader.SetBounds(left, top, right, bottom);
					}

					void SetTranslation(const int16_t x, const int16_t y)
					{
						WholeShader.SetTranslation(x, y);
						LastShader.SetTranslation(x, y);
					}

					bool IsVisible() const
					{
						return WholeShader.IsVisible() || LastShader.IsVisible();
					}

					void SetFullCount(const uint8_t index)
					{
						FullCount = MinValue<uint8_t>(index, BatteryStyle::BarCount);
					}

					uint8_t GetFullCount() const
					{
						return FullCount;
					}

					void Draw(IFrameBuffer* frame)
					{
						WholeShader.Prepare(ParentLayout::X(), ParentLayout::Y());
						LastShader.Prepare(ParentLayout::X(), ParentLayout::Y());

						if (FullCount > 0)
						{
							if (FullCount > 1)
							{
								DrawWhole(frame, FullCount - 2);
							}
							DrawLast(frame, FullCount - 1);
						}
					}

					void DrawWhole(IFrameBuffer* frame, const uint8_t end)
					{
						for (uint8_t i = 0; i <= end; i++)
						{
							WholeShader.RectangleFill(frame,
								ProgressBarsLayout::BarN(i).topLeft.x, ProgressBarsLayout::BarN(i).topLeft.y,
								ProgressBarsLayout::BarN(i).bottomRight.x, ProgressBarsLayout::BarN(i).bottomRight.y
							);
						}
					}

					void DrawLast(IFrameBuffer* frame, const uint8_t index)
					{
						if (index < BatteryStyle::BarCount - 1)
						{
							LastShader.RectangleFill(frame,
								ProgressBarsLayout::BarN(index).topLeft.x, ProgressBarsLayout::BarN(index).topLeft.y,
								ProgressBarsLayout::BarN(index).bottomRight.x, ProgressBarsLayout::BarN(index).bottomRight.y
							);
						}
						else
						{
							LastShader.RectangleFill(frame,
								ProgressBarsLayout::BarLastBody().topLeft.x, ProgressBarsLayout::BarLastBody().topLeft.y,
								ProgressBarsLayout::BarLastBody().bottomRight.x, ProgressBarsLayout::BarLastBody().bottomRight.y
							);

							LastShader.RectangleFill(frame,
								ProgressBarsLayout::BarLastHead().topLeft.x, ProgressBarsLayout::BarLastHead().topLeft.y,
								ProgressBarsLayout::BarLastHead().bottomRight.x, ProgressBarsLayout::BarLastHead().bottomRight.y
							);
						}
					}
				};
			}
		}
	}
}
#endif