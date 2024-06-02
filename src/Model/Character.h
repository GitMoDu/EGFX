// Character.h

#ifndef _CHARACTER_h
#define _CHARACTER_h

#include <stdint.h>

namespace Character
{
	static constexpr int8_t NumbersStart = '0';
	static constexpr int8_t Space = ' ';
	static constexpr int8_t NewLine = '\n';
	static constexpr int8_t Tab = '\t';
	static constexpr int8_t Minus = '-';
	static constexpr int8_t Break = (int8_t)'\0';

	static constexpr uint8_t NumbersCount = 10;
	static constexpr int8_t NumbersEnd = NumbersStart + NumbersCount;
}
#endif