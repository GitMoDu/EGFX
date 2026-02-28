#ifndef _LOLA_GRAPHICS_MODULES_WIRELESS_INDICATOR_DRAWABLE_BITMASK_h
#define _LOLA_GRAPHICS_MODULES_WIRELESS_INDICATOR_DRAWABLE_BITMASK_h

#include "../Constant.h"
#include "../Layout.h"
#include "../Shader.h"

#include "../Assets.h"

namespace Egfx
{
	namespace Modules
	{
		namespace WirelessIndicator
		{
			namespace Drawable
			{
				namespace Bitmask
				{
					using dimension_t = pixel_t;

					static constexpr uint8_t OverscaleX = 1;

					template<typename ParentLayout,
						dimension_t BitmaskWidth,
						dimension_t BitmaskHeight,
						bool Monochrome
					>
					class AbstractBarsSlice : public Framework::Bitmask::BitmaskDrawable<
						ParentLayout,
						dimension_t,
						BitmaskWidth,
						BitmaskHeight,
						Framework::Bitmask::Reader::Flash,
						(OverscaleX + MaxValue<pixel_t>(1, ParentLayout::Width() / BitmaskWidth)),
						MaxValue<pixel_t>(1, ParentLayout::Height() / BitmaskHeight),
						Shader::Primitive::Bar<
						BitmaskWidth* (OverscaleX + MaxValue<pixel_t>(1, ParentLayout::Width() / BitmaskWidth)),
						BitmaskHeight* MaxValue<pixel_t>(1, ParentLayout::Height() / BitmaskHeight),
						Monochrome
						>
					>
					{
					private:
						using Base = Framework::Bitmask::BitmaskDrawable<
							ParentLayout,
							dimension_t,
							BitmaskWidth,
							BitmaskHeight,
							Framework::Bitmask::Reader::Flash,
							(OverscaleX + MaxValue<pixel_t>(1, ParentLayout::Width() / BitmaskWidth)),
							MaxValue<pixel_t>(1, ParentLayout::Height() / BitmaskHeight),
							Shader::Primitive::Bar<
							BitmaskWidth* (OverscaleX + MaxValue<pixel_t>(1, ParentLayout::Width() / BitmaskWidth)),
							BitmaskHeight* MaxValue<pixel_t>(1, ParentLayout::Height() / BitmaskHeight),
							Monochrome
							>
						>;

					public:
						static constexpr dimension_t Width = BitmaskWidth * (OverscaleX + MaxValue<pixel_t>(1, ParentLayout::Width() / BitmaskWidth));
						static constexpr dimension_t Height = BitmaskHeight * MaxValue<pixel_t>(1, ParentLayout::Height() / BitmaskHeight);

					public:
						explicit AbstractBarsSlice(const uint8_t* bitmask)
							: Base(bitmask)
						{
							Base::TransformShader.SetWidth(ParentLayout::Width());
						}

						~AbstractBarsSlice() = default;
					};

					template<typename ParentLayout,
						bool Monochrome>
					class Bars22x16Slice0 : public AbstractBarsSlice<ParentLayout, Assets::Bitmask22x16::Width, Assets::Bitmask22x16::Slice0Height, Monochrome>
					{
					public:
						Bars22x16Slice0() : AbstractBarsSlice<ParentLayout, Assets::Bitmask22x16::Width, Assets::Bitmask22x16::Slice0Height, Monochrome>
							(Assets::Bitmask22x16::Slice_0_22x4) {
						}
						~Bars22x16Slice0() = default;
					};

					template<typename ParentLayout,
						bool Monochrome>
					class Bars22x16Slice1 : public AbstractBarsSlice<ParentLayout, Assets::Bitmask22x16::Width, Assets::Bitmask22x16::Slice1Height, Monochrome>
					{
					public:
						Bars22x16Slice1() : AbstractBarsSlice<ParentLayout, Assets::Bitmask22x16::Width, Assets::Bitmask22x16::Slice1Height, Monochrome>
							(Assets::Bitmask22x16::Slice_1_22x4) {
						}
						~Bars22x16Slice1() = default;
					};

					template<typename ParentLayout,
						bool Monochrome>
					class Bars22x16Slice2 : public AbstractBarsSlice<ParentLayout, Assets::Bitmask22x16::Width, Assets::Bitmask22x16::Slice2Height, Monochrome>
					{
					public:
						Bars22x16Slice2() : AbstractBarsSlice<ParentLayout, Assets::Bitmask22x16::Width, Assets::Bitmask22x16::Slice2Height, Monochrome>
							(Assets::Bitmask22x16::Slice_2_22x5) {
						}
						~Bars22x16Slice2() = default;
					};

					template<typename ParentLayout,
						bool Monochrome>
					class Bars22x16Slice3 : public AbstractBarsSlice<ParentLayout, Assets::Bitmask22x16::Width, Assets::Bitmask22x16::Slice3Height, Monochrome>
					{
					public:
						Bars22x16Slice3() : AbstractBarsSlice<ParentLayout, Assets::Bitmask22x16::Width, Assets::Bitmask22x16::Slice3Height, Monochrome>
							(Assets::Bitmask22x16::Slice_3_22x6) {
						}
						~Bars22x16Slice3() = default;
					};

					template<typename ParentLayout,
						bool Monochrome>
					class Bars24x16Slice0 : public AbstractBarsSlice<ParentLayout, Assets::Bitmask24x16::Width, Assets::Bitmask24x16::Slice0Height, Monochrome>
					{
					public:
						Bars24x16Slice0() : AbstractBarsSlice<ParentLayout, Assets::Bitmask24x16::Width, Assets::Bitmask24x16::Slice0Height, Monochrome>
							(Assets::Bitmask24x16::Slice_0_24x4) {
						}
						~Bars24x16Slice0() = default;
					};

					template<typename ParentLayout,
						bool Monochrome>
					class Bars24x16Slice1 : public AbstractBarsSlice<ParentLayout, Assets::Bitmask24x16::Width, Assets::Bitmask24x16::Slice1Height, Monochrome>
					{
					public:
						Bars24x16Slice1() : AbstractBarsSlice<ParentLayout, Assets::Bitmask24x16::Width, Assets::Bitmask24x16::Slice1Height, Monochrome>
							(Assets::Bitmask24x16::Slice_1_24x5) {
						}
						~Bars24x16Slice1() = default;
					};

					template<typename ParentLayout,
						bool Monochrome>
					class Bars24x16Slice2 : public AbstractBarsSlice<ParentLayout, Assets::Bitmask24x16::Width, Assets::Bitmask24x16::Slice2Height, Monochrome>
					{
					public:
						Bars24x16Slice2() : AbstractBarsSlice<ParentLayout, Assets::Bitmask24x16::Width, Assets::Bitmask24x16::Slice2Height, Monochrome>
							(Assets::Bitmask24x16::Slice_2_24x6) {
						}
						~Bars24x16Slice2() = default;
					};

					template<typename ParentLayout,
						bool Monochrome>
					class Bars24x16Slice3 : public AbstractBarsSlice<ParentLayout, Assets::Bitmask24x16::Width, Assets::Bitmask24x16::Slice3Height, Monochrome>
					{
					public:
						Bars24x16Slice3() : AbstractBarsSlice<ParentLayout, Assets::Bitmask24x16::Width, Assets::Bitmask24x16::Slice3Height, Monochrome>
							(Assets::Bitmask24x16::Slice_3_24x7) {
						}
						~Bars24x16Slice3() = default;
					};

					template<typename ParentLayout,
						bool Monochrome>
					class Bars42x30Slice0 : public AbstractBarsSlice<ParentLayout, Assets::Bitmask42x30::Width, Assets::Bitmask42x30::Slice0Height, Monochrome>
					{
					public:
						Bars42x30Slice0() : AbstractBarsSlice<ParentLayout, Assets::Bitmask42x30::Width, Assets::Bitmask42x30::Slice0Height, Monochrome>
							(Assets::Bitmask42x30::Slice_0_42x7) {
						}
						~Bars42x30Slice0() = default;
					};

					template<typename ParentLayout,
						bool Monochrome>
					class Bars42x30Slice1 : public AbstractBarsSlice<ParentLayout, Assets::Bitmask42x30::Width, Assets::Bitmask42x30::Slice1Height, Monochrome>
					{
					public:
						Bars42x30Slice1() : AbstractBarsSlice<ParentLayout, Assets::Bitmask42x30::Width, Assets::Bitmask42x30::Slice1Height, Monochrome>
							(Assets::Bitmask42x30::Slice_1_42x7) {
						}
						~Bars42x30Slice1() = default;
					};

					template<typename ParentLayout,
						bool Monochrome>
					class Bars42x30Slice2 : public AbstractBarsSlice<ParentLayout, Assets::Bitmask42x30::Width, Assets::Bitmask42x30::Slice2Height, Monochrome>
					{
					public:
						Bars42x30Slice2() : AbstractBarsSlice<ParentLayout, Assets::Bitmask42x30::Width, Assets::Bitmask42x30::Slice2Height, Monochrome>
							(Assets::Bitmask42x30::Slice_2_42x10) {
						}
						~Bars42x30Slice2() = default;
					};

					template<typename ParentLayout,
						bool Monochrome>
					class Bars42x30Slice3 : public AbstractBarsSlice<ParentLayout, Assets::Bitmask42x30::Width, Assets::Bitmask42x30::Slice3Height, Monochrome>
					{
					public:
						Bars42x30Slice3() : AbstractBarsSlice<ParentLayout, Assets::Bitmask42x30::Width, Assets::Bitmask42x30::Slice3Height, Monochrome>
							(Assets::Bitmask42x30::Slice_3_42x12) {
						}
						~Bars42x30Slice3() = default;
					};

					template<typename ParentLayout>
					class DisabledOverlay : Shader::Primitive::DisabledOverlay
					{
					private:
						using Base = Shader::Primitive::DisabledOverlay;

						using ParallelogramLayout = Layout::ParallelogramLayout<ParentLayout>;

					public:
						using Base::ColorSource;

					private:
						rgb_color_t ColorBackground = RGB_COLOR_BLACK;
						rgb_color_t ColorForeground = RGB_COLOR_WHITE;

					public:
						bool Visible = true;

					public:
						DisabledOverlay() : Base()
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
						}

						~DisabledOverlay() = default;

						void SetColors(const rgb_color_t colorBackground, const rgb_color_t colorForeground)
						{
							ColorBackground = colorBackground;
							ColorForeground = colorForeground;
						}

						void Draw(IFrameBuffer* frame)
						{
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