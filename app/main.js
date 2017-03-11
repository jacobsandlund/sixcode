'use strict';
global.Main = {};
(function () {

global.$head = 0;
global.$redoHead = 0;
global.$r = 0;
global.$c = 0;
global.$project = 0;
global.$scope = null;
global.$minR = -1;
global.$maxR = -1;
global.$minC = -1;
global.$maxC = -1;
global.$playScope = null;
global.$playC = 0;
global.$playR = 0;
global.$nextTickTime = 0;
global.$fullscreen = false;

global.$showResults = false;
global.$title = '';
global.$results = [];
global.$listRepos = false;

var gitmem;

var refName = 'refs/heads/master';

var getGitHubAccessToken = function (callback) {
    var params = parseParams();

    // http://stackoverflow.com/questions/20325763/browser-sessionstorage-share-between-tabs
    window.addEventListener('storage', function (event) {
        if (!event.newValue) {
            return;
        }

        if (event.key === 'getSessionStorage') {
            propogateSessionStorage();
        } else if (event.key === 'sessionStorage') {
            var session = JSON.parse(event.newValue);
            var key;
            for (key in session) {
                window.sessionStorage.setItem(key, session[key]);
            }
        }
    });

    if (params.error) {
        var errorDiv = document.createElement('div');
        errorDiv.innerHTML = decodeURIComponent(params.error_description).replace(/\+/g, ' ');
        document.body.appendChild(errorDiv);
        return;
    } else if (params.code) {
        window.history.replaceState({}, '', window.location.pathname);
        GitHub.getAccessToken(params.code, function (accessToken) {
            window.sessionStorage.setItem('githubAccessToken', accessToken);
            GitHub.user(accessToken, function (err, user, xhr) {
                if (err) {
                    throw err;
                }
                var email = user.email || 'email@example.com';
                window.sessionStorage.setItem('githubUsername', user.login);
                window.sessionStorage.setItem('gitUserName', user.name);
                window.sessionStorage.setItem('gitUserEmail', email);
                propogateSessionStorage();
                callback();
            });
        });
        return;
    } else if (!window.sessionStorage.length) {
        localStorage.setItem('getSessionStorage', 'request sessionStorage');
        localStorage.removeItem('getSessionStorage');

        window.setTimeout(function () {
            if (window.sessionStorage.length) {
                callback();
            } else {
                GitHub.authorize();
            }
        }, 30);
        return;
    }

    callback();
};

var propogateSessionStorage = function () {
    window.localStorage.setItem('sessionStorage', JSON.stringify({
        githubAccessToken: window.sessionStorage.githubAccessToken,
        githubUsername: window.sessionStorage.githubUsername,
        gitUserName: window.sessionStorage.gitUserName,
        gitUserEmail: window.sessionStorage.gitUserEmail,
    }));
    window.localStorage.removeItem('sessionStorage');
};

Main.initialize = function () {
    getGitHubAccessToken(postGitHubInit);
};

var postGitHubInit = function () {
    GitMem.initialize();
    gitmem = GitMem.create();

    Input.initialize();
    Cell.initialize();
    Project.initialize();
    Ui.initialize();
    Autocomplete.initialize();

    $scope = Scope.create();

    if (window.sessionStorage.gitUrl) {
        Main.initializeRepo();
    } else {
        Main.listRepos(null);
    }
};

Main.listRepos = function (username) {
    var accessToken = window.sessionStorage.githubAccessToken;

    GitHub.repos(username, accessToken, function (err, repos, xhr) {
        if (err && xhr.status === 401) {
            return GitHub.authorize();
        }

        $showResults = true;
        $listRepos = true;
        username = username || window.sessionStorage.githubUsername;
        $title = 'programs by ' + username;
        $results = [];
        var lenCells = Math.floor((window.innerHeight - 200) / Ui.ySpacing);
        if (lenCells > repos.length) {
            lenCells = repos.length;
        }
        var lenColumns = Math.ceil(repos.length / lenCells);
        var emptyResult = {name: '', full_name: ''};
        var c;
        for (c = 0; c < lenColumns; c++) {
            $results[c] = [];
            var r;
            for (r = 0; r < lenCells; r++) {
                var result = repos[c * lenCells + r] || emptyResult;
                $results[c][r] = {
                    text: result.name,
                    fullName: result.full_name,
                };
            }
        }

        firstDraw();
    });
};

var firstDraw = function () {
    Autocomplete.setSelectedCell();
    Autocomplete.show();
    Ui.draw();
};

var parseParams = function () {
    var paramPairs = window.location.search.slice(1).split('&');
    var params = {};
    paramPairs.forEach(function (pair) {
        var split = pair.split('=');
        params[split[0]] = split[1];
    });

    return params;
};

Main.initializeRepo = function () {
    var gitUrl = window.sessionStorage.gitUrl;

    Remote.queryRef(gitUrl, refName, function (remoteCommit) {
        if (remoteCommit === $[Constants.zeroHash]) {
            Main.initializeNewRepo();
            firstDraw();
        } else {
            Remote.fetch(gitUrl, remoteCommit, $[Constants.zeroHash], function () {
                $title = window.sessionStorage.repoName;
                $head = remoteCommit;
                $redoHead = $head;
                $project = get($head, Commit.tree);
                $scope = Scope.load($scope, $project);
                firstDraw();
            });
        }
    });
};

Main.initializeNewRepo = function () {
    $project = $[Project.zero];
    $scope = Scope.load($scope, $project);

    var userName = window.sessionStorage.gitUserName;
    var userEmail = window.sessionStorage.gitUserEmail;
    var timezoneOffset = (new Date()).getTimezoneOffset();

    var user = set($[Commit.User.zero],
                    Commit.User.name, hash(userName),
                    Commit.User.email, hash(userEmail),
                    Commit.User.timezoneOffset, Constants.integer(timezoneOffset));

    var info = set($[Commit.Info.zero],
                    Commit.Info.author, user,
                    Commit.Info.committer, user);

    var now = Math.round(+Date.now() / 1000);

    $head = createCommit($[Commit.zero],
                         Commit.info, info,
                         Commit.tree, $project,
                         Commit.parent, 0,
                         Commit.committerTime, now,
                         Commit.message, hash('automatic commit'));
    $redoHead = $head;
    $title = window.sessionStorage.repoName;
};

Main.save = function () {
    var gitUrl = window.sessionStorage.gitUrl;

    Remote.queryRef(gitUrl, refName, function (remoteCommit) {
        var packLength = Pack.create($head);
        Remote.push(gitUrl, refName, remoteCommit, $head, packLength, function (response) {
            // TODO: error handling
        });
    });
};

Main.tick = function (now) {
    if ($playScope === null) {
        return;
    }

    var timeToNextTick = $nextTickTime - now;
    if (timeToNextTick > 10 && $nextTickTime !== 0) {
        window.requestAnimationFrame(Main.tick);
        return;
    }

    Input.capture();
    evalAndDraw();

    if (!$nextTickTime) {
        $nextTickTime = now;
    }
    $nextTickTime += 33.3333333;
    window.requestAnimationFrame(Main.tick);
};

Main.stopPlaying = function () {
    $playScope = null;
    var oldProject = get($head, Commit.tree);

    if ($project !== oldProject) {
        var now = Math.floor(+Date.now() / 1000);
        $head = createCommit($head,
                             Commit.tree, $project,
                             Commit.parent, $head,
                             Commit.committerTime, now);
        $redoHead = $head;
    }
    Autocomplete.show();
    Ui.draw();
};

Main.startPlaying = function () {
    $nextTickTime = 0;
    $playScope = $scope;
    $playC = 0;
    $playR = len(getAt($playScope.columns, 0)) - 1;
    descendScope();
};

var descendScope = function () {
    if ($playC < len($playScope.columns)) {
        var playColumn = getAt($playScope.columns, $playC);
        var playCell = getAt(playColumn, $playR);
        var childColumns = get(playCell, Cell.columns);
        if (len(childColumns) >= 1) {
            $playScope = Scope.goInto($playScope, playCell, $playC, $playR);
            $playC = 0;
            $playR = len(getAt(childColumns, 0)) - 1;
            descendScope();
        }
    }
};

var evalAndDraw = function () {
    if ($fullscreen) {
        var result = Ui.draw();
    } else {
        var result = Ui.drawPlayPreview();
        Ui.draw();
    }
    $playC++;

    if (result === Evaluate.stop) {
        if ($playScope === $scope) {
            Main.stopPlaying();
        } else {
            $playC = $playScope.c;
            $playR = $playScope.r;
            $playScope = $playScope.parent;
            descendScope();
            evalAndDraw();
        }
    }
};

})();
