let canvas = document.getElementById('canvas');
let ctx = canvas.getContext('2d');

const SCALE_LEVELS = [
    0.3,
    0.5,
    0.75,
    1.0,
    1.5,
    2.0,
    3.0,
    4.0,
    6.0,
    8.0,
    11.0,
    16.0,
    23.0,
    32.0,
    45.0,
    64.0,
    91.0,
    128.0,
    181.0,
    256.0,
    362.0,
    512.0,
    724.0,
    1024.0,
    1448.0,
    2048.0,
    2896.0,
    4096.0,
];

const TINY_SCALE_LEVEL = SCALE_LEVELS.indexOf(0.5);
const SMALL_SCALE_LEVEL = SCALE_LEVELS.indexOf(2.0);

const FILL_STYLES = [
    'rgb(244,244,255)',
    'rgb(255,0,0)',
    'rgb(0,255,0)',
    'rgb(0,0,255)',
];

const STROKE_STYLES = [
    'rgb(190,190,190)',
    'rgb(190,190,190)',
    'rgb(190,190,190)',
    'rgb(190,190,190)',
];

const HEX_POINT_SCALE_FACTOR = Math.sqrt(3);
const HEX_TINY_POINT_STYLE = 'rgb(85,85,85)';

let layout;
let grid;
let mesh;

let scaleLevel = SCALE_LEVELS.indexOf(32.0);

function core_initialized() {
    window.addEventListener('resize', resize);
    canvas.addEventListener('wheel', wheel, {passive: true});
    canvas.addEventListener('mousedown', mouseDown);
    window.addEventListener('mousemove', mouseMove);
    window.addEventListener('mouseup', mouseUp);

    resizeUi();

    let scale = SCALE_LEVELS[scaleLevel];
    layout = Module._js_layout_create(canvas.width, canvas.height, 0, 0, scale);
    grid = Module._js_grid_create();
    mesh = Module._js_styled_mesh_create(60, FILL_STYLES.length);

    draw();
}

function draw() {
    let startTime = performance.now();

    ctx.clearRect(0, 0, canvas.width, canvas.height);

    if (scaleLevel <= TINY_SCALE_LEVEL) {
        ctx.fillStyle = HEX_TINY_POINT_STYLE;

        let hex_count = Module._js_mesh_generate_points_at_hexes(mesh, layout, grid);
        let points = Module._js_mesh_points(mesh) / 8;
        let scale = SCALE_LEVELS[scaleLevel] * HEX_POINT_SCALE_FACTOR;

        for (let h = 0; h < hex_count; ++h) {
            let ptr = points + h * 2;
            ctx.fillRect(Module.HEAPF64[ptr], Module.HEAPF64[ptr + 1], scale, scale);
        }
    } else if (scaleLevel <= SMALL_SCALE_LEVEL) {
        let hex_count = Module._js_styled_mesh_generate_points_at_hexes(mesh, layout, grid);
        let points = Module._js_mesh_points(mesh) / 8;
        let style_indices = Module._js_styled_mesh_hex_style_indices(mesh) / 4;
        let scale = SCALE_LEVELS[scaleLevel] * HEX_POINT_SCALE_FACTOR;

        for (let h = 0; h < hex_count; ++h) {
            let styleIndex = Module.HEAP32[style_indices + h];
            ctx.fillStyle = FILL_STYLES[styleIndex];
            let ptr = points + h * 2;
            ctx.fillRect(Module.HEAPF64[ptr], Module.HEAPF64[ptr + 1], scale, scale);
        }
    } else {
        let hex_count = Module._js_styled_mesh_generate_hexes(mesh, layout, grid);
        let points = Module._js_mesh_points(mesh) / 8;
        let style_indices = Module._js_styled_mesh_hex_style_indices(mesh) / 4;
        let ptr = points;

        for (let h = 0; h < hex_count; ++h) {
            let styleIndex = Module.HEAP32[style_indices + h];
            ctx.fillStyle = FILL_STYLES[styleIndex];
            ctx.strokeStyle = STROKE_STYLES[styleIndex];

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
const WHEEL_DELTA_THRESHOLD = 200;
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
    mouseDownTime = Date.now();
}

function mouseUp(e) {
    if (!draggingMouse) {
        let startTime = performance.now();

        let x = e.clientX * window.devicePixelRatio;
        let y = e.clientY * window.devicePixelRatio;

        Module._js_core_toggle_hex_at_point(layout, grid, x, y);

        let endTime = performance.now();
        console.log('click in ' + (endTime - startTime) + ' ms');

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
            Date.now() - mouseDownTime > 100 ||
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

if (isCoreInitialized) {
    core_initialized();
}
