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
					typename PixelShaderType = Shader::Pixel::BatteryOutside<dimension_t, BatteryStyle::Monochrome>
				>
				class Outside : public Framework::Shader::Primitive::TemplateShader<dimension_t, PixelShaderType>
				{
				private:
					using Base = Framework::Shader::Primitive::TemplateShader<dimension_t, PixelShaderType>;

					using BatteryLayout = typename Layout::Battery<ParentLayout, BatteryStyle>;

				public:
					Outside() : Base()
					{
						Base::Prepare(ParentLayout::X(), ParentLayout::Y());
					}
					~Outside() = default;

					void Draw(IFrameBuffer* frame)
					{
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
					typename WholePixelShaderType = Shader::Pixel::BatteryBarsWhole<dimension_t, BatteryStyle::Monochrome>,
					typename LastPixelShaderType = Shader::Pixel::BatteryBarsLast<dimension_t, BatteryStyle::Monochrome>
				>
				class ProgressBars
				{
				private:
					using WholePrimitiveShaderType = Framework::Shader::Primitive::TemplateShader<dimension_t, WholePixelShaderType>;
					using LastPrimitiveShaderType = Framework::Shader::Primitive::TemplateShader<dimension_t, LastPixelShaderType>;

					using ProgressBarsLayout = typename Layout::ProgressBars<ParentLayout, BatteryStyle>;

				public:
					WholePrimitiveShaderType WholeShader{};
					LastPrimitiveShaderType LastShader{};

				private:
					uint8_t FullCount = BatteryStyle::BarCount;

				public:
					ProgressBars()
					{
						WholeShader.Prepare(ParentLayout::X(), ParentLayout::Y());
						LastShader.Prepare(ParentLayout::X(), ParentLayout::Y());
					}
					~ProgressBars() = default;

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