#ifndef _MASTERVIEW_h
#define _MASTERVIEW_h

#include <IntegerGlassModules.h>


#include "Constants.h"

#include "Background.h"
#include "Slider.h"
#include "Title.h"

#include "Demo/BitmaskImage.h"
#include "Demo/BitmapImage.h"
#include "Demo/VectorImage.h"
#include "Demo/BitmapText.h"
#include "Demo/BitmaskText.h"
#include "Demo/VectorText.h"
#include "Demo/VectorIcon.h"


#include "Demo/Timecode.h"
#include "Demo/BatteryIndicator.h"
#include "Demo/ProgressIndicator.h"
#include "Demo/ProgressIndeterminate.h"
#include "Demo/Plot.h"
#include "Demo/TerminalWindow.h"

namespace Showcase
{
	namespace MasterView
	{
		template<typename ParentLayout>
		using ItemLayout = typename Constants::Showcase::Projector<ParentLayout>::ItemLayouter::ViewLayout;

		template<typename ParentLayout>
		using IntroLayout = Framework::Layout::Margin<
			ParentLayout,
			Constants::Showcase::Margin<ParentLayout>(),
			Constants::Showcase::Margin<ParentLayout>(),
			Constants::Showcase::Margin<ParentLayout>(),
			Constants::Showcase::Margin<ParentLayout>()>;

		template<typename ParentLayout, bool Monochrome = false>
		using GraphicsView = Framework::Projection::Projectors::PageSlide::View<
			ParentLayout,
			Constants::Showcase::PageSlideStyle<ParentLayout>,
			BitmapImage::View<ItemLayout<ParentLayout>, Monochrome>,
			BitmaskImage::View<ItemLayout<ParentLayout>, Monochrome>,
			VectorImage::View<ItemLayout<ParentLayout>, Monochrome>,
			VectorIcon::View<ItemLayout<ParentLayout>, Monochrome>,
			BitmaskText::View<ItemLayout<ParentLayout>, Monochrome>,
			BitmapText::View<ItemLayout<ParentLayout>, Monochrome>,
			VectorText::View<ItemLayout<ParentLayout>, Monochrome>
		>;

		template<typename ParentLayout, bool Monochrome = false>
		using GraphicsSliderView = SliderView<ParentLayout, GraphicsView<ParentLayout, Monochrome>, Monochrome>;

		template<typename ParentLayout, bool Monochrome = false>
		using ModulesView = Framework::Projection::Projectors::PageSlide::View<
			ParentLayout,
			Constants::Showcase::PageSlideStyle<ParentLayout>,
			ProgressIndicator::View<ItemLayout<ParentLayout>, Monochrome>,
			ProgressIndeterminate::View<ItemLayout<ParentLayout>, Monochrome>,
			Plot::View<ItemLayout<ParentLayout>, Monochrome>,
			TerminalWindow::View<ItemLayout<ParentLayout>, Monochrome>,
			Timecode::View<ItemLayout<ParentLayout>, Monochrome>,
			BatteryIndicator::View<ItemLayout<ParentLayout>, Monochrome>
		>;

		template<typename ParentLayout, bool Monochrome = false>
		using ModulesSliderView = SliderView<ParentLayout, ModulesView<ParentLayout, Monochrome>, Monochrome>;

	
		//template<typename ParentLayout, bool Monochrome = false>
		//using IntroView = Modules::IntegerGlassLogo::View::SplashLogo<
		//	IntroLayout<ParentLayout>,
		//	Monochrome,
		//	Constants::Showcase::SplashDuration,
		//	false,
		//	false>;


		template<typename ParentLayout, bool Monochrome = false>
		using IntroView = Modules::Logo::View::SplashLogo<
			IntroLayout<ParentLayout>,
			Monochrome,
			Constants::Showcase::SplashDuration,
			false,
			false>;

		template<typename ParentLayout, bool Monochrome = false>
		using ContentSelectorView = Framework::View::SelectorView<
			ParentLayout,
			Title::View<ParentLayout, Monochrome>,
			GraphicsSliderView<ParentLayout, Monochrome>,
			ModulesSliderView<ParentLayout, Monochrome>>;


		template<typename ParentLayout, bool Monochrome = false>
		using MasterViewType = Framework::View::SelectorView<
			ParentLayout,
			IntroView<ParentLayout, Monochrome>,
			Framework::View::CompositeView<
			ParentLayout,
			BackgroundView<ParentLayout, Monochrome>,
			ContentSelectorView<ParentLayout, Monochrome>
			>
		>;

		template<typename ParentLayout, bool Monochrome = false>
		class MasterView : public MasterViewType<ParentLayout, Monochrome>
		{
		private:
			using Base = MasterViewType<ParentLayout, Monochrome>;

			enum class StateEnum : uint8_t
			{
				Boot,
				Intro,
				IntroDelay,
				SplashEndDelay,
				GraphicsTitle,
				GraphicsShowcase,
				ModulesTitle,
				ModulesShowcase,
				Outro,
				Repeat
			};

			uint32_t StageStart = 0;

			StateEnum State = StateEnum::Boot;

		public:
			MasterView() : Base()
			{}
			~MasterView() = default;

		protected:
			bool ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
			{
				switch (State)
				{
				case StateEnum::Boot:
					StageStart = frameTime;
					State = StateEnum::Intro;
					Base::SetSelectedView(0);
					break;
				case StateEnum::Intro:
					if (frameTime - StageStart >= Constants::Showcase::SplashStartDelay)
					{

						Base::template view<0>().Start();
						StageStart = frameTime;
						State = StateEnum::IntroDelay;
					}
					break;
				case StateEnum::IntroDelay:
					if (frameTime - StageStart >= Constants::Showcase::SplashDuration)
					{
						StageStart = frameTime;
						State = StateEnum::SplashEndDelay;
						Background().FadeIn();
					}
					break;
				case StateEnum::SplashEndDelay:
					if (frameTime - StageStart >= Constants::Showcase::SplashEndDelay)
					{
						Base::SetSelectedView(1);
						Content().SetSelectedView(0);
						Title().SetTitle(Constants::Strings::GraphicsTitle, Constants::Showcase::TitleDuration);
						StageStart = frameTime;
						State = StateEnum::GraphicsTitle;
					}
					break;
				case StateEnum::GraphicsTitle:
					if (frameTime - StageStart >= Constants::Showcase::TitleDuration)
					{
						StartGraphics(frameTime);
						StageStart = frameTime;
						State = StateEnum::GraphicsShowcase;
					}
					break;
				case StateEnum::GraphicsShowcase:
					if (GraphicsSlider().IsReady())
					{
						Base::SetSelectedView(1);
						Content().SetSelectedView(0);
						Title().SetTitle(Constants::Strings::ModulesTitle, Constants::Showcase::TitleDuration);
						StageStart = frameTime;
						State = StateEnum::ModulesTitle;
					}
					break;
				case StateEnum::ModulesTitle:
					if (frameTime - StageStart >= Constants::Showcase::TitleDuration)
					{
						StartModules(frameTime);
						StageStart = frameTime;
						State = StateEnum::ModulesShowcase;
					}
					break;
				case StateEnum::ModulesShowcase:
					if (ModulesSlider().IsReady())
					{
						Base::SetSelectedView(1);
						StageStart = frameTime;
						State = StateEnum::Outro;
						Background().FadeOut();
					}
					break;
				case StateEnum::Outro:
					if (frameTime - StageStart >= Constants::Showcase::OutroDuration)
					{
						State = StateEnum::Repeat;
					}
					break;
				case StateEnum::Repeat:
					StageStart = frameTime;
					State = StateEnum::Intro;
					Base::SetSelectedView(0);
					break;
				default:
					break;
				}

				return true;
			}

		private:
			auto& Background()
			{
				return Base::template view<1>().template view<0>();
			}

			auto& Content()
			{
				return Base::template view<1>().template view<1>();
			}

			auto& Title()
			{
				return Content().template view<0>();
			}

			auto& GraphicsSlider()
			{
				return Content().template view<1>();
			}

			auto& ModulesSlider()
			{
				return Content().template view<2>();
			}

			void StartGraphics(const uint32_t frameTime)
			{
				auto& content = Content();
				content.SetSelectedView(1);
				GraphicsSlider().Start();
				Base::SetSelectedView(1);
			}

			void StartModules(const uint32_t frameTime)
			{
				auto& content = Content();
				content.SetSelectedView(2);
				ModulesSlider().Start();
				Base::SetSelectedView(1);
			}
		};
	}
}


#endif

