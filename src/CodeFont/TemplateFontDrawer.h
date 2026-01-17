#ifndef _EGFX_CODE_FONT_TEMPLATE_FONT_DRAWER_h
#define _EGFX_CODE_FONT_TEMPLATE_FONT_DRAWER_h

#include "Model.h"

namespace Egfx
{
	namespace CodeFont
	{
		template<typename CodeFontType>
		class TemplateFontDrawer : public CodeFontType
		{
		public:
			using dimension_t = typename CodeFontType::dimension_t;

		private:
			dimension_t TabWidth = 5;
			dimension_t SpaceWidth = 3;
			dimension_t KerningWidth;

		public:
			TemplateFontDrawer(const dimension_t width = 8,
				const dimension_t height = 8,
				const dimension_t kerning = 1)
				: CodeFontType(width, height)
				, KerningWidth(kerning)
			{
			}

			void SetFontDimensions(const dimension_t width, const dimension_t height)
			{
				CodeFontType::SetFontWidth(width);
				CodeFontType::SetFontHeight(height);
			}

			void SetFontDimensions(const dimension_t width, const dimension_t height, const dimension_t kerning)
			{
				SetFontDimensions(width, height);
				KerningWidth = kerning;
			}

			void SetFontWidth(const dimension_t width)
			{
				CodeFontType::SetFontWidth(width);
			}

			void SetFontHeight(const dimension_t height)
			{
				CodeFontType::SetFontHeight(height);
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
		};
	}
}
#endif