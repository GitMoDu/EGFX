const AtlasEditorUi = (() => {
  function renderKeyboard(
    container,
    state,
    renderAll,
    addGlyphForCharacter,
    characterForGlyphId
  ) {
    container.replaceChildren();
    const glyphByCharacter = new Map();
    state.glyphs.forEach((glyph, index) => {
      const character = characterForGlyphId(glyph.id || "");
      if (character && !glyphByCharacter.has(character))
        glyphByCharacter.set(character, index);
    });

    const groups = [
      ["Space", [" "]],
      ["Uppercase", [..."ABCDEFGHIJKLMNOPQRSTUVWXYZ"]],
      ["Lowercase", [..."abcdefghijklmnopqrstuvwxyz"]],
      ["Numbers", [..."0123456789"]],
      [
        "Symbols",
        [..."!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"],
      ],
    ];

    groups.forEach(([label, characters]) => {
      const group = document.createElement("section");
      group.className = "keyboard-group";
      const heading = document.createElement("div");
      heading.className = "keyboard-group-label";
      heading.textContent = label;
      const row = document.createElement("div");
      row.className = "keyboard-row";

      characters.forEach((character) => {
        const glyphIndex = glyphByCharacter.get(character);
        const button = document.createElement("button");
        button.type = "button";
        button.className = "glyph-chip";
        if (character === " ") button.classList.add("space");
        button.textContent = character === " " ? "Space" : character;
        button.title =
          glyphIndex === undefined
            ? `Add ${character === " " ? "space" : character}`
            : `Edit ${character === " " ? "space" : character}`;
        if (glyphIndex === undefined) button.classList.add("missing");
        if (glyphIndex === state.selected) button.classList.add("selected");
        button.addEventListener("click", () => {
          if (glyphIndex === undefined) addGlyphForCharacter(character);
          else {
            state.selected = glyphIndex;
            renderAll();
          }
        });
        row.append(button);
      });

      group.append(heading, row);
      container.append(group);
    });
  }

  function syncMetadata(document, state, byteCount) {
    const width = document.getElementById("glyphWidth");
    const height = document.getElementById("glyphHeight");
    const spaceWidth = document.getElementById("spaceWidth");
    if (width) width.value = state.width;
    if (height) height.value = state.height;
    if (spaceWidth) spaceWidth.value = state.spaceWidth;

    const atlasSize = document.getElementById("atlasSize");
    if (atlasSize) atlasSize.textContent = `${state.width} x ${state.height} pixels`;
    const atlasInfo = document.getElementById("atlasInfo");
    if (atlasInfo)
      atlasInfo.textContent = `${state.glyphs.length} glyphs - ${byteCount} bytes`;
  }

  return { renderKeyboard, syncMetadata };
})();
