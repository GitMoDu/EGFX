#ifndef _DEMO_BITMAPIMAGE_h
#define _DEMO_BITMAPIMAGE_h

#include <IntegerGlassModules.h>

#include "../IDemoPage.h"
#include "../Constants.h"

namespace Showcase
{
	using namespace IntegerGlass;

	namespace BitmapImage
	{
		using namespace Modules::ExampleImages;

		using namespace Constants;

		template<typename ParentLayout,
			typename Style>
		struct ImageLayout
		{
			static constexpr Framework::Image::ScaledTransformOrderEnum TransformOrder = Style::TransformOrder;

			static constexpr uint16_t Width() { return (static_cast<uint16_t>(Images::Bitmap::Doge::SourceType::Width) * DogeDimension::Scale<ParentLayout>::Value); }
			static constexpr uint16_t Height() { return (static_cast<uint16_t>(Images::Bitmap::Doge::SourceType::Height) * DogeDimension::Scale<ParentLayout>::Value); }

			static constexpr uint16_t MaxDimension() { return MaxValue<uint16_t>(MaxValue<int16_t>(ParentLayout::Width(), ParentLayout::Height()), MaxValue(Width(), Height())); }

			// Dimension must be scale aware to ensure that the drawable can be properly sized and positioned within the parent layout.
			using dimension_t = typename Framework::AutoDimension::ByValue<MaxDimension()>::dimension_t;

			struct TransformDimensions
			{
				static constexpr uint8_t TransformScale = (Style::TransformOrder == Framework::Image::ScaledTransformOrderEnum::TransformThenScale) ? 1 : DogeDimension::Scale<ParentLayout>::Value;
				static constexpr dimension_t Width() { return static_cast<dimension_t>(Images::Bitmap::Doge::SourceType::Width) * TransformScale; }
				static constexpr dimension_t Height() { return static_cast<dimension_t>(Images::Bitmap::Doge::SourceType::Height) * TransformScale; }
			};
		};

		template<typename ParentLayout>
		using Style = Framework::Image::TemplateImageStyle<
			Framework::Layout::AlignmentEnum::MiddleCenter,
			true, Images::Bitmap::Doge::TransparentColor,
			DogeDimension::Scale<ParentLayout>::Value, DogeDimension::Scale<ParentLayout>::Value,
			Framework::Image::ScaledTransformOrderEnum::TransformThenScale
		>;


		// Dimension must be scale aware to ensure that the drawable can be properly sized and positioned within the parent layout.
		template<typename ParentLayout>
		using dimension_t = typename ImageLayout<ParentLayout, Style<ParentLayout>>::dimension_t;


		// Transform shader must be parent and scale aware to ensure that the drawable can be properly sized and positioned within the parent layout.
		template<typename ParentLayout>
		using TransformShaderType = Modules::Kit::Shader::Transform::Rotate<
			dimension_t<ParentLayout>
		>;

		template<typename ParentLayout, bool Monochrome = false>
		class View : public IDemoPage, public Framework::Image::Bitmap::StaticImageView<
			dimension_t<ParentLayout>, ParentLayout,
			Images::Bitmap::Doge::SourceType,
			Style<ParentLayout>,
			Modules::Kit::Shader::Color::NoShader<dimension_t<ParentLayout>>,
			TransformShaderType<ParentLayout>>
		{
		private:
			using Base = Framework::Image::Bitmap::StaticImageView<
				dimension_t<ParentLayout>, ParentLayout,
				Images::Bitmap::Doge::SourceType,
				Style<ParentLayout>,
				Modules::Kit::Shader::Color::NoShader<dimension_t<ParentLayout>>,
				TransformShaderType<ParentLayout>
			>;

		private:
		public:
			View() : IDemoPage(), Base() {}
			~View() = default;

			const char* GetPageName() const override { return "Bitmap Image"; }

		protected:
			bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
			{
				const UFraction16::scalar_t progress = UFraction16::GetScalar(frameTime % Constants::Image::RotationPeriod, Constants::Image::RotationPeriod);

				const angle_t angle = UFraction16::Fraction(progress, ANGLE_RANGE);

				// Animate continuous rotation.
				Base::Drawable().TransformShader.SetDimensions(
					ImageLayout<ParentLayout, Style<ParentLayout>>::TransformDimensions::Width(),
					ImageLayout<ParentLayout, Style<ParentLayout>>::TransformDimensions::Height());
				Base::Drawable().TransformShader.SetRotation(angle);

				return true;
			}
		};

	}
}
#endif