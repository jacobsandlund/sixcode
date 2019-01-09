let canvas = document.getElementById('canvas');
let ctx = canvas.getContext('2d');

const SCALE = 20;

let layout;
let grid;
let mesh;

function core_initialized() {
    layout = Module._js_layout_create(SCALE, 0, 0);
    grid = Module._js_grid_create(1);
    mesh = Module._js_mesh_create(60);

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

    let point_count = Module._js_mesh_generate_hexes(mesh, layout, grid);
    console.log(point_count);
    let points = Module._js_mesh_points(mesh);

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

    let endTime = performance.now();
    console.log('draw in ' + (endTime - startTime) + ' ms');
}

function click(e) {
    let x = e.clientX * window.devicePixelRatio;
    let y = e.clientY * window.devicePixelRatio;

    Module._js_core_toggle_hex_at_point(layout, grid, x, y);

    draw();
}

canvas.addEventListener('click', click);
