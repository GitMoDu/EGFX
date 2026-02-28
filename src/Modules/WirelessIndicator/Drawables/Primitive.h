#ifndef _LOLA_GRAPHICS_MODULES_WIRELESS_INDICATOR_DRAWABLE_PRIMITIVE_h
#define _LOLA_GRAPHICS_MODULES_WIRELESS_INDICATOR_DRAWABLE_PRIMITIVE_h

#include "../Constant.h"
#include "../Layout.h"

namespace Egfx
{
	namespace Modules
	{
		namespace WirelessIndicator
		{
			namespace Drawable
			{
				namespace Primitive
				{
					template<bool Monochrome>
					class AbstractDrawable
					{
					private:
						static constexpr rgb_color_t RGB_COLOR_GRAY = Rgb::Color(uint32_t(0x808080));

					protected:
						rgb_color_t Colors[Dimensions::BarCount]
						{
							RGB_COLOR_WHITE,
							RGB_COLOR_WHITE,
							RGB_COLOR_WHITE,
							RGB_COLOR_WHITE
						};

					public:
						bool Visible = true;
						bool OddFrame = false;

					public:
						AbstractDrawable() = default;
						~AbstractDrawable() = default;

						void SetBarColor(const uint8_t barIndex, const rgb_color_t color)
						{
							Colors[barIndex] = color;
						}

						void SetBarsDitheringEnabled(const uint8_t barIndex, const bool enabled)
						{
							if (Monochrome)
							{
								if (enabled)
									Colors[barIndex] = RGB_COLOR_GRAY;
								else
									Colors[barIndex] = RGB_COLOR_WHITE;
							}
						}

					protected:
						void DrawHorizontalLine(IFrameBuffer* frame,
							const uint8_t index,
							const pixel_t startX,
							const pixel_t endX,
							const pixel_t y)
						{
							if (Monochrome)
							{
								DrawHorizontalDitherLine(frame,
									Colors[index] != RGB_COLOR_WHITE,
									startX, endX, y);
							}
							else
							{
								DrawHorizontalColorLine(frame,
									Colors[index],
									startX, endX, y);
							}
						}

						void DrawPixel(IFrameBuffer* frame,
							const uint8_t index,
							const pixel_t x,
							const pixel_t y)
						{
							if (Monochrome && Colors[index] != RGB_COLOR_WHITE)
							{
								if (Colors[index] == RGB_COLOR_BLACK)
									return;
								if (OddFrame)
								{
									frame->Pixel(RGB_COLOR_WHITE, x, y);
								}
							}
							else
							{
								frame->Pixel(Colors[index], x, y);
							}
						}

					private:
						void DrawHorizontalDitherLine(IFrameBuffer* frame,
							const bool dither,
							const pixel_t startX,
							const pixel_t endX,
							const pixel_t y)
						{
							if (dither)
							{
								const uint8_t offset = OddFrame ? 1 : 0;

								for (pixel_t x = offset; x <= (endX - startX); x += 2)
								{
									frame->Pixel(RGB_COLOR_WHITE, startX + x, y);
								}
							}
							else
							{
								for (pixel_t x = 0; x <= (endX - startX); x++)
								{
									frame->Pixel(RGB_COLOR_WHITE, startX + x, y);
								}
							}
						}

						static void DrawHorizontalColorLine(IFrameBuffer* frame,
							const rgb_color_t color,
							const pixel_t startX,
							const pixel_t endX,
							const pixel_t y)
						{
							for (pixel_t x = 0; x <= (endX - startX); x++)
							{
								frame->Pixel(color, startX + x, y);
							}
						}
					};

					template<typename ParentLayout, bool Monochrome>
					class Bars8x7 : public AbstractDrawable<Monochrome>
					{
					private:
						using Base = AbstractDrawable<Monochrome>;

					private:
						using InnerLayout = Layout::Primitives<ParentLayout>;

					public:
						Bars8x7() : Base() {}
						~Bars8x7() = default;

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

					template<typename ParentLayout, bool Monochrome>
					class Bars8x8 : public AbstractDrawable<Monochrome>
					{
					private:
						using Base = AbstractDrawable<Monochrome>;

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

					template<typename ParentLayout, bool Monochrome>
					class Bars10x7 : public AbstractDrawable<Monochrome>
					{
					private:
						using Base = AbstractDrawable<Monochrome>;

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

					template<typename ParentLayout, bool Monochrome>
					class Bars12x9 : public AbstractDrawable<Monochrome>
					{
					private:
						using Base = AbstractDrawable<Monochrome>;

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

					template<typename ParentLayout>
					class DisabledOverlay : Shader::Primitive::DisabledOverlay
					{
					private:
						using Base = Shader::Primitive::DisabledOverlay;

					public:
						rgb_color_t ColorBackground = RGB_COLOR_BLACK;
						rgb_color_t ColorForeground = RGB_COLOR_WHITE;

					public:
						DisabledOverlay() : Base()
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());

						}
						~DisabledOverlay() = default;

						void Draw(IFrameBuffer* frame)
						{
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
}
#endif