#ifndef _EGFX_CODE_FONT_h
#define _EGFX_CODE_FONT_h

#include "../Model/IFrameBuffer.h"
#include "../Text/FontText.h"

namespace Egfx
{
	namespace CodeFont
	{
		/// <summary>
		/// Abstract base class for code-based fonts that uses pre-calculated bounds and midpoints for fast character rendering.
		/// TODO: Add code raster indirection for optional custom vector shader.
		/// </summary>
		/// <typeparam name="dimension_t">The numeric type used for font dimensions and coordinate calculations.</typeparam>
		template<typename dimension_t>
		class AbstractCodeFont
		{
		protected:
			// Code fonts uses pre-calculated end and middle points for drawing.
			dimension_t EndX;
			dimension_t EndY;
			dimension_t MiddleX;
			dimension_t MiddleY;

		public:
			AbstractCodeFont(const dimension_t fontWidth, const dimension_t fontHeight)
				: EndX(fontWidth > 1 ? fontWidth - 1 : 1)
				, EndY(fontHeight > 1 ? fontHeight - 1 : 1)
				, MiddleX((EndX + 1) / 2)
				, MiddleY((EndY + 1) / 2)
			{
			}

			/// <summary>
			/// Sets the font width and updates the internal horizontal bounds and midpoint.
			/// </summary>
			void SetFontWidth(const dimension_t width)
			{
				EndX = width > 1 ? width - 1 : 1;
				MiddleX = (EndX + 1) / 2;
			}

			/// <summary>
			/// Sets the font height and recalculates internal vertical positioning values.
			/// </summary>
			void SetFontHeight(const dimension_t height)
			{
				EndY = height > 1 ? height - 1 : 1;
				MiddleY = (EndY + 1) / 2;
			}

			/// <summary>
			/// Gets the width of the font.
			/// </summary>
			dimension_t GetFontWidth() const
			{
				return EndX + 1;
			}

			/// <summary>
			/// Gets the pixel height of the font.
			/// </summary>
			dimension_t GetFontHeight() const
			{
				return EndY + 1;
			}
		};
	}
}
#endif