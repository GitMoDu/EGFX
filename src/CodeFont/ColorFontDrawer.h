#ifndef _EGFX_CODE_FONT_COLOR_FONT_DRAWER_h
#define _EGFX_CODE_FONT_COLOR_FONT_DRAWER_h

#include "AbstractFontDrawer.h"

namespace Egfx
{
	namespace CodeFont
	{
		struct FontColorSourceContract
		{
			static constexpr rgb_color_t GetColor()
			{
				return RGB_COLOR_WHITE;
			}
		};

		template<typename CodeFontType,
			typename FontColorSourceType = FontColorSourceContract>
		class TemplateColorFontDrawer : public AbstractFontDrawer<CodeFontType>
		{
		public:
			using dimension_t = typename CodeFontType::dimension_t;

		private:
			using Base = AbstractFontDrawer<CodeFontType>;

		protected:
			using Base::CodeDrawer;

		public:
			FontColorSourceType FontColor{};

		public:
			TemplateColorFontDrawer(const dimension_t width = 8,
				const dimension_t height = 8,
				const dimension_t kerning = 1)
				: Base(width, height, kerning)
			{
			}

		protected:
			// Protected API.
			pixel_t Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const char printableChar)
			{
				return CodeDrawer.Draw(framebuffer, FontColor.GetColor(), x, y, printableChar);
			}

			//// Protected API, get only character advance.
			pixel_t Advance(const char printableChar) const
			{
				return CodeDrawer.Advance(printableChar);
			}
		};
	}
}
#endif