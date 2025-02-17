// ITransform.h

#ifndef _EGFX_I_TRANSFORM_h
#define _EGFX_I_TRANSFORM_h

#include <stdint.h>

namespace Egfx
{
	/// <summary>
	/// Template-chainable Coordinate transformer interface.
	/// </summary>
	struct ITransform
	{
		virtual const bool Transform(pixel_t& x, pixel_t& y)
		{
			return true;
		}
	};
}
#endif