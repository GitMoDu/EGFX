// IFrame.h

#ifndef _I_FRAME_h
#define _I_FRAME_h

#include <stdint.h>
#include "RgbColor.h"

template<typename pixel_t>
class IColorConverter
{
public:
	virtual const pixel_t GetRawColor(const RgbColor& color) { return 0; }
};

class IFrame
{
public:
	enum class FrameRotationEnum
	{
		NoRotation,
		Plus90,
		Minus90
	};

	enum class DisplayAxisEnum
	{
		NoMirror,
		MirrorX,
		MirrorY,
		MirrorXY
	};


public:
	template<typename pixel_t>
	static constexpr uint32_t GetBufferSize(const uint8_t width, const uint8_t height)
	{
		return (uint32_t)width * height * sizeof(pixel_t);
	}

public:
	virtual const uint8_t GetWidth() { return 0; }
	virtual const uint8_t GetHeight() { return 0; }

public:
	const uint16_t GetFrameSize()
	{
		return (uint16_t)GetWidth() * GetHeight();
	}
};
#endif