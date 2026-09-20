(() => {
  'use strict';

  const MAX_COORDINATE = 14;
  const OPCODE_COORDINATE = 15;
	const PRIMITIVES = window.IntegerGlassVectorEditorConfig.PRIMITIVES;
  const BY_NAME = window.IntegerGlassVectorEditorConfig.PRIMITIVE_BY_NAME;
  const BY_OPCODE = window.IntegerGlassVectorEditorConfig.PRIMITIVE_BY_OPCODE;

  function clamp(value) { return Math.max(0, Math.min(MAX_COORDINATE, Number(value) || 0)); }
  function primitive(name) { return BY_NAME.get(name) || PRIMITIVES[0]; }
  function point(x = 0, y = 0) { return { x: clamp(x), y: clamp(y) }; }
  function packPoint(value) { return (clamp(value.x) << 4) | clamp(value.y); }
  function packOpcode(opcode) { return (OPCODE_COORDINATE << 4) | opcode; }
  function unpack(value) {
	const x = (value >> 4) & 0x0f;
	const y = value & 0x0f;
	if (x === OPCODE_COORDINATE) return { primitive: BY_OPCODE.get(y)?.name || 'Break' };
	return { primitive: 'Point', x, y };
  }
  function packGlyph(glyph) {
	return glyph.nodes.flatMap(node => {
	  const selected = primitive(node.primitive);
	  if (selected.name === 'Point') return [packPoint(node)];
	  const values = [packOpcode(selected.opcode)];
	  if (selected.value) return values.concat([packPoint({ x: node.value, y: 0 })]);
	  for (let index = 1; index <= (selected.operands || 0); index++) values.push(packPoint(index === 1 ? node : node[`x${index}`] !== undefined ? { x: node[`x${index}`], y: node[`y${index}`] } : point()));
	  return values;
	});
  }
  function decode(values) {
	const nodes = [];
	for (let index = 0; index < values.length; index++) {
	  const current = unpack(values[index]);
	  const selected = primitive(current.primitive);
	  if (selected.name === 'Point') { nodes.push(current); continue; }
	  if (selected.value) { nodes.push({ primitive: selected.name, value: unpack(values[++index] || 0).x }); continue; }
	  const operands = [];
	  for (let operand = 0; operand < (selected.operands || 0) && index + 1 < values.length; operand++) operands.push(unpack(values[++index]));
	  const node = { ...(operands[0] || point()), primitive: selected.name };
	  operands.slice(1).forEach((operand, operandIndex) => { node[`x${operandIndex + 2}`] = operand.x; node[`y${operandIndex + 2}`] = operand.y; });
	  nodes.push(node);
	}
	return nodes;
  }
  function glyph(id, width = 8, nodes = []) { return { id, width: clamp(width), nodes }; }
  window.IntegerGlassVectorFontModel = { MAX_COORDINATE, PRIMITIVES, BY_NAME, clamp, primitive, point, packGlyph, decode, glyph };
})();
