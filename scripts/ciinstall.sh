#!/usr/bin/env bash

PLATFORM=${PLATFORM:-"linux"}
TARGET=${TARGET:-$PLATFORM}

# Install Xmake binary.
curl -fsSL https://github.com/xmake-io/xmake/releases/download/v${XMAKE_VERSION}/xmake-bundle-v${XMAKE_VERSION}.${PLATFORM}.x86_64 -o xmake
mkdir -p $HOME/.local/bin
chmod +x xmake
mv xmake $HOME/.local/bin/
echo "$HOME/.local/bin" >> $GITHUB_PATH

if [[ $TARGET = "windows" ]]; then
    # Install LLVM-MinGW
    curl -L -o llvm-mingw.tar.xz https://github.com/mstorsjo/llvm-mingw/releases/download/20260616/llvm-mingw-20260616-ucrt-ubuntu-22.04-x86_64.tar.xz
    mkdir -p opt/llvm-mingw
    tar -xf llvm-mingw.tar.xz -C opt/llvm-mingw --strip-components=1
    echo "$GITHUB_WORKSPACE/opt/llvm-mingw/bin" >> $GITHUB_PATH
fi

if [[ $TARGET = "macos" ]]; then
    # Install LLVM for macOS
    curl -L -o llvm.tar.xz https://github.com/llvm/llvm-project/releases/download/llvmorg-${LLVM_VERSION}/LLVM-${LLVM_VERSION}-macOS-ARM64.tar.xz
    mkdir -p opt/llvm
    tar -xf llvm.tar.xz -C opt/llvm --strip-components=1
    echo "$GITHUB_WORKSPACE/opt/llvm/bin" >> $GITHUB_PATH
fi
