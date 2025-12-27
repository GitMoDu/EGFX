#ifndef _VECTOR_TEXT_DEMO_h
#define _VECTOR_TEXT_DEMO_h

#include <EgfxDrawer.h>

using namespace Egfx;

template<typename Layout, bool BinaryDisplay>
class VectorTextDemo final : public IFrameDraw
{
private:
	enum class DrawCallEnum : uint8_t
	{
		Numbers,
		LongText,
		HighText,
		EnumCount
	};

	struct LayerLayout
	{
		struct Numbers
		{
			static constexpr pixel_t Height() { return MinFontHeight; }

			static constexpr pixel_t X() { return Layout::X() + Layout::Width() / 2; }
			static constexpr pixel_t Y() { return Layout::Y(); }
		};

		struct ScalingText
		{
			static constexpr pixel_t X() { return Layout::X(); }
			static constexpr pixel_t Y() { return Numbers::Y() + Numbers::Height() + 1; }
		};

		static constexpr uint8_t Kerning = 4;

		// Minimum font height in pixels.
		static constexpr uint8_t MinFontHeight = 8;

		// Determine maximum font height based on frame size.
		static constexpr pixel_t MaxFontHeight = MinValue(Layout::Width(), Layout::Height()) - MinFontHeight - 1;
	};

private:
	// Single color font shading.
	// Share implementation with binary displays and font drawers.
	using FontColorType = Egfx::FontText::SingleColorSource;

	struct VectorFontDefinitions
	{
		// Vector fonts are set with a template parameter type.
		using VectorFontType = Egfx::VectorFonts::Epoxy::FullFontType;

		// Single color vector font drawer type.
		using VectorFontDrawerType = Egfx::VectorFont::TemplateColorFontDrawer<VectorFontType, FontColorType>;

		// Vector text drawer type with all the template parameters set.
		using VectorTextDrawerType = Egfx::FontText::TemplateTextWriter<VectorFontDrawerType, Layout>;
	};

	struct CodeFontDefinitions
	{
		// Code fonts are set with a template parameter type.
		// RawBot is a simple vector font suitable for small displays.
		using CodeFontType = Egfx::CodeFont::CodeFonts::RawBot;

		// Single color code font drawer type.
		using CodeFontDrawerType = Egfx::CodeFont::TemplateColorFontDrawer<CodeFontType, FontColorType>;

		// Code text drawer type with all the template parameters set.
		using CodeTextDrawerType = Egfx::FontText::TemplateTextWriter<CodeFontDrawerType, Layout>;
	};

private:
	typename VectorFontDefinitions::VectorTextDrawerType VectorTextDrawer{};
	typename CodeFontDefinitions::CodeTextDrawerType CodeTextDrawer{};

	uint8_t CallIndex = 0;

public:
	VectorTextDemo() : IFrameDraw()
	{
		CodeTextDrawer.SetFontDimensions(LayerLayout::MinFontHeight, LayerLayout::MinFontHeight, LayerLayout::Kerning);
	}

	~VectorTextDemo() = default;

	// Always enabled.
	virtual bool IsEnabled() const final { return true; }
	virtual void SetEnabled(const bool /*enabled*/) final {}

#if defined(SERIAL_LOG)
	void PrintDescription() const
	{
		Serial.println(F("Vector Font Text\n\tdraw vector-based text with configurable font width/height and color."));
	}
#endif

	virtual bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
	{
		switch (DrawCallEnum(CallIndex))
		{
		case DrawCallEnum::Numbers:
		{
			if (!BinaryDisplay)
			{
				// Update color based on time.
				CodeTextDrawer.FontColor.Color = Rgb::ColorFromHSV(static_cast<angle_t>(frameTime / 100), UINT8_MAX, UINT8_MAX);
				VectorTextDrawer.FontColor.Color = Rgb::Color(uint32_t(~CodeTextDrawer.FontColor.Color));
			}

			// Update font size based on time.
			const uint16_t scaleProgress = ProgressScaler::TriangleResponse(ProgressScaler::GetProgress<9000000>(frameTime));

			// Animate width.
			CodeTextDrawer.SetFontWidth(ProgressScaler::ScaleProgress(scaleProgress, LayerLayout::MinFontHeight) + LayerLayout::MinFontHeight / 2);
			CodeTextDrawer.Write(frame,
				LayerLayout::Numbers::X(), LayerLayout::Numbers::Y(),
				frameCounter,
				Egfx::FontText::TextAlignmentEnum::Center);

			// Set font size for next draw calls.
			const auto fontDimension = ProgressScaler::ScaleProgress(scaleProgress, static_cast<uint8_t>(LayerLayout::MaxFontHeight - LayerLayout::MinFontHeight)) + LayerLayout::MinFontHeight;
			VectorTextDrawer.SetFontDimensions(fontDimension, fontDimension, LayerLayout::Kerning);
		}
		break;
		case DrawCallEnum::LongText:
		{
			pixel_t cursor = VectorTextDrawer.Write(frame, LayerLayout::ScalingText::X(), LayerLayout::ScalingText::Y(),
				reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox1));
			cursor += VectorTextDrawer.GetKerningWidth();
			cursor += VectorTextDrawer.GetCharacterWidth(char(AsciiDefinition::Printable::Space));
			cursor += VectorTextDrawer.GetKerningWidth();
			cursor = VectorTextDrawer.Write(frame, cursor, LayerLayout::ScalingText::Y(),
				reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox2));
			cursor += VectorTextDrawer.GetKerningWidth();
			cursor += VectorTextDrawer.GetCharacterWidth(char(AsciiDefinition::Printable::Space));
			cursor += VectorTextDrawer.GetKerningWidth();
			VectorTextDrawer.Write(frame, cursor, LayerLayout::ScalingText::Y(),
				reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox3));
		}
		break;
		case DrawCallEnum::HighText:
			VectorTextDrawer.Write(frame, LayerLayout::ScalingText::X(), LayerLayout::ScalingText::Y() + VectorTextDrawer.GetFontHeight() + 1,
				reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox2));
			VectorTextDrawer.Write(frame, LayerLayout::ScalingText::X(), LayerLayout::ScalingText::Y() + (VectorTextDrawer.GetFontHeight() + 1) * 2,
				reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox3));
			break;
		default:
			break;
		}

		CallIndex++; // Advance call index and determine if cycle is complete.
		if (CallIndex >= uint8_t(DrawCallEnum::EnumCount))
		{
			CallIndex = 0;
			return true;
		}

		return false;
	}
};

#endif

