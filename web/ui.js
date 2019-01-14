let canvas = document.getElementById('canvas');
let ctx = canvas.getContext('2d');

const SCALE_LEVELS = [
    8.0,
    16.0,   // * 2
    32.0,
    64.0,
    110.0,  // * (Average 2 and Math.SQRT2)
    181.0,
    256.0,  // * Math.SQRT2
    362.0,
    512.0,
    724.0,
    1024.0,
    1448.0,
    2048.0,
    2896.0,
    4096.0,
];

let layout;
let grid;
let mesh;

let scaleLevel = SCALE_LEVELS.indexOf(32.0);

function core_initialized() {
    resizeUi();

    let scale = SCALE_LEVELS[scaleLevel];
    layout = Module._js_layout_create(canvas.width, canvas.height, 0, 0, scale);
    grid = Module._js_grid_create();
    mesh = Module._js_mesh_create(60);

    draw();
}

function draw() {
    let startTime = performance.now();

    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.fillStyle = 'rgb(' + 244 + ',' + 244 + ',' + 255 + ')';
    ctx.strokeStyle = 'rgb(' + 190 + ',' + 190 + ',' + 190 + ')';

    let hex_count = Module._js_mesh_generate_hexes(mesh, layout, grid);
    let points = Module._js_mesh_points(mesh);
    let ptr = points / 8;

    for (let h = 0; h < hex_count; ++h) {
        ctx.beginPath();
        ctx.moveTo(Module.HEAPF64[ptr], Module.HEAPF64[ptr + 1]);
        for (let i = 1; i < 6; ++i) {
            ptr += 2;
            ctx.lineTo(Module.HEAPF64[ptr], Module.HEAPF64[ptr + 1]);
        }

        ptr += 2;

        ctx.closePath();
        ctx.stroke();
        ctx.fill();
    }

    let endTime = performance.now();
    console.log('draw in ' + (endTime - startTime) + ' ms');
}

function resizeUi() {
    let newWidth = window.innerWidth * window.devicePixelRatio;
    let newHeight = window.innerHeight * window.devicePixelRatio;
    canvas.width = newWidth;
    canvas.height = newHeight;
    canvas.style.width = window.innerWidth;
    canvas.style.height = window.innerHeight;
}

function resize() {
    resizeUi();

    Module._js_layout_resize_viewport(layout, canvas.width, canvas.height);

    draw();
}

const CHANGE_SCALE_TIMEOUT = 100;
const WHEEL_DELTA_THRESHOLD = 400;
const FIRST_TIME_WHEEL_DELTA_THRESHOLD = 8;

let wheelDeltaY = 0.0;
let changeScaleTimeout = null;

function wheel(e) {
    wheelDeltaY += e.deltaY;

    let absDelta = Math.abs(wheelDeltaY);
    let wheelSign = wheelDeltaY < 0 ? -1 : +1;

    let absLevelChange = Math.floor(absDelta / WHEEL_DELTA_THRESHOLD);
    let firstTime = !changeScaleTimeout;

    if (
        absLevelChange === 0 &&
        firstTime &&
        absDelta > FIRST_TIME_WHEEL_DELTA_THRESHOLD
    ) {
        absLevelChange = 1;
    }

    if (absLevelChange > 0) {
        wheelDeltaY = 0.0;
    }

    let levelChange = -wheelSign * absLevelChange;
    let newScaleLevel = scaleLevel + levelChange;

    if (newScaleLevel < 0) {
        newScaleLevel = 0;
    } else if (newScaleLevel >= SCALE_LEVELS.length) {
        newScaleLevel = SCALE_LEVELS.length - 1;
    }

    if (newScaleLevel !== scaleLevel) {
        scaleLevel = newScaleLevel;
        let scale = SCALE_LEVELS[newScaleLevel];
        let x = e.clientX * window.devicePixelRatio;
        let y = e.clientY * window.devicePixelRatio;

        Module._js_layout_zoom_at_point(layout, x, y, scale);

        draw();

        if (changeScaleTimeout) {
            clearTimeout(changeScaleTimeout);
        }

        changeScaleTimeout = setTimeout(() => {
            changeScaleTimeout = null;
            wheelDeltaY = 0.0;
        }, CHANGE_SCALE_TIMEOUT);
    }
}

let mouseDownTime = 0;
let isMouseDown = false;
let draggingMouse = false;
let lastMouseX = 0.0;
let lastMouseY = 0.0;

function mouseDown(e) {
    lastMouseX = e.clientX * window.devicePixelRatio;
    lastMouseY = e.clientY * window.devicePixelRatio;
    isMouseDown = true;
    mouseDownTime = performance.now();
}

function mouseUp(e) {
    if (!draggingMouse) {
        let x = e.clientX * window.devicePixelRatio;
        let y = e.clientY * window.devicePixelRatio;

        Module._js_core_toggle_hex_at_point(layout, grid, x, y);

        draw();
    }

    isMouseDown = false;
    draggingMouse = false;
}

function mouseMove(e) {
    let dpr = window.devicePixelRatio;
    let mouseX = e.clientX * dpr;
    let mouseY = e.clientY * dpr;
    let deltaX;
    let deltaY;

    if (isMouseDown) {
        deltaX = lastMouseX - mouseX;
        deltaY = lastMouseY - mouseY;

        if (
            performance.now() - mouseDownTime > 100 ||
            deltaX * deltaX + deltaY * deltaY >= 30 * dpr * dpr
        ) {
            draggingMouse = true;
        }
    }

    if (draggingMouse) {
        lastMouseX = mouseX;
        lastMouseY = mouseY;

        Module._js_layout_translate_by_delta(layout, deltaX, deltaY);
        draw();
    }
}

window.addEventListener('resize', resize);
canvas.addEventListener('wheel', wheel, {passive: true});
canvas.addEventListener('mousedown', mouseDown);
window.addEventListener('mousemove', mouseMove);
window.addEventListener('mouseup', mouseUp);
