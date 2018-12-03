// This is revised from RedBlobGames, with influence from the style of glMatrix (http://glmatrix.net/)
// Original copyright: Generated code -- CC0 -- No Rights Reserved -- http://www.redblobgames.com/grids/hexagons/

import * as point from './point.mjs';

function createOrientation(f0, f1, f2, f3, b0, b1, b2, b3, startAngle) {
    return {
        f0: f0,
        f1: f1,
        f2: f2,
        f3: f3,
        b0: b0,
        b1: b1,
        b2: b2,
        b3: b3,
        startAngle: startAngle,
    };
}

export const Pointy = createOrientation(
    Math.sqrt(3),
    Math.sqrt(3) / 2,
    0,
    3 / 2,
    Math.sqrt(3) / 3,
    -1 / 3,
    0,
    2 / 3,
    0.5,
);

export const Flat = createOrientation(
    3 / 2,
    0,
    Math.sqrt(3) / 2,
    Math.sqrt(3),
    2 / 3,
    0,
    -1 / 3,
    Math.sqrt(3) / 3,
    0,
);

export function create(orientation, size, origin) {
    return {
        orientation: orientation,
        size: size,
        origin: origin,
    };
}

export function hexToPixel(out, layout, h) {
    let M = layout.orientation;
    let size = layout.size;
    let origin = layout.origin;

    out.x = (M.f0 * h.q + M.f1 * h.r) * size.x + origin.x;
    out.y = (M.f2 * h.q + M.f3 * h.r) * size.y + origin.y;
    return out;
}

export function pixelToHex(out, layout, p) {
    let M = layout.orientation;
    let size = layout.size;
    let origin = layout.origin;
    let x = (p.x - origin.x) / size.x;
    let y = (p.y - origin.y) / size.y;
    let q = M.b0 * x + M.b1 * y;
    let r = M.b2 * x + M.b3 * y;

    out.q = q;
    out.r = r;
    out.s = -q -r;
    return out;
}

export function hexCornerOffset(out, layout, corner) {
    let M = layout.orientation;
    let size = layout.size;
    let angle = 2.0 * Math.PI * (M.startAngle - corner) / 6.0;

    out.x = size.x * Math.cos(angle);
    out.y = size.y * Math.sin(angle);
    return out;
}

let tempPoint1 = point.create(0, 0);
let tempPoint2 = point.create(0, 0);

export function hexCorners(out, layout, h) {
    let center = hexToPixel(tempPoint1, layout, h);
    for (let i = 0; i < 6; i++) {
        let offset = hexCornerOffset(tempPoint2, layout, i);
        let corner = out[i];

        corner.x = center.x + offset.x;
        corner.y = center.y + offset.y;
    }

    return out;
}
