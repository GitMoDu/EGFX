#ifndef _EGFX_FRAMEWORK_LAYOUT_WEIGHTED_h
#define _EGFX_FRAMEWORK_LAYOUT_WEIGHTED_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Layout
		{
			/// <summary>
			/// Splits a parent layout into a horizontal sequence of weighted cells.
			/// Note: total width may be less than parent width due to integer division truncation.
			/// </summary>
			/// <typeparam name="ParentLayout">Parent layout providing X/Y/Width/Height.</typeparam>
			/// <typeparam name="...Weights">Cell weights (uint8_t pack).</typeparam>
			template<typename ParentLayout, uint8_t... Weights>
			struct HorizontalWeighted
			{
				static_assert(sizeof...(Weights) > 0, "HorizontalWeightedLayout requires at least one weight.");
				static_assert(sizeof...(Weights) <= 255, "HorizontalWeightedLayout supports at most 255 weights (uint16_t sum guarantee).");

			private:
				static constexpr uint16_t TotalWeight()
				{
					return static_cast<uint16_t>(Support::NumericPack::SumU8<0, Weights...>::Value);
				}

				static_assert(TotalWeight() > 0, "HorizontalWeightedLayout requires TotalWeight() > 0.");

				template<uint8_t Index>
				static constexpr uint8_t WeightAt()
				{
					return Support::NumericPack::ValueAtU8<Index, Weights...>::Value;
				}

				static constexpr pixel_t ParentWidth() { return ParentLayout::Width(); }

				template<uint8_t Index>
				static constexpr pixel_t CalculateWidth()
				{
					return static_cast<pixel_t>((uint32_t(ParentWidth()) * WeightAt<Index>()) / TotalWeight());
				}

				// Sum of widths for cells [0, Index). This is used to compute X offsets.
				template<uint8_t Index, bool IsZero = (Index == 0)>
				struct XSumImpl;

				template<uint8_t Index>
				struct XSumImpl<Index, false>
				{
					static constexpr pixel_t Value = static_cast<pixel_t>(XSumImpl<Index - 1>::Value + CalculateWidth<Index - 1>());
				};

				template<uint8_t Index>
				struct XSumImpl<Index, true>
				{
					static constexpr pixel_t Value = 0;
				};

				// Sum of widths for cells [0, Count). This is used to compute total layout Width().
				template<uint8_t Count, bool IsZero = (Count == 0)>
				struct WidthSumImpl;

				template<uint8_t Count>
				struct WidthSumImpl<Count, false>
				{
					static constexpr pixel_t Value = static_cast<pixel_t>(WidthSumImpl<Count - 1>::Value + CalculateWidth<Count - 1>());
				};

				template<uint8_t Count>
				struct WidthSumImpl<Count, true>
				{
					static constexpr pixel_t Value = 0;
				};

			public:
				static constexpr pixel_t X() { return ParentLayout::X(); }
				static constexpr pixel_t Y() { return ParentLayout::Y(); }

				static constexpr pixel_t Width() { return WidthSumImpl<sizeof...(Weights)>::Value; }
				static constexpr pixel_t Height() { return ParentLayout::Height(); }

				template<uint8_t Index>
				struct Cell
				{
					static_assert(Index < sizeof...(Weights), "HorizontalWeightedLayout Cell Index out of range.");

					static constexpr pixel_t X() { return ParentLayout::X() + XSumImpl<Index>::Value; }
					static constexpr pixel_t Y() { return ParentLayout::Y(); }
					static constexpr pixel_t Width() { return CalculateWidth<Index>(); }
					static constexpr pixel_t Height() { return ParentLayout::Height(); }
				};
			};

			/// <summary>
			/// Splits a parent layout into a vertical sequence of weighted cells.
			/// Note: total width may be less than parent width due to integer division truncation.
			/// </summary>
			/// <typeparam name="ParentLayout">Parent layout providing X/Y/Width/Height.</typeparam>
			/// <typeparam name="...Weights">Cell weights (uint8_t pack).</typeparam>
			template<typename ParentLayout, uint8_t... Weights>
			struct VerticalWeighted
			{
				static_assert(sizeof...(Weights) > 0, "VerticalWeightedLayout requires at least one weight.");
				static_assert(sizeof...(Weights) <= 255, "VerticalWeightedLayout supports at most 255 weights (uint16_t sum guarantee).");

			private:
				static constexpr uint16_t TotalWeight()
				{
					return static_cast<uint16_t>(Support::NumericPack::SumU8<0, Weights...>::Value);
				}

				static_assert(TotalWeight() > 0, "VerticalWeightedLayout requires TotalWeight() > 0.");

				template<uint8_t Index>
				static constexpr uint8_t WeightAt()
				{
					return Support::NumericPack::ValueAtU8<Index, Weights...>::Value;
				}

				static constexpr pixel_t ParentHeight() { return ParentLayout::Height(); }

				template<uint8_t Index>
				static constexpr pixel_t CalculateHeight()
				{
					return static_cast<pixel_t>((uint32_t(ParentHeight()) * WeightAt<Index>()) / TotalWeight());
				}

				// Sum of heights for cells [0, Index). Used for Y offsets.
				template<uint8_t Index, bool IsZero = (Index == 0)>
				struct YSumImpl;

				template<uint8_t Index>
				struct YSumImpl<Index, false>
				{
					static constexpr pixel_t Value = static_cast<pixel_t>(YSumImpl<Index - 1>::Value + CalculateHeight<Index - 1>());
				};

				template<uint8_t Index>
				struct YSumImpl<Index, true>
				{
					static constexpr pixel_t Value = 0;
				};

				// Sum of heights for cells [0, Count). Used for total Height().
				template<uint8_t Count, bool IsZero = (Count == 0)>
				struct HeightSumImpl;

				template<uint8_t Count>
				struct HeightSumImpl<Count, false>
				{
					static constexpr pixel_t Value = static_cast<pixel_t>(HeightSumImpl<Count - 1>::Value + CalculateHeight<Count - 1>());
				};

				template<uint8_t Count>
				struct HeightSumImpl<Count, true>
				{
					static constexpr pixel_t Value = 0;
				};

			public:
				static constexpr pixel_t X() { return ParentLayout::X(); }
				static constexpr pixel_t Y() { return ParentLayout::Y(); }

				static constexpr pixel_t Width() { return ParentLayout::Width(); }
				static constexpr pixel_t Height() { return HeightSumImpl<sizeof...(Weights)>::Value; }

				template<uint8_t Index>
				struct Cell
				{
					static_assert(Index < sizeof...(Weights), "VerticalWeightedLayout Cell Index out of range.");

					static constexpr pixel_t X() { return ParentLayout::X(); }
					static constexpr pixel_t Y() { return ParentLayout::Y() + YSumImpl<Index>::Value; }
					static constexpr pixel_t Width() { return ParentLayout::Width(); }
					static constexpr pixel_t Height() { return CalculateHeight<Index>(); }
				};
			};
		}
	}
}
#endif