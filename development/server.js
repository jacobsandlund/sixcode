const util = require('util');
const execFile = util.promisify(require('child_process').execFile);
const path = require('path');
const Koa = require('koa');
const send = require('koa-send');

const PORT = 3000;
const OPTIONS = {root: path.join(__dirname, '..', 'web')};
const EMMAKE = '/emscripten/emscripten/emmake';
const MAKE_ARGS = ['make', '-f', 'development/Makefile'];

const app = new Koa();

app.use(async ctx => {
    try {
        await execFile(EMMAKE, MAKE_ARGS);
    } catch (e) {
        console.error(e.message);
        ctx.status = 500;
        ctx.body = 'Make failed with: ' + e.message;
        return;
    }

    await send(ctx, ctx.path, OPTIONS);
});

app.listen(PORT);
console.log('Listening on port ' + PORT);
