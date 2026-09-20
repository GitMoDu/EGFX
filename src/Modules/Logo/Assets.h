#ifndef _INTEGERGLASS_MODULES_LOGO_ASSETS_h
#define _INTEGERGLASS_MODULES_LOGO_ASSETS_h

#include "../../IntegerGlassFramework.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Logo
		{
			namespace Assets
			{
				namespace IntegerLabel
				{

					namespace Source
					{
						static constexpr uint8_t Mask[] PROGMEM
						{
							0x80, 0x80, 0x00, 0x00, // # . . . . . . . # . . . . . . . . . . . . . . . . . . . .
							0x99, 0xcc, 0x63, 0x28, // # . . # # . . # # # . . # # . . . # # . . . # # . . # . #
							0xa4, 0x92, 0x94, 0xb0, // # . # . . # . . # . . # . . # . # . . # . # . . # . # # .
							0xa4, 0x9e, 0x97, 0xa0, // # . # . . # . . # . . # # # # . # . . # . # # # # . # . .
							0xa4, 0x90, 0x74, 0x20, // # . # . . # . . # . . # . . . . . # # # . # . . . . # . .
							0xa4, 0xce, 0x13, 0xa0, // # . # . . # . . # # . . # # # . . . . # . . # # # . # . .
							0x00, 0x00, 0x90, 0x00, // . . . . . . . . . . . . . . . . # . . # . . . . . . . . .
							0x00, 0x00, 0x60, 0x00  // . . . . . . . . . . . . . . . . . # # . . . . . . . . . .
						};
					}


					struct SourceType
					{
						static constexpr ::IntegerGlass::Framework::DataSourceTypeEnum DataSourceType = ::IntegerGlass::Framework::DataSourceTypeEnum::Flash;
						static constexpr pixel_t Width = 29;
						static constexpr pixel_t Height = 8;

						static const uint8_t* GetBitmask()
						{
							return Source::Mask;
						}
					};
				}
			}
		}
	}
}
#endif