const BitmaskAtlasSource = (() => {
  function packedSource(text) {
	const atlas = text.match(/\bAtlas\s*\[\s*\]\s*[^\{]*\{([\s\S]*?)\}/i);
	return atlas ? atlas[1] : text;
  }

  function aliasIdentities(text) {
	return [...text.matchAll(/\b(?:Letter|Number|Symbol)[A-Za-z0-9_]*\s*=\s*Atlas\s*\+\s*(\d+)/gi)]
	  .map(match => ({ id: match[0].split(/\s*=/)[0].trim(), offset: Number(match[1]) }))
	  .sort((left, right) => left.offset - right.offset);
  }

  function parse(text) {
	const dimensions = text.match(/\b(?:MaxWidth|Width)\s*[:=]\s*(\d+)[\s\S]*?\bHeight\s*[:=]\s*(\d+)/i);
	const space = text.match(/\bSpaceWidth\s*[:=]\s*(\d+)/i);
	const blocks = [...packedSource(text).matchAll(/\/\/\s*Glyph:\s*([^\r\n]+)\r?\n([\s\S]*?)(?=\/\/\s*Glyph:|$)/gi)]
	  .map(block => {
	  const metadata = block[1].match(/^(.*?)\s+Width:\s*(\d+)(?:\s*,?\s*row-padded)?\s*$/i);
		return { id: (metadata ? metadata[1] : block[1]).trim(), width: metadata ? Number(metadata[2]) : 0, body: block[2] };
	  })
	  .filter(block => block.id.toLowerCase() !== 'space');
	const aliases = blocks.length ? [] : aliasIdentities(text);
	const values = blocks.length
	  ? blocks.flatMap(block => (block.body.match(/0x[\da-f]+|\b\d+\b/gi) || []).map(token => Number.parseInt(token, token.toLowerCase().startsWith('0x') ? 16 : 10)))
	  : packedSource(text).replace(/\/\/[^\r\n]*|\/\*[\s\S]*?\*\//g, '').match(/0x[\da-f]+|\b\d+\b/gi)?.map(token => Number.parseInt(token, token.toLowerCase().startsWith('0x') ? 16 : 10)) || [];
	return {
	  width: dimensions ? Number(dimensions[1]) : 0,
	  height: dimensions ? Number(dimensions[2]) : 0,
	  spaceWidth: space ? Number(space[1]) : null,
	  blocks,
	  aliases,
	  values
	};
  }

  function serialize(state, packGlyph, sourceNamespace) {
	const lines = [];
	state.glyphs.forEach((glyph, index) => {
	  lines.push(`    // Glyph: ${glyph.id || `Glyph${index}`} Width: ${glyph.width} row-padded`);
	  const bytes = packGlyph(glyph, state.height);
	  for (let offset = 0; offset < bytes.length; offset += 12) lines.push(`    ${bytes.slice(offset, offset + 12).map(value => `0x${value.toString(16).padStart(2, '0').toUpperCase()}`).join(', ')},`);
	});
	return `// Atlas: MaxWidth: ${state.width} Height: ${state.height} SpaceWidth: ${state.spaceWidth}\nnamespace ${sourceNamespace}\n{\n\tstatic constexpr uint8_t MaxWidth = ${state.width};\n\tstatic constexpr uint8_t Height = ${state.height};\n\tstatic constexpr uint8_t SpaceWidth = ${state.spaceWidth};\n\n\tstatic constexpr uint8_t Atlas[] PROGMEM\n\t{\n${lines.map(line => `\t${line}`).join('\n')}\n\t};\n}`;
  }

	function parseLegacyAliases(text) {
	return aliasIdentities(text);
  }

  return { parse, parseLegacyAliases, serialize };
})();
