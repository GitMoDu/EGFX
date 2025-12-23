#ifndef _DISPLAY_SYNC_TYPE_h
#define _DISPLAY_SYNC_TYPE_h

#include <stdint.h>

namespace Egfx
{
	enum class DisplaySyncType : uint8_t
	{
		/// <summary>
		/// Sync with frame-rate multiples going down to 1/2, 1/4 and below to keep frame-pace.
		/// </summary>
		VSync,
		/// <summary>
		/// Uneven display sync with variable frame-rate to adjust for low frame-rates.
		/// </summary>
		Vrr,
		/// <summary>
		/// Render and display as fast as possible.
		/// </summary>
		NoSync
	};
}
#endif