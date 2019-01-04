# sixcode

`sixcode` is coding using hexagons.

### Testing

To run all tests, use `./sixc test`. All lines starting with `//=> ` are added automatically when the file runs (e.g. `./sixc run hex-test.c`). Broken tests will result in a `git diff`. If the changed output is desired, simply `git add` the diff.

If you are using Vim, the following `.vimrc` lines add a keyboard shortcut (`<leader>r`) to run the test file and reload any changes, as well as a shortcut (`<leader>d`) to diff a file to see what changed.

``` vim
" Sixcode configs:
command -nargs=0 RunSixcode execute 'silent !./sixc run % || read -n1 -r -p "... Press any key to continue"' | redraw!
nnoremap <leader>r :w<CR>:RunSixcode<CR>:e!<CR>

command -nargs=0 GitDiff execute 'silent !git --no-pager diff --exit-code -- % || read -n1 -r -p "... Press any key to continue"' | redraw!
nnoremap <leader>d :w<CR>:GitDiff<CR>
```
