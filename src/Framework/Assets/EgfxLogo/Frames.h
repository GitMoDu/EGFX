#ifndef _EGFX_FRAMEWORK_ASSETS_EGFX_LOGO_FRAMES_h
#define _EGFX_FRAMEWORK_ASSETS_EGFX_LOGO_FRAMES_h

#include <EgfxDrawer.h>
#include "Views.h"
#include "Layout.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace EgfxLogo
			{
				namespace Frames
				{
					template<typename ParentLayout, bool Monochrome,
						uint32_t AnimationDuration = 4000000,
						bool AutoStart = true>
					struct FrameStatic : Framework::View::FrameAdapter
						<Framework::Assets::EgfxLogo::Views::Splash<ParentLayout, Monochrome>>
					{
					};

					template<typename ParentLayout, bool Monochrome,
						uint32_t AnimationDuration = 4000000,
						bool AutoStart = true>
					struct FrameAnimated : Framework::View::FrameAdapter
						<Framework::Assets::EgfxLogo::Views::AnimatedSplash<ParentLayout, Monochrome, AnimationDuration, AutoStart>>
					{
					};
				}
			}
		}
	}
}
#endif