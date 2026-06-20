#!/usr/bin/env bash
# Refresh compile commands by removing the old artifact and
# rebuilding the project.
set -eu -o pipefail

xmake clean --all && rm ./compile_commands.json && xmake
