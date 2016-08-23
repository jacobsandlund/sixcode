'use strict';
global.Autocomplete = {};
(function () {

var autocompleteContainer;
var autocompleteInput;
var autocompleteOriginal;
var autocompleteResults;
var matches;
var selectedMatchIndex = 0;

var entries = [
    '',
    '+',
    '-',
    '*',
    '/',
    '^',

    'square',
    'circle',
    'image',
    'scale',
    'scale x',
    'scale y',
    'move x',
    'move y',
    'mouse x',
    'mouse y',
    'rotate',
    'combine',
    'color',
];

var actionEntries = [
    'go up',
    'go into',
    'delete row',
    'delete column',
    'delete right columns',
    'copy column',
    'copy row',
    'copy over right cols',
    'insert row',
    'insert column',
    'collapse',

    'escape',
    'undo',
    'redo',
    'play',
    'fullscreen',
    'exit fullscreen',
    'save',
    'list repositories',
];

var numArgsTable = {
    '': 0,
    '+': 2,
    '-': 2,
    '*': 2,
    '/': 2,
    '^': 2,

    'square': 0,
    'circle': 0,
    'image': 1,
    'scale': 2,
    'scale x': 2,
    'scale y': 2,
    'move x': 2,
    'move y': 2,
    'mouse x': 0,
    'mouse y': 0,
    'rotate': 2,
    'combine': 2,
    'color': 2,
};

entries = actionEntries.concat(entries);
var actionEntriesMap = {};
actionEntries.forEach(function (entry) {
    actionEntriesMap[entry] = true;
});

Autocomplete.initialize = function () {
    autocompleteContainer = document.getElementById('autocomplete-container');
    autocompleteInput = document.getElementById('autocomplete-input');
    autocompleteResults = document.getElementById('autocomplete-results');
    autocompleteOriginal = document.getElementById('autocomplete-original');

    autocompleteInput.addEventListener('input', updateMatches);
    autocompleteInput.addEventListener('keydown', onKeyDown);

    matches = [];
};

Autocomplete.show = function () {
    autocompleteContainer.style.display = 'block';
    document.body.style.cursor = null;
    autocompleteInput.focus();
    autocompleteInput.setSelectionRange(0, autocompleteInput.value.length);
};

var getSelectedCell = function () {
    if ($showResults) {
        var lenColumns = $results.length;
        var lenCells = lenColumns > 0 ? $results[0].length : 0;

        if ($c >= 0 && $c < lenColumns && $r >= 0 && $r < lenCells) {
            var result = $results[$c][$r];
            return set($[Cell.zero],
                       Cell.text, hash(result.text));
        }
    } else {
        var parentCell = Main.getParentCell();
        var columns = get(parentCell, Cell.columns);
        var lenColumns = len(columns);
        if (lenColumns > 0) {
            var lenCells = len(getAt(columns, 0));
        } else {
            var lenCells = 0;
        }

        if ($c >= 0 && $c < lenColumns) {
            var selectedColumn = getAt(columns, $c);
            if ($r >= 0 && $r < len(selectedColumn)) {
                return getAt(selectedColumn, $r);
            }
        }
    }

    return $[Cell.zero];
};

Autocomplete.setSelectedCell = function () {
    if ($c !== -1) {
        var selectedCell = getSelectedCell();
        var text = val(get(selectedCell, Cell.text));
        autocompleteInput.value = text;
    }

    autocompleteInput.focus();
    autocompleteInput.setSelectionRange(0, autocompleteInput.value.length);

    updateMatches();
};

var updateMatches = function () {
    var selectedCell = getSelectedCell();
    var originalText = val(get(selectedCell, Cell.text));
    var text = autocompleteInput.value;

    if (text === originalText || originalText === '') {
        autocompleteOriginal.style.display = 'none';
    } else {
        autocompleteOriginal.style.display = 'block';
        autocompleteOriginal.innerText = originalText;
    }

    if (text === originalText || text === '') {
        matches = [];
    } else {
        matches = [];
        var i;
        for (i = 0; i < entries.length; i++) {
            var entry = entries[i];
            if (entry.indexOf(text) === 0) {
                matches.push(entry);
                if (matches.length === 6) {
                    break;
                }
            }
        }
    }

    selectedMatchIndex = 0;
    drawMatches();
};

var drawMatches = function () {
    var matchHtml = '';
    var i;
    for (i = 0; i < matches.length; i++) {
        if (i === selectedMatchIndex) {
            matchHtml += '<li class="selected">' + matches[i] + '</li>';
        } else {
            matchHtml += '<li>' + matches[i] + '</li>';
        }
    }
    autocompleteResults.innerHTML = matchHtml;
};

var onKeyDown = function (e) {
    var tab =  e.keyCode === 9;
    var down = e.keyCode === 40;
    var up =   e.keyCode === 38;
    if (tab) {
        e.preventDefault();
    }

    var next = (tab && !e.shiftKey) || down;
    var previous = (tab && e.shiftKey) || up;
    if (next) {
        selectedMatchIndex++;
        if (selectedMatchIndex >= matches.length) {
            selectedMatchIndex = 0;
        }
    }
    if (previous) {
        selectedMatchIndex--;
        if (selectedMatchIndex < 0) {
            selectedMatchIndex = matches.length - 1;
        }
    }

    if (next || previous) {
        drawMatches();
    }

    if (e.keyCode === 13) { // enter
        var keepCellSelected = e.shiftKey;
        selectMatch(keepCellSelected);
    } else if (e.keyCode === 27) { // escape
        Autocomplete.performMatch('escape');
        Ui.draw();
    }
};

var escape = function () {
    $c = -1;
    $r = -1;
    $minC = -1;
    $maxC = -1;
    $minR = -1;
    $maxR = -1;
    autocompleteInput.value = '';
    Autocomplete.setSelectedCell();
};

var selectMatch = function (keepCellSelected) {
    var text = autocompleteInput.value;
    if (selectedMatchIndex >= matches.length) {
        var matchText = text;
    } else {
        var matchText = matches[selectedMatchIndex];
    }

    Autocomplete.performMatch(matchText, keepCellSelected);
};

Autocomplete.performMatch = function (matchText, keepCellSelected) {
    var selectedCell = getSelectedCell();

    if ($showResults) {
        var lenColumns = $results.length;
        var lenCells = lenColumns > 0 ? $results[0].length : 0;
    } else {
        var parentCell = Main.getParentCell();
        var columns = get(parentCell, Cell.columns);
        var lenColumns = len(columns);
        if (lenColumns > 0) {
            var lenCells = len(getAt(columns, 0));
        } else {
            var lenCells = 0;
        }
    }

    var isAction = actionEntriesMap[matchText];

    var originalText = val(get(selectedCell, Cell.text));
    var makeCommit = !isAction;
    var keepCommandSelected = true;

    if (isAction) {
        switch (matchText) {
        case 'undo':
            var parent = get($head, Commit.parent);
            if (parent) {
                $head = parent;
                $project = get($head, Commit.tree);
            }
            break;

        case 'redo':
            var head = $redoHead;
            var childHead = head;
            while (head !== $head) {
                childHead = head;
                head = get(head, Commit.parent);
            }
            $head = childHead;
            $project = get($head, Commit.tree);
            break;

        case 'play':
            if (lenColumns === 0) {
                break;
            }
            $nextTickTime = 0;
            $playFrame = 0;

            autocompleteContainer.style.display = 'none';
            document.body.style.cursor = 'none';
            window.requestAnimationFrame(Main.tick);
            break;

        case 'fullscreen':
            escape();
            $fullscreen = true;
            keepCommandSelected = false;
            break;

        case 'escape':
            escape();
            keepCommandSelected = false;
            break;

        case 'exit fullscreen':
            autocompleteInput.value = '';
            $fullscreen = false;
            keepCommandSelected = false;
            break;

        case 'go into':
            if ($listRepos) {
                var result = $results[$c][$r];
                if (!result.fullName) {
                    break;
                }
                var gitUrl = GitHub.baseGitUrl(window.sessionStorage.githubAccessToken) + '/' + result.fullName + '.git';
                window.sessionStorage.setItem('gitUrl', gitUrl);
                window.sessionStorage.setItem('repoName', result.fullName);
                $showResults = false;
                $listRepos = false;
                Main.initializeRepo();
                return;
            } else if ($showResults) {
                // TODO
            } else {
                $path[$pathDepth] = $c;
                $path[$pathDepth + 1] = $r;
                $minC = $maxC = $c = 0;
                $minR = $maxR = $r = 0;
                $pathDepth += 2;
                Autocomplete.setSelectedCell();
                Ui.draw();
                keepCommandSelected = false;
            }
            break;

        case 'go up':
            if ($pathDepth > 0) {
                $pathDepth -= 2;
                $minC = $maxC = $c = $path[$pathDepth];
                $minR = $maxR = $r = $path[$pathDepth + 1];
            }
            Ui.draw();
            break;

        case 'list repositories':
            window.sessionStorage.removeItem('gitUrl');
            window.sessionStorage.removeItem('repoName');
            return Main.listRepos(null);

        case 'save':
            Main.save();
            break;

        case 'copy column':
            if ($c === lenColumns) {
                break;
            }
            var column = getAt(columns, $c);
            columns = insertAt(columns, $c, column);
            $c++;
            $minC = $maxC = $c;
            Ui.moveAutocomplete();
            makeCommit = true;
            break;

        case 'copy row':
            if ($r === lenCells) {
                break;
            }
            var i;
            for (i = 0; i < lenColumns; i++) {
                var column = getAt(columns, i);
                var cell = getAt(column, $r);
                column = insertAt(column, $r, cell);
                columns = setAt(columns, i, column);
            }
            $r++;
            $minR = $maxR = $r;
            Ui.moveAutocomplete();
            makeCommit = true;
            break;

        case 'insert column':
            if ($c === lenColumns) {
                break;
            }
            var cells = ArrayTree.$zeros[0];
            var i;
            for (i = 0; i < lenCells; i++) {
                cells = push(cells, $[Cell.zero]);
            }
            columns = insertAt(columns, $c, cells);
            makeCommit = true;
            break;

        case 'insert row':
            if ($r === lenCells) {
                break;
            }
            var i;
            for (i = 0; i < lenColumns; i++) {
                var column = getAt(columns, i);
                column = insertAt(column, $r, $[Cell.zero]);
                columns = setAt(columns, i, column);
            }
            makeCommit = true;
            break;

        case 'delete column':
            if ($c < lenColumns) {
                columns = deleteAt(columns, $c);
            }
            makeCommit = true;
            break;

        case 'delete row':
            if ($r === lenCells) {
                break;
            }

            var i;
            for (i = 0; i < lenColumns; i++) {
                var column = getAt(columns, i);
                column = deleteAt(column, $r);
                columns = setAt(columns, i, column);
            }
            makeCommit = true;
            break;

        case 'delete right columns':
            if ($c < lenColumns - 1) {
                columns = take(columns, $c + 1);
            }
            makeCommit = true;
            break;

        case 'collapse':
            var childColumns = ArrayTree.$zeros[0];
            var c;
            for (c = $minC; c <= $maxC; c++) {
                var r;
                var column = getAt(columns, c);
                var childColumn = ArrayTree.$zeros[0];
                for (r = $minR; r <= $maxR; r++) {
                    var cell = getAt(column, r);
                    childColumn = push(childColumn, cell);
                }
                childColumns = push(childColumns, childColumn);
            }

            for (c = $maxC; c > $minC; c--) {
                columns = deleteAt(columns, c);
            }
            lenColumns -= $maxC - $minC;
            for (c = 0; c < lenColumns; c++) {
                var column = getAt(columns, c);
                for (r = $maxR; r > $minR; r--) {
                    column = deleteAt(column, r);
                }
                columns = setAt(columns, c, column);
            }
            var cell = set($[Cell.zero], Cell.columns, childColumns);
            var column = getAt(columns, $minC);
            column = setAt(column, $minR, cell);
            columns = setAt(columns, $minC, column);
            makeCommit = true;

            $c = $maxC = $minC;
            $r = $maxR = $minR;
            break;
        }

    } else if (matchText === originalText && originalText !== '') {
        makeCommit = false;  // do nothing
    } else {

        var numArgs = numArgsTable[matchText];
        var args = ArrayTree.$zeros[0];
        var i;
        for (i = 0; i < numArgs; i++) {
            var arg = set($[Cell.Arg.zero],
                          Cell.Arg.cDiff, Constants.$positive[0],
                          Cell.Arg.rDiff, Constants.$negative[numArgs - i]);
            args = push(args, arg);
        }

        selectedCell = set(selectedCell,
                           Cell.text, hash(matchText),
                           Cell.args, args);

        var newColumn = $c === lenColumns;
        var newRow = $r === lenCells;

        if ($listRepos) {
            if (newColumn || newRow) {
                var username = window.sessionStorage.githubUsername;
                var fullName = username + '/' + matchText;
                var accessToken = window.sessionStorage.githubAccessToken;
                var gitUrl = GitHub.baseGitUrl(accessToken) + '/' + fullName + '.git';
                window.sessionStorage.setItem('gitUrl', gitUrl);
                window.sessionStorage.setItem('repoName', fullName);

                var repoConfig = {
                    name: matchText,
                    homepage: 'https://www.getspacetime.com/' + fullName,
                };
                GitHub.createRepo(repoConfig, accessToken, function (err, repo, xhr) {
                    Main.initializeNewRepo();
                    Main.save();
                    $listRepos = false;
                    $showResults = false;
                    $c = 0;
                    $r = 0;
                    Autocomplete.setSelectedCell();
                    Ui.draw();
                });
                return;
            }

        } else {

            if (newColumn) {
                var cells = ArrayTree.$zeros[0];
                var i;
                for (i = 0; i < lenCells; i++) {
                    cells = push(cells, $[Cell.zero]);
                }
                columns = push(columns, cells);
                lenColumns++;
            }

            if (newRow && lenColumns === 1) {
                var selectedColumn = getAt(columns, 0);
                selectedColumn = push(selectedColumn, selectedCell);
                lenCells++;
            } else {
                if (newRow) {
                    var i;
                    for (i = 0; i < lenColumns; i++) {
                        var column = getAt(columns, i);
                        column = push(column, $[Cell.zero]);
                        columns = setAt(columns, i, column);
                    }
                    lenCells++;
                }

                var selectedColumn = getAt(columns, $c);
                selectedColumn = setAt(selectedColumn, $r, selectedCell);
            }

            columns = setAt(columns, $c, selectedColumn);
        }
    }

    if (makeCommit) {
        parentCell = set(parentCell, Cell.columns, columns);
        var oldProject = $project;
        Main.updatePath(parentCell);

        if ($project !== oldProject) {
            var now = Math.floor(+Date.now() / 1000);
            $head = createCommit($head,
                                 Commit.tree, $project,
                                 Commit.parent, $head,
                                 Commit.committerTime, now);
            $redoHead = $head;
        }
    }

    if (isAction) {
        if (keepCommandSelected) {
            autocompleteInput.value = matchText;
            autocompleteInput.setSelectionRange(0, matchText.length);
        }
        updateMatches();
    } else {
        if (!keepCellSelected) {
            $r++;
            $minR = $maxR = $r;
        }
        Autocomplete.setSelectedCell();
    }

    Ui.draw();
};

})();
