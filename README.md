# sixcode

`sixcode` is coding using hexagons.

### Testing

To run all tests, use `make`. All lines starting with `//=> ` are added automatically when the file runs (e.g. `./run test-hello.c`). Broken tests will result in a `git diff`. If the changed output is desired, simply `git add` the diff.

If you are using Vim, the following adds a keyboard shortcut (`<leader>r`) to run the test file and reload any changes:

``` vim
command -nargs=0 Run execute 'silent !./run %' | redraw!
nnoremap <leader>r :w<CR>:Run<CR>:e!<CR>
```
