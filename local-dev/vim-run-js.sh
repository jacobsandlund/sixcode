#!/bin/bash
./node $1 && git --no-pager diff --exit-code $1 || read -n1 -r -p 'Press any key to continue...'
echo ''
