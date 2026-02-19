#ifndef _EGFX_TEMPLATE_FRAME_BUFFER_h
#define _EGFX_TEMPLATE_FRAME_BUFFER_h

#include "../Model/RgbColor.h"
#include "../Model/DisplayOptions.h"

#if defined(ARDUINO_ARCH_RP2040)
#include "hardware/dma.h"
#endif

namespace Egfx
{
	/// <summary>
	/// Generic framebuffer template providing 2D drawing, blending, and buffer management for any color depth and display configuration.
	/// Supports mirroring, inversion, anti-aliasing, and partial buffer clearing via template parameters.
	/// Used as the core implementation for all framebuffer types in Egfx.
	/// </summary>
	/// <typeparam name="FramePainter">The base class providing low-level drawing and buffer access functionality.</typeparam>
	/// <typeparam name="clearDivisorPower">An unsigned 8-bit integer specifying the power-of-two divisor for buffer clearing steps.</typeparam>
	/// <typeparam name="displayOptions">Display configuration options (mirror, rotation, inverted colors, AA).</typeparam>
	template<typename FramePainter
		, uint8_t clearDivisorPower
		, typename displayOptions = DisplayOptions::Default>
	class TemplateFramebuffer : public FramePainter
	{
	private:
		enum class TransformCaseType : uint8_t
		{
			Identity,
			MirrorX,
			MirrorY,
			MirrorXY,
			Rotate90,
			Rotate90MirrorX,
			Rotate90MirrorY,
			Rotate90MirrorXY,
			Rotate180,
			Rotate180MirrorX,
			Rotate180MirrorY,
		};

	public:
		using FramePainter::BufferSize;
		using FramePainter::FrameWidth;
		using FramePainter::FrameHeight;

		using typename FramePainter::color_t;

		using Configuration = displayOptions;

	protected:
		using FramePainter::IntToFixed;
		using FramePainter::FixedRoundToInt;
		using FramePainter::ClipRectangle;
		using FramePainter::ClipLine;

#if defined(ARDUINO_ARCH_RP2040)
		// Static fill constant lives for the lifetime of the program; safe as DMA source.
		const uint32_t FillZero32 = 0x00000000u;
		const uint32_t FillOnes32 = 0xFFFFFFFFu;

		// Async DMA clear state
		int DmaChan = -1;
		bool DmaActive = false;
#else
		static constexpr uint8_t ClearStepsCount = uint8_t(1) << clearDivisorPower;
#endif
		static constexpr TransformCaseType TransformCase = // Explicitly map each rotation+mirror combination to a named case. C++11 compatible implementation.
			(displayOptions::Rotation == DisplayOptions::RotationEnum::None) ?
			((displayOptions::Mirror == DisplayOptions::MirrorEnum::None) ? TransformCaseType::Identity :
				(displayOptions::Mirror == DisplayOptions::MirrorEnum::MirrorX) ? TransformCaseType::MirrorX :
				(displayOptions::Mirror == DisplayOptions::MirrorEnum::MirrorY) ? TransformCaseType::MirrorY :
				TransformCaseType::MirrorXY)
			: (displayOptions::Rotation == DisplayOptions::RotationEnum::Rotate90) ?
			((displayOptions::Mirror == DisplayOptions::MirrorEnum::None) ? TransformCaseType::Rotate90 :
				(displayOptions::Mirror == DisplayOptions::MirrorEnum::MirrorX) ? TransformCaseType::Rotate90MirrorX :
				(displayOptions::Mirror == DisplayOptions::MirrorEnum::MirrorY) ? TransformCaseType::Rotate90MirrorY :
				TransformCaseType::Rotate90MirrorXY)
			: (displayOptions::Rotation == DisplayOptions::RotationEnum::Rotate180) ?
			((displayOptions::Mirror == DisplayOptions::MirrorEnum::None ||
				displayOptions::Mirror == DisplayOptions::MirrorEnum::MirrorXY) ? TransformCaseType::Rotate180 :
				(displayOptions::Mirror == DisplayOptions::MirrorEnum::MirrorX) ? TransformCaseType::Rotate180MirrorX :
				TransformCaseType::Rotate180MirrorY)
			: ((displayOptions::Mirror == DisplayOptions::MirrorEnum::None) ? TransformCaseType::Rotate90MirrorXY :
				(displayOptions::Mirror == DisplayOptions::MirrorEnum::MirrorX) ? TransformCaseType::Rotate90MirrorY :
				(displayOptions::Mirror == DisplayOptions::MirrorEnum::MirrorY) ? TransformCaseType::Rotate90MirrorX :
				TransformCaseType::Rotate90);

		static constexpr bool AxisSwapped = (TransformCase == TransformCaseType::Rotate90) ||
			(TransformCase == TransformCaseType::Rotate90MirrorX) ||
			(TransformCase == TransformCaseType::Rotate90MirrorY) ||
			(TransformCase == TransformCaseType::Rotate90MirrorXY);

	protected:
		using FramePainter::Buffer;

	private:
		uint8_t ClearIndex = 0;

	public:
		TemplateFramebuffer(uint8_t buffer[BufferSize] = nullptr)
			: FramePainter(buffer)
		{
		}

	public:
		virtual bool Flip()
		{
			// No double-buffering support by default.
			return false;
		}

		uint8_t* GetFrameBuffer() const
		{
			return Buffer;
		}

		bool ClearFrameBuffer() final
		{
#if defined(ARDUINO_ARCH_RP2040)
			return AsyncClearBuffer();
#else
			// The template keyword is required in this context to disambiguate between a static method and a template method.
			FramePainter::template ClearRaw<displayOptions::Inverted, ClearStepsCount>(ClearIndex);

			ClearIndex++;

			if (ClearIndex >= ClearStepsCount)
			{
				ClearIndex = 0; // Ready for next clear.

				return true;
			}
			else
			{
				return false;
			}
#endif
		}

		virtual void SetBuffer(uint8_t buffer[BufferSize])
		{
			Buffer = buffer;
		}

		inline void Pixel(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			Pixel(color, pixel_point_t{ x, y });
		}

		inline void Pixel(const rgb_color_t color, const pixel_point_t point) final
		{
			if (point.x >= 0 && point.x < FrameWidth &&
				point.y >= 0 && point.y < FrameHeight)
			{
				const pixel_point_t transformed = TransformCoordinates(point);
				FramePainter::PixelRaw(GetRawColor(color), transformed.x, transformed.y);
			}
		}

		void PixelBlend(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			PixelBlend(color, pixel_point_t{ x, y });
		}

		void PixelBlend(const rgb_color_t color, const pixel_point_t point) final
		{
			if (point.x >= 0 && point.x < FrameWidth &&
				point.y >= 0 && point.y < FrameHeight)
			{
				const pixel_point_t transformed = TransformCoordinates(point);
				FramePainter::PixelRawBlend(GetRawColor(color), transformed.x, transformed.y);
			}
		}

		void PixelBlendAlpha(const rgb_color_t color, const pixel_t x, const pixel_t y, const uint8_t alpha) final
		{
			PixelBlendAlpha(color, pixel_point_t{ x, y }, alpha);
		}

		void PixelBlendAlpha(const rgb_color_t color, const pixel_point_t point, const uint8_t alpha) final
		{
			if (alpha == UINT8_MAX / 2)
			{
				PixelBlend(color, point);
			}
			else if (alpha > 0 &&
				point.x >= 0 && point.x < FrameWidth &&
				point.y >= 0 && point.y < FrameHeight)
			{
				// Use the same GetRawColor (which applies inversion correctly for the configured color depth)
				const color_t rawColor = GetRawColor(color);

				const pixel_point_t transformed = TransformCoordinates(point);
				FramePainter::PixelRawBlendAlpha(rawColor, transformed.x, transformed.y, alpha);
			}
		}

		void PixelBlendAdd(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			PixelBlendAdd(color, pixel_point_t{ x, y });
		}

		void PixelBlendAdd(const rgb_color_t color, const pixel_point_t point) final
		{
			if (point.x >= 0 && point.x < FrameWidth &&
				point.y >= 0 && point.y < FrameHeight)
			{
				const pixel_point_t transformed = TransformCoordinates(point);
				FramePainter::PixelRawBlendAdd(GetRawColor(color), transformed.x, transformed.y);
			}
		}

		void PixelBlendSubtract(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			PixelBlendSubtract(color, pixel_point_t{ x, y });
		}

		void PixelBlendSubtract(const rgb_color_t color, const pixel_point_t point) final
		{
			if (point.x >= 0 && point.x < FrameWidth &&
				point.y >= 0 && point.y < FrameHeight)
			{
				const pixel_point_t transformed = TransformCoordinates(point);
				FramePainter::PixelRawBlendSubtract(GetRawColor(color), transformed.x, transformed.y);
			}
		}

		void PixelBlendMultiply(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			PixelBlendMultiply(color, pixel_point_t{ x, y });
		}

		void PixelBlendMultiply(const rgb_color_t color, const pixel_point_t point) final
		{
			if (point.x >= 0 && point.x < FrameWidth &&
				point.y >= 0 && point.y < FrameHeight)
			{
				const pixel_point_t transformed = TransformCoordinates(point);
				FramePainter::PixelRawBlendMultiply(GetRawColor(color), transformed.x, transformed.y);
			}
		}

		void PixelBlendScreen(const rgb_color_t color, const pixel_t x, const pixel_t y) final
		{
			PixelBlendScreen(color, pixel_point_t{ x, y });
		}

		void PixelBlendScreen(const rgb_color_t color, const pixel_point_t point) final
		{
			if (point.x >= 0 && point.x < FrameWidth &&
				point.y >= 0 && point.y < FrameHeight)
			{
				const pixel_point_t transformed = TransformCoordinates(point);
				FramePainter::PixelRawBlendScreen(GetRawColor(color), transformed.x, transformed.y);
			}
		}

		void LineHorizontal(const rgb_color_t color, const pixel_t x1, const pixel_t x2, const pixel_t y) final
		{
			if (y < 0 || y >= FrameHeight)
			{
				return;
			}

			// Normalize endpoints (fx1 <= fx2) for cheap reject/clamp.
			pixel_t fx1 = x1;
			pixel_t fx2 = x2;
			if (fx1 > fx2)
			{
				const pixel_t tmp = fx1;
				fx1 = fx2;
				fx2 = tmp;
			}

			// If the line is completely outside the horizontal bounds of the frame, skip drawing.
			if (fx2 < 0 || fx1 >= FrameWidth)
			{
				return;
			}

			// Clip to framebuffer bounds to keep raw painters in-range.
			fx1 = MaxValue<pixel_t>(fx1, 0);
			fx2 = MinValue<pixel_t>(fx2, FrameWidth - 1);

			const color_t rawColor = GetRawColor(color);

			const pixel_point_t start = TransformCoordinates({ fx1, y });
			const pixel_point_t end = TransformCoordinates({ fx2, y });

			if (AxisSwapped)
			{
				// Horizontal becomes vertical after 90/270-degree rotation.
				FramePainter::LineVerticalRaw(rawColor, start.x, start.y, end.y);
			}
			else
			{
				FramePainter::LineHorizontalRaw(rawColor, start.x, start.y, end.x);
			}
		}

		void LineVertical(const rgb_color_t color, const pixel_t x, const pixel_t y1, const pixel_t y2) final
		{
			if (x < 0 || x >= FrameWidth)
			{
				return;
			}

			// Normalize endpoints (fy1 <= fy2) for cheap reject/clamp.
			pixel_t fy1 = y1;
			pixel_t fy2 = y2;
			if (fy1 > fy2)
			{
				const pixel_t tmp = fy1;
				fy1 = fy2;
				fy2 = tmp;
			}

			// If the line is completely outside the vertical bounds of the frame, skip drawing.
			if (fy2 < 0 || fy1 >= FrameHeight)
			{
				return;
			}

			// Clip to framebuffer bounds to keep raw painters in-range.
			fy1 = MaxValue<pixel_t>(fy1, 0);
			fy2 = MinValue<pixel_t>(fy2, FrameHeight - 1);

			const color_t rawColor = GetRawColor(color);

			const pixel_point_t start = TransformCoordinates({ x, fy1 });
			const pixel_point_t end = TransformCoordinates({ x, fy2 });

			if (AxisSwapped)
			{
				// Vertical becomes horizontal after 90/270-degree rotation.
				FramePainter::LineHorizontalRaw(rawColor, start.x, start.y, end.x);
			}
			else
			{
				FramePainter::LineVerticalRaw(rawColor, start.x, start.y, end.y);
			}
		}

		void RectangleFill(const rgb_color_t color,
			const pixel_t topLeftX, const pixel_t topLeftY,
			const pixel_t bottomRightX, const pixel_t bottomRightY) final
		{
			RectangleFill(color, pixel_rectangle_t{ {topLeftX, topLeftY}, {bottomRightX, bottomRightY} });
		}

		void RectangleFill(const rgb_color_t color, const pixel_rectangle_t& rectangle) final
		{
			pixel_point_t topLeft = { (rectangle.topLeft.x <= rectangle.bottomRight.x) ? rectangle.topLeft.x : rectangle.bottomRight.x,
				(rectangle.topLeft.y <= rectangle.bottomRight.y) ? rectangle.topLeft.y : rectangle.bottomRight.y };
			pixel_point_t bottomRight = { (rectangle.topLeft.x <= rectangle.bottomRight.x) ? rectangle.bottomRight.x : rectangle.topLeft.x,
				(rectangle.topLeft.y <= rectangle.bottomRight.y) ? rectangle.bottomRight.y : rectangle.topLeft.y };

			if (!ClipRectangle(topLeft, bottomRight))
			{
				return;
			}

			const color_t rawColor = GetRawColor(color);

			// Transform all four corners to find actual bounding box after rotation
			pixel_point_t tl = TransformCoordinates(topLeft);
			pixel_point_t tr = TransformCoordinates({ bottomRight.x, topLeft.y });
			pixel_point_t bl = TransformCoordinates({ topLeft.x, bottomRight.y });
			pixel_point_t br = TransformCoordinates(bottomRight);

			// Find min/max across all four corners
			pixel_t minX = MinValue(MinValue(tl.x, tr.x), MinValue(bl.x, br.x));
			pixel_t maxX = MaxValue(MaxValue(tl.x, tr.x), MaxValue(bl.x, br.x));
			pixel_t minY = MinValue(MinValue(tl.y, tr.y), MinValue(bl.y, br.y));
			pixel_t maxY = MaxValue(MaxValue(tl.y, tr.y), MaxValue(bl.y, br.y));

			if (minX == maxX)
			{
				if (minY == maxY)
				{
					FramePainter::PixelRaw(rawColor, minX, minY);
				}
				else
				{
					FramePainter::LineVerticalRaw(rawColor, minX, minY, maxY);
				}
			}
			else if (minY == maxY)
			{
				FramePainter::LineHorizontalRaw(rawColor, minX, minY, maxX);
			}
			else
			{
				FramePainter::RectangleFillRaw(rawColor, minX, minY, maxX, maxY);
			}
		}

		void Fill(const rgb_color_t color) final
		{
			FramePainter::FillRaw(GetRawColor(color));
		}

	private:
		/// <summary>
		/// Draws a diagonal line between two points using Bresenham's algorithm, selecting the optimal direction based on the line's slope.
		/// Implementation based on https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/
		/// </summary>
		/// <param name="rawColor">The color to use when drawing the line.</param>
		/// <param name="start">The starting point of the line.</param>
		/// <param name="end">The ending point of the line.</param>
		void BresenhamDiagonal(const color_t rawColor, const pixel_point_t start, const pixel_point_t end)
		{
			if (end.x > start.x)
			{
				if (end.y > start.y)
				{
					if (end.x - start.x > end.y - start.y)
					{
						BresenhamRight(rawColor, start, end);
					}
					else
					{
						BresenhamUp(rawColor, start, end);
					}
				}
				else if (end.x - start.x > start.y - end.y)
				{
					BresenhamRight(rawColor, start, end);
				}
				else
				{
					BresenhamUp(rawColor, end, start);
				}
			}
			else if (end.y > start.y)
			{
				if (start.x - end.x > end.y - start.y)
				{
					BresenhamRight(rawColor, end, start);
				}
				else
				{
					BresenhamUp(rawColor, start, end);
				}
			}
			else
			{
				if (start.x - end.x > start.y - end.y)
				{
					BresenhamRight(rawColor, end, start);
				}
				else
				{
					BresenhamUp(rawColor, end, start);
				}
			}
		}

		void BresenhamRight(const color_t rawColor, const pixel_point_t start, const pixel_point_t end)
		{
			const pixel_t scaledWidth = (end.x - start.x) << 1;
			const pixel_index_t slopeMagnitude = AbsValue(end.y - start.y) << 1;
			const int8_t slopeUnit = (end.y >= start.y) ? 1 : -1;
			const int8_t slopeSign = (end.x >= start.x) ? 1 : -1;

			pixel_index_t slopeError = slopeMagnitude - (end.x - start.x);
			pixel_t y = start.y;

			for (pixel_t x = start.x; x != end.x; x += slopeSign)
			{
				FramePainter::PixelRaw(rawColor, x, y);

				slopeError += slopeMagnitude;
				if (slopeError >= 0)
				{
					y += slopeUnit;
					slopeError -= scaledWidth;
				}
			}

			// Ensure the final endpoint is drawn.
			FramePainter::PixelRaw(rawColor, end.x, end.y);
		}

		void BresenhamUp(const color_t rawColor, const pixel_point_t start, const pixel_point_t end)
		{
			const pixel_t scaledHeight = (end.y - start.y) << 1;
			const pixel_t slopeMagnitude = AbsValue(end.x - start.x) << 1;
			const int8_t slopeUnit = (end.x >= start.x) ? 1 : -1;
			const int8_t slopeSign = (end.y >= start.y) ? 1 : -1;

			pixel_index_t slopeError = (pixel_index_t)slopeMagnitude - (end.y - start.y);
			pixel_t x = start.x;

			for (pixel_t y = start.y; y != end.y; y += slopeSign)
			{
				FramePainter::PixelRaw(rawColor, x, y);

				slopeError += slopeMagnitude;
				if (slopeError >= 0)
				{
					x += slopeUnit;
					slopeError -= scaledHeight;
				}
			}

			// Ensure the final endpoint is drawn.
			FramePainter::PixelRaw(rawColor, end.x, end.y);
		}

	private:
		static constexpr color_t ColorMask = static_cast<color_t>((uint64_t(1) << FramePainter::ColorDepth) - 1);

		inline constexpr color_t GetRawColor(const rgb_color_t color)
		{
			// Get the inversion aware, native raw color from the underlying frame painter.
			return displayOptions::Inverted ?
				static_cast<color_t>(FramePainter::GetRawColor(rgb_color_t(~color | ColorMask))) : // Mask to the number of valid bits for the color depth and invert only those bits.
				FramePainter::GetRawColor(color); 	// If inversion is not enabled at compile-time, return raw unchanged.
		}

		pixel_point_t TransformCoordinates(const pixel_point_t coordinates) const
		{
			switch (TransformCase)
			{
			case TransformCaseType::Identity:
			default:
				return coordinates;

			case TransformCaseType::MirrorX:
				return { static_cast<pixel_t>(FrameWidth - 1 - coordinates.x),
						 coordinates.y };

			case TransformCaseType::MirrorY:
				return { coordinates.x,
						 static_cast<pixel_t>(FrameHeight - 1 - coordinates.y) };

			case TransformCaseType::MirrorXY:
				return { static_cast<pixel_t>(FrameWidth - 1 - coordinates.x),
						 static_cast<pixel_t>(FrameHeight - 1 - coordinates.y) };

			case TransformCaseType::Rotate90:
				return { static_cast<pixel_t>(FramePainter::PhysicalWidth - 1 - coordinates.y),
						 coordinates.x };

			case TransformCaseType::Rotate90MirrorX:
				return { static_cast<pixel_t>(FramePainter::PhysicalWidth - 1 - coordinates.y),
						 static_cast<pixel_t>(FrameWidth - 1 - coordinates.x) };

			case TransformCaseType::Rotate90MirrorY:
				return { coordinates.y,
						 coordinates.x };

			case TransformCaseType::Rotate90MirrorXY:
				return { coordinates.y,
						 static_cast<pixel_t>(FramePainter::PhysicalHeight - 1 - coordinates.x) };

			case TransformCaseType::Rotate180:
				return { static_cast<pixel_t>(FramePainter::PhysicalWidth - 1 - coordinates.x),
						 static_cast<pixel_t>(FramePainter::PhysicalHeight - 1 - coordinates.y) };

			case TransformCaseType::Rotate180MirrorX:
				return { coordinates.x,
						 static_cast<pixel_t>(FramePainter::PhysicalHeight - 1 - coordinates.y) };

			case TransformCaseType::Rotate180MirrorY:
				return { static_cast<pixel_t>(FramePainter::PhysicalWidth - 1 - coordinates.x),
						 coordinates.y };
			}
		}

#if defined(ARDUINO_ARCH_RP2040)
	private:
		bool AsyncClearBuffer()
		{
			if (Buffer == nullptr || BufferSize == 0)
				return true;

			if (!DmaActive)
			{
				// Align destination to 32-bit boundary: pre-clear leading bytes until aligned.
				uint8_t* dest = Buffer;
				size_t remaining = BufferSize;

				// Pre-align to 4-byte boundary with byte memset.
				while (((reinterpret_cast<uintptr_t>(dest) & 0x3u) != 0) && remaining > 0)
				{
					*dest++ = displayOptions::Inverted ? UINT8_MAX : 0;
					remaining--;
				}

				// Compute word count for aligned body and leave any tail bytes for after DMA.
				const size_t wordCount = remaining / sizeof(uint32_t);

				DmaChan = dma_claim_unused_channel(true);
				dma_channel_config cfg = dma_channel_get_default_config(DmaChan);
				channel_config_set_read_increment(&cfg, false);
				channel_config_set_write_increment(&cfg, true);
				channel_config_set_transfer_data_size(&cfg, DMA_SIZE_32);

				if (wordCount > 0)
				{
					dma_channel_configure(
						DmaChan,
						&cfg,
						reinterpret_cast<uint32_t*>(dest), // aligned destination
						displayOptions::Inverted ? &FillOnes32 : &FillZero32, // static source constant
						wordCount,                          // number of 32-bit words
						true                                // start immediately
					);
					DmaActive = true;
					return false; // DMA in progress
				}
				else
				{
					// No aligned words; clear any remaining tail bytes now.
					if (remaining)
					{
						memset(dest, displayOptions::Inverted ? UINT8_MAX : 0, remaining);
					}
					return true;
				}
			}

			// Poll DMA completion.
			if (dma_channel_is_busy(DmaChan))
			{
				return false;
			}

			// DMA complete: clear any tail bytes (remaining % 4).
			// Recompute aligned region to find tail safely.
			uint8_t* destAlignedStart = Buffer;
			size_t remaining = BufferSize;

			while (((reinterpret_cast<uintptr_t>(destAlignedStart) & 0x3u) != 0) && remaining > 0)
			{
				// already byte-cleared in start phase
				destAlignedStart++;
				remaining--;
			}

			const size_t alignedBytes = (remaining / sizeof(uint32_t)) * sizeof(uint32_t);
			uint8_t* tail = destAlignedStart + alignedBytes;
			const size_t tailBytes = remaining - alignedBytes;

			if (tailBytes)
			{
				memset(tail, displayOptions::Inverted ? UINT8_MAX : 0, tailBytes);
			}

			dma_channel_unclaim(DmaChan);
			DmaChan = -1;
			DmaActive = false;

			return true;
		}
#endif
	};
}
#endif