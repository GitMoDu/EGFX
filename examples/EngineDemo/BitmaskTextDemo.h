#ifndef _BITMASK_TEXT_DEMO_h
#define _BITMASK_TEXT_DEMO_h

#include <EgfxFramework.h>
#include <EgfxAssets.h>
#include "Assets.h"

// Demo of bitmask font text drawing with fixed scaling.
namespace BitmaskTextDemo
{
	using namespace Egfx;

	namespace AsciiDefinition = Framework::AsciiDefinition;

	namespace Definitions
	{
		// Bitmask fonts are set with a template parameter type.
		// Share font for regular and upscaled font drawers.
		using BitmaskFontType = Framework::Assets::Font::Bitmask::Plastic::FontType5x5;

		template<typename ParentLayout>
		struct BigFontDefinitions
		{
			// Determine scaling factors based on layout size.
			static constexpr pixel_t ScaleDimension = MinValue(ParentLayout::Width(), ParentLayout::Height());
			static constexpr uint8_t ScaleX = 1 + (ScaleDimension / 48);
			static constexpr uint8_t ScaleY = 1 + (ScaleDimension / 32);

			// Single color, bitmask scaled font drawer type.
			using BitmaskFontDrawerType = Framework::Bitmask::Font::TemplateDrawer<BitmaskFontType, ScaleX, ScaleY>;

			// Bitmask text drawer type with all the template parameters set.
			using BitmaskTextDrawerType = Framework::Text::TemplateWriter<ParentLayout, BitmaskFontDrawerType>;
		};

		template<typename ParentLayout>
		struct SmallFontDefinitions
		{
			// Single color bitmask font drawer type.
			using BitmaskFontDrawerType = Framework::Bitmask::Font::TemplateDrawer<BitmaskFontType>;

			// Bitmask text drawer type with all the template parameters set.
			using BitmaskTextDrawerType = Framework::Text::TemplateWriter<ParentLayout, BitmaskFontDrawerType>;
		};

		template<typename ParentLayout>
		using BigTextDrawerType = typename BigFontDefinitions<ParentLayout>::BitmaskTextDrawerType;
		template<typename ParentLayout>
		using SmallTextDrawerType = typename SmallFontDefinitions<ParentLayout>::BitmaskTextDrawerType;
	}

	namespace Drawables
	{
		template<typename ParentLayout, typename TextDrawerType>
		struct Numbers
		{
			TextDrawerType* TextDrawer = nullptr;
			uint16_t Value = 0;

			void Draw(IFrameBuffer* frame)
			{
				if (TextDrawer != nullptr)
				{
					TextDrawer->Write(frame,
						ParentLayout::X(),
						ParentLayout::Y(),
						Value,
						Framework::TextAlignmentEnum::Center);
				}
			}
		};



		template<typename ParentLayout, typename TextDrawerType>
		struct SmallText1
		{
			TextDrawerType* TextDrawer = nullptr;
			pixel_point_t Translation{};

			void Draw(IFrameBuffer* frame)
			{
				if (TextDrawer != nullptr)
				{
					pixel_t cursor = ParentLayout::X() + Translation.x;
					cursor = TextDrawer->Write(frame, cursor, ParentLayout::Y() + Translation.y,
						reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox1));
					cursor += TextDrawer->GetKerningWidth();
					cursor += TextDrawer->GetCharacterWidth(char(AsciiDefinition::Printable::Space));
					cursor += TextDrawer->GetKerningWidth();
					cursor = TextDrawer->Write(frame, cursor, ParentLayout::Y() + Translation.y,
						reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox2));
					cursor += TextDrawer->GetKerningWidth();
					cursor += TextDrawer->GetCharacterWidth(char(AsciiDefinition::Printable::Space));
					cursor += TextDrawer->GetKerningWidth();
					TextDrawer->Write(frame, cursor, ParentLayout::Y() + Translation.y,
						reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox3));
				}
			}
		};

		template<typename ParentLayout, typename TextDrawerType>
		struct SmallText2
		{
			TextDrawerType* TextDrawer = nullptr;
			pixel_point_t Translation{};

			void Draw(IFrameBuffer* frame)
			{
				if (TextDrawer != nullptr)
				{
					pixel_t cursor = ParentLayout::X() + Translation.x;
					cursor = TextDrawer->Write(frame, cursor, ParentLayout::Y() + Translation.y,
						reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox1));
					cursor += TextDrawer->GetKerningWidth();
					cursor += TextDrawer->GetCharacterWidth(char(AsciiDefinition::Printable::Space));
					cursor += TextDrawer->GetKerningWidth();
					cursor = TextDrawer->Write(frame, cursor, ParentLayout::Y() + Translation.y,
						reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox2));
					cursor += TextDrawer->GetKerningWidth();
					cursor += TextDrawer->GetCharacterWidth(char(AsciiDefinition::Printable::Space));
					cursor += TextDrawer->GetKerningWidth();
					TextDrawer->Write(frame, cursor, ParentLayout::Y() + Translation.y,
						reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox3));
				}
			}
		};

		template<typename ParentLayout, typename TextDrawerType>
		struct BigText
		{
			TextDrawerType* TextDrawer = nullptr;
			pixel_point_t Translation{};

			void Draw(IFrameBuffer* frame)
			{
				if (TextDrawer != nullptr)
				{
					pixel_t cursor = ParentLayout::X() + Translation.x;
					cursor = TextDrawer->Write(frame, cursor, ParentLayout::Y() + Translation.y,
						reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox1));
					cursor += TextDrawer->GetKerningWidth();
					cursor += TextDrawer->GetCharacterWidth(char(AsciiDefinition::Printable::Space));
					cursor += TextDrawer->GetKerningWidth();
					cursor = TextDrawer->Write(frame, cursor, ParentLayout::Y() + Translation.y,
						reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox2));
					cursor += TextDrawer->GetKerningWidth();
					cursor += TextDrawer->GetCharacterWidth(char(AsciiDefinition::Printable::Space));
					cursor += TextDrawer->GetKerningWidth();
					TextDrawer->Write(frame, cursor, ParentLayout::Y() + Translation.y,
						reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox3));
				}
			}
		};
	}

	template<typename ParentLayout>
	struct TextLayout
	{
		struct Numbers
		{
			static constexpr pixel_t Height() { return Definitions::SmallTextDrawerType<ParentLayout>::GetFontHeight(); }

			static constexpr pixel_t X() { return ParentLayout::X() + ParentLayout::Width() / 2; }
			static constexpr pixel_t Y() { return ParentLayout::Y(); }
		};

		struct BigText
		{
			static constexpr pixel_t X() { return ParentLayout::X(); }
			static constexpr pixel_t Y() { return Numbers::Y() + Numbers::Height() + 1; }

			static constexpr pixel_t Height() { return Definitions::BigTextDrawerType<ParentLayout>::GetFontHeight(); }
		};

		struct SmallText1
		{
			static constexpr pixel_t X() { return ParentLayout::X(); }
			static constexpr pixel_t Y() { return BigText::Y() + BigText::Height() + 1; }

			static constexpr pixel_t Height() { return Definitions::SmallTextDrawerType<ParentLayout>::GetFontHeight(); }
		};

		struct SmallText2
		{
			static constexpr pixel_t X() { return ParentLayout::X(); }
			static constexpr pixel_t Y() { return SmallText1::Y() + SmallText1::Height() + 1; }

			static constexpr pixel_t Height() { return Definitions::SmallTextDrawerType<ParentLayout>::GetFontHeight(); }
		};

		// Minimum font height in pixels.
		static constexpr uint8_t MinFontHeight = 8;

		// Determine maximum font height based on frame size.
		static constexpr pixel_t MaxFontHeight = MinValue(ParentLayout::Width(), ParentLayout::Height()) - MinFontHeight - 1;
	};

	namespace Animators
	{
		namespace TextScroller
		{
			pixel_t GetOffset(const uint32_t frameTime, const uint32_t speedDivisor, const pixel_t textWidth, const pixel_t layoutWidth)
			{
				const uint32_t scrollSpan = static_cast<uint32_t>(layoutWidth) + static_cast<uint32_t>(textWidth);
				if (scrollSpan == 0U)
				{
					return pixel_t{};
				}
				const uint32_t phase = (frameTime / speedDivisor) % scrollSpan;
				return static_cast<pixel_t>(static_cast<int32_t>(layoutWidth) - static_cast<int32_t>(phase));
			}
		}
	}

	template<typename ParentLayout, bool Monochrome>
	class View : public Framework::View::DrawablesView<
		Drawables::Numbers<typename TextLayout<ParentLayout>::Numbers, Definitions::SmallTextDrawerType<ParentLayout>>,
		Drawables::BigText<typename TextLayout<ParentLayout>::BigText, Definitions::BigTextDrawerType<ParentLayout>>,
		Drawables::SmallText1<typename TextLayout<ParentLayout>::SmallText1, Definitions::SmallTextDrawerType<ParentLayout>>,
		Drawables::SmallText2<typename TextLayout<ParentLayout>::SmallText2, Definitions::SmallTextDrawerType<ParentLayout>>>
	{
	private:
		using Base = Framework::View::DrawablesView<
			Drawables::Numbers<typename TextLayout<ParentLayout>::Numbers, Definitions::SmallTextDrawerType<ParentLayout>>,
			Drawables::BigText<typename TextLayout<ParentLayout>::BigText, Definitions::BigTextDrawerType<ParentLayout>>,
			Drawables::SmallText1<typename TextLayout<ParentLayout>::SmallText1, Definitions::SmallTextDrawerType<ParentLayout>>,
			Drawables::SmallText2<typename TextLayout<ParentLayout>::SmallText2, Definitions::SmallTextDrawerType<ParentLayout>>>;

	private:
		Definitions::BigTextDrawerType<ParentLayout> BigTextDrawer{};
		Definitions::SmallTextDrawerType<ParentLayout> SmallTextDrawer{};

	private:
		// Cached text widths for scrolling calculations.
		const pixel_t LongTextWidth;
		const pixel_t ShortTextWidth;

	public:
		View()
			: Base()
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
			, ShortTextWidth(
				SmallTextDrawer.GetTextWidth(reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox1))
				+ SmallTextDrawer.GetKerningWidth()
				+ SmallTextDrawer.GetCharacterWidth(char(AsciiDefinition::Printable::Space))
				+ SmallTextDrawer.GetKerningWidth()
				+ SmallTextDrawer.GetTextWidth(reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox2))
				+ SmallTextDrawer.GetKerningWidth()
				+ SmallTextDrawer.GetCharacterWidth(char(AsciiDefinition::Printable::Space))
				+ SmallTextDrawer.GetKerningWidth()
				+ SmallTextDrawer.GetTextWidth(reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox3)))
		{
			Base::template drawable<0>().TextDrawer = &SmallTextDrawer;
			Base::template drawable<1>().TextDrawer = &BigTextDrawer;
			Base::template drawable<2>().TextDrawer = &SmallTextDrawer;
			Base::template drawable<3>().TextDrawer = &SmallTextDrawer;
		}

	protected:
		/// <summary>
		/// View-level animation orchestration - updates all drawable state.
		/// No framebuffer access - pure state update.
		/// </summary>
		bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
		{
			auto& numbers = Base::template drawable<0>();
			auto& bigText = Base::template drawable<1>();
			auto& smallText1 = Base::template drawable<2>();
			auto& smallText2 = Base::template drawable<3>();

			numbers.Value = frameCounter;

			if (!Monochrome)
			{
				const auto primaryColor = Rgb::ColorFromHSV(static_cast<angle_t>(frameTime / 100), UINT8_MAX, UINT8_MAX);
				BigTextDrawer.ColorSource.Color = primaryColor;
				SmallTextDrawer.ColorSource.Color = Rgb::Color(uint32_t(~primaryColor));
			}

			bigText.Translation.x = Animators::TextScroller::GetOffset(frameTime, 15000U, LongTextWidth, ParentLayout::Width());
			smallText1.Translation.x = Animators::TextScroller::GetOffset(frameTime, 30000U, ShortTextWidth, ParentLayout::Width());
			smallText2.Translation.x = Animators::TextScroller::GetOffset(frameTime, 45000U, ShortTextWidth, ParentLayout::Width());
			return true;
		}
	};

	template<typename ParentLayout, bool Monochrome>
	struct Frame : public Framework::View::FrameAdapter<View<ParentLayout, Monochrome>>
	{
		using Base = Framework::View::FrameAdapter<View<ParentLayout, Monochrome>>;
		Frame() : Base() {}
		virtual ~Frame() = default;

#if defined(SERIAL_LOG)
		void PrintDescription() const
		{
			Serial.println(F("Bitmask Sprite Font\n\tfixed size fonts, with shader-capable font rendering."));
		}
#endif
	};
}
#endif