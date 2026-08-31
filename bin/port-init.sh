#!/bin/sh

# port-init.sh 

main() {
    if [ ! -n "${PORT}" ]; then
        echo "Error: PORT env var not found: ${PORT}"
        exit 1
    fi

    if [ ! -n "${BAUD}" ]; then
        echo "Error: BAUD env var not found: ${BAUD}"
        exit 1
    fi

    if [ ! -c "${PORT}" ]; then
        echo "Error: PORT device not found: ${PORT}"
        exit 1
    fi

    OS_NAME=$(uname -s)

    case "${OS_NAME}" in
        Linux)
            doas stty -F ${PORT} ${BAUD} raw -echo -hupcl
            ;;
        *)
            echo "Unknown OS: ${OS_NAME}"
            exit 1
            ;;
    esac
}

main
