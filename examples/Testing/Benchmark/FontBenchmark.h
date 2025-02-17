// FontBenchmark.h

#ifndef _FONTBENCHMARK_h
#define _FONTBENCHMARK_h

#include <ArduinoGraphicsDrawer.h>

class SpriteFontBenchmark : public IFrameDraw
{
private:
	SpriteShader::ColorShader<SpriteFont5x5Renderer> Renderer{};

public:
	SpriteFontBenchmark()
		: IFrameDraw()
	{
		Renderer.SetColor(Rgb::Color(0x00, 0xFF, 0xFF));
	}

	virtual const bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
	{
		for (uint_fast8_t i = 0; i < 4; i++)
		{
			Renderer.TextTopLeft(frame, 0, 0, F("DOGE"));
		}

		return true;
	}
};

class TextRendererBenchmark : public IFrameDraw
{
private:
	FontStyle Font{};

public:
	TextRendererBenchmark()
		: IFrameDraw()
	{
		Font.Width = SpriteFont5x5Renderer::FontWidth();
		Font.Height = SpriteFont5x5Renderer::FontHeight();
		Font.Color = Rgb::Color((uint32_t)0xff00ff);;
	}

	virtual const bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
	{
		for (uint_fast8_t i = 0; i < 16; i++)
		{
			TextRenderer::TextTopLeft(frame, Font, 0, 0, Assets::Texts::TestText);
		}

		return true;
	}
};

#endif
