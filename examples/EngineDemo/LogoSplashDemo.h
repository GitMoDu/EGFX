#ifndef _LOGO_SPLASH_DEMO_h
#define _LOGO_SPLASH_DEMO_h

#include <EgfxDrawers.h>

// Logo splash screen demo with custom renderer and auto-layout.
namespace LogoSplashDemo
{
	using namespace Egfx;

	template<typename ParentLayout, bool Monochrome,
		uint32_t AnimationDuration = 4000000,
		bool AutoStart = true>
	struct Frame : Framework::View::FrameAdapter
		<Framework::Assets::EgfxLogo::Views::AnimatedSplash<ParentLayout, Monochrome, AnimationDuration, AutoStart>>
	{
#if defined(SERIAL_LOG)
		void PrintDescription() const
		{
			Serial.print(F("Logo Splash\n\tCodeFont with auto-layout and custom shaders"));
		}
#endif
	};
}
#endif