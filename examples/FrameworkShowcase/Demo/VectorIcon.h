#ifndef _DEMO_VECTORICON_h
#define _DEMO_VECTORICON_h

#include <IntegerGlassModules.h>

#include "../IDemoPage.h"
#include "../Constants.h"

namespace Showcase
{
	using namespace IntegerGlass;

	namespace VectorIcon
	{
		template<typename ParentLayout>
		using MarginLayout = Framework::Layout::Margin<
			ParentLayout,
			0,
			Constants::Showcase::Margin<ParentLayout>(),
			0,
			0>;

		using IconSetType = Modules::Kit::Icon::Vector::Silkscreen::IconSetType;
		using IconStyle = Framework::Image::TemplateImageStyle<
			Framework::Layout::AlignmentEnum::MiddleCenter>;

		template<typename ParentLayout, uint8_t Phase = 0>
		class AnimatedIconView : public Framework::Icon::Vector::IconView<
			typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
			ParentLayout,
			IconSetType,
			IconStyle>
		{
		private:
			using dimension_t = typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t;
			static constexpr dimension_t IconSize = MinValue(ParentLayout::Width(), ParentLayout::Height());

			using Base = Framework::Icon::Vector::IconView<
				dimension_t, ParentLayout, IconSetType, IconStyle>;
			using icon_enum_t = Framework::Icon::IconEnum;

		public:
			AnimatedIconView() : Base()
			{
				Base::SetIcon(GetIconByIndex(0));
				Base::SetDimensions(IconSize, IconSize);
			}

			~AnimatedIconView() = default;


		protected:
			bool ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
			{
				static constexpr uint32_t PhaseTimeOffset = (Constants::VectorIcon::IconPeriod / 4u) * Phase;
				const size_t iconIndex = ((frameTime + PhaseTimeOffset) / Constants::VectorIcon::IconPeriod) % GetIconCount();
				Base::SetIcon(GetIconByIndex(iconIndex));
				return true;
			}

			static icon_enum_t GetIconByIndex(const size_t index)
			{
				const size_t phaseOffset = (GetIconCount() * Phase) / 4;
				const size_t phasedIndex = (index + phaseOffset) % GetIconCount();

				switch (phasedIndex)
				{
				case 0: return icon_enum_t::Attach; case 1: return icon_enum_t::Add;
				case 2: return icon_enum_t::Copy; case 3: return icon_enum_t::Cut;
				case 4: return icon_enum_t::Delete; case 5: return icon_enum_t::Edit;
				case 6: return icon_enum_t::Export; case 7: return icon_enum_t::Filter;
				case 8: return icon_enum_t::Import; case 9: return icon_enum_t::Paste;
				case 10: return icon_enum_t::Pin; case 11: return icon_enum_t::Redo;
				case 12: return icon_enum_t::Refresh; case 13: return icon_enum_t::Save;
				case 14: return icon_enum_t::Settings; case 15: return icon_enum_t::Link;
				case 16: return icon_enum_t::Share; case 17: return icon_enum_t::Sort;
				case 18: return icon_enum_t::Tag; case 19: return icon_enum_t::Undo;
				case 20: return icon_enum_t::Remove; case 21: return icon_enum_t::Zoom;
				case 22: return icon_enum_t::ZoomIn; case 23: return icon_enum_t::ZoomOut;
				case 24: return icon_enum_t::Sliders; case 25: return icon_enum_t::Battery0;
				case 26: return icon_enum_t::Battery1; case 27: return icon_enum_t::Battery2;
				case 28: return icon_enum_t::Battery3; case 29: return icon_enum_t::Battery4;
				case 30: return icon_enum_t::BatteryDisabled; case 31: return icon_enum_t::BatteryCharging0;
				case 32: return icon_enum_t::BatteryCharging1; case 33: return icon_enum_t::BatteryCharging2;
				case 34: return icon_enum_t::BatteryCharging3; case 35: return icon_enum_t::BatteryCharging4;
				case 36: return icon_enum_t::Bluetooth; case 37: return icon_enum_t::BluetoothDisabled;
				case 38: return icon_enum_t::BluetoothDisconnected; case 39: return icon_enum_t::Cellular0;
				case 40: return icon_enum_t::Cellular1; case 41: return icon_enum_t::Cellular2;
				case 42: return icon_enum_t::Cellular3; case 43: return icon_enum_t::Cellular4;
				case 44: return icon_enum_t::CellularDisabled; case 45: return icon_enum_t::AirplaneMode;
				case 46: return icon_enum_t::Location; case 47: return icon_enum_t::LocationDisabled;
				case 48: return icon_enum_t::Sync; case 49: return icon_enum_t::Computer;
				case 50: return icon_enum_t::Laptop; case 51: return icon_enum_t::Cpu;
				case 52: return icon_enum_t::Code; case 53: return icon_enum_t::Desktop;
				case 54: return icon_enum_t::Monitor; case 55: return icon_enum_t::Hdd;
				case 56: return icon_enum_t::Keyboard; case 57: return icon_enum_t::Lightning;
				case 58: return icon_enum_t::Mouse; case 59: return icon_enum_t::Plug;
				case 60: return icon_enum_t::Thermometer; case 61: return icon_enum_t::Power;
				case 62: return icon_enum_t::Usb; case 63: return icon_enum_t::Wifi0;
				case 64: return icon_enum_t::Wifi1; case 65: return icon_enum_t::Wifi2;
				case 66: return icon_enum_t::Wifi3; case 67: return icon_enum_t::WifiDisabled;
				case 68: return icon_enum_t::WifiDisconnected; case 69: return icon_enum_t::ArrowDown;
				case 70: return icon_enum_t::ArrowLeft; case 71: return icon_enum_t::ArrowRight;
				case 72: return icon_enum_t::ArrowUp; case 73: return icon_enum_t::Check;
				case 74: return icon_enum_t::ChevronDown; case 75: return icon_enum_t::ChevronLeft;
				case 76: return icon_enum_t::ChevronRight; case 77: return icon_enum_t::ChevronUp;
				case 78: return icon_enum_t::Close; case 79: return icon_enum_t::Expand;
				case 80: return icon_enum_t::Grid; case 81: return icon_enum_t::GridSmall;
				case 82: return icon_enum_t::GridLarge; case 83: return icon_enum_t::Home;
				case 84: return icon_enum_t::List; case 85: return icon_enum_t::Menu;
				case 86: return icon_enum_t::Archive; case 87: return icon_enum_t::Cloud;
				case 88: return icon_enum_t::CloudError; case 89: return icon_enum_t::CloudDisabled;
				case 90: return icon_enum_t::CloudOk; case 91: return icon_enum_t::CloudUpload;
				case 92: return icon_enum_t::CloudDownload; case 93: return icon_enum_t::CloudSync;
				case 94: return icon_enum_t::Download; case 95: return icon_enum_t::Folder;
				case 96: return icon_enum_t::FolderOpen; case 97: return icon_enum_t::NewFile;
				case 98: return icon_enum_t::Mail; case 99: return icon_enum_t::Print;
				case 100: return icon_enum_t::Upload; case 101: return icon_enum_t::Camera;
				case 102: return icon_enum_t::FastForward; case 103: return icon_enum_t::Image;
				case 104: return icon_enum_t::Music; case 105: return icon_enum_t::Mic;
				case 106: return icon_enum_t::MicMuted; case 107: return icon_enum_t::Pause;
				case 108: return icon_enum_t::Play; case 109: return icon_enum_t::Record;
				case 110: return icon_enum_t::Rewind; case 111: return icon_enum_t::Stop;
				case 112: return icon_enum_t::Video; case 113: return icon_enum_t::VideoCamera;
				case 114: return icon_enum_t::Volume0; case 115: return icon_enum_t::Volume1;
				case 116: return icon_enum_t::Volume2; case 117: return icon_enum_t::Volume3;
				case 118: return icon_enum_t::VolumeMute; case 119: return icon_enum_t::VolumeDisabled;
				case 120: return icon_enum_t::Bell; case 121: return icon_enum_t::Bookmark;
				case 122: return icon_enum_t::BrightnessHigh; case 123: return icon_enum_t::BrightnessLow;
				case 124: return icon_enum_t::BrightnessMed; case 125: return icon_enum_t::Calendar;
				case 126: return icon_enum_t::Clock; case 127: return icon_enum_t::QuietHours;
				case 128: return icon_enum_t::DarkMode; case 129: return icon_enum_t::Error;
				case 130: return icon_enum_t::GameController; case 131: return icon_enum_t::GameControllerBattery0;
				case 132: return icon_enum_t::GameControllerBattery1; case 133: return icon_enum_t::GameControllerBattery2;
				case 134: return icon_enum_t::GameControllerBattery3; case 135: return icon_enum_t::GameControllerBattery4;
				case 136: return icon_enum_t::Globe; case 137: return icon_enum_t::Web;
				case 138: return icon_enum_t::Heart; case 139: return icon_enum_t::Info;
				case 140: return icon_enum_t::Lock; case 141: return icon_enum_t::Question;
				case 142: return icon_enum_t::Shield; case 143: return icon_enum_t::Smiley;
				case 144: return icon_enum_t::SmileyHappy; case 145: return icon_enum_t::SmileyNeutral;
				case 146: return icon_enum_t::SmileySad; case 147: return icon_enum_t::SmileyAngry;
				case 148: return icon_enum_t::Star; case 149: return icon_enum_t::Unlock;
				case 150: return icon_enum_t::User; case 151: return icon_enum_t::UserAdd;
				case 152: return icon_enum_t::UserRemove; case 153: return icon_enum_t::Users;
				case 154: return icon_enum_t::Warning;
				default:
					return icon_enum_t::Attach;
				}
			}

			static constexpr size_t GetIconCount()
			{
				return 155;
			}
		};

		template<typename ParentLayout>
		using QuadCell = Framework::Layout::Grid<MarginLayout<ParentLayout>, 2, 2>;

		template<typename ParentLayout, bool Monochrome = false>
		class View : public IDemoPage, public Framework::View::CompositeView<
			ParentLayout,
			AnimatedIconView<typename QuadCell<ParentLayout>::template Cell<0, 0>, 0>,
			AnimatedIconView<typename QuadCell<ParentLayout>::template Cell<1, 0>, 1>,
			AnimatedIconView<typename QuadCell<ParentLayout>::template Cell<0, 1>, 2>,
			AnimatedIconView<typename QuadCell<ParentLayout>::template Cell<1, 1>, 3>>
		{
		private:
			using Base = Framework::View::CompositeView<
				ParentLayout,
				AnimatedIconView<typename QuadCell<ParentLayout>::template Cell<0, 0>, 0>,
				AnimatedIconView<typename QuadCell<ParentLayout>::template Cell<1, 0>, 1>,
				AnimatedIconView<typename QuadCell<ParentLayout>::template Cell<0, 1>, 2>,
				AnimatedIconView<typename QuadCell<ParentLayout>::template Cell<1, 1>, 3>>;

		public:
			using ViewLayout = ParentLayout;

			View() : IDemoPage(), Base() {}
			~View() = default;

			const char* GetPageName() const override { return "Vector Icon"; }
		};
	}
}
#endif