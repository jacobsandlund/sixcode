'use strict';
global.Input = {};
(function () {

Input.mouseXs = 0;
Input.mouseYs = 1;

Input.zero = 0;

Input.initialize = function () {
    Input.zero = $.nextIndex++;
    $[Input.zero] = createZero({
        mouseXs: ArrayTree.$zeros[0],
        mouseYs: ArrayTree.$zeros[0],
    });
};

Input.capture = function () {
    var parentCell = get($project, Project.cell);
    var input = get(parentCell, Cell.input);
    var mouseXs = get(input, Input.mouseXs);
    var mouseYs = get(input, Input.mouseYs);
    var lenMouseXs = len(mouseXs);
    var lenMouseYs = len(mouseYs);
    var i;

    if ($playFrame >= lenMouseXs) {
        for (i = lenMouseXs; i <= $playFrame; i++) {
            mouseXs = push(mouseXs, Constants.$positive[0]);
        }
    }

    if ($playFrame >= lenMouseYs) {
        for (i = lenMouseYs; i <= $playFrame; i++) {
            mouseYs = push(mouseYs, Constants.$positive[0]);
        }
    }

    var scaledX = Math.floor($mouseX / window.innerWidth * 1440) - 720;
    var scaledY = Math.floor($mouseY / window.innerHeight * 900) - 450;
    mouseXs = setAt(mouseXs, $playFrame, hash(scaledX));
    mouseYs = setAt(mouseYs, $playFrame, hash(scaledY));

    input = set(input,
                Input.mouseXs, mouseXs,
                Input.mouseYs, mouseYs);
    parentCell = set(parentCell, Cell.input, input);
    $project = set($project, Project.cell, parentCell);
};

})();
