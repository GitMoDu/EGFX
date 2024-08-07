// ITransform.h

#ifndef _I_TRANSFORM_h
#define _I_TRANSFORM_h

#include <stdint.h>

/// <summary>
/// Template-chainable Coordinate transformer interface.
/// </summary>
struct ITransform
{
	virtual const bool Transform(uint8_t& x, uint8_t& y)
	{
		return true;
	}
};
#endif