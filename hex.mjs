// This is revised from RedBlobGames, with influence from the style of glMatrix (http://glmatrix.net/)
// Original copyright: Generated code -- CC0 -- No Rights Reserved -- http://www.redblobgames.com/grids/hexagons/

export function create(q, r, s) {
    if (Math.round(q + r + s) !== 0) {
        throw new Error('q + r + s must be 0');
    }

    return {
        q: q,
        r: r,
        s: s,
    };
}

export function equal(a, b) {
    return a.q === b.q && a.s === b.s && a.r === b.r;
}

export function clone(h) {
    let out = create(0, 0, 0);
    out.q = h.q;
    out.r = h.r;
    out.s = h.s;
    return out;
}

export function set(out, q, r, s) {
    if (Math.round(q + r + s) !== 0) {
        throw new Error('q + r + s must be 0');
    }

    out.q = q;
    out.r = r;
    out.s = s;
    return out;
}

export function add(out, a, b) {
	out.q = a.q + b.q;
    out.r = a.r + b.r;
    out.s = a.s + b.s;
    return out;
}

export function subtract(out, a, b) {
	out.q = a.q - b.q;
    out.r = a.r - b.r;
    out.s = a.s - b.s;
    return out;
}

export function scale(out, a, k) {
	out.q = a.q * k;
    out.r = a.r * k;
    out.s = a.s * k;
    return out;
}

export function rotateLeft(out, h) {
    out.q = -h.s;
    out.r = -h.q;
    out.s = -h.r;
    return out;
}

export function rotateRight(out, h) {
    out.q = -h.r;
    out.r = -h.s;
    out.s = -h.q;
    return out;
}

let directions = [
    create(1, 0, -1),
    create(1, -1, 0),
    create(0, -1, 1),
    create(-1, 0, 1),
    create(-1, 1, 0),
    create(0, 1, -1),
];

export function direction(direction) {
    return directions[direction];
}

export function neighbor(out, h, direction) {
    return add(out, h, directions[direction]);
}

export function distanceFromOrigin(h) {
    return (Math.abs(h.q) + Math.abs(h.r) + Math.abs(h.s)) / 2;
}

let tempOut = create(0, 0, 0);

export function distance(a, b) {
    return distanceFromOrigin(subtract(tempOut, a, b));
}

export function lerp(out, a, b, t) {
    out.q = a.q * (1.0 - t) + b.q * t;
    out.r = a.r * (1.0 - t) + b.r * t;
    out.s = a.s * (1.0 - t) + b.s * t;
    return out;
}

export function round(out, h) {
    let qi = Math.round(h.q);
    let ri = Math.round(h.r);
    let si = Math.round(h.s);
    let q_diff = Math.abs(qi - h.q);
    let r_diff = Math.abs(ri - h.r);
    let s_diff = Math.abs(si - h.s);

    if (q_diff > r_diff && q_diff > s_diff) {
        qi = -ri - si;
    } else if (r_diff > s_diff) {
        ri = -qi - si;
    } else {
        si = -qi - ri;
    }

    out.q = qi;
    out.r = ri;
    out.s = si;
    return out;
}
