// DisplayOrientation.h

#ifndef _DISPLAY_ORIENTATION_h
#define _DISPLAY_ORIENTATION_h

#include <stdint.h>

namespace Egfx
{
	enum class DisplayRotationEnum
	{
		NoRotation,
		Plus90,
		Minus90
	};

	enum class DisplayMirrorEnum
	{
		NoMirror,
		MirrorX,
		MirrorY,
		MirrorXY
	};
}
#endif