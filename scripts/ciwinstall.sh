#!/usr/bin/env bash

curl -L -o llvm-mingw.tar.xz https://github.com/mstorsjo/llvm-mingw/releases/download/20260616/llvm-mingw-20260616-ucrt-ubuntu-22.04-x86_64.tar.xz
mkdir -p opt/llvm-mingw
tar -xf llvm-mingw.tar.xz -C opt/llvm-mingw --strip-components=1
echo "$GITHUB_WORKSPACE/opt/llvm-mingw/bin"
