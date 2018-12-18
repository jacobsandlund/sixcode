export function create() {
    return new Map();
}

// 27 is ~half the bits in a double floating point number,
// so it maximizes the range of hexes we can store
const rKeyMultiplier = 1 / Math.pow(2, 27);

export function get(grid, h) {
    return grid.get(h.q + h.r * rKeyMultiplier);
}

export function set(grid, h, data) {
    return grid.set(h.q + h.r * rKeyMultiplier, data);
}

export function remove(grid, h) {
    return grid.delete(h.q + h.r * rKeyMultiplier);
}

export function has(grid, h) {
    return grid.has(h.q + h.r * rKeyMultiplier);
}

export function clear(grid) {
    return grid.clear();
}
