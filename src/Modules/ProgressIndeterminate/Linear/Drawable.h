#ifndef _EGFX_MODULES_PROGRESS_INDETERMINATE_LINEAR_DRAWABLE_h
#define _EGFX_MODULES_PROGRESS_INDETERMINATE_LINEAR_DRAWABLE_h

#include "Constant.h"
#include "Layout.h"

namespace Egfx
{
	namespace Modules
	{
		namespace ProgressIndeterminate
		{
			namespace Linear
			{
				namespace Drawable
				{
					template<typename dimension_t,
						typename ParentLayout,
						typename PixelShaderType
					>
					class IndicatorRectangle : public Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>
					{
					private:
						using Base = Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>;
						using LayoutCalc = typename Layout::Calc<dimension_t, ParentLayout>;

						//public:
						//using SegmentPair = typename SegmentPair<dimension_t>;

					private:
						SegmentPair<dimension_t> Segments = { 0, 0, 0, 0 };

					public:
						IndicatorRectangle() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height()) {}

						~IndicatorRectangle() = default;

						void SetSegments(const SegmentPair<dimension_t> segments)
						{
							Segments = segments;
						}

						void Draw(IFrameBuffer* frame)
						{
							if (Segments.LeftStart == Segments.LeftEnd)
								return;

							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
							if (LayoutCalc::IsVertical())
							{
								if (LayoutCalc::IsReversed())
								{

								}
								else
								{

								}
							}
							else
							{
								if (LayoutCalc::IsReversed())
								{

								}
								else
								{
									const dimension_t leftWidth = Segments.LeftEnd - Segments.LeftStart;
									if (leftWidth > 0)
										Base::RectangleFill(frame, LayoutCalc::Bounds().topLeft.x + Segments.LeftStart, LayoutCalc::Bounds().topLeft.y,
											LayoutCalc::Bounds().topLeft.x + Segments.LeftEnd, LayoutCalc::Bounds().bottomRight.y);

									const dimension_t rightWidth = Segments.RightEnd - Segments.RightStart;
									if (rightWidth > 0)
										Base::RectangleFill(frame, LayoutCalc::Bounds().topLeft.x + Segments.RightStart, LayoutCalc::Bounds().topLeft.y,
											LayoutCalc::Bounds().topLeft.x + Segments.RightEnd, LayoutCalc::Bounds().bottomRight.y);
								}
							}
						}
					};

					template<typename dimension_t,
						typename ParentLayout,
						typename PixelShaderType
					>
					class TrackReal : public Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>
					{
					private:
						using Base = Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>;
						using LayoutCalc = typename Layout::Calc<dimension_t, ParentLayout>;

						//public:
						//using SegmentPair = typename SegmentPair<dimension_t>;

					private:
						SegmentPair<dimension_t> Segments = { 0, 0, 0, 0 };

					public:
						TrackReal() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height()) {}

						~TrackReal() = default;

						void SetSegments(const SegmentPair<dimension_t> segments)
						{
							Segments = segments;
						}

						void Draw(IFrameBuffer* frame)
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
							if (LayoutCalc::IsVertical())
							{
								if (LayoutCalc::IsReversed())
								{

								}
								else
								{

								}
							}
							else
							{
								if (LayoutCalc::IsReversed())
								{

								}
								else
								{
									const bool has1 = (Segments.LeftStart < Segments.LeftEnd);
									const bool has2 = (Segments.RightStart < Segments.RightEnd);

									if (!has1 && !has2)
									{
										Base::RectangleFill(frame, LayoutCalc::Bounds().topLeft.x, LayoutCalc::Bounds().topLeft.y,
											LayoutCalc::Bounds().topLeft.x + LayoutCalc::RangeLength(), LayoutCalc::Bounds().bottomRight.y);
										return;
									}

									// Gap 1: Start to first segment
									if (has1 && Segments.LeftStart > 0)
										Base::RectangleFill(frame, LayoutCalc::Bounds().topLeft.x, LayoutCalc::Bounds().topLeft.y,
											LayoutCalc::Bounds().topLeft.x + Segments.LeftStart, LayoutCalc::Bounds().bottomRight.y);
									else if (!has1 && has2 && Segments.RightStart > 0)
										Base::RectangleFill(frame, LayoutCalc::Bounds().topLeft.x, LayoutCalc::Bounds().topLeft.y,
											LayoutCalc::Bounds().topLeft.x + Segments.RightStart, LayoutCalc::Bounds().bottomRight.y);

									// Gap 2: Between segments (if both active)
									if (has1 && has2 && Segments.LeftEnd < Segments.RightStart)
										Base::RectangleFill(frame, LayoutCalc::Bounds().topLeft.x + Segments.LeftEnd, LayoutCalc::Bounds().topLeft.y,
											LayoutCalc::Bounds().topLeft.x + Segments.RightStart, LayoutCalc::Bounds().bottomRight.y);

									// Gap 3: Last segment to end
									if (has2 && Segments.RightEnd < LayoutCalc::RangeLength())
										Base::RectangleFill(frame, LayoutCalc::Bounds().topLeft.x + MaxValue(Segments.LeftEnd, Segments.RightEnd),
											LayoutCalc::Bounds().topLeft.y, LayoutCalc::Bounds().topLeft.x + LayoutCalc::RangeLength(), LayoutCalc::Bounds().bottomRight.y);
									else if (has1 && !has2 && Segments.LeftEnd < LayoutCalc::RangeLength())
										Base::RectangleFill(frame, LayoutCalc::Bounds().topLeft.x + Segments.LeftEnd, LayoutCalc::Bounds().topLeft.y,
											LayoutCalc::Bounds().topLeft.x + LayoutCalc::RangeLength(), LayoutCalc::Bounds().bottomRight.y);
								}
							}
						}
					};

					template<typename dimension_t>
					class TrackMock
					{
					public:
						TrackMock() {}
						~TrackMock() = default;

						static constexpr bool IsVisible() { return false; }
						static constexpr void Draw(IFrameBuffer* /*frame*/) {}
						void SetSegments(const SegmentPair<dimension_t> /*segments*/) {}
						static constexpr void SetBounds(const dimension_t /*left*/, const dimension_t /*top*/,
							const dimension_t /*right*/, const dimension_t /*bottom*/) {}
						static constexpr void SetTranslation(const int16_t /*x*/, const int16_t /*y*/) {}
					};

					template<typename dimension_t, typename ParentLayout, typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>
					>
					using Track = typename TypeTraits::TypeConditional::conditional_type<
						TrackMock<dimension_t>,
						TrackReal<dimension_t, ParentLayout, PixelShaderType>,
						TypeTraits::TypeDispatch::is_same<PixelShaderType, Framework::Shader::Pixel::MockShader<dimension_t>>::value
					>::type;
				}
			}
		}
	}
}
#endif