#!/bin/bash -e

if [[ -z "$SPACETIME_PATH" ]]; then
    export SPACETIME_PATH="$( cd "$(dirname "${BASH_SOURCE[0]}")" ; cd .. ; pwd -P )"
fi

if [[ $PATH != *"$SPACETIME_PATH"* ]]; then
    export PATH="$SPACETIME_PATH:$PATH"
fi

# Bash completions

complete -o bashdefault -o default -W "$( cd "$SPACETIME_PATH/bin" ; ls )" st
