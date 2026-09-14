#ifndef _EGFX_FRAMEWORK_LAYOUT_GRID_h
#define _EGFX_FRAMEWORK_LAYOUT_GRID_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Layout
		{
			/// <summary>
			/// Splits a parent layout into a grid of equally sized cells.
			///
			/// Behavior:
			/// - Cell boundaries are computed from proportional integer division.
			/// - Remainder pixels are distributed across the later cells.
			/// - The grid area is top-left anchored to the parent (X/Y match the parent).
			/// </summary>
			/// <typeparam name="ParentLayout">Parent layout providing X/Y/Width/Height.</typeparam>
			/// <typeparam name="Columns">Number of columns (must be > 0).</typeparam>
			/// <typeparam name="Rows">Number of rows (must be > 0).</typeparam>
			template<typename ParentLayout,
				uint8_t Columns,
				uint8_t Rows
			>
			struct Grid
			{
				static_assert(Rows > 0, "GridLayout Rows must be > 0.");
				static_assert(Columns > 0, "GridLayout Columns must be > 0.");

				/// <summary>Width of the first grid cell (integer division).</summary>
				static constexpr int16_t CellWidth() { return ParentLayout::Width() / Columns; }

				/// <summary>Height of the first grid cell (integer division).</summary>
				static constexpr int16_t CellHeight() { return ParentLayout::Height() / Rows; }

				/// <summary>Total width of the grid area.</summary>
				static constexpr int16_t GridWidth() { return ParentLayout::Width(); }

				/// <summary>Total height of the grid area.</summary>
				static constexpr int16_t GridHeight() { return ParentLayout::Height(); }

				/// <summary>Top-left anchor of the grid area.</summary>
				static constexpr int16_t X() { return ParentLayout::X(); }
				static constexpr int16_t Y() { return ParentLayout::Y(); }

				/// <summary>Size of the grid area (may be smaller than parent due to truncation).</summary>
				static constexpr int16_t Width() { return GridWidth(); }
				static constexpr int16_t Height() { return GridHeight(); }

				/// <summary>Grid cell addressed by column and row indices.</summary>
				/// <typeparam name="ColIndex">Zero-based column index.</typeparam>
				/// <typeparam name="RowIndex">Zero-based row index.</typeparam>
				template<uint8_t ColIndex, uint8_t RowIndex>
				struct Cell
				{
					static_assert(RowIndex < Rows, "GridLayout Cell RowIndex out of range.");
					static_assert(ColIndex < Columns, "GridLayout Cell ColIndex out of range.");

					static constexpr int16_t X() { return Grid::X() + (ParentLayout::Width() * ColIndex) / Columns; }
					static constexpr int16_t Y() { return Grid::Y() + (ParentLayout::Height() * RowIndex) / Rows; }
					static constexpr int16_t Width() { return (ParentLayout::Width() * (ColIndex + 1)) / Columns - (ParentLayout::Width() * ColIndex) / Columns; }
					static constexpr int16_t Height() { return (ParentLayout::Height() * (RowIndex + 1)) / Rows - (ParentLayout::Height() * RowIndex) / Rows; }
				};
			};
		}
	}
}
#endif