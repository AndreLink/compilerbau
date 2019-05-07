#!/bin/bash

# Install build toolchain if any argument is given
if [ $# != 0 ]; then
	sudo apt-get install build-essential gdb cmake ninja-build ccache doxygen flex bison uthash-dev -y
fi

# build dhbwcc
mkdir -p bin && cd bin && cmake .. -G Ninja && cmake --build . --target all --config Debug
