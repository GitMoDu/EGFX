#ifndef _DEMO_MODULES_TIMECODE_h
#define _DEMO_MODULES_TIMECODE_h

#include <IntegerGlassModules.h>

#include "../IDemoPage.h"
#include "../Constants.h"

namespace Showcase
{
	using namespace IntegerGlass;

	namespace Timecode
	{
		template<typename ParentLayout>
		using MarginLayout = Framework::Layout::Margin<
			ParentLayout,
			0,
			Constants::Showcase::Margin<ParentLayout>(),
			0,
			0>;


		template<typename ParentLayout, bool Monochrome = false>
		class View : public IDemoPage, public Modules::Timecode::Demo::DemoView<MarginLayout<ParentLayout>, Monochrome>
		{
		private:
			using Base = Modules::Timecode::Demo::DemoView<MarginLayout<ParentLayout>, Monochrome>;

		public:
			View() : IDemoPage(), Base() {}
			~View() = default;

			const char* GetPageName() const override { return "Timecode"; }
		};
	}
}

#endif

