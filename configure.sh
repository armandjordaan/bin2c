#!/bin/bash

cmake . -DCMAKE_BUILD_TYPE="Debug" -B ./build/linux/debug
cmake . -DCMAKE_BUILD_TYPE="Release" -B ./build/linux/release

