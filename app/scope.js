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

    var topScope = scopes[scopes.length - 1];
    topScope.cell = cell;
    var columns = get(cell, Cell.columns);
    topScope.columns = columns;
    scopes[scopes.length - 1].cell = cell;

    var i;
    for (i = scopes.length - 2; i >= 0; i--) {
        scope = scopes[i];
        if (scope.c >= len(columns)) {
            return scopes[i + 1];
        }
        var column = getAt(columns, scope.c);
        if (scope.r >= len(column)) {
            return scopes[i + 1];
        }
        var cell = getAt(column, scope.r);
        columns = get(cell, Cell.columns);
        scope.cell = cell;
        scope.columns = columns;
    }

    return scopes[0];
};

Scope.goInto = function (parentScope, cell, c, r) {
    var scope = Scope.create();
    scope.parent = parentScope;
    scope.c = c;
    scope.r = r;
    scope.cell = cell;
    scope.columns = get(cell, Cell.columns);
    return scope;
};

Scope.update = function (scope, bottomCell) {
    var cell = bottomCell;
    var columns = get(cell, Cell.columns);
    while (true) {
        scope.cell = cell;
        scope.columns = columns;
        var text = val(get(cell, Cell.text));
        if (text !== '') {
            Autocomplete.registerEntry(text, cell);
        }
        if (!scope.parent) {
            break;
        }
        columns = scope.parent.columns;
        var column = getAt(columns, scope.c);
        column = setAt(column, scope.r, cell);
        columns = setAt(columns, scope.c, column);
        scope = scope.parent;
        cell = set(scope.cell, Cell.columns, columns);
    }

    $project = set($project, Project.cell, cell);
};

})();
