(() => {
  'use strict';

  const { PRIMITIVES, PRIMITIVE_BY_NAME, PRIMITIVE_BY_OPCODE, QUADRANTS, DEFAULT_PALETTE, OUTPUT_MIN, OUTPUT_MAX, MODELS } = window.EgfxVectorEditorConfig;
  const { scaleFactor, scaleCoordinate } = window.EgfxVectorEditorScale;
  const $ = id => document.getElementById(id);
  const DEFAULT_PACKED_IMAGE = '0xC5, 0xA2, 0xA0, 0x82, 0x42, 0x20, 0x11, 0x14, 0x08, 0xF2, 0x04, 0x35, 0x57, 0x77, 0x95, 0xB7, 0xC7, 0xE5, 0x85, 0x35, 0xF2, 0xE9, 0xD6, 0xF2, 0x23, 0x22, 0x33, 0xF2, 0x7A, 0x8B, 0xAB, 0xBA, 0xC9, 0xBA, 0xA9, 0xFD, 0x99, 0x51, 0xFB, 0x59, 0x57, 0x7B, 0x5E, 0x9E, 0xF2, 0xFB, 0xB9, 0x1B, 0xB3, 0x66, 0x75, 0xF2, 0xC5, 0xB6';
  const state = { model: '8', nodes: [{ primitive: 'Point', x: 1, y: 1 }, { primitive: 'Point', x: 6, y: 6 }], selected: 0, canvasWidth: 15, canvasHeight: 15, width: 128, height: 128, preserveCanvasRatio: false, thicknessFraction: 64, inset: 1, assetName: 'MyVector', activeTool: 'Point', palette: DEFAULT_PALETTE.map(color => [...color]), draft: null, dragStart: null, editDrag: null, trace: { image: null, visible: true, layer: 'background', opacity: .5 } };

  function model() { return MODELS[state.model]; }
  function insetPixels(output, otherOutput, source, otherSource) {
    if (output <= 1 || source <= 1 || state.inset === 0) return 0;
    const maximum = Math.floor((output - 1) / 2);
    const scale = Math.max(scaleFactor(output, source), scaleFactor(otherOutput, otherSource));
    const effectiveThickness = 1 + Math.floor(state.thicknessFraction * scaleCoordinate(1 + state.inset, scale) / 128);
    return Math.min(maximum, effectiveThickness > 1 ? Math.floor((effectiveThickness - 1) / 2) : 0);
  }
  function persist() {}
  function clamp(value, min, max) { return Math.max(min, Math.min(max, Number(value) || 0)); }
  function primitiveFor(name) { return PRIMITIVE_BY_NAME.get(name) || PRIMITIVES[0]; }
  function colorHex(color) { return `#${color.map(value => value.toString(16).padStart(2, '0')).join('')}`; }
  function parseColor(value) { const match = /^#?([\da-f]{6})$/i.exec(value.trim()); return match ? [parseInt(match[1].slice(0, 2), 16), parseInt(match[1].slice(2, 4), 16), parseInt(match[1].slice(4, 6), 16)] : null; }
  function paletteCppText() { const entries = state.palette.map(color => `Egfx::Rgb::Color(uint32_t(0x${colorHex(color).slice(1).toUpperCase()}))`).join(',\n        '); return `struct PaletteSource\n{\n    static constexpr Egfx::rgb_color_t Palette[${state.palette.length}]\n    {\n        ${entries}\n    };\n\n    static constexpr Egfx::rgb_color_t GetColor(const uint8_t colorIndex)\n    {\n        return Palette[colorIndex % ${state.palette.length}];\n    }\n};`; }
  function point(x = 0, y = 0) { return { primitive: 'Point', x, y }; }
  function normalizeNode(node) {
    const primitive = primitiveFor(node.primitive);
    const m = model();
    const normalized = { ...node, primitive: primitive.name };
    if (primitive.value) normalized.value = clamp(node.value, 0, m.axisMax);
    else {
      normalized.x = clamp(node.x, 0, m.axisMax);
      normalized.y = clamp(node.y, 0, m.axisMax);
      for (let index = 2; index <= (primitive.operands || 1); index++) {
        normalized[`x${index}`] = clamp(node[`x${index}`], 0, m.axisMax);
        normalized[`y${index}`] = clamp(node[`y${index}`], 0, m.axisMax);
      }
    }
    return normalized;
  }
  function quadrantInput(node) {
    const select = document.createElement('select');
    const quadrant = (Number(node.y2) || 0) & 0x03;
    QUADRANTS.forEach(entry => select.add(new Option(entry.name, entry.value, false, quadrant === entry.value)));
    select.title = 'Circle quadrant';
    select.addEventListener('click', event => event.stopPropagation()); select.addEventListener('pointerdown', event => event.stopPropagation());
    select.addEventListener('change', event => { node.y2 = (Number(node.y2) & ~0x03) | Number(event.target.value); renderAll(); });
    return select;
  }
  function angleInput(node, property, label) { return numberInput(node, property, label); }
  const data = window.createEgfxVectorEditorData({ state, model, clamp, primitiveFor, primitiveByOpcode: PRIMITIVE_BY_OPCODE, point, normalizeNode, colorHex, paletteCppText, renderAll: () => renderAll(), setStatus, getElement: $ });
  const { cppText, importPacked, packedText, copyText } = data;
  function renderPalette() {
    const list = $('paletteList'); list.replaceChildren();
    state.palette.forEach((color, index) => {
      const row = document.createElement('div'); row.className = 'palette-row';
      const label = document.createElement('span'); label.textContent = index;
      const input = document.createElement('input'); input.type = 'color'; input.value = colorHex(color); input.title = `Palette color ${index}`;
      const hex = document.createElement('output'); hex.textContent = colorHex(color).toUpperCase();
      input.addEventListener('input', event => { state.palette[index] = parseColor(event.target.value); hex.textContent = colorHex(state.palette[index]).toUpperCase(); renderPreview(); $('cppOutput').value = cppText(); persist(); });
      const remove = actionButton('×', 'Remove palette color', () => { if (state.palette.length === 1) return; state.palette.splice(index, 1); renderAll(); });
      row.append(label, input, hex, remove); list.append(row);
    });
    $('paletteCount').textContent = `${state.palette.length} color${state.palette.length === 1 ? '' : 's'}`;
  }
  function setStatus(message) { $('status').textContent = message; clearTimeout(setStatus.timer); setStatus.timer = setTimeout(() => $('status').textContent = '', 2600); }
  function reset() { Object.assign(state, { model: '8', nodes: [], selected: 0, canvasWidth: 15, canvasHeight: 15, width: 128, height: 128, preserveCanvasRatio: false, thicknessFraction: 64, inset: 1, assetName: 'MyVector', palette: DEFAULT_PALETTE.map(color => [...color]) }); $('packedInput').value = DEFAULT_PACKED_IMAGE; importPacked(); setStatus('Reset.'); }
  function numberInput(node, property, label) {
    const input = document.createElement('input'); input.type = 'number'; input.min = 0; input.max = model().axisMax; input.value = node[property] ?? 0; input.title = label;
    input.addEventListener('click', event => event.stopPropagation()); input.addEventListener('pointerdown', event => event.stopPropagation());
    input.addEventListener('change', event => { node[property] = clamp(event.target.value, 0, model().axisMax); renderAll(); }); return input;
  }
  function nodeColor(index) {
    let color = state.palette[0] || DEFAULT_PALETTE[0];
    state.nodes.slice(0, index + 1).forEach(node => { if (primitiveFor(node.primitive).name === 'SetColor') color = state.palette[node.value % state.palette.length] || color; });
    return colorHex(color);
  }
  const { primitiveIcon, createActionButton, createToolPalette } = window.EgfxVectorEditorUi;
  function renderNodeEditor() {
    const editor = $('nodeEditor'); editor.replaceChildren();
    const node = state.nodes[state.selected];
    if (!node) { editor.textContent = 'Select a node to edit its properties.'; return; }
    const primitive = primitiveFor(node.primitive);
    const select = document.createElement('select'); PRIMITIVES.forEach(entry => select.add(new Option(entry.name, entry.name, false, node.primitive === entry.name)));
    select.addEventListener('change', event => { Object.assign(node, normalizeNode({ ...node, primitive: event.target.value })); renderAll(); });
    const fields = document.createElement('div'); fields.className = 'node-fields';
    if (primitive.value) fields.append(numberInput(node, 'value', primitive.value));
    else if (primitive.quadrant) fields.append(numberInput(node, 'x', 'Center X'), numberInput(node, 'y', 'Center Y'), numberInput(node, 'x2', 'Radius'), quadrantInput(node));
    else if (primitive.arc) { fields.append(numberInput(node, 'x', 'Center X'), numberInput(node, 'y', 'Center Y'), numberInput(node, 'x2', primitive.name.endsWith('Fill') ? 'Outer radius' : 'Radius')); if (primitive.name.endsWith('Fill')) fields.append(numberInput(node, 'y2', 'Inner radius')); fields.append(angleInput(node, 'x3', 'Start angle'), angleInput(node, 'y3', 'End angle')); }
    else if (primitive.name === 'Ring' || primitive.name === 'RingFill') fields.append(numberInput(node, 'x', 'Center X'), numberInput(node, 'y', 'Center Y'), numberInput(node, 'x2', 'Outer radius'), numberInput(node, 'y2', 'Inner radius'));
    else if (primitive.operands) for (let operand = 1; operand <= primitive.operands; operand++) fields.append(numberInput(node, operand === 1 ? 'x' : `x${operand}`, `Point ${operand} X`), numberInput(node, operand === 1 ? 'y' : `y${operand}`, `Point ${operand} Y`));
    else if (primitive.name === 'Point' ) fields.append(numberInput(node, 'x', 'X coordinate'), numberInput(node, 'y', 'Y coordinate'));
    const title = document.createElement('strong'); title.textContent = `${primitiveIcon(primitive.name)} ${primitive.name}`;
    const identity = document.createElement('div'); identity.className = 'node-identity';
    const actions = document.createElement('div'); actions.className = 'node-toolbar';
    actions.append(
      actionButton('↑', 'Move node up', () => moveNode(state.selected, -1)),
      actionButton('↓', 'Move node down', () => moveNode(state.selected, 1)),
      actionButton('+', 'Insert node after this node', addNode)
    );
    identity.append(title, select, actions);
    editor.append(identity, fields);
  }
  function renderNodeList() {
    const list = $('nodeList'); list.replaceChildren();
    state.nodes.forEach((node, index) => {
      const item = document.createElement('button'); item.type = 'button'; item.className = `node-chip${index === state.selected ? ' selected' : ''}`; item.title = `${index}: ${node.primitive}`; item.style.setProperty('--node-color', nodeColor(index));
      item.innerHTML = `<span class="node-index">${index}</span><span class="node-icon">${primitiveIcon(node.primitive)}</span><span>${node.primitive}</span>`;
      item.addEventListener('click', () => { state.selected = index; renderAll(); }); list.append(item);
    });
    $('nodeCount').textContent = `${state.nodes.length} node${state.nodes.length === 1 ? '' : 's'}`;
    renderNodeEditor();
  }
  function actionButton(text, label, action) { return createActionButton(text, label, action); }
  function renderToolPalette() { createToolPalette($('toolPalette'), [{ name: 'Select' }, ...PRIMITIVES], () => state.activeTool, tool => { state.activeTool = tool; state.draft = null; state.dragStart = null; state.editDrag = null; if (tool !== 'Select') state.selected = -1; $('dynamicPreview').classList.toggle('select-mode', tool === 'Select'); renderAll(); }); }
  function moveNode(index, delta) { const target = index + delta; if (target < 0 || target >= state.nodes.length) return; [state.nodes[index], state.nodes[target]] = [state.nodes[target], state.nodes[index]]; state.selected = target; renderAll(); }
  function addNode() { state.nodes.splice(state.selected + 1, 0, point()); state.selected++; renderAll(); }
  function duplicateNode() { if (!state.nodes.length) return addNode(); state.nodes.splice(state.selected + 1, 0, { ...state.nodes[state.selected] }); state.selected++; renderAll(); }
  function deleteNode() { if (!state.nodes.length) return; state.nodes.splice(state.selected, 1); state.selected = Math.min(state.selected, state.nodes.length - 1); renderAll(); }

  function scaledPoint(source, width, height) { return { x: scaleCoordinate(source.x, scaleFactor(width, state.canvasWidth)), y: scaleCoordinate(source.y, scaleFactor(height, state.canvasHeight)) }; }
  function raster(width, height, offsetX = 0, offsetY = 0, renderWidth = width, renderHeight = height) {
    const canvas = document.createElement('canvas');
    canvas.width = width;
    canvas.height = height;
    const context = canvas.getContext('2d');
    context.imageSmoothingEnabled = false;
    const scaleX = scaleFactor(renderWidth, state.canvasWidth);
    const scaleY = scaleFactor(renderHeight, state.canvasHeight);
    const mapX = value => offsetX + scaleCoordinate(value, scaleX);
    const mapY = value => offsetY + scaleCoordinate(value, scaleY);
    const mapLengthX = value => Math.max(0, scaleCoordinate(value, scaleX));
    const mapLengthY = value => Math.max(0, scaleCoordinate(value, scaleY));
    const nodes = [...state.nodes, ...(state.draft ? [state.draft] : [])].map(node => {
      const primitive = primitiveFor(node.primitive);
      if (primitive.value) return { ...node };

      const mapped = { ...node, x: mapX(node.x), y: mapY(node.y) };
      if (primitive.arc) {
        mapped.x2 = mapLengthX(node.x2);
        mapped.y2 = primitive.name.endsWith('Fill') ? mapLengthX(node.y2) : 0;
        mapped.x3 = node.x3;
        mapped.y3 = node.y3;
      }
      else if (primitive.quadrant) {
        mapped.x2 = mapLengthX(node.x2);
        mapped.y2 = node.y2;
      }
      else if (primitive.name === 'Ring' || primitive.name === 'RingFill') {
        mapped.x2 = mapLengthX(node.x2);
        mapped.y2 = mapLengthY(node.y2);
      }
      else {
        ['x2', 'x3', 'x4'].forEach(property => { if (node[property] !== undefined) mapped[property] = mapX(node[property]); });
        ['y2', 'y3', 'y4'].forEach(property => { if (node[property] !== undefined) mapped[property] = mapY(node[property]); });
      }
      return mapped;
    });

    window.EgfxVectorRenderer.render(context, nodes, width, height, 1, {
      angleMax: Math.max(1, model().axisMax),
      palette: state.palette,
      renderScale: Math.max(scaleX, scaleY),
      thicknessFraction: state.thicknessFraction
    });
    return context.getImageData(0, 0, width, height).data;
  }
  function previewScale(pane, width, height) { const availableWidth = Math.max(1, pane.clientWidth - 24); const availableHeight = Math.max(1, pane.clientHeight - 24); return Math.max(1, Math.floor(Math.min(availableWidth / width, availableHeight / height))); }
  function nodePoints(node) { const primitive = primitiveFor(node.primitive); if (!primitive.operands) return primitive.name === 'Point' ? [{ x: node.x, y: node.y, key: 'x' }] : []; if (primitive.quadrant) return [{ x: node.x, y: node.y, key: 'x' }, { x: node.x + Math.abs(node.x2 || 0), y: node.y, key: 'x2' }]; if (primitive.arc) return [{ x: node.x, y: node.y, key: 'x' }, { x: node.x + Math.abs(node.x2 || 0), y: node.y, key: 'x2' }]; return Array.from({ length: primitive.operands }, (_, index) => ({ x: index ? node[`x${index + 1}`] : node.x, y: index ? node[`y${index + 1}`] : node.y, key: index ? `x${index + 1}` : 'x' })); }
  function distanceToSegment(pointValue, a, b) { const dx = b.x - a.x; const dy = b.y - a.y; const length = dx * dx + dy * dy; const t = length ? clamp(((pointValue.x - a.x) * dx + (pointValue.y - a.y) * dy) / length, 0, 1) : 0; return Math.hypot(pointValue.x - (a.x + t * dx), pointValue.y - (a.y + t * dy)); }
  function pointInPolygon(pointValue, points) { let inside = false; for (let index = 0, previous = points.length - 1; index < points.length; previous = index++) { const a = points[index]; const b = points[previous]; if (((a.y > pointValue.y) !== (b.y > pointValue.y)) && pointValue.x < (b.x - a.x) * (pointValue.y - a.y) / (b.y - a.y) + a.x) inside = !inside; } return inside; }
   function hitNode(node, pointValue) { const primitive = primitiveFor(node.primitive); const points = nodePoints(node); const tolerance = Math.max(1, Math.max(state.canvasWidth, state.canvasHeight) / 24); if (!points.length) return false; if (primitive.name === 'Point') return Math.hypot(pointValue.x - points[0].x, pointValue.y - points[0].y) <= tolerance; if (primitive.quadrant) { const radius = Math.abs(node.x2 || 0); const distance = Math.hypot(pointValue.x - node.x, pointValue.y - node.y); return primitive.name.endsWith('Fill') ? distance <= radius : Math.abs(distance - radius) <= tolerance; } if (primitive.arc) { const outerRadius = Math.abs(node.x2 || 0); const innerRadius = primitive.name === 'ArcFill' ? Math.abs(node.y2 || 0) : Math.max(0, outerRadius - Math.max(1, state.thicknessFraction)); const distance = Math.hypot(pointValue.x - node.x, pointValue.y - node.y); if (distance < innerRadius - tolerance || distance > outerRadius + tolerance) return false; const angleMax = Math.max(1, model().axisMax); const angleRange = 256; const start = (node.x3 || 0) * angleRange / angleMax; const sweep = ((((node.y3 || 0) - (node.x3 || 0)) % angleMax) + angleMax) % angleMax * angleRange / angleMax; const angle = ((Math.atan2(pointValue.x - node.x, -(pointValue.y - node.y)) / (Math.PI * 2)) * angleRange + angleRange) % angleRange; return sweep > 0 && ((angle - start + angleRange) % angleRange) <= sweep; } if (primitive.name.startsWith('Rectangle')) { const left = Math.min(points[0].x, points[1].x); const right = Math.max(points[0].x, points[1].x); const top = Math.min(points[0].y, points[1].y); const bottom = Math.max(points[0].y, points[1].y); return primitive.name.endsWith('Fill') ? pointValue.x >= left && pointValue.x <= right && pointValue.y >= top && pointValue.y <= bottom : [ [{ x: left, y: top }, { x: right, y: top }], [{ x: right, y: top }, { x: right, y: bottom }], [{ x: right, y: bottom }, { x: left, y: bottom }], [{ x: left, y: bottom }, { x: left, y: top }] ].some(segment => distanceToSegment(pointValue, ...segment) <= tolerance); } if (primitive.name.startsWith('Triangle')) return primitive.name.endsWith('Fill') ? pointInPolygon(pointValue, points) : points.some((point, index) => distanceToSegment(pointValue, point, points[(index + 1) % points.length]) <= tolerance); if (primitive.name.startsWith('Circle')) { const radius = Math.hypot(points[1].x - points[0].x, points[1].y - points[0].y); const distance = Math.hypot(pointValue.x - points[0].x, pointValue.y - points[0].y); return primitive.name.endsWith('Fill') ? distance <= radius : Math.abs(distance - radius) <= tolerance; } return false; }
  function keypointAt(pointValue) { if (state.selected < 0 || !state.nodes[state.selected]) return null; const tolerance = Math.max(1, Math.max(state.canvasWidth, state.canvasHeight) / 18); return nodePoints(state.nodes[state.selected]).find(point => Math.hypot(pointValue.x - point.x, pointValue.y - point.y) <= tolerance) || null; }
  function componentAt(pointValue) { for (let index = state.nodes.length - 1; index >= 0; index--) if (hitNode(state.nodes[index], pointValue)) return index; return -1; }
   function renderSelectionOverlay(context, width, height) { if (context.canvas.id !== 'dynamicCanvas' || state.activeTool !== 'Select' || state.selected < 0 || !state.nodes[state.selected]) return; const points = nodePoints(state.nodes[state.selected]).map(pointValue => scaledPoint(pointValue, width, height)); if (!points.length) return; context.save(); context.strokeStyle = '#ff3bd4'; context.fillStyle = '#101419'; context.lineWidth = 2; context.setLineDash([4, 4]); const minX = Math.min(...points.map(pointValue => pointValue.x)) - 4; const maxX = Math.max(...points.map(pointValue => pointValue.x)) + 4; const minY = Math.min(...points.map(pointValue => pointValue.y)) - 4; const maxY = Math.max(...points.map(pointValue => pointValue.y)) + 4; context.strokeRect(minX, minY, maxX - minX, maxY - minY); context.setLineDash([]); points.forEach(pointValue => { context.beginPath(); context.arc(pointValue.x, pointValue.y, 5, 0, Math.PI * 2); context.fill(); context.stroke(); }); context.restore(); }
  function renderCanvas(canvasId, paneId, width, height, dimensionsId) { const canvas = $(canvasId); const pane = $(paneId); const scale = previewScale(pane, width, height); canvas.width = width; canvas.height = height; canvas.style.width = `${width * scale}px`; canvas.style.height = `${height * scale}px`; const context = canvas.getContext('2d'); context.imageSmoothingEnabled = false; context.fillStyle = '#000'; context.fillRect(0, 0, width, height); const xInset = insetPixels(width, height, state.canvasWidth, state.canvasHeight); const yInset = insetPixels(height, width, state.canvasHeight, state.canvasWidth); const drawableWidth = Math.max(1, width - xInset * 2); const drawableHeight = Math.max(1, height - yInset * 2); const vectorImage = new ImageData(raster(width, height, xInset, yInset, drawableWidth, drawableHeight), width, height); const drawTrace = target => { if (state.trace.visible && state.trace.image) { target.globalAlpha = state.trace.opacity; target.imageSmoothingEnabled = false; target.drawImage(state.trace.image, 0, 0, width, height); target.globalAlpha = 1; } }; if (state.trace.layer === 'background') { const composed = document.createElement('canvas'); composed.width = width; composed.height = height; const composedContext = composed.getContext('2d'); drawTrace(composedContext); composedContext.putImageData(vectorImage, 0, 0); context.drawImage(composed, 0, 0); } else { context.putImageData(vectorImage, 0, 0); drawTrace(context); } context.save(); context.translate(xInset, yInset); renderSelectionOverlay(context, drawableWidth, drawableHeight); context.restore(); $(dimensionsId).textContent = `${width}×${height} · ${scale}×`; }
  function renderPreview() { renderCanvas('nativeCanvas', 'nativePreview', state.canvasWidth, state.canvasHeight, 'nativeDimensions'); renderCanvas('dynamicCanvas', 'dynamicPreview', state.width, state.height, 'dynamicDimensions'); }
  function renderAll() { const m = model(); state.width = clamp(state.width, OUTPUT_MIN, OUTPUT_MAX); state.height = clamp(state.height, OUTPUT_MIN, OUTPUT_MAX); state.inset = clamp(state.inset, 0, Math.min(m.axisMax, Math.floor(Math.max(0, Math.min(state.canvasWidth, state.canvasHeight) - 1) / 2))); const outputScale = Math.max(state.width, state.height); $('modelSelect').value = state.model; $('modelInfo').textContent = `${m.name}: coordinates 0–${m.axisMax}, canvas up to ${m.native}×${m.native}.`; $('canvasWidth').max = m.native; $('canvasHeight').max = m.native; $('canvasWidth').value = state.canvasWidth; $('canvasHeight').value = state.canvasHeight; $('outputWidth').value = state.width; $('outputHeight').value = state.height; $('outputScaleSlider').value = outputScale; $('outputScaleValue').value = `${state.width}×${state.height}`; $('outputCanvasRatio').checked = state.preserveCanvasRatio; $('thicknessFraction').value = state.thicknessFraction; $('thicknessFractionSlider').value = state.thicknessFraction; $('thicknessFractionValue').value = state.thicknessFraction; $('inset').max = m.axisMax; $('inset').value = state.inset; $('insetSlider').max = m.axisMax; $('insetSlider').value = state.inset; $('insetValue').value = state.inset; $('assetName').value = state.assetName; $('traceVisible').checked = state.trace.visible; $('traceLayer').value = state.trace.layer; $('traceOpacity').value = state.trace.opacity * 100; $('traceOpacityValue').value = `${state.trace.opacity * 100}%`; $('dynamicPreview').classList.toggle('select-mode', state.activeTool === 'Select'); $('packedInput').value = packedText(); $('cppOutput').value = cppText(); renderToolPalette(); renderNodeList(); renderPalette(); renderPreview(); persist(); }

  function sourcePointFromPointer(event) { const canvas = $('dynamicCanvas'); const bounds = canvas.getBoundingClientRect(); const x = clamp((event.clientX - bounds.left) / Math.max(1, bounds.width), 0, 1); const y = clamp((event.clientY - bounds.top) / Math.max(1, bounds.height), 0, 1); return { x: clamp(Math.floor(x * Math.max(1, state.canvasWidth - 1)), 0, model().axisMax), y: clamp(Math.floor(y * Math.max(1, state.canvasHeight - 1)), 0, model().axisMax) }; }
  function draftGeometry(start, end) { const tool = state.activeTool; if (tool === 'Rectangle' || tool === 'RectangleFill' || tool === 'Circle' || tool === 'CircleFill') return { primitive: tool, x: start.x, y: start.y, x2: end.x, y2: end.y }; if (tool === 'Ring' || tool === 'RingFill') { const outerRadius = Math.abs(end.x - start.x); return { primitive: tool, x: start.x, y: start.y, x2: outerRadius, y2: Math.floor(outerRadius / 2) }; } if (tool === 'Arc' || tool === 'ArcFill') { const radius = Math.abs(end.x - start.x); return { primitive: tool, x: start.x, y: start.y, x2: radius, y2: tool === 'ArcFill' ? Math.floor(radius / 2) : 0, x3: 0, y3: Math.floor(model().axisMax / 4) }; } if (tool === 'CircleQuadrant' || tool === 'CircleQuadrantFill') { const horizontal = end.x >= start.x; const vertical = end.y >= start.y; const quadrant = horizontal ? (vertical ? 1 : 0) : (vertical ? 2 : 3); return { primitive: tool, x: start.x, y: start.y, x2: Math.abs(end.x - start.x), y2: QUADRANTS.find(entry => entry.value === quadrant).value }; } if (tool === 'Triangle' || tool === 'TriangleFill') return { primitive: tool, x: start.x, y: start.y, x2: end.x, y2: start.y, x3: end.x, y3: end.y }; return null; }
  function commitToolPoint(pointValue) { if (state.activeTool === 'Break') state.nodes.push({ primitive: 'Break' }); else if (state.activeTool === 'Point') state.nodes.push(point(pointValue.x, pointValue.y)); else return false; state.selected = state.nodes.length - 1; renderAll(); return true; }
  function beginCanvasGesture(event) { const pointValue = sourcePointFromPointer(event); if (state.activeTool === 'Select') { const keypoint = keypointAt(pointValue); if (keypoint) state.editDrag = { type: 'keypoint', key: keypoint.key }; else { const component = componentAt(pointValue); state.selected = component; if (component >= 0) state.editDrag = { type: 'move', origin: pointValue, node: { ...state.nodes[component] } }; } renderNodeList(); renderPreview(); if (state.editDrag) $('dynamicCanvas').setPointerCapture(event.pointerId); return; } if (commitToolPoint(pointValue)) return; state.dragStart = pointValue; state.draft = draftGeometry(pointValue, pointValue); $('dynamicCanvas').setPointerCapture(event.pointerId); renderPreview(); }
  function updateCanvasGesture(event) { const pointValue = sourcePointFromPointer(event); if (state.activeTool === 'Select') { if (!state.editDrag || state.selected < 0) return; const node = state.nodes[state.selected]; if (state.editDrag.type === 'keypoint') { if (state.editDrag.key === 'x2' && primitiveFor(node.primitive).quadrant) node.x2 = clamp(Math.abs(pointValue.x - node.x), 0, model().axisMax); else { node[state.editDrag.key] = clamp(pointValue.x, 0, model().axisMax); const yKey = state.editDrag.key === 'x' ? 'y' : state.editDrag.key.replace(/^x/, 'y'); node[yKey] = clamp(pointValue.y, 0, model().axisMax); } } else { const dx = pointValue.x - state.editDrag.origin.x; const dy = pointValue.y - state.editDrag.origin.y; const original = state.editDrag.node; if (primitiveFor(node.primitive).quadrant) { node.x = clamp(original.x + dx, 0, model().axisMax); node.y = clamp(original.y + dy, 0, model().axisMax); } else nodePoints(original).forEach(point => { const xKey = point.key; const yKey = xKey === 'x' ? 'y' : xKey.replace(/^x/, 'y'); node[xKey] = clamp(original[xKey] + dx, 0, model().axisMax); node[yKey] = clamp(original[yKey] + dy, 0, model().axisMax); }); } renderPreview(); return; } if (!state.dragStart) return; state.draft = draftGeometry(state.dragStart, pointValue); renderPreview(); }
  function endCanvasGesture(event) { if (state.activeTool === 'Select') { if (state.editDrag) { state.editDrag = null; renderAll(); } return; } if (!state.dragStart) return; const draft = draftGeometry(state.dragStart, sourcePointFromPointer(event)); state.dragStart = null; state.draft = null; if (draft) { state.nodes.push(normalizeNode(draft)); state.selected = state.nodes.length - 1; } renderAll(); }
  function cancelCanvasGesture() { state.dragStart = null; state.draft = null; state.editDrag = null; renderPreview(); }

  $('modelSelect').addEventListener('change', event => { state.model = event.target.value; state.canvasWidth = clamp(state.canvasWidth, 1, model().native); state.canvasHeight = clamp(state.canvasHeight, 1, model().native); state.nodes = state.nodes.map(normalizeNode); renderAll(); });
  $('canvasWidth').addEventListener('change', event => { state.canvasWidth = clamp(event.target.value, 1, model().native); renderAll(); }); $('canvasHeight').addEventListener('change', event => { state.canvasHeight = clamp(event.target.value, 1, model().native); renderAll(); });
  function setOutputWidth(value) { state.width = clamp(value, OUTPUT_MIN, OUTPUT_MAX); renderAll(); }
  function setOutputHeight(value) { state.height = clamp(value, OUTPUT_MIN, OUTPUT_MAX); renderAll(); }
  function setOutputScale(value) { const scale = clamp(value, OUTPUT_MIN, OUTPUT_MAX); const ratio = state.preserveCanvasRatio ? state.canvasWidth / Math.max(1, state.canvasHeight) : state.width / Math.max(1, state.height); if (ratio >= 1) { state.width = scale; state.height = clamp(Math.round(scale / ratio), OUTPUT_MIN, OUTPUT_MAX); } else { state.height = scale; state.width = clamp(Math.round(scale * ratio), OUTPUT_MIN, OUTPUT_MAX); } renderAll(); }
  function setThicknessFraction(value) { state.thicknessFraction = clamp(value, 0, 128); renderAll(); }
  $('outputWidth').addEventListener('change', event => setOutputWidth(event.target.value));
  $('outputHeight').addEventListener('change', event => setOutputHeight(event.target.value));
  $('outputScaleSlider').addEventListener('input', event => setOutputScale(event.target.value));
  $('outputCanvasRatio').addEventListener('change', event => { state.preserveCanvasRatio = event.target.checked; renderAll(); });
  $('thicknessFraction').addEventListener('change', event => setThicknessFraction(event.target.value)); $('thicknessFractionSlider').addEventListener('input', event => setThicknessFraction(event.target.value));
  function setInset(value) { state.inset = clamp(value, 0, model().axisMax); renderAll(); }
  $('inset').addEventListener('change', event => setInset(event.target.value)); $('insetSlider').addEventListener('input', event => setInset(event.target.value));
  $('assetName').addEventListener('input', event => { state.assetName = event.target.value.replace(/[^A-Za-z0-9_]/g, '_'); renderAll(); });
  $('addButton').addEventListener('click', addNode); $('duplicateButton').addEventListener('click', duplicateNode); $('deleteButton').addEventListener('click', deleteNode); $('importButton').addEventListener('click', importPacked); $('copyPackedButton').addEventListener('click', () => copyText(packedText(), 'Packed data copied.')); $('copyCppButton').addEventListener('click', () => copyText(cppText(), 'C++ declaration copied.')); $('resetButton').addEventListener('click', reset); $('addPaletteButton').addEventListener('click', () => { state.palette.push([255, 255, 255]); renderAll(); });
  $('traceImageInput').addEventListener('change', event => { const file = event.target.files[0]; if (!file) return; const reader = new FileReader(); reader.addEventListener('load', () => { const image = new Image(); image.addEventListener('load', () => { state.trace.image = image; state.trace.visible = true; $('traceVisible').checked = true; renderPreview(); setStatus('Tracing reference loaded.'); }); image.src = reader.result; }); reader.readAsDataURL(file); });
  $('traceVisible').addEventListener('change', event => { state.trace.visible = event.target.checked; renderPreview(); }); $('traceLayer').addEventListener('change', event => { state.trace.layer = event.target.value; renderPreview(); }); $('traceOpacity').addEventListener('input', event => { state.trace.opacity = Number(event.target.value) / 100; $('traceOpacityValue').value = `${event.target.value}%`; renderPreview(); }); $('traceClearButton').addEventListener('click', () => { state.trace.image = null; $('traceImageInput').value = ''; renderPreview(); setStatus('Tracing reference cleared.'); });
  document.addEventListener('keydown', event => { if (event.key === 'Escape') { state.selected = -1; cancelCanvasGesture(); renderNodeList(); } });
  $('dynamicCanvas').addEventListener('pointerdown', beginCanvasGesture); $('dynamicCanvas').addEventListener('pointermove', updateCanvasGesture); $('dynamicCanvas').addEventListener('pointerup', endCanvasGesture); $('dynamicCanvas').addEventListener('pointercancel', cancelCanvasGesture);
  const previewObserver = new ResizeObserver(() => renderPreview()); previewObserver.observe($('nativePreview')); previewObserver.observe($('dynamicPreview'));
  $('packedInput').value = DEFAULT_PACKED_IMAGE;
  importPacked();
})();
