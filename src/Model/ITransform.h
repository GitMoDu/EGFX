// ITransform.h

#ifndef _I_TRANSFORM_h
#define _I_TRANSFORM_h

#include <stdint.h>

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
#endif