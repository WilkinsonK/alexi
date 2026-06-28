#!/usr/bin/env bash
# Refresh compile commands by removing the old artifact and
# rebuilding the project.
set -eu -o pipefail

rm -rf ./build
xmake clean --all && xmake f --examples=y --testing=y -y -m debug && rm ./compile_commands.json && xmake
