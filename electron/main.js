const {
    app,
    contentTracing,
    BrowserWindow,
    ipcMain,
} = require('electron');

let mainWindow;

function createWindow() {
    mainWindow = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            nodeIntegration: true,
        },
    });

    mainWindow.loadFile('index.html');

    mainWindow.on('closed', function () {
        mainWindow = null;
    });
}

app.on('ready', createWindow);

app.on('window-all-closed', function () {
    // On macOS it is common for applications and their menu bar
    // to stay active until the user quits explicitly with Cmd + Q
    if (process.platform !== 'darwin') {
        app.quit();
    }
});

app.on('activate', function () {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (mainWindow === null) {
        createWindow();
    }
});

const traceOptions = {
    categoryFilter: '*',
    traceOptions: 'record-until-full,enable-sampling',
};

ipcMain.on('trace', (event, traceTime) => {
    console.log('Tracing started');
    contentTracing.startRecording(traceOptions).then(function () {
        setTimeout(function() {
            let date = new Date();
            let year = date.getFullYear();
            let month = date.getMonth() + 1;
            let day = date.getDate();
            let hour = date.getHours();
            let minute = date.getMinutes();
            let second = date.getSeconds();
            let millisecond = date.getMilliseconds();
            let file = `out/traces/trace-${year}-${month}-${day}T${hour}-${minute}-${second}-${millisecond}.json`;

            contentTracing.stopRecording(file).then(function (path) {
                console.log('Tracing data recorded to ' + path);
            });
        }, traceTime);
    });
});
