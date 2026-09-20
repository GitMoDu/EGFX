#ifndef _SHOWCASE_I_DEMO_PAGE_h
#define _SHOWCASE_I_DEMO_PAGE_h

#include <stdint.h>

namespace Showcase
{
	struct IDemoPage
	{
		IDemoPage() = default;
		virtual ~IDemoPage() = default;

		virtual const char* GetPageName() const = 0;
	};
}
#endif