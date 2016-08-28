'use strict';
global.Ui = {};
(function () {

var canvas;
var playPreviewCanvas;
var autocompleteContainer;
var autocompleteInput;

global.$ctx = null;
var playPreviewCtx = null;

global.$mouseX = 0;
global.$mouseY = 0;
var mouseC = 0;
var mouseR = 0;
var mouseArg = -1;

var zoom = 1;

var xSpacing = 160;
var ySpacing = 112;
var argGap = 40;
Ui.ySpacing = ySpacing;

var xHalfGap = 5;
var yHalfGap = 2;
var xTranslation = 0;
var yTranslation = 0;
var mouseXAtDown = 0;
var mouseYAtDown = 0;

var mouseDown = false;
var movingGrid = false;
var movingArg = false;
var movingArgIndex = -1;
var selectingRange = false;

var setMouseCoords = function () {
    var x = Math.round(($mouseX - xTranslation) / zoom);
    var y = Math.round(($mouseY - yTranslation) / zoom);
    mouseC = Math.floor(x / xSpacing);
    if (mouseC === 0 && y < -argGap) {
        mouseR = 0;
        var inverseArg = Math.floor((y + argGap) / ySpacing);
        mouseArg = len(get($scope.cell, Cell.args)) + inverseArg;
        return;
    } else {
        mouseR = Math.floor(y / ySpacing);
        mouseArg = -1;
    }

    if ($showResults) {
        var lenColumns = $results.length;
        var lenCells = lenColumns > 0 ? $results[0].length : 0;
    } else {
        var columns = get($scope.cell, Cell.columns);
        var lenColumns = len(columns);
        if (lenColumns > 0) {
            var lenCells = len(getAt(columns, 0));
        } else {
            var lenCells = 0;
        }
    }

    if (mouseC >= lenColumns) {
        mouseC = lenColumns;
    }
    if (mouseC < 0) {
        mouseC = 0;
    }
    if (mouseR >= lenCells) {
        mouseR = lenCells;
    }
    if (mouseR < 0) {
        mouseR = 0;
    }
};

var selectRange = function () {
    $minC = Math.min(mouseC, $c);
    $maxC = Math.max(mouseC, $c);
    $minR = Math.min(mouseR, $r);
    $maxR = Math.max(mouseR, $r);
};

Ui.initialize = function () {
    canvas = document.getElementById('canvas');
    canvas.width = window.devicePixelRatio * window.innerWidth;
    canvas.height = window.devicePixelRatio * window.innerHeight;
    canvas.style.width = window.innerWidth + 'px';
    canvas.style.height = window.innerHeight + 'px';

    playPreviewCanvas = document.getElementById('play-preview-canvas');
    playPreviewCanvas.width = window.devicePixelRatio * 320;
    playPreviewCanvas.height = window.devicePixelRatio * 200;
    playPreviewCanvas.style.width = '320px';
    playPreviewCanvas.style.height = '200px';

    autocompleteContainer = document.getElementById('autocomplete-container');
    autocompleteInput = document.getElementById('autocomplete-input');

    xTranslation = Math.floor(window.innerWidth / 3 - xSpacing / 2);
    yTranslation = Math.floor(window.innerHeight / 4 - ySpacing / 2);

    $ctx = canvas.getContext('2d');
    $ctx.font = '12px monospace';

    playPreviewCtx = playPreviewCanvas.getContext('2d');
    playPreviewCtx.font = '12px monospace';

    document.addEventListener('keydown', function (e) {
        if (e.keyCode === 27) { // escape
            Main.stopPlaying();
        }
    });

    canvas.addEventListener('click', function (e) {
        if ($fullscreen || movingGrid || movingArg || selectingRange) {
            return;
        }
        e.preventDefault();

        setMouseCoords();

        if (e.shiftKey) {
            if ($c === -1) {
                $c = mouseC;
                $r = mouseR;
                Autocomplete.setSelectedCell();
            }
            selectRange();
        } else {
            $c = mouseC;
            $r = mouseR;
            selectRange();

            Autocomplete.setSelectedCell();
        }

        Ui.draw();
    });

    canvas.addEventListener('dblclick', function (e) {
        Autocomplete.performMatch('go into');
    });

    canvas.addEventListener('mousedown', function (e) {
        e.preventDefault();
        $mouseX = e.clientX;
        $mouseY = e.clientY;
        mouseXAtDown = $mouseX;
        mouseYAtDown = $mouseY;
        mouseDown = true;

        setMouseCoords();

        if (!$showResults) {
            var columns = get($scope.cell, Cell.columns);
            var lenColumns = len(columns);
            if (lenColumns > 0) {
                var lenCells = len(getAt(columns, 0));
            } else {
                var lenCells = 0;
            }

            var selectedCell = null;
            if ($c >= 0 && $c < lenColumns) {
                var selectedColumn = getAt(columns, $c);
                if ($r >= 0 && $r < len(selectedColumn)) {
                    selectedCell = getAt(selectedColumn, $r);
                }
            }

            if (!selectedCell) {
                return;
            }

            var args = get(selectedCell, Cell.args);
            var lenArgs = len(args);
            var i;
            for (i = 0; i < lenArgs; i++) {
                var arg = getAt(args, i);
                var parentArg = val(get(arg, Cell.Arg.parentArg));
                if (parentArg >= 0) {
                    if (mouseArg === parentArg) {
                        movingArgIndex = i;
                        Ui.draw();
                        break;
                    }
                } else {
                    var argC = $c + val(get(arg, Cell.Arg.cDiff));
                    var argR = $r + val(get(arg, Cell.Arg.rDiff));

                    if (mouseC === argC && mouseR === argR) {
                        movingArgIndex = i;
                        Ui.draw();
                        break;
                    }
                }
            }
        }
    });

    canvas.addEventListener('mouseup', function (e) {
        mouseDown = false;
        var oldProject = get($head, Commit.tree);

        if ($project !== oldProject) {
            var now = Math.floor(+Date.now() / 1000);
            $head = createCommit($head,
                                 Commit.tree, $project,
                                 Commit.parent, $head,
                                 Commit.committerTime, now);
            $redoHead = $head;
        }

        movingArgIndex = -1;

        setTimeout(function () {
            movingGrid = false;
            movingArg = false;
            selectingRange = false;
            Ui.draw();
        });
    });

    canvas.addEventListener('mousemove', function (e) {
        if (!movingGrid && !movingArg && !selectingRange) {
            var moved = (
                Math.abs(e.clientX - mouseXAtDown) > 2 ||
                Math.abs(e.clientY - mouseYAtDown) > 2
            );
            if (mouseDown && moved) {
                if (e.shiftKey) {
                    selectingRange = true;
                    $c = -1;
                    $r = -1;
                } else if (movingArgIndex >= 0) {
                    movingArg = true;
                } else {
                    movingGrid = true;
                }
            }
        }
        if (movingGrid) {
            var xDiff = e.clientX - $mouseX;
            var yDiff = e.clientY - $mouseY;
            xTranslation += xDiff;
            yTranslation += yDiff;
            Ui.draw();
        }
        $mouseX = e.clientX;
        $mouseY = e.clientY;

        if (selectingRange) {
            setMouseCoords();

            if ($c === -1) {
                $c = mouseC;
                $r = mouseR;
                Autocomplete.setSelectedCell();
            }
            selectRange();
            Ui.draw();

        } else if (movingArg) {
            setMouseCoords();

            var parentCell = $scope.cell;
            var columns = get(parentCell, Cell.columns);
            var lenColumns = len(columns);
            if (lenColumns > 0) {
                var lenCells = len(getAt(columns, 0));
            } else {
                var lenCells = 0;
            }

            var selectedCell = null;
            var selectedColumn = null;
            if ($c >= 0 && $c < lenColumns) {
                selectedColumn = getAt(columns, $c);
                if ($r >= 0 && $r < len(selectedColumn)) {
                    selectedCell = getAt(selectedColumn, $r);
                }
            }

            if (!selectedCell) {
                return;
            }

            var args = get(selectedCell, Cell.args);
            var lenArgs = len(args);
            if (lenArgs === 0) {
                return;
            }

            if (mouseArg === -1 && (mouseC > $c || (mouseC === $c && mouseR >= $r))) {
                return;
            }

            var i;
            for (i = 0; i < lenArgs; i++) {
                var arg = getAt(args, i);
                var parentArg = val(get(arg, Cell.Arg.parentArg));
                if (mouseArg >= 0) {
                    if (mouseArg === parentArg) {
                        return;
                    }
                } else {
                    var argC = $c + val(get(arg, Cell.Arg.cDiff));
                    var argR = $r + val(get(arg, Cell.Arg.rDiff));
                    if (parentArg === -1 && mouseC === argC && mouseR === argR) {
                        return;
                    }
                }
            }

            if (mouseArg >= 0) {
                var parentArg = Constants.$positive[mouseArg];
                var cDiff = Constants.$positive[0];
                var rDiff = Constants.$positive[0];
            } else {
                var parentArg = Constants.$negative[1];
                var cDiff = Constants.integer(mouseC - $c);
                var rDiff = Constants.integer(mouseR - $r);
            }
            var arg = set($[Cell.Arg.zero],
                          Cell.Arg.parentArg, parentArg,
                          Cell.Arg.cDiff, cDiff,
                          Cell.Arg.rDiff, rDiff);

            var c;
            for (c = $minC; c <= $maxC; c++) {
                var column = getAt(columns, c);
                var r;
                for (r = $minR; r <= $maxR; r++) {
                    var cell = getAt(column, r);
                    var args = get(cell, Cell.args);
                    if (movingArgIndex < len(args)) {
                        args = setAt(args, movingArgIndex, arg);
                        cell = set(cell, Cell.args, args);
                        column = setAt(column, r, cell);
                    }
                }
                columns = setAt(columns, c, column);
            }

            parentCell = set(parentCell, Cell.columns, columns);
            Scope.update($scope, parentCell);
            Ui.draw();
        }
    });

    window.addEventListener('wheel', function (e) {
        var x = Math.round((e.clientX - xTranslation) / zoom);
        var y = Math.round((e.clientY - yTranslation) / zoom);

        var absDelta = Math.abs(e.deltaY);
        var sign = e.deltaY < 0 ? +1 : -1;
        var zoomFactor = 1.0 + (sign * Math.sqrt(absDelta) / 100.0);
        zoom *= zoomFactor;

        xTranslation = e.clientX - x * zoom;
        yTranslation = e.clientY - y * zoom;
        Ui.draw();
    });
};

Ui.draw = function () {
    console.time('UI.draw');

    Ui.moveAutocomplete();

    $ctx.save();
    $ctx.clearRect(0, 0, canvas.width, canvas.height);
    $ctx.scale(window.devicePixelRatio, window.devicePixelRatio);

    if ($fullscreen) {
        var ret = drawFullscreen();
    } else {
        var ret = drawGrid();
    }

    $ctx.restore();

    console.timeEnd('UI.draw');

    return ret;
};

Ui.drawPlayPreview = function () {
    console.time('UI.drawPlayPreview');

    var mainCtx = $ctx;
    $ctx = playPreviewCtx;

    $ctx.save();
    $ctx.clearRect(0, 0, playPreviewCanvas.width, playPreviewCanvas.height);
    $ctx.scale(window.devicePixelRatio, window.devicePixelRatio);


    var width = playPreviewCanvas.width / window.devicePixelRatio;
    var height = playPreviewCanvas.height / window.devicePixelRatio;
    var centerX = Math.floor(width / 2);
    var centerY = Math.floor(height / 2);
    $ctx.translate(centerX, centerY);
    $ctx.scale(width / 1440, height / 900);
    $ctx.fillStyle = '#492e85';
    $ctx.textAlign = 'center';
    $ctx.font = '180px monospace';

    var result = Evaluate.evaluate($playScope, $playColumns, $playC, $playR);
    if (typeof result === 'number') {
        $ctx.fillText('' + result, 0, 50, 1440);
    }

    $ctx.restore();

    $ctx = mainCtx;

    console.timeEnd('UI.drawPlayPreview');

    return result;
};

var drawFullscreen = function () {
    var centerX = Math.floor(window.innerWidth / 2);
    var centerY = Math.floor(window.innerHeight / 2);
    $ctx.translate(centerX, centerY);
    $ctx.scale(window.innerWidth / 1440, window.innerHeight / 900);

    if ($playScope === null) {
        var scope = $scope;
        var columns = get(scope.cell, Cell.columns);
        var lenColumns = len(columns);
        if (lenColumns > 0) {
            var lenCells = len(getAt(columns, 0));
        } else {
            var lenCells = 0;
        }
        var c = lenColumns - 1;
        var r = lenCells - 1;
        var evaluate = c >= 0 && c < lenColumns;
    } else {
        var scope = $playScope;
        var columns = $playColumns;
        var c = $playC;
        var r = $playR;
        var evaluate = true;
    }

    $ctx.fillStyle = '#492e85';
    $ctx.textAlign = 'center';
    $ctx.font = '180px monospace';

    if (evaluate) {
        var result = Evaluate.evaluate(scope, columns, c, r);
        if (typeof result === 'number') {
            $ctx.fillText('' + result, 0, 50, 1440);
        }
        return result;
    }
};

var drawGrid = function () {
    $ctx.translate(xTranslation, yTranslation);
    $ctx.scale(zoom, zoom);

    if ($showResults) {
        var lenColumns = $results.length;
        var lenCells = lenColumns > 0 ? $results[0].length : 0;
        var lenParentArgs = 0;
    } else {
        var columns = get($scope.cell, Cell.columns);
        var lenColumns = len(columns);
        if (lenColumns > 0) {
            var lenCells = len(getAt(columns, 0));
        } else {
            var lenCells = 0;
        }

        var parentArgs = get($scope.cell, Cell.args);
        var lenParentArgs = len(parentArgs);
    }

    $ctx.font = '32px monospace';
    $ctx.fillText($title, 0, lenParentArgs * -ySpacing - argGap - 20);
    $ctx.font = '12px monospace';

    var selectedCell = null;
    if (!$showResults && $c >= 0 && $c < lenColumns) {
        var selectedColumn = getAt(columns, $c);
        if ($r >= 0 && $r < len(selectedColumn)) {
            selectedCell = getAt(selectedColumn, $r);
        }
    }

    var argCs = [];
    var argRs = [];
    var selectedParentArgs = [];
    if (selectedCell) {
        var args = get(selectedCell, Cell.args);
        var lenArgs = len(args);
        var i;
        for (i = 0; i < lenArgs; i++) {
            var arg = getAt(args, i);
            var parentArgIndex = val(get(arg, Cell.Arg.parentArg));
            if (parentArgIndex >= 0) {
                selectedParentArgs[i] = parentArgIndex;
                argCs[i] = -1;
                argRs[i] = -1;
            } else {
                selectedParentArgs[i] = -1;
                argCs[i] = $c + val(get(arg, Cell.Arg.cDiff));
                argRs[i] = $r + val(get(arg, Cell.Arg.rDiff));
            }
        }
    }

    $ctx.strokeStyle = '#ccc';
    $ctx.fillStyle = '#333';
    $ctx.lineWidth = 2;

    var minX = Math.round(-xTranslation / zoom) - 2 * xHalfGap;
    var maxX = Math.round((window.innerWidth - xTranslation) / zoom) + 2 * xHalfGap;
    var minY = Math.round(-yTranslation / zoom) - 2 * yHalfGap;
    var maxY = Math.round((window.innerHeight - yTranslation) / zoom) + 2 * yHalfGap;
    var minC = Math.max(Math.floor(minX / xSpacing), 0);
    var maxC = Math.min(Math.floor(maxX / xSpacing), lenColumns - 1);
    var minR = Math.max(Math.floor(minY / ySpacing), 0);
    var maxR = Math.min(Math.floor(maxY / ySpacing), lenCells - 1);

    var argRsForC = [-1, -1, -1, -1];

    //////////////// parentArgs

    var i;
    var x = xHalfGap;
    for (i = 0; i < lenParentArgs; i++) {
        var arg = getAt(parentArgs, i);
        var argC = $scope.c + val(get(arg, Cell.Arg.cDiff));
        var argR = $scope.r + val(get(arg, Cell.Arg.rDiff));
        var column = getAt($scope.columns, argC);
        var cell = getAt(column, argR);
        var argIndex = selectedParentArgs.indexOf(i);
        var y = (lenParentArgs - i) * -ySpacing - argGap - yHalfGap;
        if (argIndex >= 0) {
            $ctx.save();
            $ctx.strokeStyle = '#777';
            $ctx.fillStyle = 'rgba(255, 255, 0, 0.2)';

            $ctx.fillRect(x - 8, y + 9, 162, 104);

            if (argIndex === movingArgIndex) {
                $ctx.lineDashOffset = 2.0;
                $ctx.setLineDash([16, 4]);
            }

            $ctx.strokeRect(x, y + 15, 146, 92);

            $ctx.restore();
        } else {
            $ctx.strokeRect(x, y + 15, 146, 92);  // 144 by 90 internal area
        }

        var text = val(get(cell, Cell.text));

        // draw result
        $ctx.save();

        $ctx.beginPath();
        $ctx.rect(x + 1, y + 16, 144, 90);
        $ctx.clip();

        $ctx.translate(x + 73, y + 61);
        $ctx.scale(0.1, 0.1);

        $ctx.textAlign = 'center';
        $ctx.font = '180px monospace';
        $ctx.fillStyle = '#492e85';

        var result = Evaluate.evaluate($scope.parent, $scope.columns, argC, argR);
        if (typeof result === 'number') {
            $ctx.fillText('' + result, 0, 50, 1440);
        }

        $ctx.restore();

        $ctx.fillText(text, x + 2, y + 11);
    }

    /////////////// grid

    var c;
    for (c = minC; c <= maxC; c++) {
        var cells = getAt(columns, c);
        var x = xSpacing * c + xHalfGap;

        var i;
        for (i = 0; i < argCs.length; i++) {
            if (argCs[i] === c) {
                argRsForC[i] = argRs[i];
            } else {
                argRsForC[i] = -1;
            }
        }

        var r;
        for (r = minR; r <= maxR; r++) {
            var cell = getAt(cells, r);
            var y = ySpacing * r + yHalfGap;

            var argIndex = argRsForC.indexOf(r);
            var withinBounds = $minC <= c && c <= $maxC && $minR <= r && r <= $maxR;
            if (argIndex >= 0) {
                $ctx.save();
                $ctx.strokeStyle = '#777';
                $ctx.fillStyle = 'rgba(255, 255, 0, 0.2)';

                if (withinBounds) {
                    $ctx.fillRect(x - 8, y + 9, 162, 52);
                    $ctx.fillStyle = 'rgba(26, 138, 249, 0.2)';
                    $ctx.fillRect(x - 8, y + 9 + 52, 162, 52);
                } else {
                    $ctx.fillRect(x - 8, y + 9, 162, 104);
                }

                if (argIndex === movingArgIndex) {
                    $ctx.lineDashOffset = 2.0;
                    $ctx.setLineDash([16, 4]);
                }

                $ctx.strokeRect(x, y + 15, 146, 92);

                $ctx.restore();
            } else if (withinBounds) {
                $ctx.save();
                $ctx.fillStyle = 'rgba(26, 138, 249, 0.2)';

                if (c === $c && r === $r) {
                    $ctx.strokeStyle = '#333';
                    $ctx.lineWidth = 4;
                    $ctx.strokeRect(x - 1, y + 14, 148, 94);
                } else {
                    $ctx.strokeStyle = '#777';
                    $ctx.strokeRect(x, y + 15, 146, 92);
                }

                $ctx.fillRect(x - 8, y + 9, 162, 104);

                $ctx.restore();
            } else {
                $ctx.strokeRect(x, y + 15, 146, 92);  // 144 by 90 internal area
            }

            if ($showResults) {
                var result = $results[c][r];
                var text = result.text;
            } else {
                var text = val(get(cell, Cell.text));

                // draw result
                $ctx.save();

                $ctx.beginPath();
                $ctx.rect(x + 1, y + 16, 144, 90);
                $ctx.clip();

                $ctx.translate(x + 73, y + 61);
                $ctx.scale(0.1, 0.1);

                $ctx.textAlign = 'center';
                $ctx.font = '180px monospace';
                $ctx.fillStyle = '#492e85';

                var result = Evaluate.evaluate($scope, columns, c, r);
                if (typeof result === 'number') {
                    $ctx.fillText('' + result, 0, 50, 1440);
                }

                $ctx.restore();
            }

            $ctx.fillText(text, x + 2, y + 11);
        }
    }

    var newColumn = $c === lenColumns;
    var newRow = $r === lenCells;
    if (newColumn || newRow) {
        $ctx.strokeStyle = '#080';
        $ctx.fillStyle = 'rgba(26,138,249,0.2)';
        $ctx.lineWidth = 4;

        var x = xSpacing * $c + xHalfGap;
        var y = ySpacing * $r + yHalfGap;

        $ctx.fillRect(x - 8, y + 9, 162, 104);

        $ctx.lineDashOffset = 2.0;
        $ctx.setLineDash([16, 4]);
        $ctx.strokeRect(x - 1, y + 14, 148, 94);
    }

    var emptyEscaped = $c === -1 && lenColumns === 0;
    if (emptyEscaped) {
        $ctx.strokeStyle = '#080';
        $ctx.fillStyle = 'rgba(26,138,249,0.2)';
        $ctx.lineWidth = 2;

        $ctx.lineDashOffset = 4.0;
        $ctx.setLineDash([16, 4]);
        $ctx.strokeRect(xHalfGap, yHalfGap + 15, 146, 92);
    }
};

Ui.moveAutocomplete = function () {
    if ($c === -1) {
        var autocompleteZoom = 0.7;
        var x = Math.floor(window.innerWidth / 2) - 100;
        var y = 30;
    } else {
        var autocompleteZoom = Math.max(0.6, Math.min(zoom, 3.0)) * 0.5;

        var x = ($c * xSpacing * zoom) + xTranslation + 2;
        var y = ($r * ySpacing * zoom) + yTranslation - 1;

    }

    autocompleteContainer.style.top = y + 'px';
    autocompleteContainer.style.left = x + 'px';
    autocompleteContainer.style.transform = 'scale(' + autocompleteZoom + ')';
};

})();
