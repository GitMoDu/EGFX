// FpsDrawer.h

#ifndef _FPS_DRAWER_h
#define _FPS_DRAWER_h

#include <ArduinoGraphicsDrawer.h>

namespace Egfx
{
	enum class FpsDrawerPosition
	{
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};

	template<typename ScreenLayout,
		typename FontRendererType,
		FpsDrawerPosition fpsDrawerPosition = FpsDrawerPosition::TopRight>
	class DisplayFpsDrawer : public ElementDrawer
	{
	private:
		enum class DrawElementsEnum : uint8_t
		{
			CalculateFrameRate,
			Number,
			Label,
			DrawElementsCount
		};

	private:
		static constexpr uint8_t NonNumberCount = 4;

	private:
		FontRendererType TextDrawer{};

		uint32_t FramePeriodAverage = 0;
		uint16_t FrameRate = 0;

	private:
		IFrameEngine* FrameEngine;

	public:
		DisplayFpsDrawer(IFrameEngine* frameEngine)
			: ElementDrawer((uint8_t)DrawElementsEnum::DrawElementsCount)
			, FrameEngine(frameEngine)
		{
		}

		FontRendererType& GetFontRenderer()
		{
			return TextDrawer;
		}

		virtual void DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter, const uint8_t elementIndex) final
		{
			switch (elementIndex)
			{
			case (uint8_t)DrawElementsEnum::CalculateFrameRate:
				FramePeriodAverage = (FramePeriodAverage + FrameEngine->GetFrameDuration()) / 2;
				if (FramePeriodAverage > 1)
				{
					FrameRate = (uint32_t)1000000 / FramePeriodAverage;
				}
				else
				{
					FrameRate = 0;
				}
				break;
			case (uint8_t)DrawElementsEnum::Number:
				if (FrameRate > 999)
				{
					TextDrawer.TextTopLeft(frame, GetX(), GetY(), F("999+"));
				}
				else
				{
					if (FrameRate > 9)
					{
						if (FrameRate > 99)
						{
							TextDrawer.WriteDigit(frame, GetX(), GetY(), (FrameRate / 100) % 10);
						}
						TextDrawer.WriteDigit(frame, GetX() + TextWidth(1), GetY(), ((FrameRate / 10) % 10));
					}
					TextDrawer.WriteDigit(frame, GetX() + TextWidth(2), GetY(), (FrameRate % 10));
				}
				break;
			case (uint8_t)DrawElementsEnum::Label:
				TextDrawer.TextTopLeft(frame, GetX() + TextWidth(4), GetY(), F("FPS"));
				break;
			default:
				break;
			}
		}

	private:
		static constexpr pixel_t TextWidth(const uint8_t characterCount)
		{
			return (FontRendererType::FontWidth() * characterCount)
				+ (FontRendererType::FontKerning() * characterCount);
		}

		static constexpr pixel_t TextMaxWidth()
		{
			return TextWidth(7); //"999 fps"
		}

		static constexpr bool IsLeft()
		{
			return fpsDrawerPosition == FpsDrawerPosition::TopLeft || fpsDrawerPosition == FpsDrawerPosition::BottomLeft;
		}

		static constexpr bool IsTop()
		{
			return fpsDrawerPosition == FpsDrawerPosition::TopLeft || fpsDrawerPosition == FpsDrawerPosition::TopRight;
		}

		static constexpr FpsDrawerPosition DrawerPosition() { return fpsDrawerPosition; }

		static constexpr pixel_t GetX()
		{
			return ScreenLayout::X() +
				(IsLeft() ? 1 : (ScreenLayout::Width() - 1 - TextMaxWidth()));
		}

		static constexpr pixel_t GetY()
		{
			return ScreenLayout::Y() +
				(IsTop() ? 1 : (ScreenLayout::Height() - 1 - FontRendererType::Height));
		}
	};
}
#endif