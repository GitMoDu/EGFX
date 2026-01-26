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
			/// - Cell size is computed using integer division:
			///   - CellWidth  = ParentWidth / Columns
			///   - CellHeight = ParentHeight / Rows
			/// - The grid area size is the product of cell size and counts:
			///   - GridWidth  = CellWidth  * Columns
			///   - GridHeight = CellHeight * Rows
			/// - Any remainder pixels (from division) are not redistributed; the grid area size is
			/// therefore GridWidth <= ParentWidth and GridHeight <= ParentHeight.
			///
			/// Positioning:
			/// - The grid area is top-left anchored to the parent (X/Y match the parent).
			///
			/// Cell access:
			/// - Use Cell<ColIndex, RowIndex> for 2D coordinates (zero-based).
			/// </summary>
			/// <typeparam name="ParentLayout">The layout type of the parent container.</typeparam>
			/// <typeparam name="Columns">Number of columns in the grid (must be > 0).</typeparam>
			/// <typeparam name="Rows">Number of rows in the grid (must be > 0).</typeparam>
			template<typename ParentLayout,
				uint8_t Columns,
				uint8_t Rows
			>
			struct Grid
			{
				static_assert(Rows > 0, "GridLayout Rows must be > 0.");
				static_assert(Columns > 0, "GridLayout Columns must be > 0.");

				/// <summary>Width of an individual cell (integer division).</summary>
				static constexpr pixel_t CellWidth() { return ParentLayout::Width() / Columns; }

				/// <summary>Height of an individual cell (integer division).</summary>
				static constexpr pixel_t CellHeight() { return ParentLayout::Height() / Rows; }

				/// <summary>Total width of the grid area (<= parent width).</summary>
				static constexpr pixel_t GridWidth() { return CellWidth() * Columns; }

				/// <summary>Total height of the grid area (<= parent height).</summary>
				static constexpr pixel_t GridHeight() { return CellHeight() * Rows; }

				/// <summary>Top-left anchor of the grid area.</summary>
				static constexpr pixel_t X() { return ParentLayout::X(); }
				static constexpr pixel_t Y() { return ParentLayout::Y(); }

				/// <summary>Size of the grid area (may be smaller than parent due to truncation).</summary>
				static constexpr pixel_t Width() { return GridWidth(); }
				static constexpr pixel_t Height() { return GridHeight(); }

				/// <summary>
				/// Grid cell addressed by column/row indices.
				/// Indices are zero-based.
				/// </summary>
				/// <typeparam name="ColIndex">Column index in range [0, Columns).</typeparam>
				/// <typeparam name="RowIndex">Row index in range [0, Rows).</typeparam>
				template<uint8_t ColIndex, uint8_t RowIndex>
				struct Cell
				{
					static_assert(RowIndex < Rows, "GridLayout Cell RowIndex out of range.");
					static_assert(ColIndex < Columns, "GridLayout Cell ColIndex out of range.");

					static constexpr pixel_t X() { return Grid::X() + (pixel_t(CellWidth()) * ColIndex); }
					static constexpr pixel_t Y() { return Grid::Y() + (pixel_t(CellHeight()) * RowIndex); }
					static constexpr pixel_t Width() { return CellWidth(); }
					static constexpr pixel_t Height() { return CellHeight(); }
				};
			};
		}
	}
}
#endif