#ifndef _INTEGERGLASS_MODULES_LOGO_CONSTANT_h
#define _INTEGERGLASS_MODULES_LOGO_CONSTANT_h

#include "../../IntegerGlassFramework.h"
#include "Assets.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Logo
		{

			namespace Colors
			{
				static constexpr rgb_color_t Background = RGB_COLOR_BLACK;
				static constexpr rgb_color_t Foreground = RGB_COLOR_WHITE;
				static constexpr rgb_color_t Glass = Rgb::Color(uint32_t(0xA0A0A0));

				// Palette colors
				static constexpr rgb_color_t Color1 = Rgb::Color(uint32_t(0xA80F21));
				static constexpr rgb_color_t Color2 = Rgb::Color(uint32_t(0xA2D300));
				static constexpr rgb_color_t Color3 = Rgb::Color(uint32_t(0x004AB2));
			}

			namespace Durations
			{
				static constexpr uint32_t ColorDuration = 1000000;
				static constexpr uint32_t SplashDuration = 7000000;

				template<uint32_t AnimationDuration>
				struct Splash
				{
					static constexpr uint32_t Margin = AnimationDuration / 24;
					static constexpr uint32_t UsableTime = AnimationDuration - Margin;
					static constexpr uint32_t Hold = (UsableTime * 5) / 10;

					static constexpr uint32_t SlideIn = (((UsableTime - Hold) * 8) / 10) - Margin;
					static constexpr uint32_t FadeOut = UsableTime - Hold - SlideIn;
				};
			}

			namespace Dimensions
			{
				using namespace IntegerSignal::FixedPoint::FactorScale;

				static constexpr uint8_t CharacterCount = 5;
				static constexpr uint8_t FontColumnCount = 6;
				static constexpr uint8_t FontRowCount = 8;


				static constexpr scale16_t BandAngleRatio = Scale16::GetFactor<int32_t>(26, 30);
				static constexpr ufraction16_t Band1WidthRatio = UFraction16::GetScalar<int32_t>(7, 33);
				static constexpr ufraction16_t Band2WidthRatio = UFraction16::GetScalar<int32_t>(20, 33);
				static constexpr ufraction16_t BandSpaceRatio = UFraction16::GetScalar<int32_t>(1, 1);

				static constexpr scale16_t LineAngleRatio = Scale16::GetFactor<int32_t>(25, 30);
				static constexpr ufraction16_t BandWidthRatio = UFraction16::GetScalar<int32_t>(20, 33);
				static constexpr ufraction16_t LineSpaceRatio = UFraction16::GetScalar<int32_t>(5, 10);

				static constexpr ufraction16_t CornerRadiusRatio = UFraction16::GetScalar<int32_t>(15, 113);

				static constexpr uint8_t GlassAspectRatioWidth = 113;
				static constexpr uint8_t GlassAspectRatioHeight = 35;
				static constexpr uint8_t TextAspectRatioWidth = 101;
				static constexpr uint8_t TextAspectRatioHeight = 23;
				static constexpr uint8_t LabelScaleUp = 2;
				static constexpr uint8_t LabelRatioMargin = 2;
				static constexpr uint8_t LabelRatioWidth = Assets::IntegerLabel::SourceType::Width * LabelScaleUp;
				static constexpr uint8_t LabelRatioHeight = Assets::IntegerLabel::SourceType::Height * LabelScaleUp;
				static constexpr uint8_t LayoutAspectRatioWidth = MaxValue<uint8_t>(GlassAspectRatioWidth, LabelRatioWidth);
				static constexpr uint8_t LayoutAspectRatioHeight = LabelRatioHeight + LabelRatioMargin + GlassAspectRatioHeight;

				static constexpr uint8_t ColumnCount = (FontColumnCount * CharacterCount) + (CharacterCount - 1);
				static constexpr uint8_t RowCount = FontRowCount;

				static constexpr uint8_t SplashVerticalShiftDivisor = 1;
			}
		}
	}
}
#endif