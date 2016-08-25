'use strict';
global.Scope = {};
(function () {

Scope.create = function () {
    return {
        parent: null,
        c: 0,
        r: 0,
        cell: 0,
        columns: 0,
    };
};

Scope.load = function (scope, project) {
    var cell = get(project, Project.cell);
    var scopes = [scope];
    while (scope.parent) {
        scope = scope.parent;
        scopes.push(scope);
    }

    scopes[scopes.length - 1].cell = cell;

    var i;
    for (i = scopes.length - 2; i >= 0; i--) {
        var columns = getAt(cell, Cell.columns);
        scope = scopes[i];
        if (scope.c >= len(columns)) {
            return scopes[i + 1];
        }
        var column = getAt(columns, scope.c);
        if (scope.r >= len(column)) {
            return scopes[i + 1];
        }
        var cell = getAt(column, scope.r);
        var columns = get(cell, Cell.columns);
        scope.cell = cell;
        scope.columns = columns;
    }

    return scopes[0];
};

Scope.goInto = function (parentScope, cell, c, r) {
    var parentCell = parentScope.cell;
    var columns = getAt(parentCell, Cell.columns);

    var scope = Scope.create();
    scope.parent = parentScope;
    scope.c = c;
    scope.r = r;
    scope.cell = cell;
    scope.columns = columns;
    return scope;
};

Scope.update = function (scope, bottomCell) {
    var cell = bottomCell;
    while (true) {
        scope.cell = cell;
        var text = val(get(cell, Cell.text));
        if (text !== '') {
            Autocomplete.registerEntry(text, cell);
        }
        if (!scope.parent) {
            break;
        }
        var columns = scope.columns;
        var column = getAt(columns, scope.c);
        column = setAt(column, scope.r, cell);
        columns = scope.columns = setAt(columns, scope.c, column);
        scope = scope.parent;
        cell = set(scope.cell, Cell.columns, columns);
    }

    $project = set($project, Project.cell, cell);
};

})();
