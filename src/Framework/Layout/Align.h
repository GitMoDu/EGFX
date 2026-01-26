#ifndef _EGFX_FRAMEWORK_LAYOUT_ALIGN_h
#define _EGFX_FRAMEWORK_LAYOUT_ALIGN_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Layout
		{
			/// <summary>
			/// Alignment position of a child rectangle within its parent rectangle.
			/// Used by Layout::Align to place a child layout inside a parent layout.
			/// </summary>
			enum class AlignmentEnum : uint8_t
			{
				TopLeft = 0,
				TopCenter = 1,
				TopRight = 2,
				MiddleLeft = 3,
				MiddleCenter = 4,
				MiddleRight = 5,
				BottomLeft = 6,
				BottomCenter = 7,
				BottomRight = 8
			};

			/// <summary>
			/// Aligns a child layout within a parent layout.
			///
			/// - Keeps the child layout's size (Width/Height) unchanged.
			/// - Moves the child layout by an X/Y offset so it sits at the requested AlignmentEnum position.
			/// - Requires ChildLayout::Width() <= ParentLayout::Width() and ChildLayout::Height() <= ParentLayout::Height().
			/// - For center alignments, uses a rounded half remainder ((remainder + 1) / 2) to avoid a persistent
			///   left/up bias when the remainder is odd.
			///
			/// Cell-forwarding:
			/// - If ChildLayout provides Cell<Index> and/or CellIndex<Index>, Align exposes the same nested templates
			///   and applies the same X/Y offset to the returned cell coordinates.
			/// </summary>
			/// <typeparam name="ParentLayout">The containing layout to align inside.</typeparam>
			/// <typeparam name="ChildLayout">The child layout to reposition within the parent.</typeparam>
			/// <typeparam name="alignment">The desired alignment of the child inside the parent.</typeparam>
			template<typename ParentLayout,
				typename ChildLayout,
				AlignmentEnum alignment = AlignmentEnum::MiddleCenter
			>
			struct Align
			{
			private:
				static constexpr pixel_t ParentWidth() { return ParentLayout::Width(); }
				static constexpr pixel_t ParentHeight() { return ParentLayout::Height(); }

				static constexpr pixel_t ChildWidth() { return ChildLayout::Width(); }
				static constexpr pixel_t ChildHeight() { return ChildLayout::Height(); }

				static_assert(ChildWidth() <= ParentWidth(), "Layout::Align requires ChildLayout::Width() <= ParentLayout::Width().");
				static_assert(ChildHeight() <= ParentHeight(), "Layout::Align requires ChildLayout::Height() <= ParentLayout::Height().");

				static constexpr pixel_t WidthRemainder() { return ParentWidth() - ChildWidth(); }
				static constexpr pixel_t HeightRemainder() { return ParentHeight() - ChildHeight(); }

				static constexpr pixel_t XOffset()
				{
					return (alignment == AlignmentEnum::TopLeft ||
						alignment == AlignmentEnum::MiddleLeft ||
						alignment == AlignmentEnum::BottomLeft)
						? 0
						: ((alignment == AlignmentEnum::TopCenter ||
							alignment == AlignmentEnum::MiddleCenter ||
							alignment == AlignmentEnum::BottomCenter)
							? ((WidthRemainder() + 1) / 2)
							: WidthRemainder());
				}

				static constexpr pixel_t YOffset()
				{
					return (alignment == AlignmentEnum::TopLeft ||
						alignment == AlignmentEnum::TopCenter ||
						alignment == AlignmentEnum::TopRight)
						? 0
						: ((alignment == AlignmentEnum::MiddleLeft ||
							alignment == AlignmentEnum::MiddleCenter ||
							alignment == AlignmentEnum::MiddleRight)
							? ((HeightRemainder() + 1) / 2)
							: HeightRemainder());
				}

			public:
				/// <summary>
				/// Aligned position of the child (top-left corner).
				/// </summary>
				static constexpr pixel_t X() { return ParentLayout::X() + XOffset(); }
				static constexpr pixel_t Y() { return ParentLayout::Y() + YOffset(); }

				/// <summary>
				/// Size of the aligned child (unchanged from ChildLayout).
				/// </summary>
				static constexpr pixel_t Width() { return ChildWidth(); }
				static constexpr pixel_t Height() { return ChildHeight(); }

				/// <summary>
				/// For child layouts exposing Cell<Index>, forwards the cell with the alignment offset applied to X/Y.
				/// </summary>
				template<uint8_t Index>
				struct Cell
				{
					static constexpr pixel_t X() { return ChildLayout::template Cell<Index>::X() + XOffset(); }
					static constexpr pixel_t Y() { return ChildLayout::template Cell<Index>::Y() + YOffset(); }
					static constexpr pixel_t Width() { return ChildLayout::template Cell<Index>::Width(); }
					static constexpr pixel_t Height() { return ChildLayout::template Cell<Index>::Height(); }
				};

				/// <summary>
				/// For child layouts exposing CellIndex<Index>, forwards the cell with the alignment offset applied to X/Y.
				/// Also forwards Row()/Col() if the child provides them.
				/// </summary>
				template<uint8_t Index>
				struct CellIndex
				{
					static constexpr uint8_t Row() { return ChildLayout::template CellIndex<Index>::Row(); }
					static constexpr uint8_t Col() { return ChildLayout::template CellIndex<Index>::Col(); }

					static constexpr pixel_t X() { return ChildLayout::template CellIndex<Index>::X() + XOffset(); }
					static constexpr pixel_t Y() { return ChildLayout::template CellIndex<Index>::Y() + YOffset(); }
					static constexpr pixel_t Width() { return ChildLayout::template CellIndex<Index>::Width(); }
					static constexpr pixel_t Height() { return ChildLayout::template CellIndex<Index>::Height(); }
				};
			};
		}
	}
}
#endif