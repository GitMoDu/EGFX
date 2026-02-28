#ifndef _LOLA_GRAPHICS_MODULES_WIRELESS_INDICATOR_VIEW_h
#define _LOLA_GRAPHICS_MODULES_WIRELESS_INDICATOR_VIEW_h

#include <EgfxAssets.h>
#include <IntegerSignal.h>

#include "Constant.h"
#include "Layout.h"
#include "Shader.h"
#include "Selector.h"

#include "Drawables/Primitive.h"
#include "Drawables/Bitmask.h"

namespace Egfx
{
	namespace Modules
	{
		namespace WirelessIndicator
		{
			namespace View
			{
				namespace Bitmask
				{
					namespace Abstract
					{
						template<typename Slice0Type,
							typename Slice1Type,
							typename Slice2Type,
							typename Slice3Type,
							bool Monochrome
						>
						class Bars4 : public Framework::View::DrawablesView<
							Slice0Type, Slice1Type, Slice2Type, Slice3Type
						>
						{
						private:
							using Base = Framework::View::DrawablesView<
								Slice0Type, Slice1Type, Slice2Type, Slice3Type
							>;

							using MonochromeTag =
								typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
								IntegerSignal::TypeTraits::TypeDispatch::TrueType,
								IntegerSignal::TypeTraits::TypeDispatch::FalseType,
								Monochrome>::type;

						public:
							bool Visible = false;

						public:
							Bars4() : Base() {}

						protected:
							virtual bool ViewStep(const uint32_t /*frameTime*/, const uint16_t frameCounter) override
							{
								if (!Visible)
									return false;

								SetBarsDithering(frameCounter, MonochromeTag{});

								return true;
							}

						public:
							void SetBarsDitheringEnabled(const uint8_t barIndex, const bool enabled)
							{
								SetBarsDitheringEnabledImpl(barIndex, enabled, MonochromeTag{});
							}

							void SetBarColor(const uint8_t barIndex, const rgb_color_t color)
							{
								SetBarColorImpl(barIndex, color, MonochromeTag{});
							}

						private:
							void SetBarsDithering(const uint16_t frameCounter, IntegerSignal::TypeTraits::TypeDispatch::TrueType)
							{
								this->template drawable<0>().TransformShader.SetMaskPhase(frameCounter & 0b1);
								this->template drawable<1>().TransformShader.SetMaskPhase(frameCounter & 0b1);
								this->template drawable<2>().TransformShader.SetMaskPhase(frameCounter & 0b1);
								this->template drawable<3>().TransformShader.SetMaskPhase(frameCounter & 0b1);
							}

							void SetBarsDithering(const uint16_t /*frameCounter*/, IntegerSignal::TypeTraits::TypeDispatch::FalseType)
							{
								// no-op (non-monochrome)
							}

							void SetBarColorImpl(const uint8_t barIndex, const rgb_color_t color, IntegerSignal::TypeTraits::TypeDispatch::FalseType)
							{
								switch (barIndex)
								{
								case 0:
									this->template drawable<0>().ColorSource.Color = color;
									break;
								case 1:
									this->template drawable<1>().ColorSource.Color = color;
									break;
								case 2:
									this->template drawable<2>().ColorSource.Color = color;
									break;
								case 3:
									this->template drawable<3>().ColorSource.Color = color;
									break;
								default:
									break;
								}
							}

							void SetBarColorImpl(const uint8_t /*barIndex*/, const rgb_color_t /*color*/, IntegerSignal::TypeTraits::TypeDispatch::TrueType)
							{
								// no-op (monochrome)
							}

							void SetBarsDitheringEnabledImpl(const uint8_t barIndex, const bool enabled, IntegerSignal::TypeTraits::TypeDispatch::TrueType)
							{
								switch (barIndex)
								{
								case 0:
									this->template drawable<0>().TransformShader.SetCheckerboardMaskEnabled(enabled);
									break;
								case 1:
									this->template drawable<1>().TransformShader.SetCheckerboardMaskEnabled(enabled);
									break;
								case 2:
									this->template drawable<2>().TransformShader.SetCheckerboardMaskEnabled(enabled);
									break;
								case 3:
									this->template drawable<3>().TransformShader.SetCheckerboardMaskEnabled(enabled);
									break;
								}
							}

							void SetBarsDitheringEnabledImpl(const uint8_t /*barIndex*/, const bool /*enabled*/, IntegerSignal::TypeTraits::TypeDispatch::FalseType)
							{
								// no-op (non-monochrome)
							}
						};
					}

					template<typename ParentLayout, bool Monochrome>
					using Bars22x16 = Abstract::Bars4<
						Drawable::Bitmask::Bars22x16Slice0<Layout::Bars22x16Slice0<ParentLayout>, Monochrome>,
						Drawable::Bitmask::Bars22x16Slice1<Layout::Bars22x16Slice1<ParentLayout>, Monochrome>,
						Drawable::Bitmask::Bars22x16Slice2<Layout::Bars22x16Slice2<ParentLayout>, Monochrome>,
						Drawable::Bitmask::Bars22x16Slice3<Layout::Bars22x16Slice3<ParentLayout>, Monochrome>,
						Monochrome>;

					template<typename ParentLayout, bool Monochrome>
					using Bars24x16 = Abstract::Bars4<
						Drawable::Bitmask::Bars24x16Slice0<Layout::Bars24x16Slice0<ParentLayout>, Monochrome>,
						Drawable::Bitmask::Bars24x16Slice1<Layout::Bars24x16Slice1<ParentLayout>, Monochrome>,
						Drawable::Bitmask::Bars24x16Slice2<Layout::Bars24x16Slice2<ParentLayout>, Monochrome>,
						Drawable::Bitmask::Bars24x16Slice3<Layout::Bars24x16Slice3<ParentLayout>, Monochrome>,
						Monochrome>;

					template<typename ParentLayout, bool Monochrome>
					using Bars42x30 = Abstract::Bars4<
						Drawable::Bitmask::Bars42x30Slice0<Layout::Bars42x30Slice0<ParentLayout>, Monochrome>,
						Drawable::Bitmask::Bars42x30Slice1<Layout::Bars42x30Slice1<ParentLayout>, Monochrome>,
						Drawable::Bitmask::Bars42x30Slice2<Layout::Bars42x30Slice2<ParentLayout>, Monochrome>,
						Drawable::Bitmask::Bars42x30Slice3<Layout::Bars42x30Slice3<ParentLayout>, Monochrome>,
						Monochrome>;

					template<typename ParentLayout>
					class DisabledOverlay : public Framework::View::DrawablesView<
						Drawable::Bitmask::DisabledOverlay<ParentLayout>
					>
					{
					private:
						using Base = Framework::View::DrawablesView<
							Drawable::Bitmask::DisabledOverlay<ParentLayout>
						>;

					public:
						bool Visible = false;

					public:
						DisabledOverlay() : Base() {}
						~DisabledOverlay() = default;

					protected:
						virtual bool ViewStep(const uint32_t /*frameTime*/, const uint16_t /*frameCounter*/) override
						{
							return Visible;
						}
					};
				}

				namespace Primitive
				{
					namespace Abstract
					{
						template<typename DrawableType, bool Monochrome>
						class Bars : public Framework::View::DrawablesView<DrawableType>
						{
						private:
							using Base = Framework::View::DrawablesView<DrawableType>;

							using MonochromeTag =
								typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
								IntegerSignal::TypeTraits::TypeDispatch::TrueType,
								IntegerSignal::TypeTraits::TypeDispatch::FalseType,
								Monochrome>::type;

						public:
							bool Visible = false;

						public:
							Bars() : Base() {}
							~Bars() = default;

							void SetBarColor(const uint8_t barIndex, const rgb_color_t color)
							{
								this->template drawable<0>().SetBarColor(barIndex, color);
							}

							void SetBarsDitheringEnabled(const uint8_t barIndex, const bool enabled)
							{
								this->template drawable<0>().SetBarsDitheringEnabled(barIndex, enabled);
							}

						protected:
							virtual bool ViewStep(const uint32_t /*frameTime*/, const uint16_t frameCounter) override
							{
								if (!Visible)
									return false;

								SetBarsDithering(frameCounter, MonochromeTag{});

								return true;
							}

						private:
							void SetBarsDithering(const uint16_t frameCounter, IntegerSignal::TypeTraits::TypeDispatch::TrueType)
							{
								this->template drawable<0>().OddFrame = frameCounter & 0b1;
							}

							void SetBarsDithering(const uint16_t /*frameCounter*/, IntegerSignal::TypeTraits::TypeDispatch::FalseType)
							{
								// no-op (non-monochrome)
							}
						};
					}

					template<typename ParentLayout, bool Monochrome>
					using Bars8x7 = Abstract::Bars<
						Drawable::Primitive::Bars8x7<Layout::Bars8x7<ParentLayout>, Monochrome>, Monochrome
					>;

					template<typename ParentLayout, bool Monochrome>
					using Bars8x8 = Abstract::Bars<
						Drawable::Primitive::Bars8x8<Layout::Bars8x8<ParentLayout>, Monochrome>, Monochrome
					>;

					template<typename ParentLayout, bool Monochrome>
					using Bars10x7 = Abstract::Bars<
						Drawable::Primitive::Bars10x7<Layout::Bars10x7<ParentLayout>, Monochrome>, Monochrome
					>;

					template<typename ParentLayout, bool Monochrome>
					using Bars12x9 = Abstract::Bars<
						Drawable::Primitive::Bars12x9<Layout::Bars12x9<ParentLayout>, Monochrome>, Monochrome
					>;

					template<typename ParentLayout>
					class DisabledOverlay : public Framework::View::DrawablesView<
						Drawable::Primitive::DisabledOverlay<ParentLayout>
					>
					{
					private:
						using Base = Framework::View::DrawablesView<
							Drawable::Primitive::DisabledOverlay<ParentLayout>
						>;

					public:
						bool Visible = false;

					public:
						DisabledOverlay() : Base() {}
						~DisabledOverlay() = default;

					protected:
						virtual bool ViewStep(const uint32_t /*frameTime*/, const uint16_t /*frameCounter*/) override
						{
							return Visible;
						}
					};
				}


				template<typename ParentLayout, bool Monochrome>
				struct ViewSelector
				{
					using BaseSelector = Selector::BaseSelector<ParentLayout, Monochrome>;
					using LayoutSelector = Layout::LayoutSelector<ParentLayout, Monochrome>;

					using PrimitiveWideOr = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						Primitive::Bars8x8<typename LayoutSelector::Bars, Monochrome>,
						Primitive::Bars8x7<typename LayoutSelector::Bars, Monochrome>,
						BaseSelector::UseWidePrimitive()>::type;

					using PrimitiveHighOr = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						Primitive::Bars10x7<typename LayoutSelector::Bars, Monochrome>,
						PrimitiveWideOr,
						BaseSelector::UseHighPrimitive()>::type;

					using PrimitiveTallOr = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						Primitive::Bars12x9<typename LayoutSelector::Bars, Monochrome>,
						PrimitiveHighOr,
						BaseSelector::UseTallPrimitive()>::type;

					using BitmaskMediumOr = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						Bitmask::Bars24x16<typename LayoutSelector::Bars, Monochrome>,
						Bitmask::Bars22x16<typename LayoutSelector::Bars, Monochrome>,
						BaseSelector::UseMediumBitmask()>::type;

					using BitmaskTallOr = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						Bitmask::Bars42x30<typename LayoutSelector::Bars, Monochrome>,
						BitmaskMediumOr,
						BaseSelector::UseTallBitmask()>::type;

					using Bars = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						BitmaskTallOr,
						PrimitiveTallOr,
						BaseSelector::UseBitmask()>::type;

					using DisabledOverlay = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						Bitmask::DisabledOverlay<typename LayoutSelector::Bars>,
						Primitive::DisabledOverlay<typename LayoutSelector::Bars>,
						BaseSelector::UseBitmask()>::type;
				};

				template<typename ParentLayout,
					bool Monochrome>
				class IndicatorView : public Framework::View::CompositeView<
					typename ViewSelector<ParentLayout, Monochrome>::Bars,
					typename ViewSelector<ParentLayout, Monochrome>::DisabledOverlay
				>
				{
				private:
					using Base = Framework::View::CompositeView<
						typename ViewSelector<ParentLayout, Monochrome>::Bars,
						typename ViewSelector<ParentLayout, Monochrome>::DisabledOverlay>;

					using MonochromeTag =
						typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						IntegerSignal::TypeTraits::TypeDispatch::TrueType,
						IntegerSignal::TypeTraits::TypeDispatch::FalseType,
						Monochrome>::type;

				public:
					static constexpr rgb_color_t RGB_COLOR_GRAY = Rgb::Color(uint32_t(0x808080));

				public: // Palette
					rgb_color_t ColorStrong = RGB_COLOR_WHITE;
					rgb_color_t ColorWeak = RGB_COLOR_GRAY;
					rgb_color_t ColorSearchingActive = RGB_COLOR_WHITE;
					rgb_color_t ColorSearchingPassive = RGB_COLOR_GRAY;

				public:
					uint32_t SearchAnimationDuration = Durations::SearchAnimationDuration;
					IndicatorModeEnum IndicatorMode = IndicatorModeEnum::NoLink;
					uint8_t SignalStrength = 0;

					bool Visible = true;

				private:
					uint32_t AnimationStart = 0;
					IndicatorModeEnum LastIndicatorMode = IndicatorModeEnum::EnumCount;

				public:
					IndicatorView() : Base() {}
					~IndicatorView() = default;

				protected:
					bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
					{
						auto& barsDrawable = this->template view<0>();

						if (IndicatorMode != LastIndicatorMode)
						{
							AnimationStart = frameCounter;
							LastIndicatorMode = IndicatorMode;

							auto& overlayDrawable = this->template view<1>();
							switch (IndicatorMode)
							{
							case IndicatorModeEnum::SignalStrength:
							case IndicatorModeEnum::Searching:
								barsDrawable.Visible = true;
								overlayDrawable.Visible = false;
								break;
							case IndicatorModeEnum::NoLink:
							default:
								barsDrawable.Visible = true;
								overlayDrawable.Visible = true;
								for (uint8_t i = 0; i < Dimensions::BarCount; i++)
								{
									if (Monochrome)
									{
										barsDrawable.SetBarsDitheringEnabled(i, true);
									}
									else
									{
										barsDrawable.SetBarColor(i, ColorWeak);
									}
								}
								break;
							}
						}

						switch (IndicatorMode)
						{
						case IndicatorModeEnum::NoLink:
							break;
						case IndicatorModeEnum::SignalStrength:
							for (uint8_t i = 0; i < Dimensions::BarCount; i++)
							{
								if (Monochrome)
								{
									barsDrawable.SetBarsDitheringEnabled(i, SignalStrength < Threshold(i));
								}
								else
								{
									barsDrawable.SetBarColor(i,
										SignalStrength >= Threshold(i) ? ColorStrong : ColorWeak);
								}
							}
							break;
						case IndicatorModeEnum::Searching:
							for (uint8_t i = 0; i < Dimensions::BarCount; i++)
							{
								const uint16_t phase = ((((frameTime - AnimationStart) * Dimensions::BarCount) / SearchAnimationDuration) & UINT16_MAX)
									% (Dimensions::BarCount * 2 - 1);
								if (phase == i || phase == (uint16_t(Dimensions::BarCount) * 2 - i - 1))
								{
									if (Monochrome)
									{
										barsDrawable.SetBarsDitheringEnabled(i, false);
									}
									else
									{
										barsDrawable.SetBarColor(i, ColorSearchingActive);
									}
								}
								else
								{
									if (Monochrome)
									{
										barsDrawable.SetBarsDitheringEnabled(i, true);
									}
									else
									{
										barsDrawable.SetBarColor(i, ColorSearchingPassive);
									}
								}
							}
							break;
						default:
							break;
						}

						return Visible;
					}

				private:
					static constexpr uint8_t Threshold(const uint8_t barIndex)
					{
						return (barIndex + 1) * (255 / Dimensions::BarCount);
					}
				};

				template<typename ParentLayout,
					bool Monochrome>
				class IndicatorDemoView : public IndicatorView<ParentLayout, Monochrome>
				{
				private:
					using Base = IndicatorView<ParentLayout, Monochrome>;

				public:
					using Base::IndicatorMode;
					using Base::SignalStrength;

				private:
					static constexpr uint32_t ModeChangeInterval = 4500000;

				private:
					uint32_t LastModeChangeTime = 0;

				public:
					IndicatorDemoView() : Base() {}
					~IndicatorDemoView() = default;

				protected:
					bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
					{
						if (frameTime - LastModeChangeTime >= ModeChangeInterval)
						{
							LastModeChangeTime = frameTime;
							switch (IndicatorMode)
							{
							case IndicatorModeEnum::NoLink:
								IndicatorMode = IndicatorModeEnum::SignalStrength;
								SignalStrength = 0;
								break;
							case IndicatorModeEnum::SignalStrength:
								IndicatorMode = IndicatorModeEnum::Searching;
								break;
							case IndicatorModeEnum::Searching:
								IndicatorMode = IndicatorModeEnum::NoLink;
								break;
							default:
								IndicatorMode = IndicatorModeEnum::NoLink;
								break;
							}
						}
						else
						{
							// Cycle signal strength for demo.
							const uint32_t elapsed = frameTime - LastModeChangeTime;

							if (elapsed < (ModeChangeInterval / Dimensions::BarCount))
							{
								SignalStrength = UINT8_MAX / Dimensions::BarCount;
							}
							else if (elapsed < (ModeChangeInterval / 2))
							{
								SignalStrength = UINT8_MAX / 2;
							}
							else if (elapsed < ((ModeChangeInterval * (Dimensions::BarCount - 1)) / Dimensions::BarCount))
							{
								SignalStrength = UINT8_MAX - (UINT8_MAX / Dimensions::BarCount);
							}
							else
							{
								SignalStrength = UINT8_MAX;
							}
						}

						return Base::ViewStep(frameTime, frameCounter);
					}
				};
			}
		}
	}
}
#endif