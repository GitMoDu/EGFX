#ifndef _CONSTANTS_h
#define _CONSTANTS_h

#include <IntegerGlassModules.h>

namespace Showcase
{
	using namespace IntegerGlass;

	namespace Constants
	{
		namespace Showcase
		{
			using DecoratorFontType = Modules::Kit::Font::Vector::Epoxy::FontType8x8;

			template<typename ParentLayout>
			static constexpr int16_t TitleHeight()
			{
				return MinValue<int16_t>(
					(ParentLayout::Height() * 20) / 100,
					(ParentLayout::Width() * 9) / 100);
			}

			template<typename ParentLayout>
			static constexpr int16_t StripHeight()
			{
				return MaxValue<int16_t>(
					DecoratorFontType::Height + 2
					, (ParentLayout::Height() * 17) / 100);
			}

			template<typename ParentLayout>
			static constexpr int16_t Margin()
			{
				return StripHeight<ParentLayout>() / 3;
			}

			static constexpr uint32_t PageDemoDuration = 5000000;
			static constexpr uint32_t PageSlideDuration = 1700000;
			static constexpr uint32_t SplashDuration = 7000000;
			static constexpr uint32_t SplashStartDelay = 500000;
			static constexpr uint32_t SplashEndDelay = 500000;
			static constexpr uint32_t TitleDuration = 2500000;
			static constexpr uint32_t OutroDuration = 1000000;

			using AnimationCurveType = Modules::Kit::Animation::Interpolator::AnticipateOvershoot<>;

			template<typename ParentLayout>
			using PageSlideStyle = Framework::Projection::PageSlideStyle<
				Framework::Projection::OrientationEnum::LeftToRight,
				0,
				0,
				0,
				Margin<ParentLayout>() * 2>;

			template<typename ParentLayout>
			using Projector = Framework::Projection::Projectors::PageSlide::Projector<
				ParentLayout,
				PageSlideStyle<ParentLayout>>;
		}



		namespace DogeDimension
		{
			using namespace Modules::ExampleImages;

			static constexpr uint16_t Width = MaxValue(Images::Bitmask::Doge::SourceType::Width, Images::Bitmap::Doge::SourceType::Width);
			static constexpr uint16_t Height = MaxValue(Images::Bitmask::Doge::SourceType::Height, Images::Bitmap::Doge::SourceType::Height);
			static constexpr uint16_t MaxDimension = MaxValue(Width, Height);

			// Automatic fit to parent layout, with 1/5 padding to avoid clipping during rotation.
			template<typename ParentLayout>
			struct Scale
			{
				// Add some padding so rotation doesn't clip the image.
				static constexpr uint16_t PaddedDimension() { return (DogeDimension::MaxDimension * 5) / 4; }

				static constexpr int16_t Dimension() { return MinValue(ParentLayout::Height(), ParentLayout::Width()); }
				static constexpr uint8_t Value = MaxValue<uint8_t>(1, Dimension() / PaddedDimension());
			};
		}

		namespace Background
		{
			static constexpr rgb_color_t Color = Rgb::Color(uint32_t(0x060030));
			static constexpr uint32_t AnimationDuration = 40000000;
			static constexpr uint32_t FadeInDuration = Showcase::OutroDuration * 3;
			static constexpr uint32_t FadeOutDuration = Showcase::OutroDuration;
			static constexpr uint32_t ColorTimeOffset = 10000000;
			static constexpr uint8_t Saturation = 190;
			static constexpr uint8_t Value = 85;
		}

		namespace VectorIcon
		{
			static constexpr uint32_t IconPeriod = 600000;
		}

		namespace Image
		{
			static constexpr uint32_t RotationPeriod = 5000000;
		}

		namespace VectorImage
		{
			static constexpr uint32_t SizePeriod = 3000000;
		}

		namespace Text
		{
			static constexpr uint32_t ScrollPeriod = 7000000;
			static constexpr uint32_t VectorSizePeriod = 3000000;
		}

		namespace Terminal
		{
			static constexpr uint32_t IntervalBase = 100000;
			static constexpr uint32_t IntervalVariable = 1500000;
		}

		namespace ProgressIndicator
		{
			static constexpr uint32_t CyclePeriod = 4100000;
		}

		namespace BatteryIndicator
		{
			static constexpr int16_t VerticalMargin = 4;
			static constexpr int16_t AspectRatioWidth = 22;
			static constexpr int16_t AspectRatioHeight = 9;

		}

		namespace Decorator
		{
			static constexpr UFraction16::scalar_t ShadowOpacity = UFraction16::GetScalar<int32_t>(90, 100);
			static constexpr rgb_color_t TextColor = Rgb::Color(uint32_t(0xFFFFFF));
			static constexpr rgb_color_t ShadowColor = Rgb::Color(uint32_t(0x000000));
		}

		namespace PageSlider
		{
			template<typename ParentLayout>
			static constexpr int16_t Margin()
			{
				return Showcase::Margin<ParentLayout>();
			}

			template<typename ParentLayout>
			static constexpr int16_t MarginBottom()
			{
				return Showcase::Margin<ParentLayout>();
			}
		}

		namespace Strings
		{
			static constexpr char GraphicsTitle[] PROGMEM = "Graphics";
			static constexpr char ModulesTitle[] PROGMEM = "Modules";

			static constexpr char FoxDoge[] PROGMEM = "The quick brown fox jumps over the lazy Doge";

			static constexpr const char* DemoLines[] =
			{
				"BOOT_SEQ: START",
				"CPU_0: ONLINE\nCORE_1: IDLE",
				"MOUNTING /dev/sda1...",
				"FILESYSTEM CHECK OK",
				"LOADING KERNEL MODULES...",
				"NET_IFACE: eth0 UP\nIP: 192.168.1.10",
				"DHCP ACQUIRED",
				"ESTABLISHING SECURE SOCKET...",
				"HANDSHAKE: TLSv1.3\nCIPHER: AES_256_GCM",
				"VERIFYING CERTIFICATE...",
				"AUTH SUCCESS: ROOT",
				"SCANNING PORTS...\n[ 80/tcp OPEN ]\n[ 443/tcp OPEN ]",
				"BYPASSING PROXY...",
				"NODE DISCOVERY: 4 PEERS",
				"SYNCING BLOCK 0x4F8A",
				"INTEGRITY CHECK PASSED",
				"DAEMON: RUNNING",
				"LISTENING ON :8080\nSTATUS: LISTENING",
				"STREAM_BUFFER: 512KB",
				"ALLOCATING HEAP...",
				"GARBAGE COLLECTOR ACTIVE",
				"INJECTING PAYLOAD...",
				"DEBUG: TRACE_ENABLED",
				"PING 10.0.0.1 -c 1\nREPLY: 12ms",
				"ROUTE_TABLE: UPDATED",
				"FIREWALL: ACTIVE\nRULES: 142 LOADED",
				"WARNING: HIGH_LOAD",
				"OPTIMIZING THREADS...",
				"CACHE FLUSHED",
				"TERMINAL: READY"
			};
		}
	}
}
#endif