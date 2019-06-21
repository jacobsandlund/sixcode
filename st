#!/bin/bash -e

if [[ $# -eq 0 ]]; then
    command='help'
else
    command=$1
    shift
fi

command="$SPACETIME_PATH/bin/$command"
$command "$@"
