const BitmapFontModel = (() => {
  function pixelCount(width, height) {
	return width * height;
  }

  function createGlyph(id, width, height, pixels) {
	return { id, width, pixels: pixels || Array(pixelCount(width, height)).fill('#00000000') };
  }

  function channels(color) {
	const value = color.replace('#', '').slice(0, 6).padEnd(6, '0');
	return [Number.parseInt(value.slice(0, 2), 16), Number.parseInt(value.slice(2, 4), 16), Number.parseInt(value.slice(4, 6), 16)];
  }

	function color(red, green, blue) {
	return `#${[red, green, blue].map(channel => channel.toString(16).padStart(2, '0')).join('').toUpperCase()}`;
  }

	function encodeRgb332(colorValue) {
	const [red, green, blue] = channels(colorValue);
	return (Math.round(red * 7 / 255) << 5) | (Math.round(green * 7 / 255) << 2) | Math.round(blue * 3 / 255);
  }

	function decodeRgb332(value) {
	return color(Math.round(((value >> 5) & 7) * 255 / 7), Math.round(((value >> 2) & 7) * 255 / 7), Math.round((value & 3) * 255 / 3));
  }

  function encodeRgb565(colorValue) {
	const [red, green, blue] = channels(colorValue);
	return (Math.round(red * 31 / 255) << 11) | (Math.round(green * 63 / 255) << 5) | Math.round(blue * 31 / 255);
  }

  function decodeRgb565(value) {
	return color(Math.round(((value >> 11) & 31) * 255 / 31), Math.round(((value >> 5) & 63) * 255 / 63), Math.round((value & 31) * 255 / 31));
  }

  function encodeRgb888(colorValue) {
	return channels(colorValue);
  }

  function decodeRgb888(values) {
	return color(values[0], values[1], values[2]);
  }

  function quantizeRgb332(colorValue) { return decodeRgb332(encodeRgb332(colorValue)); }
  function quantizeRgb565(colorValue) { return decodeRgb565(encodeRgb565(colorValue)); }
  function quantizeRgb888(colorValue) { return color(...channels(colorValue)); }

  function hsv(colorValue) {
	const [red, green, blue] = channels(colorValue).map(channel => channel / 255);
	const maximum = Math.max(red, green, blue); const minimum = Math.min(red, green, blue); const range = maximum - minimum;
	let hue = 0;
	if (range) {
	  if (maximum === red) hue = ((green - blue) / range) % 6;
	  else if (maximum === green) hue = (blue - red) / range + 2;
	  else hue = (red - green) / range + 4;
	  hue = (hue * 60 + 360) % 360;
	}
	return { hue, saturation: maximum ? range / maximum : 0, value: maximum };
  }

  function rgb332Palette() {
	return Array.from({ length: 256 }, (_, value) => decodeRgb332(value)).sort((left, right) => {
	  const a = hsv(left); const b = hsv(right);
	  if (a.saturation === 0 && b.saturation !== 0) return -1;
	  if (a.saturation !== 0 && b.saturation === 0) return 1;
	  return a.hue - b.hue || a.value - b.value || a.saturation - b.saturation;
	});
  }

  function packGlyph(glyph) {
	return glyph.pixels.map(encodeRgb332);
  }

	function unpackGlyph(bytes) {
	return bytes.map(decodeRgb332);
  }

  const formats = {
	rgb332: { name: 'RGB332', bytesPerPixel: 1, encode: encodeRgb332, decode: decodeRgb332, quantize: quantizeRgb332, packGlyph: glyph => glyph.pixels.map(encodeRgb332), unpackGlyph: unpackGlyph, palette: rgb332Palette },
	rgb565: { name: 'RGB565', bytesPerPixel: 2, encode: encodeRgb565, decode: decodeRgb565, quantize: quantizeRgb565, packGlyph: glyph => glyph.pixels.flatMap(colorValue => { const value = encodeRgb565(colorValue); return [value >> 8, value & 0xFF]; }), unpackGlyph: bytes => { const colors = []; for (let index = 0; index + 1 < bytes.length; index += 2) colors.push(decodeRgb565((bytes[index] << 8) | bytes[index + 1])); return colors; } },
	rgb888: { name: 'RGB888', bytesPerPixel: 3, encode: encodeRgb888, decode: decodeRgb888, quantize: quantizeRgb888, packGlyph: glyph => glyph.pixels.flatMap(encodeRgb888), unpackGlyph: bytes => { const colors = []; for (let index = 0; index + 2 < bytes.length; index += 3) colors.push(decodeRgb888(bytes.slice(index, index + 3))); return colors; } }
  };

  function getFormat(id) { return formats[id] || formats.rgb332; }

  return { pixelCount, createGlyph, encodeRgb332, decodeRgb332, quantizeRgb332, encodeRgb565, decodeRgb565, quantizeRgb565, encodeRgb888, decodeRgb888, quantizeRgb888, getFormat };
})();
