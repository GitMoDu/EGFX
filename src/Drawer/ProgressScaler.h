#ifndef _EGFX_PROGRESS_SCALER_h
#define _EGFX_PROGRESS_SCALER_h

#include <stdint.h>

namespace Egfx
{
	struct ProgressScaler
	{
		template<const uint32_t PeriodMicros>
		static constexpr const uint16_t GetProgress(const uint32_t frameTime)
		{
			return ((uint64_t)(frameTime % PeriodMicros) * UINT16_MAX) / PeriodMicros;
		}

		static constexpr const uint16_t GetProgress(const uint32_t periodMicros, const uint32_t frameTime)
		{
			return uint16_t(((uint64_t)(frameTime % periodMicros) * UINT16_MAX) / periodMicros);
		}

		static constexpr uint8_t ScaleProgress(const uint16_t progress, const uint8_t value)
		{
			return uint8_t(ScaleProgress(progress, (uint16_t)value));
		}

		static constexpr uint16_t ScaleProgress(const uint16_t progress, const uint16_t value)
		{
			return uint16_t((((uint32_t)progress) * value) / UINT16_MAX);
		}

		static constexpr uint16_t ScaleProgress(const uint16_t progress, const uint32_t value)
		{
			return uint16_t((((uint64_t)progress) * value) / UINT16_MAX);
		}

		static const uint16_t TriangleResponse(const uint16_t progress)
		{
			if (progress < ((uint16_t)INT16_MAX + 1))
			{
				return progress * 2;
			}
			else
			{
				return UINT16_MAX - (progress * 2);
			}
		}
	};
}
#endif