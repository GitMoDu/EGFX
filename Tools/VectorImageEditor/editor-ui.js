(() => {
  'use strict';

  function primitiveIcon(name) {
	return { Point: '•', Break: '↵', Rectangle: '▱', RectangleFill: '▰', Triangle: '△', TriangleFill: '▲', Circle: '○', CircleFill: '●', Ring: '◎', RingFill: '◉', Arc: '◡', ArcFill: '◓', CircleQuadrant: '◜', CircleQuadrantFill: '◕', SetWeight: 'W', SetColor: 'C' }[name] || '·';
  }

  function createActionButton(text, label, action) {
	const button = document.createElement('button');
	button.type = 'button';
	button.className = 'icon-button';
	button.textContent = text;
	button.title = label;
	button.addEventListener('click', event => { event.stopPropagation(); action(); });
	return button;
  }

  function createToolPalette(container, primitives, activeTool, onSelect) {
	container.replaceChildren();
	primitives.forEach(primitive => {
	  const button = document.createElement('button');
	  button.type = 'button';
	  button.dataset.tool = primitive.name;
	  button.textContent = primitive.name;
	  button.classList.toggle('active', primitive.name === activeTool());
	  button.addEventListener('click', () => onSelect(primitive.name));
	  container.append(button);
	});
  }

  window.EgfxVectorEditorUi = { primitiveIcon, createActionButton, createToolPalette };
})();
