#ifndef _INTEGERGLASS_FRAMEWORK_ICON_MODEL_h
#define _INTEGERGLASS_FRAMEWORK_ICON_MODEL_h

#include <stdint.h>

namespace IntegerGlass
{
	namespace Framework
	{
		/// <summary>
		/// Fixed-size collection of images, each with a unique IconEnum, that can be used as icons in a user interface.
		/// </summary>
		namespace Icon
		{
			struct IconSource
			{
				const uint8_t* Data;
				size_t Count;
			};

			enum class IconEnum : uint32_t
			{
				// Editing
				Add = 2401652699,
				Remove,
				Edit,
				Copy,
				Cut,
				Paste,
				Delete,

				// References & organization
				Attach = 294795178,
				Link,
				Pin,
				Tag,
				Bookmark,

				// Document operations
				NewFile = 3511953136,
				Save,
				Print,
				Archive,

				// Content management
				Filter = 3115492777,
				Sort,
				Sliders,
				Settings,

				// History & synchronization
				Undo = 2821732692,
				Redo,
				Refresh,
				Sync,

				// Transfer
				Import = 437026119,
				Export,
				Download,
				Upload,
				Share,

				// Zoom
				Zoom = 2433026697,
				ZoomIn,
				ZoomOut,

				// Primary navigation
				Home = 826869811,
				Menu,
				Close,
				Check,

				// Arrows
				ArrowUp = 290787205,
				ArrowDown,
				ArrowLeft,
				ArrowRight,

				// Chevrons
				ChevronUp = 1073561698,
				ChevronDown,
				ChevronLeft,
				ChevronRight,

				// Views & layout
				Expand = 2968780881,
				Grid,
				GridLarge,
				GridSmall,
				List,

				// Wireless
				AirplaneMode = 3281598472,
				Wifi0,
				Wifi1,
				Wifi2,
				Wifi3,
				WifiDisabled,
				WifiDisconnected,
				WifiEmpty = Wifi0,
				WifiFull = Wifi3,

				// Bluetooth
				Bluetooth = 4032959860,
				BluetoothDisabled,
				BluetoothDisconnected,

				// Cellular
				Cellular0 = 784134993,
				Cellular1,
				Cellular2,
				Cellular3,
				Cellular4,
				CellularDisabled,
				CellularEmpty = Cellular0,
				CellularFull = Cellular4,

				// Location
				Location = 2827710746,
				LocationDisabled,

				// Electrical power
				Power = 3277013532,
				Plug,
				Usb,
				Lightning,

				// Batteries
				Battery0 = 355739203,
				Battery1,
				Battery2,
				Battery3,
				Battery4,
				BatteryDisabled,
				BatteryCharging0,
				BatteryCharging1,
				BatteryCharging2,
				BatteryCharging3,
				BatteryCharging4,
				BatteryEmpty = Battery0,
				BatteryFull = Battery4,

				// Computers & peripherals
				Computer = 624470411,
				Desktop,
				Monitor,
				Laptop,
				Keyboard,
				Mouse,
				Camera,

				// Components & development
				Hdd = 1242572433,
				Cpu,
				Code,
				Thermometer,

				// Folders & mail
				Folder = 1243346549,
				FolderOpen,
				Mail,

				// Cloud
				Cloud = 2852887208,
				CloudUpload,
				CloudDownload,
				CloudSync,
				CloudError,
				CloudDisabled,
				CloudOk,

				// Web
				Web = 1515252187,

				// Media capture
				Image = 2056425283,
				Music,
				Video,
				VideoCamera,

				// Microphones
				Mic = 2652595342,
				MicMuted,

				// Media transport
				Play = 3953971279,
				Pause,
				Stop,
				Record,
				Rewind,
				FastForward,

				// Volume
				Volume0 = 4043783525,
				Volume1,
				Volume2,
				Volume3,
				VolumeMute,
				VolumeDisabled,

				// Notifications
				Bell = 1583492993,
				QuietHours,

				// Appearance
				BrightnessLow = 1511339089,
				BrightnessMed,
				BrightnessHigh,
				DarkMode,

				// Date & time
				Calendar = 1479815920,
				Clock,

				// Status & feedback
				Info = 1764990460,
				Question,
				Warning,
				Error,

				// Security
				Shield = 329969924,
				Lock,
				Unlock,

				// People
				User = 4179539581,
				UserAdd,
				UserRemove,
				Users,

				// Shapes
				Globe = 1581674244,
				Heart,
				Star,

				// Smileys
				Smiley = 4094253016,
				SmileyHappy,
				SmileyNeutral,
				SmileySad,
				SmileyAngry,

				// Game controllers
				GameController = 2770277527,
				GameControllerBattery0,
				GameControllerBattery1,
				GameControllerBattery2,
				GameControllerBattery3,
				GameControllerBattery4
			};
		}
	}
}
#endif