#ifndef _BITMAP_DEMO_h
#define _BITMAP_DEMO_h

#include <EgfxFramework.h>
#include <EgfxAssets.h>
#include "Assets.h"

namespace ImageDemo
{
	using namespace Egfx;

	namespace Constants
	{
		static constexpr uint8_t TranslationYPeriod = 205U;
		static constexpr uint8_t TranslationXPeriod = 143U;
		static constexpr uint8_t BrightnessPeriod = 177U;
		static constexpr uint8_t RotationPeriod = 80U;

		static constexpr uint8_t DogeDimension =
			MaxValue(
				Assets::Images::DogeBit::Width,
				MaxValue(
					Assets::Images::DogeBit::Height,
					MaxValue(
						Assets::Images::Doge565::Width,
						Assets::Images::Doge565::Height
					)
				)
			);
	}
	namespace Layouts
	{
		template<typename ParentLayout>
		struct Scale
		{
			static constexpr pixel_t Dimension() { return MinValue(ParentLayout::Height(), ParentLayout::Width()); }
			static constexpr uint8_t Value = MaxValue<uint8_t>(1, Dimension() / Constants::DogeDimension);
		};

		template<typename ParentLayout,
			typename DrawableType,
			uint8_t ScaleUp>
		struct TranslationRange
		{
			static constexpr pixel_t X() { return ParentLayout::X(); }
			static constexpr pixel_t Y() { return ParentLayout::Y(); }
			static constexpr pixel_t Width() { return DrawableType::Width; }
			static constexpr pixel_t Height() { return DrawableType::Height; }

			static constexpr pixel_t RangeX() { return (ParentLayout::Width() - Width()) / ScaleUp; }
			static constexpr pixel_t RangeY() { return (ParentLayout::Height() - Height()) / ScaleUp; }
		};
	}

	namespace Bitmap
	{
		using dimension_t = Assets::Images::Doge565::dimension_t;

		using ColorShaderType = Framework::Assets::Shader::Color::Brightness<dimension_t>;

		// Applies rotation, and translation.
		// Rotation doesn't depend on scaling, as it is applied before scaling in the pipeline.
		using TransformShaderType = Framework::Assets::Shader::Transform::Translate<dimension_t,
			Framework::Assets::Shader::Transform::Rotate<dimension_t,
			static_cast<dimension_t>(Assets::Images::Doge565::Width),
			static_cast<dimension_t>(Assets::Images::Doge565::Height)>
		>;

		template<typename ParentLayout>
		struct DogeDrawable : public Framework::Bitmap::Bitmap565Drawable<
			ParentLayout,
			dimension_t,
			Assets::Images::Doge565::Width,
			Assets::Images::Doge565::Height,
			Framework::Bitmap::Reader::Flash,
			Layouts::Scale<ParentLayout>::Value, Layouts::Scale<ParentLayout>::Value,
			RGB_COLOR_BLACK,
			true,
			ColorShaderType,
			TransformShaderType
		>
		{
			static constexpr dimension_t Width = Assets::Images::Doge565::Width * Layouts::Scale<ParentLayout>::Value;
			static constexpr dimension_t Height = Assets::Images::Doge565::Height * Layouts::Scale<ParentLayout>::Value;

			DogeDrawable() : Framework::Bitmap::Bitmap565Drawable<
				ParentLayout,
				dimension_t,
				Assets::Images::Doge565::Width,
				Assets::Images::Doge565::Height,
				Framework::Bitmap::Reader::Flash,
				Layouts::Scale<ParentLayout>::Value, Layouts::Scale<ParentLayout>::Value,
				RGB_COLOR_BLACK,
				true,
				ColorShaderType,
				TransformShaderType
			>(Assets::Images::Doge565::Bitmap)
			{
			}

			~DogeDrawable() = default;
		};

		template<typename ParentLayout>
		class View : public Framework::View::DrawablesView<
			DogeDrawable<ParentLayout>
		>
		{
		private:
			using Base = Framework::View::DrawablesView<
				DogeDrawable<ParentLayout>>;

			using RangeLayout = Layouts::TranslationRange<
				ParentLayout,
				DogeDrawable<ParentLayout>,
				Layouts::Scale<ParentLayout>::Value>;

		public:
			View() : Base()
			{
			}

			~View() = default;

		private:
			DogeDrawable<ParentLayout>& Doge()
			{
				return Base::template drawable<0>();
			}

		protected:
			bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
			{
				// Animate translation bouncing around the screen.
				const uint16_t progressX = ProgressScaler::TriangleResponse(
					static_cast<angle_t>(frameTime / Constants::TranslationXPeriod));
				const uint16_t progressY = ProgressScaler::TriangleResponse(
					static_cast<angle_t>(frameTime / Constants::TranslationYPeriod));
				Doge().TransformShader.SetTranslation(
					ProgressScaler::ScaleProgress(progressX, uint16_t(RangeLayout::RangeX())),
					ProgressScaler::ScaleProgress(progressY, uint16_t(RangeLayout::RangeY())));

				// Animate continuous rotation.
				Doge().TransformShader.SetRotation(static_cast<angle_t>(frameTime / Constants::RotationPeriod));

				// Animate brightness with sine wave.
				const angle_t brightnessAngle = static_cast<angle_t>(frameTime / Constants::BrightnessPeriod);
				Doge().ColorShader.SetBrightness(Sine16(brightnessAngle));
				return true;
			}
		};
	}

	namespace Bitmask
	{
		using dimension_t = Assets::Images::Doge565::dimension_t;

		using ColorShaderType = Framework::Assets::Shader::Color::NoShader<dimension_t>;

		// Applies rotation, and translation.
		// Rotation depends on scaling, as it is applied after scaling in the pipeline.
		template<uint8_t ScaleUp>
		using TransformShaderType = Framework::Assets::Shader::Transform::Translate < dimension_t,
			Framework::Assets::Shader::Transform::Rotate<dimension_t,
			static_cast<dimension_t>(Assets::Images::DogeBit::Width* ScaleUp)
			, static_cast<dimension_t>(Assets::Images::DogeBit::Height* ScaleUp)
			>
		>;

		using ColorSourceType = Framework::Shader::Source::SingleColor<dimension_t>;

		template<uint8_t ScaleUp>
		using PixelShaderType = Framework::Shader::Pixel::TemplateShader<
			dimension_t,
			ColorSourceType,
			ColorShaderType,
			TransformShaderType<ScaleUp>
		>;

		template<uint8_t ScaleUp>
		using PrimitiveShaderType = Framework::Shader::Primitive::TemplateShader<
			dimension_t,
			PixelShaderType<ScaleUp>
		>;

		template<typename ParentLayout>
		struct DogeDrawable : public Framework::Bitmask::BitmaskDrawable <
			ParentLayout,
			dimension_t,
			Assets::Images::DogeBit::Width,
			Assets::Images::DogeBit::Height,
			Framework::Bitmask::Reader::Flash,
			Layouts::Scale<ParentLayout>::Value, Layouts::Scale<ParentLayout>::Value,
			PrimitiveShaderType<Layouts::Scale<ParentLayout>::Value>
		>
		{
			static constexpr dimension_t Width = Assets::Images::DogeBit::Width * Layouts::Scale<ParentLayout>::Value;
			static constexpr dimension_t Height = Assets::Images::DogeBit::Height * Layouts::Scale<ParentLayout>::Value;

			DogeDrawable() : Framework::Bitmask::BitmaskDrawable <
				ParentLayout,
				dimension_t,
				Assets::Images::DogeBit::Width,
				Assets::Images::DogeBit::Height,
				Framework::Bitmask::Reader::Flash,
				Layouts::Scale<ParentLayout>::Value, Layouts::Scale<ParentLayout>::Value,
				PrimitiveShaderType<Layouts::Scale<ParentLayout>::Value>
			>(Assets::Images::DogeBit::Mask)
			{
			}

			~DogeDrawable() = default;
		};

		template<typename ParentLayout>
		class View : public Framework::View::DrawablesView<
			DogeDrawable<ParentLayout>
		>
		{
		private:
			using Base = Framework::View::DrawablesView<
				DogeDrawable<ParentLayout>
			>;

			using RangeLayout = Layouts::TranslationRange<
				ParentLayout,
				DogeDrawable<ParentLayout>,
				Layouts::Scale<ParentLayout>::Value>;

		public:
			View() : Base()
			{
			}

			~View() = default;

		private:
			DogeDrawable<ParentLayout>& Doge()
			{
				return Base::template drawable<0>();
			}

		protected:
			bool ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
			{
				// Animate translation bouncing around the screen.
				const uint16_t progressX = ProgressScaler::TriangleResponse(
					static_cast<angle_t>(frameTime / Constants::TranslationXPeriod));
				const uint16_t progressY = ProgressScaler::TriangleResponse(
					static_cast<angle_t>(frameTime / Constants::TranslationYPeriod));
				Doge().TransformShader.SetTranslation(
					ProgressScaler::ScaleProgress(progressX, uint16_t(RangeLayout::RangeX())),
					ProgressScaler::ScaleProgress(progressY, uint16_t(RangeLayout::RangeY())));

				// Animate continuous rotation.
				Doge().TransformShader.SetRotation(static_cast<angle_t>(frameTime / Constants::RotationPeriod));
				return true;
			}
		};
	}

	template<typename ParentLayout>
	struct FrameMonoChrome : public Framework::View::FrameAdapter<Bitmask::View<ParentLayout>>
	{
		using Base = Framework::View::FrameAdapter<Bitmask::View<ParentLayout>>;

		FrameMonoChrome() : Base() {}
		~FrameMonoChrome() = default;

#if defined(SERIAL_LOG)
		void PrintDescription() const
		{
			Serial.println(F("Bitmask\n\twith translation and rotation animation."));
		}
#endif
	};

	template<typename ParentLayout>
	struct FrameColor : public Framework::View::FrameAdapter<Bitmap::View<ParentLayout>>
	{
		using Base = Framework::View::FrameAdapter<Bitmap::View<ParentLayout>>;

		FrameColor() : Base() {}
		~FrameColor() = default;

#if defined(SERIAL_LOG)
		void PrintDescription() const
		{
			Serial.println(F("Bitmap\n\twith translation, rotation and brightness animation."));
		}
#endif
	};

	/// <summary>
	/// Type selector between monochrome and color frame types based on a compile-time parameters.
	/// </summary>
	/// <typeparam name="ParentLayout">The parent layout.</typeparam>
	/// <typeparam name="Monochrome">Monochrome display flag.</typeparam>
	template<typename ParentLayout, bool Monochrome>
	using Frame = typename TypeTraits::TypeConditional::conditional_type<
		FrameMonoChrome<ParentLayout>,
		FrameColor<ParentLayout>,
		Monochrome>::type;
}
#endif