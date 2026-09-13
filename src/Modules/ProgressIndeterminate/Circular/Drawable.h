#ifndef _EGFX_MODULES_PROGRESS_INDETERMINATE_CIRCULAR_DRAWABLE_h
#define _EGFX_MODULES_PROGRESS_INDETERMINATE_CIRCULAR_DRAWABLE_h

#include "Constant.h"
#include "Layout.h"

namespace Egfx
{
	namespace Modules
	{
		namespace ProgressIndeterminate
		{
			namespace Circular
			{
				namespace Drawable
				{
					template<typename dimension_t, typename ParentLayout,
						dimension_t ArcThickness,
						typename PixelShaderType>
					class Indicator : public Framework::Shader::Geometry::CircleArcShader<dimension_t,
						Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>
					>
					{
					private:
						using Base = Framework::Shader::Geometry::CircleArcShader<dimension_t,
							Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>>;
						using LayoutCalc = Layout::Calc<dimension_t, ParentLayout, ArcThickness>;
						using ArcBoundaryModeEnum = Framework::Shader::Geometry::ArcBoundaryModeEnum;
						using RingBoundaryModeEnum = Framework::Shader::Geometry::RingBoundaryModeEnum;
						using QuadrantEnum = Framework::Shader::Geometry::QuadrantEnum;

					private:
						angle_t StartAngle = 0;
						angle_t EndAngle = 0;

					public:
						Indicator() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height()) {}
						~Indicator() = default;

						void SetFilledAngles(const angle_t angleStart, const angle_t angleEnd)
						{
							StartAngle = angleStart;
							EndAngle = angleEnd;
						}

						void Draw(IFrameBuffer* frame)
						{
							if (EndAngle == StartAngle)
								return;

							Base::Prepare(ParentLayout::X(), ParentLayout::Y());

							Base::CircleArcFill(frame,
								LayoutCalc::CenterX(), LayoutCalc::CenterY(),
								StartAngle, EndAngle,
								LayoutCalc::OuterRadius(), LayoutCalc::InnerRadius(),
								ArcBoundaryModeEnum::Inclusive, RingBoundaryModeEnum::ExcludeInnerAndOuter);
						}
					};

					template<typename dimension_t, typename ParentLayout, dimension_t ArcThickness,
						typename PixelShaderType>
					class TrackReal : public Framework::Shader::Geometry::CircleArcShader<dimension_t,
						Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>
					>
					{
					private:
						using Base = Framework::Shader::Geometry::CircleArcShader<dimension_t,
							Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>>;
						using LayoutCalc = Layout::Calc<dimension_t, ParentLayout, ArcThickness>;
						using ArcBoundaryModeEnum = Framework::Shader::Geometry::ArcBoundaryModeEnum;
						using RingBoundaryModeEnum = Framework::Shader::Geometry::RingBoundaryModeEnum;
						using QuadrantEnum = Framework::Shader::Geometry::QuadrantEnum;

					private:
						angle_t StartAngle = 0;
						angle_t EndAngle = ANGLE_RANGE;

					public:
						TrackReal() : Base()
						{

						}
						~TrackReal() = default;

						void SetFilledAngles(const angle_t angleStart, const angle_t angleEnd)
						{
							StartAngle = angleStart;
							EndAngle = angleEnd;
						}

						void Draw(IFrameBuffer* frame)
						{
							if (StartAngle - EndAngle == 0)
								return;

							Base::Prepare(ParentLayout::X(), ParentLayout::Y());

							Base::CircleArcFill(frame,
								LayoutCalc::CenterX(), LayoutCalc::CenterY(),
								EndAngle, StartAngle,
								LayoutCalc::OuterRadius(), LayoutCalc::InnerRadius(),
								ArcBoundaryModeEnum::Inclusive, RingBoundaryModeEnum::ExcludeInnerAndOuter);
						}
					};

					template<typename dimension_t>
					class TrackMock
					{
					public:
						TrackMock() {}
						~TrackMock() = default;

						static constexpr void Draw(IFrameBuffer* /*frame*/) {}
						static constexpr void SetFilledAngles(const angle_t /*angleStart*/, const angle_t /*angleEnd*/) {}
						static constexpr void SetBounds(const dimension_t /*left*/, const dimension_t /*top*/,
							const dimension_t /*right*/, const dimension_t /*bottom*/) {}
						static constexpr void SetTranslation(const int16_t /*x*/, const int16_t /*y*/) {}
					};

					template<typename dimension_t, typename ParentLayout, dimension_t ArcThickness,
						typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>
					>
					using Track = typename TypeTraits::TypeConditional::conditional_type<
						TrackMock<dimension_t>,
						TrackReal<dimension_t, ParentLayout, ArcThickness, PixelShaderType>,
						TypeTraits::TypeDispatch::is_same<PixelShaderType, Framework::Shader::Pixel::MockShader<dimension_t>>::value
					>::type;
				}
			}
		}
	}
}

#endif
