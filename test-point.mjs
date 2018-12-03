import { test } from './test.mjs';
import * as point from './point.mjs';

test('create', _ => {
    let p = point.create(2, 5);
    _(p);
    //=> { x: 2, y: 5 }
});

test('set', _ => {
    let p = point.create(7, -3);
    _(point.set(p, 4, 9));
    //=> { x: 4, y: 9 }
    _(p.x, p.y);
    //=> 4 9
});

test('createArray', _ => {
    let points = point.createArray(6);
    _(points.length);
    //=> 6
    _(points[0]);
    //=> { x: 0, y: 0 }
    _(points[5]);
    //=> { x: 0, y: 0 }
});
