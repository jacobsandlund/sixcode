import * as hex from './hex.mjs';
import * as point from './point.mjs';
import * as layout from './layout.mjs';

let canvas = document.getElementById('canvas');
let ctx = canvas.getContext('2d');

let l = layout.create(
    layout.Pointy,
    point.create(20, 20),
    point.create(0, 0),
);

function resize() {
    let newWidth = window.innerWidth * window.devicePixelRatio;
    let newHeight = window.innerHeight * window.devicePixelRatio;
    canvas.width = newWidth;
    canvas.height = newHeight;
    canvas.style.width = window.innerWidth;
    canvas.style.height = window.innerHeight;

    l.origin.x = Math.round(newWidth / 2);
    l.origin.y = Math.round(newHeight / 2);

    draw();
}

let hexes = [
    hex.create(3, 4, -7),
    hex.create(0, 0, 0),
    hex.create(1, 1, -2),
    hex.create(-2, 1, 1),
    hex.create(1, -2, 1),
];

window.addEventListener('resize', resize);

let corners = point.createArray(6);

function draw() {
    let startTime = performance.now();

    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.fillStyle = 'rgb(' + 244 + ',' + 244 + ',' + 255 + ')';
    ctx.strokeStyle = 'rgb(' + 190 + ',' + 190 + ',' + 190 + ')';

    for (let h of hexes) {
        layout.hexCorners(corners, l, h);

        let corner = corners[0];
        ctx.beginPath();
        ctx.moveTo(corner.x, corner.y);

        for (let i = 1; i < 6; i++) {
            corner = corners[i];
            ctx.lineTo(corner.x, corner.y);
        }

        ctx.closePath();
        ctx.stroke();
        ctx.fill();
    }

    let endTime = performance.now();
    console.log('draw in ' + (endTime - startTime) + ' ms');
}

let clickPoint = point.create(0, 0);
let clickHex = hex.create(0, 0, 0);

function click(e) {
    clickPoint.x = e.clientX * window.devicePixelRatio;
    clickPoint.y = e.clientY * window.devicePixelRatio;
    hex.round(clickHex, layout.pixelToHex(clickHex, l, clickPoint));

    let existingHexI = -1;

    for (let i = 0; i < hexes.length; i++) {
        if (hex.equal(hexes[i], clickHex)) {
            existingHexI = i;
        }
    }

    if (existingHexI > 0) {
        hexes.splice(existingHexI, 1);
    } else {
        hexes.push(hex.clone(clickHex));
    }

    draw();
}

canvas.addEventListener('click', click);

resize();
