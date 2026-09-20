#ifndef _INTEGERGLASS_MODULES_WIRELESS_INDICATOR_DRAWABLE_PRIMITIVE_h
#define _INTEGERGLASS_MODULES_WIRELESS_INDICATOR_DRAWABLE_PRIMITIVE_h

#include "../Constant.h"
#include "../Layout.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace WirelessIndicator
		{
			namespace Drawable
			{

				template<typename dimension_t,
					typename ParentLayout,
					dimension_t Width,
					dimension_t Height,
					bool Monochrome,
					typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
					typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>,
					Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
				class AbstractDrawable : public Framework::Shader::Geometry::LineShader<
					dimension_t,
					Shader::Pixel::Bar<dimension_t, Width, Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>>
				{
				private:
					static constexpr rgb_color_t RGB_COLOR_GRAY = Rgb::Color(uint32_t(0x808080));

					using Base = Framework::Shader::Geometry::LineShader<
						dimension_t,
						Shader::Pixel::Bar<dimension_t, Width, Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>>;

				public:
					using Base::ColorSource;
					using Base::TransformShader;

					bool Visible = true;

				public:
					AbstractDrawable() : Base(0, 0, ParentLayout::Width() - 1, ParentLayout::Height() - 1) {}
					~AbstractDrawable() = default;

					void SetBarColor(const uint8_t barIndex, const rgb_color_t color)
					{
						Colors[barIndex] = color;
					}

					void SetBarsDitheringEnabled(const uint8_t barIndex, const bool enabled)
					{
						if (Monochrome)
						{
							Colors[barIndex] = enabled ? RGB_COLOR_GRAY : RGB_COLOR_WHITE;
							TransformShader.SetCheckerboardMaskEnabled(enabled);
						}
					}

				protected:
					void DrawHorizontalLine(IFrameBuffer* frame,
						const uint8_t index,
						const pixel_t startX,
						const pixel_t endX,
						const pixel_t y)
					{
						ColorSource.Color = Colors[index];
						Base::Line(frame,
							startX - ParentLayout::X(), y - ParentLayout::Y(),
							endX - ParentLayout::X(), y - ParentLayout::Y());
					}

					void DrawPixel(IFrameBuffer* frame,
						const uint8_t index,
						const pixel_t x,
						const pixel_t y)
					{
						ColorSource.Color = Colors[index];
						Base::Line(frame,
							x - ParentLayout::X(), y - ParentLayout::Y(),
							x - ParentLayout::X(), y - ParentLayout::Y());
					}

				protected:
					rgb_color_t Colors[Dimensions::BarCount]
					{
						RGB_COLOR_WHITE, RGB_COLOR_WHITE,
						RGB_COLOR_WHITE, RGB_COLOR_WHITE
					};
				};

				template<typename dimension_t, typename ParentLayout, bool Monochrome,
					typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>, typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>, Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
				class Bars8x7 : public AbstractDrawable<dimension_t, ParentLayout, ParentLayout::Width(), ParentLayout::Height(), Monochrome, ColorShaderType, TransformShaderType, BlendMode>
				{
				private:
					using Base = AbstractDrawable<dimension_t, ParentLayout, ParentLayout::Width(), ParentLayout::Height(), Monochrome, ColorShaderType, TransformShaderType, BlendMode>;

				private:
					using InnerLayout = Layout::Primitives<ParentLayout>;

				public:
					Bars8x7() : Base() {}
					~Bars8x7() = default;

					void Draw(IFrameBuffer* frame)
					{
						Base::Prepare(ParentLayout::X(), ParentLayout::Y());

						if (!Base::Visible)
							return;

						for (uint8_t i = 0; i < Dimensions::BarCount; i++)
						{
							const pixel_t x0 = InnerLayout::BarStartX(i);
							const pixel_t x1 = InnerLayout::BarEndX(i);
							const pixel_t y = InnerLayout::BarStartY(i);

							Base::DrawHorizontalLine(frame, i, x0, x1, y);
						}
					}
				};

				template<typename dimension_t, typename ParentLayout, bool Monochrome,
					typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>, typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>, Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
				class Bars8x8 : public AbstractDrawable<dimension_t, ParentLayout, ParentLayout::Width(), ParentLayout::Height(), Monochrome, ColorShaderType, TransformShaderType, BlendMode>
				{
				private:
					using Base = AbstractDrawable<dimension_t, ParentLayout, ParentLayout::Width(), ParentLayout::Height(), Monochrome, ColorShaderType, TransformShaderType, BlendMode>;

				private:
					using InnerLayout = Layout::Primitives<ParentLayout>;

				public:
					Bars8x8() : Base() {}
					~Bars8x8() = default;

					void Draw(IFrameBuffer* frame)
					{
						if (!Base::Visible)
							return;

						for (uint8_t i = 0; i < Dimensions::BarCount; i++)
						{
							const pixel_t x0 = InnerLayout::BarStartX(i);
							const pixel_t x1 = InnerLayout::BarEndX(i);
							const pixel_t y = InnerLayout::BarStartY(i);

							Base::DrawHorizontalLine(frame, i, x0, x1, y);
						}
					}
				};

				template<typename dimension_t, typename ParentLayout, bool Monochrome,
					typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>, typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>, Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
				class Bars10x7 : public AbstractDrawable<dimension_t, ParentLayout, ParentLayout::Width(), ParentLayout::Height(), Monochrome, ColorShaderType, TransformShaderType, BlendMode>
				{
				private:
					using Base = AbstractDrawable<dimension_t, ParentLayout, ParentLayout::Width(), ParentLayout::Height(), Monochrome, ColorShaderType, TransformShaderType, BlendMode>;

				private:
					using InnerLayout = Layout::Primitives<ParentLayout>;

				public:
					Bars10x7() : Base() {}
					~Bars10x7() = default;

					void Draw(IFrameBuffer* frame)
					{
						if (!Base::Visible)
							return;

						{
							const pixel_t x0 = InnerLayout::BarStartX(0) + 1;
							const pixel_t x1 = InnerLayout::BarEndX(0) - 1;
							const pixel_t y = InnerLayout::BarStartY(0);

							Base::DrawHorizontalLine(frame, 0, x0, x1, y);
						}

						for (uint8_t i = 1; i < Dimensions::BarCount; i++)
						{
							const pixel_t x0 = InnerLayout::BarStartX(i);
							const pixel_t x1 = InnerLayout::BarEndX(i);
							const pixel_t y = InnerLayout::BarStartY(i);

							Base::DrawHorizontalLine(frame, i, x0 + 1, x1 - 1, y);
							Base::DrawPixel(frame, i, x0, y + 1);
							Base::DrawPixel(frame, i, x1, y + 1);
						}
					}
				};

				template<typename dimension_t, typename ParentLayout, bool Monochrome,
					typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>, typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>, Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
				class Bars12x9 : public AbstractDrawable<dimension_t, ParentLayout, ParentLayout::Width(), ParentLayout::Height(), Monochrome, ColorShaderType, TransformShaderType, BlendMode>
				{
				private:
					using Base = AbstractDrawable<dimension_t, ParentLayout, ParentLayout::Width(), ParentLayout::Height(), Monochrome, ColorShaderType, TransformShaderType, BlendMode>;

				private:
					using InnerLayout = Layout::Primitives<ParentLayout>;

				public:
					Bars12x9() : Base() {}
					~Bars12x9() = default;

					void Draw(IFrameBuffer* frame)
					{
						if (!Base::Visible)
							return;

						constexpr pixel_t fillY0 = InnerLayout::BarStartY(0);
						constexpr pixel_t fillY1 = InnerLayout::BarEndY(0);
						constexpr pixel_t fillX0 = InnerLayout::BarStartX(0) + 1;
						constexpr pixel_t fillX1 = InnerLayout::BarEndX(0) - 1;

						for (pixel_t y = fillY0; y <= fillY1; y++)
						{
							Base::DrawHorizontalLine(frame, 0, fillX0, fillX1, y);
						}

						for (uint8_t i = 1; i < Dimensions::BarCount; i++)
						{
							const pixel_t lx0 = InnerLayout::BarStartX(i) + 2;
							const pixel_t lx1 = InnerLayout::BarEndX(i) - 2;
							const pixel_t ly = InnerLayout::BarStartY(i);

							Base::DrawHorizontalLine(frame, i, lx0, lx1, ly);

							const pixel_t capY = ly + 1;
							const pixel_t l0 = InnerLayout::BarStartX(i);
							const pixel_t l1 = l0 + 1;
							const pixel_t r1 = InnerLayout::BarEndX(i);
							const pixel_t r0 = r1 - 1;

							Base::DrawHorizontalLine(frame, i, l0, l1, capY);
							Base::DrawHorizontalLine(frame, i, r0, r1, capY);
						}
					}
				};

				template<typename dimension_t,
					typename ParentLayout,
					typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
					typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>,
					Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
				class DisabledOverlay : public Framework::Shader::Geometry::LineShader<
					dimension_t,
					Framework::Shader::Pixel::TemplateShader<
					dimension_t,
					Framework::Shader::Source::SingleColor<dimension_t>,
					ColorShaderType,
					TransformShaderType,
					BlendMode>>
				{
				private:
					using Base = Framework::Shader::Geometry::LineShader<
						dimension_t,
						Framework::Shader::Pixel::TemplateShader<
						dimension_t,
						Framework::Shader::Source::SingleColor<dimension_t>,
						ColorShaderType,
						TransformShaderType,
						BlendMode>>;

				public:
					rgb_color_t ColorBackground = RGB_COLOR_BLACK;
					rgb_color_t ColorForeground = RGB_COLOR_WHITE;

				public:
					DisabledOverlay() : Base(0, 0, ParentLayout::Width() - 1, ParentLayout::Height() - 1) {}
					~DisabledOverlay() = default;

					void Draw(IFrameBuffer* frame)
					{
						Base::Prepare(ParentLayout::X(), ParentLayout::Y());
						Base::ColorSource.Color = ColorBackground;
						Base::Line(frame,
							0,
							ParentLayout::Height() - 1,
							ParentLayout::Width() - 1,
							0);

						Base::Line(frame,
							0,
							ParentLayout::Height() - 2,
							ParentLayout::Width() - 1,
							0);

						Base::ColorSource.Color = ColorForeground;
						Base::Line(frame,
							0,
							ParentLayout::Height() - 1,
							ParentLayout::Width() - 1,
							1);
					}
				};
			}
		}
	}
}
#endif