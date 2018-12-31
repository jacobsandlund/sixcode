# sixcode

`sixcode` is coding using hexagons.

### Testing

To run all tests, use `bin/sixc test`. All lines starting with `//=> ` are added automatically when the file runs (e.g. `bin/run hex-test.c`). Broken tests will result in a `git diff`. If the changed output is desired, simply `git add` the diff.

If you are using Vim, the following adds a keyboard shortcut (`<leader>r`) to run the test file and reload any changes:

``` vim
command -nargs=0 RunSixcodeTest execute 'silent !bin/vim-test %' | redraw!
nnoremap <leader>r :w<CR>:RunSixcodeTest<CR>:e!<CR>
```

### Development

To run `sixc` without needing the `bin/` prefix, symlink it to a place on the $PATH:

``` bash
ln -s $PWD/bin/sixc /usr/local/bin
```
