export function create(x, y) {
    return {
        x: x,
        y: y,
    };
}

export function set(out, x, y) {
    out.x = x;
    out.y = y;
    return out;
}

export function createArray(size) {
    let points = new Array(size);
    for (let i = 0; i < size; i++) {
        points[i] = create(0, 0);
    }
    return points;
}
