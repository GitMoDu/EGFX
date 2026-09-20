#ifndef _DEMO_MODULES_TERMINAL_WINDOW_h
#define _DEMO_MODULES_TERMINAL_WINDOW_h

#include <IntegerGlassModules.h>

#include "../IDemoPage.h"
#include "../Constants.h"

namespace Showcase
{
	using namespace IntegerGlass;

	namespace TerminalWindow
	{
		template<typename ParentLayout>
		using MarginLayout = Framework::Layout::Margin<
			ParentLayout,
			Constants::Showcase::Margin<ParentLayout>(),
			0,
			Constants::Showcase::Margin<ParentLayout>(),
			Constants::Showcase::Margin<ParentLayout>()>;

		using FontType = Modules::TerminalWindow::Definitions::DefaultFont;

		static constexpr uint8_t TargetCharactersPerLine = 24;
		static constexpr uint8_t TargetLines = 8;

		template<typename ParentLayout>
		static constexpr uint8_t FontScale()
		{
			const uint16_t widthScale = static_cast<uint16_t>(ParentLayout::Width()) /
				(static_cast<uint16_t>(FontType::GetFontWidth()) * TargetCharactersPerLine);
			const uint16_t heightScale = static_cast<uint16_t>(ParentLayout::Height()) /
				(static_cast<uint16_t>(FontType::GetFontHeight()) * TargetLines);

			return MaxValue<uint8_t>(1, static_cast<uint8_t>(MinValue<uint16_t>(widthScale, heightScale)));
		}

		template<typename ParentLayout>
		using GlyphStyle = Framework::Image::TemplateImageStyle<
			Framework::Layout::AlignmentEnum::TopLeft,
			true, RGB_COLOR_BLACK,
			FontScale<ParentLayout>(), FontScale<ParentLayout>()>;

		template<typename ParentLayout>
		using Config = Modules::TerminalWindow::Definitions::TerminalConfig<
			static_cast<uint16_t>(FontType::GetFontWidth()* FontScale<ParentLayout>()),
			static_cast<uint16_t>(FontType::GetFontHeight()* FontScale<ParentLayout>())>;

		template<typename ParentLayout>
		using Layout = Modules::TerminalWindow::Layout::TerminalLayout<
			MarginLayout<ParentLayout>, FontType, GlyphStyle<ParentLayout>>;

		template<typename ParentLayout>
		using TextView = Framework::Text::Bitmask::TextView<
			typename Framework::AutoDimension::ByLayout<Layout<ParentLayout>>::dimension_t,
			Layout<ParentLayout>, FontType, GlyphStyle<ParentLayout>>;

		template<typename ParentLayout>
		using TerminalView = Modules::TerminalWindow::View::Terminal<
			Layout<ParentLayout>, TextView<ParentLayout>, Config<ParentLayout>>;

		template<typename ParentLayout, bool Monochrome = false>
		class View : public IDemoPage, public TerminalView<ParentLayout>
		{
		private:
			using Base = TerminalView<ParentLayout>;
			using Input = Modules::TerminalWindow::Input::PrintAdapter<typename Base::BufferType>;

			static constexpr size_t LineCount = sizeof(Constants::Strings::DemoLines) / sizeof(Constants::Strings::DemoLines[0]);

			Input TerminalInput;
			size_t NextLine = 0;
			uint32_t NextInterval = Constants::Terminal::IntervalBase;
			uint32_t LastLineTime = 0;

		public:
			View() : IDemoPage(), Base(), TerminalInput(Base::Buffer()) {}
			~View() = default;

			const char* GetPageName() const override { return "Terminal Window"; }

		protected:
			bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
			{
				if (NextLine < LineCount && (NextLine == 0 || frameTime - LastLineTime >= NextInterval))
				{
					TerminalInput.println(Constants::Strings::DemoLines[NextLine++]);
					LastLineTime = frameTime;
					NextInterval = Constants::Terminal::IntervalBase + (random() % Constants::Terminal::IntervalVariable);
					if (NextLine >= LineCount)
						NextLine = 0;
				}

				return Base::ViewStep(frameTime, frameCounter);
			}
		};
	}
}

#endif
