const Koa = require('koa');

const PORT = 3000;

const app = new Koa();

app.use(async ctx => {
    ctx.body = 'Hello World';
});

app.listen(PORT);
console.log('Listening on port ' + PORT);
