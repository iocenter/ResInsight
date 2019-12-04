#!/bin/bash
#cmake_ri-io_ninja.sh
PROJ_DIR=/home/bellout/git/IOC/ResInsight-io
cd ${PROJ_DIR}/ninja-build-debug

cmake -DCMAKE_BUILD_TYPE=Debug -GNinja ${PROJ_DIR}