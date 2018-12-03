import { test } from './test.mjs';
import * as hex from './hex.mjs';

test('create', _ => {
    let h = hex.create(3, 2, -5);
    _(h);
    //=> { q: 3, r: 2, s: -5 }

    hex.create(4, 5, 6);
        //=> Error: q + r + s must be 0
        //=>     at Module.create (file://.../hex.mjs:6:15)
});

test('equal', _ => {
    let h1 = hex.create(3, 2, -5);
    let h2 = hex.create(3, 2, -5);
    let h3 = hex.create(2, 3, -5);

    _(hex.equal(h1, h1));
    //=> true
    _(hex.equal(h1, h2));
    //=> true
    _(hex.equal(h1, h3));
    //=> false
});

test('clone', _ => {
    let h1 = hex.create(3, 2, -5);
    let h2 = hex.clone(h1);
    _(hex.equal(h1, h2));
    //=> true
    _(h1 === h2);
    //=> false
});

test('set', _ => {
    let h = hex.create(1, 2, -3);

    _(hex.set(h, 3, 4, -7));
    //=> { q: 3, r: 4, s: -7 }
    _(h.q, h.r, h.s);
    //=> 3 4 -7

    hex.set(h, 1, 2, 3);
        //=> Error: q + r + s must be 0
        //=>     at Module.set (file://.../hex.mjs:30:15)
});

test('add', _ => {
    let a = hex.create(1, -3, 2);
    let b = hex.create(3, -7, 4);
    let out = hex.create(0, 0, 0);
    let expected = hex.create(4, -10, 6);

    _(hex.equal(hex.add(out, a, b), expected));
    //=> true
    _(hex.equal(out, expected));
    //=> true
});

test('subtract', _ => {
    let a = hex.create(1, -3, 2);
    let b = hex.create(3, -7, 4);
    let out = hex.create(0, 0, 0);
    let expected = hex.create(-2, 4, -2);

    _(hex.equal(hex.subtract(out, a, b), expected));
    //=> true
    _(hex.equal(out, expected));
    //=> true
});

test('scale', _ => {
    let h = hex.create(1, -3, 2);
    let out = hex.create(0, 0, 0);
    let expected = hex.create(3, -9, 6);

    _(hex.equal(hex.scale(out, h, 3), expected));
    //=> true
    _(hex.equal(out, expected));
    //=> true
});

test('rotate left', _ => {
    let h = hex.create(1, -3, 2);
    let out = hex.create(0, 0, 0);
    let expected = hex.create(-2, -1, 3);

    _(hex.equal(hex.rotateLeft(out, h), expected));
    //=> true
    _(hex.equal(out, expected));
    //=> true
});

test('rotate right', _ => {
    let h = hex.create(1, -3, 2);
    let out = hex.create(0, 0, 0);
    let expected = hex.create(3, -2, -1);

    _(hex.equal(hex.rotateRight(out, h), expected));
    //=> true
    _(hex.equal(out, expected));
    //=> true
});

test('direction', _ => {
    _(hex.equal(hex.direction(2), hex.create(0, -1, 1)));
    //=> true
});

test('neighbor', _ => {
    let start = hex.create(1, -2, 1);
    let out = hex.create(0, 0, 0);
    let expected = hex.create(1, -3, 2);

    _(hex.equal(hex.neighbor(out, start, 2), expected));
    //=> true
    _(hex.equal(out, expected));
    //=> true
});

test('distance from origin', _ => {
    let h = hex.create(3, -7, 4);
    _(hex.distanceFromOrigin(h));
    //=> 7
});

test('distance', _ => {
    let a = hex.create(3, -7, 4);
    let b = hex.create(-1, -4, 5);
    let sub = hex.subtract(hex.create(0, 0, 0), a, b);
    _(hex.distanceFromOrigin(sub));
    //=> 4
    _(hex.distance(a, b));
    //=> 4
});

test('lerp', _ => {
    let a = hex.create(0, 0, 0);
    let b = hex.create(1, -1, 0);
    let c = hex.create(4, -7, 3);
    let out = hex.create(0, 0, 0);

    _(hex.lerp(out, a, b, 0));
    //=> { q: 0, r: 0, s: 0 }
    _(hex.lerp(out, a, b, 1));
    //=> { q: 1, r: -1, s: 0 }
    _(hex.lerp(out, a, b, 0.449));
    //=> { q: 0.449, r: -0.449, s: 0 }
    _(hex.lerp(out, a, b, 0.501));
    //=> { q: 0.501, r: -0.501, s: 0 }

    _(hex.lerp(out, b, c, 0.75));
    //=> { q: 3.25, r: -5.5, s: 2.25 }
});

test('round', _ => {
    let a = hex.create(0, 0, 0);
    let b = hex.create(1, -1, 0);
    let c = hex.create(0, -1, 1);
    let out = hex.create(0, 0, 0);

    _(hex.round(out, hex.lerp(out, a, hex.create(10, -20, 10), 0.51)));
    //=> { q: 5, r: -10, s: 5 }
    _(hex.equal(hex.round(out, hex.lerp(out, a, b, 0.499)), a));
    //=> true
    _(hex.equal(hex.round(out, hex.lerp(out, a, b, 0.501)), b));
    //=> true

    let closerToA = hex.create(
        a.q * 0.4 + b.q * 0.3 + c.q * 0.3,
        a.r * 0.4 + b.r * 0.3 + c.r * 0.3,
        a.s * 0.4 + b.s * 0.3 + c.s * 0.3,
    );
    let closerToB = hex.create(
        a.q * 0.3 + b.q * 0.4 + c.q * 0.3,
        a.r * 0.3 + b.r * 0.4 + c.r * 0.3,
        a.s * 0.3 + b.s * 0.4 + c.s * 0.3,
    );
    let closerToC = hex.create(
        a.q * 0.3 + b.q * 0.3 + c.q * 0.4,
        a.r * 0.3 + b.r * 0.3 + c.r * 0.4,
        a.s * 0.3 + b.s * 0.3 + c.s * 0.4,
    );

    _(hex.equal(hex.round(out, closerToA), a));
    //=> true
    _(hex.equal(hex.round(out, closerToB), b));
    //=> true
    _(hex.equal(hex.round(out, closerToC), c));
    //=> true
});
