#ifndef _EGFX_FONT_TEXT_h
#define _EGFX_FONT_TEXT_h

#include "../Platform/Platform.h"
#include "../Model/AsciiDefinition.h"

namespace Egfx
{
	namespace FontText
	{
		enum class TextAlignmentEnum : uint8_t
		{
			Left,
			Center,
			Right
		};

		static constexpr uint32_t PowerOfTen[]
		{
			1,
			10,
			100,
			1000,
			10000,
			100000,
			1000000,
			10000000,
			100000000,
			1000000000
		};

		struct DefaultLayout
		{
			static constexpr pixel_t X() { return 0; }
			static constexpr pixel_t Y() { return 0; }
			static constexpr pixel_t Width() { return 1; }
			static constexpr pixel_t Height() { return 1; }
		};

		
		//struct FullPixelShader
		//{
		//	static constexpr rgb_color_t Shade(const char /*printableChar*/)
		//	{
		//		return RGB_COLOR_WHITE;
		//	}
		//};

		//struct GridTwoColorPixelShader
		//{
		//	pixel_t X = 0;
		//	pixel_t Y = 0;
		//	rgb_color_t Color1 = RGB_COLOR_WHITE;
		//	rgb_color_t Color2 = RGB_COLOR_BLACK;

		//	rgb_color_t Shade(const pixel_t x, const pixel_t y) const
		//	{
		//		const bool isEven = ((x + y) & 0b1) == 0;
		//		return isEven ? Color1 : Color2;
		//	}
		//};

		//struct NoPixelShader
		//{
		//	pixel_t X = 0;
		//	pixel_t Y = 0;

		//	static constexpr rgb_color_t Shade(const pixel_t /*x*/, const pixel_t /*y*/)
		//	{
		//		return RGB_COLOR_WHITE;
		//	}
		//};
	}
}
#endif