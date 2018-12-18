import { test } from './test.mjs';
import * as grid from './grid.mjs';
import * as hex from './hex.mjs';

test('create', _ => {
    let g = grid.create();
    _(g);
    //=> Map {}
});

test('set and get', _ => {
    let g = grid.create();
    let h = hex.create(1, 2, -3);

    _(grid.set(g, h, 42));
    //=> Map { 1.0000000149011612 => 42 }

    _(grid.get(g, h));
    //=> 42
    _(grid.get(g, hex.create(1, 4, -5)));
    //=> undefined
    _(grid.get(g, hex.create(0, 4, -4)));
    //=> undefined

    h = hex.create(1, 1, -2);
    _(grid.set(g, h, 'foo'));
    //=> Map { 1.0000000149011612 => 42, 1.0000000074505806 => 'foo' }
    _(grid.get(g, h));
    //=> foo
});

test('remove', _ => {
    let g = grid.create();
    let h = hex.create(1, 2, -3);

    grid.set(g, h, {a: 'b'});
    _(grid.get(g, h));
    //=> { a: 'b' }

    grid.remove(g, h);

    _(grid.get(g, h));
    //=> undefined
});

test('has', _ => {
    let g = grid.create();
    let h = hex.create(3, -2, -1);

    _(grid.has(g, h));
    //=> false

    grid.set(g, h, 'yup');

    _(grid.has(g, h));
    //=> true
});

test('clear', _ => {
    let g = grid.create();
    let h1 = hex.create(1, 2, -3);
    let h2 = hex.create(10, 20, -30);
    grid.set(g, h1, true);
    grid.set(g, h2, 'hi');

    _(grid.get(g, h1));
    //=> true
    _(grid.has(g, h2));
    //=> true

    _(grid.clear(g));
    //=> undefined

    _(grid.get(g, h1));
    //=> undefined
    _(grid.get(g, h2));
    //=> undefined
});
