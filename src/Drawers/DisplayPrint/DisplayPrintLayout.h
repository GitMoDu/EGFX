// DisplayPrintLayout.h

#ifndef _DISPLAY_PRINT_LAYOUT_h
#define _DISPLAY_PRINT_LAYOUT_h

#include <ArduinoGraphicsDrawer.h>
#include "CharacterBuffer.h"

namespace Egfx
{
	template<uint8_t x, uint8_t y,
		uint8_t width, uint8_t height,
		typename FontRendererType,
		uint8_t maxLineCount = UINT8_MAX,
		uint8_t maxCharacterCount = UINT8_MAX>
	struct DisplayPrintLayout : public FontLayoutElement<x, y, width, height, FontRendererType::FontWidth(), FontRendererType::FontHeight(), FontRendererType::FontKerning()>
	{
	private:
		static constexpr uint8_t SmallestUint8(const uint8_t a, const uint8_t b)
		{
			return (a * (a <= b)) | (b * (a > b));
		}

		static constexpr uint8_t LinesCount(const uint8_t fontHeight)
		{
			return SmallestUint8(((height - (fontHeight + 1)) / (fontHeight + 1)), maxLineCount);
		}

		static constexpr uint8_t CharactersCount(const uint8_t fontWidth)
		{
			return SmallestUint8(width / (fontWidth + 1), maxCharacterCount);
		}

	public:
		static constexpr uint8_t CharacterCount()
		{
			return CharactersCount(FontRendererType::FontWidth());
		}

		static constexpr uint8_t LineCount()
		{
			return LinesCount(FontRendererType::FontHeight());
		}

		static constexpr uint8_t ElementsCount()
		{
			return 1 + LineCount();
		}
	};
}
#endif