const SCALE_LEVELS = [
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

let canvas;
let grid;
let areaGrid;
let ui;
let view;

let scaleLevel = SCALE_LEVELS.indexOf(32.0);

function core_initialized() {
    canvas = Module['canvas'];
    resizeUI();

    grid = Module._web_grid_malloc();
    areaGrid = Module._web_area_grid_malloc();
    Module._web_core_grids_initialize(grid, areaGrid);

    // Big

    let count = 6000000;
    let size = 4096 - 2;

    for (let i = 0; i < count; ++i) {
        let r = Math.floor(Math.random() * size) - 4096/2 + 1;
        let c = 2 * Math.floor(Math.random() * size) + (r & 1) - 4096 + 2;
        let style = Math.floor(Math.random() * 15) + 1;
        Module._web_grid_set(grid, c, r, style);
    }

    // Small

    //let count = 512;
    //let size = 64 - 2;

    //for (let i = 0; i < count; ++i) {
    //    let r = Math.floor(Math.random() * size) + 1;
    //    let c = 2 * Math.floor(Math.random() * size) + (r & 1) + 2;
    //    let style = Math.floor(Math.random() * 15) + 1;
    //    Module._web_grid_set(grid, c, r, style);
    //}

    let scale = SCALE_LEVELS[scaleLevel];
    view = Module._web_view_malloc();
    Module._web_view_initialize(view, canvas.width, canvas.height, 0, 0, scale);

    ui = Module._web_ui_all_malloc();
    Module._web_ui_all_initialize(ui);
    let uig = Module._web_ui_grid(ui);
    Module._web_ui_grid_update_styles(uig, grid);

    window.addEventListener('resize', resize);
    canvas.addEventListener('wheel', wheel, {passive: true});
    canvas.addEventListener('mousedown', mouseDown);
    window.addEventListener('mousemove', mouseMove);
    window.addEventListener('mouseup', mouseUp);

    draw();
}

function draw() {
    let startTime = performance.now();

    Module._web_ui_all_draw(ui, view, grid);

    let endTime = performance.now();
    console.log('draw in ' + (endTime - startTime) + ' ms');
}

function resizeUI() {
    let dpr = window.devicePixelRatio;
    let newWidth = window.innerWidth * dpr;
    let newHeight = window.innerHeight * dpr;
    canvas.width = newWidth;
    canvas.height = newHeight;
    canvas.style.width = window.innerWidth;
    canvas.style.height = window.innerHeight;
}

function resize() {
    resizeUI();

    Module._web_view_resize(view, canvas.width, canvas.height);

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
        let dpr = window.devicePixelRatio;
        let x = e.clientX * dpr;
        let y = e.clientY * dpr;

        Module._web_view_zoom_at_screen_point(view, x, y, scale);

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
    let dpr = window.devicePixelRatio;
    lastMouseX = e.clientX * dpr;
    lastMouseY = e.clientY * dpr;
    isMouseDown = true;
    mouseDownTime = Date.now();
}

function mouseUp(e) {
    if (!draggingMouse) {
        let startTime = performance.now();

        let x = e.clientX * window.devicePixelRatio;
        let y = e.clientY * window.devicePixelRatio;

        Module._web_core_toggle_hex_at_point(ui, view, grid, x, y);

        let endTime = performance.now();
        console.log('click in ' + (endTime - startTime) + ' ms');
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

        Module._web_view_translate(view, deltaX, deltaY);

        draw();
    }
}

if (isCoreInitialized) {
    core_initialized();
}
