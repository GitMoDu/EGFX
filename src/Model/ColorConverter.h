#ifndef _COLOR_CONVERTER_h
#define _COLOR_CONVERTER_h

#include "RgbColor.h"
#include "GraphicsBuffer.h"

namespace Egfx
{
	struct AbstractColorConverter1
	{
		using color_t = uint8_t;

		static constexpr uint8_t ColorDepth()
		{
			return 1;
		}

		static constexpr size_t BufferSize(const uint16_t width, const uint16_t height)
		{
			return Egfx::GetFrameBufferMonochromeSize(width, height);
		}

		static constexpr bool Monochrome()
		{
			return true;
		}
	};

	struct AbstractColorConverter4
	{
		using color_t = uint8_t;

		static constexpr size_t BufferSize(const uint16_t width, const uint16_t height)
		{
			return Egfx::GetFrameBufferLowColorSize<4>(width, height);
		}

		static constexpr uint8_t ColorDepth()
		{
			return 4;
		}

		static constexpr bool Monochrome()
		{
			return true;
		}
	};

	struct AbstractColorConverter8
	{
		using color_t = uint8_t;

		static constexpr uint8_t ColorDepth()
		{
			return 8;
		}

		static constexpr size_t BufferSize(const uint16_t width, const uint16_t height)
		{
			return Egfx::GetFrameBufferSize<color_t>(width, height);
		}
	};

	struct AbstractColorConverter16
	{
		using color_t = uint16_t;

		static constexpr uint8_t ColorDepth()
		{
			return 16;
		}

		static constexpr size_t BufferSize(const uint16_t width, const uint16_t height)
		{
			return Egfx::GetFrameBufferSize<color_t>(width, height);
		}
	};

	struct AbstractColorConverter32
	{
		using color_t = uint32_t;

		static constexpr uint8_t ColorDepth()
		{
			return 24;
		}

		static constexpr size_t BufferSize(const uint16_t width, const uint16_t height)
		{
			return Egfx::GetFrameBufferSize<color_t>(width, height);
		}

		static constexpr bool Monochrome()
		{
			return false;
		}
	};

	/// <summary>
	/// Converter for 8 bit color, 3-3-2 color format.
	/// </summary>
	struct ColorConverter8 : public AbstractColorConverter8
	{
#if defined(EGFX_PLATFORM_HDR)
		/// <summary>
		/// Convert 8-8-8 to 3-3-2 color.
		/// </summary>
		/// <param name="color">EGFX-native color.</param>
		/// <returns>Framebuffer-native color (8-8-8).</returns>
		static constexpr color_t GetRawColor(const rgb_color_t color)
		{
			return ((Rgb::R(color) >> 5) << 5)
				| ((Rgb::G(color) >> 5) << 2)
				| (Rgb::B(color) >> 6);
		}
#else
		/// <summary>
		/// Convert 5-6-5 to 3-3-2 color.
		/// </summary>
		/// <param name="color"></param>
		/// <returns></returns>
		static constexpr color_t GetRawColor(const rgb_color_t color)
		{
			return ((Rgb::R5(color) >> 2) << 5)
				| ((Rgb::G6(color) >> 3) << 2)
				| (Rgb::B5(color) >> 3);
		}
#endif

		static constexpr bool Monochrome()
		{
			return false;
		}
	};

	/// <summary>
	/// Converter for 16 bit color, 5-6-5 color format.
	/// </summary>
	struct ColorConverter16 : public AbstractColorConverter16
	{
		static constexpr bool Monochrome()
		{
			return false;
		}

#if defined(EGFX_PLATFORM_HDR)
		/// <summary>
		/// Convert 8-8-8 color to 5-6-5 color.
		/// </summary>
		/// <param name="color">EGFX-native color (8-8-8).</param>
		/// <returns>Framebuffer-native color (5-6-5).</returns>
		static constexpr color_t GetRawColor(const rgb_color_t color)
		{
			return Rgb::Color565(color);
		}
#else
		/// <summary>
		/// Passthrough 5-6-5 color.
		/// </summary>
		/// <param name="color">EGFX-native color (5-6-5).</param>
		/// <returns>Framebuffer-native color (5-6-5).</returns>
		static constexpr color_t GetRawColor(const rgb_color_t color)
		{
			return color;
		}
#endif
	};

	/// <summary>
	/// Converter for 32 bit color, 8-8-8 color format.
	/// </summary>
	struct ColorConverter32 : public AbstractColorConverter32
	{
#if defined(EGFX_PLATFORM_HDR)
		/// <summary>
		/// Passthrough 8-8-8 color.
		/// </summary>
		/// <param name="color">EGFX-native color (8-8-8).</param>
		/// <returns>Framebuffer-native color (8-8-8).</returns>
		static constexpr color_t GetRawColor(const rgb_color_t color)
		{
			return color;
		}

#else
		/// <summary>
		/// Convert 5-6-5 color to 8-8-8 color.
		/// </summary>
		/// <param name="color">EGFX-native color (5-6-5).</param>
		/// <returns>Framebuffer-native color (8-8-8).</returns>
		static constexpr color_t GetRawColor(const rgb_color_t color)
		{
			return Rgb::Color(Rgb::R(color), Rgb::G(color), Rgb::B(color));
		}
#endif
	};

	/// <summary>
	/// Converter for 8 bit grayscale.
	/// </summary>
	struct GrayScaleConverter8 : public AbstractColorConverter8
	{
#if defined(EGFX_PLATFORM_HDR)
	private:
		enum class Weights : uint8_t
		{
			R = 77,
			G = 150,
			B = 29
		};

		static constexpr uint16_t WeightsSum = (uint16_t)Weights::R + (uint16_t)Weights::G + (uint16_t)Weights::B;

	public:
		/// <summary>
		/// Converts native (8-8-8) color to 256 tones grayscale.
		/// </summary>
		/// <param name="color">EGFX-native color (8-8-8).</param>
		/// <returns>Framebuffer-native color (8).</returns>
		static constexpr color_t GetRawColor(const rgb_color_t color)
		{
			return (((uint32_t)Rgb::R(color) * (uint8_t)Weights::R)
				+ ((uint32_t)Rgb::G(color) * (uint8_t)Weights::G)
				+ ((uint32_t)Rgb::B(color) * (uint8_t)Weights::B)
				) >> IntegerSignal::GetBitShifts(WeightsSum);
		}
#else
		/// <summary>
		/// Converts native (5-6-5) color to 256 tones grayscale.
		/// </summary>
		/// <param name="color">EGFX-native color (5-6-5).</param>
		/// <returns>Framebuffer-native color (8).</returns>
		static constexpr color_t GetRawColor(const rgb_color_t color)
		{
			return (uint16_t)Rgb::B5(color) + Rgb::R5(color) + (Rgb::G6(color) << 2);
		}
#endif
		static constexpr bool Monochrome()
		{
			return true;
		}
	};

	/// <summary>
	/// Converter for 4 bit grayscale.
	/// </summary>
	struct GrayScaleConverter4 : public AbstractColorConverter4
	{
#if defined(EGFX_PLATFORM_HDR)
		/// <summary>
		/// Converts 8-8-8 color to 16 tones grayscale.
		/// </summary>
		/// <param name="color">EGFX-native color (8-8-8).</param>
		/// <returns>Framebuffer-native color (4).</returns>
		static constexpr color_t GetRawColor(const rgb_color_t color)
		{
			return (((uint16_t)Rgb::R(color) * 77) + ((uint16_t)Rgb::G(color) * 150) + ((uint16_t)Rgb::B(color) * 29)) >> 12;
		}
#else
		/// <summary>
		/// Converts 5-6-5 color to 16 tones grayscale.
		/// </summary>
		/// <param name="color">EGFX-native color (5-6-5).</param>
		/// <returns>Framebuffer-native color (4).</returns>
		static constexpr color_t GetRawColor(const rgb_color_t color)
		{
			return ((Rgb::R5(color) + Rgb::G6(color) + Rgb::B5(color)) >> 3) & 0b1111;
		}
#endif
	};

	/// <summary>
	/// Fast Converter for 1 bit color, with templated threshold.
	/// </summary>
	/// <typeparam name="threshold">Grayscale threshold for color.</typeparam>
	template<const uint8_t threshold>
	struct BinaryColorConverter1 : public AbstractColorConverter1
	{
		/// <summary>
		/// Converts native (8-8-8, 5-6-5) color to 1 bit color.
		/// </summary>
		/// <param name="color">EGFX-native color (8-8-8, 5-6-5).</param>
		/// <returns>Framebuffer-native color (1).</returns>
		static constexpr color_t GetRawColor(const rgb_color_t color)
		{
			return Rgb::R(color) > threshold
				|| Rgb::G(color) > threshold
				|| Rgb::B(color) > threshold;
		}

		static constexpr bool Monochrome()
		{
			return true;
		}
	};
}
#endif