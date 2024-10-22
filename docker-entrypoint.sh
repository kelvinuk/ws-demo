#!/bin/sh
set -e

echo "Container's IP address: `awk 'END{print $1}' /etc/hosts`"

if [ "$1" = 'server' ]; then
    sh /app/server/$@ -bi
else
    if [ "$1" = 'client' ]; then
        sh /app/client/$@
    else
        exec "$@"
    fi
fi