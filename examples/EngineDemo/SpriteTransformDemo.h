#ifndef _SPRITE_TRANSFORM_DEMO_h
#define _SPRITE_TRANSFORM_DEMO_h

#include <EgfxDrawer.h>
#include "Assets.h"

// Demo of various sprite transforms applied to different sprites.
namespace SpriteTransformDemo
{
	using namespace Egfx;

	namespace Definitions
	{
		// Centralize sprite/transform traits so layout and drawables stay templated but lean.
		template<typename ParentLayout>
		struct Types
		{
			static constexpr bool UseHeartBig = (ParentLayout::Width() > 128) || (ParentLayout::Height() > 128);
			using HeartSpriteType = typename Assets::HeartSelect<UseHeartBig>::type;
			using HeartShadedSprite = SpriteShader::ColorShader<HeartSpriteType>;
			using HeartTransformType = SpriteTransform::DownScaleXYTransform<HeartSpriteType::Width, HeartSpriteType::Height>;

			static constexpr pixel_t RectangleSide = MinValue(ParentLayout::Width(), ParentLayout::Height()) / 3;
			using RectangleSpriteType = RectangleFixedSprite<RectangleSide, RectangleSide>;
			using RectangleShadedSprite = SpriteShader::GridShader<RectangleSpriteType>;
			using RectangleTransformType = SpriteTransform::SkewHorizontalTransform<RectangleSide,
				SpriteTransform::SkewVerticalTransform<RectangleSide>>;

			using DogeSpriteType = Assets::DogeBitSprite;
			using DogeTransformType = SpriteTransform::RotateTransform<DogeSpriteType::Width, DogeSpriteType::Height>;
		};
	}

	template<typename ParentLayout>
	struct Layout
	{
		using Types = Definitions::Types<ParentLayout>;

		// Each section maps to one drawable; helpers keep the math in one place.
		struct Heart
		{
			static constexpr pixel_t Width() { return Types::HeartSpriteType::Width; }
			static constexpr pixel_t Height() { return Types::HeartSpriteType::Height; }
			static constexpr pixel_t X() { return ParentLayout::X() + 2; }
			static constexpr pixel_t Y() { return ParentLayout::Y() + (ParentLayout::Height() / 2) - (Height() / 2); }
		};

		struct Rectangle
		{
			static constexpr pixel_t Width() { return Types::RectangleSide; }
			static constexpr pixel_t Height() { return Types::RectangleSide; }
			static constexpr pixel_t X() { return ParentLayout::X() + ParentLayout::Width() - Width() - (Width() / 3); }
			static constexpr pixel_t Y() { return ParentLayout::Y() + (ParentLayout::Height() / 2) - (Height() / 2); }
		};

		struct Doge
		{
			static constexpr pixel_t Width() { return Types::DogeSpriteType::Width; }
			static constexpr pixel_t Height() { return Types::DogeSpriteType::Height; }
			static constexpr pixel_t X() { return ParentLayout::X(); }
			static constexpr pixel_t Y() { return ParentLayout::Y(); }
			static constexpr pixel_t RangeX() { return ParentLayout::Width() - Width(); }
			static constexpr pixel_t RangeY() { return ParentLayout::Height() - Height(); }
		};
	};

	namespace Drawables
	{
		// Heart pulsates via down-scale transform, offset is tracked separately.
		template<typename Layout, typename SpriteType, typename TransformType, bool Monochrome>
		struct Heart
		{
			TransformType Transform{};
			pixel_point_t Translation{};
			SpriteType Sprite{};

			Heart()
			{
				if (Monochrome)
				{
					Sprite.SetColor(RGB_COLOR_WHITE);
				}
				else
				{
					Sprite.SetColor(RGB_COLOR_RED);
				}
			}

			void Draw(IFrameBuffer* frame)
			{
				SpriteRenderer::TransformDraw(frame, &Sprite, &Transform, Layout::X() + Translation.x, Layout::Y() + Translation.y);
			}
		};

		// Rectangle alternates between horizontal/vertical skew sweeps.
		template<typename Layout, typename SpriteType, typename TransformType, bool Monochrome>
		struct Rectangle
		{
			SpriteType Sprite{};
			TransformType Transform{};

			Rectangle()
			{
				if (Monochrome)
				{
					Sprite.SetColor1(RGB_COLOR_WHITE);
					Sprite.SetColor2(RGB_COLOR_BLACK);
				}

				Transform.SetReferenceX(0);
				Transform.SetReferenceY((Layout::Height() - 1) / 2);
				Transform.SetSkewX(0);
				Transform.SetSkewY(0);
			}

			void Draw(IFrameBuffer* frame)
			{
				SpriteRenderer::TransformDraw(frame, &Sprite, &Transform, Layout::X(), Layout::Y());
			}
		};

		// Doge uses either gradient or solid shader, selected at construction.
		template<typename Layout, typename TransformType, bool Monochrome>
		struct Doge
		{
			ISprite* DogeSprite = nullptr;

			TransformType Transform{};
			pixel_point_t Position{};

			void Draw(IFrameBuffer* frame)
			{
				if (DogeSprite == nullptr)
				{
					return;
				}

				SpriteRenderer::TransformDraw(frame, DogeSprite, &Transform, Position.x, Position.y);
			}
		};
	}

	template<typename ParentLayout, bool Monochrome>
	class View : public Framework::View::DrawablesView<
		Drawables::Heart<typename Layout<ParentLayout>::Heart,
		typename Definitions::Types<ParentLayout>::HeartShadedSprite,
		typename Definitions::Types<ParentLayout>::HeartTransformType, Monochrome>,
		Drawables::Rectangle<typename Layout<ParentLayout>::Rectangle,
		typename Definitions::Types<ParentLayout>::RectangleShadedSprite,
		typename Definitions::Types<ParentLayout>::RectangleTransformType, Monochrome>,
		Drawables::Doge<typename Layout<ParentLayout>::Doge,
		typename Definitions::Types<ParentLayout>::DogeTransformType, Monochrome>>
	{
	private:
		using Types = Definitions::Types<ParentLayout>;
		using LayoutDefinition = Layout<ParentLayout>;
		using Base = Framework::View::DrawablesView<
			Drawables::Heart<typename LayoutDefinition::Heart,
			typename Types::HeartShadedSprite,
			typename Types::HeartTransformType, Monochrome>,
			Drawables::Rectangle<typename LayoutDefinition::Rectangle,
			typename Types::RectangleShadedSprite,
			typename Types::RectangleTransformType, Monochrome>,
			Drawables::Doge<typename LayoutDefinition::Doge,
			typename Types::DogeTransformType, Monochrome>>;

		static constexpr uint32_t RectanglePeriodUs = 1400000U;
		static constexpr uint32_t RectangleSectionPeriodUs = RectanglePeriodUs * 2U;
		static constexpr uint32_t DogeProgressXPeriodUs = 7000000U;
		static constexpr uint32_t DogeProgressYPeriodUs = 13000000U;

	private:
		SpriteShader::HorizontalGradientShader<typename Types::DogeSpriteType> GradientDoge{};
		SpriteShader::ColorShader<typename Types::DogeSpriteType> DogeShader{};

	public:
		View()
			: Base()
		{
			if (Monochrome)
			{
				DogeShader.SetColor(Rgb::Color(RGB_COLOR_WHITE));
			}
			else
			{
				GradientDoge.SetColor1(Rgb::Color(0xFF, 0x5A, 0x23));
				GradientDoge.SetColor2(Rgb::Color(0xFF, 0xD3, 0x91));
			}

			auto& doge = Base::template drawable<2>();
			if (Monochrome)
			{
				doge.DogeSprite = &DogeShader;
			}
			else
			{
				doge.DogeSprite = &GradientDoge;
			}
		}

	protected:
		void ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
		{
			UpdateHeart(frameTime);
			UpdateRectangle(frameTime);
			UpdateDoge(frameTime);
		}

	private:
		void UpdateHeart(const uint32_t frameTime)
		{
			// Heart pulses with a simple triangle-wave scale driven from frame time.
			const uint16_t progress = ProgressScaler::TriangleResponse(frameTime / 20);
			const pixel_t downScale = static_cast<pixel_t>(ProgressScaler::ScaleProgress(progress, static_cast<uint8_t>(LayoutDefinition::Heart::Height())));

			auto& heart = Base::template drawable<0>();
			heart.Transform.SetDownScaleXY(downScale);
			heart.Translation.x = static_cast<pixel_t>(downScale / 2);
			heart.Translation.y = static_cast<pixel_t>(downScale / 2);
		}

		void UpdateRectangle(const uint32_t frameTime)
		{
			auto& rectangle = Base::template drawable<1>();

			const uint16_t sectionProgress = ProgressScaler::GetProgress<RectangleSectionPeriodUs>(frameTime);
			const uint16_t progress = static_cast<uint16_t>(
				ProgressScaler::GetProgress<RectanglePeriodUs>(frameTime) + (INT16_MAX / 2));
			const uint16_t triangle = ProgressScaler::TriangleResponse(progress);

			if (ProgressScaler::ScaleProgress(sectionProgress, static_cast<uint8_t>(2)) == 0)
			{
				const int16_t width = static_cast<int16_t>(LayoutDefinition::Rectangle::Width());
				const int16_t scaled = static_cast<int16_t>(
					ProgressScaler::ScaleProgress(triangle,
						static_cast<uint8_t>(LayoutDefinition::Rectangle::Width() * 2)));
				rectangle.Transform.SetSkewX(static_cast<int8_t>(-width + scaled));
				rectangle.Transform.SetSkewY(0);
			}
			else
			{
				const int16_t height = static_cast<int16_t>(LayoutDefinition::Rectangle::Height());
				const int16_t scaled = static_cast<int16_t>(
					ProgressScaler::ScaleProgress(triangle,
						static_cast<uint8_t>(LayoutDefinition::Rectangle::Height() * 2)));
				rectangle.Transform.SetSkewX(0);
				rectangle.Transform.SetSkewY(static_cast<int8_t>(-height + scaled));
			}

			if (!Monochrome)
			{
				rectangle.Sprite.SetColor1(Rgb::ColorFromHSV(static_cast<angle_t>(frameTime / 233), UINT8_MAX, UINT8_MAX));
				rectangle.Sprite.SetColor2(Rgb::ColorFromHSV(static_cast<angle_t>(frameTime / 175), UINT8_MAX, UINT8_MAX));
			}
		}

		void UpdateDoge(const uint32_t frameTime)
		{
			auto& doge = Base::template drawable<2>();

			const uint16_t progressX = ProgressScaler::TriangleResponse(
				ProgressScaler::GetProgress<DogeProgressXPeriodUs>(frameTime));
			const uint16_t progressY = ProgressScaler::TriangleResponse(
				ProgressScaler::GetProgress<DogeProgressYPeriodUs>(frameTime));

			doge.Position.x = static_cast<pixel_t>(
				LayoutDefinition::Doge::X() +
				ProgressScaler::ScaleProgress(progressX, static_cast<uint16_t>(LayoutDefinition::Doge::RangeX())));

			doge.Position.y = static_cast<pixel_t>(
				LayoutDefinition::Doge::Y() +
				ProgressScaler::ScaleProgress(progressY, static_cast<uint16_t>(LayoutDefinition::Doge::RangeY())));

			doge.Transform.SetRotation(frameTime / 100);
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
			Serial.println(F("Sprite Transform\n\tdraw and transform sprites with scaling, skewing, and rotation."));
		}
#endif
	};
}

#endif