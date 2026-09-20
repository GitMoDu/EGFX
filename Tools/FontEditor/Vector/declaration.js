(() => {
  'use strict';

  function identifier(value, fallback) { return (value || fallback).replace(/[^A-Za-z0-9_]/g, '_'); }
  function characterLiteral(character) {
	if (character === "'") return "'\\\''";
	if (character === '\\') return "'\\\\'";
	return `'${character}'`;
  }
  function generate(state) {
	const asset = identifier(state.assetName, 'MyVectorFont');
	const source = identifier(state.sourceNamespace, 'SourceVector');
	const glyphs = state.glyphs;
	let offset = 0;
	const entries = glyphs.map(glyph => {
	  const values = IntegerGlassVectorFontModel.packGlyph(glyph);
	  const entry = { glyph, offset, count: values.length, values };
	  offset += values.length;
	  return entry;
	});
	const atlas = entries.map(entry => `        // Glyph: ${identifier(entry.glyph.id, 'Glyph')} Character: ${characterLiteral(entry.glyph.character)} Width: ${entry.glyph.width} Offset: ${entry.offset} Count: ${entry.count}\n        ${entry.values.map(value => `0x${value.toString(16).padStart(2, '0').toUpperCase()}`).join(', ')}`).join(',\n');
	const cases = entries.map(entry => `        case ${characterLiteral(entry.glyph.character)}:\n            return glyph_t{ ${source}::Atlas + ${entry.offset}, ${entry.count}, ${entry.glyph.width}, Height };`).join('\n');
	const advances = glyphs.map(glyph => `        case ${characterLiteral(glyph.character)}:\n            return ${glyph.width};`).join('\n');
	return `#include <IntegerGlassFramework.h>\n\nnamespace ${source}\n{\n    static constexpr uint8_t Width = ${state.maxWidth};\n    static constexpr uint8_t Height = ${state.height};\n    static constexpr uint8_t SpaceWidth = ${state.spaceWidth};\n\n    static constexpr uint8_t Atlas[] PROGMEM\n    {\n${atlas}\n    };\n}\n\nstruct ${asset}\n{\n    using dimension_t = uint8_t;\n    using glyph_t = IntegerGlass::Framework::Text::Vector::Glyph15x15::Glyph;\n\n    static constexpr dimension_t Width = ${source}::Width;\n    static constexpr dimension_t Height = ${source}::Height;\n    static constexpr IntegerGlass::Framework::Image::DataSourceTypeEnum DataSourceType = IntegerGlass::Framework::Image::DataSourceTypeEnum::Flash;\n\n    void SetFontWidth(const dimension_t) {}\n    void SetFontHeight(const dimension_t) {}\n\n    dimension_t GetFontWidth() const { return Width; }\n    dimension_t GetFontHeight() const { return Height; }\n    dimension_t GetSpaceWidth() const { return ${source}::SpaceWidth; }\n    dimension_t GetTabWidth() const { return Width * 2; }\n    dimension_t GetKerningWidth() const { return Width / 3; }\n    dimension_t GetLineSpacing() const { return 1; }\n\n    static glyph_t GetGlyph(const uint8_t character)\n    {\n        switch (static_cast<char>(character))\n        {\n${cases}\n        default:\n            return glyph_t{ nullptr, 0, 0, 0 };\n        }\n    }\n\n    dimension_t GetAdvance(const char printableChar) const\n    {\n        switch (printableChar)\n        {\n${advances}\n        default:\n            return 0;\n        }\n    }\n};`;
  }
  window.IntegerGlassVectorFontDeclaration = { generate };
})();
