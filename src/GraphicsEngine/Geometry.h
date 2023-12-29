// Geometry.h

#ifndef _GEOMETRY_h
#define _GEOMETRY_h

#include <stdint.h>

class Geometry
{
private:
	static constexpr uint8_t Root2Remainder8 = 105;

public:
	/// <summary>
	/// Approximates multiplying by sqrt(2)
	/// </summary>
	/// <param name="radius"></param>
	/// <returns>sqrt(2) x radius</returns>
	static constexpr uint8_t GetSquareInsideCircleDimension(const uint8_t radius)
	{
		return ((uint16_t)radius - 1) + (((uint16_t)radius * Root2Remainder8) / UINT8_MAX);
	}
};
#endif