#ifndef _EGFX_MODULES_KIT_ICON_VECTOR_SILKSCREEN_ICON_SET_TYPE_h
#define _EGFX_MODULES_KIT_ICON_VECTOR_SILKSCREEN_ICON_SET_TYPE_h

#include <EgfxFramework.h>
#include "Source.h"

namespace Egfx
{
	namespace Modules
	{
		namespace Kit
		{
			namespace Icon
			{
				namespace Vector
				{
					/// <summary>
					/// Vector icon set in outlines only style, with a mix of old and modern style icons.
					/// Primitives used: line, rectangle, rectangle fill, triangle, circle, arc and arcill.
					/// </summary>
					namespace Silkscreen
					{
						using IconEnum = Egfx::Framework::Icon::IconEnum;
						using IconSource = Egfx::Framework::Icon::IconSource;


						struct IconSetType
						{
							static constexpr Egfx::Framework::DataSourceTypeEnum DataSourceType = Egfx::Framework::DataSourceTypeEnum::Flash;

							static constexpr uint8_t CanvasWidth = 15;
							static constexpr uint8_t CanvasHeight = 15;
							static constexpr uint8_t EdgeInset = 0;

							static constexpr Egfx::Framework::Vector::PackedVectorEnum VectorType =
								Egfx::Framework::Vector::PackedVectorEnum::Image15x15;

							enum class SetIconEnum : uint32_t
							{
								Attach = uint32_t(IconEnum::Attach),
								Add = uint32_t(IconEnum::Add),
								Copy = uint32_t(IconEnum::Copy),
								Cut = uint32_t(IconEnum::Cut),
								Delete = uint32_t(IconEnum::Delete),
								Edit = uint32_t(IconEnum::Edit),
								Export = uint32_t(IconEnum::Export),
								Filter = uint32_t(IconEnum::Filter),
								Import = uint32_t(IconEnum::Import),
								Paste = uint32_t(IconEnum::Paste),
								Pin = uint32_t(IconEnum::Pin),
								Redo = uint32_t(IconEnum::Redo),
								Refresh = uint32_t(IconEnum::Refresh),
								Save = uint32_t(IconEnum::Save),
								Settings = uint32_t(IconEnum::Settings),
								Link = uint32_t(IconEnum::Link),
								Share = uint32_t(IconEnum::Share),
								Sort = uint32_t(IconEnum::Sort),
								Tag = uint32_t(IconEnum::Tag),
								Undo = uint32_t(IconEnum::Undo),
								Remove = uint32_t(IconEnum::Remove),
								Zoom = uint32_t(IconEnum::Zoom),
								ZoomIn = uint32_t(IconEnum::ZoomIn),
								ZoomOut = uint32_t(IconEnum::ZoomOut),
								Sliders = uint32_t(IconEnum::Sliders),
								Battery0 = uint32_t(IconEnum::Battery0),
								Battery1 = uint32_t(IconEnum::Battery1),
								Battery2 = uint32_t(IconEnum::Battery2),
								Battery3 = uint32_t(IconEnum::Battery3),
								Battery4 = uint32_t(IconEnum::Battery4),
								BatteryDisabled = uint32_t(IconEnum::BatteryDisabled),
								BatteryCharging0 = uint32_t(IconEnum::BatteryCharging0),
								BatteryCharging1 = uint32_t(IconEnum::BatteryCharging1),
								BatteryCharging2 = uint32_t(IconEnum::BatteryCharging2),
								BatteryCharging3 = uint32_t(IconEnum::BatteryCharging3),
								BatteryCharging4 = uint32_t(IconEnum::BatteryCharging4),
								Bluetooth = uint32_t(IconEnum::Bluetooth),
								BluetoothDisabled = uint32_t(IconEnum::BluetoothDisabled),
								BluetoothDisconnected = uint32_t(IconEnum::BluetoothDisconnected),
								Cellular0 = uint32_t(IconEnum::Cellular0),
								Cellular1 = uint32_t(IconEnum::Cellular1),
								Cellular2 = uint32_t(IconEnum::Cellular2),
								Cellular3 = uint32_t(IconEnum::Cellular3),
								Cellular4 = uint32_t(IconEnum::Cellular4),
								CellularDisabled = uint32_t(IconEnum::CellularDisabled),
								AirplaneMode = uint32_t(IconEnum::AirplaneMode),
								Location = uint32_t(IconEnum::Location),
								LocationDisabled = uint32_t(IconEnum::LocationDisabled),
								Sync = uint32_t(IconEnum::Sync),
								Computer = uint32_t(IconEnum::Computer),
								Laptop = uint32_t(IconEnum::Laptop),
								Cpu = uint32_t(IconEnum::Cpu),
								Code = uint32_t(IconEnum::Code),
								Desktop = uint32_t(IconEnum::Desktop),
								Monitor = uint32_t(IconEnum::Monitor),
								Hdd = uint32_t(IconEnum::Hdd),
								Keyboard = uint32_t(IconEnum::Keyboard),
								Lightning = uint32_t(IconEnum::Lightning),
								Mouse = uint32_t(IconEnum::Mouse),
								Plug = uint32_t(IconEnum::Plug),
								Thermometer = uint32_t(IconEnum::Thermometer),
								Power = uint32_t(IconEnum::Power),
								Usb = uint32_t(IconEnum::Usb),
								Wifi0 = uint32_t(IconEnum::Wifi0),
								Wifi1 = uint32_t(IconEnum::Wifi1),
								Wifi2 = uint32_t(IconEnum::Wifi2),
								Wifi3 = uint32_t(IconEnum::Wifi3),
								WifiDisabled = uint32_t(IconEnum::WifiDisabled),
								WifiDisconnected = uint32_t(IconEnum::WifiDisconnected),
								ArrowDown = uint32_t(IconEnum::ArrowDown),
								ArrowLeft = uint32_t(IconEnum::ArrowLeft),
								ArrowRight = uint32_t(IconEnum::ArrowRight),
								ArrowUp = uint32_t(IconEnum::ArrowUp),
								Check = uint32_t(IconEnum::Check),
								ChevronDown = uint32_t(IconEnum::ChevronDown),
								ChevronLeft = uint32_t(IconEnum::ChevronLeft),
								ChevronRight = uint32_t(IconEnum::ChevronRight),
								ChevronUp = uint32_t(IconEnum::ChevronUp),
								Close = uint32_t(IconEnum::Close),
								Expand = uint32_t(IconEnum::Expand),
								Grid = uint32_t(IconEnum::Grid),
								GridSmall = uint32_t(IconEnum::GridSmall),
								GridLarge = uint32_t(IconEnum::GridLarge),
								Home = uint32_t(IconEnum::Home),
								List = uint32_t(IconEnum::List),
								Menu = uint32_t(IconEnum::Menu),
								Archive = uint32_t(IconEnum::Archive),
								Cloud = uint32_t(IconEnum::Cloud),
								CloudError = uint32_t(IconEnum::CloudError),
								CloudDisabled = uint32_t(IconEnum::CloudDisabled),
								CloudOk = uint32_t(IconEnum::CloudOk),
								CloudUpload = uint32_t(IconEnum::CloudUpload),
								CloudDownload = uint32_t(IconEnum::CloudDownload),
								CloudSync = uint32_t(IconEnum::CloudSync),
								Download = uint32_t(IconEnum::Download),
								Folder = uint32_t(IconEnum::Folder),
								FolderOpen = uint32_t(IconEnum::FolderOpen),
								NewFile = uint32_t(IconEnum::NewFile),
								Mail = uint32_t(IconEnum::Mail),
								Print = uint32_t(IconEnum::Print),
								Upload = uint32_t(IconEnum::Upload),
								Camera = uint32_t(IconEnum::Camera),
								FastForward = uint32_t(IconEnum::FastForward),
								Image = uint32_t(IconEnum::Image),
								Music = uint32_t(IconEnum::Music),
								Mic = uint32_t(IconEnum::Mic),
								MicMuted = uint32_t(IconEnum::MicMuted),
								Pause = uint32_t(IconEnum::Pause),
								Play = uint32_t(IconEnum::Play),
								Record = uint32_t(IconEnum::Record),
								Rewind = uint32_t(IconEnum::Rewind),
								Stop = uint32_t(IconEnum::Stop),
								Video = uint32_t(IconEnum::Video),
								VideoCamera = uint32_t(IconEnum::VideoCamera),
								Volume0 = uint32_t(IconEnum::Volume0),
								Volume1 = uint32_t(IconEnum::Volume1),
								Volume2 = uint32_t(IconEnum::Volume2),
								Volume3 = uint32_t(IconEnum::Volume3),
								VolumeMute = uint32_t(IconEnum::VolumeMute),
								VolumeDisabled = uint32_t(IconEnum::VolumeDisabled),
								Bell = uint32_t(IconEnum::Bell),
								Bookmark = uint32_t(IconEnum::Bookmark),
								BrightnessHigh = uint32_t(IconEnum::BrightnessHigh),
								BrightnessLow = uint32_t(IconEnum::BrightnessLow),
								BrightnessMed = uint32_t(IconEnum::BrightnessMed),
								Calendar = uint32_t(IconEnum::Calendar),
								Clock = uint32_t(IconEnum::Clock),
								QuietHours = uint32_t(IconEnum::QuietHours),
								DarkMode = uint32_t(IconEnum::DarkMode),
								Error = uint32_t(IconEnum::Error),
								GameController = uint32_t(IconEnum::GameController),
								GameControllerBattery0 = uint32_t(IconEnum::GameControllerBattery0),
								GameControllerBattery1 = uint32_t(IconEnum::GameControllerBattery1),
								GameControllerBattery2 = uint32_t(IconEnum::GameControllerBattery2),
								GameControllerBattery3 = uint32_t(IconEnum::GameControllerBattery3),
								GameControllerBattery4 = uint32_t(IconEnum::GameControllerBattery4),
								Globe = uint32_t(IconEnum::Globe),
								Web = uint32_t(IconEnum::Web),
								Heart = uint32_t(IconEnum::Heart),
								Info = uint32_t(IconEnum::Info),
								Lock = uint32_t(IconEnum::Lock),
								Question = uint32_t(IconEnum::Question),
								Shield = uint32_t(IconEnum::Shield),
								Smiley = uint32_t(IconEnum::Smiley),
								SmileyHappy = uint32_t(IconEnum::SmileyHappy),
								SmileyNeutral = uint32_t(IconEnum::SmileyNeutral),
								SmileySad = uint32_t(IconEnum::SmileySad),
								SmileyAngry = uint32_t(IconEnum::SmileyAngry),
								Star = uint32_t(IconEnum::Star),
								Unlock = uint32_t(IconEnum::Unlock),
								User = uint32_t(IconEnum::User),
								UserAdd = uint32_t(IconEnum::UserAdd),
								UserRemove = uint32_t(IconEnum::UserRemove),
								Users = uint32_t(IconEnum::Users),
								Warning = uint32_t(IconEnum::Warning),
							};

							static IconSource GetIcon(const Egfx::Framework::Icon::IconEnum icon)
							{
								switch (icon)
								{
								case Egfx::Framework::Icon::IconEnum::Attach: return { Atlas + 0, 25 };
								case Egfx::Framework::Icon::IconEnum::Add: return { Atlas + 25, 5 };
								case Egfx::Framework::Icon::IconEnum::Copy: return { Atlas + 30, 16 };
								case Egfx::Framework::Icon::IconEnum::Cut: return { Atlas + 46, 10 };
								case Egfx::Framework::Icon::IconEnum::Delete: return { Atlas + 56, 35 };
								case Egfx::Framework::Icon::IconEnum::Edit: return { Atlas + 91, 9 };
								case Egfx::Framework::Icon::IconEnum::Export: return { Atlas + 100, 18 };
								case Egfx::Framework::Icon::IconEnum::Filter: return { Atlas + 118, 7 };
								case Egfx::Framework::Icon::IconEnum::Import: return { Atlas + 125, 20 };
								case Egfx::Framework::Icon::IconEnum::Paste: return { Atlas + 145, 24 };
								case Egfx::Framework::Icon::IconEnum::Pin: return { Atlas + 169, 19 };
								case Egfx::Framework::Icon::IconEnum::Redo: return { Atlas + 188, 9 };
								case Egfx::Framework::Icon::IconEnum::Refresh: return { Atlas + 197, 7 };
								case Egfx::Framework::Icon::IconEnum::Save: return { Atlas + 204, 16 };
								case Egfx::Framework::Icon::IconEnum::Settings: return { Atlas + 220, 24 };
								case Egfx::Framework::Icon::IconEnum::Link: return { Atlas + 244, 20 };
								case Egfx::Framework::Icon::IconEnum::Share: return { Atlas + 264, 13 };
								case Egfx::Framework::Icon::IconEnum::Sort: return { Atlas + 277, 13 };
								case Egfx::Framework::Icon::IconEnum::Tag: return { Atlas + 290, 13 };
								case Egfx::Framework::Icon::IconEnum::Undo: return { Atlas + 303, 9 };
								case Egfx::Framework::Icon::IconEnum::Remove: return { Atlas + 312, 2 };
								case Egfx::Framework::Icon::IconEnum::Zoom: return { Atlas + 314, 5 };
								case Egfx::Framework::Icon::IconEnum::ZoomIn: return { Atlas + 319, 10 };
								case Egfx::Framework::Icon::IconEnum::ZoomOut: return { Atlas + 329, 7 };
								case Egfx::Framework::Icon::IconEnum::Sliders: return { Atlas + 336, 23 };
								case Egfx::Framework::Icon::IconEnum::Battery0: return { Atlas + 359, 6 };
								case Egfx::Framework::Icon::IconEnum::Battery1: return { Atlas + 365, 9 };
								case Egfx::Framework::Icon::IconEnum::Battery2: return { Atlas + 374, 9 };
								case Egfx::Framework::Icon::IconEnum::Battery3: return { Atlas + 383, 9 };
								case Egfx::Framework::Icon::IconEnum::Battery4: return { Atlas + 392, 9 };
								case Egfx::Framework::Icon::IconEnum::BatteryDisabled: return { Atlas + 401, 15 };
								case Egfx::Framework::Icon::IconEnum::BatteryCharging0: return { Atlas + 416, 14 };
								case Egfx::Framework::Icon::IconEnum::BatteryCharging1: return { Atlas + 430, 17 };
								case Egfx::Framework::Icon::IconEnum::BatteryCharging2: return { Atlas + 447, 31 };
								case Egfx::Framework::Icon::IconEnum::BatteryCharging3: return { Atlas + 478, 31 };
								case Egfx::Framework::Icon::IconEnum::BatteryCharging4: return { Atlas + 509, 34 };
								case Egfx::Framework::Icon::IconEnum::Bluetooth: return { Atlas + 543, 22 };
								case Egfx::Framework::Icon::IconEnum::BluetoothDisabled: return { Atlas + 565, 39 };
								case Egfx::Framework::Icon::IconEnum::BluetoothDisconnected: return { Atlas + 604, 24 };
								case Egfx::Framework::Icon::IconEnum::Cellular0: return { Atlas + 628, 2 };
								case Egfx::Framework::Icon::IconEnum::Cellular1: return { Atlas + 630, 5 };
								case Egfx::Framework::Icon::IconEnum::Cellular2: return { Atlas + 635, 8 };
								case Egfx::Framework::Icon::IconEnum::Cellular3: return { Atlas + 643, 11 };
								case Egfx::Framework::Icon::IconEnum::Cellular4: return { Atlas + 654, 14 };
								case Egfx::Framework::Icon::IconEnum::CellularDisabled: return { Atlas + 668, 26 };
								case Egfx::Framework::Icon::IconEnum::AirplaneMode: return { Atlas + 694, 21 };
								case Egfx::Framework::Icon::IconEnum::Location: return { Atlas + 715, 10 };
								case Egfx::Framework::Icon::IconEnum::LocationDisabled: return { Atlas + 725, 21 };
								case Egfx::Framework::Icon::IconEnum::Sync: return { Atlas + 746, 14 };
								case Egfx::Framework::Icon::IconEnum::Computer: return { Atlas + 760, 17 };
								case Egfx::Framework::Icon::IconEnum::Laptop: return { Atlas + 777, 31 };
								case Egfx::Framework::Icon::IconEnum::Cpu: return { Atlas + 808, 50 };
								case Egfx::Framework::Icon::IconEnum::Code: return { Atlas + 858, 13 };
								case Egfx::Framework::Icon::IconEnum::Desktop: return { Atlas + 871, 18 };
								case Egfx::Framework::Icon::IconEnum::Monitor: return { Atlas + 889, 7 };
								case Egfx::Framework::Icon::IconEnum::Hdd: return { Atlas + 896, 19 };
								case Egfx::Framework::Icon::IconEnum::Keyboard: return { Atlas + 915, 22 };
								case Egfx::Framework::Icon::IconEnum::Lightning: return { Atlas + 937, 8 };
								case Egfx::Framework::Icon::IconEnum::Mouse: return { Atlas + 945, 16 };
								case Egfx::Framework::Icon::IconEnum::Plug: return { Atlas + 961, 31 };
								case Egfx::Framework::Icon::IconEnum::Thermometer: return { Atlas + 992, 34 };
								case Egfx::Framework::Icon::IconEnum::Power: return { Atlas + 1026, 6 };
								case Egfx::Framework::Icon::IconEnum::Usb: return { Atlas + 1032, 24 };
								case Egfx::Framework::Icon::IconEnum::Wifi0: return { Atlas + 1056, 3 };
								case Egfx::Framework::Icon::IconEnum::Wifi1: return { Atlas + 1059, 7 };
								case Egfx::Framework::Icon::IconEnum::Wifi2: return { Atlas + 1066, 11 };
								case Egfx::Framework::Icon::IconEnum::Wifi3: return { Atlas + 1077, 15 };
								case Egfx::Framework::Icon::IconEnum::WifiDisabled: return { Atlas + 1092, 29 };
								case Egfx::Framework::Icon::IconEnum::WifiDisconnected: return { Atlas + 1121, 19 };
								case Egfx::Framework::Icon::IconEnum::ArrowDown: return { Atlas + 1140, 6 };
								case Egfx::Framework::Icon::IconEnum::ArrowLeft: return { Atlas + 1146, 6 };
								case Egfx::Framework::Icon::IconEnum::ArrowRight: return { Atlas + 1152, 6 };
								case Egfx::Framework::Icon::IconEnum::ArrowUp: return { Atlas + 1158, 6 };
								case Egfx::Framework::Icon::IconEnum::Check: return { Atlas + 1164, 3 };
								case Egfx::Framework::Icon::IconEnum::ChevronDown: return { Atlas + 1167, 3 };
								case Egfx::Framework::Icon::IconEnum::ChevronLeft: return { Atlas + 1170, 3 };
								case Egfx::Framework::Icon::IconEnum::ChevronRight: return { Atlas + 1173, 3 };
								case Egfx::Framework::Icon::IconEnum::ChevronUp: return { Atlas + 1176, 3 };
								case Egfx::Framework::Icon::IconEnum::Close: return { Atlas + 1179, 5 };
								case Egfx::Framework::Icon::IconEnum::Expand: return { Atlas + 1184, 10 };
								case Egfx::Framework::Icon::IconEnum::Grid: return { Atlas + 1194, 12 };
								case Egfx::Framework::Icon::IconEnum::GridSmall: return { Atlas + 1206, 12 };
								case Egfx::Framework::Icon::IconEnum::GridLarge: return { Atlas + 1218, 12 };
								case Egfx::Framework::Icon::IconEnum::Home: return { Atlas + 1230, 12 };
								case Egfx::Framework::Icon::IconEnum::List: return { Atlas + 1242, 19 };
								case Egfx::Framework::Icon::IconEnum::Menu: return { Atlas + 1261, 8 };
								case Egfx::Framework::Icon::IconEnum::Archive: return { Atlas + 1269, 19 };
								case Egfx::Framework::Icon::IconEnum::Cloud: return { Atlas + 1288, 28 };
								case Egfx::Framework::Icon::IconEnum::CloudError: return { Atlas + 1316, 32 };
								case Egfx::Framework::Icon::IconEnum::CloudDisabled: return { Atlas + 1348, 29 };
								case Egfx::Framework::Icon::IconEnum::CloudOk: return { Atlas + 1377, 31 };
								case Egfx::Framework::Icon::IconEnum::CloudUpload: return { Atlas + 1408, 33 };
								case Egfx::Framework::Icon::IconEnum::CloudDownload: return { Atlas + 1441, 33 };
								case Egfx::Framework::Icon::IconEnum::CloudSync: return { Atlas + 1474, 42 };
								case Egfx::Framework::Icon::IconEnum::Download: return { Atlas + 1516, 11 };
								case Egfx::Framework::Icon::IconEnum::Folder: return { Atlas + 1527, 11 };
								case Egfx::Framework::Icon::IconEnum::FolderOpen: return { Atlas + 1538, 12 };
								case Egfx::Framework::Icon::IconEnum::NewFile: return { Atlas + 1550, 16 };
								case Egfx::Framework::Icon::IconEnum::Mail: return { Atlas + 1566, 8 };
								case Egfx::Framework::Icon::IconEnum::Print: return { Atlas + 1574, 22 };
								case Egfx::Framework::Icon::IconEnum::Upload: return { Atlas + 1596, 11 };
								case Egfx::Framework::Icon::IconEnum::Camera: return { Atlas + 1607, 12 };
								case Egfx::Framework::Icon::IconEnum::FastForward: return { Atlas + 1619, 9 };
								case Egfx::Framework::Icon::IconEnum::Image: return { Atlas + 1628, 11 };
								case Egfx::Framework::Icon::IconEnum::Music: return { Atlas + 1639, 10 };
								case Egfx::Framework::Icon::IconEnum::Mic: return { Atlas + 1649, 14 };
								case Egfx::Framework::Icon::IconEnum::MicMuted: return { Atlas + 1663, 25 };
								case Egfx::Framework::Icon::IconEnum::Pause: return { Atlas + 1688, 6 };
								case Egfx::Framework::Icon::IconEnum::Play: return { Atlas + 1694, 4 };
								case Egfx::Framework::Icon::IconEnum::Record: return { Atlas + 1698, 6 };
								case Egfx::Framework::Icon::IconEnum::Rewind: return { Atlas + 1704, 9 };
								case Egfx::Framework::Icon::IconEnum::Stop: return { Atlas + 1713, 3 };
								case Egfx::Framework::Icon::IconEnum::Video: return { Atlas + 1716, 72 };
								case Egfx::Framework::Icon::IconEnum::VideoCamera: return { Atlas + 1788, 7 };
								case Egfx::Framework::Icon::IconEnum::Volume0: return { Atlas + 1795, 7 };
								case Egfx::Framework::Icon::IconEnum::Volume1: return { Atlas + 1802, 11 };
								case Egfx::Framework::Icon::IconEnum::Volume2: return { Atlas + 1813, 15 };
								case Egfx::Framework::Icon::IconEnum::Volume3: return { Atlas + 1828, 19 };
								case Egfx::Framework::Icon::IconEnum::VolumeMute: return { Atlas + 1847, 13 };
								case Egfx::Framework::Icon::IconEnum::VolumeDisabled: return { Atlas + 1860, 23 };
								case Egfx::Framework::Icon::IconEnum::Bell: return { Atlas + 1883, 14 };
								case Egfx::Framework::Icon::IconEnum::Bookmark: return { Atlas + 1897, 6 };
								case Egfx::Framework::Icon::IconEnum::BrightnessHigh: return { Atlas + 1903, 26 };
								case Egfx::Framework::Icon::IconEnum::BrightnessLow: return { Atlas + 1929, 18 };
								case Egfx::Framework::Icon::IconEnum::BrightnessMed: return { Atlas + 1947, 26 };
								case Egfx::Framework::Icon::IconEnum::Calendar: return { Atlas + 1973, 29 };
								case Egfx::Framework::Icon::IconEnum::Clock: return { Atlas + 2002, 7 };
								case Egfx::Framework::Icon::IconEnum::QuietHours: return { Atlas + 2009, 17 };
								case Egfx::Framework::Icon::IconEnum::DarkMode: return { Atlas + 2026, 11 };
								case Egfx::Framework::Icon::IconEnum::Error: return { Atlas + 2037, 9 };
								case Egfx::Framework::Icon::IconEnum::GameController: return { Atlas + 2046, 39 };
								case Egfx::Framework::Icon::IconEnum::GameControllerBattery0: return { Atlas + 2085, 33 };
								case Egfx::Framework::Icon::IconEnum::GameControllerBattery1: return { Atlas + 2118, 36 };
								case Egfx::Framework::Icon::IconEnum::GameControllerBattery2: return { Atlas + 2154, 36 };
								case Egfx::Framework::Icon::IconEnum::GameControllerBattery3: return { Atlas + 2190, 36 };
								case Egfx::Framework::Icon::IconEnum::GameControllerBattery4: return { Atlas + 2226, 36 };
								case Egfx::Framework::Icon::IconEnum::Globe: return { Atlas + 2262, 25 };
								case Egfx::Framework::Icon::IconEnum::Web: return { Atlas + 2287, 17 };
								case Egfx::Framework::Icon::IconEnum::Heart: return { Atlas + 2304, 16 };
								case Egfx::Framework::Icon::IconEnum::Info: return { Atlas + 2320, 9 };
								case Egfx::Framework::Icon::IconEnum::Lock: return { Atlas + 2329, 11 };
								case Egfx::Framework::Icon::IconEnum::Question: return { Atlas + 2340, 11 };
								case Egfx::Framework::Icon::IconEnum::Shield: return { Atlas + 2351, 11 };
								case Egfx::Framework::Icon::IconEnum::Smiley: return { Atlas + 2362, 12 };
								case Egfx::Framework::Icon::IconEnum::SmileyHappy: return { Atlas + 2374, 15 };
								case Egfx::Framework::Icon::IconEnum::SmileyNeutral: return { Atlas + 2389, 11 };
								case Egfx::Framework::Icon::IconEnum::SmileySad: return { Atlas + 2400, 12 };
								case Egfx::Framework::Icon::IconEnum::SmileyAngry: return { Atlas + 2412, 12 };
								case Egfx::Framework::Icon::IconEnum::Star: return { Atlas + 2424, 11 };
								case Egfx::Framework::Icon::IconEnum::Unlock: return { Atlas + 2435, 9 };
								case Egfx::Framework::Icon::IconEnum::User: return { Atlas + 2444, 9 };
								case Egfx::Framework::Icon::IconEnum::UserAdd: return { Atlas + 2453, 14 };
								case Egfx::Framework::Icon::IconEnum::UserRemove: return { Atlas + 2467, 12 };
								case Egfx::Framework::Icon::IconEnum::Users: return { Atlas + 2479, 19 };
								case Egfx::Framework::Icon::IconEnum::Warning: return { Atlas + 2498, 10 };
								default: return {};
								}
							}

						};
					}
				}
			}
		}
	}
}
#endif
