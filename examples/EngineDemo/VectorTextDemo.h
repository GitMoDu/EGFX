#ifndef _VECTOR_TEXT_DEMO_h
#define _VECTOR_TEXT_DEMO_h

#include <EgfxFramework.h>
#include <EgfxAssets.h>
#include "Assets.h"

// Demo of vector font text drawing with dynamic scaling.
namespace VectorTextDemo
{
	using namespace Egfx;

	namespace AsciiDefinition = Framework::AsciiDefinition;

	namespace Definitions
	{
		template<typename ParentLayout>
		struct VectorFontDefinitions
		{
			using VectorFontType = Framework::Assets::Font::Vector::Epoxy::FullFontType;

			using VectorFontDrawerType = Framework::Vector::Font::TemplateDrawer<VectorFontType>;
			using VectorTextDrawerType = Framework::Text::TemplateWriter<ParentLayout, VectorFontDrawerType>;
		};

		template<typename ParentLayout>
		struct CodeFontDefinitions
		{
			using CodeFontDrawerType = Framework::Assets::Font::Code::RawBot<>;
			using CodeTextDrawerType = Framework::Text::TemplateWriter<ParentLayout, CodeFontDrawerType>;
		};

		template<typename ParentLayout>
		using VectorTextDrawerType = typename VectorFontDefinitions<ParentLayout>::VectorTextDrawerType;

		template<typename ParentLayout>
		using CodeTextDrawerType = typename CodeFontDefinitions<ParentLayout>::CodeTextDrawerType;
	}

	template<typename ParentLayout>
	struct Layout
	{
		struct Numbers
		{
			static constexpr pixel_t Height() { return MinFontHeight; }
			static constexpr pixel_t X() { return ParentLayout::X() + ParentLayout::Width() / 2; }
			static constexpr pixel_t Y() { return ParentLayout::Y(); }
		};

		struct ScalingText
		{
			static constexpr pixel_t X() { return ParentLayout::X(); }
			static constexpr pixel_t Y() { return Numbers::Y() + Numbers::Height() + 1; }
		};

		static constexpr uint8_t Kerning = 4;
		static constexpr uint8_t MinFontHeight = 8;
		static constexpr pixel_t MaxFontHeight = MinValue(ParentLayout::Width(), ParentLayout::Height()) - MinFontHeight - 1;
	};

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
		struct LongText
		{

			TextDrawerType* TextDrawer = nullptr;

			void Draw(IFrameBuffer* frame)
			{
				if (TextDrawer == nullptr)
				{
					return;
				}

				pixel_t cursor = ParentLayout::X();
				const pixel_t baselineY = ParentLayout::Y();

				cursor = TextDrawer->Write(frame, cursor, baselineY,
					reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox1));
				cursor += TextDrawer->GetKerningWidth();
				cursor += TextDrawer->GetCharacterWidth(char(AsciiDefinition::Printable::Space));
				cursor += TextDrawer->GetKerningWidth();
				cursor = TextDrawer->Write(frame, cursor, baselineY,
					reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox2));
				cursor += TextDrawer->GetKerningWidth();
				cursor += TextDrawer->GetCharacterWidth(char(AsciiDefinition::Printable::Space));
				cursor += TextDrawer->GetKerningWidth();
				TextDrawer->Write(frame, cursor, baselineY,
					reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox3));
			}
		};

		template<typename ParentLayout, typename TextDrawerType, uint8_t LineIndex>
		struct HighTextLine
		{
			TextDrawerType* TextDrawer = nullptr;
			const __FlashStringHelper* Text = nullptr;

			void Draw(IFrameBuffer* frame)
			{
				if (TextDrawer == nullptr || Text == nullptr)
				{
					return;
				}

				const pixel_t lineHeight = static_cast<pixel_t>(TextDrawer->GetFontHeight());
				const pixel_t y = ParentLayout::Y() + (lineHeight + 1) * static_cast<pixel_t>(LineIndex);

				TextDrawer->Write(frame, ParentLayout::X(), y, Text);
			}
		};
	}

	template<typename ParentLayout, bool Monochrome>
	class View : public Framework::View::DrawablesView<
		Drawables::Numbers<typename Layout<ParentLayout>::Numbers, Definitions::CodeTextDrawerType<ParentLayout>>,
		Drawables::LongText<typename Layout<ParentLayout>::ScalingText, Definitions::VectorTextDrawerType<ParentLayout>>,
		Drawables::HighTextLine<typename Layout<ParentLayout>::ScalingText, Definitions::VectorTextDrawerType<ParentLayout>, 1>,
		Drawables::HighTextLine<typename Layout<ParentLayout>::ScalingText, Definitions::VectorTextDrawerType<ParentLayout>, 2>>
	{
	private:
		using LayoutDefinition = Layout<ParentLayout>;
		using Base = Framework::View::DrawablesView<
			Drawables::Numbers<typename LayoutDefinition::Numbers, Definitions::CodeTextDrawerType<ParentLayout>>,
			Drawables::LongText<typename LayoutDefinition::ScalingText, Definitions::VectorTextDrawerType<ParentLayout>>,
			Drawables::HighTextLine<typename LayoutDefinition::ScalingText, Definitions::VectorTextDrawerType<ParentLayout>, 1>,
			Drawables::HighTextLine<typename LayoutDefinition::ScalingText, Definitions::VectorTextDrawerType<ParentLayout>, 2>>;

		static constexpr uint32_t FontScalePeriodUs = 9000000U;

	private:
		Definitions::VectorTextDrawerType<ParentLayout> VectorTextDrawer{};
		Definitions::CodeTextDrawerType<ParentLayout> CodeTextDrawer{};

	public:
		View()
			: Base()
		{
			CodeTextDrawer.SetFontDimensions(LayoutDefinition::MinFontHeight, LayoutDefinition::MinFontHeight, 1);
			VectorTextDrawer.SetFontDimensions(LayoutDefinition::MinFontHeight, LayoutDefinition::MinFontHeight, LayoutDefinition::Kerning);

			auto& numbers = Base::template drawable<0>();
			numbers.TextDrawer = &CodeTextDrawer;

			auto& longText = Base::template drawable<1>();
			longText.TextDrawer = &VectorTextDrawer;

			auto& highTextLine1 = Base::template drawable<2>();
			highTextLine1.TextDrawer = &VectorTextDrawer;
			highTextLine1.Text = reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox2);

			auto& highTextLine2 = Base::template drawable<3>();
			highTextLine2.TextDrawer = &VectorTextDrawer;
			highTextLine2.Text = reinterpret_cast<const __FlashStringHelper*>(Assets::Strings::ShortFox3);
		}

	protected:
		bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
		{
			auto& numbers = Base::template drawable<0>();
			numbers.Value = frameCounter;

			const uint16_t scaleProgress = ProgressScaler::TriangleResponse(
				ProgressScaler::GetProgress<FontScalePeriodUs>(frameTime));

			const auto animatedWidth = static_cast<uint8_t>(
				ProgressScaler::ScaleProgress(scaleProgress, LayoutDefinition::MinFontHeight))
				+ static_cast<uint8_t>(LayoutDefinition::MinFontHeight / 2);
			CodeTextDrawer.SetFontWidth(animatedWidth);

			const auto maxDelta = static_cast<uint8_t>(LayoutDefinition::MaxFontHeight - LayoutDefinition::MinFontHeight);
			const auto fontDimension = static_cast<uint8_t>(
				ProgressScaler::ScaleProgress(scaleProgress, maxDelta)) + LayoutDefinition::MinFontHeight;
			VectorTextDrawer.SetFontDimensions(fontDimension, fontDimension, LayoutDefinition::Kerning);

			if (!Monochrome)
			{
				CodeTextDrawer.ColorSource.Color = Rgb::ColorFromHSV(static_cast<angle_t>(frameTime / 100), UINT8_MAX, UINT8_MAX);
				VectorTextDrawer.ColorSource.Color = Rgb::Color(uint32_t(~CodeTextDrawer.ColorSource.Color));
			}
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
			Serial.println(F("Vector Font Text\n\tdraw vector-based text with configurable font width/height and color."));
		}
#endif
	};
}

#endif