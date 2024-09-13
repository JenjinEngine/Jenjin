#!/usr/bin/env bash

mkdir jenpak-out

set -e

cmake --build ./build
cp ./build/test/test ./jenpak-out/build -u
cp ./engine/shaders/ ./jenpak-out/engine -r
cp ./test/assets/ ./jenpak-out/test -r
