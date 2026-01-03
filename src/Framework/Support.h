#ifndef _EGFX_FRAMEWORK_SUPPORT_h
#define _EGFX_FRAMEWORK_SUPPORT_h

#include <stdint.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Support
		{
			// Parameter pack utilities
			namespace ParameterPack
			{
				// Simple SFINAE helper (no std)
				template<bool Condition, typename T = void>
				struct EnableIf {};

				template<typename T>
				struct EnableIf<true, T> { typedef T type; };

				// Type helper for Get<Index>
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

				template<typename... Ts>
				struct ElementPack;

				template<typename T, typename... Rest>
				struct ElementPack<T, Rest...>
				{
					T first;
					ElementPack<Rest...> rest;

					// Default constructor - default-constructs all elements
					ElementPack() : first(), rest() {}

					// Forwarding constructor for non-default construction (optional)
					template<typename TArg, typename... RestArgs>
					ElementPack(TArg&& f, RestArgs&&... r)
						: first(static_cast<TArg&&>(f)), rest(static_cast<RestArgs&&>(r)...) {
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
					// Overload for Index > 0 (recurse into rest)
					template<uint8_t Index>
					static typename EnableIf<(Index > 0), typename GetHelper<Index, T, Rest...>::type&>::type
						GetImpl(ElementPack& pack, int)
					{
						return pack.rest.template Get<Index - 1>();
					}

					template<uint8_t Index>
					static typename EnableIf<(Index > 0), const typename GetHelper<Index, T, Rest...>::type&>::type
						GetImpl(const ElementPack& pack, int)
					{
						return pack.rest.template Get<Index - 1>();
					}

					// Overload for Index == 0 (return first)
					template<uint8_t Index>
					static typename EnableIf<(Index == 0), typename GetHelper<Index, T, Rest...>::type&>::type
						GetImpl(ElementPack& pack, long)
					{
						return pack.first;
					}

					template<uint8_t Index>
					static typename EnableIf<(Index == 0), const typename GetHelper<Index, T, Rest...>::type&>::type
						GetImpl(const ElementPack& pack, long)
					{
						return pack.first;
					}
				};

				// Empty pack specialization
				template<>
				struct ElementPack<>
				{
					ElementPack() = default;
				};
			}
		}
	}
}
#endif