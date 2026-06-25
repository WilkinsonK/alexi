#!/usr/bin/env bash

set -eu -o pipefail

if [[ $# -lt 1 ]]; then
    echo "error: expected argument <MODNAME>" >&2
    exit 1
fi

MODNAME=$1
MODROOT="alexi/${MODNAME}"
mkdir -p ${MODROOT}/include/alexi
touch ${MODROOT}/${MODNAME}.cpp
