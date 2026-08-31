#!/usr/bin/env bash

# select.sh 

declare -A boards=(
    [uno]=arduino_uno_r3
    [mega]=arduino_mega_2560
    [every]=arduino_nano_every
)

if [ "$#" -eq 0 ]; then
    for board in "${!boards[@]}"; do
        echo "${board}"
    done
else
    board=$1
    if [ "${BASH_SOURCE[0]}" != "${0}" ]; then
        if [ -v boards[${board}] ]; then
            export "BOARD=${boards[${board}]}"
            echo "BOARD=${BOARD}"
        else
            echo "ERROR: unknown board: ${board}"
        fi
    else
        echo "ERROR: run using source command"
    fi
fi
