// ISprite.h

#ifndef _I_SPRITE_h
#define _I_SPRITE_h

#include <stdint.h>
#include "RgbColor.h"

class ISprite
{
public:
	virtual const uint8_t GetWidth() { return 0; }
	virtual const uint8_t GetHeight() { return 0; }

	/// <summary>
	/// Color getter for sprite.
	/// </summary>
	/// <param name="color">Color to be written.</param>
	/// <param name="x">Sprite coordinate X.</param>
	/// <param name="y">Sprite coordinate Y.</param>
	/// <returns>False when coordinates are transparent.</returns>
	virtual const bool Get(RgbColor& color, const uint8_t x, const uint8_t y) { return false; }
};

template<const uint8_t width,
	const uint8_t height>
class AbstractSprite : public ISprite
{
public:
	static constexpr uint8_t Width = width;
	static constexpr uint8_t Height = height;

public:
	AbstractSprite() : ISprite()
	{}

	const uint8_t GetWidth() final
	{
		return Width;
	}

	const uint8_t GetHeight() final
	{
		return Height;
	}
};

#endif