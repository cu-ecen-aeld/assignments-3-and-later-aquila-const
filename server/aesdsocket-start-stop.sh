#!/bin/sh

case "$1" in
        start)
            echo "Starting aesdsocket server"
            start-stop-daemon -S -n aesdsocket -a /usr/bin/aesdsocket -- -d
            ;;
        stop)
            echo "Stopping aesdsocket server"
            start-stop-daemon -K -n aesdsocket
            ;;
        restart)
            stop
            start
            ;;
        *)
            echo "Usage: $0 {start|stop}"
            exit 1
esac
exit 0