'use strict';
global.Input = {};
(function () {

Input.mouseDowns = 0;
Input.mouseXs = 1;
Input.mouseYs = 2;

Input.zero = 0;

Input.initialize = function () {
    Input.zero = $.nextIndex++;
    $[Input.zero] = createZero({
        mouseDowns: ArrayTree.$zeros[0],
        mouseXs: ArrayTree.$zeros[0],
        mouseYs: ArrayTree.$zeros[0],
    });
};

Input.capture = function () {
    var parentCell = $scope.cell;
    var input = get(parentCell, Cell.input);
    var mouseDowns = get(input, Input.mouseDowns);
    var mouseXs = get(input, Input.mouseXs);
    var mouseYs = get(input, Input.mouseYs);
    var lenMouseDowns = len(mouseDowns);
    var lenMouseXs = len(mouseXs);
    var lenMouseYs = len(mouseYs);
    var i;

    if ($playC >= lenMouseDowns) {
        for (i = lenMouseDowns; i <= $playC; i++) {
            mouseDowns = push(mouseDowns, Constants.$positive[0]);
        }
    }

    if ($playC >= lenMouseXs) {
        for (i = lenMouseXs; i <= $playC; i++) {
            mouseXs = push(mouseXs, Constants.$positive[0]);
        }
    }

    if ($playC >= lenMouseYs) {
        for (i = lenMouseYs; i <= $playC; i++) {
            mouseYs = push(mouseYs, Constants.$positive[0]);
        }
    }

    mouseDowns = setAt(mouseDowns, $playC, Constants.$positive[+$mouseDown]);
    var scaledX = Math.floor($mouseX / window.innerWidth * 1440) - 720;
    var scaledY = Math.floor($mouseY / window.innerHeight * 900) - 450;
    mouseXs = setAt(mouseXs, $playC, Constants.integer(scaledX));
    mouseYs = setAt(mouseYs, $playC, Constants.integer(scaledY));

    input = set(input,
                Input.mouseDowns, mouseDowns,
                Input.mouseXs, mouseXs,
                Input.mouseYs, mouseYs);
    parentCell = set(parentCell, Cell.input, input);
    Scope.update($scope, parentCell);
};

})();
