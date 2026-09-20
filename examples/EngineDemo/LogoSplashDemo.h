#ifndef _LOGO_SPLASH_DEMO_h
#define _LOGO_SPLASH_DEMO_h

#include <IntegerGlassModules.h>

// Logo splash screen demo with custom renderer and auto-layout.
namespace LogoSplashDemo
{
	using namespace IntegerGlass;

	template<typename ParentLayout, bool Monochrome,
		uint32_t AnimationDuration = 4000000>
	struct StaticView : Modules::Logo::View::Logo<ParentLayout, Monochrome>
	{
		StaticView() : Modules::Logo::View::Logo<ParentLayout, Monochrome>() {}
		~StaticView() = default;

#if defined(SERIAL_LOG)
		void PrintDescription() const
		{
			Serial.print(F("Logo Splash\n\tCodeFont with auto-layout and custom shaders"));
		}
#endif
	};

	template<typename ParentLayout, bool Monochrome,
		uint32_t AnimationDuration = 8000000,
		bool AutoStart = true,
		bool Loop = true>
	struct AnimatedView : Modules::Logo::View::SplashLogo<ParentLayout, Monochrome, AnimationDuration, AutoStart, Loop>
	{
		AnimatedView() : Modules::Logo::View::SplashLogo<ParentLayout, Monochrome, AnimationDuration, AutoStart, Loop>() {}
		~AnimatedView() = default;

#if defined(SERIAL_LOG)
		void PrintDescription() const
		{
			Serial.print(F("Logo Splash\n\tCodeFont with auto-layout and custom shaders"));
		}
#endif
	};
}
#endif