#ifndef _ASSETS_h
#define _ASSETS_h

#include "Sprites.h"

namespace Assets
{
	using namespace Egfx;

	namespace Strings
	{
		static constexpr char ShortFox1[] PROGMEM = "The quick brown";
		static constexpr char ShortFox2[] PROGMEM = "fox jumps over";
		static constexpr char ShortFox3[] PROGMEM = "the lazy Doge";
	}


	struct HeartSprite : public FlashBitMaskSprite<Heart::Width, Heart::Height>
	{
		HeartSprite() : FlashBitMaskSprite<Heart::Width, Heart::Height>(Heart::Mask)
		{
		}
	};

	struct HeartBigSprite : public FlashBitMaskSprite<HeartBig::Width, HeartBig::Height>
	{
		HeartBigSprite() : FlashBitMaskSprite<HeartBig::Width, HeartBig::Height>(HeartBig::Mask)
		{
		}
	};

	struct DogeBitSprite : public FlashBitMaskSprite<DogeBit::Width, DogeBit::Height>
	{
		DogeBitSprite() : FlashBitMaskSprite<DogeBit::Width, DogeBit::Height>(DogeBit::Mask)
		{
		}
	};

	struct DogeSprite : public FlashBitmapRgb565Sprite<Doge565::Width, Doge565::Height>
	{
		DogeSprite() : FlashBitmapRgb565Sprite<Doge565::Width, Doge565::Height>(Doge565::Bitmap)
		{
		}
	};

	template<bool useBig>
	struct HeartSelect;
	template<> struct HeartSelect<true> { using type = HeartBigSprite; };
	template<> struct HeartSelect<false> { using type = HeartSprite; };
	
}

#endif

