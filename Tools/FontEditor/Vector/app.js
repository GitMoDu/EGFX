(() => {
  'use strict';
  const $ = id => document.getElementById(id);
  const M = window.EgfxVectorFontModel;
	const FONT_THICKNESS_SCALE = 64;
	const DEFAULT_PALETTE = [[255, 255, 255], [98, 198, 255], [255, 128, 127], [126, 235, 167], [255, 210, 112], [202, 155, 255]];
  const state = { maxWidth: 8, height: 15, spaceWidth: 2, assetName: 'MyVectorFont', glyphs: [], selected: 0, selectedGlyph: 0, selectedNode: -1, activeTool: 'Point', fontPreviewScale: 32, palette: DEFAULT_PALETTE.map(color => [...color]) };
  const characters = Array.from({ length: 95 }, (_, index) => String.fromCharCode(32 + index));
  const label = character => character === ' ' ? 'Space' : character;
  const glyphForCharacter = character => state.glyphs.find(glyph => glyph.character === character);
  function setStatus(message) { $('status').textContent = message; clearTimeout(setStatus.timer); setStatus.timer = setTimeout(() => $('status').textContent = '', 2600); }
	function colorHex(color) { return `#${color.map(value => value.toString(16).padStart(2, '0')).join('')}`; }
  function parseColor(value) { const match = /^#?([\da-f]{6})$/i.exec(value.trim()); return match ? [parseInt(match[1].slice(0, 2), 16), parseInt(match[1].slice(2, 4), 16), parseInt(match[1].slice(4, 6), 16)] : null; }
  function renderPalette() { const list = $('paletteList'); list.replaceChildren(); state.palette.forEach((color, index) => { const row = document.createElement('div'); row.className = 'palette-row'; const label = document.createElement('span'); label.textContent = index; const input = document.createElement('input'); input.type = 'color'; input.value = colorHex(color); input.title = `Palette color ${index}`; const hex = document.createElement('output'); hex.textContent = colorHex(color).toUpperCase(); input.addEventListener('input', event => { state.palette[index] = parseColor(event.target.value); hex.textContent = colorHex(state.palette[index]).toUpperCase(); renderTextPreviews(); renderPreview(); }); const remove = document.createElement('button'); remove.type = 'button'; remove.className = 'icon-button'; remove.textContent = '×'; remove.title = 'Remove palette color'; remove.addEventListener('click', () => { if (state.palette.length === 1) return; state.palette.splice(index, 1); renderAll(); }); row.append(label, input, hex, remove); list.append(row); }); $('paletteCount').textContent = `${state.palette.length} color${state.palette.length === 1 ? '' : 's'}`; }
  function ensureGlyphs() { if (!state.glyphs.length) state.glyphs.push({ id: 'GlyphSpace', character: ' ', width: 2, nodes: [] }); }
	function selected() { ensureGlyphs(); state.selected = Math.min(state.selected, state.glyphs.length - 1); state.selectedGlyph = state.selected; const glyph = state.glyphs[state.selected]; if (state.selectedNode < 0 && glyph.nodes.length) state.selectedNode = 0; return glyph; }
	function makeGlyph(character) { return { id: `Glyph${character === ' ' ? 'Space' : character.charCodeAt(0).toString(16).toUpperCase()}`, character, width: Math.min(state.maxWidth, 8), nodes: [{ primitive: 'Point', x: 1, y: 1 }, { primitive: 'Point', x: 6, y: 6 }] }; }
  function normalizeNode(node, primitiveName) { const primitive = M.primitive(primitiveName); const normalized = { ...node, primitive: primitive.name }; if (primitive.value) normalized.value ??= 0; else { normalized.x ??= 0; normalized.y ??= 0; for (let operand = 2; operand <= (primitive.operands || 0); operand++) { normalized[`x${operand}`] ??= 0; normalized[`y${operand}`] ??= 0; } } if (primitive.quadrant) normalized.y2 = (normalized.y2 & ~3) | ((normalized.y2 || 0) & 3); return normalized; }
	function drawCanvas(canvas, glyph, scale, text, showGrid = true, selectedNode = -1, renderScale = 256) {
    const context = canvas.getContext('2d');
    context.fillStyle = '#000';
    context.fillRect(0, 0, canvas.width, canvas.height);
    if (showGrid) {
      context.strokeStyle = '#263642';
      context.lineWidth = 1;
      for (let x = 0; x <= state.maxWidth; x++) { context.beginPath(); context.moveTo(x * scale, 0); context.lineTo(x * scale, state.height * scale); context.stroke(); }
      for (let y = 0; y <= state.height; y++) { context.beginPath(); context.moveTo(0, y * scale); context.lineTo(state.maxWidth * scale, y * scale); context.stroke(); }
    }
    if (!glyph || !window.EgfxVectorRenderer) return;
	window.EgfxVectorRenderer.render(context, glyph.nodes, Math.max(1, Math.round(canvas.width / scale)), Math.max(1, Math.round(canvas.height / scale)), scale, { angleMax: M.MAX_COORDINATE, renderScale, thicknessFraction: FONT_THICKNESS_SCALE, fixedFontThickness: true, fixedFontWeight: true, palette: state.palette });
    const selectedItem = glyph.nodes[selectedNode];
    if (!selectedItem || selectedItem.x === undefined || selectedItem.y === undefined) return;
	const marker = (x, y) => { const size = 10; const half = size / 2; context.fillStyle = '#111'; context.fillRect(x * scale + scale / 2 - half, y * scale + scale / 2 - half, size, size); context.strokeStyle = '#ff3bd4'; context.lineWidth = 2; context.strokeRect(x * scale + scale / 2 - half, y * scale + scale / 2 - half, size, size); };
    marker(selectedItem.x, selectedItem.y);
    if (selectedItem.primitive === 'Circle' || selectedItem.primitive === 'CircleFill') marker(selectedItem.x2 ?? selectedItem.x, selectedItem.y2 ?? selectedItem.y);
    else if (['Ring', 'RingFill', 'Arc', 'ArcFill', 'CircleQuadrant', 'CircleQuadrantFill'].includes(selectedItem.primitive)) marker(selectedItem.x + Math.abs(selectedItem.x2 ?? 0), selectedItem.y);
  }
	function characterForGlyphId(id) { return state.glyphs.find(glyph => glyph.id === id)?.character || null; }
	function addGlyphForCharacter(character) { const existing = glyphForCharacter(character); if (!existing) state.glyphs.push(makeGlyph(character)); state.selected = state.glyphs.findIndex(item => item.character === character); state.selectedGlyph = state.selected; state.selectedNode = selected().nodes.length ? 0 : -1; renderAll(); }
  function renderGlyphs() { AtlasEditorUi.renderKeyboard($('glyphList'), state, renderAll, addGlyphForCharacter, characterForGlyphId); $('glyphCount').textContent = `${state.glyphs.length} glyph${state.glyphs.length === 1 ? '' : 's'}`; }
	function renderToolPalette() { const palette = $('toolPalette'); window.EgfxVectorEditorUi.createToolPalette(palette, [{ name: 'Select' }, ...M.PRIMITIVES], () => state.activeTool, tool => { state.activeTool = tool; renderToolPalette(); }); }
	function renderNodes() {
	const glyph = selected();
	const editor = $('nodeEditor'); editor.replaceChildren();
	const list = $('nodeList');
	list.replaceChildren();
	const primitiveIcon = window.EgfxVectorEditorUi.primitiveIcon;
	const coordinateInput = (node, property, title) => {
	  const input = document.createElement('input');
	  input.type = 'number'; input.min = 0; input.max = 14; input.value = node[property] ?? 0; input.title = title;
	  input.addEventListener('pointerdown', event => event.stopPropagation());
	  input.addEventListener('change', event => { node[property] = M.clamp(event.target.value); renderAll(); });
	  return input;
	};
	const node = glyph.nodes[state.selectedNode];
	if (node) {
	  const primitive = M.primitive(node.primitive);
	  const title = document.createElement('strong'); title.textContent = `${primitiveIcon(primitive.name)} ${primitive.name}`;
	  const identity = document.createElement('div'); identity.className = 'node-identity';
	  const type = document.createElement('select'); M.PRIMITIVES.forEach(entry => type.add(new Option(entry.name, entry.name, false, node.primitive === entry.name)));
	  type.addEventListener('change', event => { node.primitive = event.target.value; renderAll(); });
	  const actions = document.createElement('div'); actions.className = 'node-toolbar';
	  const action = (text, label, callback) => { const button = document.createElement('button'); button.type = 'button'; button.className = 'icon-button'; button.textContent = text; button.title = label; button.addEventListener('click', callback); return button; };
	  actions.append(action('↑', 'Move node up', () => { if (state.selectedNode > 0) { const nodes = glyph.nodes; [nodes[state.selectedNode - 1], nodes[state.selectedNode]] = [nodes[state.selectedNode], nodes[state.selectedNode - 1]]; state.selectedNode--; renderAll(); } }), action('↓', 'Move node down', () => { if (state.selectedNode < glyph.nodes.length - 1) { const nodes = glyph.nodes; [nodes[state.selectedNode + 1], nodes[state.selectedNode]] = [nodes[state.selectedNode], nodes[state.selectedNode + 1]]; state.selectedNode++; renderAll(); } }));
	  identity.append(title, type, actions);
	  const fields = document.createElement('div'); fields.className = 'node-fields';
	  if (primitive.value) fields.append(coordinateInput(node, 'value', primitive.value));
	  else if (primitive.quadrant) { fields.append(coordinateInput(node, 'x', 'Center X'), coordinateInput(node, 'y', 'Center Y'), coordinateInput(node, 'x2', 'Radius')); const quadrant = document.createElement('select'); [['Top right', 0], ['Bottom right', 1], ['Bottom left', 2], ['Top left', 3]].forEach(([name, value]) => quadrant.add(new Option(name, value, false, ((node.y2 ?? 0) & 3) === value))); quadrant.title = 'Circle quadrant'; quadrant.addEventListener('change', event => { node.y2 = ((node.y2 ?? 0) & ~3) | Number(event.target.value); renderAll(); }); fields.append(quadrant); }
	  else if (primitive.name !== 'Break') { fields.append(coordinateInput(node, 'x', 'X coordinate'), coordinateInput(node, 'y', 'Y coordinate')); for (let operand = 2; operand <= (primitive.operands || 0); operand++) fields.append(coordinateInput(node, `x${operand}`, `Point ${operand} X`), coordinateInput(node, `y${operand}`, `Point ${operand} Y`)); }
	  editor.append(identity, fields);
	}
	glyph.nodes.forEach((node, index) => {
	  const chip = document.createElement('button'); chip.type = 'button'; chip.className = `node-chip${index === state.selectedNode ? ' selected' : ''}`; chip.innerHTML = `<span class="node-index">${index}</span><span class="node-icon">${primitiveIcon(node.primitive)}</span><span>${node.primitive}</span>`; chip.addEventListener('click', () => { state.selectedNode = index; renderAll(); }); list.append(chip);
	});
	$('glyphInfo').textContent = `${label(glyph.character)} · ${glyph.width} px · ${glyph.nodes.length} nodes`;
	$('glyphWidth').value = glyph.width;
  }
  function renderPreview() { const glyph = selected(); const canvas = $('previewCanvas'); const scale = 32; const width = Math.max(1, glyph.width); canvas.width = width * scale; canvas.height = state.height * scale; canvas.style.width = `${width * scale}px`; canvas.style.height = `${state.height * scale}px`; drawCanvas(canvas, glyph, scale, '', true, state.selectedNode); }
  function renderTextPreviews() {
	const container = $('textPreviews'); container.replaceChildren();
	const samples = [
	  ['Uppercase alphabet', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'],
	  ['Lowercase alphabet', 'abcdefghijklmnopqrstuvwxyz'],
	  ['Digits and symbols', '0123456789 !@#$%^&*()-_=+[]{}\\|;:\'",.<>/?'],
	  ['Pangram', 'The quick brown fox jumps over the lazy dog.']
	];
	const glyphByCharacter = new Map(state.glyphs.map(glyph => [glyph.character, glyph]));
	const outputHeight = Math.max(1, state.fontPreviewScale); const layoutScale = 8; const spacing = 1; const widthScale = (outputHeight - 1) / Math.max(1, state.height - 1);
	const scaleFactor = (target, source) => target <= 1 || source <= 1 ? 0 : Math.min(65535, Math.floor((target - 1) * 256 / (source - 1)));
	const scaleCoordinate = (value, factor) => Math.floor((value ?? 0) * factor / 256);
	const outputWidth = Math.max(1, Math.round((state.maxWidth - 1) * widthScale) + 1);
	const fullScale = Math.max(scaleFactor(outputWidth, state.maxWidth), scaleFactor(outputHeight, state.height));
	const insetPixels = (output, otherOutput, source, otherSource) => {
	  const maximum = Math.floor((output - 1) / 2);
	  const scale = Math.max(scaleFactor(output, source), scaleFactor(otherOutput, otherSource));
	  const effectiveThickness = 1 + Math.floor(FONT_THICKNESS_SCALE * scaleCoordinate(1, scale) / 128);
	  return Math.min(maximum, effectiveThickness > 1 ? Math.floor((effectiveThickness - 1) / 2) : 0);
	};
	const insetX = insetPixels(outputWidth, outputHeight, state.maxWidth, state.height); const insetY = insetPixels(outputHeight, outputWidth, state.height, state.maxWidth);
	const drawableWidth = Math.max(1, outputWidth - insetX * 2); const drawableHeight = Math.max(1, outputHeight - insetY * 2);
	const xScale = scaleFactor(drawableWidth, state.maxWidth); const yScale = scaleFactor(drawableHeight, state.height);
	const scaleGlyph = glyph => {
	  const glyphOutputWidth = Math.max(1, Math.round(glyph.width * outputWidth / Math.max(1, state.maxWidth)));
	  const mapX = value => insetX + scaleCoordinate(value, xScale); const mapY = value => insetY + scaleCoordinate(value, yScale);
	  const nodes = glyph.nodes.map(node => {
		const primitive = M.primitive(node.primitive);
		if (primitive.value) return { ...node, value: node.value };
		const scaled = { ...node };
		if (scaled.x !== undefined) scaled.x = mapX(scaled.x);
		if (scaled.y !== undefined) scaled.y = mapY(scaled.y);
		if (primitive.quadrant) {
		  if (scaled.x2 !== undefined) scaled.x2 = Math.max(0, scaleCoordinate(scaled.x2, xScale));
		  scaled.y2 = (node.y2 ?? 0) & 3;
		  return scaled;
		}
		if (primitive.arc) {
		  scaled.x2 = Math.max(0, scaleCoordinate(node.x2, xScale));
		  scaled.y2 = primitive.name.endsWith('Fill') ? Math.max(0, scaleCoordinate(node.y2, xScale)) : 0;
		  scaled.x3 = node.x3;
		  scaled.y3 = node.y3;
		  return scaled;
		}
		if (primitive.name === 'Ring' || primitive.name === 'RingFill') {
		  scaled.x2 = Math.max(0, scaleCoordinate(node.x2, xScale));
		  scaled.y2 = Math.max(0, scaleCoordinate(node.y2, yScale));
		  return scaled;
		}
		['x2', 'x3', 'x4'].forEach(property => { if (scaled[property] !== undefined) scaled[property] = mapX(scaled[property]); });
		['y2', 'y3', 'y4'].forEach(property => { if (scaled[property] !== undefined) scaled[property] = mapY(scaled[property]); });
		return scaled;
	  });
	  return { ...glyph, width: outputWidth, nodes, drawableWidth, drawableHeight, advanceWidth: glyphOutputWidth, renderScale: Math.max(1, fullScale) };
	};
	samples.forEach(([titleText, text]) => {
	  const wrapper = document.createElement('div'); wrapper.className = 'text-preview';
	  const title = document.createElement('div'); title.className = 'text-preview-label'; title.textContent = titleText;
	  const logicalWidths = [...text].map(character => Math.max(1, character === ' ' ? state.spaceWidth : glyphByCharacter.get(character)?.width || state.maxWidth));
	  const scaledGlyphByCharacter = new Map([...text].filter(character => character !== ' ').map(character => [character, glyphByCharacter.get(character)]).filter(([, glyph]) => !!glyph).map(([character, glyph]) => [character, scaleGlyph(glyph)]));
	  const widths = [...text].map(character => character === ' ' ? Math.max(1, Math.round(state.spaceWidth * widthScale)) : (scaledGlyphByCharacter.get(character)?.advanceWidth || outputWidth));
	  const width = Math.max(1, widths.reduce((total, glyphWidth) => total + glyphWidth + spacing, 0));
	  const logicalWidth = Math.max(1, logicalWidths.reduce((total, glyphWidth) => total + glyphWidth + spacing, 0));
	  const canvas = document.createElement('canvas'); canvas.className = 'text-preview-canvas'; canvas.width = width; canvas.height = outputHeight; canvas.style.width = `${logicalWidth * layoutScale}px`; canvas.style.height = `${state.height * layoutScale}px`;
	  const context = canvas.getContext('2d'); context.fillStyle = '#000'; context.fillRect(0, 0, canvas.width, canvas.height);
	  let x = 0;
	  [...text].forEach((character, index) => {
		const outputGlyph = scaledGlyphByCharacter.get(character);
		if (character !== ' ' && outputGlyph) {
		  const glyphCanvas = document.createElement('canvas'); glyphCanvas.width = outputGlyph.width; glyphCanvas.height = outputHeight;
		  drawCanvas(glyphCanvas, outputGlyph, 1, '', false, -1, outputGlyph.renderScale); context.drawImage(glyphCanvas, 0, 0, outputGlyph.advanceWidth, outputHeight, x, 0, outputGlyph.advanceWidth, outputHeight);
		}
		x += widths[index] + spacing;
	  });
	  wrapper.append(title, canvas); container.append(wrapper);
	});
  }
	function renderAll() { ensureGlyphs(); $('maxWidth').value = state.maxWidth; $('height').value = state.height; $('spaceWidth').value = state.spaceWidth; $('assetName').value = state.assetName; $('sourceNamespace').value = state.sourceNamespace; $('fontPreviewScale').value = state.fontPreviewScale; $('fontPreviewScaleValue').textContent = `${state.fontPreviewScale} px`; renderToolPalette(); renderPalette(); renderGlyphs(); renderNodes(); renderPreview(); renderTextPreviews(); const output = EgfxVectorFontDeclaration.generate(state); $('cppOutput').value = output; $('fontOutput').value = output; }
  function parseCharacter(value) { const literal = value.slice(1, -1); if (literal === "\\\\") return '\\'; if (literal === "\\'") return "'"; return literal; }
	function parseSource(source) {
	const width = Number(source.match(/Width\s*=\s*(\d+)/)?.[1] || 8);
	const height = Number(source.match(/Height\s*=\s*(\d+)/)?.[1] || 15);
	const spaceWidth = Number(source.match(/SpaceWidth\s*=\s*(\d+)/)?.[1] || 2);
	const atlas = source.match(/\bAtlas\s*\[\s*\]\s*PROGMEM\s*\{([\s\S]*?)\};/i)?.[1];
	if (!atlas) throw new Error('No packed Atlas[] declaration found.');

	const packedAtlas = atlas.replace(/\/\/.*$/gm, '');
	const values = (packedAtlas.match(/0x[\da-f]+|\b\d+\b/gi) || []).map(value => Number(value));
	const glyphs = [];
	const pattern = /\/\/\s*Glyph:\s*(\w+)\s+Character:\s*(?<character>'(?:\\\\.|[^'])+')\s+Width:\s*(\d+)\s+Offset:\s*(\d+)\s+Count:\s*(\d+)/g;
	let match;
	while ((match = pattern.exec(atlas))) {
	  const offset = Number(match[4]);
	  const count = Number(match[5]);
	  if (offset + count > values.length) throw new Error(`Glyph ${match[1]} exceeds the packed atlas.`);
	  glyphs.push({ id: match[1], character: parseCharacter(match.groups.character), width: Number(match[3]), nodes: M.decode(values.slice(offset, offset + count)) });
	}
	if (!glyphs.length) throw new Error('No glyph spans found in the packed atlas.');
	Object.assign(state, { maxWidth: width, height, spaceWidth, glyphs, selected: 0, selectedGlyph: 0, selectedNode: -1 });
  }
	function loadDefaultAtlas() {
	  const source = window.EgfxVectorFontDefaults?.Epoxy8x8 || '';
	  if (!source) return false;
	  $('sourceInput').value = source;
	  parseSource(source);
	  state.assetName = 'Epoxy8x8';
	  state.sourceNamespace = 'Epoxy8x8';
	  return true;
	}
	$('resetButton').addEventListener('click', () => {
	  if (!loadDefaultAtlas()) Object.assign(state, { maxWidth: 8, height: 15, spaceWidth: 2, assetName: 'MyVectorFont', sourceNamespace: 'SourceVector', glyphs: [makeGlyph(' ')], selected: 0, selectedGlyph: 0, selectedNode: -1 });
	  renderAll();
	});
  $('addNodeButton').addEventListener('click', () => { selected().nodes.splice(state.selectedNode + 1, 0, { primitive: 'Point', x: 0, y: 0 }); state.selectedNode++; renderAll(); });
	$('duplicateNodeButton').addEventListener('click', () => { if (state.selectedNode < 0) return; selected().nodes.splice(state.selectedNode + 1, 0, { ...selected().nodes[state.selectedNode] }); state.selectedNode++; renderAll(); });
  $('deleteNodeButton').addEventListener('click', () => { if (state.selectedNode >= 0) selected().nodes.splice(state.selectedNode, 1); state.selectedNode = Math.min(state.selectedNode, selected().nodes.length - 1); renderAll(); });
  $('glyphWidth').addEventListener('change', event => { selected().width = M.clamp(event.target.value); renderAll(); });
  [['maxWidth', 'maxWidth'], ['height', 'height'], ['spaceWidth', 'spaceWidth']].forEach(([id, property]) => $(id).addEventListener('change', event => { state[property] = M.clamp(event.target.value); renderAll(); }));
	$('assetName').addEventListener('input', event => { state.assetName = event.target.value; renderAll(); }); $('sourceNamespace').addEventListener('input', event => { state.sourceNamespace = event.target.value; renderAll(); });
	$('fontPreviewScale').addEventListener('input', event => { state.fontPreviewScale = Number(event.target.value); renderAll(); });
	$('addPaletteButton').addEventListener('click', () => { state.palette.push([255, 255, 255]); renderAll(); });
  $('importButton').addEventListener('click', () => { try { parseSource($('sourceInput').value); renderAll(); setStatus('Vector font imported.'); } catch (error) { setStatus(error.message); } });
	const copyOutput = id => navigator.clipboard?.writeText($(id).value).then(() => setStatus('C++ copied.'));
  $('copyCppButton').addEventListener('click', () => copyOutput('cppOutput')); $('copyFontButton').addEventListener('click', () => copyOutput('fontOutput')); $('copySourceButton').addEventListener('click', () => copyOutput('cppOutput'));
	$('previewCanvas').addEventListener('pointerdown', event => {
	const bounds = event.currentTarget.getBoundingClientRect();
	const x = Math.min(state.maxWidth - 1, M.clamp(Math.floor((event.clientX - bounds.left) * state.maxWidth / bounds.width)));
	const y = Math.min(state.height - 1, M.clamp(Math.floor((event.clientY - bounds.top) * state.height / bounds.height)));
	if (state.activeTool === 'Select') {
	  const glyph = selected(); let closest = -1; let distance = 1.5;
	  glyph.nodes.forEach((node, index) => { if (node.x === undefined || node.y === undefined) return; const candidate = Math.hypot(node.x - x, node.y - y); if (candidate <= distance) { distance = candidate; closest = index; } });
	  if (closest >= 0) { state.selectedNode = closest; state.drag = { index: closest }; event.currentTarget.setPointerCapture(event.pointerId); renderAll(); }
	  return;
	}
	const primitive = M.primitive(state.activeTool); if (primitive.name === 'Break') { selected().nodes.splice(state.selectedNode + 1, 0, { primitive: 'Break' }); state.selectedNode++; renderAll(); return; }
	if (primitive.name === 'Point' || primitive.value) { const node = { primitive: primitive.name, x, y }; if (primitive.value) node.value = x; selected().nodes.splice(state.selectedNode + 1, 0, node); state.selectedNode++; renderAll(); return; }
	state.dragStart = { x, y }; state.dragCurrent = { x, y }; event.currentTarget.setPointerCapture(event.pointerId); renderAll();
  });
	$('previewCanvas').addEventListener('pointermove', event => {
	  if (state.dragStart) { const bounds = event.currentTarget.getBoundingClientRect(); state.dragCurrent = { x: Math.min(state.maxWidth - 1, M.clamp(Math.floor((event.clientX - bounds.left) * state.maxWidth / bounds.width))), y: Math.min(state.height - 1, M.clamp(Math.floor((event.clientY - bounds.top) * state.height / bounds.height))) }; renderAll(); return; }
	  if (!state.drag) return;
	  const bounds = event.currentTarget.getBoundingClientRect(); const x = Math.min(state.maxWidth - 1, M.clamp(Math.floor((event.clientX - bounds.left) * state.maxWidth / bounds.width))); const y = Math.min(state.height - 1, M.clamp(Math.floor((event.clientY - bounds.top) * state.height / bounds.height)));
	  const node = selected().nodes[state.drag.index]; if (node) { node.x = x; node.y = y; renderAll(); }
	});
	$('previewCanvas').addEventListener('pointerup', event => { if (state.dragStart) { const start = state.dragStart; const end = state.dragCurrent || start; const primitive = M.primitive(state.activeTool); const node = primitive.name === 'Circle' || primitive.name === 'CircleFill' ? { primitive: primitive.name, x: start.x, y: start.y, x2: end.x, y2: end.y } : primitive.name === 'Ring' || primitive.name === 'RingFill' ? { primitive: primitive.name, x: start.x, y: start.y, x2: Math.abs(end.x - start.x), y2: Math.floor(Math.abs(end.x - start.x) / 2) } : primitive.name === 'Arc' || primitive.name === 'ArcFill' ? { primitive: primitive.name, x: start.x, y: start.y, x2: Math.abs(end.x - start.x), y2: primitive.name.endsWith('Fill') ? Math.floor(Math.abs(end.x - start.x) / 2) : 0, x3: 0, y3: Math.floor((state.maxWidth - 1) / 4) } : primitive.quadrant ? { primitive: primitive.name, x: start.x, y: start.y, x2: Math.abs(end.x - start.x), y2: end.y >= start.y ? (end.x >= start.x ? 1 : 2) : (end.x >= start.x ? 0 : 3) } : primitive.name === 'Triangle' || primitive.name === 'TriangleFill' ? { primitive: primitive.name, x: start.x, y: start.y, x2: end.x, y2: start.y, x3: end.x, y3: end.y } : null; if (node) { selected().nodes.splice(state.selectedNode + 1, 0, node); state.selectedNode++; } state.dragStart = null; state.dragCurrent = null; event.currentTarget.releasePointerCapture?.(event.pointerId); renderAll(); } else if (state.drag) { state.drag = null; event.currentTarget.releasePointerCapture?.(event.pointerId); } });
	$('previewCanvas').addEventListener('pointercancel', () => { state.drag = null; });
	loadDefaultAtlas(); renderAll();
})();
