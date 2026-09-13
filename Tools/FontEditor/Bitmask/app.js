(() => {
  'use strict';

  const $ = id => document.getElementById(id);
	const defaultAtlasSource = BitmaskDefaultAtlas.defaultAtlasSource;
	const state = { width: 5, height: 5, spaceWidth: 1, glyphs: [], selected: 0, assetName: 'MyBitmaskAtlas' };

	function pixelCount(width = state.width) { return BitmaskAtlasModel.pixelCount(width, state.height); }
	function createGlyph(id, width = state.width, pixels) { return BitmaskAtlasModel.createGlyph(id, width, state.height, pixels); }
  function clamp(value, min, max) { return Math.max(min, Math.min(max, Number(value) || 0)); }
  const printableCharacters = Array.from({ length: 95 }, (_, index) => String.fromCharCode(32 + index));
	function characterForGlyphId(id) {
	return BitmaskAtlasModel.characterForGlyphId(id);
  }

  function fontText() {
	const name = ($('assetName').value.trim() || 'MyBitmaskFont').replace(/[^A-Za-z0-9_]/g, '_');
	const source = ($('sourceNamespace').value.trim() || 'Source5x5').replace(/[^A-Za-z0-9_]/g, '_');
	return BitmaskFontDeclaration.generate(state, characterForGlyphId, packGlyph, source, name);
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
	state.glyphs.forEach(glyph => { const character = characterForGlyphId(glyph.id || ''); if (character && !glyphByCharacter.has(character)) glyphByCharacter.set(character, glyph); });
	samples.forEach(([label, text]) => {
	  const wrapper = document.createElement('div'); wrapper.className = 'text-preview';
	  const title = document.createElement('div'); title.className = 'text-preview-label'; title.textContent = label;
	  const canvas = document.createElement('canvas'); canvas.className = 'text-preview-canvas';
	const scale = 8; const spacing = 1; const glyphs = [...text].map(character => character === ' ' ? null : glyphByCharacter.get(character));
	const width = Math.max(1, glyphs.reduce((total, glyph, index) => total + (text[index] === ' ' ? state.spaceWidth + spacing : glyph ? glyph.width + spacing : state.width + spacing), 0));
	  canvas.width = width * scale; canvas.height = state.height * scale;
		  const context = canvas.getContext('2d'); context.fillStyle = '#000'; context.fillRect(0, 0, canvas.width, canvas.height); context.fillStyle = '#fff';
	  let x = 0;
	  glyphs.forEach((glyph, index) => {
		if (!glyph) { if (text[index] === ' ') { x += state.spaceWidth + spacing; return; } context.fillStyle = '#8b4b4b'; context.fillRect(x * scale, 0, state.width * scale, state.height * scale); context.fillStyle = '#fff'; x += state.width + spacing; return; }
		glyph.pixels.forEach((on, pixelIndex) => { if (on) context.fillRect((x + pixelIndex % glyph.width) * scale, Math.floor(pixelIndex / glyph.width) * scale, scale, scale); });
		x += glyph.width + spacing;
	  });
	  wrapper.append(title, canvas); container.append(wrapper);
	});
  }
  function glyphIdForCharacter(character) {
	return BitmaskAtlasModel.glyphIdForCharacter(character);
  }
  function addGlyphForCharacter(character) {
	const id = glyphIdForCharacter(character);
	const index = state.glyphs.findIndex(glyph => characterForGlyphId(glyph.id || '') === character);
	if (index !== -1) {
	  state.selected = index; renderAll(); return;
	}
	state.glyphs.push(createGlyph(id, state.width));
	state.selected = state.glyphs.length - 1;
	renderAll(); setStatus(`Glyph ${id} added.`);
  }
	function keyForGlyphId(id) {
	return BitmaskAtlasModel.keyForGlyphId(id);
  }

  function removeGlyph() {
	if (state.glyphs.length === 1) { setStatus('The atlas must contain at least one glyph.'); return; }
	state.glyphs.splice(state.selected, 1); state.selected = Math.min(state.selected, state.glyphs.length - 1);
	renderAll(); setStatus('Glyph removed.');
  }
  function setStatus(message) { $('status').textContent = message; clearTimeout(setStatus.timer); setStatus.timer = setTimeout(() => $('status').textContent = '', 2600); }

  function reset() {
 state.width = 5; state.height = 5; state.spaceWidth = 1; state.selected = 0; state.assetName = 'Contact5x5';
	$('atlasSourceInput').value = defaultAtlasSource;
	importPacked();
	setStatus('Reset.');
  }

  function resizeAtlas(width, height, count) {
	const oldWidth = state.width; const oldHeight = state.height;
	state.width = width; state.height = height;
	state.glyphs = Array.from({ length: count }, (_, index) => {
	  const old = state.glyphs[index];
		const oldWidthForGlyph = old?.width || oldWidth;
	  const glyphWidth = Math.min(oldWidthForGlyph, width);
	  const pixels = Array(pixelCount(glyphWidth)).fill(false);
	  if (old) {
		for (let y = 0; y < Math.min(oldHeight, height); y++) {
		for (let x = 0; x < glyphWidth; x++) pixels[y * glyphWidth + x] = old.pixels[y * oldWidthForGlyph + x];
		}
	  }
		return createGlyph(old?.id || `Glyph${index}`, glyphWidth, pixels);
	});
	state.selected = Math.min(state.selected, count - 1);
	renderAll();
  }

	function packGlyph(glyph) { return BitmaskAtlasModel.packGlyph(glyph, state.height); }

	function packedBytes() { return BitmaskAtlasModel.packedBytes(state.glyphs, state.height); }
	function importPacked() {
	const source = $('atlasSourceInput').value;
	const parsed = BitmaskAtlasSource.parse(source);
	const importedWidth = parsed.width || state.width;
	const importedHeight = parsed.height || state.height;
	const blocks = parsed.blocks;
	const aliases = parsed.aliases;
	const values = parsed.values;
	const importedSpaceWidth = parsed.spaceWidth;
	const importedBytesPerGlyph = importedHeight * Math.ceil(importedWidth / 8);
	const importedCount = blocks.length ? blocks.length : values.length / importedBytesPerGlyph;
	if (importedWidth < 1 || importedHeight < 1 || values.length === 0 || !Number.isInteger(importedCount) || values.some(value => !Number.isInteger(value) || value < 0 || value > 255)) {
	  setStatus('Could not find a complete byte-aligned Atlas[] initializer.'); return;
	}
	if (blocks.length || aliases.length) {
		state.width = importedWidth; state.height = importedHeight; state.spaceWidth = importedSpaceWidth === null ? state.spaceWidth : importedSpaceWidth; state.selected = 0;
		  if (blocks.length) state.glyphs = blocks.map(block => createGlyph(block.id, Math.min(block.width || importedWidth, importedWidth)));
	  if (aliases.length) state.glyphs = aliases.map(alias => createGlyph(alias.id, importedWidth));
	} else if (importedWidth !== state.width || importedHeight !== state.height || importedCount !== state.glyphs.length) resizeAtlas(importedWidth, importedHeight, importedCount);
	let byteOffset = 0;
	state.glyphs.forEach((glyph, glyphIndex) => {
		  const glyphBytes = blocks[glyphIndex] ? (blocks[glyphIndex].body.match(/0x[\da-f]+|\b\d+\b/gi) || []) : [];
	  const glyphByteCount = blocks[glyphIndex] ? glyphBytes.length : importedBytesPerGlyph;
	  const bytes = values.slice(byteOffset, byteOffset + glyphByteCount);
	  byteOffset += glyphByteCount;
		const glyphWidth = blocks[glyphIndex] ? Math.min(blocks[glyphIndex].width || importedWidth, importedWidth) : Math.min(glyph.width || importedWidth, importedWidth);
	  glyph.width = glyphWidth;
	  const rowPadded = glyphByteCount === importedHeight * Math.ceil(glyphWidth / 8);
	  glyph.pixels = Array.from({ length: pixelCount(glyphWidth) }, (_, index) => {
		const x = index % glyphWidth;
		const y = Math.floor(index / glyphWidth);
		return rowPadded
		  ? (bytes[y * Math.ceil(glyphWidth / 8) + Math.floor(x / 8)] & (0x80 >> (x % 8))) !== 0
		  : (bytes[Math.floor(index / 8)] & (0x80 >> (index % 8))) !== 0;
	  });
	});
	renderAll(); setStatus(`Imported ${state.glyphs.length} glyphs.`);
  }

  function cppText() {
	const name = ($('assetName').value.trim() || 'MyBitmaskAtlas').replace(/[^A-Za-z0-9_]/g, '_');
	const source = ($('sourceNamespace').value.trim() || 'Source5x5').replace(/[^A-Za-z0-9_]/g, '_');
	return BitmaskAtlasSource.serialize(state, packGlyph, source);
  }

  function renderGlyphList() {
	const list = $('glyphList'); list.replaceChildren();
	AtlasEditorUi.renderKeyboard(list, state, renderAll, addGlyphForCharacter, keyForGlyphId);
	$('glyphCountInfo').textContent = `${state.glyphs.length} glyphs`;
  }

	function renderPixelEditor() {
	const glyph = state.glyphs[state.selected];
	const editor = $('pixelEditor'); editor.replaceChildren(); editor.style.setProperty('--grid-width', glyph.width);
	glyph.pixels.forEach((on, index) => {
		const pixel = document.createElement('button'); pixel.type = 'button'; pixel.className = `pixel${on ? ' on' : ''}`; pixel.title = `Pixel ${index % glyph.width}, ${Math.floor(index / glyph.width)}`;
	  pixel.addEventListener('click', () => { glyph.pixels[index] = !glyph.pixels[index]; renderAll(); }); editor.append(pixel);
	});
  }

  function renderPreview() {
	const glyph = state.glyphs[state.selected]; const canvas = $('previewCanvas'); const scale = Math.max(12, Math.floor(Math.min(260 / glyph.width, 260 / state.height))); canvas.width = glyph.width * scale; canvas.height = state.height * scale;
	const context = canvas.getContext('2d'); context.fillStyle = '#000'; context.fillRect(0, 0, canvas.width, canvas.height);
	context.fillStyle = '#fff';
	glyph.pixels.forEach((on, index) => { if (on) context.fillRect((index % glyph.width) * scale, Math.floor(index / glyph.width) * scale, scale, scale); });
  }

  function renderAll() {
	const atlasByteCount = packedBytes().length;
	AtlasEditorUi.syncMetadata(document, state, atlasByteCount);
	$('selectedGlyphWidth').value = state.glyphs[state.selected]?.width || state.width;
	$('glyphInfo').textContent = `${state.selected}: ${state.glyphs[state.selected]?.id || `Glyph${state.selected}`} · ${state.glyphs[state.selected]?.width || state.width}×${state.height}`;
	$('atlasSourceInput').value = cppText(); $('cppOutput').value = cppText(); $('fontOutput').value = fontText();
	renderGlyphList(); renderPixelEditor(); renderPreview(); renderTextPreviews();
  }

  $('glyphWidth').addEventListener('change', () => resizeAtlas(clamp($('glyphWidth').value, 1, 32), state.height, state.glyphs.length));
  $('glyphHeight').addEventListener('change', () => resizeAtlas(state.width, clamp($('glyphHeight').value, 1, 32), state.glyphs.length));
	$('spaceWidth').addEventListener('change', event => { state.spaceWidth = clamp(event.target.value, 0, 32); renderAll(); });
  $('selectedGlyphWidth').addEventListener('change', event => {
	const glyph = state.glyphs[state.selected]; const width = clamp(event.target.value, 0, state.width); const pixels = Array(pixelCount(width)).fill(false);
	for (let y = 0; y < state.height; y++) for (let x = 0; x < width; x++) pixels[y * width + x] = glyph.pixels[y * glyph.width + x] || false;
	glyph.width = width; glyph.pixels = pixels; renderAll();
  });
  $('removeGlyphButton').addEventListener('click', removeGlyph);
  $('exampleButton').addEventListener('click', reset);
  $('assetName').addEventListener('input', event => { state.assetName = event.target.value; $('cppOutput').value = cppText(); });
	$('sourceNamespace').addEventListener('input', () => $('fontOutput').value = fontText());
  $('importButton').addEventListener('click', importPacked);
	$('copySourceButton').addEventListener('click', () => navigator.clipboard.writeText(cppText()).then(() => setStatus('C++ source copied.')));
  $('copyCppButton').addEventListener('click', () => navigator.clipboard.writeText(cppText()).then(() => setStatus('C++ atlas copied.')));
	$('copyFontButton').addEventListener('click', () => navigator.clipboard.writeText(fontText()).then(() => setStatus('Font declaration copied.')));
  $('clearButton').addEventListener('click', () => { state.glyphs[state.selected].pixels.fill(false); renderAll(); });
  $('fillButton').addEventListener('click', () => { state.glyphs[state.selected].pixels.fill(true); renderAll(); });
  $('resetButton').addEventListener('click', reset);
  reset();
})();

