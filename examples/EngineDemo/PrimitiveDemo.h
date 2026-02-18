#ifndef _PRIMITIVE_DEMO_h
#define _PRIMITIVE_DEMO_h

#include <EgfxFramework.h>
#include <EgfxAssets.h>


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

		namespace Animator
		{
			template<typename ParentLayout, bool Monochrome>
			struct Lines
			{
				template<typename LineElementType>
				static void Animate(LineElementType& lineElement, const uint32_t frameTime, const uint16_t /*frameCounter*/)
				{
					lineElement.ColorSource.Color = Monochrome ? RGB_COLOR_WHITE :
						Rgb::ColorFromHSV(static_cast<angle_t>(frameTime / 100), UINT8_MAX, (uint16_t(UINT8_MAX) * 9) / 10);

					// Calculate deltaX
					pixel_t deltaX = (frameTime / 70000) % ParentLayout::Width();
					if (deltaX >= ParentLayout::Width() / 2)
					{
						deltaX = (ParentLayout::Width() - deltaX) * 2;
					}
					else
					{
						deltaX = deltaX * 2;
					}
					lineElement.DeltaX = deltaX;

					// Calculate deltaY
					pixel_t deltaY = (frameTime / 60000) % ParentLayout::Height();
					if (deltaY >= ParentLayout::Height() / 2)
					{
						deltaY = (ParentLayout::Height() - deltaY) * 2;
					}
					else
					{
						deltaY = deltaY * 2;
					}
					lineElement.DeltaY = deltaY;
				}
			};

			template<typename ParentLayout, bool Monochrome>
			struct Triangle
			{
				template<typename TriangleElementType>
				static pixel_rectangle_t Animate(TriangleElementType& triangleElement, const uint32_t frameTime, const uint16_t /*frameCounter*/)
				{
					triangleElement.ColorSource.Color = GetColor(frameTime);
					triangleElement.TriangleA = GetRoundSquare<ParentLayout::MaxHeight()>(frameTime / 90);
					triangleElement.TriangleB = GetRoundSquare<ParentLayout::MaxHeight()>(frameTime / 166);
					triangleElement.TriangleC = GetRoundSquare<ParentLayout::MaxHeight()>(frameTime / 75);

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
		}

		namespace Shader
		{
			namespace Pixel
			{
				using Lines = Framework::Shader::Pixel::TemplateShader<pixel_t, Framework::Shader::Source::SingleColor<pixel_t, RGB_COLOR_WHITE>>;
				using Triangle = Framework::Shader::Pixel::TemplateShader<pixel_t, Framework::Shader::Source::SingleColor<pixel_t, RGB_COLOR_WHITE>>;

				using Rectangle = Framework::Shader::Pixel::TemplateShader<pixel_t,
					Framework::Shader::Source::StaticColor<pixel_t, RGB_COLOR_WHITE>,
					Framework::Shader::Color::NoShader<pixel_t>,
					Framework::Assets::Shader::Transform::CheckerboardMask<pixel_t>
				>;
			}

			namespace Primitive
			{
				using Lines = Framework::Shader::Primitive::TemplateShader<pixel_t, Pixel::Lines>;
				using Triangle = Framework::Shader::Primitive::TemplateShader<pixel_t, Pixel::Triangle>;

				using Rectangle = Framework::Shader::Primitive::TemplateShader<pixel_t, Pixel::Rectangle>;
			}
		}

		namespace Drawable
		{
			/// <summary>
			/// Passive lines drawable - renders animated lines based on current state.
			/// </summary>
			template<typename ParentLayout, bool Monochrome>
			class Lines : public Shader::Primitive::Lines
			{
			private:
				using Base = Shader::Primitive::Lines;

			public:
				pixel_t DeltaX = 0;
				pixel_t DeltaY = 0;

			public:
				Lines() : Base()
				{
					Base::Prepare(ParentLayout::X(), ParentLayout::Y());
				}

				~Lines() = default;

				void Draw(IFrameBuffer* frame)
				{
					for (uint8_t i = 0; i < ParentLayout::HorizontalCount(); i++)
					{
						const pixel_t x = (ParentLayout::Width() / 2) + ((((ParentLayout::HorizontalSeparation() / 2) * i) + DeltaX / 2) % (ParentLayout::Width() / 2));
						Base::Line(frame, x, 0, x + DeltaX, ParentLayout::Height() - 1);

						const pixel_t y = ((((ParentLayout::VerticalSeparation() / 2) * i) + DeltaY / 2) % ParentLayout::Height());
						Base::Line(frame, 0, y, ParentLayout::Width() / 2, y + DeltaY);
					}
				}
			};

			/// <summary>
			/// Passive triangle drawable - renders triangle based on current state.
			/// </summary>
			template<typename ParentLayout, bool Monochrome>
			class Triangle : public Shader::Primitive::Triangle
			{
			private:
				using Base = Shader::Primitive::Triangle;

			public:
				pixel_point_t TriangleA{};
				pixel_point_t TriangleB{};
				pixel_point_t TriangleC{};

			public:
				Triangle() : Base()
				{
					Base::Prepare(ParentLayout::CenterX(), ParentLayout::CenterY());
				}

				~Triangle() = default;

				void Draw(IFrameBuffer* frame)
				{
					Base::TriangleFill(frame,
						TriangleA.x, TriangleA.y,
						TriangleB.x, TriangleB.y,
						TriangleC.x, TriangleC.y);
				}
			};

			/// <summary>
			/// Passive rectangle drawable - renders bounding box around triangle.
			/// </summary>
			template<typename ParentLayout, bool Monochrome>
			class Rectangle : public Shader::Primitive::Rectangle
			{
			private:
				using Base = Shader::Primitive::Rectangle;

			public:
				pixel_rectangle_t BoundingBox{};

			public:
				Rectangle() : Base()
				{
					Base::Prepare(ParentLayout::CenterX(), ParentLayout::CenterY());
				}

				~Rectangle() = default;

				void Draw(IFrameBuffer* frame)
				{
					Base::RectangleFill(frame,
						BoundingBox.topLeft.x, BoundingBox.topLeft.y,
						BoundingBox.bottomRight.x,
						BoundingBox.bottomRight.y);
				}
			};
		}
	}

	template<typename ParentLayout, bool Monochrome>
	class View : public Framework::View::DrawablesView<
		Assets::Drawable::Lines<typename Assets::Layout<ParentLayout>::Lines, Monochrome>,
		Assets::Drawable::Rectangle<typename Assets::Layout<ParentLayout>::Rectangle, Monochrome>,
		Assets::Drawable::Triangle<typename Assets::Layout<ParentLayout>::Triangle, Monochrome>
	>
	{
	private:
		using Base = Framework::View::DrawablesView<
			Assets::Drawable::Lines<typename Assets::Layout<ParentLayout>::Lines, Monochrome>,
			Assets::Drawable::Rectangle<typename Assets::Layout<ParentLayout>::Rectangle, Monochrome>,
			Assets::Drawable::Triangle<typename Assets::Layout<ParentLayout>::Triangle, Monochrome>>;

	private:
		using LinesAnimator = Assets::Animator::Lines<typename Assets::Layout<ParentLayout>::Lines, Monochrome>;
		using TriangleAnimator = Assets::Animator::Triangle<typename Assets::Layout<ParentLayout>::Triangle, Monochrome>;

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
		bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
		{
			// Get references to drawables by index. The order is determined by the class template parameters.
			auto& lines = Base::template drawable<0>();
			auto& rectangle = Base::template drawable<1>();
			auto& triangle = Base::template drawable<2>();

			LinesAnimator::Animate(lines, frameTime, frameCounter);

			// Animate triangle and set the resulting bounding box on rectangle drawable
			rectangle.BoundingBox = TriangleAnimator::Animate(triangle, frameTime, frameCounter);
			return true;
		}
	};

	/// <summary>
	/// Wrapper type exposing the demo's single view via the IFrameDraw interface.
	/// </summary>
	template<typename ParentLayout, bool Monochrome>
	struct Frame : Framework::View::FrameAdapter<View<ParentLayout, Monochrome>>
	{
		using Base = Framework::View::FrameAdapter<View<ParentLayout, Monochrome>>;
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