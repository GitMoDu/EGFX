#ifndef _BITMAP_DEMO_h
#define _BITMAP_DEMO_h

#include <EgfxDrawer.h>
#include "Assets.h"

// Demo of bitmap sprite with translation, rotation, and brightness animation.
namespace BitmapDemo
{
	using namespace Egfx;
	using namespace Assets;
	using namespace SpriteShader;
	using namespace SpriteShaderEffect;
	using namespace SpriteTransform;
	using namespace IntegerSignal::FixedPoint;

	template<typename ParentLayout>
	struct Layout
	{
		using SpriteType = Assets::DogeSprite;

		// Anchor sprite inside the parent layout.
		static constexpr pixel_t X() { return ParentLayout::X(); }
		static constexpr pixel_t Y() { return ParentLayout::Y(); }
		static constexpr pixel_t Width() { return SpriteType::Width; }
		static constexpr pixel_t Height() { return SpriteType::Height; }

		static constexpr pixel_t RangeX() { return ParentLayout::Width() - Width(); }
		static constexpr pixel_t RangeY() { return ParentLayout::Height() - Height(); }

		// Partial drawing keeps big sprites responsive.
		static constexpr uint8_t DrawSteps() { return SpriteType::Height / 2; }
		static constexpr pixel_t SectionHeight()
		{
			return SpriteType::Height / DrawSteps();
		}
	};

	template<typename ParentLayout, bool Monochrome>
	class View : public Framework::View::AbstractView
	{
	private:
		static constexpr uint32_t TranslationXDuration = 7000000U;
		static constexpr uint32_t TranslationYDuration = 13000000U;
		static constexpr uint32_t BrightnessPeriod = 3876543U;
		static constexpr uint16_t RotationFrequency = 50U;

		using LayoutDefinition = Layout<ParentLayout>;
		using SpriteType = typename LayoutDefinition::SpriteType;

		BrightnessEffect<TransparentColorEffect<SpriteType>> Doge{};
		RotateTransform<SpriteType::Width, SpriteType::Height> DogeRotator{};
		pixel_point_t Position{};
		uint8_t SectionIndex = 0;

	public:
		View() : Framework::View::AbstractView()
		{
			Doge.SetTransparentColor(0);
		}

	protected:
		void ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
		{
			// Ping-pong translation across the layout.
			const uint16_t progressX = ProgressScaler::TriangleResponse(
				ProgressScaler::GetProgress<TranslationXDuration>(frameTime));
			const uint16_t progressY = ProgressScaler::TriangleResponse(
				ProgressScaler::GetProgress<TranslationYDuration>(frameTime));

			Position.x = static_cast<pixel_t>(
				LayoutDefinition::X() +
				ProgressScaler::ScaleProgress(progressX, static_cast<uint16_t>(LayoutDefinition::RangeX())));
			Position.y = static_cast<pixel_t>(
				LayoutDefinition::Y() +
				ProgressScaler::ScaleProgress(progressY, static_cast<uint16_t>(LayoutDefinition::RangeY())));

			// Color displays breathe brightness; mono stays steady.
			if (Monochrome)
			{
				Doge.SetBrightness(0);
			}
			else
			{
				const fraction16_t brightnessFraction =
					Fraction16::GetScalar(frameTime % BrightnessPeriod, BrightnessPeriod);
				const angle_t brightnessAngle = Fraction(brightnessFraction, ANGLE_RANGE);
				Doge.SetBrightness(Sine16(brightnessAngle));
			}

			// Rotate slowly and restart the partial draw cycle.
			DogeRotator.SetRotation(frameTime / RotationFrequency);
			SectionIndex = 0;
		}

		bool Draw(IFrameBuffer* frame) override
		{
			// Render a narrow strip per call to stay within frame budgets.
			SpriteRenderer::TransformDrawPartial(
				frame,
				&Doge,
				&DogeRotator,
				Position.x,
				Position.y,
				0,
				SectionIndex * LayoutDefinition::SectionHeight(),
				SpriteType::Width,
				LayoutDefinition::SectionHeight());

			SectionIndex++;
			if (SectionIndex >= LayoutDefinition::DrawSteps())
			{
				SectionIndex = 0;
				return true;
			}

			return false;
		}
	};

	template<typename ParentLayout, bool Monochrome>
	struct Frame : public Framework::View::FrameAdapter<View<ParentLayout, Monochrome>>
	{
		using Base = Framework::View::FrameAdapter<View<ParentLayout, Monochrome>>;

		Frame() : Base() {}
		virtual ~Frame() = default;

#if defined(SERIAL_LOG)
		void PrintDescription() const
		{
			Serial.println(F("Bitmap\n\twith translation, rotation and brightness animation."));
		}
#endif
	};
}
#endif