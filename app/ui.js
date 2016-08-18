'use strict';
global.Ui = {};
(function () {

var canvas;
var autocompleteContainer;
var autocompleteInput;

global.$ctx = null;

global.$mouseX = 0;
global.$mouseY = 0;

var zoom = 1;

var xSpacing = 160;
var ySpacing = 112;
Ui.ySpacing = ySpacing;

var xHalfGap = 5;
var yHalfGap = 2;
var xTranslation = 0;
var yTranslation = 0;
var mouseXAtDown = 0;
var mouseYAtDown = 0;

var mouseDown = false;
var movingGrid = false;

Ui.initialize = function () {
    canvas = document.getElementById('canvas');
    canvas.width = window.devicePixelRatio * window.innerWidth;
    canvas.height = window.devicePixelRatio * window.innerHeight;
    canvas.style.width = window.innerWidth + 'px';
    canvas.style.height = window.innerHeight + 'px';

    autocompleteContainer = document.getElementById('autocomplete-container');
    autocompleteInput = document.getElementById('autocomplete-input');

    xTranslation = Math.floor(window.innerWidth / 3 - xSpacing / 2);
    yTranslation = Math.floor(window.innerHeight / 4 - ySpacing / 2);

    $ctx = canvas.getContext('2d');
    $ctx.font = '12px monospace';

    canvas.addEventListener('click', function (e) {
        if ($fullscreen || movingGrid) {
            return;
        }
        e.preventDefault();

        var x = Math.round((e.clientX - xTranslation) / zoom);
        var y = Math.round((e.clientY - yTranslation) / zoom);
        var c = Math.floor(x / xSpacing);
        var r = Math.floor(y / ySpacing);

        if ($showResults) {
            var lenColumns = $results.length;
            var lenCells = lenColumns > 0 ? $results[0].length : 0;
        } else {
            var project = get($head, Commit.tree);
            var parentCell = get(project, Project.cell);
            var columns = get(parentCell, Cell.columns);
            var lenColumns = len(columns);
            if (lenColumns > 0) {
                var lenCells = len(getAt(columns, 0));
            } else {
                var lenCells = 0;
            }
        }

        if (c >= lenColumns) {
            c = lenColumns;
        }
        if (c < 0) {
            c = 0;
        }
        if (r >= lenCells) {
            r = lenCells;
        }
        if (r < 0) {
            r = 0;
        }

        if (e.shiftKey && !$showResults) {
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

            if (c > $c || (c === $c && r >= $r)) {
                return;
            }

            var i;
            for (i = 0; i < lenArgs; i++) {
                var arg = getAt(args, i);
                var argC = $c + val(get(arg, Cell.Arg.cDiff));
                var argR = $r + val(get(arg, Cell.Arg.rDiff));
                if (c === argC && r === argR) {
                    $argIndex = i;
                    Ui.draw();
                    return;
                }
            }

            var cDiff = c - $c;
            var rDiff = r - $r;
            var arg = getAt(args, $argIndex);

            arg = set(arg,
                      Cell.Arg.cDiff, hash(cDiff),
                      Cell.Arg.rDiff, hash(rDiff));

            args = setAt(args, $argIndex, arg);
            selectedCell = set(selectedCell, Cell.args, args);
            selectedColumn = setAt(selectedColumn, $r, selectedCell);
            columns = setAt(columns, $c, selectedColumn);
            parentCell = set(parentCell, Cell.columns, columns);
            var oldProject = project;
            project = set(project, Project.cell, parentCell);

            if (project !== oldProject) {
                var now = Math.floor(+Date.now() / 1000);
                $head = createCommit($head,
                                     Commit.tree, project,
                                     Commit.parent, $head,
                                     Commit.committerTime, now);
                $redoHead = $head;
            }

        } else {
            $c = c;
            $r = r;
            $argIndex = 0;
            Autocomplete.setSelectedCell();
        }

        Ui.draw();
    });

    canvas.addEventListener('mousedown', function (e) {
        $mouseX = e.clientX;
        $mouseY = e.clientY;
        mouseXAtDown = $mouseX;
        mouseYAtDown = $mouseY;
        mouseDown = true;
        e.preventDefault();
    });

    canvas.addEventListener('mouseup', function (e) {
        mouseDown = false;
        setTimeout(function () {
            movingGrid = false;
        });
    });

    canvas.addEventListener('mousemove', function (e) {
        if (!movingGrid) {
            var moved = (
                Math.abs(e.clientX - mouseXAtDown) > 2 ||
                Math.abs(e.clientY - mouseYAtDown) > 2
            );
            if (mouseDown && moved) {
                movingGrid = true;
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
        drawFullscreen();
    } else {
        drawGrid();
    }

    $ctx.restore();

    console.timeEnd('UI.draw');
}

var drawFullscreen = function () {
    var centerX = Math.floor(window.innerWidth / 2);
    var centerY = Math.floor(window.innerHeight / 2);
    $ctx.translate(centerX, centerY);
    $ctx.scale(window.innerWidth / 1440, window.innerHeight / 900);

    var project = get($head, Commit.tree);
    var parentCell = get(project, Project.cell);
    var columns = get(parentCell, Cell.columns);
    var lenColumns = len(columns);
    if (lenColumns > 0) {
        var lenCells = len(getAt(columns, 0));
    } else {
        var lenCells = 0;
    }

    if ($playFrame === -1) {
        var c = lenColumns - 1;
    } else {
        var c = $playFrame;
    }
    var r = lenCells - 1;

    $ctx.fillStyle = '#492e85';

    if (c >= 0 && c < lenColumns) {
        Evaluate.evaluate(parentCell, columns, c, r);
    }
};

var drawGrid = function () {
    $ctx.translate(xTranslation, yTranslation);
    $ctx.scale(zoom, zoom);

    $ctx.font = '32px monospace';
    $ctx.fillText($title, 0, -20);
    $ctx.font = '12px monospace';

    if ($showResults) {
        var lenColumns = $results.length;
        var lenCells = lenColumns > 0 ? $results[0].length : 0;
    } else {
        var project = get($head, Commit.tree);
        var parentCell = get(project, Project.cell);
        var columns = get(parentCell, Cell.columns);
        var lenColumns = len(columns);
        if (lenColumns > 0) {
            var lenCells = len(getAt(columns, 0));
        } else {
            var lenCells = 0;
        }
    }

    var selectedCell = null;
    if (!$showResults && $c >= 0 && $c < lenColumns) {
        var selectedColumn = getAt(columns, $c);
        if ($r >= 0 && $r < len(selectedColumn)) {
            selectedCell = getAt(selectedColumn, $r);
        }
    }

    var argCs = [];
    var argRs = [];
    if (selectedCell) {
        var args = get(selectedCell, Cell.args);
        var lenArgs = len(args);
        var i;
        for (i = 0; i < lenArgs; i++) {
            var arg = getAt(args, i);
            argCs[i] = $c + val(get(arg, Cell.Arg.cDiff));
            argRs[i] = $r + val(get(arg, Cell.Arg.rDiff));
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
            if (argIndex >= 0) {
                $ctx.save();
                $ctx.strokeStyle = '#777';
                $ctx.fillStyle = 'rgba(255, 255, 0, 0.2)';

                $ctx.fillRect(x - 8, y + 9, 162, 104);

                if (argIndex === $argIndex) {
                    $ctx.lineDashOffset = 2.0;
                    $ctx.setLineDash([16, 4]);
                }

                $ctx.strokeRect(x, y + 15, 146, 92);

                $ctx.restore();
            } else if (c === $c && r === $r) {
                $ctx.save();
                $ctx.strokeStyle = '#333';
                $ctx.fillStyle = 'rgba(26, 138, 249, 0.2)';
                $ctx.lineWidth = 4;

                $ctx.strokeRect(x - 1, y + 14, 148, 94);
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

                var result = Evaluate.evaluate(parentCell, columns, c, r);
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
