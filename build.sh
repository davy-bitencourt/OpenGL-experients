#!/usr/bin/env bash

BUILD_PATH="build"

if [ ! -d "$BUILD_PATH" ]; then 
    echo -e "[ CONSTRUINDO CAMINHO DO PROGRAMA ]"
    cmake -S . -B "$BUILD_PATH"
fi

echo -e "[ BUILDANDO PROGRAMA ]"
cmake --build "$BUILD_PATH"

echo -e "[ EXECUTANDO PROGRAMA ]"
./build/programa