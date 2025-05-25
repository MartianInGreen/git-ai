#!/bin/bash

# Remove existing build directory
rm -rf build CMakeCache.txt CMakeFiles/ cmake_install.cmake Makefile
rm -rf gia.deb

mkdir -p build

# Build the Debian package
docker build -t gia-deb .
docker run -v $(pwd)/build:/output gia-deb bash -c "cp ../*.deb /output/"