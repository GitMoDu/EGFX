#ifndef _DEMO_MODULES_PROGRESS_INDETERMINATE_h
#define _DEMO_MODULES_PROGRESS_INDETERMINATE_h

#include <IntegerGlassModules.h>

#include "../IDemoPage.h"
#include "../Constants.h"

namespace Showcase
{
	using namespace IntegerGlass;

	namespace ProgressIndeterminate
	{
		template<typename ParentLayout>
		static constexpr int16_t LineHeight()
		{
			return MaxValue<int16_t>(1, ParentLayout::Height() / 32);
		}

		template<typename ParentLayout>
		static constexpr int16_t LineMargin()
		{
			return MaxValue<int16_t>(1, ParentLayout::Height() / 8);
		}

		template<typename ParentLayout>
		using MarginLayout = Framework::Layout::Margin<
			ParentLayout,
			0,
			Constants::Showcase::Margin<ParentLayout>(),
			0,
			0>;

		template<typename ParentLayout>
		using LineRawLayout = Framework::TemplateLayout<
			ParentLayout::X(),
			ParentLayout::Y(),
			ParentLayout::Width(),
			LineHeight<ParentLayout>()>;

		template<typename ParentLayout>
		using LineLayout = Framework::Layout::Translate<
			LineRawLayout<ParentLayout>,
			0,
			ParentLayout::Height() - Constants::Showcase::StripHeight<ParentLayout>() - LineHeight<ParentLayout>()>;

		template<typename ParentLayout>
		using CircularLayout = Framework::Layout::Align<
			ParentLayout,
			Framework::Layout::Margin<
			Framework::Layout::ConstrainedRatio<ParentLayout, 1, 1>,
			LineMargin<ParentLayout>(),
			LineMargin<ParentLayout>(),
			LineMargin<ParentLayout>(),
			LineMargin<ParentLayout>() + LineHeight<ParentLayout>()>,
			Framework::Layout::AlignmentEnum::TopCenter>;

		template<typename ParentLayout, bool Monochrome>
		using CircularView = Modules::ProgressIndeterminate::Circular::Demo::DemoView<
			CircularLayout<ParentLayout>, Monochrome>;

		template<typename ParentLayout, bool Monochrome>
		using LinearView = Modules::ProgressIndeterminate::Linear::Demo::DemoView<
			LineLayout<ParentLayout>, Monochrome>;

		template<typename ParentLayout, bool Monochrome = false>
		class View : public IDemoPage, public Framework::View::CompositeView<
			ParentLayout,
			CircularView<MarginLayout<ParentLayout>, Monochrome>,
			LinearView<MarginLayout<ParentLayout>, Monochrome>>
		{
		private:
			using Base = Framework::View::CompositeView<
				ParentLayout,
				CircularView<MarginLayout<ParentLayout>, Monochrome>,
				LinearView<MarginLayout<ParentLayout>, Monochrome>>;

		public:
			View() : IDemoPage(), Base() {}
			~View() = default;
			const char* GetPageName() const override { return "Progress Indeterminate"; }
		};
	}
}

#endif
