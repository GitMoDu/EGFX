#ifndef _EGFX_SURFACE_FPS_DRAWER_h
#define _EGFX_SURFACE_FPS_DRAWER_h

#include "../Model/IFrameDraw.h"

#include "../BitmaskFont/ColorFontDrawer.h"
#include "../BitmaskFont/Fonts/Plastic/Plastic.h"

namespace Egfx
{
	/// <summary>
	/// Specifies the corner position for the FPS overlay on the display.
	/// </summary>
	enum class FpsDrawerPosition
	{
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};

	/// <summary>
	/// Draws a frame-per-second overlay using a templated layout and font writer.
	/// Keeps a rolling average fps counter at the specified position on the layout.
	/// </summary>
	/// <typeparam name="Layout">Layout providing origin (X, Y) and size (Width, Height) for text placement.</typeparam>
	/// <typeparam name="fpsDrawerPosition">Corner of the layout where the FPS overlay is rendered.</typeparam>
	/// <typeparam name="FontDrawerType">Font drawer used to render the numeric FPS value and label. Defaults to Bitmask Font with Plastic FontType3x5.</typeparam>
	/// <typeparam name="AveragingSampleCount">Number of recent frames used to compute the displayed FPS average.</typeparam>
	template<typename Layout
		, FpsDrawerPosition fpsDrawerPosition = FpsDrawerPosition::TopRight
		, typename FontDrawerType = BitmaskFont::TemplateColorFontDrawer<BitmaskFonts::Plastic::FontType3x5, FontText::FullColorSource>
		, uint8_t AveragingSampleCount = 5>
	class DisplayFpsDrawer : public IFrameDraw
	{
	private:
		enum class StateEnum : uint8_t
		{
			Disabled,
			WaitingFirstFrame,
			AccumulatingFirstFrames,
			AveragingFrames,
		};

	private:
		static constexpr char LabelText[] PROGMEM = "FPS";
		static constexpr uint32_t MaxSampleDuration = UINT32_MAX / AveragingSampleCount;

	public:
		FontText::TemplateTextWriter<FontDrawerType, Layout> TextDrawer{};

	private:
		uint32_t LastFrametime = 0;
		uint32_t Accumulator = 0;

		pixel_t LabelWidth;
		pixel_t SpaceWidth;

		uint16_t LastFrameRate = 0;
		uint8_t SampleCount = 0;
		StateEnum CurrentState = StateEnum::WaitingFirstFrame;

	public:
		DisplayFpsDrawer() : IFrameDraw()
			, LabelWidth(TextDrawer.GetTextWidth(LabelText))
			, SpaceWidth(TextDrawer.GetSpaceWidth() + TextDrawer.GetKerningWidth())
		{
		}

		bool IsEnabled() const
		{
			return CurrentState != StateEnum::Disabled;
		}

		void SetEnabled(const bool enabled)
		{
			if (enabled)
			{
				if (CurrentState == StateEnum::Disabled)
				{
					CurrentState = StateEnum::WaitingFirstFrame;
				}
			}
			else
			{
				CurrentState = StateEnum::Disabled;
			}
		}

		bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
		{
			const uint32_t frameDuration = frameTime - LastFrametime;
			LastFrametime = frameTime;

			switch (CurrentState)
			{
			case StateEnum::Disabled:
				return true;
			case StateEnum::WaitingFirstFrame:
				CurrentState = StateEnum::AccumulatingFirstFrames;
				Accumulator = 0;
				SampleCount = 0;
				LastFrameRate = 0;
				return true;
			case StateEnum::AccumulatingFirstFrames:
				Accumulator += MinValue(frameDuration, MaxSampleDuration);
				SampleCount++;
				LastFrameRate = Accumulator > 0 ? (uint32_t)1000000 * SampleCount / Accumulator : 0;
				if (SampleCount >= AveragingSampleCount)
				{
					CurrentState = StateEnum::AveragingFrames;
				}
				break;
			case StateEnum::AveragingFrames:
				Accumulator += MinValue(frameDuration, MaxSampleDuration);
				SampleCount++;
				if (SampleCount >= AveragingSampleCount)
				{
					LastFrameRate = Accumulator > 0 ? (uint32_t)1000000 * AveragingSampleCount / Accumulator : 0;
					Accumulator = 0;
					SampleCount = 0;
				}
				break;
			default:
				break;
			};

			const pixel_t valueWidth = TextDrawer.GetNumberWidth(LastFrameRate);

			switch (fpsDrawerPosition)
			{
			case FpsDrawerPosition::TopLeft:
				TextDrawer.Write(frame, Layout::X(), Layout::Y(), LastFrameRate);
				TextDrawer.Write(frame, Layout::X() + valueWidth + SpaceWidth, Layout::Y(), reinterpret_cast<const char*>(LabelText));
				break;
			case FpsDrawerPosition::TopRight:
				TextDrawer.Write(frame, Layout::X() + Layout::Width() - LabelWidth, Layout::Y(), reinterpret_cast<const char*>(LabelText));
				TextDrawer.Write(frame, Layout::X() + Layout::Width() - LabelWidth - valueWidth - SpaceWidth, Layout::Y(), LastFrameRate);
				break;
			case FpsDrawerPosition::BottomLeft:
				TextDrawer.Write(frame, Layout::X(), Layout::Y() + Layout::Height() - TextDrawer.GetFontHeight(), LastFrameRate);
				TextDrawer.Write(frame, Layout::X() + valueWidth + SpaceWidth, Layout::Y() + Layout::Height() - TextDrawer.GetFontHeight(), reinterpret_cast<const char*>(LabelText));
				break;
			case FpsDrawerPosition::BottomRight:
				TextDrawer.Write(frame, Layout::X() + Layout::Width() - LabelWidth, Layout::Y() + Layout::Height() - TextDrawer.GetFontHeight(), reinterpret_cast<const char*>(LabelText));
				TextDrawer.Write(frame, Layout::X() + Layout::Width() - LabelWidth - valueWidth - SpaceWidth, Layout::Y() + Layout::Height() - TextDrawer.GetFontHeight(), LastFrameRate);
				break;
			default:
				break;
			}

			return true;
		}
	};
}
#endif