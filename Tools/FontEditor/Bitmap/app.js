(() => {
  'use strict';

  const $ = id => document.getElementById(id);
	const state = { width: 5, height: 5, spaceWidth: 1, format: 'rgb332', glyphs: [], selected: 0, assetName: 'MyBitmapFont' };
  const model = BitmapFontModel;

  function clamp(value, min, max) { return Math.max(min, Math.min(max, Number(value) || 0)); }
	function createGlyph(id, width = state.width, pixels) {
	const glyph = model.createGlyph(id, width, state.height, pixels);
	glyph.subpixelValues = glyph.pixels.map(model.encodeRgb332);
	return glyph;
  }
  function keyForGlyphId(id) { return BitmaskAtlasModel.keyForGlyphId(id); }
  function glyphIdForCharacter(character) { return BitmaskAtlasModel.glyphIdForCharacter(character); }
  function setStatus(message) { $('status').textContent = message; }
	function format() { return model.getFormat(state.format); }
  function packedBytes() { return state.glyphs.reduce((total, glyph) => total + format().packGlyph(glyph).length, 0); }
  function selectedColor() { return format().quantize($('selectedColor').value); }
  function syncSelectedColor() { const color = selectedColor(); $('selectedColor').value = color; $('selectedColorValue').textContent = `${color} · ${format().name}`; return color; }
  function subpixelPreviewEnabled() { return $('subpixelPreview').checked && state.format === 'rgb332'; }
	function subpixelValue(color) {
	const value = BitmapFontModel.encodeRgb332(color);
	const masks = [0xE0, 0x1C, 0x06];
	return masks.reduce((result, mask) => result | ((value & mask) === mask ? mask : 0), 0);
  }
	function drawPixel(context, color, x, y, scale, encodedValue) {
	if (!subpixelPreviewEnabled()) {
	  context.fillStyle = color;
	  context.fillRect(x * scale, y * scale, scale, scale);
	  return;
	}
	const value = encodedValue === undefined ? BitmapFontModel.encodeRgb332(color) : encodedValue;
	[0xE0, 0x1C, 0x06].forEach((mask, index) => {
	  if ((value & mask) === mask) {
		context.fillStyle = ['#FF0000', '#00FF00', '#0000FF'][index];
		context.fillRect((x * 3 + index) * scale, y * scale, scale, scale);
	  }
	});
  }
	function renderColorPalette() {
	const palette = $('colorPalette'); palette.replaceChildren();
	(format().palette ? format().palette() : []).forEach(color => {
	  const swatch = document.createElement('button'); swatch.type = 'button'; swatch.className = 'color-swatch'; swatch.style.backgroundColor = color; swatch.title = color; swatch.setAttribute('aria-label', color);
	  swatch.addEventListener('click', () => { $('selectedColor').value = color; syncSelectedColor(); renderPixelEditor(); }); palette.append(swatch);
	});
  }

  function renderTextPreviews() {
	const container = $('textPreviews'); container.replaceChildren();
	const samples = [
	  ['Uppercase alphabet', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'],
	  ['Lowercase alphabet', 'abcdefghijklmnopqrstuvwxyz'],
	  ['Digits and symbols', '0123456789 !@#$%^&*()-_=+[]{}\\|;:\'",.<>/?'],
	  ['Pangram', 'The quick brown fox jumps over the lazy dog.']
	];
	const glyphByCharacter = new Map();
	state.glyphs.forEach(glyph => { const character = keyForGlyphId(glyph.id || ''); if (character && !glyphByCharacter.has(character)) glyphByCharacter.set(character, glyph); });
	samples.forEach(([label, text]) => {
	  const wrapper = document.createElement('div'); wrapper.className = 'text-preview';
	  const title = document.createElement('div'); title.className = 'text-preview-label'; title.textContent = label;
	  const canvas = document.createElement('canvas'); canvas.className = 'text-preview-canvas';
		  const scale = 8; const spacing = 1; const pixelScale = subpixelPreviewEnabled() ? 3 : 1; const glyphs = [...text].map(character => character === ' ' ? null : glyphByCharacter.get(character));
	  const width = Math.max(1, glyphs.reduce((total, glyph, index) => total + (text[index] === ' ' ? state.spaceWidth + spacing : glyph ? glyph.width + spacing : state.width + spacing), 0));
	  canvas.width = width * pixelScale * scale; canvas.height = state.height * scale;
	  const context = canvas.getContext('2d'); context.fillStyle = '#000'; context.fillRect(0, 0, canvas.width, canvas.height);
		  let x = 0;
	  glyphs.forEach((glyph, index) => {
		if (!glyph) { if (text[index] === ' ') { x += state.spaceWidth + spacing; return; } x += state.width + spacing; return; }
		glyph.pixels.forEach((color, pixelIndex) => { if (color !== '#00000000') drawPixel(context, color, x + pixelIndex % glyph.width, Math.floor(pixelIndex / glyph.width), scale, glyph.subpixelValues[pixelIndex]); });
		x += glyph.width + spacing;
	  });
	  wrapper.append(title, canvas); container.append(wrapper);
	});
  }
	function assetName() { return (state.assetName.trim() || 'MyBitmapFont').replace(/[^A-Za-z0-9_]/g, '_'); }
  function sourceNamespace() { return `${assetName()}Source`; }
	function cppText() { return BitmapAtlasSource.serialize(state, format(), sourceNamespace()); }
  function fontText() { return BitmapFontDeclaration.generate(state, format(), keyForGlyphId, sourceNamespace(), assetName()); }

  function renderKeyboard() {
	AtlasEditorUi.renderKeyboard($('glyphList'), state, renderAll, addGlyphForCharacter, keyForGlyphId);
	$('glyphCountInfo').textContent = `${state.glyphs.length} glyphs`;
  }

  function addGlyphForCharacter(character) {
	const id = glyphIdForCharacter(character);
	if (!id) return;
	const index = state.glyphs.findIndex(glyph => keyForGlyphId(glyph.id || '') === character);
	if (index !== -1) state.selected = index;
	else { state.glyphs.push(createGlyph(id)); state.selected = state.glyphs.length - 1; }
	renderAll();
  }

  function renderPixelEditor() {
	const glyph = state.glyphs[state.selected];
	const editor = $('pixelEditor'); editor.replaceChildren(); editor.style.setProperty('--grid-width', subpixelPreviewEnabled() ? glyph.width * 3 : glyph.width);
	if (subpixelPreviewEnabled()) {
	  const masks = [0xE0, 0x1C, 0x06]; const colors = ['#FF0000', '#00FF00', '#0000FF'];
	  glyph.pixels.forEach((color, index) => {
		const value = glyph.subpixelValues[index];
		masks.forEach((mask, channel) => {
		  const pixel = document.createElement('button'); pixel.type = 'button'; pixel.className = 'pixel subpixel';
		  pixel.style.backgroundColor = (value & mask) === mask ? colors[channel] : '#000000';
		  pixel.title = `${['Red', 'Green', 'Blue'][channel]} sub-pixel ${index % glyph.width}, ${Math.floor(index / glyph.width)}`;
		  pixel.addEventListener('click', () => {
			const current = glyph.subpixelValues[index];
			const next = (current & mask) === mask ? current & ~mask : current | mask;
			glyph.subpixelValues[index] = next;
			glyph.pixels[index] = BitmapFontModel.decodeRgb332(next);
			renderAll();
		  });
		  editor.append(pixel);
		});
	  });
	  return;
	}
	glyph.pixels.forEach((color, index) => {
	  const pixel = document.createElement('button'); pixel.type = 'button'; pixel.className = 'pixel'; pixel.style.backgroundColor = color; pixel.title = `Pixel ${index % glyph.width}, ${Math.floor(index / glyph.width)}`;
		pixel.addEventListener('click', () => { glyph.pixels[index] = syncSelectedColor(); renderAll(); }); editor.append(pixel);
	});
  }

  function renderPreview() {
	const glyph = state.glyphs[state.selected]; const canvas = $('previewCanvas'); const scale = Math.max(12, Math.floor(Math.min(260 / glyph.width, 260 / state.height)));
	const pixelScale = subpixelPreviewEnabled() ? 3 : 1;
	canvas.width = glyph.width * pixelScale * scale; canvas.height = state.height * scale;
	const context = canvas.getContext('2d'); context.fillStyle = '#000'; context.fillRect(0, 0, canvas.width, canvas.height);
	 glyph.pixels.forEach((color, index) => { drawPixel(context, color, index % glyph.width, Math.floor(index / glyph.width), scale, glyph.subpixelValues[index]); });
  }

  function resizeAtlas(width, height) {
	const oldWidth = state.width; const oldHeight = state.height;
	state.width = width; state.height = height;
	state.glyphs.forEach(glyph => {
	  const oldPixels = glyph.pixels; const oldGlyphWidth = glyph.width; glyph.width = Math.min(oldGlyphWidth, width); glyph.pixels = Array(model.pixelCount(glyph.width, height)).fill('#00000000');
	  for (let y = 0; y < Math.min(oldHeight, height); y++) for (let x = 0; x < glyph.width; x++) glyph.pixels[y * glyph.width + x] = oldPixels[y * oldGlyphWidth + x] || '#00000000';
	});
	renderAll();
  }

  function renderAll() {
	const glyph = state.glyphs[state.selected];
	syncSelectedColor();
	renderColorPalette();
	AtlasEditorUi.syncMetadata(document, state, packedBytes());
	$('colorFormat').value = state.format;
	$('selectedGlyphWidth').value = glyph.width; $('glyphInfo').textContent = `${state.selected}: ${glyph.id} · ${glyph.width}×${state.height}`;
	$('atlasSourceInput').value = cppText(); $('cppOutput').value = cppText(); $('fontOutput').value = fontText();
	renderKeyboard(); renderPixelEditor(); renderPreview(); renderTextPreviews();
  }

  function importSource() {
	const parsed = BitmapAtlasSource.parse($('atlasSourceInput').value);
	if (parsed.width < 1 || parsed.height < 1 || !parsed.blocks.length || parsed.values.some(value => value < 0 || value > 255)) { setStatus('Could not find a complete RGB332, RGB565, or RGB888 bitmap atlas.'); return; }
	state.width = parsed.width; state.height = parsed.height; state.spaceWidth = parsed.spaceWidth === null ? state.spaceWidth : parsed.spaceWidth; state.format = parsed.format;
	const importedFormat = model.getFormat(state.format);
	state.glyphs = parsed.blocks.map(block => {
	  const glyphWidth = Math.min(block.width || parsed.width, parsed.width);
	  return createGlyph(block.id, glyphWidth, importedFormat.unpackGlyph(block.values).slice(0, glyphWidth * parsed.height));
	}); state.selected = 0;
	renderAll(); setStatus(`Imported ${state.glyphs.length} glyphs.`);
  }

  function importBitmaskSource() {
	const parsed = BitmaskAtlasSource.parse($('bitmaskSourceInput').value);
	if (parsed.width < 1 || parsed.height < 1 || !parsed.blocks.length || parsed.values.some(value => value < 0 || value > 255)) { setStatus('Could not find a complete bitmask atlas.'); return; }
	const paintColor = selectedColor();
	state.width = parsed.width; state.height = parsed.height; state.spaceWidth = parsed.spaceWidth === null ? state.spaceWidth : parsed.spaceWidth;
	state.glyphs = parsed.blocks.map(block => {
	  const width = Math.min(block.width || parsed.width, parsed.width);
	  const bytesPerGlyph = Math.ceil(width * parsed.height / 8);
	  const pixels = Array(model.pixelCount(width, parsed.height)).fill('#00000000');
	  block.body.match(/0x[\da-f]+|\b\d+\b/gi)?.slice(0, bytesPerGlyph).forEach((token, byteIndex) => {
		const value = Number.parseInt(token, token.toLowerCase().startsWith('0x') ? 16 : 10);
		for (let bit = 0; bit < 8; bit++) {
		  const pixelIndex = byteIndex * 8 + bit;
		  if (pixelIndex < pixels.length && (value & (0x80 >> bit)) !== 0) pixels[pixelIndex] = paintColor;
		}
	  });
	  return createGlyph(block.id, width, pixels);
	});
	state.selected = 0; renderAll(); setStatus(`Imported ${state.glyphs.length} bitmask glyphs.`);
  }

  function reset() {
	$('atlasSourceInput').value = BitmapDefaultAtlas.defaultAtlasSource;
	importSource();
 state.assetName = 'ContactChromatic5x5';
 renderAll(); setStatus('ContactChromatic default loaded.');
  }

  $('glyphWidth').addEventListener('change', () => resizeAtlas(clamp($('glyphWidth').value, 1, 32), state.height));
  $('glyphHeight').addEventListener('change', () => resizeAtlas(state.width, clamp($('glyphHeight').value, 1, 32)));
	$('spaceWidth').addEventListener('change', event => { state.spaceWidth = clamp(event.target.value, 0, 32); renderAll(); });
  $('assetName').addEventListener('input', event => { state.assetName = event.target.value; renderAll(); });
	$('selectedColor').addEventListener('input', () => { syncSelectedColor(); renderPixelEditor(); });
  $('subpixelPreview').addEventListener('change', renderAll);
  $('colorFormat').addEventListener('change', event => {
	state.format = event.target.value;
	const activeFormat = format();
	state.glyphs.forEach(glyph => glyph.pixels = glyph.pixels.map(activeFormat.quantize));
	syncSelectedColor(); renderAll();
  });
  $('selectedGlyphWidth').addEventListener('change', event => { const glyph = state.glyphs[state.selected]; const width = clamp(event.target.value, 1, state.width); const pixels = Array(model.pixelCount(width, state.height)).fill('#00000000'); for (let y = 0; y < state.height; y++) for (let x = 0; x < width; x++) pixels[y * width + x] = glyph.pixels[y * glyph.width + x] || '#00000000'; glyph.width = width; glyph.pixels = pixels; renderAll(); });
  $('removeGlyphButton').addEventListener('click', () => { if (state.glyphs.length === 1) return; state.glyphs.splice(state.selected, 1); state.selected = Math.min(state.selected, state.glyphs.length - 1); renderAll(); });
  $('resetButton').addEventListener('click', reset);
	$('importButton').addEventListener('click', importSource);
	$('importBitmaskButton').addEventListener('click', importBitmaskSource);
  $('copySourceButton').addEventListener('click', () => navigator.clipboard.writeText(cppText()).then(() => setStatus('Bitmap C++ source copied.')));
  $('copyCppButton').addEventListener('click', () => navigator.clipboard.writeText(cppText()).then(() => setStatus('Bitmap C++ atlas copied.')));
	$('copyFontButton').addEventListener('click', () => navigator.clipboard.writeText(fontText()).then(() => setStatus('Bitmap font declaration copied.')));
  reset();
})();
