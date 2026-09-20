const BitmapFontDeclaration = (() => {
  function identifier(value, fallback) {
	return (value || fallback).replace(/[^A-Za-z0-9_]/g, '_');
  }

  function characterLiteral(character) {
	if (character === "'") return String.fromCharCode(39, 92, 39, 39);
	if (character === '\\') return String.fromCharCode(39, 92, 92, 39);
	return `'${character}'`;
  }

  function generate(state, format, characterForGlyphId, sourceNamespace, fontName) {
	let offset = 0;
	const cases = state.glyphs.flatMap(glyph => {
	  const character = characterForGlyphId(glyph.id || '');
	  const glyphOffset = offset;
	  offset += format.packGlyph(glyph).length;
	  return character ? [`\t\t\t\t\tcase ${characterLiteral(character)}:\n\t\t\t\t\t\treturn glyph_t{ ${sourceNamespace}::Atlas + ${glyphOffset}, ${glyph.width}, ${sourceNamespace}::Height };`] : [];
	});
	const advanceCases = state.glyphs.flatMap(glyph => {
	  const character = characterForGlyphId(glyph.id || '');
	  return character ? [`\t\t\t\t\tcase ${characterLiteral(character)}:\n\t\t\t\t\t\treturn ${glyph.width};`] : [];
	});
	const name = identifier(fontName, 'MyBitmapFont');
	return [
	  '#include <IntegerGlassFramework.h>',
	  '',
	  'struct ' + name,
	  '{',
	  '\tusing dimension_t = uint8_t;',
	  '\tusing color_t = uint8_t;',
	  '\tusing glyph_t = IntegerGlass::Framework::Text::Bitmap::GlyphBitmap<dimension_t, color_t>;',
	  '',
	  '\tstatic constexpr IntegerGlass::Framework::Image::DataSourceTypeEnum DataSourceType = IntegerGlass::Framework::Image::DataSourceTypeEnum::Flash;',
	  '',
	  '\tvoid SetFontWidth(const dimension_t /*width*/) {}',
	  '\tvoid SetFontHeight(const dimension_t /*height*/) {}',
	  '\tdimension_t GetFontWidth() const { return ' + sourceNamespace + '::Width; }',
	  '\tdimension_t GetFontHeight() const { return ' + sourceNamespace + '::Height; }',
	  '\tdimension_t GetSpaceWidth() const { return ' + sourceNamespace + '::SpaceWidth; }',
	  '\tdimension_t GetTabWidth() const { return ' + sourceNamespace + '::Width * 2; }',
	  '\tdimension_t GetKerningWidth() const { return ' + sourceNamespace + '::Width / 3; }',
	  '\tdimension_t GetLineSpacing() const { return 1; }',
	  '',
	  '\tstatic glyph_t GetGlyph(const uint8_t character)',
	  '\t{',
	  '\t\tswitch (static_cast<char>(character))',
	  '\t\t{',
	  cases.join('\n'),
	  '\t\tdefault:',
	  '\t\t\treturn glyph_t{ nullptr, 0, 0 };',
	  '\t\t}',
	  '\t}',
	  '',
	  '\tdimension_t GetAdvance(const char printableChar) const',
	  '\t{',
	  '\t\tswitch (printableChar)',
	  '\t\t{',
	  advanceCases.join('\n'),
	  '\t\tdefault:',
	  '\t\t\treturn 0;',
	  '\t\t}',
	  '\t}',
	  '};'
	].join('\n');
	}

  return { generate };
})();
