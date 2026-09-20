(function () {
  'use strict';

  const PRIMITIVES = [
      { name: 'Point' },
      { name: 'SetWeight', opcode: 0, value: 'Weight' },
      { name: 'SetColor', opcode: 1, value: 'Palette index' },
      { name: 'Break', opcode: 2 },
      { name: 'Rectangle', opcode: 3, operands: 2 },
      { name: 'RectangleFill', opcode: 4, operands: 2 },
      { name: 'Triangle', opcode: 5, operands: 3 },
      { name: 'TriangleFill', opcode: 6, operands: 3 },
      { name: 'Circle', opcode: 7, operands: 2 },
      { name: 'CircleFill', opcode: 8, operands: 2 },
      { name: 'Ring', opcode: 9, operands: 2 },
      { name: 'RingFill', opcode: 10, operands: 2 },
      { name: 'Arc', opcode: 11, operands: 3, arc: true },
      { name: 'ArcFill', opcode: 12, operands: 3, arc: true },
      { name: 'CircleQuadrant', opcode: 13, operands: 2, quadrant: true },
      { name: 'CircleQuadrantFill', opcode: 14, operands: 2, quadrant: true }
    ];
  const PRIMITIVE_BY_NAME = new Map(PRIMITIVES.map(primitive => [primitive.name, primitive]));
  const PRIMITIVE_BY_OPCODE = new Map(PRIMITIVES.filter(primitive => primitive.opcode !== undefined).map(primitive => [primitive.opcode, primitive]));
  const QUADRANTS = [
      { value: 0, name: 'Top right' },
      { value: 1, name: 'Bottom right' },
      { value: 2, name: 'Bottom left' },
      { value: 3, name: 'Top left' }
    ];
  const DISPLAY_PALETTE = [[255, 255, 255], [98, 198, 255], [255, 128, 127], [126, 235, 167], [255, 210, 112], [202, 155, 255]];
  const DEFAULT_PALETTE = DISPLAY_PALETTE.map(color => [...color]);
  const OUTPUT_MIN = 2;
  const OUTPUT_MAX = 512;
  const MODELS = {
      '8': { name: 'Image15x15', type: 'uint8_t', bytes: 1, axisMax: 14, axisMask: 15, native: 15 },
      '65': { name: 'Image255x255', type: 'uint16_t', bytes: 2, axisMax: 254, axisMask: 255, native: 255 }
    };

  window.IntegerGlassVectorEditorConfig = { PRIMITIVES, PRIMITIVE_BY_NAME, PRIMITIVE_BY_OPCODE, QUADRANTS, DISPLAY_PALETTE, DEFAULT_PALETTE, OUTPUT_MIN, OUTPUT_MAX, MODELS };
})();
