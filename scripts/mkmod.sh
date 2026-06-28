#!/usr/bin/env bash
# Creates a new submodule for Alexi. This includes creating
# the subdirectories and initializing an empty source file.

set -eu -o pipefail

if [[ $# -lt 1 ]]; then
    echo "error: expected argument <MODNAME>" >&2
    exit 1
fi

MODNAME=$1
MODROOT="alexi/${MODNAME}"
mkdir -p ${MODROOT}/include/alexi ${MODROOT}/src
touch ${MODROOT}/src/${MODNAME}.cpp
