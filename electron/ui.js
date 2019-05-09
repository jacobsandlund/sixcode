const SCALE_LEVELS = [
    0.5,
    0.75,
    1.0,
    1.5,
    2.0,
    3.0,
    4.0,
    6.0,
    8.0,
    12.0,
    16.0,
    23.0,
    32.0,
    46.0,
    64.0,
    90.0,
    128.0,
    181.0,
    256.0,
    362.0,
    512.0,
    724.0,
    1024.0,
    1448.0,
    2048.0,
];

const CANVAS_BACKGROUND_COLOR = 'rgb(48,48,48)';

const GRADIENT_COLOR = [
    null,                                   // 0.5,
    null,                                   // 0.75,
    null,                                   // 1.0,
    null,                                   // 1.5,
    null,                                   // 2.0,
    null,                                   // 3.0,
    null,                                   // 4.0,
    null,                                   // 6.0,
    'rgb(60, 60, 60), rgb(36, 36, 36)',     // 8.0,
    'rgb(74, 74, 74), rgb(22, 22, 22)',     // 12.0,
    'rgb(88, 88, 88), rgb(8, 8, 8)',        // 16.0,
    'rgb(100, 100, 100), rgb(0, 0, 0)',     // 23.0,
    'rgb(110, 110, 110), rgb(0, 0, 0)',     // 32.0,
    'rgb(120, 120, 120), rgb(0, 0, 0)',     // 46.0,
    'rgb(130, 130, 130), rgb(0, 0, 0)',     // 64.0,
    'rgb(140, 140, 140), rgb(0, 0, 0)',     // 90.0,
    'rgb(142, 142, 142), rgb(0, 0, 0)',     // 128.0,
    'rgb(144, 144, 144), rgb(0, 0, 0)',     // 181.0,
    'rgb(158, 158, 158), rgb(0, 0, 0)',     // 256.0,
    'rgb(174, 174, 174), rgb(0, 0, 0)',     // 362.0,
    'rgb(194, 194, 194), rgb(0, 0, 0)',     // 512.0,
    'rgb(214, 214, 214), rgb(0, 0, 0)',     // 724.0,
    'rgb(234, 234, 234), rgb(0, 0, 0)',     // 1024.0,
    'rgb(254, 254, 254), rgb(0, 0, 0)',     // 1448.0,
    'rgb(254, 254, 254), rgb(0, 0, 0)',     // 2048.0,
];

const NO_GRADIENT_INDEX = GRADIENT_COLOR.lastIndexOf(null);

const VIEW_NUM_LAYOUTS = 2;

let canvas;
let grid;
let ui;
let view;

let scaleLevel = SCALE_LEVELS.indexOf(16.0);

function sixcode_initialized() {
    canvas = Module['canvas'];
    resizeUI();
    canvas.style.backgroundColor = CANVAS_BACKGROUND_COLOR;
    setGradient(scaleLevel, NO_GRADIENT_INDEX);

    grid = Module._web_grid_malloc();
    Module._web_core_grid_initialize(grid);

    // Big

    let count = 6000000;
    let size = 4096 - 2;

    for (let i = 0; i < count; ++i) {
        let y = Math.floor(Math.random() * size) - 4096/2 + 1;
        let x = Math.floor(Math.random() * size) - 4096/2 + 1;
        let style = Math.floor(Math.random() * 15) + 1;
        Module._web_grid_set(grid, x, y, style);
    }

    // Small

    //let count = 512;
    //let size = 64 - 2;

    //for (let i = 0; i < count; ++i) {
    //    let y = Math.floor(Math.random() * size) + 1;
    //    let x = Math.floor(Math.random() * size) + 1;
    //    let style = Math.floor(Math.random() * 15) + 1;
    //    Module._web_grid_set(grid, x, y, style);
    //}

    let scale = SCALE_LEVELS[scaleLevel];
    view = Module._web_view_malloc();
    Module._web_view_initialize(view, canvas.width, canvas.height, 0, 0, scale);

    ui = Module._web_ui_all_malloc();
    Module._web_ui_all_initialize(ui);
    Module._web_texture_update(ui, grid);

    window.addEventListener('resize', resize);
    canvas.addEventListener('wheel', wheel, {passive: true});
    canvas.addEventListener('mousedown', mouseDown);
    window.addEventListener('mousemove', mouseMove);
    window.addEventListener('mouseup', mouseUp);
    window.addEventListener('keydown', keyDown);
    window.oncontextmenu = function () {
        return false;
    };

    draw();
}

function draw() {
    let startTime = performance.now();

    Module._web_core_tick(ui, view, grid);

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

function setGradient(scaleLevel, oldScaleLevel) {
    let gradient = GRADIENT_COLOR[scaleLevel];
    let oldGradient = GRADIENT_COLOR[oldScaleLevel];

    if (gradient !== oldGradient) {
        if (gradient) {
            canvas.style['background-image'] = 'radial-gradient(' + gradient + ')';
        } else {
            canvas.style['background-image'] = null;
        }
    }
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
        setGradient(newScaleLevel, scaleLevel);
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
    if (draggingMouse) {
        Module._web_ui_blend_enabled(ui, true);
        setGradient(scaleLevel, NO_GRADIENT_INDEX);

        draw();
    } else {
        let startTime = performance.now();

        let x = e.clientX * window.devicePixelRatio;
        let y = e.clientY * window.devicePixelRatio;

        Module._web_core_toggle_hex_at_point(ui, view, grid, x, y);

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
            Module._web_ui_blend_enabled(ui, false);
            setGradient(NO_GRADIENT_INDEX, scaleLevel);
        }
    }

    if (draggingMouse) {
        lastMouseX = mouseX;
        lastMouseY = mouseY;

        Module._web_view_translate(view, deltaX, deltaY);

        draw();
    }
}

function keyDown(e) {
    if (e.code === 'Digit1') {
        let layout = Module._web_view_layout_get(view);
        layout = (layout + 1) % VIEW_NUM_LAYOUTS;
        Module._web_view_layout_set(view, layout);

        draw();
    }
}

if (isSixcodeInitialized) {
    sixcode_initialized();
}
