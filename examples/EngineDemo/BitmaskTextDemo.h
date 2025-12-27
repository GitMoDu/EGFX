#ifndef _TEXTSPRITEDEMO_h
#define _TEXTSPRITEDEMO_h

#include <EgfxDrawer.h>

using namespace Egfx;

template<typename Layout, bool BinaryDisplay>
class BitmaskTextDemo final : public IFrameDraw
{
private:
	enum class DrawCallEnum : uint8_t
	{
		Numbers,
		BigText,
		SmallText1,
		SmallText2,
		EnumCount
	};

	// Single color font shading.
	// Share implementation with binary displays and font drawers.
	using FontColorType = Egfx::VectorFont::FontColorSourceSingle;

	// Bitmask fonts are set with a template parameter type.
	// Share font for regular and upscaled font drawers.
	using BitmaskFontType = Egfx::BitmaskFonts::Plastic::FontType5x5;

	struct BigFontDefinitions
	{
		// Determine scaling factors based on layout size.
		static constexpr pixel_t ScaleDimension = MaxValue(Layout::Width(), Layout::Height());
		static constexpr uint8_t ScaleX = 1 + (ScaleDimension / 128);
		static constexpr uint8_t ScaleY = 1 + (ScaleDimension / 96);

		// Single color, bitmask scaled font drawer type.
		using BitmaskFontDrawerType = Egfx::BitmaskFont::TemplateColorScaledFontDrawer<BitmaskFontType, FontColorType, ScaleX, ScaleY>;

		// Bitmask text drawer type with all the template parameters set.
		using BitmaskTextDrawerType = Egfx::FontText::TemplateTextWriter<BitmaskFontDrawerType, Layout>;
	};

	struct SmallFontDefinitions
	{
		// Single color bitmask font drawer type.
		using BitmaskFontDrawerType = Egfx::BitmaskFont::TemplateColorFontDrawer<BitmaskFontType, FontColorType>;

		// Bitmask text drawer type with all the template parameters set.
		using BitmaskTextDrawerType = Egfx::FontText::TemplateTextWriter<BitmaskFontDrawerType, Layout>;
	};

	struct LayerLayout
	{
		struct Numbers
		{
			static constexpr pixel_t Height() { return SmallFontDefinitions::BitmaskFontDrawerType::GetFontHeight(); }

			static constexpr pixel_t X() { return Layout::X() + Layout::Width() / 2; }
			static constexpr pixel_t Y() { return Layout::Y(); }
		};

		struct BigText
		{
			static constexpr pixel_t X() { return Layout::X(); }
			static constexpr pixel_t Y() { return Numbers::Y() + Numbers::Height() + 1; }

			static constexpr pixel_t Height() { return BigFontDefinitions::BitmaskFontDrawerType::GetFontHeight(); }
		};

		struct SmallText1
		{
			static constexpr pixel_t X() { return Layout::X(); }
			static constexpr pixel_t Y() { return BigText::Y() + BigText::Height() + 1; }

			static constexpr pixel_t Height() { return SmallFontDefinitions::BitmaskFontDrawerType::GetFontHeight(); }
		};

		struct SmallText2
		{
			static constexpr pixel_t X() { return Layout::X(); }
			static constexpr pixel_t Y() { return SmallText1::Y() + SmallText1::Height() + 1; }

			static constexpr pixel_t Height() { return SmallFontDefinitions::BitmaskFontDrawerType::GetFontHeight(); }
		};

		static constexpr uint8_t Kerning = 1;

		// Minimum font height in pixels.
		static constexpr uint8_t MinFontHeight = 8;

		// Determine maximum font height based on frame size.
		static constexpr pixel_t MaxFontHeight = MinValue(Layout::Width(), Layout::Height()) - MinFontHeight - 1;
	};

private:
	typename BigFontDefinitions::BitmaskTextDrawerType BigTextDrawer{};
	typename SmallFontDefinitions::BitmaskTextDrawerType SmallTextDrawer{};

	const pixel_t LongTextWidth;
	const pixel_t ShortTextWidth;

	uint8_t CallIndex = 0;

public:
	BitmaskTextDemo() : IFrameDraw()
		, LongTextWidth(
			BigTextDrawer.GetTextWidth(reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox1))
			+ BigTextDrawer.GetKerningWidth()
			+ BigTextDrawer.GetCharacterWidth(char(AsciiDefinition::Printable::Space))
			+ BigTextDrawer.GetKerningWidth()
			+ BigTextDrawer.GetTextWidth(reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox2))
			+ BigTextDrawer.GetKerningWidth()
			+ BigTextDrawer.GetCharacterWidth(char(AsciiDefinition::Printable::Space))
			+ BigTextDrawer.GetKerningWidth()
			+ BigTextDrawer.GetTextWidth(reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox3)))
		, ShortTextWidth((SmallTextDrawer.GetTextWidth(reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox1))
			+ SmallTextDrawer.GetKerningWidth()
			+ SmallTextDrawer.GetCharacterWidth(char(AsciiDefinition::Printable::Space))
			+ SmallTextDrawer.GetKerningWidth()
			+ SmallTextDrawer.GetTextWidth(reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox2))
			+ SmallTextDrawer.GetKerningWidth()
			+ SmallTextDrawer.GetCharacterWidth(char(AsciiDefinition::Printable::Space))
			+ SmallTextDrawer.GetKerningWidth()
			+ SmallTextDrawer.GetTextWidth(reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox3))))
	{
	}

	~BitmaskTextDemo() = default;

	// Always enabled.
	bool IsEnabled() const final { return true; }
	void SetEnabled(const bool /*enabled*/) final {}

#if defined(SERIAL_LOG)
	void PrintDescription() const
	{
		Serial.println(F("Bitmask Sprite Font\n\tfixed size fonts, with shader-capable font rendering."));
	}
#endif

	bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
	{
		switch (DrawCallEnum(CallIndex))
		{
		case DrawCallEnum::Numbers:
			if (!BinaryDisplay)
			{
				// Update color based on time.
				BigTextDrawer.FontColor.Color = Rgb::ColorFromHSV(static_cast<angle_t>(frameTime / 100), UINT8_MAX, UINT8_MAX);
				SmallTextDrawer.FontColor.Color = Rgb::Color(uint32_t(~BigTextDrawer.FontColor.Color));
			}
			SmallTextDrawer.Write(frame,
				LayerLayout::Numbers::X(), LayerLayout::Numbers::Y(),
				frameCounter,
				Egfx::FontText::TextAlignmentEnum::Center);
			break;
		case DrawCallEnum::BigText:
		{
			pixel_t cursor = LayerLayout::BigText::X() + Layout::Width() - ((frameTime / 15000) % (LongTextWidth + Layout::Width()));
			cursor = BigTextDrawer.Write(frame, cursor, LayerLayout::BigText::Y(),
				reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox1));
			cursor += BigTextDrawer.GetKerningWidth();
			cursor += BigTextDrawer.GetCharacterWidth(char(AsciiDefinition::Printable::Space));
			cursor += BigTextDrawer.GetKerningWidth();
			cursor = BigTextDrawer.Write(frame, cursor, LayerLayout::BigText::Y(),
				reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox2));
			cursor += BigTextDrawer.GetKerningWidth();
			cursor += BigTextDrawer.GetCharacterWidth(char(AsciiDefinition::Printable::Space));
			cursor += BigTextDrawer.GetKerningWidth();
			BigTextDrawer.Write(frame, cursor, LayerLayout::BigText::Y(),
				reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox3));
		}
		break;
		case DrawCallEnum::SmallText1:
		{
			pixel_t cursor = LayerLayout::SmallText1::X() + Layout::Width() - ((frameTime / 30000) % (ShortTextWidth + Layout::Width()));
			cursor = SmallTextDrawer.Write(frame, cursor, LayerLayout::SmallText1::Y(),
				reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox1));
			cursor += SmallTextDrawer.GetKerningWidth();
			cursor += SmallTextDrawer.GetCharacterWidth(char(AsciiDefinition::Printable::Space));
			cursor += SmallTextDrawer.GetKerningWidth();
			cursor = SmallTextDrawer.Write(frame, cursor, LayerLayout::SmallText1::Y(),
				reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox2));
			cursor += SmallTextDrawer.GetKerningWidth();
			cursor += SmallTextDrawer.GetCharacterWidth(char(AsciiDefinition::Printable::Space));
			cursor += SmallTextDrawer.GetKerningWidth();
			SmallTextDrawer.Write(frame, cursor, LayerLayout::SmallText1::Y(),
				reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox3));
		}
		break;
		case DrawCallEnum::SmallText2:
		{
			pixel_t cursor = LayerLayout::SmallText2::X() + Layout::Width() - ((frameTime / 45000) % (ShortTextWidth + Layout::Width()));
			cursor = SmallTextDrawer.Write(frame, cursor, LayerLayout::SmallText2::Y(),
				reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox1));
			cursor += SmallTextDrawer.GetKerningWidth();
			cursor += SmallTextDrawer.GetCharacterWidth(char(AsciiDefinition::Printable::Space));
			cursor += SmallTextDrawer.GetKerningWidth();
			cursor = SmallTextDrawer.Write(frame, cursor, LayerLayout::SmallText2::Y(),
				reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox2));
			cursor += SmallTextDrawer.GetKerningWidth();
			cursor += SmallTextDrawer.GetCharacterWidth(char(AsciiDefinition::Printable::Space));
			cursor += SmallTextDrawer.GetKerningWidth();
			SmallTextDrawer.Write(frame, cursor, LayerLayout::SmallText2::Y(),
				reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox3));
		}
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