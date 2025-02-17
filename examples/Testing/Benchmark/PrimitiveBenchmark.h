// PrimitiveBenchmark.h

#ifndef _PRIMITIVE_BENCHMARK_h
#define _PRIMITIVE_BENCHMARK_h

#include <ArduinoGraphicsDrawer.h>

class PixelBenchmark : public IFrameDraw
{
private:
	rgb_color_t Color = Rgb::Color(UINT8_MAX, UINT8_MAX, UINT8_MAX);

public:
	PixelBenchmark() :IFrameDraw() {}

	virtual const bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
	{
		const uint8_t width = frame->GetFrameWidth();
		const uint8_t height = frame->GetFrameHeight();

		for (uint_fast8_t i = 0; i < 128; i++)
		{
			frame->Pixel(Color, 0, 0);
			frame->Pixel(Color, width - 1, height - 1);
			frame->Pixel(Color, width - 1, 0);
			frame->Pixel(Color, 0, height - 1);
		}

		return true;
	}
};

class LineBenchmark : public IFrameDraw
{
private:
	rgb_color_t Color = Rgb::Color(UINT32_MAX);

public:
	LineBenchmark() :IFrameDraw() {}

	virtual const bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
	{
		const uint8_t width = frame->GetFrameWidth();
		const uint8_t height = frame->GetFrameHeight();

		for (uint_fast8_t i = 0; i < 16; i++)
		{
			frame->Line(Color, 0, 0, width - 1, 0);
			frame->Line(Color, 0, 0, 0, height - 1);
			frame->Line(Color, 0, 0, width - 1, height - 1);
		}
		return true;
	}
};

class RectangleBenchmark : public IFrameDraw
{
private:
	rgb_color_t Color = Rgb::Color(UINT32_MAX);

public:
	RectangleBenchmark() :IFrameDraw() {}

	virtual const bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
	{
		const uint8_t width = frame->GetFrameWidth();
		const uint8_t height = frame->GetFrameHeight();

		for (uint_fast8_t i = 0; i < 8; i++)
		{
			frame->Rectangle(Color, i, i, width - 1, height - 1);
		}

		return true;
	}
};

class RectangleFillBenchmark : public IFrameDraw
{
private:
	rgb_color_t Color = Rgb::Color(UINT32_MAX);

public:
	RectangleFillBenchmark() :IFrameDraw() {}

	virtual const bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
	{
		const uint8_t width = frame->GetFrameWidth();
		const uint8_t height = frame->GetFrameHeight();
		frame->RectangleFill(Color, 0, 0, width - 1, height - 1);

		return true;
	}
};
#endif

