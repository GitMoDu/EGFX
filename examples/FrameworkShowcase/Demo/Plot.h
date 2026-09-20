#ifndef _DEMO_MODULES_PLOT_h
#define _DEMO_MODULES_PLOT_h

#include <IntegerGlassModules.h>

#include "../IDemoPage.h"

namespace Showcase
{
	using namespace IntegerGlass;

	namespace Plot
	{
		template<typename ParentLayout>
		using MarginLayout = Framework::Layout::Margin<
			ParentLayout,
			0,
			Constants::Showcase::Margin<ParentLayout>(),
			0,
			0>;

		template<typename ParentLayout>
		static constexpr bool IsVerticalLayout()
		{
			return (ParentLayout::Width() < ParentLayout::Height());
		}

		template<typename ParentLayout>
		using VerticalLayout = Framework::Layout::VerticalWeighted<ParentLayout, 1, 1>;

		template<typename ParentLayout>
		using HorizontalLayout = Framework::Layout::HorizontalWeighted<ParentLayout, 1, 1>;

		template<typename ParentLayout>
		using CellLayout = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
			VerticalLayout<ParentLayout>,
			HorizontalLayout<ParentLayout>,
			IsVerticalLayout<ParentLayout>()
		>::type;

		template<typename ParentLayout>
		using CircularView = Modules::Plot::Demo::DemoCircularBufferView<
			typename CellLayout<ParentLayout>::template Cell<0>>;

		template<typename ParentLayout>
		using LinearView = Modules::Plot::Demo::DemoLinearBufferView<
			typename CellLayout<ParentLayout>::template Cell<1>>;

		template<typename ParentLayout, bool Monochrome = false>
		class View : public IDemoPage, public Framework::View::CompositeView<
			ParentLayout,
			CircularView<MarginLayout<ParentLayout>>,
			LinearView<MarginLayout<ParentLayout>>
		>
		{
		private:
			using Base = Framework::View::CompositeView<
				ParentLayout,
				CircularView<MarginLayout<ParentLayout>>,
				LinearView<MarginLayout<ParentLayout>>>;

		public:
			View() : IDemoPage(), Base() {}
			~View() = default;

			const char* GetPageName() const override { return "Plot"; }
		};
	}
}

#endif
