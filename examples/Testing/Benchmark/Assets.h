// Assets.h

#ifndef _ASSETS_h
#define _ASSETS_h

#include <ArduinoGraphicsDrawer.h>

namespace Assets
{
	namespace Texts
	{
		static constexpr char TestText[]{ "DOGE" };

	}
	namespace SpriteSource
	{
		namespace Grid
		{
			static constexpr uint8_t Mask[] PROGMEM
			{
				0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55,
				0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55
			};

			static constexpr uint8_t Width = 16;
			static constexpr uint8_t Height = 16;
		}

		namespace Grid565
		{
			static constexpr uint16_t Bitmap[] PROGMEM
			{
				0xffff, 0xf800, 0xffff, 0xf800, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000,
				0xf800, 0xffff, 0xf800, 0xffff, 0xf800, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff,
				0xffff, 0xf800, 0xffff, 0xf800, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x07e4,
				0xf800, 0xffff, 0xf800, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x07e4, 0xffff,
				0xffff, 0xf800, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x07e4, 0xffff, 0x07e4,
				0xf800, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0xdfe0, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x07e4, 0xffff,
				0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0xf800, 0xffff, 0xdfe0, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x07e4,
				0x0000, 0xffff, 0x0000, 0xffff, 0xf800, 0xffff, 0x00bf, 0xffff, 0xdfe0, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff,
				0xffff, 0x0000, 0xffff, 0xf800, 0xffff, 0x00bf, 0xffff, 0xf800, 0xffff, 0xdfe0, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000,
				0x0000, 0xffff, 0xf800, 0xffff, 0x00bf, 0xffff, 0xf800, 0xffff, 0x0000, 0xffff, 0x07e4, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff,
				0xffff, 0x881f, 0xffff, 0x00bf, 0xffff, 0xf800, 0xffff, 0x0000, 0xffff, 0x00bf, 0xffff, 0x07e4, 0xffff, 0x0000, 0xffff, 0x0000,
				0x077f, 0xffff, 0x881f, 0xffff, 0xf800, 0xffff, 0x0000, 0xffff, 0x00bf, 0xffff, 0x00bf, 0xffff, 0x07e4, 0xffff, 0x0000, 0xffff,
				0xffff, 0x077f, 0xffff, 0x881f, 0xffff, 0x0000, 0xffff, 0x00bf, 0xffff, 0x07e4, 0xffff, 0x00bf, 0xffff, 0x0000, 0xffff, 0x0000,
				0x0000, 0xffff, 0x077f, 0xffff, 0x881f, 0xffff, 0x07e4, 0xffff, 0x00bf, 0xffff, 0x07e4, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff,
				0xffff, 0x0000, 0xffff, 0x077f, 0xffff, 0x881f, 0xffff, 0x07e4, 0xffff, 0x00bf, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000,
				0x0000, 0xffff, 0x0000, 0xffff, 0x077f, 0xffff, 0x881f, 0xffff, 0x07e4, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff
			};

			static constexpr uint8_t Width = Grid::Width;
			static constexpr uint8_t Height = Grid::Height;
		}
	}

	struct GridMaskSprite : public FlashBitMaskSprite<SpriteSource::Grid::Width, SpriteSource::Grid::Height>
	{
		GridMaskSprite() : FlashBitMaskSprite<SpriteSource::Grid::Width, SpriteSource::Grid::Height>(SpriteSource::Grid::Mask) {}
	};

	struct GridColorSprite : public FlashBitmapRgb565Sprite<SpriteSource::Grid::Width, SpriteSource::Grid::Height>
	{
		GridColorSprite() : FlashBitmapRgb565Sprite<SpriteSource::Grid::Width, SpriteSource::Grid::Height>(SpriteSource::Grid565::Bitmap) {}
	};
}


#endif

