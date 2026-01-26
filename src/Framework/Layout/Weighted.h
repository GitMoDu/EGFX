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
			///
			/// - Each cell i receives: floor(ParentWidth * Weight[i] / TotalWeight) pixels.
			/// - Any remainder pixels introduced by integer division are NOT redistributed; the total used width
			///   is therefore <= ParentLayout::Width().
			/// - The resulting strip is top-left anchored to the parent (X/Y match the parent).
			///
			/// Notes:
			/// - Weights are uint8_t, maximum of 255 weights supported (to guarantee TotalWeight() fits in uint16_t).
			/// - Height always matches ParentLayout::Height().
			/// </summary>
			/// <typeparam name="ParentLayout">The layout type of the parent container.</typeparam>
			/// <typeparam name="...Weights">The weight values for each child cell.</typeparam>
			template<typename ParentLayout, uint8_t... Weights>
			struct HorizontalWeighted
			{
				static_assert(sizeof...(Weights) > 0, "HorizontalWeightedLayout requires at least one weight.");
				static_assert(sizeof...(Weights) <= 255, "HorizontalWeightedLayout supports at most 255 weights (uint16_t sum guarantee).");

			private:
				template<typename WeightedLayout, uint32_t TotalWeightValue, uint8_t Index, uint8_t... InnerWeights>
				struct PrefixSumScaledWidthU8;

				template<typename WeightedLayout, uint32_t TotalWeightValue, uint8_t Index, uint8_t Head, uint8_t... Tail>
				struct PrefixSumScaledWidthU8<WeightedLayout, TotalWeightValue, Index, Head, Tail...>
				{
					static constexpr uint32_t Value =
						PrefixSumScaledWidthU8<WeightedLayout, TotalWeightValue, Index - 1, Tail...>::Value
						+ ((uint32_t(WeightedLayout::Width()) * Head) / TotalWeightValue);
				};

				template<typename WeightedLayout, uint32_t TotalWeightValue, uint8_t Head, uint8_t... Tail>
				struct PrefixSumScaledWidthU8<WeightedLayout, TotalWeightValue, 0, Head, Tail...>
				{
					static constexpr uint32_t Value = 0;
				};

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
				static constexpr pixel_t CalculateX()
				{
					return static_cast<pixel_t>(
						PrefixSumScaledWidthU8<ParentLayout, TotalWeight(), Index, Weights...>::Value);
				}

				template<uint8_t Index>
				static constexpr pixel_t CalculateWidth()
				{
					return static_cast<pixel_t>((uint32_t(ParentWidth()) * WeightAt<Index>()) / TotalWeight());
				}

				template<uint8_t Index, uint8_t Count>
				struct WidthSumImpl
				{
					static constexpr pixel_t Value = static_cast<pixel_t>(WidthSumImpl<Index - 1, Count>::Value + CalculateWidth<Index - 1>());
				};

				template<uint8_t Count>
				struct WidthSumImpl<0, Count>
				{
					static constexpr pixel_t Value = 0;
				};

			public:
				static constexpr pixel_t X() { return ParentLayout::X(); }
				static constexpr pixel_t Y() { return ParentLayout::Y(); }

				static constexpr pixel_t Width() { return WidthSumImpl<sizeof...(Weights), sizeof...(Weights)>::Value; }
				static constexpr pixel_t Height() { return ParentLayout::Height(); }

				template<uint8_t Index>
				struct Cell
				{
					static_assert(Index < sizeof...(Weights), "HorizontalWeightedLayout Cell Index out of range.");

					static constexpr pixel_t X() { return ParentLayout::X() + CalculateX<Index>(); }
					static constexpr pixel_t Y() { return ParentLayout::Y(); }
					static constexpr pixel_t Width() { return CalculateWidth<Index>(); }
					static constexpr pixel_t Height() { return ParentLayout::Height(); }
				};
			};

			/// <summary>
			/// Splits a parent layout into a vertical sequence of weighted cells.
			///
			/// - Each cell i receives: floor(ParentHeight * Weight[i] / TotalWeight) pixels.
			/// - Any remainder pixels introduced by integer division are NOT redistributed; the total used height
			///   is therefore <= ParentLayout::Height().
			/// - The resulting strip is top-left anchored to the parent (X/Y match the parent).
			///
			/// Notes:
			/// - Weights are uint8_t, maximum of 255 weights supported (to guarantee TotalWeight() fits in uint16_t).
			/// - Width always matches ParentLayout::Width().
			/// </summary>
			/// <typeparam name="ParentLayout">The layout type of the parent container.</typeparam>
			/// <typeparam name="...Weights">The weight values for each child cell.</typeparam>
			template<typename ParentLayout, uint8_t... Weights>
			struct VerticalWeighted
			{
				static_assert(sizeof...(Weights) > 0, "VerticalWeightedLayout requires at least one weight.");
				static_assert(sizeof...(Weights) <= 255, "VerticalWeightedLayout supports at most 255 weights (uint16_t sum guarantee).");

			private:
				template<typename WeightedLayout, uint32_t TotalWeightValue, uint8_t Index, uint8_t... InnerWeights>
				struct PrefixSumScaledHeightU8;

				template<typename WeightedLayout, uint32_t TotalWeightValue, uint8_t Index, uint8_t Head, uint8_t... Tail>
				struct PrefixSumScaledHeightU8<WeightedLayout, TotalWeightValue, Index, Head, Tail...>
				{
					static constexpr uint32_t Value =
						PrefixSumScaledHeightU8<WeightedLayout, TotalWeightValue, Index - 1, Tail...>::Value
						+ ((uint32_t(WeightedLayout::Height()) * Head) / TotalWeightValue);
				};

				template<typename WeightedLayout, uint32_t TotalWeightValue, uint8_t Head, uint8_t... Tail>
				struct PrefixSumScaledHeightU8<WeightedLayout, TotalWeightValue, 0, Head, Tail...>
				{
					static constexpr uint32_t Value = 0;
				};

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
				static constexpr pixel_t CalculateY()
				{
					return static_cast<pixel_t>(
						PrefixSumScaledHeightU8<ParentLayout, TotalWeight(), Index, Weights...>::Value);
				}

				template<uint8_t Index>
				static constexpr pixel_t CalculateHeight()
				{
					return static_cast<pixel_t>((uint32_t(ParentHeight()) * WeightAt<Index>()) / TotalWeight());
				}

				template<uint8_t Index, uint8_t Count>
				struct HeightSumImpl
				{
					static constexpr pixel_t Value = static_cast<pixel_t>(HeightSumImpl<Index - 1, Count>::Value + CalculateHeight<Index - 1>());
				};

				template<uint8_t Count>
				struct HeightSumImpl<0, Count>
				{
					static constexpr pixel_t Value = 0;
				};

			public:
				static constexpr pixel_t X() { return ParentLayout::X(); }
				static constexpr pixel_t Y() { return ParentLayout::Y(); }

				static constexpr pixel_t Width() { return ParentLayout::Width(); }
				static constexpr pixel_t Height() { return HeightSumImpl<sizeof...(Weights), sizeof...(Weights)>::Value; }

				template<uint8_t Index>
				struct Cell
				{
					static_assert(Index < sizeof...(Weights), "VerticalWeightedLayout Cell Index out of range.");

					static constexpr pixel_t X() { return ParentLayout::X(); }
					static constexpr pixel_t Y() { return ParentLayout::Y() + CalculateY<Index>(); }
					static constexpr pixel_t Width() { return ParentLayout::Width(); }
					static constexpr pixel_t Height() { return CalculateHeight<Index>(); }
				};
			};
		}
	}
}
#endif