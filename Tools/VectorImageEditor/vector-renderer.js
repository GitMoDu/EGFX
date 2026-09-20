(() => {
  'use strict';

  function render(context, nodes, width, height, scale = 1, options = {}) {
    const pixels = document.createElement('canvas');
    pixels.width = width;
    pixels.height = height;
    const raster = pixels.getContext('2d');
    raster.imageSmoothingEnabled = false;
    raster.clearRect(0, 0, width, height);
    const thicknessFraction = options.fixedFontThickness ? 64 : options.thicknessFraction ?? 64;
    const palette = options.palette || [[255, 255, 255]];
    const rgba = value => Array.isArray(value) ? `rgb(${value[0]}, ${value[1]}, ${value[2]})` : value;
    const paletteColor = colorIndex => rgba(palette[((Number(colorIndex) || 0) % palette.length + palette.length) % palette.length] || palette[0]);
    raster.fillStyle = options.color || paletteColor(0);
    const set = (x, y) => { if (x >= 0 && y >= 0 && x < width && y < height) raster.fillRect(x, y, 1, 1); };
    const line = (a, b, drawPoint) => { let x = Math.round(a.x); let y = Math.round(a.y); const targetX = Math.round(b.x); const targetY = Math.round(b.y); const dx = Math.abs(targetX - x); const sx = x < targetX ? 1 : -1; const dy = -Math.abs(targetY - y); const sy = y < targetY ? 1 : -1; let error = dx + dy; while (true) { drawPoint(x, y); if (x === targetX && y === targetY) break; const twice = error * 2; if (twice >= dy) { error += dy; x += sx; } if (twice <= dx) { error += dx; y += sy; } } };
    const renderScale = Math.max(1, options.renderScale || 1);
    let streamWeight = 0;
    const scaledWeight = () => Math.floor(renderScale * (options.fixedFontWeight ? 1 : 1 + streamWeight) / 256);
    const thickness = () => 1 + Math.floor(thicknessFraction * scaledWeight() / 128);
    const circleStrokeRadii = radius => { const outer = Math.abs(radius); return { outer, inner: Math.max(0, outer - thickness() + 1) }; };
    const arcStrokeRadii = radius => { const value = Math.abs(radius); const widthValue = thickness(); return { outer: value + Math.floor(widthValue / 2), inner: Math.max(0, value - Math.floor((widthValue - 1) / 2)) }; };
    const drawPoint = (x, y) => { const radius = Math.floor((thickness() - 1) / 2); for (let py = y - radius; py <= y + radius; py++) for (let px = x - radius; px <= x + radius; px++) set(px, py); };
    const circleLine = (center, radius, include = () => true) => { const plot = (x, y) => { if (include(x, y)) set(center.x + x, center.y + y); }; if (radius === 0) { plot(0, 0); return; } if (radius === 1) { plot(0, -1); plot(1, 0); plot(0, 1); plot(-1, 0); return; } plot(radius, 0); plot(-radius, 0); plot(0, radius); plot(0, -radius); let x = radius; let y = 0; let error = 1 - x; while (y + 1 < x) { y++; if (error <= 0) error += 2 * y + 1; else { x--; error += 2 * (y - x) + 1; } if (y < x) { plot(x, y); plot(-x, y); plot(x, -y); plot(-x, -y); } plot(y, x); plot(-y, x); plot(y, -x); plot(-y, -x); } };
    const arcLine = (center, radius, start, sweep) => { const steps = Math.max(1, Math.ceil(Math.abs(radius) * Math.PI * 2 * sweep / 256 * 2)); let previous = null; for (let step = 0; step <= steps; step++) { const angle = (start + sweep * step / steps) * Math.PI * 2 / 256; const point = { x: Math.round(center.x + Math.sin(angle) * radius), y: Math.round(center.y - Math.cos(angle) * radius) }; if (previous) line(previous, point, drawPoint); else drawPoint(point.x, point.y); previous = point; } };
    const rectangle = (a, b, fill) => { const left = Math.min(a.x, b.x); const right = Math.max(a.x, b.x); const top = Math.min(a.y, b.y); const bottom = Math.max(a.y, b.y); if (fill) for (let y = top; y <= bottom; y++) for (let x = left; x <= right; x++) set(x, y); else { line({ x: left, y: top }, { x: right, y: top }, drawPoint); line({ x: right, y: top }, { x: right, y: bottom }, drawPoint); line({ x: right, y: bottom }, { x: left, y: bottom }, drawPoint); line({ x: left, y: bottom }, { x: left, y: top }, drawPoint); } };
    const triangle = (a, b, c, fill) => {
      if (!fill) { line(a, b, drawPoint); line(b, c, drawPoint); line(c, a, drawPoint); return; }
      const vertices = [{ x: a.x, y: a.y }, { x: b.x, y: b.y }, { x: c.x, y: c.y }];
      vertices.sort((left, right) => left.y - right.y);
      const top = vertices[0];
      const middle = vertices[1];
      const bottom = vertices[2];
      if (top.y === bottom.y) {
        line({ x: Math.min(top.x, middle.x, bottom.x), y: top.y },
          { x: Math.max(top.x, middle.x, bottom.x), y: top.y }, drawPoint);
        return;
      }

      const fixedScale = 256;
      const fixedRound = value => Math.floor((value + 128) / fixedScale);
      const fixedSlope = (deltaX, deltaY) => deltaY === 0 ? 0 : Math.trunc(deltaX * fixedScale / deltaY);
      const longStep = fixedSlope(bottom.x - top.x, bottom.y - top.y);
      const topStep = fixedSlope(middle.x - top.x, middle.y - top.y);
      const bottomStep = fixedSlope(bottom.x - middle.x, bottom.y - middle.y);
      const topHeight = middle.y - top.y;
      const bottomHeight = bottom.y - middle.y;
      const longAtMiddle = top.x * fixedScale + longStep * topHeight;
      const longOnLeft = longAtMiddle <= middle.x * fixedScale;
      const fillSegment = (startY, endY, leftX, rightX, leftStep, rightStep) => {
        for (let y = startY; y < endY; y++) {
          const startX = fixedRound(leftX);
          const endX = fixedRound(rightX) - 1;
          if (startX <= endX) for (let x = startX; x <= endX; x++) set(x, y);
          leftX += leftStep;
          rightX += rightStep;
        }
      };

      if (topHeight > 0) {
        const leftStep = longOnLeft ? longStep : topStep;
        const rightStep = longOnLeft ? topStep : longStep;
        fillSegment(top.y, middle.y, top.x * fixedScale, top.x * fixedScale, leftStep, rightStep);
      }
      if (bottomHeight > 0) {
        const leftStep = longOnLeft ? longStep : bottomStep;
        const rightStep = longOnLeft ? bottomStep : longStep;
        const leftX = (longOnLeft ? longAtMiddle : middle.x * fixedScale);
        const rightX = (longOnLeft ? middle.x * fixedScale : longAtMiddle);
        fillSegment(middle.y, bottom.y, leftX, rightX, leftStep, rightStep);
      }
    };
    const circle = (center, edge, fill) => { const radius = Math.abs(edge.x - center.x); if (fill) { let x = radius; let y = 0; let error = 1 - x; const span = (py, left, right) => { for (let px = left; px <= right; px++) set(px, py); }; while (x >= y) { span(center.y + y, center.x - x, center.x + x); span(center.y - y, center.x - x, center.x + x); span(center.y + x, center.x - y, center.x + y); span(center.y - x, center.x - y, center.x + y); y++; if (error < 0) error += 2 * y + 1; else { x--; error += 2 * (y - x) + 1; } } } else if (thickness() <= 1) circleLine(center, radius); else { const radii = circleStrokeRadii(radius); const outerSquared = radii.outer * radii.outer; const innerSquared = radii.inner * radii.inner; for (let y = -radii.outer; y <= radii.outer; y++) for (let x = -radii.outer; x <= radii.outer; x++) { const distanceSquared = x * x + y * y; if (distanceSquared <= outerSquared && distanceSquared >= innerSquared) set(center.x + x, center.y + y); } } };
    const ring = (center, radii) => { const outerRadius = Math.abs(radii.x); const innerRadius = Math.abs(radii.y); if (outerRadius <= 0 || innerRadius > outerRadius) return; const outerSquared = outerRadius * outerRadius; const innerSquared = innerRadius * innerRadius; for (let y = -outerRadius; y <= outerRadius; y++) for (let x = -outerRadius; x <= outerRadius; x++) { const distanceSquared = x * x + y * y; if (distanceSquared <= outerSquared && distanceSquared >= innerSquared) set(center.x + x, center.y + y); } };
    const arc = (center, outerRadius, innerRadius, startAngle, endAngle, fill) => { const angleMax = Math.max(1, options.angleMax ?? Math.max(width, height) - 1); const angleRange = 256; const start = ((startAngle * angleRange / angleMax) % angleRange + angleRange) % angleRange; const rawSweep = (endAngle - startAngle) * angleRange / angleMax; const sweep = ((rawSweep % angleRange) + angleRange) % angleRange; if (!fill) { if (Math.abs(rawSweep) >= angleRange) circleLine(center, Math.abs(outerRadius)); else if (sweep) arcLine(center, Math.abs(outerRadius), start, sweep); return; } if (!sweep) return; const radii = { outer: Math.abs(outerRadius), inner: Math.max(0, Math.abs(innerRadius)) }; if (radii.outer <= 0 || radii.inner > radii.outer) return; const outerSquared = radii.outer * radii.outer; const innerSquared = radii.inner * radii.inner; for (let y = -radii.outer; y <= radii.outer; y++) for (let x = -radii.outer; x <= radii.outer; x++) { const distanceSquared = x * x + y * y; if (distanceSquared < innerSquared || distanceSquared > outerSquared) continue; const angle = ((Math.atan2(x, -y) / (Math.PI * 2)) * angleRange + angleRange) % angleRange; const relative = (angle - start + angleRange) % angleRange; if (relative <= sweep) set(center.x + x, center.y + y); } };
    const quadrant = (center, radiusPoint, value, fill) => { const radius = Math.abs(radiusPoint.x); const inside = (x, y) => value === 0 ? x >= center.x && y <= center.y : value === 1 ? x >= center.x && y >= center.y : value === 2 ? x <= center.x && y >= center.y : x <= center.x && y <= center.y; if (!fill && thickness() <= 1) { circleLine(center, radius, (x, y) => inside(center.x + x, center.y + y)); return; } const radii = fill ? { outer: radius, inner: 0 } : arcStrokeRadii(radius); const outerSquared = radii.outer * radii.outer; const innerSquared = radii.inner * radii.inner; for (let y = -radii.outer; y <= radii.outer; y++) for (let x = -radii.outer; x <= radii.outer; x++) { const pointX = center.x + x; const pointY = center.y + y; const distanceSquared = x * x + y * y; if (distanceSquared >= innerSquared && distanceSquared <= outerSquared && inside(pointX, pointY)) set(pointX, pointY); } };
    let previous = null;
    nodes.forEach(node => { const name = node.primitive; if (name === 'SetWeight') { if (!options.fixedFontWeight) streamWeight = node.value; return; } if (name === 'SetColor') { raster.fillStyle = paletteColor(node.value); return; } if (name === 'Break') { previous = null; return; } if (name === 'Point') { const current = { x: node.x, y: node.y }; if (previous) line(previous, current, drawPoint); else drawPoint(current.x, current.y); previous = current; return; } const center = { x: node.x, y: node.y }; const second = { x: node.x2 || 0, y: node.y2 || 0 }; const fill = name.endsWith('Fill'); if (name === 'Rectangle' || name === 'RectangleFill') rectangle(center, second, fill); else if (name === 'Triangle' || name === 'TriangleFill') triangle(center, second, { x: node.x3 || 0, y: node.y3 || 0 }, fill); else if (name === 'Circle' || name === 'CircleFill') circle(center, second, fill); else if (name === 'Ring' || name === 'RingFill') ring(center, second); else if (name === 'Arc' || name === 'ArcFill') arc(center, node.x2 || 0, fill ? node.y2 || 0 : 0, node.x3 || 0, node.y3 || 0, fill); else if (name === 'CircleQuadrant' || name === 'CircleQuadrantFill') quadrant(center, { x: node.x2 || 0 }, (node.y2 || 0) & 3, fill); previous = null; });
    context.imageSmoothingEnabled = false;
    context.drawImage(pixels, 0, 0, width * scale, height * scale);
  }

  window.IntegerGlassVectorRenderer = { render };
})();
