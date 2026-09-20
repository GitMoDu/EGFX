#ifndef _INTEGERGLASS_FRAMEWORK_LAYOUT_COMBINE_H
#define _INTEGERGLASS_FRAMEWORK_LAYOUT_COMBINE_H

#include "Model.h"

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Layout
		{
			namespace Detail
			{
				template<typename... LayoutTypes>
				struct CombineBounds
				{
				private:
					template<typename LayoutType>
					static constexpr int16_t MinX()
					{
						return LayoutType::X();
					}

					template<typename LayoutType, typename NextLayout, typename... RestLayouts>
					static constexpr int16_t MinX()
					{
						return MinValue<int16_t>(LayoutType::X(), MinX<NextLayout, RestLayouts...>());
					}

					template<typename LayoutType>
					static constexpr int16_t MinY()
					{
						return LayoutType::Y();
					}

					template<typename LayoutType, typename NextLayout, typename... RestLayouts>
					static constexpr int16_t MinY()
					{
						return MinValue<int16_t>(LayoutType::Y(), MinY<NextLayout, RestLayouts...>());
					}

					template<typename LayoutType>
					static constexpr int16_t MaxRight()
					{
						return LayoutType::X() + LayoutType::Width();
					}

					template<typename LayoutType, typename NextLayout, typename... RestLayouts>
					static constexpr int16_t MaxRight()
					{
						return MaxValue<int16_t>(
							LayoutType::X() + LayoutType::Width(),
							MaxRight<NextLayout, RestLayouts...>());
					}

					template<typename LayoutType>
					static constexpr int16_t MaxBottom()
					{
						return LayoutType::Y() + LayoutType::Height();
					}

					template<typename LayoutType, typename NextLayout, typename... RestLayouts>
					static constexpr int16_t MaxBottom()
					{
						return MaxValue<int16_t>(
							LayoutType::Y() + LayoutType::Height(),
							MaxBottom<NextLayout, RestLayouts...>());
					}

				public:
					static constexpr int16_t X() { return MinX<LayoutTypes...>(); }
					static constexpr int16_t Y() { return MinY<LayoutTypes...>(); }
					static constexpr int16_t Right() { return MaxRight<LayoutTypes...>(); }
					static constexpr int16_t Bottom() { return MaxBottom<LayoutTypes...>(); }
				};
			}

			/// <summary>
			/// Combines one or more layouts into their compile-time bounding rectangle.
			/// </summary>
			/// <typeparam name="LayoutTypes">Layouts providing X(), Y(), Width(), and Height().</typeparam>
			template<typename... LayoutTypes>
			struct Combine
			{
				static_assert(sizeof...(LayoutTypes) > 0, "Combine requires at least one layout type.");

			private:
				using Bounds = Detail::CombineBounds<LayoutTypes...>;

			public:
				static constexpr int16_t X() { return Bounds::X(); }
				static constexpr int16_t Y() { return Bounds::Y(); }
				static constexpr int16_t Width() { return Bounds::Right() - Bounds::X(); }
				static constexpr int16_t Height() { return Bounds::Bottom() - Bounds::Y(); }
			};
		}
	}
}

#endif