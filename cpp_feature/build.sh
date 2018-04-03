#!/usr/bin/env bash
#
# Build the project.
#
# usage: ./build.sh <BUILD_TYPE> [<TARGET>] [<CMAKE_ARGS>...]
#
# <BUILD_TYPE> must be one of [debug, release, doc, clean].
#
# <TARGET> can be [test, notest]. Not important for doc or clean.
#
# <CMAKE_ARGS> are arguments to forward to cmake. You can use any cmake argument
# here such as -DCMAKE_CXX_COMPILER=g++-5.
#
# example usages:
# 
# 1. Build only the library
#
#        ./build.sh release notest
#
# 2. Build tests
#
#        ./build.sh release test
#
# 3. Build doxygen documentation
#
#        ./build.sh doc

# Read arguments
build=$1
shift
target=$1
shift

# Set build flag
if [[ ${build} == "debug" ]]; then
	build_flag="-DCMAKE_BUILD_TYPE=Debug"
elif [[ ${build} == "release" ]]; then
	build_flag="-DCMAKE_BUILD_TYPE=Release"
elif [[ ${build} == "doc" ]]; then
	mkdir -p doc
	doxygen Doxyfile
	exit
elif [[ ${build} == "clean" ]]; then
	rm -rf build ||:
	rm -rf doc ||:
	rm feature 2> /dev/null ||:
	exit
else
	echo "Unknown build type: Use one of debug, release, clean"
	exit
fi

# Set target flag
if [[ ${target} == "test" ]]; then
	target_flag="-DBUILD_TESTING=ON"
elif [[ ${target} == "notest" ]]; then
	target_flag="-DBUILD_TESTING=OFF"
else
	echo "Unknown target: Use test or notest"
	exit
fi

# Build
mkdir -p build
cd build
cmake ${build_flag} ${target_flag} $* ..
make -j6

# Test
if [[ ${target} == "test" ]]; then
	./tests
fi
