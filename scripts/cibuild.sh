#!/usr/bin/env bash

PLATFORM=${PLATFORM:-linux}
ARCH=${ARCH:-arm64}
TOOLCHAIN=${TOOLCHAIN:-@zig}

if [[ $PLATFORM = "macosx" ]]; then
    ARCHIVE_NAME=alexi-${PLATFORM}-${ARCH}
    INSTALL_NAME=staging_macos
else
    ARCHIVE_NAME=alexi-${PLATFORM}-${ARCH}
    INSTALL_NAME=staging_${PLATFORM}_${ARCH}
fi

if [[ $PLATFORM = "linux" ]]; then
    xmake f -m release -p ${PLATFORM} -a ${ARCH} --toolchain=${TOOLCHAIN}
fi

if [[ $PLATFORM = "macosx" ]]; then
    xmake f -m release -p ${PLATFORM} -a ${ARCH}
fi

if [[ $PLATFORM = "windows" ]]; then
    xmake f -m release -p ${PLATFORM} -a ${ARCH} --toolchain=${TOOLCHAIN} --sdk=$GITHUB_WORKSPACE/opt/llvm-mingw
fi

xmake
xmake install -o ${INSTALL_NAME}

if [[ $PLATFORM = "windows" ]]; then
    cd ${INSTALL_NAME} && zip -r ../release_packages/${ARCHIVE_NAME}.zip . && cd ..
else
    tar -czvf release_packages/${ARCHIVE_NAME}.tar.gz -C ${INSTALL_NAME} .
fi
