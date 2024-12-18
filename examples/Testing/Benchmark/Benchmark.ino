
#define SERIAL_BAUD_RATE 115200

#include <ArduinoGraphicsCore.h>

#include "Benchmark.h"

#include "PrimitiveBenchmark.h"
#include "SpriteBenchmark.h"
#include "SpriteEffectBenchmark.h"
#include "FontBenchmark.h"


// Uncomment framebuffer type. Only one framebuffer type benchmark at a time.
//using FrameBufferType = BinaryFrameBuffer<Benchmark::MockScreen::Width, Benchmark::MockScreen::Height>;
//using FrameBufferType = Color8FrameBuffer<Benchmark::MockScreen::Width, Benchmark::MockScreen::Height>;
//using FrameBufferType = Color16FrameBuffer<Benchmark::MockScreen::Width, Benchmark::MockScreen::Height>;

// In-memory frame-buffer.
uint8_t Buffer[FrameBufferType::BufferSize]{};
FrameBufferType FrameBuffer(Buffer);

// Primitive Benchmarks.
PixelBenchmark Pixels{};
LineBenchmark Lines{};
RectangleBenchmark Rectangles{};
RectangleFillBenchmark RectangleFills{};

// Sprite Benchmarks.
BitMaskBenchmark BitMasks{};
BitMapBenchmark BitMaps{};

// Sprite Effects Benchmaks.
SpriteShaderBenchmark SpriteShaders{};
SpriteTransformBenchmark SpriteTransforms{};

// Font Benchmarks.
SpriteFontBenchmark SpriteFonts{};
TextRendererBenchmark TextRenderers{};


void halt()
{
#if defined(USE_DYNAMIC_FRAME_BUFFER)
	delete[] Buffer;
#endif
	Serial.println(F("Screen Setup Failed."));
	while (true)
		;
}

void setup()
{
	Serial.begin(SERIAL_BAUD_RATE);
	while (!Serial)
		;
	delay(1000);

	Serial.print(F("Benchmark v"));
	Serial.print(Benchmark::Version);

	Serial.print(' ');
	Serial.print(Benchmark::MockScreen::Width);
	Serial.print('x');
	Serial.print(Benchmark::MockScreen::Height);
	Serial.println(F(" Mock Screen"));

	PrintPlaform();

	Serial.print(FrameBuffer.GetColorDepth());
	Serial.println(F(" Bit Color Frame Buffer"));


	Serial.println(F("\tPrimitives"));
	const uint32_t primitiveScore = BenchmarkPrimitives();
	Serial.println();

	Serial.println(F("\tSprites"));
	const uint32_t spriteScore = BenchmarkSprites();
	Serial.println();

	Serial.println(F("\tSprite Effects"));
	const uint32_t effectScore = BenchmarkSpriteEffects();
	Serial.println();

	Serial.println(F("\tFont Renderers"));
	const uint32_t fontScore = BenchmarkFonts();
	Serial.println();

	const uint32_t score = Benchmark::GetWeighedScore(primitiveScore, spriteScore, effectScore, fontScore);

	Serial.print(F("Weighed Score\t\t"));
	Serial.println(score);
	Serial.print(F("\tPrimitives\t"));
	Serial.println(primitiveScore);
	Serial.print(F("\tSprites\t\t"));
	Serial.println(spriteScore);
	Serial.print(F("\tEffects\t\t"));
	Serial.println(effectScore);
	Serial.print(F("\tFonts\t\t"));
	Serial.println(fontScore);
	Serial.println();
}

void loop()
{
}

const uint32_t BenchmarkPrimitives()
{
	uint32_t total = 0;
	uint32_t score = 0;
	Serial.print(F("\tPixel"));
	score = BenchmarkDrawer(&Pixels);
	total += score;
	Serial.print('\t');
	Serial.print('\t');
	Serial.println(score);

	Serial.print(F("\tLine"));
	score = BenchmarkDrawer(&Lines);
	total += score;
	Serial.print('\t');
	Serial.print('\t');
	Serial.println(score);

	Serial.print(F("\tRectangle\t"));
	score = BenchmarkDrawer(&Rectangles);
	total += score;
	Serial.println(score);
	Serial.print(F("\tRectangle fill\t"));
	score = BenchmarkDrawer(&RectangleFills);
	Serial.println(score);
	total += score;

	return total;
}

const uint32_t BenchmarkSprites()
{
	uint32_t total = 0;
	uint32_t score = 0;
	Serial.print(F("\tBitMask"));
	score = BenchmarkDrawer(&BitMasks);
	total += score;
	Serial.print('\t');
	Serial.print('\t');
	Serial.println(score);

	Serial.print(F("\tBitMap"));
	score = BenchmarkDrawer(&BitMaps);
	total += score;
	Serial.print('\t');
	Serial.print('\t');
	Serial.println(score);

	return total;
}

const uint32_t BenchmarkSpriteEffects()
{
	uint32_t total = 0;
	uint32_t score = 0;
	Serial.print(F("\tShaders"));
	score = BenchmarkDrawer(&SpriteShaders);
	total += score;
	Serial.print('\t');
	Serial.print('\t');
	Serial.println(score);

	Serial.print(F("\tTransforms"));
	score = BenchmarkDrawer(&SpriteTransforms);
	total += score;
	Serial.print('\t');
	Serial.println(score);

	return total;
}

const uint32_t BenchmarkFonts()
{
	uint32_t total = 0;
	uint32_t score = 0;
	Serial.print(F("\tSprite Font"));
	score = BenchmarkDrawer(&SpriteFonts);
	total += score;
	Serial.print('\t');
	Serial.println(score);

	Serial.print(F("\tText Renderer"));
	score = BenchmarkDrawer(&TextRenderers);
	total += score;
	Serial.print('\t');
	Serial.println(score);

	return total;
}

const uint32_t BenchmarkDrawer(IFrameDraw* drawer)
{
	uint32_t count = 0;
	uint32_t start = millis();
	while (millis() - start < Benchmark::LoopDuration)
	{
		drawer->DrawCall(&FrameBuffer, start, (uint16_t)count);
		count++;
	}

	return count;
}

void PrintPlaform()
{
#if defined(ARDUINO_ARCH_AVR)
	Serial.print(F("AVR"));
#elif defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32)
	Serial.print(F("STM32 F1"));
#elif defined(ARDUINO_ARCH_STM32F4)
	Serial.print(F("STM32 F4"));
#elif defined(ARDUINO_ARCH_RP2040)
#if defined(PICO_RP2350)
	Serial.print(F("RP2350"));
#else
	Serial.print(F("RP2040"));
#endif
#elif defined(ARDUINO_ARCH_NRF52)
	Serial.print(F("NRF52840"));
#elif defined(ARDUINO_ARCH_ESP32)
	Serial.print(F("ESP32"));
#endif

	Serial.print(F(" @ "));
#if defined(ARDUINO_ARCH_STM32F4)
	Serial.print(CLOCK_SPEED_MHZ);
#else
	Serial.print(F_CPU / 1000000);
#endif
	Serial.println(F(" MHz"));
}
