#ifndef _PRIMITIVE_DEMO_h
#define _PRIMITIVE_DEMO_h

#include <EgfxDrawer.h>

// Demo of primitive shapes: lines, rectangles, and triangles.
namespace PrimitiveDemo
{
	using namespace Egfx;

	namespace Assets
	{
		template<typename ParentLayout>
		struct Layout
		{
			struct Lines
			{
				static constexpr pixel_t X() { return ParentLayout::X(); }
				static constexpr pixel_t Y() { return ParentLayout::Y(); }
				static constexpr pixel_t Width() { return ParentLayout::Width(); }
				static constexpr pixel_t Height() { return ParentLayout::Height(); }
				static constexpr pixel_t CenterX() { return X() + Width() / 2; }
				static constexpr pixel_t CenterY() { return Y() + Height() / 2; }
				static constexpr pixel_t HorizontalSeparation() { return ParentLayout::Width() / 4; }
				static constexpr pixel_t VerticalSeparation() { return ParentLayout::Height() / 4; }
				static constexpr pixel_t HorizontalCount() { return ParentLayout::Width() / HorizontalSeparation(); }
				static constexpr pixel_t VerticalCount() { return ParentLayout::Height() / VerticalSeparation(); }
			};

			struct Rectangle
			{
				static constexpr pixel_t X() { return ParentLayout::X(); }
				static constexpr pixel_t Y() { return ParentLayout::Y(); }
				static constexpr pixel_t Width() { return ParentLayout::Width(); }
				static constexpr pixel_t Height() { return ParentLayout::Height(); }
				static constexpr pixel_t CenterX() { return ParentLayout::X() + (ParentLayout::Width() / 2); }
				static constexpr pixel_t CenterY() { return ParentLayout::Y() + (ParentLayout::Height() / 2); }
			};

			struct Triangle
			{
				static constexpr pixel_t X() { return ParentLayout::X(); }
				static constexpr pixel_t Y() { return ParentLayout::Y(); }
				static constexpr pixel_t Width() { return ParentLayout::Width(); }
				static constexpr pixel_t Height() { return ParentLayout::Height(); }
				static constexpr pixel_t CenterX() { return ParentLayout::X() + (ParentLayout::Width() / 2); }
				static constexpr pixel_t CenterY() { return ParentLayout::Y() + (ParentLayout::Height() / 2); }
				static constexpr pixel_t MaxHeight() { return ((MinValue(ParentLayout::Width(), ParentLayout::Height())) * 3) / 4; }
			};
		};

		namespace Animators
		{
			template<typename Layout, bool Monochrome>
			struct Lines
			{
				template<typename LineElementType>
				static void Animate(LineElementType& lineElement, const uint32_t frameTime, const uint16_t /*frameCounter*/)
				{
					lineElement.Color = Monochrome ? RGB_COLOR_WHITE :
						Rgb::ColorFromHSV(static_cast<angle_t>(frameTime / 100), UINT8_MAX, (uint16_t(UINT8_MAX) * 9) / 10);

					// Calculate deltaX
					pixel_t deltaX = (frameTime / 70000) % Layout::Width();
					if (deltaX >= Layout::Width() / 2)
					{
						deltaX = (Layout::Width() - deltaX) * 2;
					}
					else
					{
						deltaX = deltaX * 2;
					}
					lineElement.DeltaX = deltaX;

					// Calculate deltaY
					pixel_t deltaY = (frameTime / 60000) % Layout::Height();
					if (deltaY >= Layout::Height() / 2)
					{
						deltaY = (Layout::Height() - deltaY) * 2;
					}
					else
					{
						deltaY = deltaY * 2;
					}
					lineElement.DeltaY = deltaY;
				}
			};

			template<typename Layout, bool Monochrome>
			struct Triangle
			{
				template<typename TriangleElementType>
				static pixel_rectangle_t Animate(TriangleElementType& triangleElement, const uint32_t frameTime, const uint16_t /*frameCounter*/)
				{
					triangleElement.Color = GetColor(frameTime);
					triangleElement.TriangleA = GetRoundSquare<Layout::MaxHeight()>(frameTime / 90);
					triangleElement.TriangleB = GetRoundSquare<Layout::MaxHeight()>(frameTime / 166);
					triangleElement.TriangleC = GetRoundSquare<Layout::MaxHeight()>(frameTime / 75);

					pixel_rectangle_t boundingBox{};
					boundingBox.topLeft.x = MinValue(triangleElement.TriangleA.x, MinValue(triangleElement.TriangleB.x, triangleElement.TriangleC.x));
					boundingBox.topLeft.y = MinValue(triangleElement.TriangleA.y, MinValue(triangleElement.TriangleB.y, triangleElement.TriangleC.y));
					boundingBox.bottomRight.x = MaxValue(triangleElement.TriangleA.x, MaxValue(triangleElement.TriangleB.x, triangleElement.TriangleC.x));
					boundingBox.bottomRight.y = MaxValue(triangleElement.TriangleA.y, MaxValue(triangleElement.TriangleB.y, triangleElement.TriangleC.y));

					const auto xShift = -((boundingBox.topLeft.x + boundingBox.bottomRight.x) / 2);
					const auto yShift = -((boundingBox.topLeft.y + boundingBox.bottomRight.y) / 2);

					triangleElement.TriangleA.x += xShift;
					triangleElement.TriangleA.y += yShift;
					triangleElement.TriangleB.x += xShift;
					triangleElement.TriangleB.y += yShift;
					triangleElement.TriangleC.x += xShift;
					triangleElement.TriangleC.y += yShift;

					boundingBox.topLeft.x += xShift;
					boundingBox.topLeft.y += yShift;
					boundingBox.bottomRight.x += xShift;
					boundingBox.bottomRight.y += yShift;

					return boundingBox;
				}

			private:
				static rgb_color_t GetColor(const uint32_t frameTime)
				{
					if (Monochrome)
					{
						return RGB_COLOR_WHITE;
					}
					else
					{
						const uint8_t segment = (frameTime / 400000) % 3;
						switch (segment)
						{
						case 0:
							return RGB_COLOR_RED;
						case 1:
							return RGB_COLOR_GREEN;
						case 2:
						default:
							return RGB_COLOR_BLUE;
						}
					}
				}

				template<pixel_t SquareDimension>
				static pixel_point_t GetRoundSquare(const uint16_t progress)
				{
					const uint16_t section = ProgressScaler::ScaleProgress(progress, uint8_t(4));
					const uint16_t innerProgress = static_cast<uint16_t>(progress * 4);
					switch (section)
					{
					case 0:
						return pixel_point_t{
							static_cast<pixel_t>(ProgressScaler::ScaleProgress(innerProgress, uint16_t(SquareDimension))),
							static_cast<pixel_t>(0),
						};
					case 1:
						return pixel_point_t{
							static_cast<pixel_t>(SquareDimension - 1),
							static_cast<pixel_t>(ProgressScaler::ScaleProgress(innerProgress, uint16_t(SquareDimension))),
						};
					case 2:
						return pixel_point_t{
							static_cast<pixel_t>(SquareDimension - 1 - int16_t(ProgressScaler::ScaleProgress(innerProgress, uint16_t(SquareDimension)))),
							static_cast<pixel_t>(SquareDimension - 1),
						};
					case 3:
					default:
						return pixel_point_t{
							static_cast<pixel_t>(0),
							static_cast<pixel_t>(SquareDimension - 1 - int16_t(ProgressScaler::ScaleProgress(innerProgress, uint16_t(SquareDimension)))),
						};
					}
				}
			};

			template<bool Monochrome>
			struct Rectangle
			{
				template<typename RectangleElementType>
				static void Animate(RectangleElementType& rectangleElement, const uint32_t /*frameTime*/, const uint16_t frameCounter)
				{
					rectangleElement.Active = Monochrome ? (frameCounter & 0b1) : true;
				}
			};
		}

		namespace Drawables
		{
			/// <summary>
			/// Passive lines drawable - renders animated lines based on current state.
			/// </summary>
			template<typename Layout, bool Monochrome>
			struct Lines
			{
				pixel_t DeltaX = 0;
				pixel_t DeltaY = 0;
				rgb_color_t Color = RGB_COLOR_WHITE;

				void Draw(IFrameBuffer* frame)
				{
					for (uint8_t i = 0; i < Layout::HorizontalCount(); i++)
					{
						const pixel_t x = Layout::CenterX() + ((((Layout::HorizontalSeparation() / 2) * i) + DeltaX / 2) % (Layout::Width() / 2));
						frame->Line(Color, x, Layout::Y(), x + DeltaX, Layout::Y() + Layout::Height() - 1);

						const pixel_t y = Layout::Y() + ((((Layout::VerticalSeparation() / 2) * i) + DeltaY / 2) % Layout::Height());
						frame->Line(Color, Layout::X(), y, Layout::CenterX(), y + DeltaY);
					}
				}
			};

			/// <summary>
			/// Passive triangle drawable - renders triangle based on current state.
			/// </summary>
			template<typename Layout, bool Monochrome>
			struct Triangle
			{
				pixel_point_t TriangleA{};
				pixel_point_t TriangleB{};
				pixel_point_t TriangleC{};

				rgb_color_t Color = RGB_COLOR_WHITE;

				void Draw(IFrameBuffer* frame)
				{
					// Draw triangle with current color
					frame->TriangleFill(Color,
						Layout::CenterX() + TriangleA.x, Layout::CenterY() + TriangleA.y,
						Layout::CenterX() + TriangleB.x, Layout::CenterY() + TriangleB.y,
						Layout::CenterX() + TriangleC.x, Layout::CenterY() + TriangleC.y);
				}
			};

			/// <summary>
			/// Passive rectangle drawable - renders bounding box around triangle.
			/// </summary>
			template<typename Layout, bool Monochrome>
			struct Rectangle
			{
				pixel_rectangle_t BoundingBox{};

				bool Active = true;

				void Draw(IFrameBuffer* frame)
				{
					// Draw rectangle around triangle
					if (Active)
						frame->RectangleFill(RGB_COLOR_WHITE,
							Layout::CenterX() + BoundingBox.topLeft.x, Layout::CenterY() + BoundingBox.topLeft.y,
							Layout::CenterX() + BoundingBox.bottomRight.x,
							Layout::CenterY() + BoundingBox.bottomRight.y);
				}
			};
		}
	}

	template<typename Layout, bool Monochrome>
	class View : public Egfx::Framework::View::DrawablesView<
		Assets::Drawables::Lines<typename Assets::Layout<Layout>::Lines, Monochrome>,
		Assets::Drawables::Rectangle<typename Assets::Layout<Layout>::Rectangle, Monochrome>,
		Assets::Drawables::Triangle<typename Assets::Layout<Layout>::Triangle, Monochrome>
	>
	{
	private:
		using Base = Egfx::Framework::View::DrawablesView<
			Assets::Drawables::Lines<typename Assets::Layout<Layout>::Lines, Monochrome>,
			Assets::Drawables::Rectangle<typename Assets::Layout<Layout>::Rectangle, Monochrome>,
			Assets::Drawables::Triangle<typename Assets::Layout<Layout>::Triangle, Monochrome>>;

	private:
		using LinesAnimator = Assets::Animators::Lines<typename Assets::Layout<Layout>::Lines, Monochrome>;
		using RectangleAnimator = Assets::Animators::Rectangle<Monochrome>;
		using TriangleAnimator = Assets::Animators::Triangle<typename Assets::Layout<Layout>::Triangle, Monochrome>;

	public:
		View() : Base()
		{
		}

		~View() = default;

	protected:
		/// <summary>
		/// View-level animation orchestration - updates all drawable state.
		/// No framebuffer access - pure state update.
		/// </summary>
		void ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
		{
			// Get references to drawables by index. The order is determined by the class template parameters.
			auto& lines = Base::template drawable<0>();
			auto& rectangle = Base::template drawable<1>();
			auto& triangle = Base::template drawable<2>();

			LinesAnimator::Animate(lines, frameTime, frameCounter);
			RectangleAnimator::Animate(rectangle, frameTime, frameCounter);

			// Animate triangle and set the resulting bounding box on rectangle drawable
			rectangle.BoundingBox = TriangleAnimator::Animate(triangle, frameTime, frameCounter);
		}
	};

	/// <summary>
	/// Wrapper type exposing the demo's single view via the IFrameDraw interface.
	/// </summary>
	template<typename Layout, bool Monochrome>
	struct Frame : Egfx::Framework::View::FrameAdapter<View<Layout, Monochrome>>
	{
		using Base = Egfx::Framework::View::FrameAdapter<View<Layout, Monochrome>>;
		Frame() : Base() {}
		virtual ~Frame() = default;

#if defined(SERIAL_LOG)
		void PrintDescription() const
		{
			Serial.println();
			Serial.print(F("Primitives\n\tfast draw lines, rectangles, and triangles."));
		}
#endif
	};
}
#endif