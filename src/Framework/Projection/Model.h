#ifndef _EGFX_FRAMEWORK_PROJECTION_MODEL_h
#define _EGFX_FRAMEWORK_PROJECTION_MODEL_h

#include "../Layout/Model.h"
#include "../Shader/Model.h"
#include "../View/Model.h"

namespace Egfx
{
	namespace Framework
	{
		/// <summary>
		/// Projection maps controller state to child viewports. Projectors provide geometry policies,
		/// while projection views own controller semantics and relay effective bounds and translation.
		/// </summary>
		namespace Projection
		{
			// Projection view packs support up to 255 children.
			using count_t = uint8_t;

			// Use a wide unsigned type for viewport dimensions while view coordinates remain native dimension_t.
			using dimension_t = uint16_t;

			/// <summary>
			/// Runtime bounds and translation calculated for one projected child.
			/// </summary>
			using viewport_t = Shader::viewport_t<dimension_t>;

			/// <summary>
			/// Projection effects derive normalized values from a view's effective projected viewport
			/// while leaving interpretation to the wrapped view.
			/// </summary>

			enum class OrientationEnum
			{
				// Used by effect adapters and layout styles to define axis and direction of travel.
				LeftToRight,
				RightToLeft,
				TopToBottom,
				BottomToTop
			};

			template<
				OrientationEnum orientation = OrientationEnum::LeftToRight,
				uint16_t itemMarginLeft = 0,
				uint16_t itemMarginTop = 0,
				uint16_t itemMarginRight = 0,
				uint16_t itemMarginBottom = 0,
				uint8_t visibleCount = 1,
				uint8_t visibleRange = 1
			>
			struct ListStyle
			{
				static constexpr OrientationEnum Orientation = orientation;
				static constexpr uint8_t VisibleCount = visibleCount;
				static constexpr uint8_t VisibleRange = visibleRange;

				static constexpr uint16_t ItemMarginLeft = itemMarginLeft;
				static constexpr uint16_t ItemMarginTop = itemMarginTop;
				static constexpr uint16_t ItemMarginRight = itemMarginRight;
				static constexpr uint16_t ItemMarginBottom = itemMarginBottom;
			};

			template<
				OrientationEnum orientation = OrientationEnum::LeftToRight,
				uint16_t itemMarginLeft = 0,
				uint16_t itemMarginTop = 0,
				uint16_t itemMarginRight = 0,
				uint16_t itemMarginBottom = 0
			>
			using PageSlideStyle = ListStyle<orientation, itemMarginLeft, itemMarginTop, itemMarginRight, itemMarginBottom, 1, 1>;


			namespace Contract
			{
				/// <summary>
				/// Documents the minimum shape expected of a projection projector.
				/// This serves as documentation only and is not intended to be inherited from.
				/// Concrete projectors should define their controller and state types and provide
				/// a viewport for each projected child.
				/// </summary>
				struct ProjectorContract
				{
					/// <summary>
					/// Placeholder for the controller type used by a concrete projector.
					/// </summary>
					using ControllerType = void*;

					/// <summary>
					/// Placeholder for the immutable controller state consumed by projection.
					/// </summary>
					using StateType = void*;

					/// <summary>
					/// Describes the viewport to apply to one child for the supplied state.
					/// </summary>
					viewport_t GetViewport(uint8_t, const StateType&) const { return {}; };
				};
			}
		}
	}
}
#endif