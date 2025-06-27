#ifndef _EGFX_I_SPRITE_h
#define _EGFX_I_SPRITE_h

#include "RgbColor.h"

namespace Egfx
{
	struct ISprite
	{
		/// <summary>
		/// Sprite width.
		/// </summary>
		/// <returns></returns>
		virtual pixel_t GetWidth() const { return 0; }

		/// <summary>
		/// Sprite height.
		/// </summary>
		/// <returns></returns>
		virtual pixel_t GetHeight() const { return 0; }

		/// <summary>
		/// Color getter for sprite.
		/// </summary>
		/// <param name="color">Color to be written.</param>
		/// <param name="x">Sprite coordinate X.</param>
		/// <param name="y">Sprite coordinate Y.</param>
		/// <returns>False when pixel is transparent at these coordinates.</returns>
		virtual bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y) { return false; }
	};

	/// <summary>
	/// Templated implementation of sprite dimensions only.
	/// </summary>
	/// <typeparam name="width">Sprite width.</typeparam>
	/// <typeparam name="height">Sprite height.</typeparam>
	template<const pixel_t width,
		const pixel_t height>
	class AbstractSprite : public ISprite
	{
	public:
		static constexpr pixel_t Width = width;
		static constexpr pixel_t Height = height;

	public:
		AbstractSprite() : ISprite()
		{
		}

		virtual pixel_t GetWidth() const
		{
			return Width;
		}

		virtual pixel_t GetHeight() const
		{
			return Height;
		}
	};
}
#endif