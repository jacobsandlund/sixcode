'use strict';
global.Evaluate = {};
(function () {

var images = [];

Evaluate.evaluate = function (parentCell, columns, c, r) {
    var cells = getAt(columns, c);
    var cell = getAt(cells, r);
    var text = val(get(cell, Cell.text));
    if (!isNaN(+text) && text !== '') {
        return +text;
    }

    var args = get(cell, Cell.args);
    var lenArgs = len(args);
    var argResult = function (i) {
        var arg = getAt(args, i);
        var argC = c + val(get(arg, Cell.Arg.cDiff));
        var argR = r + val(get(arg, Cell.Arg.rDiff));
        return Evaluate.evaluate(parentCell, columns, argC, argR);
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
    case 'square':
        ctx.fillRect(-50, -50, 100, 100);
        break;
    case 'circle':
        ctx.beginPath();
        ctx.arc(0, 0, 50, 0, 2 * Math.PI);
        ctx.fill();
        break;
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
            ctx.drawImage(image, 0, 0);
        } else {
            image = new Image();
            image.src = src;
            image.addEventListener('load', Ui.draw);
            images.push(image);
        }
        break;
    case 'scale':
        var scaleBy = argResult(1);
        ctx.save();
        ctx.scale(scaleBy, scaleBy);
        argResult(0);
        ctx.restore();
        break;
    case 'scale x':
        var scaleBy = argResult(1);
        ctx.save();
        ctx.scale(scaleBy, 1);
        argResult(0);
        ctx.restore();
        break;
    case 'scale y':
        var scaleBy = argResult(1);
        ctx.save();
        ctx.scale(1, scaleBy);
        argResult(0);
        ctx.restore();
        break;
    case 'move x':
        var moveBy = argResult(1);
        ctx.save();
        ctx.translate(moveBy, 0);
        argResult(0);
        ctx.restore();
        break;
    case 'move y':
        var moveBy = argResult(1);
        ctx.save();
        ctx.translate(0, moveBy);
        argResult(0);
        ctx.restore();
        break;
    case 'combine':
        argResult(0);
        argResult(1);
        break;
    case 'rotate':
        var rotateBy = argResult(1);
        ctx.save();
        ctx.rotate(rotateBy * Math.PI / 180);
        argResult(0);
        ctx.restore();
        break;
    case 'color':
        var color = argResult(1);
        ctx.save();
        ctx.fillStyle = color;
        argResult(0);
        ctx.restore();
        break;

    case 'mouse x':
        var input = get(parentCell, Cell.input);
        var mouseXs = get(input, Input.mouseXs);
        if (c >= len(mouseXs)) {
            return 0;
        }
        return val(getAt(mouseXs, c));

    case 'mouse y':
        var input = get(parentCell, Cell.input);
        var mouseYs = get(input, Input.mouseYs);
        if (c >= len(mouseYs)) {
            return 0;
        }
        return val(getAt(mouseYs, c));
    }

    return text;
};

})();
