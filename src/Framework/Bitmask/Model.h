#ifndef _INTEGERGLASS_FRAMEWORK_BITMASK_MODEL_h
#define _INTEGERGLASS_FRAMEWORK_BITMASK_MODEL_h

#include "../../IntegerGlassCore.h"

namespace IntegerGlass
{
	namespace Framework
	{
		/// <summary>
		/// Bitmask graphics model.
		///
		/// Bitmasks are 1-bit-per-pixel images stored as packed bytes. Provides basic
		/// constants and readers for accessing bitmask data from RAM or flash/PROGMEM.
		/// </summary>
		namespace Bitmask
		{
			/// <summary>Number of bits stored in a single byte of bitmask data.</summary>
			static constexpr uint8_t BitsPerByte = 8;

			namespace Contract
			{
				struct StaticBitmaskSource
				{
					using dimension_t = uint8_t;
					static constexpr dimension_t Width = 0;
					static constexpr dimension_t Height = 0;

					static constexpr Framework::DataSourceTypeEnum DataSourceType = Framework::DataSourceTypeEnum::Ram;

					static const uint8_t* GetBitmask()
					{
						return nullptr;
					}
				};
			}
		}
	}
}
#endif