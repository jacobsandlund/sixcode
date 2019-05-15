# 6code

`6code` is coding using hexagons.

### Testing

To run all tests, use `bin/test`. All lines starting with `//=> ` are added automatically when the file runs (e.g. `bin/run hex-test.c`). Broken tests will result in a `git diff`. If the changed output is desired, simply `git add` the diff.

If you are using Vim, the following `.vimrc` lines add a keyboard shortcut (`<leader>r`) to run the test file and reload any changes, as well as a shortcut to `git add --patch` (`<leader>a`) a file.

``` vim
" 6code configs:
command -nargs=0 Run6code execute 'silent !bin/run % || read -n1 -r -p "=============== Press any key to continue..."' | redraw!
nnoremap <leader>r :w<CR>:Run6code<CR>:e!<CR>

command -nargs=0 GitAddPatch execute 'silent !git add --intent-to-add -- % && git add --patch -- %' | redraw!
nnoremap <leader>a :w<CR>:GitAddPatch<CR>
```
