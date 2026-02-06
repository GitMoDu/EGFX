#ifndef _EGFX_MODULES_EGFX_LOGO_SHADER_DITHER_h
#define _EGFX_MODULES_EGFX_LOGO_SHADER_DITHER_h

#include <EgfxCore.h>

namespace Egfx
{
	namespace Modules
	{
		namespace EgfxLogo
		{
			namespace Shader
			{
				namespace Dither
				{
					/// <summary>
					/// Simple statefull RNG-based ditherer, controlled via chance (0-255).
					/// </summary>
					class RngDitherer
					{
					private:
						uint16_t Rng = 67;

					public:
						RngDitherer() = default;
						~RngDitherer() = default;

						bool Dither(const uint8_t chance)
						{
							Rng ^= Rng << 7;
							Rng ^= Rng >> 9;
							Rng ^= Rng << 8;

							return chance == 0 ? false : chance >= static_cast<uint8_t>(Rng);
						}
					};

				}
			}
		}
	}
}
#endif