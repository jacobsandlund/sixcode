'use strict';
global.Evaluate = {};
(function () {

var images = [];

Evaluate.stop = {};

Evaluate.evaluate = function (scope, c, r) {
    var columns = scope.columns;
    var lenColumns = len(columns);
    if (c === lenColumns) {
        if (lenColumns <= 1) {
            return Evaluate.stop;
        } else {
            columns = push(columns, getAt(columns, c - 1));
            scope.cell = set(scope.cell, Cell.columns, columns);
            Scope.update(scope, scope.cell);
            lenColumns++;
        }
    }
    if (c < 0 || c >= lenColumns) {
        return null;
    }
    var cells = getAt(columns, c);
    if (r < 0 || r >= len(cells)) {
        return null;
    }
    var cell = getAt(cells, r);
    var text = val(get(cell, Cell.text));
    if (!isNaN(+text) && text !== '') {
        return +text;
    }

    var args = get(cell, Cell.args);
    var lenArgs = len(args);
    var argResult = function (i) {
        var arg = getAt(args, i);
        var parentArgIndex = val(get(arg, Cell.Arg.parentArg));
        if (parentArgIndex >= 0) {
            var parentArgs = get(scope.cell, Cell.args);
            arg = getAt(parentArgs, parentArgIndex);
            var argC = scope.c + val(get(arg, Cell.Arg.cDiff));
            var argR = scope.r + val(get(arg, Cell.Arg.rDiff));
            return Evaluate.evaluate(scope.parent, argC, argR);
        } else {
            var argC = c + val(get(arg, Cell.Arg.cDiff));
            var argR = r + val(get(arg, Cell.Arg.rDiff));
            return Evaluate.evaluate(scope, argC, argR);
        }
    };

    switch (text) {
    case '+':
        return argResult(0) + argResult(1);
    case '-':
        return argResult(0) - argResult(1);
    case '*':
        return argResult(0) * argResult(1);
    case '/':
        return argResult(0) / argResult(1);
    case '^':
        return Math.pow(argResult(0), argResult(1));
    case '=':
        return argResult(0);

    case 'is <':
        return +(argResult(0) < argResult(1));
    case 'is <=':
        return +(argResult(0) <= argResult(1));
    case 'is =':
        return +(argResult(0) == argResult(1));
    case 'is >=':
        return +(argResult(0) >= argResult(1));
    case 'is >':
        return +(argResult(0) > argResult(1));
    case 'and':
        return +(argResult(0) && argResult(1));
    case 'or':
        return +(argResult(0) || argResult(1));
    case 'not':
        return +!argResult(0);
    case 'choose':
        var choose = +argResult(2);
        if (choose > 0) {
            return argResult(0);
        } else {
            return argResult(1);
        }

    case 'square':
        $ctx.fillRect(-50, -50, 100, 100);
        return null;
    case 'circle':
        $ctx.beginPath();
        $ctx.arc(0, 0, 50, 0, 2 * Math.PI);
        $ctx.fill();
        return null;
    case 'image':
        var src = argResult(0);
        var image;
        for (var i = 0; i < images.length; i++) {
            if (images[i].src === src) {
                image = images[i];
                break;
            }
        }
        if (image) {
            $ctx.drawImage(image, 0, 0);
        } else {
            image = new Image();
            image.src = src;
            image.addEventListener('load', Ui.draw);
            images.push(image);
        }
        return src;
    case 'scale':
        var scaleBy = argResult(1);
        $ctx.save();
        $ctx.scale(scaleBy, scaleBy);
        argResult(0);
        $ctx.restore();
        return null;
    case 'scale x':
        var scaleBy = argResult(1);
        $ctx.save();
        $ctx.scale(scaleBy, 1);
        argResult(0);
        $ctx.restore();
        return null;
    case 'scale y':
        var scaleBy = argResult(1);
        $ctx.save();
        $ctx.scale(1, scaleBy);
        argResult(0);
        $ctx.restore();
        return null;
    case 'move x':
        var moveBy = argResult(1);
        $ctx.save();
        $ctx.translate(moveBy, 0);
        argResult(0);
        $ctx.restore();
        return null;
    case 'move y':
        var moveBy = argResult(1);
        $ctx.save();
        $ctx.translate(0, moveBy);
        argResult(0);
        $ctx.restore();
        return null;
    case 'combine':
        argResult(0);
        argResult(1);
        return null;
    case 'rotate':
        var rotateBy = argResult(1);
        $ctx.save();
        $ctx.rotate(rotateBy * Math.PI / 180);
        argResult(0);
        $ctx.restore();
        return null;
    case 'color':
        var color = argResult(1);
        $ctx.save();
        $ctx.fillStyle = color;
        argResult(0);
        $ctx.restore();
        return null;
    case 'mouse down':
        var input = get(scope.cell, Cell.input);
        var mouseDowns = get(input, Input.mouseDowns);
        if (c >= len(mouseDowns)) {
            return 0;
        }
        return val(getAt(mouseDowns, c));

    case 'mouse x':
        var input = get(scope.cell, Cell.input);
        var mouseXs = get(input, Input.mouseXs);
        if (c >= len(mouseXs)) {
            return 0;
        }
        return val(getAt(mouseXs, c));

    case 'mouse y':
        var input = get(scope.cell, Cell.input);
        var mouseYs = get(input, Input.mouseYs);
        if (c >= len(mouseYs)) {
            return 0;
        }
        return val(getAt(mouseYs, c));

    case 'stop':
        return Evaluate.stop;

    default:
        var childColumns = get(cell, Cell.columns);
        if (len(childColumns) === 0) {
            return '';
        }
        var lastRow = len(getAt(childColumns, 0)) - 1;
        var childScope = Scope.goInto(scope, cell, c, r);
        c = 0;
        var result = null;
        var lastResult = null;
        while (result !== Evaluate.stop) {
            lastResult = result;
            result = Evaluate.evaluate(childScope, c, lastRow);
            c++;
            if (c >= 1000000) {
                throw new Error('Infinite loop');
            }
        }
        return lastResult;
    }

    return text;
};

})();
