(() => {
  'use strict';

  window.createEgfxVectorEditorData = function createData({ state, model, clamp, primitiveFor, primitiveByOpcode, point, normalizeNode, colorHex, paletteCppText, renderAll, setStatus, getElement }) {
	function encodePoint(value) {
	  const m = model();
	  const x = clamp(value.x, 0, m.axisMax);
	  const y = clamp(value.y, 0, m.axisMax);
	  return m.bytes === 1 ? (x << 4) | y : (x << 8) | y;
	}

	function encodeOpcode(opcode) {
	  const m = model();
	  return m.bytes === 1 ? (m.axisMask << 4) | opcode : (m.axisMask << 8) | opcode;
	}

	function packedValues() {
	  return state.nodes.flatMap(node => {
		const primitive = primitiveFor(node.primitive);
		if (primitive.opcode === undefined) return [encodePoint(node)];
		const values = [encodeOpcode(primitive.opcode)];
		if (primitive.value) values.push(encodePoint({ x: node.value, y: 0 }));
		for (let index = 1; index <= (primitive.operands || 0); index++) values.push(encodePoint(index === 1 ? node : { x: node[`x${index}`], y: node[`y${index}`] }));
		return values;
	  });
	}

	function packedText() { return packedValues().map(value => `0x${value.toString(16).padStart(model().bytes * 2, '0').toUpperCase()}`).join(', '); }

	function cppText() {
	  const type = model().name;
	  const values = packedValues().map(value => `0x${value.toString(16).padStart(model().bytes * 2, '0').toUpperCase()}`).join(', ');
	  const asset = state.assetName || 'MyVector';
	  return `${paletteCppText()}

static constexpr uint8_t ${asset}Data[] PROGMEM
{
	${values}
};

static constexpr source_t ${asset}Source
{
	PackedVectorEnum::${type},
	${asset}Data,
	sizeof(${asset}Data) / sizeof(${asset}Data[0]),
	${state.canvasWidth},
	${state.canvasHeight},
	${state.thicknessFraction}
};`;
	}

	function decodeRaw(raw) {
	  const m = model(); const x = m.bytes === 1 ? (raw >> 4) & m.axisMask : (raw >> 8) & m.axisMask; const y = raw & m.axisMask;
	  if (x === m.axisMask) return { opcode: y };
	  if (y === m.axisMask) return { opcode: m.axisMask + 1 + x };
	  return point(x, y);
	}

	function importPacked() {
	  const packedSource = getElement('packedInput').value.replace(/\/\*[\s\S]*?\*\//g, '').replace(/\/\/.*$/gm, '');
	  const tokens = packedSource.match(/0x[\da-f]+|\d+/gi) || [];
	  if (!tokens.length) return setStatus('No packed values found.');
	  const limit = model().bytes === 1 ? 255 : 65535; const values = tokens.map(Number);
	  if (values.some(value => !Number.isInteger(value) || value < 0 || value > limit)) return setStatus('Packed values are outside this model.');
	  const nodes = [];
	  for (let index = 0; index < values.length; index++) {
		const decoded = decodeRaw(values[index]);
		if (decoded.opcode === undefined) { nodes.push(decoded); continue; }
		const primitive = primitiveByOpcode.get(decoded.opcode);
		if (!primitive) { nodes.push({ primitive: 'Break' }); continue; }
		const operands = [];
		const count = primitive.value ? 1 : primitive.operands || 0;
		for (let operand = 0; operand < count && index + 1 < values.length; operand++) {
		  const value = decodeRaw(values[++index]);
		  if (value.opcode !== undefined) break;
		  operands.push(value);
		}
		if (primitive.value) nodes.push({ primitive: primitive.name, value: operands[0]?.x || 0 });
		else if (primitive.operands) {
		  const node = { ...operands[0] || point(), primitive: primitive.name };
		  operands.slice(1).forEach((operand, operandIndex) => { node[`x${operandIndex + 2}`] = operand.x; node[`y${operandIndex + 2}`] = operand.y; });
		  nodes.push(normalizeNode(node));
		} else nodes.push({ primitive: primitive.name });
	  }
	  const geometryNodes = nodes.filter(node => !primitiveFor(node.primitive).value && primitiveFor(node.primitive).operands !== undefined || primitiveFor(node.primitive).name === 'Point');
	const coordinates = geometryNodes.flatMap(node => {
	  const primitive = primitiveFor(node.primitive);
	  if (primitive.name === 'Point') return [node];
	  return Array.from({ length: primitive.operands || 0 }, (_, operandIndex) => operandIndex === 0
		? node
		: { x: node[`x${operandIndex + 1}`], y: node[`y${operandIndex + 1}`] });
	});
	if (coordinates.length) {
	  state.canvasWidth = Math.max(1, Math.min(model().native, Math.max(...coordinates.map(coordinate => coordinate.x)) + 1));
	  state.canvasHeight = Math.max(1, Math.min(model().native, Math.max(...coordinates.map(coordinate => coordinate.y)) + 1));
	}
	state.nodes = nodes; state.selected = 0; renderAll(); setStatus(`Imported ${nodes.length} editor node${nodes.length === 1 ? '' : 's'}.`);
	}

	function copyText(text, message) { if (navigator.clipboard) navigator.clipboard.writeText(text).then(() => setStatus(message)).catch(() => setStatus('Clipboard access was denied.')); else setStatus('Clipboard is unavailable; select the text to copy.'); }

	return { cppText, importPacked, packedText, copyText };
  };
})();
