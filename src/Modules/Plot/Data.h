#ifndef _EGFX_MODULES_PLOT_DATA_h
#define _EGFX_MODULES_PLOT_DATA_h

#include "Constant.h"

namespace Egfx
{
	namespace Modules
	{
		namespace Plot
		{
			namespace Data
			{
				template<typename dimension_t,
					count_t sampleCount>
				class CircularBuffer
				{
				private:
					dimension_t Samples[sampleCount]{};
					count_t Index = 0;

				public:
					CircularBuffer() = default;
					~CircularBuffer() = default;

					void SetAll(const dimension_t value)
					{
						for (count_t i = 0; i < sampleCount; i++)
						{
							Samples[i] = value;
						}
					}

					void Push(const dimension_t newValue)
					{
						Samples[Index] = newValue;
						Index = (Index + 1) % sampleCount;
					};

					dimension_t Get(const count_t i) const
					{
						return Samples[(Index + i) % sampleCount];
					}
				};
			}
		}
	}
}

#endif