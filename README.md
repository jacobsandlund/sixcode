# spacetime

### Testing

To run all tests, use `bin/test`. All lines starting with `//=> ` are added automatically when the file runs (e.g. `bin/test view-test.c`). Broken tests will result in a `git diff`. If the changed output is desired, simply `git add` the diff.

If you are using Vim, the following `.vimrc` lines add a keyboard shortcut (`<leader>r`) to run the test file and reload any changes, as well as a shortcut to `git add --patch` (`<leader>a`) a file.

``` vim
" spacetime configs:
command -nargs=0 RunSpacetime execute 'silent !$SPACETIME_PATH/bin/test --no-diff % || read -n1 -r -p "=============== Press any key to continue..."' | redraw!
nnoremap <leader>r :w<CR>:RunSpacetime<CR>:e!<CR>

command -nargs=0 GitAddPatch execute 'silent !git add --intent-to-add -- % && git add --patch -- %' | redraw!
nnoremap <leader>a :w<CR>:GitAddPatch<CR>
```
