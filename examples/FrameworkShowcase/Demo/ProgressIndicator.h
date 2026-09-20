#ifndef _DEMO_MODULES_PROGRESS_INDICATOR_h
#define _DEMO_MODULES_PROGRESS_INDICATOR_h

#include <IntegerGlassModules.h>

#include "../IDemoPage.h"
#include "../Constants.h"

namespace Showcase
{
	using namespace IntegerGlass;

	namespace ProgressIndicator
	{
		template<typename ParentLayout>
		using MarginLayout = Framework::Layout::Margin<
			ParentLayout,
			0,
			Constants::Showcase::Margin<ParentLayout>(),
			0,
			0>;

		template<typename ParentLayout>
		using SquareLayout = Framework::Layout::Align<
			ParentLayout,
			Framework::Layout::ConstrainedRatio<MarginLayout<ParentLayout>, 1, 1>,
			Framework::Layout::AlignmentEnum::MiddleCenter>;



		template<typename ParentLayout>
		static constexpr int16_t OneThird()
		{
			return SquareLayout<ParentLayout>::Height() / 3;
		}

		template<typename ParentLayout>
		static constexpr int16_t SmallMargin()
		{
			return MaxValue<int16_t>(1, OneThird<ParentLayout>() / 4);
		}

		template<typename ParentLayout>
		using TopSquareLayout = Framework::TemplateLayout<
			SquareLayout<ParentLayout>::X(),
			SquareLayout<ParentLayout>::Y(),
			OneThird<ParentLayout>() * 2,
			OneThird<ParentLayout>() * 2>;

		template<typename ParentLayout>
		using BottomLineRawLayout = Framework::TemplateLayout<
			TopSquareLayout<ParentLayout>::X(),
			TopSquareLayout<ParentLayout>::Y() + (OneThird<ParentLayout>() * 2),
			OneThird<ParentLayout>() * 3,
			OneThird<ParentLayout>()>;

		template<typename ParentLayout>
		using BottomLineLayout = Framework::Layout::Margin<
			BottomLineRawLayout<ParentLayout>,
			0,
			SmallMargin<ParentLayout>(),
			0,
			SmallMargin<ParentLayout>()>;

		template<typename ParentLayout>
		using RightLineRawLayout = Framework::TemplateLayout<
			TopSquareLayout<ParentLayout>::X() + (OneThird<ParentLayout>() * 2),
			TopSquareLayout<ParentLayout>::Y(),
			OneThird<ParentLayout>(),
			OneThird<ParentLayout>() * 2>;

		template<typename ParentLayout>
		using RightLineLayout = Framework::Layout::Margin<
			RightLineRawLayout<ParentLayout>,
			SmallMargin<ParentLayout>(),
			0,
			SmallMargin<ParentLayout>(),
			0>;

		template<typename ParentLayout, bool Monochrome>
		using CircularView = Modules::ProgressIndicator::Circular::Demo::DemoView<
			TopSquareLayout<ParentLayout>, Monochrome>;

		template<typename ParentLayout>
		struct HorizontalStyle
		{
			using dimension_t = typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t;

			static constexpr pixel_t CornerRadius()
			{
				return MaxValue<pixel_t>(1, (MinValue(ParentLayout::Width(), ParentLayout::Height()) / 2) - 1);
			}

			using DemoStyle = Modules::ProgressIndicator::TemplateLinearProgressStyle<
				Modules::ProgressIndicator::OrientationEnum::LeftToRight, 1, 1, CornerRadius()>;

			using IndicatorPixelShader = Framework::Shader::Pixel::TemplateShader<dimension_t>;
			using BorderPixelShader = Framework::Shader::Pixel::TemplateShader<dimension_t>;
			using TrackPixelShader = Framework::Shader::Pixel::TemplateShader<
				dimension_t,
				Framework::Shader::Source::StaticColor<dimension_t>,
				Framework::Shader::Color::NoShader<dimension_t>,
				Modules::Kit::Shader::Transform::CheckerboardMask<dimension_t, true>>;
		};

		template<typename ParentLayout>
		struct VerticalStyle
		{
			using dimension_t = typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t;

			static constexpr pixel_t CornerRadius()
			{
				return MaxValue<pixel_t>(1, (MinValue(ParentLayout::Width(), ParentLayout::Height()) / 2) - 2);
			}

			using DemoStyle = Modules::ProgressIndicator::TemplateLinearProgressStyle<
				Modules::ProgressIndicator::OrientationEnum::BottomToTop, 2, CornerRadius() / 4, CornerRadius()>;

			using IndicatorPixelShader = Framework::Shader::Pixel::TemplateShader<dimension_t>;
			using BorderPixelShader = Framework::Shader::Pixel::TemplateShader<dimension_t>;
			using TrackPixelShader = Framework::Shader::Pixel::MockShader<dimension_t>;
		};

		template<typename ParentLayout, bool Monochrome, typename Style>
		class LinearView : public Modules::ProgressIndicator::Linear::View<
			typename Style::dimension_t,
			ParentLayout,
			typename Style::DemoStyle,
			typename Style::IndicatorPixelShader,
			typename Style::BorderPixelShader,
			typename Style::TrackPixelShader>
		{
		private:
			using Base = Modules::ProgressIndicator::Linear::View<
				typename Style::dimension_t, ParentLayout, typename Style::DemoStyle,
				typename Style::IndicatorPixelShader,
				typename Style::BorderPixelShader, typename Style::TrackPixelShader>;

		public:
			LinearView() : Base() {}
			~LinearView() = default;

		protected:
			bool ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
			{
				const auto elapsed = Framework::Animation::TimeCurve::Triangle<Constants::ProgressIndicator::CyclePeriod>(frameTime);
				Base::SetProgress(UFraction16::GetScalar<uint32_t>(elapsed, Constants::ProgressIndicator::CyclePeriod));
				return true;
			}
		};

		template<typename ParentLayout, bool Monochrome>
		using HorizontalView = LinearView<BottomLineLayout<ParentLayout>, Monochrome,
			HorizontalStyle<BottomLineLayout<ParentLayout>>>;

		template<typename ParentLayout, bool Monochrome>
		using VerticalView = LinearView<RightLineLayout<ParentLayout>, Monochrome,
			VerticalStyle<RightLineLayout<ParentLayout>>>;

		template<typename ParentLayout, bool Monochrome = false>
		class View : public IDemoPage, public Framework::View::CompositeView<
			ParentLayout,
			CircularView<ParentLayout, Monochrome>,
			HorizontalView<ParentLayout, Monochrome>,
			VerticalView<ParentLayout, Monochrome>>
		{
		private:
			using Base = Framework::View::CompositeView<
				ParentLayout, CircularView<ParentLayout, Monochrome>,
				HorizontalView<ParentLayout, Monochrome>, VerticalView<ParentLayout, Monochrome>>;

		public:
			View() : IDemoPage(), Base() {}
			~View() = default;
			const char* GetPageName() const override { return "Progress Indicator"; }
		};

	}
}

#endif
