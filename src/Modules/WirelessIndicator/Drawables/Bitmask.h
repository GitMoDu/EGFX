#ifndef _INTEGERGLASS_MODULES_WIRELESS_INDICATOR_DRAWABLE_BITMASK_h
#define _INTEGERGLASS_MODULES_WIRELESS_INDICATOR_DRAWABLE_BITMASK_h

#include "../Constant.h"
#include "../Layout.h"
#include "../Shader.h"

#include "../Assets.h"
#include "../../../Framework/Image/Bitmask/Drawable.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace WirelessIndicator
		{
			namespace Drawable
			{
				namespace Bitmask
				{
					static constexpr uint8_t OverscaleX = 1;

					template<typename dimension_t,
						typename ParentLayout,
						dimension_t BitmaskWidth,
						dimension_t BitmaskHeight,
						bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>,
						Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
					class AbstractBarsSlice : public ::IntegerGlass::Framework::Shader::Image::Bitmask::Scaled<
						dimension_t,
						static_cast<uint8_t>(OverscaleX + MaxValue<pixel_t>(1, ParentLayout::Width() / BitmaskWidth)),
						static_cast<uint8_t>(MaxValue<pixel_t>(1, ParentLayout::Height() / BitmaskHeight)),
						::IntegerGlass::Framework::DataSourceTypeEnum::Flash,
						true,
						RGB_COLOR_BLACK,
						Framework::Image::ScaledTransformOrderEnum::TransformThenScale,
						Shader::Source::Bar<dimension_t>,
						ColorShaderType,
						Shader::Transform::Bar<dimension_t, BitmaskWidth, BitmaskHeight, Monochrome, TransformShaderType>,
						BlendMode>
					{
					private:
						using Base = ::IntegerGlass::Framework::Shader::Image::Bitmask::Scaled<
							dimension_t,
							static_cast<uint8_t>(OverscaleX + MaxValue<pixel_t>(1, ParentLayout::Width() / BitmaskWidth)),
							static_cast<uint8_t>(MaxValue<pixel_t>(1, ParentLayout::Height() / BitmaskHeight)),
							::IntegerGlass::Framework::DataSourceTypeEnum::Flash,
							true,
							RGB_COLOR_BLACK,
							Framework::Image::ScaledTransformOrderEnum::TransformThenScale,
							Shader::Source::Bar<dimension_t>,
							ColorShaderType,
							Shader::Transform::Bar<dimension_t, BitmaskWidth, BitmaskHeight, Monochrome, TransformShaderType>,
							BlendMode>;

						const uint8_t* BitmaskData;

					public:
						static constexpr dimension_t ScaleX = static_cast<dimension_t>(OverscaleX + MaxValue<pixel_t>(1, ParentLayout::Width() / BitmaskWidth));
						static constexpr dimension_t ScaleY = static_cast<dimension_t>(MaxValue<pixel_t>(1, ParentLayout::Height() / BitmaskHeight));
						static constexpr dimension_t TransformWidth = static_cast<dimension_t>(ParentLayout::Width() / ScaleX);
						static constexpr dimension_t Width = TransformWidth * ScaleX;
						static constexpr dimension_t Height = BitmaskHeight * ScaleY;

						explicit AbstractBarsSlice(const uint8_t* bitmask)
							: Base(0, 0, ParentLayout::Width(), ParentLayout::Height()), BitmaskData(bitmask)
						{
							Base::TransformShader.SetWidth(TransformWidth);
						}

						~AbstractBarsSlice() = default;

						void SetColor(const rgb_color_t color)
						{
							SetColor(color, typename TypeTraits::TypeConditional::conditional_type<
								TypeTraits::TypeDispatch::FalseType,
								TypeTraits::TypeDispatch::TrueType,
								Monochrome>::type{});
						}

						void Draw(IFrameBuffer* framebuffer)
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
							Base::Draw(framebuffer, BitmaskData, BitmaskWidth, BitmaskHeight, 0, 0);
						}

					private:
						void SetColor(const rgb_color_t, TypeTraits::TypeDispatch::TrueType)
						{}

						void SetColor(const rgb_color_t color, TypeTraits::TypeDispatch::FalseType)
						{
							Base::ColorSource.Color = color;
						}
					};

					template<typename dimension_t, typename ParentLayout, bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>, typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>, Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
					class Bars22x16Slice0 : public AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask22x16::Width, Assets::Bitmask22x16::Slice0Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
					{
					public:
						Bars22x16Slice0() : AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask22x16::Width, Assets::Bitmask22x16::Slice0Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
							(Assets::Bitmask22x16::Slice_0_22x4) {}
						~Bars22x16Slice0() = default;
					};

					template<typename dimension_t, typename ParentLayout, bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>, typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>, Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
					class Bars22x16Slice1 : public AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask22x16::Width, Assets::Bitmask22x16::Slice1Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
					{
					public:
						Bars22x16Slice1() : AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask22x16::Width, Assets::Bitmask22x16::Slice1Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
							(Assets::Bitmask22x16::Slice_1_22x4) {}
						~Bars22x16Slice1() = default;
					};

					template<typename dimension_t, typename ParentLayout, bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>, typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>, Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
					class Bars22x16Slice2 : public AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask22x16::Width, Assets::Bitmask22x16::Slice2Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
					{
					public:
						Bars22x16Slice2() : AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask22x16::Width, Assets::Bitmask22x16::Slice2Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
							(Assets::Bitmask22x16::Slice_2_22x5) {}
						~Bars22x16Slice2() = default;
					};

					template<typename dimension_t, typename ParentLayout, bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>, typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>, Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
					class Bars22x16Slice3 : public AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask22x16::Width, Assets::Bitmask22x16::Slice3Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
					{
					public:
						Bars22x16Slice3() : AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask22x16::Width, Assets::Bitmask22x16::Slice3Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
							(Assets::Bitmask22x16::Slice_3_22x6) {}
						~Bars22x16Slice3() = default;
					};

					template<typename dimension_t, typename ParentLayout, bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>, typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>, Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
					class Bars24x16Slice0 : public AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask24x16::Width, Assets::Bitmask24x16::Slice0Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
					{
					public:
						Bars24x16Slice0() : AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask24x16::Width, Assets::Bitmask24x16::Slice0Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
							(Assets::Bitmask24x16::Slice_0_24x4) {}
						~Bars24x16Slice0() = default;
					};

					template<typename dimension_t, typename ParentLayout, bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>, typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>, Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
					class Bars24x16Slice1 : public AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask24x16::Width, Assets::Bitmask24x16::Slice1Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
					{
					public:
						Bars24x16Slice1() : AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask24x16::Width, Assets::Bitmask24x16::Slice1Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
							(Assets::Bitmask24x16::Slice_1_24x5) {}
						~Bars24x16Slice1() = default;
					};

					template<typename dimension_t, typename ParentLayout, bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>, typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>, Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
					class Bars24x16Slice2 : public AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask24x16::Width, Assets::Bitmask24x16::Slice2Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
					{
					public:
						Bars24x16Slice2() : AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask24x16::Width, Assets::Bitmask24x16::Slice2Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
							(Assets::Bitmask24x16::Slice_2_24x6) {}
						~Bars24x16Slice2() = default;
					};

					template<typename dimension_t, typename ParentLayout, bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>, typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>, Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
					class Bars24x16Slice3 : public AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask24x16::Width, Assets::Bitmask24x16::Slice3Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
					{
					public:
						Bars24x16Slice3() : AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask24x16::Width, Assets::Bitmask24x16::Slice3Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
							(Assets::Bitmask24x16::Slice_3_24x7) {}
						~Bars24x16Slice3() = default;
					};

					template<typename dimension_t, typename ParentLayout, bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>, typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>, Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
					class Bars42x30Slice0 : public AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask42x30::Width, Assets::Bitmask42x30::Slice0Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
					{
					public:
						Bars42x30Slice0() : AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask42x30::Width, Assets::Bitmask42x30::Slice0Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
							(Assets::Bitmask42x30::Slice_0_42x7) {}
						~Bars42x30Slice0() = default;
					};

					template<typename dimension_t, typename ParentLayout, bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>, typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>, Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
					class Bars42x30Slice1 : public AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask42x30::Width, Assets::Bitmask42x30::Slice1Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
					{
					public:
						Bars42x30Slice1() : AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask42x30::Width, Assets::Bitmask42x30::Slice1Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
							(Assets::Bitmask42x30::Slice_1_42x7) {}
						~Bars42x30Slice1() = default;
					};

					template<typename dimension_t, typename ParentLayout, bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>, typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>, Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
					class Bars42x30Slice2 : public AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask42x30::Width, Assets::Bitmask42x30::Slice2Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
					{
					public:
						Bars42x30Slice2() : AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask42x30::Width, Assets::Bitmask42x30::Slice2Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
							(Assets::Bitmask42x30::Slice_2_42x10) {}
						~Bars42x30Slice2() = default;
					};

					template<typename dimension_t, typename ParentLayout, bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>, typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>, Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
					class Bars42x30Slice3 : public AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask42x30::Width, Assets::Bitmask42x30::Slice3Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
					{
					public:
						Bars42x30Slice3() : AbstractBarsSlice<dimension_t, ParentLayout, Assets::Bitmask42x30::Width, Assets::Bitmask42x30::Slice3Height, Monochrome, ColorShaderType, TransformShaderType, BlendMode>
							(Assets::Bitmask42x30::Slice_3_42x12) {}
						~Bars42x30Slice3() = default;
					};



					template<typename dimension_t,
						typename ParentLayout,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>,
						Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
					class DisabledOverlay : public Framework::Shader::Geometry::TriangleShader<
						dimension_t,
						Framework::Shader::Pixel::TemplateShader<
						dimension_t,
						Framework::Shader::Source::SingleColor<dimension_t>,
						ColorShaderType,
						TransformShaderType,
						BlendMode>
					>
					{
					private:
						using Base = Framework::Shader::Geometry::TriangleShader<
							dimension_t,
							Framework::Shader::Pixel::TemplateShader<
							dimension_t,
							Framework::Shader::Source::SingleColor<dimension_t>,
							ColorShaderType,
							TransformShaderType,
							BlendMode>
						>;

						using ParallelogramLayout = Layout::ParallelogramLayout<ParentLayout>;

					public:
						using Base::ColorSource;

					private:
						rgb_color_t ColorBackground = RGB_COLOR_BLACK;
						rgb_color_t ColorForeground = RGB_COLOR_WHITE;

					public:
						bool Visible = true;

					public:
						DisabledOverlay() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height())
						{}

						~DisabledOverlay() = default;

						void SetColors(const rgb_color_t colorBackground, const rgb_color_t colorForeground)
						{
							ColorBackground = colorBackground;
							ColorForeground = colorForeground;
						}

						void Draw(IFrameBuffer* frame)
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
							if (!Visible)
								return;

							ColorSource.Color = ColorForeground;

							Base::TriangleFill(frame,
								ParallelogramLayout::Triangle1().a.x, ParallelogramLayout::Triangle1().a.y,
								ParallelogramLayout::Triangle1().b.x, ParallelogramLayout::Triangle1().b.y,
								ParallelogramLayout::Triangle1().c.x, ParallelogramLayout::Triangle1().c.y
							);

							Base::TriangleFill(frame,
								ParallelogramLayout::Triangle2().a.x, ParallelogramLayout::Triangle2().a.y,
								ParallelogramLayout::Triangle2().b.x, ParallelogramLayout::Triangle2().b.y,
								ParallelogramLayout::Triangle2().c.x, ParallelogramLayout::Triangle2().c.y
							);

							Base::TriangleFill(frame,
								ParallelogramLayout::Triangle3().a.x, ParallelogramLayout::Triangle3().a.y,
								ParallelogramLayout::Triangle3().b.x, ParallelogramLayout::Triangle3().b.y,
								ParallelogramLayout::Triangle3().c.x, ParallelogramLayout::Triangle3().c.y
							);

							Base::TriangleFill(frame,
								ParallelogramLayout::Triangle4().a.x, ParallelogramLayout::Triangle4().a.y,
								ParallelogramLayout::Triangle4().b.x, ParallelogramLayout::Triangle4().b.y,
								ParallelogramLayout::Triangle4().c.x, ParallelogramLayout::Triangle4().c.y
							);

							ColorSource.Color = ColorBackground;
							for (size_t i = 0; i < ParallelogramLayout::BackgroundLinesCount(); i++)
							{
								Base::Line(frame,
									ParallelogramLayout::LineTop(i).start.x, ParallelogramLayout::LineTop(i).start.y,
									ParallelogramLayout::LineTop(i).end.x, ParallelogramLayout::LineTop(i).end.y);
								Base::Line(frame,
									ParallelogramLayout::LineBottom(i).start.x, ParallelogramLayout::LineBottom(i).start.y,
									ParallelogramLayout::LineBottom(i).end.x, ParallelogramLayout::LineBottom(i).end.y);
							}
						}
					};
				}
			}
		}
	}
}
#endif