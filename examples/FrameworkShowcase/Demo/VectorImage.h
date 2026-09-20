#ifndef _VECTORIMAGE_h
#define _VECTORIMAGE_h

#include <IntegerGlassModules.h>

#include "../IDemoPage.h"
#include "../Constants.h"

namespace Showcase
{
	using namespace IntegerGlass;

	namespace VectorImage
	{
		using namespace Modules::ExampleImages;

		using ImageStyle = Framework::Image::TemplateImageStyle<
			Framework::Layout::AlignmentEnum::MiddleCenter>;

		template<typename ParentLayout, bool Monochrome = false>
		class View : public IDemoPage, public Framework::Image::Vector::ImageView<
			typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t, ParentLayout,
			ImageStyle,
			Images::Vector::Doge::PaletteSource>
		{
		private:
			using dimension_t = typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t;
			using Base = Framework::Image::Vector::ImageView<dimension_t, ParentLayout, ImageStyle, Images::Vector::Doge::PaletteSource>;

			static constexpr dimension_t CanvasWidth = Images::Vector::Doge::CanvasWidth;
			static constexpr dimension_t CanvasHeight = Images::Vector::Doge::CanvasHeight;
			static constexpr dimension_t CanvasDimension = MaxValue(CanvasWidth, CanvasHeight);
			static constexpr dimension_t MaximumSize = static_cast<dimension_t>(MinValue(ParentLayout::Width(), ParentLayout::Height())) - 2;
			static constexpr dimension_t MinimumSize = MinValue(CanvasWidth, CanvasHeight) * 2;

			using InterpolatorType = Modules::Kit::Animation::Interpolator::Sine;
			Framework::Animation::Animator<dimension_t> Animator{};
			Framework::Animation::Interpolator::InterpolatorAdapter<InterpolatorType> Interpolator{};

		public:
			View() : IDemoPage(), Base()
			{
				auto& drawable = Base::Drawable();
				drawable.SetImage(Images::Vector::Doge::Data,
					sizeof(Images::Vector::Doge::Data) / sizeof(Images::Vector::Doge::Data[0]),
					Images::Vector::Doge::CanvasWidth,
					Images::Vector::Doge::CanvasHeight,
					Images::Vector::Doge::ThicknessScale,
					Images::Vector::Doge::Inset,
					Framework::DataSourceTypeEnum::Flash);

				Animator.Start(MaximumSize, MinimumSize, Constants::VectorImage::SizePeriod);
				Animator.SetInterpolator(Interpolator);
				Animator.SetLoop(true);
				drawable.SetDimensions(0, 0);
			}
			~View() = default;

			virtual const char* GetPageName() const override { return "Vector Image"; }

		protected:
			bool ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
			{
				const auto animationState = Animator.Step(frameTime);
				const auto size = animationState.Value;

				const dimension_t width = static_cast<dimension_t>((static_cast<uint32_t>(size) * CanvasWidth) / CanvasDimension);
				const dimension_t height = static_cast<dimension_t>((static_cast<uint32_t>(size) * CanvasHeight) / CanvasDimension);
				auto& drawable = Base::Drawable();
				drawable.SetDimensions(width, height);

				return true;
			}
		};
	}
}
#endif

