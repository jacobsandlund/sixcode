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
    var parentCell = $scope.cell;
    var input = get(parentCell, Cell.input);
    var mouseXs = get(input, Input.mouseXs);
    var mouseYs = get(input, Input.mouseYs);
    var lenMouseXs = len(mouseXs);
    var lenMouseYs = len(mouseYs);
    var i;

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

    var scaledX = Math.floor($mouseX / window.innerWidth * 1440) - 720;
    var scaledY = Math.floor($mouseY / window.innerHeight * 900) - 450;
    mouseXs = setAt(mouseXs, $playC, Constants.integer(scaledX));
    mouseYs = setAt(mouseYs, $playC, Constants.integer(scaledY));

    input = set(input,
                Input.mouseXs, mouseXs,
                Input.mouseYs, mouseYs);
    parentCell = set(parentCell, Cell.input, input);
    Scope.update($scope, parentCell);
};

})();
