#ifndef _EGFX_CODE_FONT_ABSTRACT_FONT_DRAWER_h
#define _EGFX_CODE_FONT_ABSTRACT_FONT_DRAWER_h

#include "Model.h"

namespace Egfx
{
	namespace CodeFont
	{
		template<typename CodeFontType>
		class AbstractFontDrawer
		{
		public:
			using dimension_t = typename CodeFontType::dimension_t;

		protected:
			CodeFontType CodeDrawer;

		private:
			dimension_t TabWidth = 5;
			dimension_t SpaceWidth = 3;
			dimension_t KerningWidth;

		public:
			AbstractFontDrawer(const dimension_t width = 8,
				const dimension_t height = 8,
				const dimension_t kerning = 1)
				: CodeDrawer(width, height)
				, KerningWidth(kerning)
			{
			}

			void SetFontDimensions(const dimension_t width, const dimension_t height)
			{
				CodeDrawer.SetFontWidth(width);
				CodeDrawer.SetFontHeight(height);
			}

			void SetFontDimensions(const dimension_t width, const dimension_t height, const dimension_t kerning)
			{
				SetFontDimensions(width, height);
				KerningWidth = kerning;
			}

			void SetFontWidth(const dimension_t width)
			{
				CodeDrawer.SetFontWidth(width);
			}

			void SetFontHeight(const dimension_t height)
			{
				CodeDrawer.SetFontHeight(height);
			}

			void SetKerningWidth(const dimension_t kerning)
			{
				KerningWidth = kerning;
			}

			void SetSpaceWidth(const dimension_t spaceWidth)
			{
				SpaceWidth = spaceWidth;
			}

			void SetTabWidth(const dimension_t tabWidth)
			{
				TabWidth = tabWidth;
			}

			/// <summary>
			/// Gets the pixel width of a space character.
			/// </summary>
			dimension_t GetSpaceWidth() const
			{
				return SpaceWidth;
			}

			/// <summary>
			/// Gets the pixel width of a tab character.
			/// </summary>
			dimension_t GetTabWidth() const
			{
				return TabWidth;
			}

			/// <summary>
			/// Gets the kerning pixel width.
			/// </summary>
			dimension_t GetKerningWidth() const
			{
				return KerningWidth;
			}

			/// <summary>
			/// Gets the pixel width of the font.
			/// </summary>
			dimension_t GetFontWidth() const
			{
				return CodeDrawer.GetFontWidth();
			}

			/// <summary>
			/// Gets the pixel height of the font.
			/// </summary>
			dimension_t GetFontHeight() const
			{
				return CodeDrawer.GetFontHeight();
			}
		};
	}
}
#endif