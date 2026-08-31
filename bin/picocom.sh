#!/bin/sh

# Linux:   addgroup <username> dialout

# To exit picocom: Ctrl-A Ctrl-Q

OS_NAME=$(uname -s)

case "${OS_NAME}" in
    Linux)
        SERIAL_PORT=/dev/ttyACM0
        ;;
    *)
        echo "Unknown OS: ${OS_NAME}"
        exit 1
        ;;
esac

if [ ! -e ${SERIAL_PORT} ]; then
    echo "serial device not found: ${SERIAL_PORT}"
    echo "--------------------------------------"
    echo "hint: capture USB device in VirtualBox"
    echo "--------------------------------------"
    exit 1
fi

picocom -q -b 115200 ${SERIAL_PORT}
