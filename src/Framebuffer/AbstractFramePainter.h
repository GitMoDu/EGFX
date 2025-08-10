#ifndef _EGFX_ABSTRACT_FRAME_PAINTER_h
#define _EGFX_ABSTRACT_FRAME_PAINTER_h

#include "../Model/ColorConverter.h"
#include "../Model/IFrameBuffer.h"
#include <IntegerSignal.h>

namespace Egfx
{
	template<typename ColorConverter, pixel_t frameWidth, pixel_t frameHeight>
	class AbstractFramePainter: public IFrameBuffer
	{
	public:
		static constexpr size_t BufferSize = ColorConverter::BufferSize(frameWidth, frameHeight);
		static constexpr pixel_t FrameWidth = frameWidth;
		static constexpr pixel_t FrameHeight = frameHeight;

		using color_t = typename ColorConverter::color_t;

	protected:
		uint8_t* Buffer;

	public:
		AbstractFramePainter(uint8_t* buffer = nullptr)
			: IFrameBuffer()
			, Buffer(buffer)
		{
		}

		bool IsMonochrome() const final
		{
			return ColorConverter::Monochrome();
		}

		uint8_t GetColorDepth() const final
		{
			return ColorConverter::ColorDepth();
		}

		static constexpr color_t GetRawColor(const rgb_color_t color)
		{
			return ColorConverter::GetRawColor(color);
		}
	};
}
#endif