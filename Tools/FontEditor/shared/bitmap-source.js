const BitmapAtlasSource = (() => {
  function parse(text) {
	const dimensions = text.match(/\b(?:Width|MaxWidth)\s*[:=]\s*(\d+)[\s\S]*?\bHeight\s*[:=]\s*(\d+)/i);
	const space = text.match(/\bSpaceWidth\s*[:=]\s*(\d+)/i);
	const format = text.match(/\bFormat\s*[:=]\s*(RGB332|RGB565|RGB888)\b/i);
	const atlasBody = text.match(/\bAtlas\s*\[\s*\]\s*[^\{]*\{([\s\S]*?)\}/i)?.[1] || text;
	const blocks = [...atlasBody.matchAll(/\/\/\s*Glyph:\s*([^\r\n]+)\r?\n([\s\S]*?)(?=\/\/\s*Glyph:|$)/gi)];
	const values = blocks.flatMap(block => (block[2].match(/0x[\da-f]+|\b\d+\b/gi) || []).map(token => Number.parseInt(token, token.toLowerCase().startsWith('0x') ? 16 : 10)));
	return {
	  width: dimensions ? Number(dimensions[1]) : 0,
	  height: dimensions ? Number(dimensions[2]) : 0,
	  spaceWidth: space ? Number(space[1]) : null,
		format: format ? format[1].toLowerCase() : 'rgb332',
		blocks: blocks.map(block => {
		const metadata = block[1].match(/^(.*?)\s+Width:\s*(\d+)\s*$/i);
		return { id: (metadata ? metadata[1] : block[1]).trim(), width: metadata ? Number(metadata[2]) : null, values: (block[2].match(/0x[\da-f]+|\b\d+\b/gi) || []).map(token => Number.parseInt(token, token.toLowerCase().startsWith('0x') ? 16 : 10)) };
	  }),
	  values
	};
  }

	function serialize(state, format, sourceNamespace) {
	const lines = [];
	state.glyphs.forEach((glyph, index) => {
		lines.push(`    // Glyph: ${glyph.id || `Glyph${index}`} Width: ${glyph.width}`);
		const bytes = format.packGlyph(glyph);
	  for (let offset = 0; offset < bytes.length; offset += 12) lines.push(`    ${bytes.slice(offset, offset + 12).map(value => `0x${value.toString(16).padStart(2, '0').toUpperCase()}`).join(', ')},`);
	});
	return `// Bitmap Atlas: Width: ${state.width} Height: ${state.height} Format: ${format.name} SpaceWidth: ${state.spaceWidth}\nnamespace ${sourceNamespace}\n{\n\tstatic constexpr uint8_t Width = ${state.width};\n\tstatic constexpr uint8_t Height = ${state.height};\n\tstatic constexpr uint8_t SpaceWidth = ${state.spaceWidth};\n\n\tstatic constexpr uint8_t Atlas[] PROGMEM\n\t{\n${lines.map(line => `\t${line}`).join('\n')}\n\t};\n}`;
  }

  return { parse, serialize };
})();
