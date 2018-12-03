import * as fs from 'fs';
import * as util from 'util';

let allFileInfo = new Map();

const maxOutputLines = 200;
const indentSpace = '                                                                                                    ';
const stackLineRe = /at [^\/]*(\/.+):(\d+):(\d+)\)?$/;
const errorSourceScrubRe = /\/.*\//;
const isLogRe = /^\s*\/\/=>/;

export function test(name, callback) {
    try {
        callback(_);
    } catch (e) {
        let stackLines = e.stack.split('\n');
        let testLineI = stackLines.findIndex(line => line.startsWith('    at test ('));
        let stackLine = stackLines[testLineI - 1];
        let lineMatch = stackLine && stackLineRe.exec(stackLine);
        if (!lineMatch) {
            throw new Error('Could not find source of Error: ' + e);
        }

        let errorSourceLines = stackLines.slice(1, testLineI - 1).map(line => {
            return line.replace(errorSourceScrubRe, '//.../');
        });

        let output = '' + e;
        if (errorSourceLines.length > 0) {
            output += '\n' + errorSourceLines.join('\n');
        }

        writeOutputAtLineMatch(lineMatch, output);
    }
}

function _(...args) {
    let stackLine = getStackLine();
    let lineMatch = stackLine && stackLineRe.exec(stackLine);
    if (!lineMatch) {
        throw new Error("Could not find source of '_' call");
    }

    let output = util.format.apply(util, args);

    writeOutputAtLineMatch(lineMatch, output);
}

function writeOutputAtLineMatch(lineMatch, output) {
    let file = lineMatch[1];
    let line = +lineMatch[2] - 1;
    let character = +lineMatch[3] - 1;

    let fileInfo = allFileInfo.get(file);

    if (!fileInfo) {
        fileInfo = {
            lines: null,
            changed: false,
            lineCountDelta: 0,
            writeArguments: [],
        };

        allFileInfo.set(file, fileInfo);

        fs.readFile(file, 'utf8', function (err, text) {
            if (err) {
                throw err;
            }

            fileInfo.lines = text.split('\n');

            for (let writeArgs of fileInfo.writeArguments) {
                writeOutputToFile(...writeArgs);
            }
        });
    }

    if (fileInfo.lines) {
        writeOutputToFile(file, line, character, output);
    } else {
        fileInfo.writeArguments.push([file, line, character, output]);
    }
}

function getStackLine() {
    try {
        throw new Error('intentional error');
    } catch (e) {
        return e.stack.split('\n', 5)[3];
    }
}

function writeOutputToFile(file, line, character, output) {
    let fileInfo = allFileInfo.get(file);
    let lines = fileInfo.lines;

    let outputLines = output.split('\n');
    if (outputLines.length > maxOutputLines) {
        outputLines[maxOutputLines] = '... and ' + outputLines.length + ' more';
        outputLines.length = maxOutputLines + 1;
    }

    outputLines = outputLines.map(line => {
        let indent = indentSpace.slice(0, character);
        if (line.length) {
            return indent + '//=> ' + line;
        } else {
            return indent + '//=>';
        }
    });

    let j = line + fileInfo.lineCountDelta + 1;

    let i;
    for (i = 0; i < outputLines.length; i++) {
        let line = lines[j + i];
        let outLine = outputLines[i];
        if (outLine !== line) {
            fileInfo.changed = true;
            if (isLogRe.test(line)) {
                lines[j + i] = outLine;
            } else {
                lines.splice(j + i, 0, outLine);
                fileInfo.lineCountDelta++;
            }
        }
    }

    i += j;
    while (i < lines.length) {
        let line = lines[i];
        if (isLogRe.test(line)) {
            lines.splice(i, 1);
            fileInfo.changed = true;
            fileInfo.lineCountDelta--;
        } else {
            break;
        }
    }
}

process.on('exit', () => {
    for (let [file, fileInfo] of allFileInfo) {
        if (fileInfo.changed) {
            fs.writeFileSync(file, fileInfo.lines.join('\n'), 'utf8');
        }
    }
});
