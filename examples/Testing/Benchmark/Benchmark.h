// Benchmark.h

#ifndef _BENCHMARK_h
#define _BENCHMARK_h

#include <stdint.h>

namespace Benchmark
{
	static constexpr uint8_t Version = 0;


	static constexpr uint32_t LoopDuration = 1000;

	enum class ScoreWeights : uint8_t
	{
		Primitives = 1,
		Sprites = 3,
		SpriteEffects = 2,
		Fonts = 3,
		WeightSum = (uint8_t)Primitives + (uint8_t)Sprites + (uint8_t)SpriteEffects + (uint8_t)Fonts
	};

	/// <summary>
	/// Small dimensions frame buffer, keeps compatibility (i.e. RAM) with AVR
	/// while still being a reasonable size to benchmark fills and sprites.
	/// </summary>
	struct MockScreen
	{
#if defined(ARDUINO_ARCH_AVR)
		static const uint8_t Width = 32;
		static const uint8_t Height = 16;
#else
		static const uint8_t Width = 64;
		static const uint8_t Height = 32;
#endif
	};

	/// <summary>
	/// Calculates weight average of scores. 
	/// Scores are only comparable for the same platform and FrameBuffer type.
	/// </summary>
	/// <param name="primitiveScore"></param>
	/// <param name="spriteScore"></param>
	/// <param name="effectScore"></param>
	/// <param name="fontScore"></param>
	/// <returns></returns>
	static constexpr uint32_t GetWeighedScore(const uint32_t primitiveScore,
		const uint32_t spriteScore,
		const uint32_t effectScore,
		const uint32_t fontScore)
	{
		return ((primitiveScore * (uint8_t)ScoreWeights::Primitives)
			+ (spriteScore * (uint8_t)ScoreWeights::Sprites)
			+ (effectScore * (uint8_t)ScoreWeights::SpriteEffects)
			+ (fontScore * (uint8_t)ScoreWeights::Fonts)) / (uint8_t)ScoreWeights::WeightSum;
	}
}

#endif
