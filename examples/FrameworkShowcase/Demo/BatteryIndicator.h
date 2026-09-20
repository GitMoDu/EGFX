#ifndef _DEMO_MODULES_BATTERY_INDICATOR_h
#define _DEMO_MODULES_BATTERY_INDICATOR_h

#include <IntegerGlassModules.h>

#include "../IDemoPage.h"
#include "../Constants.h"

namespace Showcase
{
	using namespace IntegerGlass;

	namespace BatteryIndicator
	{
		template<typename ParentLayout>
		using MarginLayout = Framework::Layout::Margin<
			ParentLayout,
			0,
			Constants::Showcase::Margin<ParentLayout>(),
			0,
			0>;

		template<typename ParentLayout>
		using AspectAlignedLayout = Framework::Layout::Align <
			ParentLayout,
			Framework::Layout::ConstrainedRatio<MarginLayout<ParentLayout>, Constants::BatteryIndicator::AspectRatioWidth, Constants::BatteryIndicator::AspectRatioHeight * 2>,
			Framework::Layout::AlignmentEnum::MiddleCenter>;

		template<typename ParentLayout>
		using SplitLayout = Framework::Layout::Grid<AspectAlignedLayout<ParentLayout>, 1, 2>;

		template<typename ParentLayout>
		using TopLayout = Framework::Layout::Margin<
			typename SplitLayout<ParentLayout>::template Cell<0, 0>,
			0, Constants::BatteryIndicator::VerticalMargin, 0, Constants::BatteryIndicator::VerticalMargin>;

		template<typename ParentLayout>
		using BottomLayout = Framework::Layout::Margin<
			typename SplitLayout<ParentLayout>::template Cell<0, 1>,
			0, Constants::BatteryIndicator::VerticalMargin, 0, Constants::BatteryIndicator::VerticalMargin>;

		template<bool Monochrome>
		using VariationStyle = Modules::BatteryIndicator::TemplateBatteryStyle<
			Monochrome,
			Modules::BatteryIndicator::OrientationEnum::LeftToRight,
			3,
			Modules::BatteryIndicator::Constants::DefaultChargeAnimationPeriod / 2,
			1,
			Modules::BatteryIndicator::Constants::DefaultInnerMargin * 0,
			Modules::BatteryIndicator::Constants::DefaultHeadHeight / 2,
			Modules::BatteryIndicator::Constants::DefaultHeadWidth / 2,
			Modules::BatteryIndicator::Constants::DefaultAlignment>;

		template<typename ParentLayout, bool Monochrome = false>
		class View : public IDemoPage, public Framework::View::CompositeView<
			ParentLayout,
			Modules::BatteryIndicator::Demo::DemoView<TopLayout<ParentLayout>, Monochrome>,
			Modules::BatteryIndicator::Demo::DemoView<BottomLayout<ParentLayout>, Monochrome, VariationStyle<Monochrome>>>
		{
		private:
			using Base = Framework::View::CompositeView<
				ParentLayout,
				Modules::BatteryIndicator::Demo::DemoView<TopLayout<ParentLayout>, Monochrome>,
				Modules::BatteryIndicator::Demo::DemoView<BottomLayout<ParentLayout>, Monochrome, VariationStyle<Monochrome>>>;

		public:
			View() : IDemoPage(), Base() {}
			~View() = default;

			const char* GetPageName() const override { return "Battery Indicator"; }
		};
	}
}

#endif
