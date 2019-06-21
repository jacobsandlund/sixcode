#!/bin/sh

if [ "x$1" == 'xhelp' ]; then
    echo 'Usage: development/setup.sh           One time setup'
    exit 0
fi

subsystems='os gpu engine'

case "$OSTYPE" in
darwin*)
    for subsystem in $subsystems; do
        echo sudo log config --mode "persist:debug" --subsystem "computer.spacetime.$subsystem"
        sudo log config --mode "persist:debug" --subsystem "computer.spacetime.$subsystem"
    done

    for subsystem in $subsystems; do
        echo sudo log config --mode "level:debug" --subsystem "computer.spacetime-test.$subsystem"
        sudo log config --mode "level:debug" --subsystem "computer.spacetime-test.$subsystem"
    done
    ;;
esac
