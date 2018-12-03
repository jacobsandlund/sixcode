import { test } from './test.mjs';
import * as layout from './layout.mjs';
import * as hex from './hex.mjs';
import * as point from './point.mjs';

test('orientation', _ => {
    _(layout.Pointy);
    //=> { f0: 1.7320508075688772,
    //=>   f1: 0.8660254037844386,
    //=>   f2: 0,
    //=>   f3: 1.5,
    //=>   b0: 0.5773502691896257,
    //=>   b1: -0.3333333333333333,
    //=>   b2: 0,
    //=>   b3: 0.6666666666666666,
    //=>   startAngle: 0.5 }

    _(layout.Flat);
    //=> { f0: 1.5,
    //=>   f1: 0,
    //=>   f2: 0.8660254037844386,
    //=>   f3: 1.7320508075688772,
    //=>   b0: 0.6666666666666666,
    //=>   b1: 0,
    //=>   b2: -0.3333333333333333,
    //=>   b3: 0.5773502691896257,
    //=>   startAngle: 0 }
});

test('create', _ => {
    let size = point.create(15, 20);
    let origin = point.create(100, 250);
    let l = layout.create(layout.Pointy, size, origin);

    _(l.orientation.startAngle);
    //=> 0.5
    _(l.orientation.f0);
    //=> 1.7320508075688772
    _(l.size.x, l.size.y);
    //=> 15 20
    _(l.origin.x, l.origin.y);
    //=> 100 250
});

test('hexToPixel', _ => {
    let l = layout.create(layout.Pointy, point.create(10, 20), point.create(100, 200));
    let h = hex.create(3, 4, -7);
    let out = point.create(0, 0);

    _(layout.hexToPixel(out, l, h));
    //=> { x: 186.60254037844385, y: 320 }
    _(out.x, out.y);
    //=> 186.60254037844385 320

    _(Math.sqrt(3) * 3 + Math.sqrt(3) / 2 * 4);
    //=> 8.660254037844386
    _((Math.sqrt(3) * 3 + Math.sqrt(3) / 2 * 4) * 10 + 100);
    //=> 186.60254037844385
    _(0 * 3 + 3 / 2 * 4);
    //=> 6
    _((0 * 3 + 3 / 2 * 4) * 20 + 200);
    //=> 320
});

test('pixelToHex', _ => {
    let l = layout.create(layout.Pointy, point.create(10, 20), point.create(100, 200));
    let p = point.create(186.6, 320);
    let out = hex.create(0, 0, 0);

    _(layout.pixelToHex(out, l, p));
    //=> { q: 2.999853331182159, r: 4, s: -6.999853331182159 }
    _(out.q, out.r, out.s);
    //=> 2.999853331182159 4 -6.999853331182159

    _(hex.round(out, out));
    //=> { q: 3, r: 4, s: -7 }
});

test('hex to pixel to hex roundtrips', _ => {
    let pointy = layout.create(layout.Pointy, point.create(10, 15), point.create(35, 71));
    let flat = layout.create(layout.Flat, point.create(10, 15), point.create(35, 71));
    let h = hex.create(3, 4, -7);
    let p = point.create(0, 0);
    let out = hex.create(0, 0, 0);

    _(hex.equal(hex.round(out, layout.pixelToHex(out, pointy, layout.hexToPixel(p, pointy, h))), h));
    //=> true

    _(hex.equal(hex.round(out, layout.pixelToHex(out, flat, layout.hexToPixel(p, flat, h))), h));
    //=> true
});

test('hexCornerOffset', _ => {
    let l = layout.create(layout.Pointy, point.create(10, 20), point.create(100, 200));
    let out = point.create(0, 0);
    _(layout.hexCornerOffset(out, l, 0));
    //=> { x: 8.660254037844387, y: 9.999999999999998 }
    _(layout.hexCornerOffset(out, l, 1));
    //=> { x: 8.660254037844387, y: -9.999999999999998 }
    _(layout.hexCornerOffset(out, l, 2));
    //=> { x: 6.123233995736766e-16, y: -20 }
    _(layout.hexCornerOffset(out, l, 3));
    //=> { x: -8.660254037844387, y: -9.999999999999998 }
    _(layout.hexCornerOffset(out, l, 4));
    //=> { x: -8.660254037844389, y: 9.999999999999995 }
    _(layout.hexCornerOffset(out, l, 5));
    //=> { x: -1.8369701987210296e-15, y: 20 }
});

test('hexCorners', _ => {
    let l = layout.create(layout.Pointy, point.create(10, 20), point.create(100, 200));
    let h = hex.create(3, 4, -7);
    let out = point.createArray(6);

    out = layout.hexCorners(out, l, h);

    _(out);
    //=> [ { x: 195.26279441628824, y: 330 },
    //=>   { x: 195.26279441628824, y: 310 },
    //=>   { x: 186.60254037844385, y: 300 },
    //=>   { x: 177.94228634059945, y: 310 },
    //=>   { x: 177.94228634059945, y: 330 },
    //=>   { x: 186.60254037844385, y: 340 } ]
});
