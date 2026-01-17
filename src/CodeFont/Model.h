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
			dimension_t Width;
			dimension_t Height;
			dimension_t KerningWidth = 1;

		public:
			AbstractCodeFont(const dimension_t width, const dimension_t height, const dimension_t kerningWidth)
				: Width(width)
				, Height(height)
				, KerningWidth(kerningWidth)
			{
			}

			// Get only character advance.
			virtual dimension_t Advance(const char /*printableChar*/) const
			{
				// Default to monospaced font.
				return Width;
			}

			// Default implementation.
			virtual dimension_t GetSpaceWidth() const
			{
				return Width;
			}

			// Default implementation.
			virtual dimension_t GetTabWidth() const
			{
				return Width * 3; // 3 spaces per tab.
			}

			/// <summary>
			/// Sets the font width and updates the internal horizontal bounds and midpoint.
			/// </summary>
			virtual void SetFontWidth(const dimension_t width)
			{
				Width = width;
			}

			/// <summary>
			/// Sets the font height and recalculates internal vertical positioning values.
			/// </summary>
			virtual void SetFontHeight(const dimension_t height)
			{
				Height = height;
			}

			/// <summary>
			/// Gets the width of the font.
			/// </summary>
			dimension_t GetFontWidth() const
			{
				return Width;
			}

			/// <summary>
			/// Gets the pixel height of the font.
			/// </summary>
			dimension_t GetFontHeight() const
			{
				return Height;
			}

			dimension_t GetKerningWidth() const
			{
				return KerningWidth;
			}

		};
	}
}
#endif