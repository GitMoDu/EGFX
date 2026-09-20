(() => {
  'use strict';

  window.IntegerGlassVectorEditorScale = {
	scaleFactor(targetCount, sourceCount) { return targetCount <= 1 || sourceCount <= 1 ? 0 : Math.min(65535, Math.floor((targetCount - 1) * 256 / (sourceCount - 1))); },
	scaleCoordinate(coordinate, factor) { return Math.floor(coordinate * factor / 256); }
  };
})();
