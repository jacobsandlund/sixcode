let canvas = document.getElementById('canvas');
let ctx = canvas.getContext('2d');

const SCALE = 20;

//let l = layout.create(
//    layout.Pointy,
//    point.create(20, 20),
//    point.create(0, 0),
//);

let l;
let g;
let m;

function core_initialized() {
    l = Module._js_layout_create(SCALE, 0, 0);
    g = Module._js_group_create(1);
    m = Module._js_mesh_create(60);

    resize();
}

function resize() {
    let newWidth = window.innerWidth * window.devicePixelRatio;
    let newHeight = window.innerHeight * window.devicePixelRatio;
    canvas.width = newWidth;
    canvas.height = newHeight;
    canvas.style.width = window.innerWidth;
    canvas.style.height = window.innerHeight;

    let origin_x = Math.round(newWidth / 2);
    let origin_y = Math.round(newHeight / 2);
    Module._js_layout_set_origin(origin_x, origin_y);

    draw();
}

window.addEventListener('resize', resize);

function draw() {
    let startTime = performance.now();

    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.fillStyle = 'rgb(' + 244 + ',' + 244 + ',' + 255 + ')';
    ctx.strokeStyle = 'rgb(' + 190 + ',' + 190 + ',' + 190 + ')';

    let point_count = Module._js_mesh_generate_hexes(m, l, g);
    console.log(point_count);
    let points = Module._js_mesh_points(m);

    for (let p = 0; p < point_count; p += 6) {
        ctx.beginPath();
        let ptr = (points + (p * 16)) / 8;
        ctx.moveTo(Module.HEAPF64[ptr], Module.HEAPF64[ptr + 1]);
        for (let i = 1; i < 6; ++i) {
            let ptr = (points + (p + i) * 16) / 8;
            ctx.lineTo(Module.HEAPF64[ptr], Module.HEAPF64[ptr + 1]);
        }

        ctx.closePath();
        ctx.stroke();
        ctx.fill();
    }

//    for (let h of hexes) {
//        layout.hexCorners(corners, l, h);
//
//        let corner = corners[0];
//        ctx.beginPath();
//        ctx.moveTo(corner.x, corner.y);
//
//        for (let i = 1; i < 6; i++) {
//            corner = corners[i];
//            ctx.lineTo(corner.x, corner.y);
//        }
//
//        ctx.closePath();
//        ctx.stroke();
//        ctx.fill();
//    }

    let endTime = performance.now();
    console.log('draw in ' + (endTime - startTime) + ' ms');
}

function click(e) {
    let x = e.clientX * window.devicePixelRatio;
    let y = e.clientY * window.devicePixelRatio;

    Module._js_core_toggle_hex_at_point(l, g, x, y);

    draw();
}

canvas.addEventListener('click', click);
