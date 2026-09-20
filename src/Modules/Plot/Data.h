#ifndef _INTEGERGLASS_MODULES_PLOT_DATA_h
#define _INTEGERGLASS_MODULES_PLOT_DATA_h

#include "Constant.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Plot
		{
			namespace Data
			{
				template<typename dimension_t,
					count_t sampleCount>
				class LinearBuffer
				{
				public:
					static constexpr count_t SampleCount = sampleCount;

				private:
					dimension_t Samples[sampleCount]{};

				public:
					LinearBuffer() = default;
					~LinearBuffer() = default;

					void SetAll(const dimension_t value)
					{
						for (count_t i = 0; i < sampleCount; i++)
						{
							Samples[i] = value;
						}
					}

					void Clear()
					{
						SetAll(0);
					}

					void Set(const count_t i, const dimension_t value)
					{
						Samples[i % sampleCount] = value;
					}

					void CopyAll(const dimension_t* source, const count_t count)
					{
						const count_t copyCount = (count < sampleCount) ? count : sampleCount;
						for (count_t i = 0; i < copyCount; i++)
						{
							Samples[i] = source[i];
						}
					}

					dimension_t Get(const count_t i) const
					{
						return Samples[i % sampleCount];
					}
				};

				template<typename dimension_t,
					count_t sampleCount>
				class CircularExternalBuffer
				{
				public:
					static constexpr count_t SampleCount = sampleCount;

				private:
					dimension_t* Samples;
					count_t Index = 0;

				public:
					CircularExternalBuffer(dimension_t* samples)
						: Samples(samples)
					{}

					~CircularExternalBuffer() = default;

					void SetAll(const dimension_t value)
					{
						for (count_t i = 0; i < sampleCount; i++)
						{
							Samples[i] = value;
						}
					}

					void Clear()
					{
						SetAll(0);
						Index = 0;
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


				template<typename dimension_t,
					count_t sampleCount>
				class CircularBuffer : public CircularExternalBuffer<dimension_t, sampleCount>
				{
				public:
					static constexpr count_t SampleCount = sampleCount;

				private:
					using Base = CircularExternalBuffer<dimension_t, sampleCount>;

				private:
					dimension_t Samples[sampleCount]{};

				public:
					CircularBuffer() : Base(Samples) {}

					~CircularBuffer() = default;
				};
			}
		}
	}
}

#endif