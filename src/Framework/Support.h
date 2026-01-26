#ifndef _EGFX_FRAMEWORK_SUPPORT_h
#define _EGFX_FRAMEWORK_SUPPORT_h

#include "../Platform/Platform.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Support
		{
			/// <summary>
			/// Generic pack traversal/storage helpers.
			/// </summary>
			namespace ParameterPack
			{
				/// <summary>
				/// Type helper for accessing the type at a given pack index.
				/// </summary>
				template<uint8_t I, typename First, typename... Others>
				struct GetHelper
				{
					typedef typename GetHelper<I - 1, Others...>::type type;
				};

				template<typename First, typename... Others>
				struct GetHelper<0, First, Others...>
				{
					typedef First type;
				};

				/// <summary>
				/// Minimal "tuple-like" storage for a parameter pack (C++11, no <tuple> dependency).
				/// </summary>
				template<typename... Ts>
				struct ElementPack;

				template<typename T, typename... Rest>
				struct ElementPack<T, Rest...>
				{
					T first;
					ElementPack<Rest...> rest;

					ElementPack() : first(), rest() {}

					template<typename TArg, typename... RestArgs>
					ElementPack(TArg&& f, RestArgs&&... r)
						: first(static_cast<TArg&&>(f)), rest(static_cast<RestArgs&&>(r)...)
					{
					}

					template<uint8_t Index>
					typename GetHelper<Index, T, Rest...>::type& Get()
					{
						return GetImpl<Index>(*this, 0);
					}

					template<uint8_t Index>
					const typename GetHelper<Index, T, Rest...>::type& Get() const
					{
						return GetImpl<Index>(*this, 0);
					}

				private:
					// Index > 0: recurse into rest
					template<uint8_t Index>
					static typename IntegerSignal::TypeTraits::TypeEnableIf::enable_if<
						(Index > 0),
						typename GetHelper<Index, T, Rest...>::type&
					>::type
						GetImpl(ElementPack& pack, int)
					{
						return pack.rest.template Get<Index - 1>();
					}

					template<uint8_t Index>
					static typename IntegerSignal::TypeTraits::TypeEnableIf::enable_if<
						(Index > 0),
						const typename GetHelper<Index, T, Rest...>::type&
					>::type
						GetImpl(const ElementPack& pack, int)
					{
						return pack.rest.template Get<Index - 1>();
					}

					// Index == 0: return first
					template<uint8_t Index>
					static typename IntegerSignal::TypeTraits::TypeEnableIf::enable_if<
						(Index == 0),
						typename GetHelper<Index, T, Rest...>::type&
					>::type
						GetImpl(ElementPack& pack, long)
					{
						return pack.first;
					}

					template<uint8_t Index>
					static typename IntegerSignal::TypeTraits::TypeEnableIf::enable_if<
						(Index == 0),
						const typename GetHelper<Index, T, Rest...>::type&
					>::type
						GetImpl(const ElementPack& pack, long)
					{
						return pack.first;
					}
				};

				template<>
				struct ElementPack<>
				{
					ElementPack() = default;
				};
			}

			/// <summary>
			/// Numeric helpers for uint8_t parameter packs.
			/// </summary>
			namespace NumericPack
			{
				/// <summary>
				/// Returns the uint8_t element at a given index in a uint8_t parameter pack.
				/// </summary>
				template<uint8_t Index, uint8_t Head, uint8_t... Tail>
				struct ValueAtU8
				{
					static constexpr uint8_t Value = ValueAtU8<Index - 1, Tail...>::Value;
				};

				template<uint8_t Head, uint8_t... Tail>
				struct ValueAtU8<0, Head, Tail...>
				{
					static constexpr uint8_t Value = Head;
				};

				/// <summary>
				/// Sums a uint8_t parameter pack into a uint32_t accumulator.
				/// </summary>
				template<uint32_t Sum, uint8_t... Rest>
				struct SumU8;

				template<uint32_t Sum>
				struct SumU8<Sum>
				{
					static constexpr uint32_t Value = Sum;
				};

				template<uint32_t Sum, uint8_t Head, uint8_t... Tail>
				struct SumU8<Sum, Head, Tail...>
				{
					static constexpr uint32_t Value = SumU8<Sum + Head, Tail...>::Value;
				};
			}
		}
	}
}
#endif