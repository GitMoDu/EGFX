(() => {
  'use strict';

  const config = window.EgfxVectorEditorConfig;
	const icons = window.EgfxVectorIconCatalog.map(icon => ({ ...icon, packed: [], nodes: [] }));
	const state = { width: 15, height: 15, inset: 0, columns: 8, selected: 0, setPreviewScale: 32 };
  const $ = id => document.getElementById(id);
  const clamp = (value, min, max) => Math.max(min, Math.min(max, Number(value) || 0));
  const selectedIcon = () => icons[state.selected];

  function decodePacked(values) {
	const nodes = [];
	for (let index = 0; index < values.length; index++) {
	  const raw = values[index];
	  const x = (raw >> 4) & 0x0f;
	  const y = raw & 0x0f;
	  if (x !== 15 && y !== 15) {
		nodes.push({ primitive: 'Point', x, y });
		continue;
	  }
	  const opcode = x === 15 ? y : 16 + x;
	  const primitive = config.PRIMITIVE_BY_OPCODE.get(opcode);
	  if (!primitive) {
		nodes.push({ primitive: 'Break' });
		continue;
	  }
	  const count = primitive.value ? 1 : primitive.operands || 0;
	  const operands = [];
	  for (let operand = 0; operand < count && index + 1 < values.length; operand++) {
		const value = values[++index];
		operands.push({ x: (value >> 4) & 0x0f, y: value & 0x0f });
	  }
	  if (primitive.value) nodes.push({ primitive: primitive.name, value: operands[0]?.x || 0 });
	  else if (primitive.operands) {
		const node = { ...(operands[0] || { x: 0, y: 0 }), primitive: primitive.name };
		operands.slice(1).forEach((operand, operandIndex) => {
		  node[`x${operandIndex + 2}`] = operand.x;
		  node[`y${operandIndex + 2}`] = operand.y;
		});
		nodes.push(node);
	  } else nodes.push({ primitive: primitive.name });
	}
	return nodes;
  }

  function parsePacked(text) {
	const tokens = text.replace(/\/\*[\s\S]*?\*\//g, '').replace(/\/\/.*$/gm, '').match(/0x[\da-f]+|\d+/gi) || [];
	return tokens.map(Number).filter(value => Number.isInteger(value) && value >= 0 && value <= 255);
  }

	function stripColorCommands(values) {
	const sanitized = [];
	let removed = 0;
	for (let index = 0; index < values.length; index++) {
		const value = values[index];
	  const x = (value >> 4) & 0x0f;
	  const y = value & 0x0f;
	  const opcode = x === 0x0f ? y : y === 0x0f ? 0x10 + x : undefined;
	  if (opcode === 1) {
			removed++;
			if (index + 1 < values.length) index++;
			continue;
		}
		sanitized.push(value);
	}
	return { values: sanitized, removed };
  }

  function packedText(icon) {
	return icon.packed.map(value => `0x${value.toString(16).padStart(2, '0').toUpperCase()}`).join(', ');
  }

	function encodePacked(nodes) {
	const axisMax = Math.max(0, Math.min(14, Math.max(state.width, state.height) - 1));
	const encodePoint = point => (clamp(point.x, 0, axisMax) << 4) | clamp(point.y, 0, axisMax);
	return nodes.flatMap(node => {
		const primitive = config.PRIMITIVE_BY_NAME.get(node.primitive);
		if (!primitive) return [];
		if (primitive.name === 'Point') return [encodePoint(node)];
		if (primitive.opcode === undefined) return [];
		const values = [0xF0 | primitive.opcode];
		if (primitive.value) return values.concat(encodePoint({ x: node.value, y: 0 }));
		for (let index = 1; index <= (primitive.operands || 0); index++) {
			values.push(encodePoint(index === 1 ? node : { x: node[`x${index}`], y: node[`y${index}`] }));
		}
		return values;
	});
	}

	function applyOffset(offsetX, offsetY) {
	const icon = selectedIcon();
	if ((!offsetX && !offsetY) || !icon.nodes.length) return;
	const axisMax = Math.max(0, Math.min(14, Math.max(state.width, state.height) - 1));
	icon.nodes = icon.nodes.map(node => {
		const primitive = config.PRIMITIVE_BY_NAME.get(node.primitive);
		if (!primitive || primitive.value) return { ...node };
		const shifted = { ...node };
		if (shifted.x !== undefined) shifted.x = clamp(shifted.x + offsetX, 0, axisMax);
		if (shifted.y !== undefined) shifted.y = clamp(shifted.y + offsetY, 0, axisMax);
		if (primitive.operands && !primitive.arc && !primitive.quadrant && primitive.name !== 'Ring' && primitive.name !== 'RingFill') {
			for (let index = 2; index <= primitive.operands; index++) {
				if (shifted[`x${index}`] !== undefined) shifted[`x${index}`] = clamp(shifted[`x${index}`] + offsetX, 0, axisMax);
				if (shifted[`y${index}`] !== undefined) shifted[`y${index}`] = clamp(shifted[`y${index}`] + offsetY, 0, axisMax);
			}
		}
		return shifted;
	});
	icon.packed = encodePacked(icon.nodes);
	renderAll();
	$('status').textContent = `Applied offset ${offsetX >= 0 ? '+' : ''}${offsetX}, ${offsetY >= 0 ? '+' : ''}${offsetY}`;
	}

  function parseAtlasSource(text) {
	const width = text.match(/CanvasWidth\s*=\s*(\d+)/i);
	const height = text.match(/CanvasHeight\s*=\s*(\d+)/i);
	const inset = text.match(/EdgeInset\s*=\s*(\d+)/i);
	const metadata = text.match(/\/\/\s*IconAtlas:\s*CanvasWidth:\s*(\d+)\s+CanvasHeight:\s*(\d+)\s+EdgeInset:\s*(\d+)\s+Columns:\s*(\d+)/i);
	 const dataMatch = text.match(/(?:static\s+constexpr\s+)?(?:const\s+)?uint8_t\s+(\w+)\s*(?:\[\s*\d*\s*\])?\s*(?:PROGMEM\s*)?\{([\s\S]*?)\}/i);
	if (!dataMatch) return null;
	const data = parsePacked(dataMatch[2].replace(/\/\/.*$/gm, ''));
	if (!data.length) return null;
	const symbol = dataMatch[1];
	const entries = [];
	const metadataPattern = /\/\/\s*Icon:\s*(\w+)\s+Offset:\s*(\d+)\s+Count:\s*(\d+)/gi;
	let metadataEntry;
	while ((metadataEntry = metadataPattern.exec(dataMatch[2]))) entries.push({ name: metadataEntry[1], offset: Number(metadataEntry[2]), length: Number(metadataEntry[3]) });
	const casePattern = new RegExp(`case\\s+(?:[\\w:]+::)?IconEnum::(\\w+)\\s*:\\s*return\\s*\\{\\s*(?:${symbol}|Data)\\s*\\+\\s*(\\d+)\\s*,\\s*(\\d+)`, 'gi');
	let match;
	if (!entries.length) while ((match = casePattern.exec(text))) entries.push({ name: match[1], offset: Number(match[2]), length: Number(match[3]) });
	return {
		width: metadata ? Number(metadata[1]) : width && Number(width[1]),
		height: metadata ? Number(metadata[2]) : height && Number(height[1]),
		inset: metadata ? Number(metadata[3]) : inset && Number(inset[1]),
		columns: metadata ? Number(metadata[4]) : null,
		data,
		entries
	};
  }

  function importAtlasSource() {
	const atlas = parseAtlasSource($('sourceInput').value);
	if (!atlas) { $('status').textContent = 'Import failed: expected unified atlas C++ output'; return; }
	if (atlas.width) $('canvasWidth').value = atlas.width;
	if (atlas.height) $('canvasHeight').value = atlas.height;
	if (atlas.inset !== null && atlas.inset !== false) $('inset').value = atlas.inset;
	if (atlas.columns) $('atlasColumns').value = atlas.columns;
	if (!atlas.entries.length) {
		const sanitized = stripColorCommands(atlas.data);
		const icon = selectedIcon();
		icon.packed = sanitized.values;
		icon.nodes = decodePacked(icon.packed);
		renderAll();
		$('status').textContent = `Imported ${icon.name} from unified atlas data`;
		return true;
	}
	atlas.entries.forEach(entry => {
		const icon = icons.find(candidate => candidate.name === entry.name);
		if (!icon) return;
		const values = atlas.data.slice(entry.offset, entry.offset + entry.length);
		const sanitized = stripColorCommands(values);
		icon.packed = sanitized.values;
		icon.nodes = decodePacked(icon.packed);
	});
	renderAll();
	$('status').textContent = `Imported ${atlas.entries.length} icon${atlas.entries.length === 1 ? '' : 's'} from unified atlas`;
	return true;
  }

  function syncSettings() {
	state.width = clamp($('canvasWidth').value, 1, 255);
	state.height = clamp($('canvasHeight').value, 1, 255);
	state.inset = clamp($('inset').value, 0, Math.floor(Math.max(0, Math.min(state.width, state.height) - 1) / 2));
	state.columns = clamp($('atlasColumns').value, 1, 32);
  }

  function renderIcons() {
	const assigned = icons.filter(icon => icon.packed.length);
	$('iconCount').textContent = `${assigned.length} of ${icons.length} assigned`;
	$('iconList').innerHTML = icons.map((icon, index) => `<button type="button" class="icon-button ${index === state.selected ? 'selected' : ''} ${icon.packed.length ? 'assigned' : 'unassigned'}" data-icon="${index}" title="IconEnum::${icon.name} · U+${icon.code.toString(16).toUpperCase().padStart(4, '0')}"><span class="icon-emoji" aria-hidden="true">${icon.glyph}</span><span class="icon-name">${icon.name}</span></button>`).join('');
	$('assignedCount').textContent = `${assigned.length} icon${assigned.length === 1 ? '' : 's'}`;
	$('assignedList').innerHTML = assigned.length ? assigned.map(icon => { const index = icons.indexOf(icon); return `<button type="button" class="assigned-tile" data-icon="${index}" title="Edit IconEnum::${icon.name}"><canvas data-assigned-canvas="${index}"></canvas><span>${icon.name}</span><small>IconEnum::${icon.name}</small></button>`; }).join('') : '<span class="empty-state">No icons assigned yet.</span>';
	$('reassignTarget').innerHTML = icons.filter(icon => icon !== selectedIcon()).map(icon => `<option value="${icons.indexOf(icon)}">${icon.name}</option>`).join('');
	assigned.forEach(icon => { const canvas = document.querySelector(`[data-assigned-canvas="${icons.indexOf(icon)}"]`); draw(canvas, icon.nodes, state.width, state.height, Math.max(2, Math.floor(96 / Math.max(state.width, state.height)))); });
  }

  function renderNodes() {
	const icon = selectedIcon();
	$('iconInfo').textContent = `${icon.name} · ${icon.packed.length} packed values · ${state.width}×${state.height}`;
	$('packedInput').value = packedText(icon);
  }
	function scaleCoordinate(value, scale) { return Math.floor(value * scale / 256); }

	function draw(canvas, nodes, width, height, scale) {
	const outputWidth = width * scale;
	const outputHeight = height * scale;
	const insetX = Math.min(Math.floor((outputWidth - 1) / 2), Math.floor(state.inset * scale));
	const insetY = Math.min(Math.floor((outputHeight - 1) / 2), Math.floor(state.inset * scale));
	const drawableWidth = Math.max(1, outputWidth - insetX * 2);
	const drawableHeight = Math.max(1, outputHeight - insetY * 2);
	const scaleX = width <= 1 ? 0 : Math.floor((drawableWidth - 1) * 256 / (width - 1));
	const scaleY = height <= 1 ? 0 : Math.floor((drawableHeight - 1) * 256 / (height - 1));
	const mapX = value => insetX + scaleCoordinate(value, scaleX);
	const mapY = value => insetY + scaleCoordinate(value, scaleY);
	const mapLengthX = value => Math.max(0, scaleCoordinate(value, scaleX));
	const mapLengthY = value => Math.max(0, scaleCoordinate(value, scaleY));
	const mappedNodes = nodes.map(node => {
	  const primitive = config.PRIMITIVE_BY_NAME.get(node.primitive);
	  if (!primitive || primitive.value || primitive.name === 'Break') return { ...node };
	  const mapped = { ...node, x: mapX(node.x), y: mapY(node.y) };
	  if (primitive.arc) { mapped.x2 = mapLengthX(node.x2); mapped.y2 = primitive.name.endsWith('Fill') ? mapLengthX(node.y2) : 0; }
	  else if (primitive.quadrant) { mapped.x2 = mapLengthX(node.x2); mapped.y2 = node.y2; }
	  else if (primitive.name === 'Ring' || primitive.name === 'RingFill') { mapped.x2 = mapLengthX(node.x2); mapped.y2 = mapLengthY(node.y2); }
	  else { ['x2', 'x3', 'x4'].forEach(property => { if (node[property] !== undefined) mapped[property] = mapX(node[property]); }); ['y2', 'y3', 'y4'].forEach(property => { if (node[property] !== undefined) mapped[property] = mapY(node[property]); }); }
	  return mapped;
	});
  canvas.width = outputWidth; canvas.height = outputHeight;
	window.EgfxVectorRenderer.render(canvas.getContext('2d'), mappedNodes, outputWidth, outputHeight, 1, { angleMax: 14, renderScale: Math.max(scaleX, scaleY) });
  }

  function renderPreview() {
	draw($('previewCanvas'), selectedIcon().nodes, state.width, state.height, Math.max(4, Math.floor(320 / Math.max(state.width, state.height))));
	const assigned = icons.filter(icon => icon.packed.length);
	const rows = Math.max(1, Math.ceil(assigned.length / state.columns));
	const setPreviewScale = clamp(state.setPreviewScale, 2, 128);
	state.setPreviewScale = setPreviewScale;
	const scale = setPreviewScale / Math.max(state.width, state.height);
	const tileWidth = Math.max(1, Math.round(state.width * scale));
	const tileHeight = Math.max(1, Math.round(state.height * scale));
	const atlas = $('atlasCanvas'); atlas.width = state.columns * tileWidth; atlas.height = rows * tileHeight;
	const context = atlas.getContext('2d'); context.imageSmoothingEnabled = false; context.clearRect(0, 0, atlas.width, atlas.height);
	assigned.forEach((icon, index) => { const tile = document.createElement('canvas'); draw(tile, icon.nodes, state.width, state.height, scale); context.drawImage(tile, (index % state.columns) * tileWidth, Math.floor(index / state.columns) * tileHeight, tileWidth, tileHeight); });
	$('atlasInfo').textContent = assigned.length ? `${assigned.length} icons · ${state.columns}×${rows} tiles · ${atlas.width}×${atlas.height}px` : 'No assigned icons';
  }

	function packed(icon) { return icon.packed.map(value => `0x${value.toString(16).padStart(2, '0').toUpperCase()}`); }
  function cppOutput() {
	const asset = $('assetName').value || 'MyIconSet';
	const assigned = icons.filter(icon => icon.packed.length);
	let offset = 0;
	const values = assigned.map(icon => { const result = `\t// Icon: ${icon.name} Offset: ${offset} Count: ${icon.packed.length}\n\t${icon.packed.map(value => `0x${value.toString(16).padStart(2, '0').toUpperCase()}`).join(', ')}`; offset += icon.packed.length; return result; }).join(',\n');
	const count = assigned.reduce((total, icon) => total + icon.packed.length, 0);
	const data = `// Generated unified packed vector IconSet\n// IconAtlas: CanvasWidth: ${state.width} CanvasHeight: ${state.height} EdgeInset: ${state.inset} Columns: ${state.columns}\n\nstatic constexpr uint8_t ${asset}Data[] PROGMEM\n{\n${values}\n};`;
	return `${data}\n\n${asset}Data contains ${count} packed values.`;
  }

  function combinedOutput() {
	return `${cppOutput()}\n\n${declarationOutput()}`;
  }

  function declarationOutput() {
	const asset = $('assetName').value || 'MyIconSet';
	const assigned = icons.filter(icon => icon.packed.length);
	const enumValues = assigned.map(icon => `\t\t${icon.name} = uint32_t(IconEnum::${icon.name}),`).join('\n');
	let offset = 0;
	const cases = assigned.map(icon => { const result = `\t\t\tcase Egfx::Framework::Icon::IconEnum::${icon.name}: return { ${asset}Data + ${offset}, ${icon.packed.length} };`; offset += icon.packed.length; return result; }).join('\n');
	return `struct ${asset}\n{\n\tenum class SetIconEnum : uint32_t\n\t{\n${enumValues}\n\t};\n\n\tstatic constexpr uint8_t CanvasWidth = ${state.width};\n\tstatic constexpr uint8_t CanvasHeight = ${state.height};\n\tstatic constexpr uint8_t EdgeInset = ${state.inset};\n\n\tstatic constexpr IconSource GetIcon(const Egfx::Framework::Icon::IconEnum icon)\n\t{\n\t\tswitch (icon)\n\t\t{\n${cases}\n\t\t\tdefault: return {};\n\t\t}\n\t}\n};`;
  }

  function serializedSet() {
	return JSON.stringify({ canvasWidth: state.width, canvasHeight: state.height, atlasColumns: state.columns, palette: state.palette, icons: icons.map(icon => ({ name: icon.name, packed: [...icon.packed] })) }, null, 2);
  }

  function importSet() {
	try {
	  const value = JSON.parse($('sourceInput').value);
	  if (value.canvasWidth) $('canvasWidth').value = value.canvasWidth;
	  if (value.canvasHeight) $('canvasHeight').value = value.canvasHeight;
		if (value.inset !== undefined) $('inset').value = value.inset;
	  if (value.atlasColumns) $('atlasColumns').value = value.atlasColumns;
		if (Array.isArray(value.icons)) value.icons.forEach(source => { const target = icons.find(icon => icon.name === source.name); if (target && Array.isArray(source.packed)) { target.packed = stripColorCommands(source.packed.map(Number).filter(Number.isInteger)).values; target.nodes = decodePacked(target.packed); } });
	  renderAll();
	  $('status').textContent = 'Imported icon set';
	} catch (error) { $('status').textContent = 'Import failed: expected JSON icon-set data'; }
  }

	async function copyText(value) {
	if (navigator.clipboard) await navigator.clipboard.writeText(value);
	else { const input = document.createElement('textarea'); input.value = value; document.body.appendChild(input); input.select(); document.execCommand('copy'); input.remove(); }
	$('status').textContent = 'Copied';
  }

	function renderAll() { syncSettings(); $('iconPreviewScale').value = state.setPreviewScale; $('iconPreviewScaleValue').textContent = `${state.setPreviewScale} px`; renderIcons(); renderNodes(); renderPreview(); $('cppOutput').value = cppOutput(); $('declarationOutput').value = declarationOutput(); }
  document.addEventListener('click', event => {
	const icon = event.target.closest('[data-icon]'); if (icon) { state.selected = Number(icon.dataset.icon); renderAll(); return; }
	if (event.target.id === 'importPackedButton') { const parsed = parsePacked($('packedInput').value); if (!parsed.length) { $('status').textContent = 'No packed values found'; return; } const sanitized = stripColorCommands(parsed); selectedIcon().packed = sanitized.values; selectedIcon().nodes = decodePacked(sanitized.values); renderAll(); $('status').textContent = sanitized.removed ? `Imported ${sanitized.values.length} values; removed ${sanitized.removed} color command${sanitized.removed === 1 ? '' : 's'}` : `Imported ${sanitized.values.length} packed values`; }
	if (event.target.id === 'removeIconButton') { selectedIcon().packed = []; selectedIcon().nodes = []; renderAll(); $('status').textContent = 'Removed icon assignment'; }
	if (event.target.id === 'reassignButton') { const target = icons[Number($('reassignTarget').value)]; if (target && selectedIcon().packed.length) { target.packed = [...selectedIcon().packed]; target.nodes = decodePacked(target.packed); selectedIcon().packed = []; selectedIcon().nodes = []; state.selected = icons.indexOf(target); renderAll(); $('status').textContent = `Reassigned to ${target.name}`; } }
	if (event.target.id === 'copyPackedButton') copyText(packedText(selectedIcon()));
	const offsetButton = event.target.closest('[data-offset-x], [data-offset-y]');
	if (offsetButton) applyOffset(Number(offsetButton.dataset.offsetX) || 0, Number(offsetButton.dataset.offsetY) || 0);
	if (event.target.id === 'resetButton') { icons.forEach(icon => { icon.packed = []; icon.nodes = []; }); state.selected = 0; renderAll(); }
	if (event.target.id === 'importButton') { if (!importAtlasSource()) importSet(); }
	if (event.target.id === 'copySourceButton') copyText(serializedSet());
	if (event.target.id === 'copyCppButton') copyText(combinedOutput());
	if (event.target.id === 'copyDeclarationButton') copyText(declarationOutput());
  });
	document.addEventListener('input', event => {
	if (['canvasWidth', 'canvasHeight', 'inset', 'atlasColumns', 'assetName'].includes(event.target.id)) renderAll();
	if (event.target.id === 'iconPreviewScale') { state.setPreviewScale = Number(event.target.value); renderAll(); }
  });
	const defaultSource = window.EgfxVectorIconDefaults?.MockAtlas || '';
  if (defaultSource) {
	$('sourceInput').value = defaultSource;
	importAtlasSource();
  } else renderAll();
})();
