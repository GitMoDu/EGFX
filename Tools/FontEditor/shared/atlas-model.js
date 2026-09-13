const BitmaskAtlasModel = (() => {
function pixelCount(width, height) {
  return width * height;
}

function bytesPerGlyph(glyph, height, defaultWidth) {
  return Math.ceil((glyph ? glyph.width : defaultWidth) * height / 8);
}

function createGlyph(id, width, height, pixels) {
  return { id, width, pixels: pixels || Array(pixelCount(width, height)).fill(false) };
}

function packGlyph(glyph, height) {
	const bytesPerRow = Math.ceil(glyph.width / 8);
  const bytes = Array(height * bytesPerRow).fill(0);
  for (let y = 0; y < height; y++) {
	for (let x = 0; x < glyph.width; x++) {
		if (glyph.pixels[y * glyph.width + x]) bytes[y * bytesPerRow + Math.floor(x / 8)] |= 0x80 >> (x % 8);
	}
  }
  return bytes;
}

function packedBytes(glyphs, height) {
  return glyphs.flatMap(glyph => packGlyph(glyph, height));
}

const symbolKeys = {
  SymbolExclamation: '!', Exclamation: '!', SymbolDoubleQuote: '"', DoubleQuote: '"',
  SymbolNumberSign: '#', NumberSign: '#', SymbolDollar: '$', Dollar: '$',
  SymbolAmpersand: '&', Ampersand: '&', SymbolApostrophe: "'", Apostrophe: "'",
  SymbolColon: ':', Colon: ':', SymbolSemicolon: ';', Semicolon: ';', SymbolAt: '@', At: '@',
  SymbolQuestion: '?', Question: '?', SymbolDecimalPoint: '.', SymbolPeriod: '.', Period: '.',
  SymbolComma: ',', Comma: ',', SymbolDecimalComma: ',', SymbolPercent: '%',
  SymbolParenthesisOpen: '(', SymbolParenthesisClose: ')', SymbolBracketsOpen: '[', SymbolBracketOpen: '[', BracketOpen: '[',
  SymbolBracketsClose: ']', SymbolBracketClose: ']', BracketClose: ']', SymbolEquals: '=', SymbolPlus: '+', SymbolMinus: '-',
  SymbolSmaller: '<', SymbolLessThan: '<', LessThan: '<', SymbolBigger: '>', SymbolGreaterThan: '>', GreaterThan: '>',
  SymbolForwardSlash: '/', ForwardSlash: '/', SymbolBackslash: '\\', Backslash: '\\', SymbolCaret: '^', Caret: '^',
  SymbolUnderscore: '_', Underscore: '_', SymbolGraveAccent: '`', GraveAccent: '`', SymbolTilde: '~', Tilde: '~',
  SymbolAsterisk: '*', Asterisk: '*', SymbolLeftBrace: '{', LeftBrace: '{', SymbolVerticalBar: '|', VerticalBar: '|',
  SymbolRightBrace: '}', RightBrace: '}'
};

function characterForGlyphId(id) {
  return keyForGlyphId(id);
}

function keyForGlyphId(id) {
  if (/^Letter[A-Z]$/.test(id) || /^Letter[a-z]$/.test(id)) return id.slice(-1);
  if (/^Number[0-9]$/.test(id)) return id.slice(-1);
  if (symbolKeys[id]) return symbolKeys[id];
  if (id === 'Space') return null;
  if (id.length === 1) return /[a-z]/.test(id) ? id.toUpperCase() : id;
  return null;
}

function glyphIdForCharacter(character) {
  if (character === ' ') return null;
  if (/[A-Z]/.test(character) || /[a-z]/.test(character)) return `Letter${character}`;
  if (/[0-9]/.test(character)) return `Number${character}`;
  const names = {
	'!': 'Exclamation', '"': 'DoubleQuote', '#': 'NumberSign', '$': 'Dollar', '%': 'Percent', '&': 'Ampersand', "'": 'Apostrophe',
	'(': 'ParenthesisOpen', ')': 'ParenthesisClose', '*': 'Asterisk', '+': 'Plus', ',': 'Comma', '-': 'Minus', '.': 'Period', '/': 'ForwardSlash',
	':': 'Colon', ';': 'Semicolon', '<': 'LessThan', '=': 'Equals', '>': 'GreaterThan', '?': 'Question', '@': 'At', '[': 'BracketOpen', '\\': 'Backslash',
	']': 'BracketClose', '^': 'Caret', '_': 'Underscore', '`': 'GraveAccent', '{': 'LeftBrace', '|': 'VerticalBar', '}': 'RightBrace', '~': 'Tilde'
  };
  return names[character] ? `Symbol${names[character]}` : `Glyph${character.charCodeAt(0)}`;
}

return { pixelCount, bytesPerGlyph, createGlyph, packGlyph, packedBytes, characterForGlyphId, keyForGlyphId, glyphIdForCharacter };
})();
