#ifndef _INTEGERGLASS_FRAMEWORK_PROJECTION_EFFECT_ADAPTER_h
#define _INTEGERGLASS_FRAMEWORK_PROJECTION_EFFECT_ADAPTER_h

#include "Model.h"

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Projection
		{
			namespace Effect
			{
				/// <summary>
				/// Abstract adapter for deriving visibility and logical scroll from projected geometry
				/// and forwarding the resulting effect to the wrapped view.
				/// Inheriting classes must implement ProjectEffect() to apply the effect to the wrapped view.
				/// </summary>
				/// <typeparam name="ViewType">The view whose projected bounds and translation are inspected.</typeparam>
				/// <typeparam name="Orientation">The projection axis and direction used for scroll calculation.</typeparam>
				template<typename ViewType,
					OrientationEnum Orientation = OrientationEnum::LeftToRight>
				class Adapter : public ViewType
				{
				private:
					using Base = ViewType;

				private:
					static constexpr uint32_t MaxProjectedArea = static_cast<uint32_t>(ViewType::ViewLayout::Width()) * static_cast<uint32_t>(ViewType::ViewLayout::Height());

					static constexpr bool IsHorizontal =
						Orientation == OrientationEnum::LeftToRight || Orientation == OrientationEnum::RightToLeft;
					static constexpr int32_t Direction =
						(Orientation == OrientationEnum::LeftToRight || Orientation == OrientationEnum::TopToBottom) ? 1 : -1;

				private:
					effect_t LastEffect{ 0, 0 };
					bool Invalidated = true;

				protected:
					/// <summary>
					/// Abstract method that receives the calculated effect and applies it to the wrapped view.
					/// </summary>
					/// <param name="effect">The effect state to be applied to the wrapped view.</param>
					virtual void ProjectEffect(const effect_t effect) = 0;

				public:
					Adapter() : ViewType() {}
					~Adapter() = default;

					virtual bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
					{
						if (Base::IsVisible())
						{
							const effect_t effect = CalculateEffect();

							if (!Invalidated
								&& (effect.Visibility != LastEffect.Visibility || effect.Scroll != LastEffect.Scroll))
							{
								Invalidated = true;
							}

							if (Invalidated)
							{
								Invalidated = false;
								LastEffect = effect;
								ProjectEffect(effect);
							}

							return Base::ViewStep(frameTime, frameCounter);
						}
						else
						{
							// Mark as invalidated so that the effect is recalculated when the view becomes visible again.
							Invalidated = true;

							return false;
						}
					}

				private:
					UFraction16::scalar_t CalculateVisibility() const
					{
						const uint32_t projectedWidth = static_cast<uint32_t>(Base::GetBoundsWidth());
						const uint32_t projectedHeight = static_cast<uint32_t>(Base::GetBoundsHeight());
						const uint32_t projectedArea = projectedWidth * projectedHeight;

						return UFraction16::GetScalar<uint32_t>(projectedArea, MaxProjectedArea);
					}

					int32_t CalculateScroll() const
					{
						const int32_t layoutDimension = static_cast<int32_t>(IsHorizontal
							? ViewType::ViewLayout::Width() : ViewType::ViewLayout::Height());
						const int32_t translation = static_cast<int32_t>(IsHorizontal
							? Base::GetTranslationX() : Base::GetTranslationY()) * Direction;
						const int32_t wholeUnits = translation / layoutDimension;
						const int32_t remainder = translation % layoutDimension;
						const int32_t fractionalUnit = Fraction16::GetScalar<int32_t>(remainder, layoutDimension);

						return wholeUnits * UFraction16::SCALAR_UNIT + fractionalUnit;
					}

					effect_t CalculateEffect() const
					{
						return effect_t{ CalculateVisibility(), CalculateScroll() };
					}
				};
			}
		}
	}
}
#endif