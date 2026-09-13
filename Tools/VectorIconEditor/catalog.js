(() => {
  'use strict';

  // Synchronized from Framework/Vector/Model.h. IconCode remains an internal C++ authoring detail.
  const names = [
	'Attach', 'Add', 'Copy', 'Cut', 'Delete', 'Edit', 'Export', 'Filter', 'Import', 'Paste', 'Pin', 'Redo', 'Refresh', 'Save', 'Settings', 'Link', 'Share', 'Sort', 'Tag', 'Undo', 'Remove', 'Zoom', 'ZoomIn', 'ZoomOut', 'Sliders',
	'Battery0', 'Battery1', 'Battery2', 'Battery3', 'Battery4', 'BatteryDisabled', 'BatteryCharging0', 'BatteryCharging1', 'BatteryCharging2', 'BatteryCharging3', 'BatteryCharging4', 'Bluetooth', 'BluetoothDisabled', 'BluetoothDisconnected', 'Cellular0', 'Cellular1', 'Cellular2', 'Cellular3', 'Cellular4', 'CellularDisabled', 'AirplaneMode', 'Location', 'LocationDisabled', 'Sync', 'Computer', 'Laptop', 'Cpu', 'Code', 'Desktop', 'Monitor', 'Hdd', 'Keyboard', 'Lightning', 'Mouse', 'Plug', 'Thermometer', 'Power', 'Usb', 'Wifi0', 'Wifi1', 'Wifi2', 'Wifi3', 'WifiDisabled', 'WifiDisconnected',
	'ArrowDown', 'ArrowLeft', 'ArrowRight', 'ArrowUp', 'Check', 'ChevronDown', 'ChevronLeft', 'ChevronRight', 'ChevronUp', 'Close', 'Expand', 'Grid', 'GridSmall', 'GridLarge', 'Home', 'List', 'Menu',
	'Archive', 'Cloud', 'CloudError', 'CloudDisabled', 'CloudOk', 'CloudUpload', 'CloudDownload', 'CloudSync', 'Download', 'Folder', 'FolderOpen', 'NewFile', 'Mail', 'Print', 'Upload',
	'Camera', 'FastForward', 'Image', 'Music', 'Mic', 'MicMuted', 'Pause', 'Play', 'Record', 'Rewind', 'Stop', 'Video', 'VideoCamera', 'Volume0', 'Volume1', 'Volume2', 'Volume3', 'VolumeMute', 'VolumeDisabled',
	'Bell', 'Bookmark', 'BrightnessHigh', 'BrightnessLow', 'BrightnessMed', 'Calendar', 'Clock', 'QuietHours', 'DarkMode', 'Error', 'GameController', 'GameControllerBattery0', 'GameControllerBattery1', 'GameControllerBattery2', 'GameControllerBattery3', 'GameControllerBattery4', 'Globe', 'Web', 'Heart', 'Info', 'Lock', 'Question', 'Shield', 'Smiley', 'SmileyHappy', 'SmileyNeutral', 'SmileySad', 'SmileyAngry', 'Star', 'Unlock', 'User', 'UserAdd', 'UserRemove', 'Users', 'Warning'
  ];

	const symbolByName = {
		Attach: 0xF629,
		Add: 0xE710, Copy: 0xE16F, Cut: 0xE16B, Delete: 0xE107, Edit: 0xE104, Export: 0xEDE1,
		Filter: 0xE16E, Import: 0xE150, Pin: 0xE141, Redo: 0xE10D, Refresh: 0xE149, Save: 0xE105,
		Settings: 0xF617, Link: 0xE167, Share: 0xE72D, Sort: 0xE174, Undo: 0xE10E, Remove: 0xE738, ZoomIn: 0xE12E,
		ZoomOut: 0xE1A4, Zoom: 0xF616, Sliders: 0xF61C,
		Battery0: 0xEBA0, Battery1: 0xEBA2, Battery2: 0xEBA5, Battery3: 0xEBAA, Battery4: 0xF626, BatteryDisabled: 0xF600,
		BatteryCharging0: 0xF609, BatteryCharging1: 0xF60A, BatteryCharging2: 0xF60B, BatteryCharging3: 0xF60C, BatteryCharging4: 0xF628,
		Bluetooth: 0xE702, BluetoothDisabled: 0xF601, BluetoothDisconnected: 0xF61A,
		Cellular0: 0xEC38, Cellular1: 0xEC39, Cellular2: 0xEC3A, Cellular3: 0xEC3B, Cellular4: 0xF627,
		CellularDisabled: 0xF602, AirplaneMode: 0xF604,
		Location: 0xF605, LocationDisabled: 0xF606, Sync: 0xF607, Computer: 0xE772,
		Laptop: 0xE7F7, Cpu: 0xE950, Code: 0xE943, Desktop: 0xE7F9, Monitor: 0xE7F4, Hdd: 0xEDA2, Keyboard: 0xE144, Power: 0xF61D, Usb: 0xF61E,
		Paste: 0xE77F, Camera: 0xE114, Video: 0xE116, VideoCamera: 0xE116, Download: 0xE118, Upload: 0xE11C,
		Clock: 0xE121, Folder: 0xE188, List: 0xE14C, Play: 0xE102, Pause: 0xE103,
		Stop: 0xE15B, Calendar: 0xE163, Tag: 0xE1CB, Print: 0xE749,
		Wifi0: 0xE701, Wifi1: 0xE872, Wifi2: 0xE873, Wifi3: 0xE874, WifiDisabled: 0xF608, WifiDisconnected: 0xF60D,
		ArrowDown: 0xE74B, ArrowLeft: 0xE76B, ArrowRight: 0xE76C, ArrowUp: 0xE74A,
		Check: 0xE73E, ChevronDown: 0xE70D, ChevronLeft: 0xE76B, ChevronRight: 0xE76C, ChevronUp: 0xE70E,
		Close: 0xE711, Expand: 0xE740, Grid: 0xF0E2, GridSmall: 0xF0E2, GridLarge: 0xECA5, Home: 0xF618,
		Menu: 0xF619,
		Archive: 0xE7B8, Cloud: 0xE753, CloudError: 0xEA6A, CloudDisabled: 0xF636, CloudOk: 0xF637, CloudUpload: 0xF60E, CloudDownload: 0xF60F, CloudSync: 0xF610, FolderOpen: 0xE838, NewFile: 0xE132, Mail: 0xF61B,
		FastForward: 0xEB9D, Image: 0xE8B9, Music: 0xF61F, Mic: 0xE720, MicMuted: 0xEC54,
		Record: 0xE7C8, Rewind: 0xEB9E, Volume0: 0xF620, Volume1: 0xF621, Volume2: 0xF622, Volume3: 0xF623, VolumeMute: 0xF624, VolumeDisabled: 0xF625,
		Bell: 0xE8C8, Bookmark: 0xE12F,
		GameController: 0xF615, GameControllerBattery0: 0xF630, GameControllerBattery1: 0xF631, GameControllerBattery2: 0xF632, GameControllerBattery3: 0xF633, GameControllerBattery4: 0xF634, Globe: 0xE774, Web: 0xF635, Heart: 0xEB51,
		Info: 0xE946, Lock: 0xE72E, Question: 0xE897,
		Shield: 0xEA18, Smiley: 0xE76E, SmileyHappy: 0x1F600, SmileyNeutral: 0xF62A, SmileySad: 0xE719, SmileyAngry: 0x1F620, Star: 0xE734, Unlock: 0xE785, User: 0xE77B, UserAdd: 0xE8FA, UserRemove: 0xE8F8, Users: 0xE716, Warning: 0xE7BA
	};
	window.EgfxVectorIconCatalog = names.map((name, index) => {
		const symbol = symbolByName[name] || 0;
		return { name, emoji: symbol ? String.fromCodePoint(symbol) : '□', glyph: symbol ? String.fromCodePoint(symbol) : '□', code: symbol, scalar: symbol, variant: 0, nodes: [] };
	});
})();
